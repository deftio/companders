/**
 * arduino_compander.ino - Arduino example for companders library
 *
 * Demonstrates A-Law companding with ADC input and IIR DC offset correction
 * on an Arduino (or compatible board).
 *
 * Wiring:
 *   Audio input capacitively coupled to analog pin A0 with a resistor
 *   divider biasing the pin to ~VCC/2:
 *
 *       +5V (or 3.3V)
 *         |
 *         R  (10k)
 *         |
 *   A0 <--------C---- audio_input
 *         |
 *         R  (10k)
 *         |
 *        GND
 *
 * Copyright (c) 2001-2024, M. A. Chatterjee <deftio at deftio dot com>
 * BSD 2-Clause License (see LICENSE.txt)
 */

#include <companders.h>

// Configuration
const int   AUDIO_PIN            = A0;
const int   SAMPLE_RATE_HZ       = 8000;
const unsigned long SAMPLE_PERIOD_US = 1000000UL / SAMPLE_RATE_HZ;

// IIR averager state for DC offset estimation
// Window length = 2^8 = 256 samples (~32 ms at 8 kHz)
const unsigned char IIR_WINDOW_BITS = 8;
const unsigned char IIR_RADIX       = 6;

DIO_s32 dcBiasEstimate;  // fixed-radix representation of DC bias

void setup() {
    Serial.begin(115200);
    while (!Serial) { ; }

    Serial.println(F("Companders Arduino Example"));
    Serial.print(F("Library version: "));
    Serial.println(DIO_COMPANDERS_VER_STRING);
    Serial.println();

    // Seed the DC bias estimate at midpoint of ADC range
    // Arduino Uno: 10-bit ADC -> 0..1023, midpoint = 512
    int initialGuess = 512;
    dcBiasEstimate = DIO_I2FR(initialGuess, IIR_RADIX);

    Serial.println(F("sample, raw_adc, dc_bias, corrected, alaw"));
}

void loop() {
    static unsigned long lastSampleTime = 0;
    static unsigned long sampleCount    = 0;

    unsigned long now = micros();
    if (now - lastSampleTime < SAMPLE_PERIOD_US) {
        return;
    }
    lastSampleTime = now;

    // Read raw ADC value (0..1023 on most Arduinos)
    int rawAdc = analogRead(AUDIO_PIN);

    // Update IIR DC bias estimate (adapts to actual resistor divider voltage)
    dcBiasEstimate = DIO_IIRavgPower2FR(
        dcBiasEstimate,
        IIR_WINDOW_BITS,
        (DIO_s16)rawAdc,
        IIR_RADIX
    );

    // Subtract DC bias to get a signed, zero-centered sample
    DIO_s16 corrected = (DIO_s16)(rawAdc - DIO_FR2I(dcBiasEstimate, IIR_RADIX));

    // Scale 10-bit corrected value up to 16-bit range for the compander
    // (-512..511) -> (-32768..32704)
    DIO_s16 scaled = corrected << 6;

    // Compress to 8-bit A-Law
    DIO_s8 alawValue = DIO_LinearToALaw(scaled);

    // Decompress back (to demonstrate round-trip)
    DIO_s16 recovered = DIO_ALawToLinear(alawValue);

    // Print every 800th sample (~10 lines per second at 8 kHz)
    if ((sampleCount % 800) == 0) {
        Serial.print(sampleCount);
        Serial.print(F(", "));
        Serial.print(rawAdc);
        Serial.print(F(", "));
        Serial.print(DIO_FR2I(dcBiasEstimate, IIR_RADIX));
        Serial.print(F(", "));
        Serial.print(corrected);
        Serial.print(F(", "));
        Serial.print((int)alawValue);
        Serial.print(F(", recovered="));
        Serial.println(recovered);
    }

    sampleCount++;
}
