#include <stdio.h>
#include <stdint.h>

// 已更改
typedef struct {
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

/* float32 mantissa multiply */
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

/* float32 multiply */
unsigned int fmul_b16(unsigned int a, unsigned int b){
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

/* float32 add */
unsigned int fadd_b16(unsigned int a, unsigned int b) {
    unsigned int MD1 = a;
    unsigned int MD2 = b;
    /* define sign */
    int32_t sa_h = MD1 >> 31;
    int32_t sb_h = MD2 >> 31;
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
        if(m_MD1_h != 0x80 || (sa_h ^ sb_h) == 0x1) {
            h_result = 0x7f810000;                          // f_nan_h
            return h_result;
        }
        else {
            h_result = 0x7f800000 | sa_h << 31;             // f_inf_h
            return h_result;
        }
    }
    if(e_MD2_h == 0xFF) {
        if(m_MD2_h != 0x80 || (sa_h ^ sb_h) == 0x1) {
            h_result = 0x7f810000;                          // f_nan_h
            return h_result;
        }
        else {
            h_result = 0x7f800000 | sa_h << 31;             // f_inf_h
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
    sr_h = sa_h;
    int32_t madd;
    if((sa_h ^ sb_h) == 0) {
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
        h_result = 0x7f800000 | sa_h << 31;             // f_inf_h
        return h_result;
    }
    /* result */
    h_result = (sr_h << 15) | ((er_h & 0xFF) << 7) | (mr_h & 0x7F);
    result = h_result << 16;
    return result;
}

unsigned int retdata[9] = {0}; /* preserve space for malloc replacement */
matf16_t retmat = {0, 0, 0}; /* preserve space, not initialize */
matf16_t *matmul(matf16_t *first, matf16_t *second) {
    /* (m * n) * (n * o) -> (m * o) */
    int32_t m = first->row;
    int32_t n = first->col;
    int32_t o = second->col;
    if(n != second->row) {
        return NULL;
    }
    matf16_t *ret = &retmat; /* replace malloc struct */
    ret->row = m;
    ret->col = o;
    ret->data = retdata; /* replace malloc array */
    unsigned int *a = first->data;
    unsigned int *b = second->data;
    unsigned int *c = ret->data;
    unsigned int subtotal;
    int32_t arow = 0;
    int32_t aidx;
    int32_t bidx;
    int32_t cidx = 0;
    int32_t i=0,j=0,k=0;
    for(i = 0; i < m; i ++) {
        for(j = 0; j < o; j ++) {
            subtotal = 0;
            aidx = arow;
            bidx = j;
            for(k = 0; k < n; k ++) {
                subtotal = fadd_b16(fmul_b16(a[aidx], b[bidx]), subtotal);
                aidx += 1;
                bidx += o;
            }
            c[cidx] = subtotal;
            cidx += 1;
        }
        arow += n;
    }
    return ret;
}
unsigned int aarr[9] = {0x3f630000, 0x3e380000, 0x3d230000,
			  0x3d8f0000, 0x3f800000, 0x3e4c0000,
			  0x3e050000, 0x00000000, 0x3f330000};
unsigned int barr[9] = {0x3ca30000, 0x3f810000, 0x3e0f0000,
			  0x3dcc0000, 0x3e0f0000, 0x3f7d0000,
			  0x3f5e0000, 0x3e4c0000, 0x3db80000};
matf16_t amat = {.row = 3, .col = 3, .data = aarr};
matf16_t bmat = {.row = 3, .col = 3, .data = barr};

static inline void printstr(const char *str) {
    asm("li  a7, 4");
    asm("ecall");
}
static inline void printline() {
    asm("li  a0, 10");
    asm("li  a7, 11");
    asm("ecall");
}
static inline void printfloat(int a) {
    asm("li  a7, 2");
    asm("ecall");
}

int main() {
    /*
    answer should be
    0.0706  0.9321  0.3064
    0.2753  0.2507  1.0178
    0.6116  0.2713  0.0812
    */
    matf16_t *cmat = matmul(&amat, &bmat);
    unsigned int *c = cmat->data;
    //printstr("result:\n");	// Ripes
    printf("result:\n");
	int32_t i = 0;

    // normal c in windows
    do {
    	printf("MD_r_h[%d] = 0x%08x\n", i, c[i]);
        i++;
    } while(i < 9);
    
    // Ripes
    /*
    do {
        printfloat(c[i]);
        printline();
        i ++;
    } while(i < 9);
	*/
	return 0;
}
