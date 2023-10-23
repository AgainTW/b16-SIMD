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

unsigned int data_mul(unsigned int a, unsigned int b)
{
    unsigned int MD1 = a;
    unsigned int MD2 = b;
    unsigned int h_falg = 0;
    /* define sign */
    int32_t s_MD1_h = MD1 >> 31;
    int32_t s_MD2_h = MD2 >> 31;
    int32_t sr_h;
    /* define mantissa */
    int32_t m_MD1_h = ((MD1 & 0x7F0000) | 0x800000)>>16;
    int32_t m_MD2_h = ((MD2 & 0x7F0000) | 0x800000)>>16;
    int32_t mr_h;
    /* define exponent */
    int32_t e_MD1_h = ((MD1 >> 23) & 0xFF);
    int32_t e_MD2_h = ((MD2 >> 23) & 0xFF);
    int32_t er_h;
    /* define result */
    unsigned int h_result;
    
    /*special values (high)*/
    if(e_MD1_h == 0xFF && m_MD1_h != 0x80 && h_falg == 0) 
    {
        h_result = 0x7F81;          // f_nan_h
        h_falg = 1;
    }
    if(e_MD2_h == 0xFF && m_MD2_h != 0x80 && h_falg == 0) 
    {
        h_result = 0x7F81;          // f_nan_h
        h_falg = 1;
    }
    if(e_MD1_h == 0xFF && m_MD1_h == 0x80 && h_falg==0) 
    {
        if(e_MD2_h == 0) 
        {
            h_result = 0x7F81;      // f_nan_h
            h_falg = 1;
        }
        else 
        {
            h_result = 0x7F80 | (s_MD1_h ^ s_MD2_h) << 15;              // f_inf_h
            h_falg = 1;
        }
    }
    if(e_MD2_h == 0xFF && m_MD2_h == 0x80 && h_falg==0) 
    {
        if(e_MD1_h == 0) 
        {
            h_result = 0x7F81;      // f_nan_h
            h_falg = 1;
        }
        else 
        {
            h_result = 0x7F80 | (s_MD1_h ^ s_MD2_h) << 15;              // f_inf_h
            h_falg = 1;
        }
    }
    if((e_MD1_h == 0 || e_MD2_h == 0) && h_falg==0) 
    {
        h_result = 0 | (s_MD1_h ^ s_MD2_h) << 15;                       // f_zero_h
        h_falg = 1;
    }
    
    /*calculate*/
    if(h_falg==0)
    {
        /* multiplication */
        /**(high)**/
        sr_h = s_MD1_h ^ s_MD2_h;
        int32_t mrtmp_h = m_mul(m_MD1_h, m_MD2_h);
        int32_t ertmp_h = e_MD1_h + e_MD2_h - 127;
        
        /* realign mantissa */
        /**(high)**/
        int32_t mshift_h = (mrtmp_h >> 8) & 1;
        mr_h = mrtmp_h >> mshift_h;
        er_h = ertmp_h + mshift_h;
        
        /* overflow and underflow */
        /**(high)**/
        if(er_h <= 0 && h_falg==0) 
        {
            h_result = 0 | (s_MD1_h ^ s_MD2_h) << 15;                   // f_zero_h
            h_falg = 1;
        }
        if(er_h >= 0xff && h_falg==0) 
        {
            h_result = 0x7F80 | (s_MD1_h ^ s_MD2_h) << 15;              // f_inf_h
            h_falg = 1; 
        }
    }
    
    /* result */
    if(h_falg==0) h_result = (sr_h << 15) | ((er_h & 0xFF) << 7) | (mr_h & 0x7F);  
    h_result = (h_result << 16);
    return h_result;
}

void main()
{
	unsigned int MD1_h=0x3fc00000, MD2_h=0x42620000, MD1_l=0x401d0000, MD2_l=0x42c80000;
	unsigned int MD_r_h=0, MD_r_l=0;
	
	MD_r_h = data_mul(MD1_h, MD2_h);
	MD_r_l = data_mul(MD1_l, MD2_l);
	//printf("MD_r_h = 0x%08x \n", MD_r_h);
	//printf("MD_r_l = 0x%08x \n", MD_r_l);
}
