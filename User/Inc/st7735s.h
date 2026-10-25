#ifndef ST7735S_H_
#define ST7735S_H_

#include "main.h"
#include "spi.h"
#include "src/hal/lv_hal_disp.h"

// ============ 真实接线 ============
#define ST7735_CS_PIN    GPIO_PIN_2
#define ST7735_CS_PORT   GPIOA

#define ST7735_DC_PIN    GPIO_PIN_0
#define ST7735_DC_PORT   GPIOA

#define ST7735_BL_PIN    GPIO_PIN_3
#define ST7735_BL_PORT   GPIOA

#define ST7735_RST_PIN   GPIO_PIN_1
#define ST7735_RST_PORT  GPIOA

// 使用SPI1
extern SPI_HandleTypeDef hspi1;
extern uint8_t spi_dma_tx_busy;
extern lv_disp_drv_t *g_disp_drv;
#define ST7735_SPI_HANDLE  hspi1

//函数声明
void st7735_init(void);
void st7735_cs_low(void);
void st7735_cs_high(void);
void st7735_set_window(uint16_t x1, uint16_t y1, uint16_t x2, uint16_t y2);
void st7735_send_pixel_dma(uint8_t *data, uint32_t len);
void st7735_send_cmd(uint8_t cmd);
void st7735_send_byte(uint8_t dat);

#endif
