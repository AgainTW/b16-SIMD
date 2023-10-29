#include<stdio.h>
#include<stdint.h>

float fp32_to_bf16(float x)
{
	float y = x;
	int *p = (int *) &y;
	unsigned int exp = *p & 0x7F800000;
	unsigned int man = *p & 0x007FFFFF;
	if (exp == 0 && man == 0) /* zero */
		return x;
	if (exp == 0x7F800000) /* infinity or NaN */
		return x;

	/* Normalized number */
	/* round to nearest */
	float r = x;
	int *pr = (int *) &r;
	*pr &= 0xFF800000;  /* r has the same exp as x */
	r /= 0x100;
	y = x + r;

	*p &= 0xFFFF0000;

	return y;
}

int bf16_to_muti_data(float D1, float D2)
{
	float y1 = D1;
	float y2 = D2;
	int *p1 = (int *) &y1;
	int *p2 = (int *) &y2;
	unsigned int D1_b16_HEX = *p1;
	unsigned int D2_b16_HEX = *p2;
	
	unsigned int MD = 0;
	MD = D1_b16_HEX + (D2_b16_HEX>>16);
	
	return MD;
}

unsigned int m_mul(unsigned int a, unsigned int b) 
{
    unsigned int r = 0;
    unsigned int a_h, a_l, b_h, b_l;
    a_h = (a & 0xFFFF0000) << 1;    /* to counter last right shift */
    a_l = (a & 0xFFFF) << 1;       /* to counter last right shift */
    b_h = (b & 0xFFFF0000) >>16;
    b_l = b & 0xFFFF;
    while(b_h != 0 || b_l != 0) 
    {
        if((b_h & 1) != 0) r = r + a_h;
        if((b_l & 1) != 0) r = r + a_l;

        if(b_h != 0 && b_l != 0) r = (r>>1) & 0xFFFF7FFF;
        else if(b_h == 0) r = (r & 0xFFFF0000) | (r & 0xFFFE)>>1;
        else if(b_l == 0) r = (r & 0xFFFF) | (r & 0xFFFE0000)>>1;
        
        b_h = b_h >> 1;
        b_l = b_l >> 1;
    }
    return r;
}

unsigned int muti_data_mul(unsigned int a, unsigned int b)
{
	unsigned int MD1 = a;
	unsigned int MD2 = b;
	unsigned int falg = 0;
	// 0x()(result_h_0)(result_h_nan)(result_h_inf)_()(result_l_0)(result_l_nan)(result_l_inf)
	     
	/* define sign (optimization) */
	unsigned int s_MD1 = MD1 & 0X80008000;
	unsigned int s_MD2 = MD2 & 0X80008000;
    unsigned int sr;
    /* define mantissa (optimization) */
    unsigned int m_MD1 = (MD1 & 0x7F007F) | 0x800080;
    unsigned int m_MD2 = (MD2 & 0x7F007F) | 0x800080;
    unsigned int mr;
    /* define exponent (optimization) */
    int32_t e_MD1_h = (MD1 & 0x7F800000) >> 23;
    int32_t e_MD1_l = (MD1 & 0x7F80) >> 7;
    int32_t e_MD2_h = (MD2 & 0x7F800000) >> 23;
    int32_t e_MD2_l = (MD2 & 0x7F80) >> 7;
    unsigned int er;
    
    /* special values (optimization) */
    if(e_MD1_h != 0xFF && e_MD1_h != 0 && e_MD2_h != 0xFF && e_MD2_h != 0 ) falg = falg;
    else if(e_MD1_h == 0 && e_MD1_h == 0) falg = falg | 0x40;
    else if(e_MD1_h == 0xFF && (m_MD1 & 0x7F0000) != 0 ) falg = falg | 0x20;
	else if(e_MD1_h == 0xFF && (m_MD1 & 0x7F0000) == 0 && e_MD2_h == 0) falg = falg | 0x20;
	else if(e_MD1_h == 0xFF && (m_MD1 & 0x7F0000) == 0 && e_MD2_h != 0) falg = falg | 0x10;
	else if(e_MD2_h == 0xFF && (m_MD2 & 0x7F0000) != 0 ) falg = falg | 0x20;
	else if(e_MD2_h == 0xFF && (m_MD2 & 0x7F0000) == 0 && e_MD1_h == 0) falg = falg | 0x20;
	else if(e_MD2_h == 0xFF && (m_MD2 & 0x7F0000) == 0 && e_MD1_h != 0) falg = falg | 0x10;
	
	if(e_MD1_l != 0xFF && e_MD1_l != 0 && e_MD2_l != 0xFF && e_MD2_l != 0 ) falg = falg;
	else if(e_MD1_l == 0 && e_MD1_l == 0) falg = falg | 0x4;
    else if(e_MD1_l == 0xFF && (m_MD1 & 0x7F) != 0 ) falg = falg | 0x2;
	else if(e_MD1_l == 0xFF && (m_MD1 & 0x7F) == 0 && e_MD2_l == 0) falg = falg | 0x2;
	else if(e_MD1_l == 0xFF && (m_MD1 & 0x7F) == 0 && e_MD2_l != 0) falg = falg | 0x1;
	else if(e_MD2_l == 0xFF && (m_MD2 & 0x7F) != 0 ) falg = falg | 0x2;
	else if(e_MD2_l == 0xFF && (m_MD2 & 0x7F) == 0 && e_MD1_l == 0) falg = falg | 0x2;
	else if(e_MD2_l == 0xFF && (m_MD2 & 0x7F) == 0 && e_MD1_l != 0) falg = falg | 0x1;
	
	/* calculate: multiplication > sign */
	sr = s_MD1 ^ s_MD2;
	
	/* Exit subroutine in advance */
	if( ((falg & 0xF0) != 0) && ((falg & 0xF) != 0) )
	{
		switch(falg)
		{
	    	case 0x11 : return sr | 0x7F8078F0;
	    	case 0x12 : return sr | 0x7F807F81;
	    	case 0x14 : return sr | 0x7F800000;
	    	case 0x21 : return sr | 0x7F8178F0;
	    	case 0x22 : return sr | 0x7F8178F1;
	    	case 0x24 : return sr | 0x7F810000;
	    	case 0x41 : return sr | 0x7F80;
	    	case 0x42 : return sr | 0x7F81;
	    	case 0x44 : return sr;
		}
	}
    
    /* calculate: multiplication > mantissa */
	unsigned int mrtmp = m_mul(m_MD1, m_MD2);
	/* calculate: multiplication > exponent */
	int32_t ertmp_h = e_MD1_h + e_MD2_h - 127;
	int32_t ertmp_l = e_MD1_l + e_MD2_l - 127;
	/* calculate: realign mantissa */
	int32_t mshift_h = (mrtmp >> 24) & 1;
	int32_t mshift_l = (mrtmp >> 8) & 1;
	mr = ((mrtmp & 0xFFFF0000) >> mshift_h ) | ((mrtmp & 0xFFFF) >> mshift_l);
	int32_t er_h = ertmp_h + mshift_h;
	int32_t er_l = ertmp_l + mshift_l;
	er = ((er_h & 0xFF) << 23) | ((er_l & 0xFF) << 7);
	
	printf("sr = 0x%08x \n", sr);
	printf("er = 0x%08x \n", er & 0x7F807F80);
	printf("mr = 0x%08x \n", mr & 0x7F007F);
	printf("mrtmp = 0x%08x \n", mrtmp);
	/* overflow and underflow */
	if((falg & 0xF0) == 0)
	{
		if(er_h <= 0) falg = falg | 0x40;
		if(er_h >= 0xff) falg = falg | 0x10;	
	}
	if((falg & 0xF) == 0)
	{
		if(er_l <= 0) falg = falg | 0x4;
		if(er_l >= 0xff) falg = falg | 0x1;
	}	
	
	/* result */
	if((falg & 0xF0) != 0 && (falg & 0xF) != 0)	// high and low are special
	{
		switch(falg)
		{
	    	case 0x11 : return sr | 0x7F8078F0;
	    	case 0x12 : return sr | 0x7F807F81;
	    	case 0x14 : return sr | 0x7F800000;
	    	case 0x21 : return sr | 0x7F8178F0;
	    	case 0x24 : return sr | 0x7F810000;
	    	case 0x41 : return sr | 0x7F80;
	    	case 0x42 : return sr | 0x7F81;
	    	case 0x44 : return sr;
		}	
	}
	if((falg & 0xF0) != 0)							// high is special
	{
		switch(falg)
		{
	    	case 0x10 : return sr | 0x7F800000 | (er & 0x7F80) | (mr & 0x7F);
	    	case 0x20 : return sr | 0x7F810000 | (er & 0x7F80) | (mr & 0x7F);
	    	case 0x40 : return sr | (er & 0x7F80) | (mr & 0x7F);
		}	
	}
	if((falg & 0xF) != 0)							// low is special
	{
		switch(falg)
		{
	    	case 0x1 : return sr | 0x7F80 | (er & 0x7F800000) | (mr & 0x7F0000);
	    	case 0x2 : return sr | 0x7F81 | (er & 0x7F800000) | (mr & 0x7F0000);
	    	case 0x4 : return sr | (er & 0x7F800000) | (mr & 0x7F0000);
		}
	}
	if(falg==0) return sr | ((er & 0x7F807F80)) | (mr & 0x7F007F);
}

void main()
{
	float a1=1.5, a2=2.458, a3=56.4984, a4=100.01;
	float a1_to_bf16, a2_to_bf16, a3_to_bf16, a4_to_bf16;
	float MD_h_test=0, MD_l_test=0;
	unsigned int MD1=0, MD2=0, MD_r=0;
	
	a1_to_bf16 = fp32_to_bf16(a1);
	a2_to_bf16 = fp32_to_bf16(a2);
	a3_to_bf16 = fp32_to_bf16(a3);
	a4_to_bf16 = fp32_to_bf16(a4);
	
	MD1 = bf16_to_muti_data(a1_to_bf16, a2_to_bf16);
	MD2 = bf16_to_muti_data(a3_to_bf16, a4_to_bf16);

	printf("MD1 = 0x%08x \n", MD1);
	printf("MD2 = 0x%08x \n", MD2);
	printf("\n");
		
	MD_h_test = a1 * a3;
	MD_l_test = a2 * a4;	
	int *p1 = (int *) &MD_h_test;
	int *p2 = (int *) &MD_l_test;
	unsigned int h_b16_HEX = *p1;
	unsigned int l_b16_HEX = *p2;
	unsigned int result = (h_b16_HEX & 0xFFFF0000) | (l_b16_HEX & 0xFFFF0000)>>16; 
	printf("MD_h_r = 0x%08x \n", h_b16_HEX);
	printf("MD_l_r = 0x%08x \n", l_b16_HEX);
	printf("MD_result = 0x%08x \n", result);
	printf("MD_sr = 0x%08x \n", (result & 0x80008000));
	printf("MD_er = 0x%08x \n", (result & 0x7F807F80));
	printf("MD_mr = 0x%08x \n", (result & 0x7F007F));
	printf("\n");
	
	MD_r = muti_data_mul(MD1, MD2);
	printf("MD_r = 0x%08x \n", MD_r);
}
