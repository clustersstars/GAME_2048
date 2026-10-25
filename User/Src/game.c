#include "game.h"
#include "main_layout.h"
#include "game_layout.h"
#include "checkout_layout.h"
#include "stm32f1xx_hal.h"
uint16_t key_trig_pin = 0;
uint32_t key_trig_tick = 0;

extern ADC_HandleTypeDef hadc1;

GPIO_TypeDef *GPIO_X = NULL;
static uint8_t key_event = 0;
layout_manage_t layout_manage = {0};
static uint32_t get_adc_seed(void);

void layoutInit(void) {
    //取随机不用校准
    srand(get_adc_seed());
    HAL_ADC_Stop(&hadc1);
    mainLayoutInit();
    gameLayoutInit();
    checkoutLayoutInit();
}
void switchLayout(layout_obj_t *layout_obj,uint8_t child_flag) {
    if (layout_manage.active_layout_obj != NULL) {
      layout_manage.active_layout_obj->freeChildELm();
    }
    //生成子元素
    layout_obj->generateChildELm(child_flag);
    lv_scr_load(layout_obj->obj);
    layout_manage.active_layout_obj = layout_obj;
}
void keyEventListening(void) {
    uint32_t now_tick = HAL_GetTick();
    if(key_trig_pin != 0)
    {
      // 非阻塞判断：距离中断触发 >=20ms消抖时间
      if(now_tick - key_trig_tick >= 20)
      {
        if(HAL_GPIO_ReadPin(GPIO_X, key_trig_pin) == GPIO_PIN_RESET)
        {
          switch(key_trig_pin)
          {
            case GPIO_PIN_11:  key_event = KEY_A; break;
            case GPIO_PIN_12:  key_event = KEY_B; break;
            case GPIO_PIN_13:  key_event = KEY_C; break;
            case GPIO_PIN_14:  key_event = KEY_D; break;
            case GPIO_PIN_15:  key_event = KEY_E; break;
            case GPIO_PIN_8:   key_event = KEY_F; break;
            default: break;
          }
        }
        key_trig_pin = 0;  // 清除临时触发标记，等待下一次下降沿
      }
    }
    // 处理按键事件
    if(key_event != 0)
    {
      layout_manage.active_layout_obj->keyEventHandler(key_event);
      key_event = 0; // 事件消费完成，清零
    }
}

static uint32_t get_adc_seed(void)
{
  uint32_t seed = 0;
  uint16_t adc_val;
  // 多次采样，积累噪声
  HAL_ADCEx_Calibration_Start(&hadc1);
  for(uint8_t i = 0; i < 16; i++)
  {
    HAL_ADC_Start(&hadc1);
    if(HAL_ADC_PollForConversion(&hadc1, 10) == HAL_OK)
    {
      adc_val = HAL_ADC_GetValue(&hadc1);
      // 只拿最低2bit，噪声集中在这里，异或搅拌
      seed ^= (adc_val & 0x03);
      seed = (seed << 2) | (adc_val & 0x03);
    }
    HAL_ADC_Stop(&hadc1);
  }
  return seed;
}