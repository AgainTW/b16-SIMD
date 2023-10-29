#include<stdio.h>
#include<stdint.h>

typedef struct 
{
    int32_t row;
    int32_t col;
    unsigned int *data;
} matf16_t;

uint32_t highestbit(uint32_t x) {
    x |= (x >> 1);
    x |= (x >> 2);
    x |= (x >> 4);
    x |= (x >> 8);
    /* count ones (population count) */
    x -= ((x >> 1) & 0x5555);
    x = ((x >> 2) & 0x3333) + (x & 0x3333);
    x = ((x >> 4) + x) & 0x0f0f;
    x += (x >> 8);
    return x & 0x7f;
}

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
    
    /*special values (low)*/
    if(e_MD1_l == 0xFF && m_MD1_l != 0x80 && l_falg == 0) 
    {
        l_result = 0x7F81;          // l_nan_h
        l_falg = 1;
    }
    if(e_MD2_l == 0xFF && m_MD2_l != 0x80 && l_falg == 0) 
    {
        l_result = 0x7F81;          // l_nan_h
        l_falg = 1;
    }
    if(e_MD1_l == 0xFF && m_MD1_l == 0x80 && l_falg==0) 
    {
        if(e_MD2_l == 0) 
        {
            l_result = 0x7F81;      // l_nan_h
            l_falg = 1;
        }
        else 
        {
            l_result = 0x7F80 | (s_MD1_l ^ s_MD2_l) << 15;              // l_inf_h
            l_falg = 1;
        }
    }
    if(e_MD2_l == 0xFF && m_MD2_l == 0x80 && l_falg==0) 
    {
        if(e_MD1_l == 0) 
        {
            l_result = 0x7F81;      // l_nan_h
            l_falg = 1;
        }
        else 
        {
            l_result = 0x7F80 | (s_MD1_h ^ s_MD2_h) << 15;              // l_inf_h
            l_falg = 1;
        }
    }
    if((e_MD1_l == 0 || e_MD2_l == 0) && l_falg==0) 
    {
        l_result = 0 | (s_MD1_l ^ s_MD2_l) << 15;                       // l_zero_h
        l_falg = 1;
    }
    
    /*calculate*/
    if(h_falg==0)
    {
        /* multiplication */
        sr_h = s_MD1_h ^ s_MD2_h;
        int32_t mrtmp_h = m_mul(m_MD1_h, m_MD2_h);
        int32_t ertmp_h = e_MD1_h + e_MD2_h - 127;
        
        /* realign mantissa */
        int32_t mshift_h = (mrtmp_h >> 8) & 1;
        mr_h = mrtmp_h >> mshift_h;
        er_h = ertmp_h + mshift_h;
        
        /* overflow and underflow */
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

    if(l_falg==0)
    {
        /* multiplication */
        sr_l = s_MD1_l ^ s_MD2_l;
        int32_t mrtmp_l = m_mul(m_MD1_l, m_MD2_l);
        int32_t ertmp_l = e_MD1_l + e_MD2_l - 127;
        
        /* realign mantissa */
        int32_t mshift_l = (mrtmp_l >> 8) & 1;
        mr_l = mrtmp_l >> mshift_l;
        er_l = ertmp_l + mshift_l;
        
        /* overflow and underflow */
        if(er_l <= 0 && l_falg==0) 
        {
            l_result = 0 | (s_MD1_l ^ s_MD2_l) << 15;                   // f_zero_h
            l_falg = 1;
        }
        if(er_l >= 0xff && l_falg==0) 
        {
            l_result = 0x7F80 | (s_MD1_l ^ s_MD2_l) << 15;              // f_inf_h
            l_falg = 1; 
        }
    }
    
    /* result */
    if(h_falg==0) h_result = (sr_h << 15) | ((er_h & 0xFF) << 7) | (mr_h & 0x7F);
    if(l_falg==0) l_result = (sr_l << 15) | ((er_l & 0xFF) << 7) | (mr_l & 0x7F);   
    result = (h_result << 16) + l_result;
    return result;
}

// high-half single word
unsigned int hhw_single_data_add(unsigned int a, unsigned int b)
{
    unsigned int MD1 = a;
    unsigned int MD2 = b;
    /* define sign */
    int32_t s_MD1 = MD1 >> 31;
    int32_t s_MD2 = MD2 >> 31;
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
    unsigned int result;
    /* special values */
    if(e_MD1_h == 0xFF) {
        if(m_MD1_h != 0x80 || (s_MD1 ^ s_MD2) == 0x1) {
            h_result = 0x7f810000;                          // f_nan_h
            return h_result;
        }
        else {
            h_result = 0x7f800000 | s_MD1 << 31;             // f_inf_h
            return h_result;
        }
    }
    if(e_MD2_h == 0xFF) {
        if(m_MD2_h != 0x80 || (s_MD1 ^ s_MD2) == 0x1) {
            h_result = 0x7f810000;                          // f_nan_h
            return h_result;
        }
        else {
            h_result = 0x7f800000 | s_MD1 << 31;             // f_inf_h
            return h_result;
        }
    }
    /* exponent align */
    if(e_MD1_h >= e_MD2_h) {
        if(e_MD1_h - e_MD2_h <= 8) {
            m_MD2_h = m_MD2_h >> (e_MD1_h - e_MD2_h);
        }
        else {
            m_MD2_h = 0;
        }
        er_h = e_MD1_h;
    }
    else {
        if(e_MD2_h - e_MD1_h <= 8) {
            m_MD1_h = m_MD1_h >> (e_MD2_h - e_MD1_h);
        }
        else {
            m_MD1_h = 0;
        }
        er_h = e_MD2_h;
    }
    /* addition or substraction */
    sr_h = s_MD1;
    int32_t madd;
    if((s_MD1 ^ s_MD2) == 0) {
        madd = m_MD1_h + m_MD2_h;
    }
    else {
        madd = m_MD1_h - m_MD2_h;
        if(madd < 0) {
            sr_h ^= 1;
            madd = 0 - madd;
        }
    }
    /* realign mantissa */
    int32_t digits = highestbit(madd);
    if(digits == 9) {
        mr_h = (madd + 1) >> 1;
    }
    else {
        mr_h = madd << (8 - digits);
    }
    er_h = er_h - (8 - digits);
    /* overflow and underflow */
    if(er_h < 0) {
        h_result = sr_h << 31;                          // f_zero_h
        return h_result;
    }
    if(er_h >= 0xff) {
        h_result = 0x7f800000 | s_MD1 << 31;             // f_inf_h
        return h_result;
    }
    /* result */
    h_result = (sr_h << 15) | ((er_h & 0xFF) << 7) | (mr_h & 0x7F);
    result = h_result << 16;
    return result;
}

matf16_t *matmul(matf16_t *first, matf16_t *second, matf16_t *retmat, unsigned int *retdata) 
{
    /* (m * n) * (n * o) -> (m * o) */
    int32_t m = first->row;
    int32_t n = first->col;
    int32_t o = second->col;
    unsigned int MD1=0, MD2=0, MD_r=0;

    if(n != second->row) {
        return NULL;
    }
    matf16_t *ret = retmat; /* replace malloc struct */
    ret->row = m;
    ret->col = o;
    ret->data = retdata; /* replace malloc array */
    unsigned int *a = first->data;
    unsigned int *b = second->data;
    unsigned int *c = ret->data;
    unsigned int subtotal, temp_subtotal;
    int32_t arow = 0;
    int32_t aidx;
    int32_t bidx;
    int32_t cidx = 0;
    int32_t i=0,j=0,k=0;
    for(i = 0; i < m; i ++) {
        for(j = 0; j < o; j ++) {
            subtotal = 0;
            temp_subtotal = 0;
            aidx = arow;
            bidx = j;

            if(n%2==1){
                for(k = 0; k < (n-1); k+=2 ) {
                    MD1 = a[aidx+1] + (a[aidx]>>16);
                    MD2 = b[bidx+o] + (b[bidx]>>16);
                    MD_r = muti_data_mul(MD1, MD2);
                    temp_subtotal = hhw_single_data_add(MD_r,(MD_r<<16));
                    subtotal = hhw_single_data_add(subtotal, temp_subtotal);
                    //printf("test MD1: 0x%08x 0x%08x 0x%08x\n",MD1, a[aidx+1], a[aidx]);
                    //printf("test MD2: 0x%08x 0x%08x 0x%08x\n",MD2, b[bidx+o], b[bidx]);
                    //printf("test MD_r: 0x%08x 0x%08x \n",MD_r, (MD_r<<16));
                    //printf("test total: 0x%08x 0x%08x \n",subtotal, temp_subtotal);
                    aidx += 2;
                    bidx += 2*o;
                }
                MD1 = a[aidx];
                MD2 = b[bidx];
                MD_r = muti_data_mul(MD1, MD2);
                subtotal = hhw_single_data_add(subtotal, MD_r);
                //printf("test MD1: 0x%08x 0x%08x\n",MD1, a[aidx]);
                //printf("test MD2: 0x%08x 0x%08x\n",MD2, b[bidx]);
                //printf("test MD_r: 0x%08x 0x%08x \n",MD_r, (MD_r<<16));
                //printf("test total: 0x%08x 0x%08x \n\n",subtotal, MD_r);
            }
            else{
                for(k = 0; k < n; k+=2 ) {
                    MD1 = a[aidx+1] + (a[aidx]>>16);
                    MD2 = b[bidx+o] + (b[bidx]>>16);
                    MD_r = muti_data_mul(MD1, MD2);
                    temp_subtotal = hhw_single_data_add(MD_r, (MD_r<<16));
                    subtotal = hhw_single_data_add(subtotal, temp_subtotal);
                    aidx += 2;
                    bidx += 2*o;
                }
            }

            c[cidx] = subtotal;
            cidx += 1;
        }
        arow += n;
    }
    return ret;
}
