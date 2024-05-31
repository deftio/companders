#include <stdio.h>
#include "companders.h"

void test_DIO_LinearToALaw() {
    printf("Testing DIO_LinearToALaw...\n");
    DIO_s16 sample = 12345;
    DIO_s8 result = DIO_LinearToALaw(sample);
    printf("Result: %d\n", result);
}

void test_DIO_ALawToLinear() {
    printf("Testing DIO_ALawToLinear...\n");
    DIO_s8 aLawByte = 123;
    DIO_s16 result = DIO_ALawToLinear(aLawByte);
    printf("Result: %d\n", result);
}

void test_DIO_IIRavgFR() {
    printf("Testing DIO_IIRavgFR...\n");
    DIO_s32 prevAvg = 12345;
    DIO_u16 windowLen = 10;
    DIO_s16 newSample = 123;
    DIO_u8 radix = 8;
    DIO_s32 result = DIO_IIRavgFR(prevAvg, windowLen, newSample, radix);
    printf("Result: %ld\n", result);
}

void test_DIO_IIRavgPower2FR() {
    printf("Testing DIO_IIRavgPower2FR...\n");
    DIO_s32 prevAvg = 12345;
    DIO_u8 windowLenInBits = 10;
    DIO_s16 newSample = 123;
    DIO_u8 radix = 8;
    DIO_s32 result = DIO_IIRavgPower2FR(prevAvg, windowLenInBits, newSample, radix);
    printf("Result: %ld\n", result);
}

int main() {
    test_DIO_LinearToALaw();
    test_DIO_ALawToLinear();
    test_DIO_IIRavgFR();
    test_DIO_IIRavgPower2FR();
    return 0;
}
