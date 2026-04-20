#include "lcd.h"

#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "esp_log.h"

static const char *TAG = "LCD";
static spi_device_handle_t lcd_spi_handle = NULL;

/**
 * @brief       Initialize LCD GPIO pins
 * @param       none
 * @retval      none
 */
void LCD_GPIO_Init(void)
{
    // Configure LCD control pins as output
    gpio_config_t io_conf = {0};
    io_conf.intr_type = GPIO_INTR_DISABLE;
    io_conf.mode = GPIO_MODE_OUTPUT;
    io_conf.pin_bit_mask = ((1ULL << LCD_RST_GPIO_PIN) |
                           (1ULL << LCD_DC_GPIO_PIN) |
                           (1ULL << LCD_BLK_GPIO_PIN));
    io_conf.pull_down_en = GPIO_PULLDOWN_DISABLE;
    io_conf.pull_up_en = GPIO_PULLUP_ENABLE;
    gpio_config(&io_conf);

    // Set initial states
    LCD_DC_Set();
    LCD_RES_Set();
    LCD_BLK_Set();

    ESP_LOGI(TAG, "LCD GPIO initialized");
}

/**
 * @brief       Initialize SPI for LCD communication
 * @param       none
 * @retval      none
 */
void LCD_SPI_Init(void)
{
   
    // Initialize SPI bus (if not already done)
    spi_init(&lcd_spi_handle);

	
}

/**
 * @brief       Write command to LCD
 * @param       cmd: command to write
 * @retval      none
 */
void LCD_Write_Cmd(uint8_t cmd)
{
    LCD_DC_Clr();  // Command mode
    spi_write_cmd(lcd_spi_handle, cmd);
    LCD_DC_Set();  // Switch back to data mode
}

/**
 * @brief       Write 8-bit data to LCD
 * @param       dat: data to write
 * @retval      none
 */
void LCD_Write_Data(uint8_t dat)
{
    LCD_DC_Set();  // Data mode
    uint8_t data = dat;
    spi_write_data(lcd_spi_handle, &data, 1);
}

/**
 * @brief       Write data array to LCD
 * @param       data: pointer to data array
 * @param       len: length of data array
 * @retval      none
 */
void LCD_Write_Data_Array(uint8_t *data, uint16_t len)
{
    LCD_DC_Set();  // Data mode
    spi_write_data(lcd_spi_handle, data, len);
}


/**
 * @brief       Initialize LCD
 * @param       none
 * @retval      none
 */
void LCD_Init(void)
{
    ESP_LOGI(TAG, "Initializing LCD...");

    // Initialize GPIO and SPI
    LCD_GPIO_Init();
    LCD_SPI_Init();

    // Reset sequence
    LCD_RES_Clr();
    vTaskDelay(pdMS_TO_TICKS(100));
    LCD_RES_Set();
    vTaskDelay(pdMS_TO_TICKS(120));

    // Turn on backlight
    LCD_BLK_Set();
    vTaskDelay(pdMS_TO_TICKS(100));

    // LCD initialization commands

    // Color format - RGB565
    LCD_Write_Cmd(0x3A);
    LCD_Write_Data(0x05);

    // VCOM setting
    LCD_Write_Cmd(0xC5);
    LCD_Write_Data(0x1A);

    // Memory data access control
    LCD_Write_Cmd(0x36);
    LCD_Write_Data(0x00);   // RGB mode

    // Porch control
    LCD_Write_Cmd(0xB2);
    LCD_Write_Data(0x05);
    LCD_Write_Data(0x05);
    LCD_Write_Data(0x00);
    LCD_Write_Data(0x33);
    LCD_Write_Data(0x33);

    // Gate control
    LCD_Write_Cmd(0xB7);
    LCD_Write_Data(0x05);

    // Voltage control
    LCD_Write_Cmd(0xBB);
    LCD_Write_Data(0x3F);

    LCD_Write_Cmd(0xC0);
    LCD_Write_Data(0x2C);

    LCD_Write_Cmd(0xC2);
    LCD_Write_Data(0x01);

    LCD_Write_Cmd(0xC3);
    LCD_Write_Data(0x0F);

    LCD_Write_Cmd(0xC4);
    LCD_Write_Data(0x20);

    LCD_Write_Cmd(0xC6);
    LCD_Write_Data(0x01);

    LCD_Write_Cmd(0xD0);
    LCD_Write_Data(0xA4);
    LCD_Write_Data(0xA1);

    LCD_Write_Cmd(0xE8);
    LCD_Write_Data(0x03);

    LCD_Write_Cmd(0xE9);
    LCD_Write_Data(0x09);
    LCD_Write_Data(0x09);
    LCD_Write_Data(0x08);

    // Gamma settings
    LCD_Write_Cmd(0xE0);
    LCD_Write_Data(0xD0);
    LCD_Write_Data(0x05);
    LCD_Write_Data(0x09);
    LCD_Write_Data(0x09);
    LCD_Write_Data(0x08);
    LCD_Write_Data(0x14);
    LCD_Write_Data(0x28);
    LCD_Write_Data(0x33);
    LCD_Write_Data(0x3F);
    LCD_Write_Data(0x07);
    LCD_Write_Data(0x13);
    LCD_Write_Data(0x14);
    LCD_Write_Data(0x28);
    LCD_Write_Data(0x30);

    LCD_Write_Cmd(0xE1);
    LCD_Write_Data(0xD0);
    LCD_Write_Data(0x05);
    LCD_Write_Data(0x09);
    LCD_Write_Data(0x09);
    LCD_Write_Data(0x08);
    LCD_Write_Data(0x03);
    LCD_Write_Data(0x24);
    LCD_Write_Data(0x32);
    LCD_Write_Data(0x32);
    LCD_Write_Data(0x3B);
    LCD_Write_Data(0x14);
    LCD_Write_Data(0x13);
    LCD_Write_Data(0x28);
    LCD_Write_Data(0x2F);

    // Inversion on
    LCD_Write_Cmd(0x21);

    // Sleep out
    LCD_Write_Cmd(0x11);
    vTaskDelay(pdMS_TO_TICKS(120));

    // Display on
    LCD_Write_Cmd(0x29);

    ESP_LOGI(TAG, "LCD initialization complete");
}

/**
 * @brief       Set address window for drawing
 * @param       x: start X coordinate
 * @param       y: start Y coordinate
 * @param       w: window width
 * @param       h: window height
 * @retval      none
 */
void LCD_Set_Address_Window(uint16_t x, uint16_t y, uint16_t w, uint16_t h)
{
    uint16_t xe = x + w - 1;
    uint16_t ye = y + h - 1;

    // Column address set
    LCD_Write_Cmd(0x2A);
    LCD_Write_Data(x >> 8);
    LCD_Write_Data(x & 0xFF);
    LCD_Write_Data(xe >> 8);
    LCD_Write_Data(xe & 0xFF);

    // Row address set
    LCD_Write_Cmd(0x2B);
    LCD_Write_Data(y >> 8);
    LCD_Write_Data(y & 0xFF);
    LCD_Write_Data(ye >> 8);
    LCD_Write_Data(ye & 0xFF);

    // Memory write
    LCD_Write_Cmd(0x2C);
}

/**
 * @brief       Clear entire LCD with color
 * @param       color: fill color
 * @retval      none
 */
void LCD_Clear(uint16_t color)
{
    uint32_t i;
    uint32_t count = LCD_W * LCD_H;
    uint8_t color_data[2];

    color_data[0] = color >> 8;    // High byte
    color_data[1] = color & 0xFF;  // Low byte

    LCD_Set_Address_Window(0, 0, LCD_W, LCD_H);

    LCD_DC_Set();   // Data mode

    // Use array write for better performance
    for(i = 0; i < count; i++) {
        LCD_Write_Data(color_data[0]);
        LCD_Write_Data(color_data[1]);
    }
}

/**
 * @brief       Draw a single pixel
 * @param       x: X coordinate
 * @param       y: Y coordinate
 * @param       color: pixel color
 * @retval      none
 */
void LCD_Draw_Pixel(uint16_t x, uint16_t y, uint16_t color)
{
    if (x >= LCD_W || y >= LCD_H) return;

    LCD_Set_Address_Window(x, y, 1, 1);

    LCD_DC_Set();
    LCD_Write_Data(color >> 8);  // High byte
    LCD_Write_Data(color & 0xFF); // Low byte
}






/**
 * @brief  LCD矩形填充函数（按结束坐标，适配LVGL颜色缓冲区）
 * @param  Xstart: 起始X坐标
 * @param  Ystart: 起始Y坐标
 * @param  Xend: 结束X坐标
 * @param  Yend: 结束Y坐标
 * @param  color_buf: LVGL颜色缓冲区指针（lv_color_t* 强转为uint16_t*）
 * @retval 无
 * @note   1. 适配SPI 16位半字传输模式
 *         2. 自动修正RGB565高低字节顺序
 *         3. 完全兼容LVGL的disp_flush接口
 */
void LCD_Fill(uint16_t Xstart, uint16_t Ystart, uint16_t Xend, uint16_t Yend, uint16_t* color_buf)
{
    uint16_t w, h;
    uint32_t pixel_count;
    uint32_t data_len; // 字节总长度 = 像素数 × 2

    // 计算宽高
    w = Xend - Xstart + 1;
    h = Yend - Ystart + 1;
    pixel_count = (uint32_t)w * h;
    data_len = pixel_count * 2; // RGB565 每个像素2字节

    // 空指针检查
    if(color_buf == NULL) return;

    // 设置显示窗口
    LCD_Set_Address_Window(Xstart, Ystart, w, h);

    LCD_DC_Set();   // 数据模式

	LCD_Write_Data_Array((uint8_t*)color_buf, data_len);
    
}




/**
 * @brief       Fill rectangle with color
 * @param       x: start X coordinate
 * @param       y: start Y coordinate
 * @param       w: rectangle width
 * @param       h: rectangle height
 * @param       color: fill color
 * @retval      none
 */
void LCD_Fill_Rect(uint16_t x, uint16_t y, uint16_t w, uint16_t h, uint16_t color)
{
    uint32_t i;
    uint32_t count = w * h;
    uint8_t color_data[2];

    color_data[0] = color >> 8;    // High byte
    color_data[1] = color & 0xFF;  // Low byte

    LCD_Set_Address_Window(x, y, w, h);

    LCD_DC_Set();

    for(i = 0; i < count; i++) {
        LCD_Write_Data(color_data[0]);
        LCD_Write_Data(color_data[1]);
    }
}



/**
 * @brief       Invert color
 * @param       c: original color
 * @retval      inverted color
 */
uint16_t Invert_color(uint16_t c)
{
    switch(c)
    {
        case 0xF800: return 0xFFE0; // Red -> Yellow
        case 0x07E0: return 0xF81F; // Green -> Magenta
        case 0x001F: return 0x07FF; // Blue -> Cyan

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