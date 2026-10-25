#include "st7735s.h"


extern SPI_HandleTypeDef hspi1;
uint8_t spi_dma_tx_busy = 0;
lv_disp_drv_t *g_disp_drv = NULL;
#define ST7735_SPI_HANDLE  hspi1

// 拉低CS
void st7735_cs_low(void)
{
    HAL_GPIO_WritePin(ST7735_CS_PORT, ST7735_CS_PIN, GPIO_PIN_RESET);

}
// 拉高CS
void st7735_cs_high(void)
{
    HAL_GPIO_WritePin(ST7735_CS_PORT, ST7735_CS_PIN, GPIO_PIN_SET);
}
// DC=0 命令 DC=1 数据
static void st7735_dc_cmd(void)
{
    HAL_GPIO_WritePin(ST7735_DC_PORT, ST7735_DC_PIN, GPIO_PIN_RESET);
}
static void st7735_dc_data(void)
{
    HAL_GPIO_WritePin(ST7735_DC_PORT, ST7735_DC_PIN, GPIO_PIN_SET);
}

void st7735_send_byte(uint8_t dat)
{
    // HAL_SPI_Transmit_DMA(&ST7735_SPI_HANDLE, &dat, 1);
    HAL_SPI_Transmit(&ST7735_SPI_HANDLE, &dat, 1, HAL_MAX_DELAY);
}

void st7735_send_cmd(uint8_t cmd)
{
    st7735_cs_low();
    st7735_dc_cmd();
    st7735_send_byte(cmd);
    st7735_cs_high();
}
static void st7735_send_data_block(uint8_t *data, uint32_t len)
{
    st7735_cs_low();
    st7735_dc_data();
    HAL_SPI_Transmit(&ST7735_SPI_HANDLE, data, len, HAL_MAX_DELAY);
    st7735_cs_high();
}

void st7735_send_pixel_dma(uint8_t *data, uint32_t len)
{
    if(spi_dma_tx_busy)
    {
        return; // 上一轮还没发完，拒绝新传输，防止DMA重入
    }
    spi_dma_tx_busy = 1;
    st7735_dc_data();
    HAL_SPI_Transmit_DMA(&ST7735_SPI_HANDLE, data, len);
}

void st7735_set_window(uint16_t x1, uint16_t y1, uint16_t x2, uint16_t y2)
{
    x1 += 2;
    x2 += 2;
    y1 += 1;
    y2 += 1;

    st7735_send_cmd(0x2A); // CASET 列地址
    uint8_t buf[4];
    buf[0] = (x1 >> 8) & 0xFF;
    buf[1] = x1 & 0xFF;
    buf[2] = (x2 >> 8) & 0xFF;
    buf[3] = x2 & 0xFF;
    st7735_send_data_block(buf,4);

    st7735_send_cmd(0x2B); // RASET 行地址
    buf[0] = (y1 >> 8) & 0xFF;
    buf[1] = y1 & 0xFF;
    buf[2] = (y2 >> 8) & 0xFF;
    buf[3] = y2 & 0xFF;
    st7735_send_data_block(buf,4);

    st7735_send_cmd(0x2C); // 准备写像素RAM
}

// 屏幕初始化序列，ST7735S 128*160
void st7735_init(void)
{
    // 复位RST
    HAL_GPIO_WritePin(ST7735_RST_PORT, ST7735_RST_PIN, GPIO_PIN_RESET);
    HAL_Delay(100);
    HAL_GPIO_WritePin(ST7735_RST_PORT, ST7735_RST_PIN, GPIO_PIN_SET);
    HAL_Delay(100);

    st7735_send_cmd(0x01); // SW reset
    HAL_Delay(150);
    st7735_send_cmd(0x11); // Sleep out
    HAL_Delay(50);

    st7735_send_cmd(0x3A); // 颜色模式 16bit RGB565
    st7735_send_data_block((uint8_t[]){0x05},1);

    st7735_send_cmd(0x29); // 开显示
}

void HAL_SPI_TxCpltCallback(SPI_HandleTypeDef *hspi)
{
    if(hspi->Instance == ST7735_SPI_HANDLE.Instance)
    {
        spi_dma_tx_busy = 0;
        st7735_cs_high();
        if(g_disp_drv != NULL)
        {
            lv_disp_flush_ready(g_disp_drv);
        }
    }
}