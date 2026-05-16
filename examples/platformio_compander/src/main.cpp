/**
 * PlatformIO example for companders library
 *
 * Works on Arduino Uno, ESP32, and other PlatformIO-supported boards.
 * Demonstrates A-Law and Mu-Law companding with ADC input and IIR DC
 * offset correction.
 *
 * Wiring:
 *   Audio input capacitively coupled to analog pin A0 with a resistor
 *   divider biasing the pin to ~VCC/2.
 *
 * Copyright (c) 2001-2024, M. A. Chatterjee <deftio at deftio dot com>
 * BSD 2-Clause License (see LICENSE.txt)
 */

#include <Arduino.h>
#include <companders.h>

const int AUDIO_PIN = A0;

// IIR averager: window = 2^8 = 256 samples, 6 bits fractional precision
const uint8_t IIR_WINDOW_BITS = 8;
const uint8_t IIR_RADIX       = 6;

DIO_s32 dcBias;

void setup()
{
    Serial.begin(115200);
    while (!Serial) { ; }

    Serial.println("Companders PlatformIO Example");
    Serial.print("Version: ");
    Serial.print(DIO_COMPANDERS_VER_STRING);
    Serial.print(" (hex: 0x");
    Serial.print(DIO_COMPANDERS_VER_HEX, HEX);
    Serial.println(")");
    Serial.println();

    // Seed DC bias at ADC midpoint
    // Uno = 10-bit (512), ESP32 = 12-bit (2048)
#if defined(ESP32) || defined(ESP8266)
    analogReadResolution(12);
    dcBias = DIO_I2FR(2048, IIR_RADIX);
#else
    dcBias = DIO_I2FR(512, IIR_RADIX);
#endif

    Serial.println("n, raw, dc_bias, alaw, ulaw");
}

void loop()
{
    static unsigned long n = 0;

    int raw = analogRead(AUDIO_PIN);

    // Update DC bias estimate
    dcBias = DIO_IIRavgPower2FR(dcBias, IIR_WINDOW_BITS, (DIO_s16)raw, IIR_RADIX);

    // Zero-center and scale to 16-bit range
    DIO_s16 corrected = (DIO_s16)(raw - DIO_FR2I(dcBias, IIR_RADIX));

#if defined(ESP32) || defined(ESP8266)
    DIO_s16 scaled = corrected << 4;  // 12-bit -> 16-bit
#else
    DIO_s16 scaled = corrected << 6;  // 10-bit -> 16-bit
#endif

    DIO_s8 alaw = DIO_LinearToALaw(scaled);
    DIO_s8 ulaw = DIO_LinearToULaw(scaled);

    if ((n % 800) == 0) {
        Serial.print(n);
        Serial.print(", ");
        Serial.print(raw);
        Serial.print(", ");
        Serial.print((int)DIO_FR2I(dcBias, IIR_RADIX));
        Serial.print(", ");
        Serial.print((int)alaw);
        Serial.print(", ");
        Serial.println((int)ulaw);
    }
    n++;

    delayMicroseconds(125);  // ~8 kHz sample rate
}
