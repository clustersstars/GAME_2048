#include "lvgl_port.h"
#include "st7735s.h"

// 局部绘制缓冲区：128宽，10行，节约F103内存
static lv_disp_draw_buf_t draw_buf;
static lv_color_t buf[128 * 10];


void st7735_flush(lv_disp_drv_t * disp, const lv_area_t * area, lv_color_t * color_p)
{
    if(spi_dma_tx_busy)
    {
        return;
    }
    g_disp_drv = disp;
    st7735_set_window(area->x1, area->y1, area->x2, area->y2);
    st7735_cs_low();
    uint32_t pixel_cnt = (area->x2 - area->x1 + 1) * (area->y2 - area->y1 + 1);
    st7735_send_pixel_dma((uint8_t *)color_p, pixel_cnt * 2);
}

void lv_port_disp_init(void)
{
    // 初始化绘制buffer
    lv_disp_draw_buf_init(&draw_buf, buf, NULL, 128 * 10);

    static lv_disp_drv_t disp_drv;
    lv_disp_drv_init(&disp_drv);

    disp_drv.hor_res = 128;   // ST7735S宽度
    disp_drv.ver_res = 160;   // ST7735S高度
    disp_drv.draw_buf = &draw_buf;
    disp_drv.flush_cb = st7735_flush;

    lv_disp_drv_register(&disp_drv);
}

