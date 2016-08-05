// NeoPixelFunFadeInOut
// This example will randomly pick a color and fade all pixels to that color, then
// it will fade them to black and restart over
//
// This example demonstrates the use of a single animation channel to animate all
// the pixels at once.
//
#include "Arduino.h"
#include <NeoPixelBus.h>
#include <NewPing.h>
#include <avr/interrupt.h>
#include <avr/eeprom.h>

const uint8_t TriggerPin = 12;
const uint8_t EchoPin = 12;
const uint8_t ButtonPin = 11;
const float AbsMaxDistance = 500.0;
const float MaxDistance = 60.0;
const uint16_t DefaultDesiredDistance = 15;
float DesiredDistance;

const uint16_t PixelCount = 24; // make sure to set this to the number of pixels in your strip
const uint8_t PixelPin = 6;  // make sure to set this to the correct pin, ignored for Esp8266
const uint8_t AnalogInPin = 0;
RgbColor color;
RgbColor altColor;
RgbColor offColor;
RgbColor stopColor;
RgbColor tooFarColor;


int val = 0;

NeoGamma<NeoGammaTableMethod> colorGamma;
NeoPixelBus<NeoGrbFeature, Neo800KbpsMethod> strip(PixelCount, PixelPin);

NewPing sonar(TriggerPin, EchoPin, AbsMaxDistance);

// uint16_t uS;
float distanceCm = 0.0;
float previousDistanceCm = 0.0;
uint8_t PixelsLit = 0;
uint8_t PrevPixels = 0;

void evalDistance(float distanceCm);
void checkDefaultButtonPress(float distanceCm);
float getDesiredDistance();
void storeDesiredDistance(uint16_t dist);

void setup(){
    Serial.begin(115200);
    DesiredDistance = getDesiredDistance();
    strip.Begin();
    strip.Show();
    pinMode(ButtonPin, INPUT_PULLUP);
    color = RgbColor(0,255,0);
    color.Darken(200);
    altColor = RgbColor(238, 198, 95);
    altColor.Darken(200);
    offColor = RgbColor(0,0,0);
    stopColor = RgbColor(255,0,0);
    stopColor.Darken(128);
    tooFarColor = RgbColor(250,250,60);
    tooFarColor.Darken(160);
}

void loop(){
    distanceCm = sonar.ping_cm();
    Serial.print("Distance: ");
    Serial.print(distanceCm);
    Serial.print("cm");

    evalDistance(distanceCm);
    //checkDefaultButtonPress(distanceCm);
    Serial.println(' ');
    delay(150);
}

float getDesiredDistance(){
    uint16_t dist;
    while(!eeprom_is_ready());
    cli();
    dist = eeprom_read_word((uint16_t*)1);
    sei();
    // Serial.print("desired distance from EEPROM: ");
    // Serial.println(dist);
    if((float)dist >= MaxDistance || (float)dist <= 0.0){
        dist = DefaultDesiredDistance;
        // Serial.print("desired distance: ");
        // Serial.println(dist);
        storeDesiredDistance(dist);
    }

    return (float)dist;
}

void storeDesiredDistance(uint16_t dist){
    while(!eeprom_is_ready());
    cli();
    if(eeprom_read_word((uint16_t*)1) != dist) {
        eeprom_write_word((uint16_t*)1, dist);
    }
    sei();
}

void checkDefaultButtonPress(float distanceCm){
    bool buttonState = digitalRead(ButtonPin);
    if(distanceCm != DesiredDistance && buttonState == LOW){
        storeDesiredDistance((uint16_t)distanceCm);
        DesiredDistance = getDesiredDistance();
    }
}

void evalDistance(float distanceCm){
    float distanceDiv = (MaxDistance - DesiredDistance) / 12.0;
    // float distanceDiv = (MaxDistance - distanceCm) / (MaxDistance - DesiredDistance);
    Serial.print(" / Dist Div: ");
    Serial.print(distanceDiv);
    if(distanceCm == 0){
        //system returns 0.0 as distance when beyond max distance
        //turn off lights
        strip.ClearTo(colorGamma.Correct(offColor));
        strip.Show();
    }else if(distanceCm > MaxDistance){
        strip.ClearTo(colorGamma.Correct(offColor));
        strip.Show();
    }else if(distanceCm < DesiredDistance - distanceDiv){
        strip.ClearTo(colorGamma.Correct(tooFarColor));
        strip.Show();
    }else if(distanceCm <= DesiredDistance){
        strip.ClearTo(colorGamma.Correct(stopColor));
        strip.Show();
    }else{
        //ABS(12  − ROUND((distanceCm − DesiredDistance) / distanceDiv,0))
        // PixelsLit = (uint8_t)round(distanceDiv * 12.0);
        PixelsLit = (uint8_t)abs(12 - round((distanceCm - DesiredDistance) / distanceDiv));
        if(PixelsLit == 12){
            strip.ClearTo(colorGamma.Correct(stopColor));
        }
        else if(PixelsLit > 0){
            for(uint16_t index = 0; index <= strip.PixelCount() / 2; index++){
                if(index < PixelsLit){
                    strip.SetPixelColor(index, colorGamma.Correct(color));
                    strip.SetPixelColor(strip.PixelCount() - index, colorGamma.Correct(color));
                }else{
                    strip.SetPixelColor(index, colorGamma.Correct(altColor));
                    strip.SetPixelColor(strip.PixelCount() - index, colorGamma.Correct(altColor));
                }
            }
        }
        Serial.print(" / Num Pixels Lit: ");
        Serial.print(PixelsLit);

        strip.Show();
    }
}
