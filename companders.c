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

const static DIO_s8 LogTable[128] =
	{
		1, 1, 2, 2, 3, 3, 3, 3, 4, 4, 4, 4, 4, 4, 4, 4, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5,
		6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6,
		7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7,
		7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7};

DIO_s8 DIO_LinearToALaw(DIO_s16 sample)
{
	const DIO_s16 cClip = 32635;

	DIO_s32 sign, exponent, mantissa;
	DIO_s8 compandedValue;
	sample = (sample == -32768) ? -32767 : sample;
	sign = ((~sample) >> 8) & 0x80;
	if (!sign)
		sample = (short)-sample;
	if (sample > cClip)
		sample = cClip;
	if (sample >= 256)
	{
		exponent = (int)LogTable[(sample >> 8) & 0x7F];
		mantissa = (sample >> (exponent + 3)) & 0x0F;
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
	const static DIO_s16 ALawDecompTable[256] = {
		5504, 5248, 6016, 5760, 4480, 4224, 4992, 4736,
		7552, 7296, 8064, 7808, 6528, 6272, 7040, 6784,
		2752, 2624, 3008, 2880, 2240, 2112, 2496, 2368,
		3776, 3648, 4032, 3904, 3264, 3136, 3520, 3392,
		22016, 20992, 24064, 23040, 17920, 16896, 19968, 18944,
		30208, 29184, 32256, 31232, 26112, 25088, 28160, 27136,
		11008, 10496, 12032, 11520, 8960, 8448, 9984, 9472,
		15104, 14592, 16128, 15616, 13056, 12544, 14080, 13568,
		344, 328, 376, 360, 280, 264, 312, 296,
		472, 456, 504, 488, 408, 392, 440, 424,
		88, 72, 120, 104, 24, 8, 56, 40,
		216, 200, 248, 232, 152, 136, 184, 168,
		1376, 1312, 1504, 1440, 1120, 1056, 1248, 1184,
		1888, 1824, 2016, 1952, 1632, 1568, 1760, 1696,
		688, 656, 752, 720, 560, 528, 624, 592,
		944, 912, 1008, 976, 816, 784, 880, 848,
		-5504, -5248, -6016, -5760, -4480, -4224, -4992, -4736,
		-7552, -7296, -8064, -7808, -6528, -6272, -7040, -6784,
		-2752, -2624, -3008, -2880, -2240, -2112, -2496, -2368,
		-3776, -3648, -4032, -3904, -3264, -3136, -3520, -3392,
		-22016, -20992, -24064, -23040, -17920, -16896, -19968, -18944,
		-30208, -29184, -32256, -31232, -26112, -25088, -28160, -27136,
		-11008, -10496, -12032, -11520, -8960, -8448, -9984, -9472,
		-15104, -14592, -16128, -15616, -13056, -12544, -14080, -13568,
		-344, -328, -376, -360, -280, -264, -312, -296,
		-472, -456, -504, -488, -408, -392, -440, -424,
		-88, -72, -120, -104, -24, -8, -56, -40,
		-216, -200, -248, -232, -152, -136, -184, -168,
		-1376, -1312, -1504, -1440, -1120, -1056, -1248, -1184,
		-1888, -1824, -2016, -1952, -1632, -1568, -1760, -1696,
		-688, -656, -752, -720, -560, -528, -624, -592,
		-944, -912, -1008, -976, -816, -784, -880, -848};
	DIO_s16 addr = ((DIO_s16)aLawByte) + 128; // done for compilers with poor expr type enforcement
	return ALawDecompTable[addr];
}

#define cBias 0x84
#define cClip 32635

DIO_s8 DIO_LinearToULaw(DIO_s16 sample)
{
	DIO_s32 sign, exponent, mantissa;
	DIO_s8 compandedValue;

	// Get the sign and the magnitude of the sample
	sign = (sample >> 8) & 0x80;
	if (sign != 0)
		sample = -sample;
	if (sample > cClip)
		sample = cClip;
	sample += cBias;

	// Convert the magnitude to uLaw
	if (sample >= 256)
	{
		exponent = LogTable[(sample >> 7) & 0xFF];
		mantissa = (sample >> (exponent + 3)) & 0x0F;
		compandedValue = ~((sign | (exponent << 4) | mantissa) & 0xFF);
	}
	else
	{
		compandedValue = ~((sign | sample >> 4) & 0xFF);
	}

	return compandedValue;
}

DIO_s16 DIO_ULawToLinear(DIO_s8 uLawByte)
{
	static const DIO_s16 ULawDecompTable[256] = {
		-32124, -31100, -30076, -29052, -28028, -27004, -25980, -24956,
		-23932, -22908, -21884, -20860, -19836, -18812, -17788, -16764,
		-15996, -15484, -14972, -14460, -13948, -13436, -12924, -12412,
		-11900, -11388, -10876, -10364, -9852, -9340, -8828, -8316,
		-7932, -7676, -7420, -7164, -6908, -6652, -6396, -6140,
		-5884, -5628, -5372, -5116, -4860, -4604, -4348, -4092,
		-3900, -3772, -3644, -3516, -3388, -3260, -3132, -3004,
		-2876, -2748, -2620, -2492, -2364, -2236, -2108, -1980,
		-1884, -1828, -1772, -1716, -1660, -1604, -1548, -1492,
		-1436, -1380, -1324, -1268, -1212, -1156, -1100, -1044,
		-988, -932, -876, -820, -764, -708, -652, -596,
		-556, -532, -508, -484, -460, -436, -412, -388,
		-364, -340, -316, -292, -268, -244, -220, -196,
		-172, -148, -124, -100, -76, -52, -28, -4,
		32124, 31100, 30076, 29052, 28028, 27004, 25980, 24956,
		23932, 22908, 21884, 20860, 19836, 18812, 17788, 16764,
		15996, 15484, 14972, 14460, 13948, 13436, 12924, 12412,
		11900, 11388, 10876, 10364, 9852, 9340, 8828, 8316,
		7932, 7676, 7420, 7164, 6908, 6652, 6396, 6140,
		5884, 5628, 5372, 5116, 4860, 4604, 4348, 4092,
		3900, 3772, 3644, 3516, 3388, 3260, 3132, 3004,
		2876, 2748, 2620, 2492, 2364, 2236, 2108, 1980,
		1884, 1828, 1772, 1716, 1660, 1604, 1548, 1492,
		1436, 1380, 1324, 1268, 1212, 1156, 1100, 1044,
		988, 932, 876, 820, 764, 708, 652, 596,
		556, 532, 508, 484, 460, 436, 412, 388,
		364, 340, 316, 292, 268, 244, 220, 196,
		172, 148, 124, 100, 76, 52, 28, 4};

	return ULawDecompTable[(unsigned char)uLawByte];
}

// see companders.h
// fixed-radix IIR averager implementation supporting arbitrarily chosen windows
DIO_s32 DIO_IIRavgFR(DIO_s32 prevAvg, DIO_u16 windowLen, DIO_s16 newSample, DIO_u8 radix)
{
	DIO_s32 iirAvg = 0;
	iirAvg = ((prevAvg * (windowLen - 1)) + (DIO_I2FR(newSample, radix))) / windowLen;
	return iirAvg;
}

// see companders.h
// fixed-radix IIR averager implementation using power-of-2 sized windows
// and only shift operations for cpu efficiency
DIO_s32 DIO_IIRavgPower2FR(DIO_s32 prevAvg, DIO_u8 windowLenInBits, DIO_s16 newSample, DIO_u8 radix)
{
	DIO_s32 iirAvg = 0;
	iirAvg = (((prevAvg << windowLenInBits) - prevAvg) + (DIO_I2FR(newSample, radix))) >> windowLenInBits;
	return iirAvg;
}
