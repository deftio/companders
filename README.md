# Companders (a fixed point audio compression library) 

(c) 2001-2016  M. A. Chatterjee  < deftio [at] deftio [dot] com >
 
This document is a brief overview of the simple audio compression library for microcontrollers using A-Law (a type of compander).  This uses fixed-radix (integer only) math with
a small introductory disucssion and use of associated DC-offset correction with an IIR fixed-radix filter.

## Welcome..

The accompanying companders.c contains a small set of functions written in C using only integer math for companding operations.  I developed this several years ago for use in several embedded interized math projects and this small, slightly cleaned up version is made available for the public here.  It worked very well on M*CORE, 80x86 and ARM processors for small embedded systems.   

update: 2016 -- minor edits to this read (changed to markdown format).  updated license information (still free but uses OSI approved text now!)


usage:
```
#include "companders.h"   //no other dependancies or libaries are required.

#compile and link companders.c

```

## License
See attached LICENSE.txt file  (OSI approved BSD)

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

## Versions
1.0.1  3 Sep 2012  -- original release
1.0.2 15 Jul 2016  -- updated README.md to markdown format.  updated license to be OSI compliant.  no code changes. some minor doc updates.
				   -- shout out to John R Strohm giving me the nudge to update the docs here

## About companding...

Companding is a special type of lossy compression in which linear format samples are converted to logarithmic representation in order to preserve dynamic range of the original linear sample at the expense of uniform precision.  Companding is an old method and is free of patents (all have expired).

Theory of how companding works:
Suppose that we have a signed 16 bit linear sample in 2's complement math.  The range of this sample can vary from -32768 to + 32767 in integer steps.  Now lets put the constraint that we only have 8 bits with which to represent the data not the full 16 bits of the original linear number.  A simple way to preserve high order information would be to linearly truncate off the lower 8 bits giving us a signed number from -128 to +127.  We could make a mental note to keep track of the fact that we lost the lower 8 bits and so when use this representation we multiply by 8 bits (or 256) to preserve the input range.
so: 
-128 * 256 = -32768
-127 * 256 = -32512
...
  -1 * 256 =   -256
   0 * 256 =      0
   1 * 256 =    256
   2 * 256 =    512
...
 126 * 256 =  32256 
 127 * 256 =  32512 
 
 Notice that the steps between these linearly rounded off samples are quite large.  This truncated 8 bit representation would be very good at representing a linear quantity system such as linear displacement transducer which moves through its whole range as part of normal operation (like a door). but terrible at logarithmic phenomonen such as audio.  Audio information tends to be grouped around zero with occaisonaly peaks of loudness.  So with linear quantization soft audio would be lost due to the large quanitization steps at low volumes. To address this companders were developed for use in landline telephony for compressing audio data logarithmically instead of linearly.  Hence we use more bits for audio samples near zero and less with audio samples near the integer max extremes.
 
A-Law and it cousin mu-Law are companders.  Rather than represent samples in linear steps the more bits are allocated to samples near zero while larger magnitude (positive or negative) samples are represented with proportionately larger interval sizes.

Differential encoding (taking the difference of neighboring samples) can greatly help with compressing data to where we "have more bits" as well, provided samples rates are fast enough.  Telephony typically operated with 8 bit non-differential (companded) samples at 8KHz.
 
In A-Law the following table (in bits) shows how a 13 bit signed linear integer is companded in to A-Law:
(source is Sun microsystems, a similar table is on wikipedia or the G.711 specification).


| 		Linear Input Code	| Compressed Code |
|------------------------	| --------------- |
|   0000000wxyza			| 000wxyz         |
|   0000001wxyza			| 001wxyz         |
|   000001wxyzab			| 010wxyz         |
|   00001wxyzabc			| 011wxyz         |
|   0001wxyzabcd			| 100wxyz         |
|   001wxyzabcde			| 101wxyz         |
|   01wxyzabcdef			| 110wxyz         |
|   1wxyzabcdefg			| 111wxyz         |
 
 
 ## About this library:
 
 This free library supports A-Law and IIR averages.  A-Law is used for the companding while the IIR averagers can be used for for embedded ADCs where the zero point is set loosely.  Since the companders are sensitive, and allocate more bits, to values near zero its important to define a good zero.  For example a microcontroller has a pin with an ADC which is fed digitized audio signal.  The smallest value for the microtroncoller is 0V = 0x00 and the 3.3V = 0x3ff for 10 effective bits of resolution.  A resisitive analog divider is used to center the ADC near the half-input range or about 1.6V while the audio is capacitively coupled as shown here:  

 ```
 
             +3.3V
               |
               R
               |            
uC_ADC_Pin<-----------C---- audio_input_source
               |
               R
               |
              GND
 ```

 However cheap resistors have tolerances 5 to 10%, so the setpoint voltage could easily be anywhere from 1.4 to 1.8V.  To address this software should read the ADC before audio is coming in and determine the actual DC bias voltage set by the real world resistors.  Then when audio is coming in this value is subtracted from the ADC input to give a signed number which is fed to the LinearToAlaw encoder.  
 
 If it not easily possible to turn off the analog / audio source then the long term DC average can be inferred by using one of the IIR functions included here with a long window length (perhaps 2000 samples if sampling at 8 KHz or about 1/4 second).  These IIR functions are cheap to run in realtime even with reasonably high sample rates as they take little memory and are simple integer-math IIRs.
 
 ## About the Fixed Radix (FR) Math IIR averages
 
The (Infinite Impulse Reponse) IIR functions included here use fixed radix math to represent fractional parts of an integer.  By providing a settable radix (amount of bits devoted to fractional resolution), the programmer can make tradeoffs between resolution and dynamic range.  The larger the radix specified the more bits that will be used in the fractional portion of the representation, which for smaller window sizes may not by necessary.  There are more comprehensive ways to deal with fractional representation in binary systems (floating point, bigNum / arbitrary length registers, separation of numerator/denomators etc) but these incur much larger compute/code/memory overhead.  The simple system used here avoids the need for testing for overflow/underflow which allows for low foot print code/cpu/memory bandwidth.
 
To calculate how many bits of fractional part while preventing overflow use the following formulas:

``` 
Nb = number of bits in use = ceiling(log2(highest_number_represented))

radix = 32-Nb(sample_resolution)-Nb(WindowLength)-2

for example if you have a ADC generating counts from 0 to 1023 then the Nb(ADC-range) = 
  Nb = ceiling(log2(1023)) = 10 bits
  
If you use a DC average window length of 2000 
  Nb = ceiling(log2(2000)) = 11 bits
  
so the max radix that should be specified is:
  radix = 32 - 10 - 11 - 2 = 32 - 23 = 9 bits

```  
with 9 bits in the radix fractional precision of 512 units per integer (e.g 1/512) is possible.  The "2" in for formula comes from reserving bits for the sign bit and the additional operation in averager.


The function DIO_s32 DIO_IIRavgFR() allows any integer length window size to be used, while the function DIO_IIRavgPower2FR() specifies window lengths in powers of two but all calculations are based on shift operations which can be significantly faster on lower end microprocessors.


## Embedded Systems

Now back to an embedded microcontroller example.  Let's say it has an ADC which maps the voltage on 1 pin from 0-3.3V in to 0-4095 counts (12 bits).  We capacitively couple the input voltage and use the bias resistors to set mid point in the center of the range.  Lets say the the our resistors set the bias at 1.55V.  This equals our "zero point".  Below this point is negative from the incoming capacitively coupled audio and above this is positive.  

Our "guess" as to the bias = both resistors are the same = (3.3V/2) =1.65V = (1.65V)/(4095 counts/3.3V)= 2048 counts

Resistor actual set bias "zero point" = 1.55V = (1.55V) *(4095 counts/3.3V)) = 1923 counts 
We want this to be "zero" we use for the companded A/D process.

To do this we start our ADC to periodically sample for sometime before we start "recording" audio.   We will feed the ADC values in to our IIR average to find the actual zero point. Note that even when we decide not to "record" we still can still run the A/D and  IIR averager so its adapting to the real zero point.


## C-like Pseudo code
```
#include "companders.h"

static volatile int32 gIIRavg= 2048; // global static var holds DC average as estimated by the IIR
static volatile int   gDoSomethingWithCompandedValue=0;

void processAudioSample() //interrupt handler -- runs at the sample rate
{
	short adcValue;
	char  aLawValue;
	
	//read the raw uncorrected sample (has some DC offset bias) 
	adcValue= inputPin.read_u16(); // read a 16 bit unsigned sample
	
	// this updates the IIR average everytime the adc returns a sample
	gIIRavg = DIO_IIRavgPower2FR(gIIRavg,11,adcValue,8);
	
	//now compute companded value with DC offset correction
	if (gDoSomethingWithCompandedValue)
	{
		aLawValue = DIO_LinearToALaw(adcValue-DIO_FR2I(gIIRavg));  // subtract off the offset and get a good A-Law value
		doSomethingWithALaw(aLawValue);  // store it to a buffer, send it the cloud ;) etc
	}
}

main()
{
	gDoSomethingWithCompandedValue=0;
	inputPin.attachInterrupt(&processAudioSample,8000); //attach interrupt handler, 8000 Hz sample rate
	
	// ... somepoint later in the code
	
	inputPin.start(); // start the collecting audio.  inside the interrupt handler we'll start estimating the DC bias
	
	// ... some time later  (when we actually want the audio) we set this flag
	gDoSomethingWithCompandedValue=1; //now the interrupt routine will call the doSomethingWithALaw() function
	
	// ... some time later
	gDoSomethingWithCompandedValue=0; //we're no longer collecting ALaw companded date, but we're 
									  // still running the IIR averager
	
	
}
 
```

### Some Closing comments..
Finally,  it can be in some systems that we can't turn off the audio input source it may be hard wired to some sensor or mic or perhaps the A/D center bias circuit (the 2 resistors) always is on when the audio is on.  In this case running the IIR with a long filter length all the time can remove the bias even when the audio is running.  For example in an 8KHz sampling system with an IIR length of 1024 is about 1/8 of a second or a cutoff freq well below 10Hz and costs almost nothing to run.
 
 
 
