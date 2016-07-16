/**
 *	@companders.c - implementation
 *		
 *	@copy copyright (c) <2001-2016>  <M. A. Chatterjee>
 *  @author M A Chatterjee <deftio [at] deftio [dot] com>
 *	@version 1.02 M. A. Chatterjee, cleaned up naming, license
 *
 
LICENSE: 

Copyright (c) 2001-2016, M. A. Chatterjee < deftio at deftio dot com >
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

#include "companders.h"

DIO_s8  DIO_LinearToALaw(DIO_s16 sample)
{ 
     const DIO_s16 cClip = 32635;
     const static DIO_s8 LogTable[128] = 
     { 
     1,1,2,2,3,3,3,3,4,4,4,4,4,4,4,4,5,5,5,5,5,5,5,5,5,5,5,5,5,5,5,5, 
     6,6,6,6,6,6,6,6,6,6,6,6,6,6,6,6,6,6,6,6,6,6,6,6,6,6,6,6,6,6,6,6, 
     7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7, 
     7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7 
     };

     DIO_s32 sign, exponent, mantissa; 
     DIO_s8 compandedValue; 
	 sample = (sample ==-32768) ? -32767 : sample;
     sign = ((~sample) >> 8) & 0x80; 
     if (!sign) 
          sample = (short)-sample; 
     if (sample > cClip) 
          sample = cClip; 
     if (sample >= 256) 
     { 
          exponent = (int)LogTable[(sample >> 8) & 0x7F]; 
          mantissa = (sample >> (exponent + 3) ) & 0x0F; 
          compandedValue = ((exponent << 4) | mantissa); 
     } 
     else 
     { 
          compandedValue = (unsigned char)(sample >> 4); 
     } 
     compandedValue ^= (sign ^ 0x55); 
     return compandedValue; 
}

DIO_s16 DIO_ALawToLinear(DIO_s8 aLawByte)
{
	 const static DIO_s16 ALawDecompTable[256]={
	  5504,  5248,  6016,  5760,  4480,  4224,  4992,  4736,
	  7552,  7296,  8064,  7808,  6528,  6272,  7040,  6784,
	  2752,  2624,  3008,  2880,  2240,  2112,  2496,  2368,
	  3776,  3648,  4032,  3904,  3264,  3136,  3520,  3392,
	 22016, 20992, 24064, 23040, 17920, 16896, 19968, 18944,
	 30208, 29184, 32256, 31232, 26112, 25088, 28160, 27136,
	 11008, 10496, 12032, 11520,  8960,  8448,  9984,  9472,
	 15104, 14592, 16128, 15616, 13056, 12544, 14080, 13568,
	   344,   328,   376,   360,   280,   264,   312,   296,
	   472,   456,   504,   488,   408,   392,   440,   424,
		88,    72,   120,   104,    24,     8,    56,    40,
	   216,   200,   248,   232,   152,   136,   184,   168,
	  1376,  1312,  1504,  1440,  1120,  1056,  1248,  1184,
	  1888,  1824,  2016,  1952,  1632,  1568,  1760,  1696,
	   688,   656,   752,   720,   560,   528,   624,   592,
	   944,   912,  1008,   976,   816,   784,   880,   848,
	 -5504, -5248, -6016, -5760, -4480, -4224, -4992, -4736,
	 -7552, -7296, -8064, -7808, -6528, -6272, -7040, -6784,
	 -2752, -2624, -3008, -2880, -2240, -2112, -2496, -2368,
	 -3776, -3648, -4032, -3904, -3264, -3136, -3520, -3392,
	-22016,-20992,-24064,-23040,-17920,-16896,-19968,-18944,
	-30208,-29184,-32256,-31232,-26112,-25088,-28160,-27136,
	-11008,-10496,-12032,-11520, -8960, -8448, -9984, -9472,
	-15104,-14592,-16128,-15616,-13056,-12544,-14080,-13568,
	  -344,  -328,  -376,  -360,  -280,  -264,  -312,  -296,
	  -472,  -456,  -504,  -488,  -408,  -392,  -440,  -424,
	   -88,   -72,  -120,  -104,   -24,    -8,   -56,   -40,
	  -216,  -200,  -248,  -232,  -152,  -136,  -184,  -168,
	 -1376, -1312, -1504, -1440, -1120, -1056, -1248, -1184,
	 -1888, -1824, -2016, -1952, -1632, -1568, -1760, -1696,
	  -688,  -656,  -752,  -720,  -560,  -528,  -624,  -592,
	  -944,  -912, -1008,  -976,  -816,  -784,  -880,  -848};
	 DIO_s16 addr = ((DIO_s16)aLawByte)+128; // done for compilers with poor expr type enforcement
	 return ALawDecompTable[addr]; 
}

// see companders.h
// fixed-radix IIR averager implementation supporting arbitrarily chosen windows
DIO_s32 DIO_IIRavgFR	   (DIO_s32 prevAvg, DIO_u16 windowLen, DIO_s16 newSample, DIO_u8 radix)
{
	DIO_s32 iirAvg=0;
	iirAvg = ((prevAvg * (windowLen-1)) + (DIO_I2FR(newSample,radix)))/windowLen;
	return iirAvg;
}

// see companders.h
// fixed-radix IIR averager implementation using power-of-2 sized windows
// and only shift operations for cpu efficiency
DIO_s32 DIO_IIRavgPower2FR (DIO_s32 prevAvg, DIO_u8 windowLenInBits, DIO_s16 newSample, DIO_u8 radix)
{
	DIO_s32 iirAvg=0;
	iirAvg = (((prevAvg<<windowLenInBits)-prevAvg) + (DIO_I2FR(newSample,radix)))>>windowLenInBits;
	return iirAvg;
}
