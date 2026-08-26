# 一个项目带你精通 STM32

> 用一个“多功能智能数据采集工作站”，从点灯一路做到采集、存储、回放、低功耗、状态机、通信协议和系统可靠性。

本仓库面向 STM32 零基础学习者。第一版默认硬件为 **野火 STM32F407ZGT6 开发板**，软件使用 **STM32CubeIDE + HAL 库**，不使用容器。业务代码与板级代码已经分开；如果你的开发板型号不同，主要修改 `Firmware/Port` 和 CubeMX 引脚配置即可。

## 从这里开始

1. 阅读 [环境搭建速查表](Docs/00-环境搭建速查表.md)。
2. 用 STM32CubeIDE 新建/生成一个 STM32F407ZGT6 HAL 工程。
3. 按 [固件接入说明](Firmware/README.md) 加入本仓库源码。
4. 从 [实训 0](Docs/01-实训0-入门点灯与中断.md) 开始，逐章完成。

## 教材与代码对应关系

| 章节 | 主题 | 主要代码 |
|---|---|---|
| 实训 0 | LED、按键、中断、烧录 | `Firmware/App/button.*`、`Firmware/Examples/lab0.c` |
| 实训 1 | 参数配置、消抖、EEPROM | `Firmware/App/config.*` |
| 实训 2 | ADC、滤波、SD 卡 | `Firmware/App/sampler.*`、`storage.*` |
| 实训 3 | 手动采样 | `Firmware/App/workstation.*` |
| 实训 4 | 数据回放 | `Firmware/App/storage.*` |
| 实训 5 | 低功耗、看门狗 | `Firmware/Port/board_port.h` |
| 实训 6 | 四状态状态机 | `Firmware/App/workstation.*` |
| 实训 7 | UART 协议 | `Firmware/App/protocol.*` |
| 实训 8 | DAC、DMA、系统集成 | `Firmware/Examples/lab8.c` |
| 实训 9 | 异常与恢复 | `Firmware/App/fault.*` |

完整目录见 [Docs/README.md](Docs/README.md)。先在电脑上运行 `make test`，可验证滤波、协议和状态机等不依赖硬件的逻辑。

## 第一版边界

- 仓库包含完整的教学业务代码和清晰的 HAL 接口，但 ST 官方 HAL、FatFs 中间件由 STM32CubeIDE/CubeMX 生成，不重复提交厂商代码。
- LED、按键、EEPROM、SD 卡片选和串口的具体引脚因野火板卡型号不同而不同，必须照你手中开发板原理图配置。
- 配图目前按照大纲保留占位，方便后续换成实际板卡照片和仪器截图。

## 目录

```text
Docs/                 教材正文，每个实训一章
Firmware/App/         与芯片型号无关的业务模块
Firmware/Port/        STM32 HAL 板级适配接口与参考实现
Firmware/Examples/    各阶段入口示例
Tests/                可在电脑上运行的逻辑测试
```

## 许可证

代码采用 MIT License；文档采用 CC BY-SA 4.0。详见 [LICENSE](LICENSE)。
