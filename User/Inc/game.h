#ifndef GAME_H_
#define GAME_H_
#include "game_def.h"
#include "stm32f103xe.h"

extern uint16_t key_trig_pin;
extern uint32_t key_trig_tick;
extern GPIO_TypeDef *GPIO_X;
void layoutInit(void);
void keyEventListening(void);

#endif