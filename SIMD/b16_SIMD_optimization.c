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

int m_mul(int32_t a, int32_t b) 
{
    int32_t r = 0;
    a_h = (a & 0xFFFF0000) << 1; 	/* to counter last right shift */
    a_l = (a & 0xFFFF)  << 1; 		/* to counter last right shift */
    b_h = b & 0xFFFF0000; 			/* to counter last right shift */
    b_l = b & 0xFFFF; 				/* to counter last right shift */
    while(b_h != 0 || b_l != 0) 
	{
        if((b_h & 1) != 0) r = r + a_h;
        if((b_l & 1) != 0) r = r + a_l;
        b_h = b_h >> 1;
        b_l = b_l >> 1;
        r = (r>>1) & 0xFFFF7FFF;
    }
    return r;
}

unsigned int muti_data_mul(unsigned int a, unsigned int b)
{
	unsigned int MD1 = a;
	unsigned int MD2 = b;
	unsigned int falg = 0;
		//	            (all is 1)    (all is 0)    (all is 0)             (all is 1)    (all is 0)    (all is 0)
		// 0x(s1_h_flag)(e1_h_=1_flag)(e1_h_=0_flag)(m1_h_flag)_(s1_l_flag)(e1_l_=1_flag)(e1_l_=0_flag)(m1_l_flag)_		
		//   (s2_h_flag)(e2_h_=1_flag)(e2_h_=0_flag)(m2_h_flag)_(s2_l_flag)(e2_l_=1_flag)(e2_l_=0_flag)(m2_l_flag)
	     
	/* define sign (optimization) */
	int32_t s_MD1 = MD1 & 0X80008000;
	int32_t s_MD2 = MD2 & 0X80008000;
    int32_t sr;
    /* define mantissa (optimization) */
    int32_t m_MD1 = (MD1 & 0x7F007F) | 0x800080;
    int32_t m_MD2 = (MD2 & 0x7F007F) | 0x800080;
    int32_t mr;
    /* define exponent (optimization) */
    int32_t e_MD1 = MD1 & 0x7F807F80;
    int32_t e_MD2 = MD2 & 0x7F807F80;
    int32_t er;
	/* define result (optimization) */
    unsigned int result;
    
    /*special values (optimization)*/
    /*** h_result = f_nan_h ***/
    if(e_MD1==0x7F807F80) falg = falg | 0x4400;
    else if(e_MD1==0) falg = falg | 0x2200;
    else if((e_MD1 & 0x7F800000)==0x7F800000) falg = falg | 0x4200;
    else if((e_MD1 & 0x7F80)==0x7F80) falg = falg | 0x2400;
    
    if(e_MD2==0x7F807F80) falg = falg | 0x44;
    else if(e_MD2==0) falg = falg | 0x22;
    else if((e_MD2 & 0x7F800000)==0x7F800000) falg = falg | 0x4200;
    else if((e_MD2 & 0x7F80)==0x7F80) falg = falg | 0x2400;
    
	if((m_MD1 & 0x7F0000)==0) falg = falg | 0x1000;
	if((m_MD1 & 0x7F)==0) falg = falg | 0x100;
    if((m_MD2 & 0x7F0000)==0) falg = falg | 0x10;
	if((m_MD2 & 0x7F)==0) falg = falg | 0x1;   
    
/* 還沒完成 
    /*calculate*/
    if(h_falg==0 && l_falg==0)
	{
	    /* multiplication */
	    sr = s_MD1 ^ s_MD2;

	    unsigned int mrtmp = m_mul(m_MD1, m_MD2);
		int32_t ertmp_h = ((m_MD1 & 0xFFFF0000)>>16) + ((m_MD2 & 0xFFFF0000)>>16) - 127;
		int32_t ertmp_l = (m_MD1 & 0xFFFF) + (m_MD1 & 0xFFFF) - 127;
	    
	    /* realign mantissa */
	    int32_t mshift_h = (mrtmp >> 8) & 1;
	    int32_t mshift_l = (mrtmp >> 24) & 1;
	    mr = mrtmp >> mshift_h;
	    ertmp_h = ertmp_h + mshift_h;
	    ertmp_l = ertmp_l + mshift_l;
	    er = (ertmp_h << 23) + ertmp_l;
	    
	    /* overflow and underflow */
	    /**(high)**/
	    if(er_h <= 0 && h_falg==0) 
		{
			h_result = 0 | (s_MD1_h ^ s_MD2_h) << 15;					// f_zero_h
			h_falg = 1;
		}
	    if(er_h >= 0xff && h_falg==0) 
		{
			h_result = 0x7F80 | (s_MD1_h ^ s_MD2_h) << 15;				// f_inf_h
			h_falg = 1;	
		}
	    /**(low)**/
	    if(er_l <= 0 && l_falg==0) 
		{
			l_result = 0 | (s_MD1_l ^ s_MD2_l) << 15;					// f_zero_h
			l_falg = 1;
		}
	    if(er_l >= 0xff && l_falg==0) 
		{
			l_result = 0x7F80 | (s_MD1_l ^ s_MD2_l) << 15;				// f_inf_h
			l_falg = 1;	
		}
	}
    
    /* result */
    if(h_falg==0) h_result = (sr_h << 15) | ((er_h & 0xFF) << 7) | (mr_h & 0x7F);
	if(l_falg==0) l_result = (sr_l << 15) | ((er_l & 0xFF) << 7) | (mr_l & 0x7F);	
	result = (h_result << 16) + l_result;
    return result;
*/ 
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
		
	MD_h_test = a1 * a3;
	MD_l_test = a2 * a4;	
	int *p1 = (int *) &MD_h_test;
	int *p2 = (int *) &MD_l_test;
	unsigned int h_b16_HEX = *p1;
	unsigned int l_b16_HEX = *p2;
	printf("MD_h_r = 0x%08x \n", h_b16_HEX);
	printf("MD_l_r = 0x%08x \n", l_b16_HEX);
	
	MD_r = muti_data_mul(MD1, MD2);
	printf("MD_r = 0x%08x \n", MD_r);
}
