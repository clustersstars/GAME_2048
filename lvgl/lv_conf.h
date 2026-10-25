//
// Created by 23057 on 2026/10/13.
//
#if 1
#ifndef LV_CONF_H
#define LV_CONF_H

#include <stdint.h>

/*====================    COLOR SETTINGS  *====================*/
#define LV_COLOR_DEPTH     16
#define LV_COLOR_16_SWAP   1

/*====================    MEMORY SETTINGS  *====================*/
// 【修改这里】调到 32KB！STM32F103 一般有20KB以上RAM，32KB稳妥
#define LV_MEM_SIZE        (32U * 1024U)

/*====================    DEMO USAGE  *====================*/
#define LV_USE_DEMO_WIDGETS        0
#define LV_USE_DEMO_BENCHMARK      0
#define LV_USE_DEMO_MUSIC          0

/*====================    EXTRA COMPONENTS  *====================*/
#define LV_USE_EXTRA_WIDGETS       0
#define LV_USE_GIF                 0

#define LV_USE_IMG_TRANSFORM 1   //图片缩放旋转
#define LV_USE_ANIMATION 1       //动画总开关

//字体
#define LV_FONT_MONTSERRAT_8  0
#define LV_FONT_MONTSERRAT_10 1
#define LV_FONT_MONTSERRAT_12 1
#define LV_FONT_MONTSERRAT_14 1
#define LV_FONT_MONTSERRAT_16 0
#define LV_FONT_MONTSERRAT_18 0
#define LV_FONT_MONTSERRAT_20 0
#define LV_FONT_MONTSERRAT_22 0
#define LV_FONT_MONTSERRAT_24 1
#define LV_FONT_MONTSERRAT_26 0
#define LV_FONT_MONTSERRAT_28 0

#endif /*LV_CONF_H*/

// 渲染模式和缓冲区大小，放在lv_conf外面，属于底层驱动配置
#define LV_DISPLAY_RENDER_MODE_PARTIAL 1
#define LV_DISPLAY_BUFFER_SIZE    (128 * 80)

#endif /*End of "#if 1"*/

