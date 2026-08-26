# 固件接入 STM32CubeIDE

本目录不提交 ST 厂商自动生成的 HAL 和 FatFs 文件。这样仓库更小，也避免把某一版本的生成文件冒充为所有野火板卡都通用。

## 接入步骤

1. 按 `Docs/00-环境搭建速查表.md` 用真实 MCU 新建 HAL 工程。
2. CubeMX 开启 GPIO、ADC1、I2C1、SPI/FatFs、USART1、TIM2、IWDG；实训 8 再开启 DAC1/TIM6/DMA。
3. 给引脚设置用户标签：`LED`、`KEY`、`BUZZER`、`SD_CS`。生成后会得到 `LED_Pin`、`LED_GPIO_Port` 等宏。
4. 把 `Firmware/App` 和 `Firmware/Port` 作为 Source Location 加入项目，把这两个目录加入 MCU GCC Compiler 的 Include paths。
5. `storage.c` 依赖 CubeMX 生成的 `fatfs.h`/`ff.h`，在实训 2 之前可先 Exclude from Build。
6. 在生成的 `main.c` 的 `USER CODE BEGIN 2` 中调用相应 `labX_init()`，在 `while (1)` 的用户代码区调用 `labX_loop()`。
7. 将 `HAL_GPIO_EXTI_Callback`、`HAL_TIM_PeriodElapsedCallback`、UART 接收回调按章节示例连接到事件入口。

## 必须核对的板级差异

- `board_port_stm32f407_hal.c` 默认 LED/蜂鸣器高电平有效、KEY 低电平按下。
- EEPROM 默认是 I2C 地址 `0x50`、16 位存储地址的 AT24Cxx。若板上没有 EEPROM，可外接模块，或另写片内 Flash 适配。
- TIM2 的时钟推导按 APB1 定时器时钟为 PCLK1×2；修改时钟树后要重新核对。
- FatFs 的底层磁盘驱动必须选择与你硬件一致的 SPI SD 驱动。
- STOP 唤醒后必须重配系统时钟；参考实现会调用 CubeMX 生成的 `SystemClock_Config()`。

## 中断原则

中断里只设置标志，不做 SD 卡写入、串口长时间发送或延时。主循环读取标志并完成耗时工作。这是避免“偶尔卡死”的第一条规则。
