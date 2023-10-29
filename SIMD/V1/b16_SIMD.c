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
    a = a << 1; /* to counter last right shift */
    while(b != 0) {
        if((b & 1) != 0) {
            r = r + a;
        }
        b = b >> 1;
        r = r >> 1;
    }
    return r;
}

unsigned int muti_data_mul(unsigned int a, unsigned int b)
{
	unsigned int MD1 = a;
	unsigned int MD2 = b;
	unsigned int h_falg = 0;
	unsigned int l_falg = 0;
	/* define sign */
	int32_t s_MD1_h = MD1 >> 31;
	int32_t s_MD1_l = (MD1 & 0x8000) >> 15;
	int32_t s_MD2_h = MD2 >> 31;
	int32_t s_MD2_l = (MD1 & 0x8000) >> 15;
    int32_t sr_h;
    int32_t sr_l;
    /* define mantissa */
    int32_t m_MD1_h = ((MD1 & 0x7F0000) | 0x800000)>>16;
    int32_t m_MD1_l = (MD1 & 0x7F) | 0x80;
    int32_t m_MD2_h = ((MD2 & 0x7F0000) | 0x800000)>>16;
    int32_t m_MD2_l = (MD2 & 0x7F) | 0x80;
    int32_t mr_h;
    int32_t mr_l;
    /* define exponent */
    int32_t e_MD1_h = ((MD1 >> 23) & 0xFF);
    int32_t e_MD1_l = ((MD1 >> 7) & 0xFF);
    int32_t e_MD2_h = ((MD2 >> 23) & 0xFF);
    int32_t e_MD2_l = ((MD2 >> 7) & 0xFF);
    int32_t er_h;
	int32_t er_l;
	/* define result */
	unsigned int h_result, l_result;
    unsigned int result;
    
    /*special values (high)*/
    if(e_MD1_h == 0xFF && m_MD1_h != 0x80 && h_falg == 0) 
	{
        h_result = 0x7F81;			// f_nan_h
        h_falg = 1;
    }
    if(e_MD2_h == 0xFF && m_MD2_h != 0x80 && h_falg == 0) 
	{
        h_result = 0x7F81;			// f_nan_h
        h_falg = 1;
    }
    if(e_MD1_h == 0xFF && m_MD1_h == 0x80 && h_falg==0) 
	{
        if(e_MD2_h == 0) 
		{
            h_result = 0x7F81;		// f_nan_h
            h_falg = 1;
        }
        else 
		{
			h_result = 0x7F80 | (s_MD1_h ^ s_MD2_h) << 15;				// f_inf_h
            h_falg = 1;
        }
    }
    if(e_MD2_h == 0xFF && m_MD2_h == 0x80 && h_falg==0) 
	{
        if(e_MD1_h == 0) 
		{
            h_result = 0x7F81;		// f_nan_h
            h_falg = 1;
        }
        else 
		{
			h_result = 0x7F80 | (s_MD1_h ^ s_MD2_h) << 15;				// f_inf_h
            h_falg = 1;
        }
    }
    if((e_MD1_h == 0 || e_MD2_h == 0) && h_falg==0) 
	{
        h_result = 0 | (s_MD1_h ^ s_MD2_h) << 15;						// f_zero_h
        h_falg = 1;
    }
    
    /*special values (low)*/
    if(e_MD1_l == 0xFF && m_MD1_l != 0x80 && l_falg == 0) 
	{
        l_result = 0x7F81;			// l_nan_h
        l_falg = 1;
    }
    if(e_MD2_l == 0xFF && m_MD2_l != 0x80 && l_falg == 0) 
	{
        l_result = 0x7F81;			// l_nan_h
        l_falg = 1;
    }
    if(e_MD1_l == 0xFF && m_MD1_l == 0x80 && l_falg==0) 
	{
        if(e_MD2_l == 0) 
		{
            l_result = 0x7F81;		// l_nan_h
            l_falg = 1;
        }
        else 
		{
			l_result = 0x7F80 | (s_MD1_l ^ s_MD2_l) << 15;				// l_inf_h
            l_falg = 1;
        }
    }
    if(e_MD2_l == 0xFF && m_MD2_l == 0x80 && l_falg==0) 
	{
        if(e_MD1_l == 0) 
		{
            l_result = 0x7F81;		// l_nan_h
            l_falg = 1;
        }
        else 
		{
			l_result = 0x7F80 | (s_MD1_h ^ s_MD2_h) << 15;				// l_inf_h
            l_falg = 1;
        }
    }
    if((e_MD1_l == 0 || e_MD2_l == 0) && l_falg==0) 
	{
        l_result = 0 | (s_MD1_l ^ s_MD2_l) << 15;						// l_zero_h
        l_falg = 1;
    }
    
    /*calculate*/
    if(h_falg==0 && l_falg==0)
	{
	    /* multiplication */
	    /**(high)**/
	    sr_h = s_MD1_h ^ s_MD2_h;
	    int32_t mrtmp_h = m_mul(m_MD1_h, m_MD2_h);
	    int32_t ertmp_h = e_MD1_h + e_MD2_h - 127;
		/**(low)**/
	    sr_l = s_MD1_l ^ s_MD2_l;
	    int32_t mrtmp_l = m_mul(m_MD1_l, m_MD2_l);
	    int32_t ertmp_l = e_MD1_l + e_MD2_l - 127;
	    
	    /* realign mantissa */
	    /**(high)**/
	    int32_t mshift_h = (mrtmp_h >> 8) & 1;
	    mr_h = mrtmp_h >> mshift_h;
	    er_h = ertmp_h + mshift_h;
	    /**(low)**/
	    int32_t mshift_l = (mrtmp_l >> 8) & 1;
	    mr_l = mrtmp_l >> mshift_l;
	    er_l = ertmp_l + mshift_l;
	    
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
