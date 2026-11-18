# 两轮循迹平衡小车 · 固件源码

基于 STM32F103C8T6 的自平衡 + 循迹小车固件，Keil MDK 工程。

对应作品集页面：[两轮循迹平衡小车](../../projects/balance-car.html)

## 硬件

| 模块 | 型号 |
|:--|:--|
| 主控 | STM32F103C8T6 |
| 姿态 | MPU6050（移植 DMP 库做姿态解算） |
| 电机驱动 | MP1584EN 降压 + 双路直流电机 |
| 循迹 | 五路红外 |
| 显示 | 0.96 寸 OLED（I2C） |
| 通信 | ESP8266（WiFi） |
| 扩展 | 超声波、舵机、蜂鸣器、按键 |

## 目录

```
balance-car/
├── Users/         main.c、中断服务
├── HardWare/      外设驱动：电机、编码器、MPU6050、OLED、PID、循迹、超声波…
├── Systems/       延时、定时器、串口、Flash、RTC
├── Start/         启动文件与 CMSIS
├── Libraries/     ST 标准外设库
└── Project.uvprojx
```

## 编译

1. Keil MDK 5（需要 STM32F1 器件包）
2. 打开 `Project.uvprojx`
3. Build，输出 `Project.hex`

工程内全部使用相对路径（`.\Start\...`），换机器不用改配置。

## 实现要点

- **双环 PID**：纵向速度环 + 横向平衡环，`HardWare/PID.c`
- **姿态解算下沉**：MPU6050 的 DMP 直接输出四元数，主控只做控制
- **模块分层**：外设驱动 / 系统服务 / 应用逻辑分开，便于单独调试
- **自设计 PCB**：集成电源与电机驱动，减少杜邦线接触问题

## 说明

个人项目，代码供参考。烧录前请按自己的硬件核对引脚定义（`HardWare/` 下各驱动文件顶部）。
