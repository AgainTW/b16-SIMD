#ifndef _MAT_INT32_
#define _MAT_INT32_

struct _matf16_t
{
    int32_t row;
    int32_t col;
    unsigned int *data;
};
typedef struct _matf16_t matf16_t;

uint32_t highestbit(uint32_t x);

int m_mul(int32_t a, int32_t b) ;

unsigned int fmul_b16(unsigned int a, unsigned int b);

unsigned int fadd_b16(unsigned int a, unsigned int b);

matf16_t *matmul(matf16_t *first, matf16_t *second, matf16_t *retmat, unsigned int *retdata);

#endif