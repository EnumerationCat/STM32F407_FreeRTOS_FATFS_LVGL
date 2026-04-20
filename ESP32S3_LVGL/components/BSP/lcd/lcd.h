#ifndef __LCD_H
#define __LCD_H
#ifdef __cplusplus
extern "C" {
#endif

#include "driver/gpio.h"
#include "driver/spi_master.h"
#include "spi.h"

/* ESP32-S3 LCD Pin Definitions */
#define LCD_SCL_GPIO_PIN        SPI2_CLK_GPIO_PIN         /* SPI2_CLK - SCL */
#define LCD_SDA_GPIO_PIN        SPI2_MOSI_GPIO_PIN        /* SPI2_MOSI - SDA */



#define LCD_RST_GPIO_PIN        GPIO_NUM_4        /* Reset pin */
#define LCD_DC_GPIO_PIN         GPIO_NUM_5        /* Data/Command pin */
#define LCD_BLK_GPIO_PIN        GPIO_NUM_7        /* Backlight pin */

/* LCD Control Macros for ESP32-S3 */
#define LCD_RES_Clr()           gpio_set_level(LCD_RST_GPIO_PIN, 0)
#define LCD_RES_Set()           gpio_set_level(LCD_RST_GPIO_PIN, 1)

#define LCD_DC_Clr()            gpio_set_level(LCD_DC_GPIO_PIN, 0)
#define LCD_DC_Set()            gpio_set_level(LCD_DC_GPIO_PIN, 1)

#define LCD_BLK_Clr()           gpio_set_level(LCD_BLK_GPIO_PIN, 0)
#define LCD_BLK_Set()           gpio_set_level(LCD_BLK_GPIO_PIN, 1)

/* LCD Dimensions */
#define LCD_W 240
#define LCD_H 320

/* Color definitions */
#define WHITE         	 0xFFFF
#define BLACK         	 0x0000
#define BLUE           	 0x001F
#define BRED             0XF81F
#define GRED 			 0XFFE0
#define GBLUE			 0X07FF
#define RED           	 0xF800
#define MAGENTA       	 0xF81F
#define GREEN         	 0x07E0
#define CYAN          	 0x7FFF
#define YELLOW        	 0xFFE0
#define BROWN 			 0XBC40 //brown
#define BRRED 			 0XFC07 //brick red
#define GRAY  			 0X8430 //gray
#define DARKBLUE      	 0X01CF	//dark blue
#define LIGHTBLUE      	 0X7D7C	//light blue
#define GRAYBLUE       	 0X5458 //gray-blue
#define LIGHTGREEN     	 0X841F //light green
#define LGRAY 			 0XC618 //light gray
#define LGRAYBLUE        0XA651 //light gray-blue
#define LBBLUE           0X2B12 //light blue-gray

/* Function declarations */
void LCD_GPIO_Init(void);                               //Initialize GPIO
void LCD_SPI_Init(void);                                //Initialize SPI for LCD
void LCD_Write_Cmd(uint8_t cmd);                       //Write command
void LCD_Write_Data(uint8_t dat);                      //Write data byte
void LCD_Write_Data_Array(uint8_t *data, uint16_t len); //Write data array
void LCD_Init(void);                                   //LCD initialization

void LCD_Set_Address_Window(uint16_t x, uint16_t y, uint16_t w, uint16_t h);
void LCD_Clear(uint16_t color);
void LCD_Draw_Pixel(uint16_t x, uint16_t y, uint16_t color);


void LCD_Fill(uint16_t Xstart, uint16_t Ystart, uint16_t Xend, uint16_t Yend, uint16_t* color_buf);
void LCD_Fill_Rect(uint16_t x, uint16_t y, uint16_t w, uint16_t h, uint16_t color);

uint16_t Invert_color(uint16_t c);

#ifdef __cplusplus
}
#endif
#endif // __LCD_H