# 🏠 桌面级智能环境监测与控制系统

[![Platform](https://img.shields.io/badge/Platform-STM32F103C8T6-blue)](https://www.st.com/en/microcontrollers-microprocessors/stm32f103c8.html)
[![Firmware](https://img.shields.io/badge/Firmware-STM32%20Std%20Periph%20Lib-green)](https://github.com/STMicroelectronics/STM32F10x_StdPeriph_Lib)
[![IDE](https://img.shields.io/badge/IDE-Keil%20MDK5-orange)](https://www.keil.com/)
[![License](https://img.shields.io/badge/License-MIT-yellow)](LICENSE)

> **一句话总结**：**把办公桌变成小型智能环境站** —— 温度高了自动吹风扇，天暗了自动亮灯，下雨/烟雾/有人靠近立即声光报警，所有数据实时显示在 OLED 上。

---

## 📦 目录

- [🚀 快速开始](#-快速开始)
- [🧰 硬件清单](#-硬件清单)
- [🧩 系统架构](#-系统架构)
- [⚙️ 软件特性](#️-软件特性)
- [📊 传感器与阈值](#-传感器与阈值)
- [🔌 Proteus 仿真](#-proteus-仿真)
- [🧪 二次开发](#-二次开发)
- [🤝 贡献](#-贡献)
- [📄 License](#-license)

---

## 🚀 快速开始

| 步骤 | 命令 / 操作 |
|------|-------------|
| 1. 克隆仓库 | `git clone https://github.com/yourname/Desk-Environment-Station.git` |
| 2. 打开工程 | 双击 `Project/MDK-ARM/DeskEnv.uvprojx` |
| 3. 编译下载 | Keil → **Build** → **Download** |
| 4. 仿真 | Proteus → **Run** → 查看 OLED & 串口输出 |

---

## 🧰 硬件清单

| 模块 | 型号 | 接口 | 功能 |
|------|------|------|------|
| MCU | STM32F103C8T6 | — | 主控 |
| OLED | 0.96" I²C 128×64 | I²C1 | 实时数据显示 |
| 温湿度 | DHT11 | GPIO | 温度/湿度 |
| 光照 | 光敏电阻 + 分压 | ADC1-CH1 | 环境光强 |
| 烟雾 | MQ-2 | ADC1-CH2 | 烟雾/可燃气体 |
| 雨滴 | 雨滴感应板 | GPIO-EXTI | 下雨检测 |
| 红外 | TCRT5000 | GPIO-EXTI | 遮挡/手势 |
| 风扇 | 5 V 直流 | TIM3-CH1 PWM | 自动散热 |
| LED | 3 mm × 3 | GPIO | 自动补光 |
| 蜂鸣器 | 有源 5 V | GPIO | 多源报警 |
| 窗帘 | 5 V 减速电机 | TIM2-CH1/CH2 | 升降帘 |

## ⚙️ 软件特性
- **多任务分时调度**：1 kHz SysTick → 100/200/500/1000 ms 任务队列  
- **模块化分层**  
  - `board/`：MCU 外设初始化  
  - `module/`：传感器与执行器驱动  
  - `app/`：业务逻辑  
- **一键仿真**：Proteus 8.15+ 模型 + 仿真脚本  
- **CLI 调试**：USART1 115200，menuconfig 式指令（`status`、`set thr 40`）

---

## 📊 传感器与阈值（可调）

| 传感器 | 阈值 | 触发动作 |
|--------|------|----------|
| 温度 | ≥ 30 ℃ | 风扇 PWM 占空比 80 % |
| 湿度 | > 70 % | 关闭风扇，打开窗帘 |
| 光照 | < 30 % | LED 点亮 |
| 烟雾 | > 60 % | 蜂鸣器 + 串口警报 |
| 雨滴 | 高电平 | 蜂鸣器 + 关窗 |
| 红外 | 低电平 | 蜂鸣器 + OLED 提示 |

> 所有阈值可通过串口 `set` 命令动态修改，掉电保存到 RTC Backup Register。

---

## 🔌 Proteus 仿真
1. 打开 `Simulation/DeskEnv.pdsprj`  
2. 运行仿真 → 虚拟串口 `VIRTUAL TERMINAL` 实时打印  
3. 鼠标点击 **雨滴/红外** 开关，观察 OLED & 蜂鸣器变化  

![Simulation Demo](doc/assets/proteus_demo.gif)

---

## 🧪 二次开发

| 需求 | 修改位置 |
|------|----------|
| 新增传感器 | `module/module_config.c` → `MODULE_InitXXX()` |
| 调整阈值 | `app/config.h` 或 串口 `set` 命令 |
| 更换屏幕 | `driver/oled/` 实现 `OLED_WriteByte()` |
| 接入 Wi-Fi | 预留 UART2，CubeMX 一键生成 ESP8266 AT 框架 |

---

---

## 🧩 系统架构

- **STM32F103C8T6**
  - **传感器输入**
    - DHT11 → 温湿度（GPIO）
    - 光敏电阻 → 光照（ADC）
    - MQ-2 → 烟雾（ADC）
    - 雨滴传感器 → 雨滴（EXTI）
    - 红外反射 → 遮挡（EXTI）
  - **任务调度器**
    - 时基：SysTick 1 kHz
    - 周期：100 ms / 200 ms / 500 ms / 1 s
  - **逻辑决策**
    - 阈值比较
    - 事件处理
  - **外设控制**
    - 风扇（PWM）
    - LED（GPIO）
    - 蜂鸣器（GPIO）
    - 窗帘电机（PWM）
  - **人机交互**
    - OLED（I²C）
    - USART（CLI）
