#ifndef __ST7789_H
#define __ST7789_H 
#include "stm32f4xx_hal.h"
#include "gpio.h"
#include "spi.h"

#include "stdlib.h"	
#include "stm32f4xx_hal_gpio.h"



#define SPI_Normal_Mode 0
#define SPI_DMA_Mode 1
#define FreeRTOS_Mode 0

/********************* LCD硬件引脚宏定义 *********************/
// 复位引脚（RES）：低电平复位，高电平正常工作
#define LCD_RES_Clr()  HAL_GPIO_WritePin(LCD_RST_GPIO_Port,LCD_RST_Pin,GPIO_PIN_RESET)
#define LCD_RES_Set()  HAL_GPIO_WritePin(LCD_RST_GPIO_Port,LCD_RST_Pin,GPIO_PIN_SET)

// 数据/命令引脚（DC）：低电平写命令，高电平写数据
#define LCD_DC_Clr()   HAL_GPIO_WritePin(LCD_DC_GPIO_Port,LCD_DC_Pin,GPIO_PIN_RESET)
#define LCD_DC_Set()   HAL_GPIO_WritePin(LCD_DC_GPIO_Port,LCD_DC_Pin,GPIO_PIN_SET)

// 片选引脚（CS）：低电平选中LCD，高电平取消选中
#define LCD_CS_Clr()   HAL_GPIO_WritePin(LCD_CS_GPIO_Port,LCD_CS_Pin,GPIO_PIN_RESET)
#define LCD_CS_Set()   HAL_GPIO_WritePin(LCD_CS_GPIO_Port,LCD_CS_Pin,GPIO_PIN_SET)

// 背光引脚（BLK）：低电平关闭背光，高电平开启背光
#define LCD_BLK_Clr()  HAL_GPIO_WritePin(LCD_BLK_GPIO_Port,LCD_BLK_Pin,GPIO_PIN_RESET)
#define LCD_BLK_Set()  HAL_GPIO_WritePin(LCD_BLK_GPIO_Port,LCD_BLK_Pin,GPIO_PIN_SET)

/********************* LCD分辨率宏定义 *********************/
#define LCD_W 240  // LCD宽度（像素）
#define LCD_H 320  // LCD高度（像素）

/********************* 常用颜色宏定义（RGB565格式） *********************/
#define WHITE          0xFFFF  // 白色
#define BLACK          0x0000  // 黑色
#define BLUE           0x001F  // 蓝色
#define BRED           0XF81F  // 蓝红色
#define GRED           0XFFE0  // 绿红色
#define GBLUE          0X07FF  // 绿蓝色
#define RED            0xF800  // 红色
#define MAGENTA        0xF81F  // 品红色
#define GREEN          0x07E0  // 绿色
#define CYAN           0x7FFF  // 青色
#define YELLOW         0xFFE0  // 黄色
#define BROWN          0XBC40  // 棕色
#define BRRED          0XFC07  // 棕红色
#define GRAY           0X8430  // 灰色
#define DARKBLUE       0X01CF  // 深蓝色
#define LIGHTBLUE      0X7D7C  // 浅蓝色
#define GRAYBLUE       0X5458  // 灰蓝色
#define LIGHTGREEN     0X841F  // 浅绿色
#define LGRAY          0XC618  // 浅灰色（面板专用）
#define LGRAYBLUE      0XA651  // 浅灰蓝色（中间层颜色）
#define LBBLUE         0X2B12  // 浅蓝蓝色（选中目标颜色）

/********************* 函数声明 *********************/
/**
 * @brief  LCD SPI总线写数据函数（底层SPI传输）
 * @param  dat: 要写入的8位数据
 * @retval 无
 */
void LCD_Writ_Bus(uint8_t dat);

/**
 * @brief  LCD写8位数据
 * @param  dat: 要写入的8位数据
 * @retval 无
 */
void LCD_WR_DATA8(uint8_t dat);

/**
 * @brief  LCD写16位数据（RGB565格式）
 * @param  dat: 要写入的16位颜色数据
 * @retval 无
 */
void LCD_WR_DATA(uint16_t dat);

/**
 * @brief  LCD写8位命令
 * @param  dat: 要写入的8位命令寄存器地址
 * @retval 无
 */
void LCD_WR_REG(uint8_t dat);

/**
 * @brief  LCD初始化函数（配置寄存器、时序、显示模式）
 * @param  无
 * @retval 无
 */
void LCD_Init(void);

/**
 * @brief  设置LCD显示窗口（地址窗口）
 * @param  x: 窗口起始X坐标
 * @param  y: 窗口起始Y坐标
 * @param  w: 窗口宽度（像素）
 * @param  h: 窗口高度（像素）
 * @retval 无
 */
void LCD_Set_Address_Window(uint16_t x, uint16_t y, uint16_t w, uint16_t h);

/**
 * @brief  全屏清屏函数
 * @param  color: 清屏颜色（RGB565格式）
 * @retval 无
 */
void LCD_Clear(uint16_t color);

/**
 * @brief  绘制单个像素点
 * @param  x: 像素点X坐标
 * @param  y: 像素点Y坐标
 * @param  color: 像素点颜色（RGB565格式）
 * @retval 无
 * @note   坐标超出分辨率时会直接返回
 */
void LCD_Draw_Pixel(uint16_t x, uint16_t y, uint16_t color);

/**
 * @brief  矩形填充函数（按结束坐标，适配LVGL颜色缓冲区）
 * @param  Xstart: 起始X坐标
 * @param  Ystart: 起始Y坐标
 * @param  Xend: 结束X坐标
 * @param  Yend: 结束Y坐标
 * @param  color_buf: LVGL颜色缓冲区指针（lv_color_t* 强转为uint16_t*）
 * @retval 无
 */
void LCD_Fill(uint16_t Xstart, uint16_t Ystart, uint16_t Xend, uint16_t Yend, uint16_t* color_buf);

/**
 * @brief  矩形填充函数（按宽度高度）
 * @param  x: 起始X坐标
 * @param  y: 起始Y坐标
 * @param  w: 矩形宽度（像素）
 * @param  h: 矩形高度（像素）
 * @param  color: 填充颜色（RGB565格式）
 * @retval 无
 */
void LCD_Fill_Rect(uint16_t x, uint16_t y, uint16_t w, uint16_t h, uint16_t color);

/**
 * @brief  颜色反转函数（自定义反转规则）
 * @param  c: 原始颜色（RGB565格式）
 * @retval 反转后的颜色（RGB565格式）
 */
uint16_t Invert_color(uint16_t c);



#endif // __ST7789_H