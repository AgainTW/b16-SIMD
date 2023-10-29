#include<stdio.h>

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

void main()
{
	float a1, a2;
	float a1_to_bf16, a2_to_bf16;
	
	scanf("%f %f", &a1, &a2);
	
	a1_to_bf16 = fp32_to_bf16(a1);
	a2_to_bf16 = fp32_to_bf16(a2);
	
	float y1 = a1_to_bf16;
	float y2 = a2_to_bf16;
	float y3 = a1;
	float y4 = a2;
	int *p1 = (int *) &y1;
	int *p2 = (int *) &y2;
	int *p3 = (int *) &y3;
	int *p4 = (int *) &y4;
	unsigned int a1_b16_HEX = *p1;
	unsigned int a2_b16_HEX = *p2;
	unsigned int a1_HEX = *p3;
	unsigned int a2_HEX = *p4;
	
	unsigned int MD = 0;
	MD = a1_b16_HEX + (a2_b16_HEX>>16);

	printf("a1_HEX = 0x%08x \n", a1_HEX);
	printf("a2_HEX = 0x%08x \n", a2_HEX);	
	printf("a1_b16 = %f \n", a1_to_bf16);
	printf("a2_b16 = %f \n", a2_to_bf16);
	printf("a1_b16_HEX = 0x%08x \n", a1_b16_HEX);
	printf("a2_b16_HEX = 0x%08x \n", a2_b16_HEX);
	printf("MD = 0x%08x \n", MD);
	
}
