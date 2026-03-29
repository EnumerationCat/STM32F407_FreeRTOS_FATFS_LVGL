#include "ST7789.h"
#include "stm32f4xx_hal.h"






/******************************************************************************
      ����˵����LCD��������д�뺯��
      ������ݣ�dat  Ҫд��Ĵ�������
      ����ֵ��  ��
******************************************************************************/
void LCD_Writ_Bus(uint8_t dat)
{
	LCD_CS_Clr();
	HAL_SPI_Transmit(&hspi1, &dat, 1, HAL_MAX_DELAY);
	LCD_CS_Set();
}


/******************************************************************************
      ����˵����LCDд������
      ������ݣ�dat д�������
      ����ֵ��  ��
******************************************************************************/
void LCD_WR_DATA8(uint8_t dat)
{
	LCD_Writ_Bus(dat);
}


/******************************************************************************
      ����˵����LCDд������
      ������ݣ�dat д�������
      ����ֵ��  ��
******************************************************************************/
void LCD_WR_DATA(uint16_t dat)
{
	LCD_Writ_Bus(dat>>8);
	LCD_Writ_Bus(dat);
	
}

void LCD_WR_DATA_Array(uint8_t *data, uint16_t len)
{
    LCD_CS_Clr();
    HAL_SPI_Transmit_DMA(&hspi1, data, len);
    // Wait for DMA transfer to complete
    while (HAL_SPI_GetState(&hspi1) != HAL_SPI_STATE_READY)
    {
        // You might want to add a timeout here in production code
        HAL_Delay(1);
    }
    LCD_CS_Set();
}
/******************************************************************************
      ����˵����LCDд������
      ������ݣ�dat д�������
      ����ֵ��  ��
******************************************************************************/
void LCD_WR_REG(uint8_t dat)
{
	LCD_DC_Clr();//д����
	LCD_Writ_Bus(dat);
	LCD_DC_Set();//д����
}





void LCD_Init(void)
{


    LCD_RES_Clr();
    HAL_Delay(100);
    LCD_RES_Set();
    HAL_Delay(120);

    LCD_BLK_Set();
    HAL_Delay(100);

    // ===== �ؼ�1����ɫ��ʽ =====
    LCD_WR_REG(0x3A);
    LCD_WR_DATA8(0x05);   // RGB565
	
	LCD_WR_REG(0xC5); 		//VCOM
	LCD_WR_DATA8(0x1A);

    // ===== �ؼ�2������ + BGR =====
    LCD_WR_REG(0x36);
    LCD_WR_DATA8(0x00);   

    // ===== Porch =====
    LCD_WR_REG(0xB2);
    LCD_WR_DATA8(0x05);
    LCD_WR_DATA8(0x05);
    LCD_WR_DATA8(0x00);
    LCD_WR_DATA8(0x33);
    LCD_WR_DATA8(0x33);

    // ===== Gate =====
    LCD_WR_REG(0xB7);
    LCD_WR_DATA8(0x05);

    // ===== ��ѹ���� =====
    LCD_WR_REG(0xBB);
    LCD_WR_DATA8(0x3F);

    LCD_WR_REG(0xC0);
    LCD_WR_DATA8(0x2C);

    LCD_WR_REG(0xC2);
    LCD_WR_DATA8(0x01);

    LCD_WR_REG(0xC3);
    LCD_WR_DATA8(0x0F);

    LCD_WR_REG(0xC4);
    LCD_WR_DATA8(0x20);

    LCD_WR_REG(0xC6);
    LCD_WR_DATA8(0x01);

    LCD_WR_REG(0xD0);
    LCD_WR_DATA8(0xA4);
    LCD_WR_DATA8(0xA1);

    LCD_WR_REG(0xE8);
    LCD_WR_DATA8(0x03);

    LCD_WR_REG(0xE9);
    LCD_WR_DATA8(0x09);
    LCD_WR_DATA8(0x09);
    LCD_WR_DATA8(0x08);

    // ===== Gamma =====
    LCD_WR_REG(0xE0);
	LCD_WR_DATA8(0xD0);
	LCD_WR_DATA8(0x05);
	LCD_WR_DATA8(0x09);
	LCD_WR_DATA8(0x09);
	LCD_WR_DATA8(0x08);
	LCD_WR_DATA8(0x14);
	LCD_WR_DATA8(0x28);
	LCD_WR_DATA8(0x33);
	LCD_WR_DATA8(0x3F);
	LCD_WR_DATA8(0x07);
	LCD_WR_DATA8(0x13);
	LCD_WR_DATA8(0x14);
	LCD_WR_DATA8(0x28);
	LCD_WR_DATA8(0x30);

    LCD_WR_REG(0xE1);
	LCD_WR_DATA8(0xD0);
	LCD_WR_DATA8(0x05);
	LCD_WR_DATA8(0x09);
	LCD_WR_DATA8(0x09);
	LCD_WR_DATA8(0x08);
	LCD_WR_DATA8(0x03);
	LCD_WR_DATA8(0x24);
	LCD_WR_DATA8(0x32);
	LCD_WR_DATA8(0x32);
	LCD_WR_DATA8(0x3B);
	LCD_WR_DATA8(0x14);
	LCD_WR_DATA8(0x13);
	LCD_WR_DATA8(0x28);
	LCD_WR_DATA8(0x2F);
  

    // ===== ����=====
    LCD_WR_REG(0x21);
    // ===== ���� =====
    LCD_WR_REG(0x11);
    HAL_Delay(120);
    // ===== ����ʾ =====
    LCD_WR_REG(0x29);
}





void LCD_Set_Address_Window(uint16_t x, uint16_t y, uint16_t w, uint16_t h)
{
    uint16_t xe = x + w - 1;
    uint16_t ye = y + h - 1;

    // �е�ַ����
    LCD_WR_REG(0x2A);
    LCD_WR_DATA8(x >> 8);
    LCD_WR_DATA8(x & 0xFF);
    LCD_WR_DATA8(xe >> 8);
    LCD_WR_DATA8(xe & 0xFF);

    // �е�ַ����
    LCD_WR_REG(0x2B);
    LCD_WR_DATA8(y >> 8);
    LCD_WR_DATA8(y & 0xFF);
    LCD_WR_DATA8(ye >> 8);
    LCD_WR_DATA8(ye & 0xFF);

    // д��GRAM
    LCD_WR_REG(0x2C);
}


void LCD_Clear(uint16_t color)
{
    uint32_t i;
    uint32_t count = LCD_W * LCD_H;
    uint8_t color_data[2];

    color_data[0] = color >> 8;    // High byte
    color_data[1] = color & 0xFF;  // Low byte

    LCD_Set_Address_Window(0, 0, LCD_W, LCD_H);

    LCD_DC_Set();   // ����ģʽ
    LCD_CS_Clr();   // ѡ��LCD

    // Use DMA for bulk transfer - send color data repeatedly
    for(i = 0; i < count; i++)
    {
        HAL_SPI_Transmit(&hspi1, color_data, 2, HAL_MAX_DELAY);
    }

    LCD_CS_Set();   // �ͷ�LCD
}


void LCD_Draw_Pixel(uint16_t x, uint16_t y, uint16_t color)
{
    if (x >= 240 || y >= 320) return;

    LCD_Set_Address_Window(x, y, 1, 1);

    LCD_DC_Set();
    LCD_CS_Clr();

    uint8_t color_data[2];
    color_data[0] = color >> 8;    // High byte
    color_data[1] = color & 0xFF;  // Low byte

    HAL_SPI_Transmit(&hspi1, color_data, 2, HAL_MAX_DELAY);

    LCD_CS_Set();
}

void LCD_Fill_Rect(uint16_t x, uint16_t y, uint16_t w, uint16_t h, uint16_t color)
{
    uint32_t i;
    uint32_t count = w * h;
    uint8_t color_data[2];

    color_data[0] = color >> 8;    // High byte
    color_data[1] = color & 0xFF;  // Low byte

    LCD_Set_Address_Window(x, y, w, h);

    LCD_DC_Set();
    LCD_CS_Clr();

    for(i = 0; i < count; i++)
    {
        HAL_SPI_Transmit(&hspi1, color_data, 2, HAL_MAX_DELAY);
    }

    LCD_CS_Set();
}



//��Ļ�ٷ�����
void TFT_full(uint16_t color)
{
    // Use the optimized LCD_Clear function instead
    LCD_Clear(color);
    return;
}





uint16_t Invert_color(uint16_t c)
{
    switch(c)
    {
        case 0xF800: return 0xFFE0; // �� -> ��
        case 0x07E0: return 0xF81F; // �� -> ��
        case 0x001F: return 0x07FF; // �� -> ��

        default:
        {
            
            uint16_t r = (c >> 11) & 0x1F;
            uint16_t g = (c >> 5)  & 0x3F;
            uint16_t b =  c        & 0x1F;

            uint16_t r2 = (g | b);   // R = G+B
            uint16_t g2 = (r | b);   // G = R+B
            uint16_t b2 = (r | g);   // B = R+G

            return (r2 << 11) | (g2 << 5) | b2;
        }
    }
}
