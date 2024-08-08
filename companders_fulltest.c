/**
 *  @file companders_fulltest.c - test file for integer companding C implementation
 *
 *  @copy copyright (c)  <M. A. Chatterjee>
 *  @autor M A Chatterjee <deftio [at] deftio [dot] com>
 *
LICENSE: 

Copyright (c) 2001-2024, M. A. Chatterjee < deftio at deftio dot com >
All rights reserved.

Redistribution and use in source and binary forms, with or without
modification, are permitted provided that the following conditions are met:

* Redistributions of source code must retain the above copyright notice, this
  list of conditions and the following disclaimer.

* Redistributions in binary form must reproduce the above copyright notice,
  this list of conditions and the following disclaimer in the documentation
  and/or other materials provided with the distribution.

THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE
FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL
DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR
SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER
CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY,
OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.

 *
 */

#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include "companders.h"


void test_LinearToALaw() {
    DIO_s16 testSamples[] = {-2460, -338, -1, 499, 980};
    DIO_s8 expectedResults[] = {22, 64, 85, -54, -5};
    size_t numSamples = sizeof(testSamples) / sizeof(testSamples[0]);
    for (size_t i = 0; i < numSamples; ++i) {
        DIO_s8 companded = DIO_LinearToALaw(testSamples[i]);
        printf("Linear to A-Law: %5d -> %5d (expected: %5d)\n", testSamples[i], companded, expectedResults[i]);
        assert(companded == expectedResults[i]);
    }
}

void test_ALawToLinear() {
    DIO_s8 testSamples[] = {22, 64, 85, -54, -5};
    DIO_s16 expectedResults[] = {-2496, -344,-8,504,976};
    size_t numSamples = sizeof(testSamples) / sizeof(testSamples[0]);
    for (size_t i = 0; i < numSamples; ++i) {
        DIO_s16 linear = DIO_ALawToLinear(testSamples[i]);
        printf("A-Law to Linear: %5d -> %5d (expected: %5d)\n", testSamples[i], linear, expectedResults[i]);
        assert(linear == expectedResults[i]);
    }
}

void test_LinearToULaw() {
    DIO_s16 testSamples[]     = {-2460, -338,   -1,  499, 7000};
    DIO_s8  expectedResults[] = { 0x01c, 0x048, 0x07e, 0x0bf, 0x084};
    size_t numSamples = sizeof(testSamples) / sizeof(testSamples[0]);
    for (size_t i = 0; i < numSamples; ++i) {
        DIO_s8 companded = DIO_LinearToULaw(testSamples[i]);
        printf("Linear to u-Law: %5d -> %5d (expected: %5d)\n", testSamples[i], companded), (int)expectedResults[i];
       // assert(companded == expectedResults[i]);
    }
}

void test_ULawToLinear() {
    DIO_s8 testSamples[]      = { 0x1c, 0x61, 0x7e, 0xbf, 0x84};
    DIO_s16 expectedResults[] = {-2463,  -89,   -2,  495, 7007};
    size_t numSamples = sizeof(testSamples) / sizeof(testSamples[0]);
    for (size_t i = 0; i < numSamples; ++i) {
        DIO_s16 linear = DIO_ULawToLinear(testSamples[i]);
        printf("u-Law to Linear: %5d -> %5d (expected: %5d)\n", testSamples[i], linear, expectedResults[i]);
       // assert(linear == expectedResults[i]);
    }
}

void test_IIRavgFR() {
    DIO_s32 prevAvg = 1000;
    DIO_u16 windowLen = 10;
    DIO_s16 newSample = 2000;
    DIO_u8 radix = 8;
    DIO_s32 expectedAvg = 203; // Replace with actual expected value based on calculation
    DIO_s32 avg = DIO_IIRavgFR(prevAvg, windowLen, newSample, radix) / (1 << radix);
    printf("IIRavgFR: prevAvg=%ld, windowLen=%d, newSample=%d, radix=%d -> avg=%ld\n",
           prevAvg, windowLen, newSample, radix, avg);
    assert(avg == expectedAvg);
}

void test_IIRavgPower2FR() {
    DIO_s32 prevAvg = 1000;
    DIO_u8 windowLenInBits = 4;
    DIO_s16 newSample = 2000;
    DIO_u8 radix = 8;
    DIO_s32 expectedAvg = 128; // Replace with actual expected value based on calculation
    DIO_s32 avg = DIO_IIRavgPower2FR(prevAvg, windowLenInBits, newSample, radix) / (1 << radix);
    printf("IIRavgPower2FR: prevAvg=%ld, windowLenInBits=%d, newSample=%d, radix=%d -> avg=%ld\n",
           prevAvg, windowLenInBits, newSample, radix, avg);
    assert(avg == expectedAvg);
}

int main() {
    printf("Testing LinearToALaw...\n");
    test_LinearToALaw();
    printf("\nTesting ALawToLinear...\n");
    test_ALawToLinear();
    printf("\nTesting LinearToULaw...\n");
    test_LinearToULaw();
    printf("\nTesting ULawToLinear...\n");
    test_ULawToLinear();
    printf("\nTesting IIRavgFR...\n");
    test_IIRavgFR();
    printf("\nTesting IIRavgPower2FR...\n");
    test_IIRavgPower2FR();
    return 0;
}
