#include <stdio.h>
#include <stdint.h>
#include "b16_M_SIMD_opt.h"

//struct _matf16_t;  
typedef struct _matf16_t matf16_t;

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
    uint32_t start;
    uint32_t end;

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

    start = get_cycles();

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
                    aidx += 2;
                    bidx += 2*o;
                }
                MD1 = a[aidx];
                MD2 = b[bidx];
                MD_r = muti_data_mul(MD1, MD2);
                subtotal = hhw_single_data_add(subtotal, MD_r);
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

    end = get_cycles();
    printf("Mat Elapse cycle: %d\n",end-start);

    return ret;
}
