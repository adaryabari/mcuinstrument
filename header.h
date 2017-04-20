#ifndef HEADER_H
#define HEADER_H

#include <STM32F0xx.h>

void initHex();
uint8_t bPress();
void initSpeaker();
void speakerOn(uint16_t period, uint16_t compare1, uint16_t compare2);
void initSpeaker2();
void speaker2On(uint16_t period);
void initButton();
uint32_t usrButPress();
void initPhotocell();
void initLed();
void initPorts();

void pinHigh(char port, int pin);
void pinLow(char port, int pin);
void initPin(char port, int pin, int mode);

void printNote(int on, int note, uint64_t mil);
void serialStart(uint16_t baud);
uint8_t getChar();
int putChar(uint8_t txChar);

#endif
