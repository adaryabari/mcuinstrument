#include "header.h"

int row = 0;
int col = 0;

void initHex() 
{
  GPIOC->MODER |= (0x55); 
  GPIOC->PUPDR |= (0x55 << 8);
}

uint8_t bPress() 
{
  GPIOC->BSRR |= (0xf);
  for(row = 0; row < 4; row++) 
  {
    GPIOC->BRR |= (1 << row);
    
    for(col = 0; col < 4; col++)
    	if(~GPIOC->IDR & (1 << (4 + col))) 
    	{
      		GPIOC->BSRR |= (1 << row);
      		return (row * 4) + col + 1;
    	}
    
    GPIOC->BSRR |= (1 << row );
  }
  return 0;
}

void initSpeaker() 
{
  RCC->APB2ENR  |= 	(1 << 11);
  TIM1->PSC 	 = 	48;
  TIM1->CCER 	|= 	(0x55);
  TIM1->CCMR1   |= 	(0x60);
  TIM1->CCMR1   |= 	(0x60 << 8);
  TIM1->BDTR 	|= 	(1 << 15);
  GPIOA->MODER  |= 	(0xA << 16);
  GPIOA->AFR[1] |= 	(0x22);
}

void speakerOn(uint16_t period, uint16_t duty1, uint16_t duty2) 
{
  if((TIM1->CCR1)<(TIM1->ARR) && !(TIM1->CCR1 == 0)) 
  	return;
  period = ((SystemCoreClock / 48) / (period)) - 1;
  TIM1->ARR = period;
  TIM1->CCR1 = (duty1 * period / 100);
  TIM1->CCR2 = (duty2 * period / 100);
  TIM1->CR1 |= (1<<0);
}

void initSpeaker2() 
{
  RCC->APB1ENR  |= (1 << 1);
  TIM3->PSC 	 = 48;
  TIM3->CCER 	|= (0x01);
  TIM3->CCMR1 	|= (0x60);
  GPIOB->MODER 	|= (2 << 8);
  GPIOB->AFR[0] |= (1 << 16);
}

void speaker2On(uint16_t period) 
{
  period = ((SystemCoreClock / 48) / (period)) - 1;
  if(TIM3->CNT > period) 
  	return;
  TIM3->ARR = period;
  TIM3->CCR1 = (50 * period / 100);
  TIM3->CR1 |= (1);
}

void serialStart(uint16_t baud) 
{
  RCC->APB2ENR |= (1 << 14); 
  initPin('B', 7, 2);    	 //RX
  initPin('B', 6, 2);    	 //TX
  GPIOA->AFR[0] |= (0x00 << 24);
  baud = SystemCoreClock / baud;
  USART1->BRR = baud;
  USART1->CR1 |= 0xD;
}

uint8_t getChar() 
{
  if(USART1->ISR & (1 << 5)) 
  	return USART1->RDR;
  return 0xFF;
}

int putChar(uint8_t txChar) 
{
  if(!(USART1->ISR & (1 << 7))) 
  	return 0;
  USART1->TDR = txChar;
  return 1;
}

void initButton() 
{
  GPIOA->MODER  &= ~3;
  GPIOA->OTYPER &= ~3;
}

void initPhotocell() 
{
  initPin('A', 1, 3);
  RCC->APB2ENR |= (1 << 9);
  ADC1->CR     |= (1);
  ADC1->CHSELR |= (1 << 1);
}

void pinHigh(char port, int pin) 
{
  if(port == 'A')
  	GPIOA->BSRR |= (1 << pin);
  if(port == 'B')
  	GPIOB->BSRR |= (1 << pin);
  if(port == 'C')
  	GPIOC->BSRR |= (1 << pin);
  if(port == 'D')
  	GPIOD->BSRR |= (1 << pin);
  if(port == 'F')
  	GPIOF->BSRR |= (1 << pin);
}

void pinLow(char port, int pin) 
{
  if(port == 'A')
  	GPIOA->BRR |= (1 << pin);
  if(port == 'B')
  	GPIOB->BRR |= (1 << pin);
  if(port == 'C')
  	GPIOC->BRR |= (1 << pin);
  if(port == 'D')
  	GPIOD->BRR |= (1 << pin);
  if(port == 'F')
  	GPIOF->BRR |= (1 << pin);
}

void initPin(char port, int pin, int mode) 
{
  if(port == 'A')
  	GPIOA->MODER |= (mode << pin * 2);
  if(port == 'B')
  	GPIOB->MODER |= (mode << pin * 2);
  if(port == 'C')
  	GPIOC->MODER |= (mode << pin * 2);
  if(port == 'D')
  	GPIOD->MODER |= (mode << pin * 2);
  if(port == 'F')
  	GPIOF->MODER |= (mode << pin * 2);
}

void initPorts()
{
  RCC->AHBENR |= (1 << 17); // Enable Port A
  RCC->AHBENR |= (1 << 18); // Enable Port B
  RCC->AHBENR |= (1 << 19); // Enable Port C
}

void initLed() 
{
  RCC->AHBENR  |= (1 << 19);
  GPIOC->MODER |= (1 << (8 * 2)) | (1 << (9 * 2));
}
