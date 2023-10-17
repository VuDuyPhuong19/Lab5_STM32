#include <stdint.h>
#include "SysTick.h"
#include "PLL.h"
#include "stm32f10x.h"

struct State {
	uint32_t out_traffic;
	uint32_t out_walk;
	uint32_t time;
	uint32_t next[8];
};
typedef const struct State State_t;

#define goS 0
#define waitS 1
#define allStop1 2
#define goW 3
#define waitW 4 
#define allStop2 5
#define walk 6
#define onred1 7
#define offred1 8
#define onred2 9
#define offred2 10
#define onred3 11
#define offred3 12
#define allStop3 13
State_t FSM[14] = {
	{0x008, 0x2000, 100, {goS,waitS,goS,waitS,waitS,waitS,waitS,waitS}},   										 					// goS
	{0x010, 0x2000, 20, {allStop1,allStop1,allStop1,allStop1,allStop1,allStop1,allStop1,allStop1}},			// waitS		
	{0x120, 0x2000, 10, {goW,goW,goS,goW,walk,goW,walk,goW}},																						// allStop1
	{0x040, 0x2000, 100, {goW,goW,waitW,waitW,waitW,waitW,waitW,waitW}},												 				// goW	
	{0x080, 0x2000, 20, {allStop2,allStop2,allStop2,allStop2,allStop2,allStop2,allStop2,allStop2}},			// waitW
	{0x120, 0x2000, 10, {goS,goW,goS,walk,walk,walk,walk,walk}},
	{0x120, 0x4000, 100, {walk,onred1,onred1,onred1,walk,onred1,onred1,onred1}},
	{0x120, 0x2000, 5, {offred1,offred1,offred1,offred1,offred1,offred1,offred1,offred1}},
	{0x120, 0x00, 5, {onred2,onred2,onred2,onred2,onred2,onred2,onred2,onred2}},
	{0x120, 0x2000, 5, {offred2,offred2,offred2,offred2,offred2,offred2,offred2,offred2}},
	{0x120, 0x00, 5, {onred3,onred3,onred3,onred3,onred3,onred3,onred3,onred3}},
	{0x120, 0x2000, 5, {offred3,offred3,offred3,offred3,offred3,offred3,offred3,offred3}},
	{0x120, 0x00, 5, {allStop3,allStop3,allStop3,allStop3,allStop3,allStop3,allStop3,allStop3}},
	{0x120, 0x2000, 10, {goS,goW,goS,goS,walk,goW,goS,goS}}																			 					// allStop2
};

// switches A0(west),A1(south),A2(walk)
// walklight C13(red),C14(green)
// trafficlight B3(green_south),B4(yellow_south),B5(red_south),B6(green_west),B7(yellow_west),B8(red_west)
int main(void){
	PLL_Init();
	SysTick_Init();
	// Initialize GPIO ports 
	RCC->APB2ENR |=  RCC_APB2ENR_IOPAEN | RCC_APB2ENR_IOPBEN | RCC_APB2ENR_IOPCEN; // PortA,PortB
	
	// set A0,A1,A2 as switches
	GPIOA->CRL &= ~(0x00000FFF);
	GPIOA->CRL |= 0x8;
	//GPIOA->BSRR = GPIO_BSRR_BS0;
	// set C13,C14 as walk light output
	GPIOC->CRH &= ~(GPIO_CRH_MODE13 | GPIO_CRH_CNF13 | GPIO_CRH_MODE14 | GPIO_CRH_CNF14);
	GPIOC->CRH |= GPIO_CRH_MODE13_0 | GPIO_CRH_MODE14_0;
	
	// set B3,B4,B5,B6,B7,B8 as trafic light output
	GPIOB->CRL &= ~(0xFFFFF000);
	GPIOB->CRH &= ~(0x0000000F);
	GPIOB->CRL |= GPIO_CRL_MODE3_0 | GPIO_CRL_MODE4_0 | GPIO_CRL_MODE5_0 | GPIO_CRL_MODE6_0 | GPIO_CRL_MODE7_0;
	GPIOB->CRH |= GPIO_CRH_MODE8_0;
	// Initialize state
	uint32_t S;
	uint32_t Input;
	S = goS;
	// Ð?c giá tr? t? chân A0
	while(1){
   

// Ki?m tra tr?ng thái c?a nút b?m
if ((GPIOA->IDR & 0x001) != 0) {
            // Nút b?m dã du?c nh?n, b?t dèn
            GPIOB->ODR = GPIOB->ODR & (1<<5); 
				}
else GPIOB->BRR = GPIOB->BRR & (1<<5); 
	
/*		GPIOB->ODR = 1 << 5;
		SysTick_Wait10ms(100);
		GPIOB->BRR = 1 << 5;
 		SysTick_Wait10ms(100);
*/
}
	
	
/*	while(1){
	// set traffic light
	GPIOB->ODR = (GPIOB->ODR & ~(0x1F8)) | FSM[S].out_traffic;
		
	// set walk light
	GPIOC->ODR = (GPIOC->ODR & ~(0x6000)) | FSM[S].out_walk;
		
	// wait 
	SysTick_Wait10ms(FSM[S].time);
	
	// read input
	Input = GPIOA->IDR & 0x07;
		
	// next state depends on input and current state
	S = FSM[S].next[Input];
		
}*/
}	