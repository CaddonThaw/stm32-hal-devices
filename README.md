# stm32-hal-devices

适配 STM32 HAL 库的常用外设/传感器模块，提供统一的配置入口与简单的使用接口。

## 📑 目录

- [📖 项目简介](#-项目简介)

- [📁 项目结构](#-项目结构)

- [🚀 快速开始](#-快速开始)

  - [1. 安装STM32CubeIDE](#1-安装stm32cubeide)
  - [2. 新建并配置工程](#2-新建并配置工程)
  - [3. 导入Devices库](#3-导入devices库)

- [⚙️ 模块配置](#️-模块配置)
  
  - [1. TB6612 电机驱动模块](#1-tb6612-电机驱动模块)
  - [2. Encoder 编码器模块](#2-encoder-编码器模块)
  - [3. MPU6050 六轴传感器模块](#3-mpu6050-六轴传感器模块)
  - [4. OLED 显示模块](#4-oled-显示模块)

## 📖 项目简介

本项目为 STM32 HAL 库提供了一套通用的外设/传感器驱动模块。所有模块通过统一的 `config.h` 配置文件进行管理，支持模块化使能和硬件映射，方便在不同 STM32 项目中快速集成和使用。

## 📁 项目结构

```
stm32-hal-devices/
├── config.h                # 统一配置文件（硬件映射与模块使能）
├── TB6612/                 # TB6612 双路电机驱动模块
├── Encoder/                # 增量式编码器模块
├── MPU6050/                # MPU6050 六轴传感器模块
├── OLED/                   # I2C OLED 显示模块
├── images/
├── README.md              
└── LICENSE
```

## 🚀 快速开始

### 1. 安装STM32CubeIDE

- 下载并安装：[STM32CubeIDE](https://www.st.com.cn/zh/development-tools/stm32cubeide.html)。

  **注意**：选择安装**1.19.0**版本的。

  ![2](images/2.png)

- 配置及汉化教程：[STM32CubeIDE配置及汉化教程](https://blog.csdn.net/c858845275/article/details/144321873)。

### 2. 新建并配置工程

- 选择 `文件` ->  `新建` -> `STM32 Project`。

- 选择芯片型号 `STM32F103C8T6`（以stm32f103c8t6为例），点击下一步。

  ![3](images/3.png)

- 填写项目名称 `test`，其他选项保持默认，点击完成。

  ![4](images/4.png)

- 打开 `Pinout & Configuration` 选项卡：

  - 点击 `RCC`，将 `HSE` 设置为 `Cystal/Ceramic Resonator`。

    ![5](images/5.png)

  - 点击 `SYS`，将 `DEBUG` 设置为 `Serial Wire`。

    ![6](images/6.png)

- 打开 `Clock Configuration` 选项卡：

  - 选择 `HSE` 频率为 `8MHz`。
  - 将 `SYSCLK` 设置为 `72MHz`。
    ![1](images/1.png)

- 打开 `Project Manager` 选项卡，进入 `Code Generator` 选择框，勾选：

  ![7](images/7.png)

- 按 `Ctrl + S` 生成代码，点击上方工具栏中的🔨图标编译无报错则表示生成的工程没有问题。

  ![8](images/8.png)

### 3. 导入Devices库

- 右键点击工程 -> 点击 `显示位置` -> 点击 `系统资源管理器`。

  ![9](images/9.png)

- 右键打开终端，输入以下命令克隆当前工程导入Devices库：

  ```bash
  git clone https://github.com/CaddonThaw/stm32-hal-devices.git Devices
  ```

  ![10](images/10.png)

- 刷新工程，点击 `项目` -> 点击 `属性` -> 点击 `C/C++ 常规` -> 点击 `路径和符号` -> 在 `包含` 和 `源位置` 中添加Devices库路径。

  ![11](images/11.png)

  ![12](images/12.png)

- 点击 `应用并关闭` -> 点击工具栏🔨图标编译无报错则成功导入Devices库。

## ⚙️ 模块配置

### 1. TB6612 电机驱动模块

TB6612FNG 是一款双路直流电机驱动芯片，支持 PWM 调速和方向控制。

#### 硬件连接

| TB6612引脚 | STM32引脚 | 说明 |
|-----------|----------|------|
| PWMA | TIM2_CH1 | 电机A的PWM信号 |
| AIN1 | GPIO输出 | 电机A方向控制1 |
| AIN2 | GPIO输出 | 电机A方向控制2 |
| PWMB | TIM2_CH2 | 电机B的PWM信号 |
| BIN1 | GPIO输出 | 电机B方向控制1 |
| BIN2 | GPIO输出 | 电机B方向控制2 |
| VM | 电机电源 | 电机驱动电压（建议12V） |
| VCC | 3.3V/5V | 逻辑电平 |
| GND | GND | 公共地 |

#### CubeMX 配置

**定时器配置（PWM输出）：**

- 选择 TIM2（或其他定时器）
- Channel 1/2 设置为 **PWM Generation CHx**
- 配置 PWM 频率（推荐 1kHz-20kHz）
- PSC（预分频器）：根据时钟频率设置
- ARR（自动重装载值）：决定 PWM 分辨率，例如 999（0-999 共1000级）

**GPIO配置：**
- 设置 AIN1、AIN2、BIN1、BIN2 为 **GPIO_Output**
- 输出模式：Push-Pull
- 上拉/下拉：No pull-up and no pull-down
- 初始电平：Low

#### config.h 配置

```c
#define DEVICE_TB6612 1
#if DEVICE_TB6612
    #include "gpio.h"
    #include "tim.h"
    
    // PWM定时器和通道
    #define TB6612_PWMA_TIM         htim2
    #define TB6612_PWMA_CH          TIM_CHANNEL_1
    #define TB6612_PWMB_TIM         htim2
    #define TB6612_PWMB_CH          TIM_CHANNEL_2
    
    // 电机方向修正（1为正向，-1为反向）
    #define TB6612_DIRA             1
    #define TB6612_DIRB             1
    
    // GPIO控制宏（CubeMX生成的引脚名称）
    #define AIN1(x)                 HAL_GPIO_WritePin(TB6612_AIN1_GPIO_Port, TB6612_AIN1_Pin, (x))
    #define AIN2(x)                 HAL_GPIO_WritePin(TB6612_AIN2_GPIO_Port, TB6612_AIN2_Pin, (x))
    #define BIN1(x)                 HAL_GPIO_WritePin(TB6612_BIN1_GPIO_Port, TB6612_BIN1_Pin, (x))
    #define BIN2(x)                 HAL_GPIO_WritePin(TB6612_BIN2_GPIO_Port, TB6612_BIN2_Pin, (x))
#endif
```

#### API 接口

```c
void TB6612_Init(void);                        // 初始化（启动PWM）
void TB6612_Motor(int16_t motora, int16_t motorb); // 控制电机（正值正转，负值反转）
```

#### 使用示例

```c
TB6612_Init();

// 两个电机都以500的速度正转（范围：-999~999）
TB6612_Motor(500, 500);

// 左电机正转，右电机反转（原地转向）
TB6612_Motor(300, -300);

// 停止
TB6612_Motor(0, 0);
```

---

### 2. Encoder 编码器模块

支持增量式编码器速度读取，使用 STM32 定时器的编码器模式。

#### 硬件连接

| 编码器引脚 | STM32引脚 | 说明 |
|-----------|----------|------|
| A相（左轮） | TIM3_CH1 | 编码器A相信号 |
| B相（左轮） | TIM3_CH2 | 编码器B相信号 |
| A相（右轮） | TIM1_CH1 | 编码器A相信号 |
| B相（右轮） | TIM1_CH2 | 编码器B相信号 |
| VCC | 3.3V/5V | 电源 |
| GND | GND | 公共地 |

#### CubeMX 配置

**定时器配置（编码器模式）：**

- 选择 TIM1 和 TIM3（或其他支持编码器模式的定时器）
- Combined Channels 设置为 **Encoder Mode**
- Encoder Mode 选择 **Encoder Mode TI1 and TI2**
- Counter Period（ARR）设置为 **65535**（最大值，防止溢出）
- Polarity 设置为 **Rising Edge**

#### config.h 配置

```c
#define DEVICE_ENCODER  1
#if DEVICE_ENCODER
    #include "tim.h"
    
    // 编码器定时器
    #define Encoder_Left            htim3
    #define Encoder_Right           htim1
    
    // 机械参数
    #define ENCODER_WHEEL_DIAMETER  0.065f      // 轮径（米）
    #define ENCODER_PULSE_PER_REV   1470.0f     // 每转脉冲数（4倍频）
    #define ENCODER_PI              3.1415926f
#endif
```

**参数说明：**
- `ENCODER_WHEEL_DIAMETER`：车轮直径（米），用于计算线速度
- `ENCODER_PULSE_PER_REV`：编码器每转一圈的脉冲数（4倍频后），例如：
  - 如果编码器标称 11 线，则 4 倍频后为 44 脉冲
  - 如果带减速器，需乘以减速比，例如 44 × 33.5 ≈ 1474

#### API 接口

```c
void Encoder_Init(void);                       // 初始化（启动编码器）
void Encoder_Read(float *left, float *right);  // 读取左右轮速度（m/s）
```

#### 使用示例

```c
Encoder_Init();

float left_speed, right_speed;

// 在定时器回调中读取速度（例如每10ms读取一次）
void HAL_TIM_PeriodElapsedCallback(TIM_HandleTypeDef *htim)
{
    if (htim->Instance == TIM6)  // 假设TIM6为10ms定时器
    {
        Encoder_Read(&left_speed, &right_speed);
        printf("Left: %.3f m/s, Right: %.3f m/s\n", left_speed, right_speed);
    }
}
```

---

### 3. MPU6050 六轴传感器模块

MPU6050 是一款集成三轴陀螺仪和三轴加速度计的 MEMS 传感器，支持 DMP（数字运动处理器）进行姿态解算。

#### 硬件连接

| MPU6050引脚 | STM32引脚 | 说明 |
|------------|----------|------|
| VCC | 3.3V | 电源（注意：不能接5V） |
| GND | GND | 公共地 |
| SCL | I2C2_SCL | I2C时钟线 |
| SDA | I2C2_SDA | I2C数据线 |
| AD0 | GND | I2C地址选择（接GND为0x68） |
| INT | GPIO（可选） | 中断输出（DMP数据就绪） |

#### CubeMX 配置

**I2C配置：**
- 选择 I2C2（或其他I2C接口）
- I2C Speed Mode 设置为 **Standard Mode**（100kHz）或 **Fast Mode**（400kHz）
- 启用 I2C 外设时钟
- 配置 SCL 和 SDA 引脚为 **I2C2_SCL** 和 **I2C2_SDA**
- GPIO模式：Open-Drain，上拉电阻

#### config.h 配置

```c
#define DEVICE_MPU6050  1
#if DEVICE_MPU6050
    #include "i2c.h"
    
    // I2C接口
    #define MPU6050_I2C             hi2c2
#endif
```

#### API 接口

```c
void MPU6050_Init(void);                       // 基础初始化（寄存器配置）
uint8_t MPU6050_GetID(void);                   // 获取设备ID（正常为0x68）

// 原始数据读取
void MPU6050_GetData(int16_t *AccX, int16_t *AccY, int16_t *AccZ,
                     int16_t *GyroX, int16_t *GyroY, int16_t *GyroZ);

// DMP姿态解算
int MPU6050_DMP_Init(void);                    // DMP初始化（返回0成功）
int MPU6050_DMP_ReadEuler(float *pitchDeg, float *rollDeg, float *yawDeg); // 读取欧拉角（度）
float MPU6050_DMP_GetROSGyroz(void);           // 获取Z轴角速度（rad/s，ROS格式）
```

#### 使用示例

```c
// 方式1：读取原始数据
MPU6050_Init();
uint8_t id = MPU6050_GetID();
printf("MPU6050 ID: 0x%02X\n", id);  // 应为0x68

int16_t ax, ay, az, gx, gy, gz;
MPU6050_GetData(&ax, &ay, &az, &gx, &gy, &gz);
printf("Acc: %d, %d, %d | Gyro: %d, %d, %d\n", ax, ay, az, gx, gy, gz);

// 方式2：使用DMP姿态解算
if (MPU6050_DMP_Init() == 0)
{
    printf("DMP initialized successfully!\n");
    
    while (1)
    {
        float pitch, roll, yaw;
        if (MPU6050_DMP_ReadEuler(&pitch, &roll, &yaw) == 0)
        {
            printf("Pitch: %.2f° Roll: %.2f° Yaw: %.2f°\n", pitch, roll, yaw);
        }
        
        float gz_rad = MPU6050_DMP_GetROSGyroz();
        printf("Gyro Z: %.3f rad/s\n", gz_rad);
        
        HAL_Delay(100);
    }
}
```

---

### 4. OLED 显示模块

支持 0.96 寸 I2C OLED 显示屏（SSD1306 控制器），使用软件模拟 I2C。

#### 硬件连接

| OLED引脚 | STM32引脚 | 说明 |
|---------|----------|------|
| VCC | 3.3V/5V | 电源 |
| GND | GND | 公共地 |
| SCL | GPIO输出 | I2C时钟线（软件模拟） |
| SDA | GPIO输出 | I2C数据线（软件模拟） |

#### CubeMX 配置

**GPIO配置：**
- 选择两个 GPIO 引脚（例如 PB6、PB7）
- 设置为 **GPIO_Output**
- 输出模式：Push-Pull
- 速度：High
- 初始电平：High

**引脚标签（可选）：**
- 将引脚标签设置为 `OLED_SCL` 和 `OLED_SDA`，方便识别

#### config.h 配置

```c
#define DEVICE_OLED     1
#if DEVICE_OLED 
    #include "gpio.h"
    
    // GPIO控制宏（CubeMX生成的引脚名称）
    #define OLED_W_SCL(x)           HAL_GPIO_WritePin(OLED_SCL_GPIO_Port, OLED_SCL_Pin, (x))
    #define OLED_W_SDA(x)           HAL_GPIO_WritePin(OLED_SDA_GPIO_Port, OLED_SDA_Pin, (x))
#endif
```

#### API 接口

```c
void OLED_Init(void);                          // 初始化OLED
void OLED_Clear(void);                         // 清屏

// 显示字符和字符串（行：1-4，列：1-16）
void OLED_ShowChar(uint8_t Line, uint8_t Column, char Char);
void OLED_ShowString(uint8_t Line, uint8_t Column, char *String);

// 显示数字
void OLED_ShowNum(uint8_t Line, uint8_t Column, uint32_t Number, uint8_t Length);
void OLED_ShowSignedNum(uint8_t Line, uint8_t Column, int32_t Number, uint8_t Length);
void OLED_ShowHexNum(uint8_t Line, uint8_t Column, uint32_t Number, uint8_t Length);
void OLED_ShowBinNum(uint8_t Line, uint8_t Column, uint32_t Number, uint8_t Length);

// 显示浮点数
void OLED_ShowSignedFloat(uint8_t line, uint8_t column, float value, uint8_t intWidth);
```

#### 使用示例

```c
OLED_Init();
OLED_Clear();

// 第1行显示标题
OLED_ShowString(1, 1, "STM32 HAL");

// 第2行显示整数
OLED_ShowString(2, 1, "Count:");
OLED_ShowNum(2, 8, 12345, 5);

// 第3行显示浮点数
float speed = -12.345f;
OLED_ShowString(3, 1, "Speed:");
OLED_ShowSignedFloat(3, 8, speed, 3);  // 显示为 "-12.34"

// 第4行显示十六进制
OLED_ShowString(4, 1, "ID:0x");
OLED_ShowHexNum(4, 6, 0x68, 2);

while (1)
{
    // 动态更新显示
    static uint32_t count = 0;
    OLED_ShowNum(2, 8, count++, 5);
    HAL_Delay(100);
}
```

---

**📧 联系方式**

如有问题或建议，欢迎提交 Issue 或 Pull Request！
