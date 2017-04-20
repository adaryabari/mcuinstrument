#include <STM32F0xx.h>
#include <math.h>
#include "header.h"

int i;
int blueVal1 = 1; 
int blueVal2 = 0;
int blueVal3 = 0;
int greenVal1 = 1; 
int greenVal2 = 0; 
int greenVal3 = 0;
int photoOn = 1;
int64_t debounce = 0;
int64_t msTicks = 0;
int buf[100];
int size = 11;
int getBuff = 0;
int numIndex = 0;
int scancode = 0, noteNum = 0, startKey = 60, newNote = 0, prevNote = 0;
float frequency;

void initAll();                   
void playNote(int scan);
void turnOff();
void printNote(int on, int note, uint64_t mil);
int getKey();
void SysTick_Handler();

int main() 
{
  initAll();

  while(1) 
	{
    scancode = bPress();
    if(scancode == 8) 
			noteNum = 1;
    else if(noteNum) 
		{
      if(getKey() == -1) 
			{
        noteNum = 0;
        startKey = newNote;
        newNote = 0;
      }
      else if(scancode && bPress() && debounce <= 0) 
			{
        newNote = (newNote * 10) + getKey();
        debounce = 7;
      }
    }
    else if(bPress() && scancode) 
		{
			debounce = 7;
			playNote(scancode);
		}
		else if(!bPress()) 
		{
      turnOff();
		}
		
    if(size)
      if(putChar(buf[0]))
			{
        for(i = 0; i < size - 1; i++)
          buf[i] = buf[i + 1]; //test
				
        size--;
      }

    if(photoOn) 
			speaker2On((ADC1->DR * .59) + 100);
    else 
			TIM3->CCR1 = 0;
		
    ADC1->CR |= (1<<2);
			
		if(blueVal2 <= 0) 
		{
			if(blueVal3) 
				GPIOC->BRR |= (1 << 8);
			else 
				GPIOC->BSRR |= (1 << 8);
			
			blueVal3 ^= 1;
			blueVal2 = 1000/(2 * blueVal1);
		}
		if(greenVal2 <= 0) 
		{
			if(greenVal3) 
				GPIOC->BRR |= (1 << 9);
			else 
				GPIOC->BSRR |= (1 << 9);
			
			greenVal3 ^= 1;
			greenVal2 = 1000 / (2 * greenVal1);
		}
  }
}

void playNote(int scan) 
{
  if(scan > 8) 
		scan--;
	if(!prevNote) 
	{ 
		prevNote = scan;
		printNote(0, scan + startKey, msTicks);
	}
  frequency = pow(2.0, ((float)(startKey + (scan * 2)) - 69.0) / 12.0) * 440.0;
  speakerOn(frequency, 50, 50);
}

void turnOff() 
{
	if(prevNote) 
	{
		printNote(prevNote, prevNote + startKey, msTicks);
		prevNote = 0;
	}
	
  prevNote = 0;
  TIM1->CCR1 = 0;
  TIM1->CCR2 = 0;
}

int getKey() 
{
	scancode = bPress();
  while(scancode > 12 || scancode % 4 == 0) 
	{
    scancode = bPress();
    if(scancode == 15) 
			return -1;
    if(scancode == 14) 
			return 0;
  }
  return scancode - (scancode / 4);
}

void SysTick_Handler() 
{
  if(!bPress() && !(GPIOA->IDR & 1)) debounce--;
  msTicks++;
	blueVal2--;
	greenVal2--;

  if((GPIOA->IDR & 1) && debounce <= 0) 
	{
    photoOn ^= 1;
    debounce = 7;
  }
	
	getBuff = getChar();
	if(getBuff == 0x62) 
	{
		blueVal1++;
		if(blueVal1 == 11) blueVal1 = 1;
		buf[size] = 'b';
		size++;
		buf[size] = 0x0D;
		buf[size + 1] = 0x0A;
		size += 2;
	}
	if(getBuff == 0x67) 
	{
		greenVal1++;
		if(greenVal1 == 11) 
			greenVal1 = 1;
		buf[size] = 'g';
		size++;
		buf[size] = 0x0D;
		buf[size + 1] = 0x0A;
		size += 2;
	}
 }
 
 void printNote(int on, int note, uint64_t mil) 
{
  buf[size] 		= 'N';
	buf[size + 1] = 'o';
	buf[size + 2] = 't';
	buf[size + 3] = 'e';
	buf[size + 4] = ' ';
  size = size + 5;
	numIndex = size;
  do 
	{
    for(i = size; i > numIndex; i--)
      buf[i] = buf[i - 1];
    buf[numIndex] = (note % 10) + '0';
    size++;
    note = note / 10;
  } 
	while(note);
	
	if(on) 
	{
    buf[size]     = ' ';
    buf[size + 1] = 'o';
    buf[size + 2] = 'f';
    buf[size + 3] = 'f';
		buf[size + 4] = ' ';
    size += 5;
  }
  else 
	{
    buf[size]     = ' ';
    buf[size + 1] = 'o';
    buf[size + 2] = 'n';
    buf[size + 3] = ' ';
    size += 4;
  }
  numIndex = size;
	
  do 
	{
    for(i = size; i > numIndex; i--)
      buf[i] = buf[i - 1];
    buf[numIndex] = (mil % 10) + '0';
    size++;
    mil /= 10;
  } 
	while(mil);
	
  buf[size]     = ' ';
  buf[size + 1] = 'm';
  buf[size + 2] = 's';
  size += 3;

  buf[size] = 0x0D;
  buf[size + 1] = 0x0A;
  size += 2;
}

 void initAll() 
{
	initPorts();
  initHex();
  initSpeaker();
  initSpeaker2();
  initPhotocell();
  initButton();
  ADC1->CR |= (1<<2);
  initLed();
  SysTick_Config(SystemCoreClock / 1000);
  serialStart(9600);
}
