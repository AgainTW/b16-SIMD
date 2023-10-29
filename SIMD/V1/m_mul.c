#include<stdio.h>
#include<stdint.h>

int m_mul_1(int32_t a, int32_t b) 
{
    int32_t r = 0;
    a = a << 1; /* to counter last right shift */
    while(b != 0) 
    {
        if((b & 1) != 0) 
        {
            r = r + a;
        }
        b = b >> 1;
        r = r >> 1;
    }
    return r;
}

int m_mul_2(int32_t a, int32_t b) 
{
    int32_t r = 0;
    unsigned int a_h, a_l, b_h, b_l;
    a_h = (a & 0xFFFF0000) << 1;    /* to counter last right shift */
    a_l = (a & 0xFFFF)  << 1;       /* to counter last right shift */
    b_h = (b & 0xFFFF0000) >>16;    /* to counter last right shift */
    b_l = b & 0xFFFF;               /* to counter last right shift */
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

void main()
{
    unsigned int MD1=0x7F007F, MD2=0x7F007F;
    unsigned int result_1, result_1_h, result_1_l;
    unsigned int result_2;

    unsigned int MD1_h = (MD1 & 0xFFFF0000) >> 16;
    unsigned int MD2_h = (MD2 & 0xFFFF0000) >> 16;
    unsigned int MD1_l = MD1 & 0xFFFF;
    unsigned int MD2_l = MD2 & 0xFFFF;

    result_1_h = m_mul_1(MD1_h, MD2_h);
    result_1_l = m_mul_1(MD1_l, MD2_l);
    result_1 = (result_1_h << 16) + result_1_l;

    result_2 = m_mul_2(MD1, MD2);

    printf("result_1 = 0x%08x \n", result_1);
    printf("result_2 = 0x%08x \n", result_2);
}
