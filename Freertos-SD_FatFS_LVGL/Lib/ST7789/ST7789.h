#ifndef __ST7789_H
#define __ST7789_H 
#include "stm32f4xx_hal.h"
#include "gpio.h"
#include "spi.h"

#include "stdlib.h"	
#include "stm32f4xx_hal_gpio.h"




#define LCD_RES_Clr()  HAL_GPIO_WritePin(LCD_RST_GPIO_Port,LCD_RST_Pin,GPIO_PIN_RESET)//RES
#define LCD_RES_Set()  HAL_GPIO_WritePin(LCD_RST_GPIO_Port,LCD_RST_Pin,GPIO_PIN_SET)

#define LCD_DC_Clr()   HAL_GPIO_WritePin(LCD_DC_GPIO_Port,LCD_DC_Pin,GPIO_PIN_RESET)//DC
#define LCD_DC_Set()   HAL_GPIO_WritePin(LCD_DC_GPIO_Port,LCD_DC_Pin,GPIO_PIN_SET)

#define LCD_CS_Clr()   HAL_GPIO_WritePin(LCD_CS_GPIO_Port,LCD_CS_Pin,GPIO_PIN_RESET)//CS
#define LCD_CS_Set()   HAL_GPIO_WritePin(LCD_CS_GPIO_Port,LCD_CS_Pin,GPIO_PIN_SET)

#define LCD_BLK_Clr()  HAL_GPIO_WritePin(LCD_BLK_GPIO_Port,LCD_BLK_Pin,GPIO_PIN_RESET)//BLK
#define LCD_BLK_Set()  HAL_GPIO_WritePin(LCD_BLK_GPIO_Port,LCD_BLK_Pin,GPIO_PIN_SET)






#define LCD_W 240
#define LCD_H 320



//������ɫ
#define WHITE         	 0xFFFF
#define BLACK         	 0x0000	  
#define BLUE           	 0x001F  
#define BRED             0XF81F
#define GRED 			       0XFFE0
#define GBLUE			       0X07FF
#define RED           	 0xF800
#define MAGENTA       	 0xF81F
#define GREEN         	 0x07E0
#define CYAN          	 0x7FFF
#define YELLOW        	 0xFFE0
#define BROWN 			     0XBC40 //��ɫ
#define BRRED 			     0XFC07 //�غ�ɫ
#define GRAY  			     0X8430 //��ɫ
#define DARKBLUE      	 0X01CF	//����ɫ
#define LIGHTBLUE      	 0X7D7C	//ǳ��ɫ  
#define GRAYBLUE       	 0X5458 //����ɫ
#define LIGHTGREEN     	 0X841F //ǳ��ɫ
#define LGRAY 			     0XC618 //ǳ��ɫ(PANNEL),���屳��ɫ
#define LGRAYBLUE        0XA651 //ǳ����ɫ(�м����ɫ)
#define LBBLUE           0X2B12 //ǳ����ɫ(ѡ����Ŀ�ķ�ɫ)


void LCD_Writ_Bus(uint8_t dat);//ģ��SPIʱ��
void LCD_WR_DATA8(uint8_t dat);//д��һ���ֽ�
void LCD_WR_DATA(uint16_t dat);//д�������ֽ�
void LCD_WR_REG(uint8_t dat);//д��һ��ָ��
void LCD_Init(void);//LCD��ʼ��

void LCD_Set_Address_Window(uint16_t x, uint16_t y, uint16_t w, uint16_t h);
void LCD_Clear(uint16_t color);
void LCD_Draw_Pixel(uint16_t x, uint16_t y, uint16_t color);
void LCD_Fill_Rect(uint16_t x, uint16_t y, uint16_t w, uint16_t h, uint16_t color);



uint16_t Invert_color(uint16_t c);

// External SPI handle for HAL communication
extern SPI_HandleTypeDef hspi1;

#endif

