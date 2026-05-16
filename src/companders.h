/**
 *	@companders.h - header definition file for embedded companding routines
 *
 *	@copy Copyright (C) <2001-2026>  <M. A. Chatterjee>
 *  @author M A Chatterjee <deftio [at] deftio [dot] com>
 *	@version 1.0.7 M. A. Chatterjee, cleaned up naming, license
 *
 *  This file contains integer math settable fixed point radix math routines for
 *  use on systems in which floating point is not desired or unavailable.
 *
LICENSE:

Copyright (c) 2001-2026, M. A. Chatterjee < deftio at deftio dot com >
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

#ifndef DIO_COMPAND_H
#define DIO_COMPAND_H

#ifdef __cplusplus
extern "C"
{
#endif

// Version as 32-bit hex: 0x00MMmmpp (Major.Minor.Patch)
#define DIO_COMPANDERS_VER_MAJOR  1
#define DIO_COMPANDERS_VER_MINOR  0
#define DIO_COMPANDERS_VER_PATCH  7

#define DIO_COMPANDERS_VER_HEX    ((DIO_COMPANDERS_VER_MAJOR << 16) | (DIO_COMPANDERS_VER_MINOR << 8) | DIO_COMPANDERS_VER_PATCH)  // 0x00010007
#define DIO_COMPANDERS_VER_STRING "1.0.7"

//"DIO" prefixes are to assist in preventing name collisions if global namespace is used.

// typedefs for portable fixed-width integers
// If your compiler supports <stdint.h> (C99+), we use it directly.
// For older embedded compilers, define DIO_NO_STDINT to fall back to
// platform-specific typedefs which you may need to adjust.
#if !defined(DIO_NO_STDINT)
#include <stdint.h>
typedef uint8_t   DIO_u8  ;
typedef int8_t    DIO_s8  ;
typedef uint16_t  DIO_u16 ;
typedef int16_t   DIO_s16 ;
typedef uint32_t  DIO_u32 ;
typedef int32_t   DIO_s32 ;
#else
typedef unsigned char   DIO_u8  ;
typedef signed char     DIO_s8  ;
typedef unsigned short  DIO_u16 ;
typedef signed short    DIO_s16 ;
typedef unsigned long   DIO_u32 ;
typedef signed long     DIO_s32 ;
#endif

// macros for converting from Fixed-Radix to integer, vice-versa
// r represents radix precision in bits, converts to/from integer w truncation
#define DIO_I2FR(x,r)		((x)<<(r))
#define DIO_FR2I(x,r)		((x)>>(r))


// convert FR to double, this is for debug only and WILL NOT compile under many embedded systems.
// use this in test harnesses. Since this is a macro if its not used it won't expand / link
#define DIO_FR2D(x,r) ((double)(((double)(x))/((double)(1<<(r)))))

//convert signed linear 16 bit sample to an 8 bit A-Law companded sample
DIO_s8  DIO_LinearToALaw(DIO_s16 sample);

//convert 8bit Alaw companded representation back to linear 16 bit
DIO_s16 DIO_ALawToLinear(DIO_s8 aLawByte);

//convert signed linear 16 bit sample to an 8 bit u-Law companded sample
DIO_s8  DIO_LinearToULaw(DIO_s16 sample);

//convert signed linear 16 bit sample to an 8 bit u-Law companded sample
DIO_s16 DIO_ULawToLinear(DIO_s8 uLawByte);


//DC Offset correction for integer companders
//IIR: y_0=(y_1*(w-1)+x_0)/(w)
//where w is the window length
//below are fixed radix precision IIR averagers which allow runtime tradeoffs for windowLen & precision
//Note that (windowLen)*(1<<radix) must < 32767


//DIO_IIRavgFR() allows any window length but uses a divide instruction.
//output is in radix number of bits
DIO_s32 DIO_IIRavgFR	   (DIO_s32 prevAvg, DIO_u16 windowLen, DIO_s16 newSample, DIO_u8 radix);

//DIO_IIRavgPower2FR() similar to above, but window length is specified as a number of bits
//which removes the need for a divide in the implementation
//output is in radix number of bits
DIO_s32 DIO_IIRavgPower2FR (DIO_s32 prevAvg, DIO_u8 windowLenInBits, DIO_s16 newSample, DIO_u8 radix);

#ifdef __cplusplus
}
#endif

#endif /* DIO_COMPAND_H */
