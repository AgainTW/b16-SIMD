#include<stdio.h>
#include<stdint.h>

int m_mul(int32_t a, int32_t b) 
{
    int32_t r = 0;
    a = a << 1; /* to counter last right shift */
    while(b != 0) 
	{
        if((b & 1) != 0) r = r + a;
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
	unsigned int MD1=0x3fc0401d, MD2=0x426242c8, MD_r=0;
	
	MD_r = muti_data_mul(MD1, MD2);
	//printf("MD_r = 0x%08x \n", MD_r);
}
