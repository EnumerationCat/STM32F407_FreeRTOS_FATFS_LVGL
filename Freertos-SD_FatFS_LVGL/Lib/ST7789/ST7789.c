#include "ST7789.h"
#include "stm32f4xx_hal.h"

/********************* 底层硬件操作函数 *********************/
/**
 * @brief  LCD SPI总线写数据函数（底层实现）
 * @param  dat: 要写入的8位数据
 * @retval 无
 * @note   操作流程：拉低CS选中LCD -> SPI发送数据 -> 拉高CS取消选中
 */
void LCD_Writ_Bus(uint8_t dat)
{
	LCD_CS_Clr();
	HAL_SPI_Transmit(&hspi1, &dat, 1, HAL_MAX_DELAY);
	LCD_CS_Set();
}

/**
 * @brief  LCD写8位数据（封装SPI总线写操作）
 * @param  dat: 要写入的8位数据
 * @retval 无
 */
void LCD_WR_DATA8(uint8_t dat)
{
	LCD_Writ_Bus(dat);
}

/**
 * @brief  LCD写16位数据（RGB565格式，分两次发送高8位/低8位）
 * @param  dat: 要写入的16位颜色数据
 * @retval 无
 * @note   先发送高8位，再发送低8位
 */
void LCD_WR_DATA(uint16_t dat)
{
	LCD_Writ_Bus(dat>>8);  // 发送高8位
	LCD_Writ_Bus(dat);     // 发送低8位
}

/**
 * @brief  LCD批量写数据（DMA方式）
 * @param  data: 数据缓冲区指针
 * @param  len: 数据长度（字节数）
 * @retval 无
 * @note   1. 使用DMA传输提升批量数据写入效率
 *         2. 传输完成前会等待SPI状态就绪，建议生产环境添加超时机制
 */
void LCD_WR_DATA_Array(uint8_t *data, uint16_t len)
{
    LCD_CS_Clr();
    HAL_SPI_Transmit_DMA(&hspi1, data, len);
    // 等待DMA传输完成（SPI状态就绪）
    while (HAL_SPI_GetState(&hspi1) != HAL_SPI_STATE_READY)
    {
        // 生产环境建议添加超时判断，避免死等
        HAL_Delay(1);
    }
    LCD_CS_Set();
}

/**
 * @brief  LCD写8位命令（区分命令/数据模式）
 * @param  dat: 要写入的8位命令寄存器地址
 * @retval 无
 * @note   操作流程：拉低DC进入命令模式 -> 发送命令 -> 拉高DC恢复数据模式
 */
void LCD_WR_REG(uint8_t dat)
{
	LCD_DC_Clr();  // 切换到命令模式
	LCD_Writ_Bus(dat);
	LCD_DC_Set();  // 切换回数据模式
}

/********************* LCD初始化配置 *********************/
/**
 * @brief  LCD初始化函数（配置ST7789寄存器，初始化显示参数）
 * @param  无
 * @retval 无
 * @note   初始化流程：复位 -> 开启背光 -> 配置颜色模式 -> 配置时序/电压 -> 配置Gamma -> 开启显示
 */
void LCD_Init(void)
{
    // 硬件复位
    LCD_RES_Clr();
    HAL_Delay(100);  // 复位低电平保持100ms
    LCD_RES_Set();
    HAL_Delay(120);  // 复位后等待120ms稳定

    // 开启背光
    LCD_BLK_Set();
    HAL_Delay(100);

    // 配置颜色模式：RGB565（0x3A是颜色模式寄存器，0x05表示RGB565）
    LCD_WR_REG(0x3A);
    LCD_WR_DATA8(0x05);
	
	// 配置VCOM电压（0xC5是VCOM寄存器，0x1A为推荐值）
	LCD_WR_REG(0xC5); 		
	LCD_WR_DATA8(0x1A);

    // 配置内存访问模式（0x36是内存访问寄存器，0x00配置扫描方向/BGR格式）
    LCD_WR_REG(0x36);
    LCD_WR_DATA8(0x00);   

    // 配置Porch设置（帧时序相关）
    LCD_WR_REG(0xB2);
    LCD_WR_DATA8(0x05);
    LCD_WR_DATA8(0x05);
    LCD_WR_DATA8(0x00);
    LCD_WR_DATA8(0x33);
    LCD_WR_DATA8(0x33);

    // 配置Gate控制（栅极驱动相关）
    LCD_WR_REG(0xB7);
    LCD_WR_DATA8(0x05);

    // 配置电压设置（电源管理相关）
    LCD_WR_REG(0xBB);  // VCOM电压配置
    LCD_WR_DATA8(0x3F);
    LCD_WR_REG(0xC0);  // 电源控制1
    LCD_WR_DATA8(0x2C);
    LCD_WR_REG(0xC2);  // 电源控制2
    LCD_WR_DATA8(0x01);
    LCD_WR_REG(0xC3);  // 电源控制3
    LCD_WR_DATA8(0x0F);
    LCD_WR_REG(0xC4);  // 电源控制4
    LCD_WR_DATA8(0x20);
    LCD_WR_REG(0xC6);  // 帧率控制
    LCD_WR_DATA8(0x01);
    LCD_WR_REG(0xD0);  // 电源控制A
    LCD_WR_DATA8(0xA4);
    LCD_WR_DATA8(0xA1);

    // 配置时序控制（0xE8/0xE9为时序相关寄存器）
    LCD_WR_REG(0xE8);
    LCD_WR_DATA8(0x03);
    LCD_WR_REG(0xE9);
    LCD_WR_DATA8(0x09);
    LCD_WR_DATA8(0x09);
    LCD_WR_DATA8(0x08);

    // 配置Gamma曲线（提升显示效果）
    LCD_WR_REG(0xE0);  // 正Gamma校正
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

    LCD_WR_REG(0xE1);  // 负Gamma校正
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

    // 开启显示翻转（0x21为显示翻转寄存器）
    LCD_WR_REG(0x21);
    // 退出睡眠模式（0x11为睡眠模式寄存器）
    LCD_WR_REG(0x11);
    HAL_Delay(120);  // 等待退出睡眠模式
    // 开启显示（0x29为显示使能寄存器）
    LCD_WR_REG(0x29);
}

/********************* 显示控制函数 *********************/
/**
 * @brief  设置LCD显示窗口（地址窗口）
 * @param  x: 窗口起始X坐标
 * @param  y: 窗口起始Y坐标
 * @param  w: 窗口宽度（像素）
 * @param  h: 窗口高度（像素）
 * @retval 无
 * @note   1. 先设置列地址（0x2A），再设置行地址（0x2B），最后开启写GRAM（0x2C）
 *         2. 地址范围：X(0~239)，Y(0~319)
 */
void LCD_Set_Address_Window(uint16_t x, uint16_t y, uint16_t w, uint16_t h)
{
    uint16_t xe = x + w - 1;  // 窗口结束X坐标
    uint16_t ye = y + h - 1;  // 窗口结束Y坐标

    // 设置列地址范围（0x2A为列地址寄存器）
    LCD_WR_REG(0x2A);
    LCD_WR_DATA8(x >> 8);    // 起始X高8位
    LCD_WR_DATA8(x & 0xFF);  // 起始X低8位
    LCD_WR_DATA8(xe >> 8);   // 结束X高8位
    LCD_WR_DATA8(xe & 0xFF); // 结束X低8位

    // 设置行地址范围（0x2B为行地址寄存器）
    LCD_WR_REG(0x2B);
    LCD_WR_DATA8(y >> 8);    // 起始Y高8位
    LCD_WR_DATA8(y & 0xFF);  // 起始Y低8位
    LCD_WR_DATA8(ye >> 8);   // 结束Y高8位
    LCD_WR_DATA8(ye & 0xFF); // 结束Y低8位

    // 开启写GRAM（0x2C为写GRAM寄存器，后续数据写入显示RAM）
    LCD_WR_REG(0x2C);
}

/**
 * @brief  全屏清屏函数
 * @param  color: 清屏颜色（RGB565格式）
 * @retval 无
 * @note   1. 设置全屏地址窗口，逐像素写入指定颜色
 *         2. 总像素数 = LCD_W * LCD_H = 240*320 = 76800
 */
void LCD_Clear(uint16_t color)
{
    uint32_t i;
    uint32_t count = LCD_W * LCD_H;  // 全屏总像素数
    uint8_t color_data[2];          // RGB565颜色拆分存储

    color_data[0] = color >> 8;    // 颜色高8位
    color_data[1] = color & 0xFF;  // 颜色低8位

    // 设置全屏地址窗口
    LCD_Set_Address_Window(0, 0, LCD_W, LCD_H);

    LCD_DC_Set();   // 切换到数据模式
    LCD_CS_Clr();   // 选中LCD

    // 逐像素写入颜色数据
    for(i = 0; i < count; i++)
    {
        HAL_SPI_Transmit(&hspi1, color_data, 2, HAL_MAX_DELAY);
    }

    LCD_CS_Set();   // 取消选中LCD
}

/**
 * @brief  绘制单个像素点
 * @param  x: 像素点X坐标
 * @param  y: 像素点Y坐标
 * @param  color: 像素点颜色（RGB565格式）
 * @retval 无
 * @note   1. 坐标超出分辨率（X≥240或Y≥320）时直接返回
 *         2. 设置1x1地址窗口，写入单个像素颜色
 */
void LCD_Draw_Pixel(uint16_t x, uint16_t y, uint16_t color)
{
    // 坐标合法性检查
    if (x >= 240 || y >= 320) return;

    // 设置1x1地址窗口（仅当前像素点）
    LCD_Set_Address_Window(x, y, 1, 1);

    LCD_DC_Set();
    LCD_CS_Clr();

    uint8_t color_data[2];
    color_data[0] = color >> 8;    // 颜色高8位
    color_data[1] = color & 0xFF;  // 颜色低8位

    // 写入像素颜色
    HAL_SPI_Transmit(&hspi1, color_data, 2, HAL_MAX_DELAY);

    LCD_CS_Set();
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
    uint32_t i;

    // 计算宽高
    w = Xend - Xstart + 1;
    h = Yend - Ystart + 1;
    pixel_count = (uint32_t)w * h;

    // 空指针检查
    if(color_buf == NULL) return;

    // 设置显示窗口
    LCD_Set_Address_Window(Xstart, Ystart, w, h);

    LCD_DC_Set();   // 数据模式
    LCD_CS_Clr();   // 选中LCD

    // ===================== 关键修复 =====================
    // SPI 16位半字传输 + 颜色高低字节反转修复
    // ====================================================
    for(i = 0; i < pixel_count; i++)
    {
        // 反转高低字节：解决颜色反色问题
        uint16_t color = (color_buf[i] << 8) | (color_buf[i] >> 8);

        // 16位半字SPI发送（适配你的硬件配置）
        HAL_SPI_Transmit(&hspi1, (uint8_t*)&color, 2, HAL_MAX_DELAY);
    }

    LCD_CS_Set();
}
/**
 * @brief  LCD矩形填充函数（按宽度高度）
 * @param  x: 起始X坐标
 * @param  y: 起始Y坐标
 * @param  w: 矩形宽度（像素）
 * @param  h: 矩形高度（像素）
 * @param  color: 填充颜色（RGB565格式）
 * @retval 无
 * @note   与LCD_Fill功能一致，仅参数形式不同（宽度高度替代结束坐标）
 */
void LCD_Fill_Rect(uint16_t x, uint16_t y, uint16_t w, uint16_t h, uint16_t color)
{
    uint32_t i;
    uint32_t count = w * h;  // 矩形总像素数
    uint8_t color_data[2];

    color_data[0] = color >> 8;    // 颜色高8位
    color_data[1] = color & 0xFF;  // 颜色低8位

    // 设置矩形地址窗口
    LCD_Set_Address_Window(x, y, w, h);

    LCD_DC_Set();
    LCD_CS_Clr();

    // 逐像素写入颜色
    for(i = 0; i < count; i++)
    {
        HAL_SPI_Transmit(&hspi1, color_data, 2, HAL_MAX_DELAY);
    }

    LCD_CS_Set();
}

/**
 * @brief  颜色反转函数（自定义反转规则）
 * @param  c: 原始颜色（RGB565格式）
 * @retval 反转后的颜色（RGB565格式）
 * @note   1. 对红/绿/蓝三色做固定反转，其他颜色按R=G+B、G=R+B、B=R+G规则反转
 *         2. RGB565格式拆分：R(bit15~11)、G(bit10~5)、B(bit4~0)
 */
uint16_t Invert_color(uint16_t c)
{
    switch(c)
    {
        case 0xF800: return 0xFFE0; // 红 -> 黄
        case 0x07E0: return 0xF81F; // 绿 -> 品红
        case 0x001F: return 0x07FF; // 蓝 -> 青
        default:
        {
            // 拆分RGB565的R/G/B分量
            uint16_t r = (c >> 11) & 0x1F;  // R分量（5位）
            uint16_t g = (c >> 5)  & 0x3F;  // G分量（6位）
            uint16_t b =  c        & 0x1F;  // B分量（5位）

            // 自定义反转规则
            uint16_t r2 = (g | b);   // 新R = 原G | 原B
            uint16_t g2 = (r | b);   // 新G = 原R | 原B
            uint16_t b2 = (r | g);   // 新B = 原R | 原G

            // 重组为RGB565格式
            return (r2 << 11) | (g2 << 5) | b2;
        }
    }
}
