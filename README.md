# GAME_2048
基于 STM32F103 + LVGL 8.3 + ST7735S 的嵌入式2048小游戏，SPI-DMA驱动屏幕，适合嵌入式学习练手。

## 📦 硬件
- 主控：STM32F103ZET6
- 屏幕：ST7735S 1.8寸 TFT 128×160
- 通信：SPI1 + DMA
- 保护：IWDG独立看门狗

## ✨ 功能
- 标准2048游戏逻辑，方块合并、移动
- 暖色调方块配色，数字自动适配字号
- 胜利、游戏结束判定
- ADC噪声作为随机种子生成新方块
- LVGL分片渲染，小缓冲区节省内存
- IWDG硬件看门狗，卡死/栈溢出自动复位

## 🧰 依赖
- LVGL 8.3.x
- STM32 HAL库
- CMake
- STM32CubeCLT / Arm GNU Toolchain
- CLion

## 🚀 编译烧录
1. CLion打开工程，配置STM32CubeCLT工具链
2. CMake配置，构建生成 `game_2048.elf`
3. 下载 bin/elf 到单片机，上电运行
> 调试阶段建议关闭IWDG，正式版本开启看门狗兜底

## ⚙️ 驱动要点
- ST7735S：命令阻塞发送，像素使用DMA传输，降低CPU占用
- IWDG：2s超时，仅主循环喂狗，中断内不喂狗
- 随机源：ADC悬空引脚噪声做熵源，芯片无硬件RNG

## License
MIT
