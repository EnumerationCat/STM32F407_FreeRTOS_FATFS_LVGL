#include "spi.h"




// 注意：回调函数需放在 IRAM 中（避免 Flash 缓存失效）





/**
 * @brief       初始化SPI
 * @param       无
 * @retval      无
 */
void spi_init(spi_device_handle_t *spi_device_handle)
{
    esp_err_t ret = 0;
    spi_bus_config_t spi_bus_conf = {0};

    /* SPI总线配置 */
    spi_bus_conf.miso_io_num = SPI2_MISO_GPIO_PIN;                               /* SPI_MISO引脚 */
    spi_bus_conf.mosi_io_num = SPI2_MOSI_GPIO_PIN;                               /* SPI_MOSI引脚 */
    spi_bus_conf.sclk_io_num = SPI2_CLK_GPIO_PIN;                                /* SPI_SCLK引脚 */
    spi_bus_conf.quadwp_io_num = -1;                                            /* SPI写保护信号引脚，该引脚未使能 */
    spi_bus_conf.quadhd_io_num = -1;                                            /* SPI保持信号引脚，该引脚未使能 */
    spi_bus_conf.max_transfer_sz = 320 * 240 * 2;                               /* 配置最大传输大小，以字节为单位 */
    
    /* 初始化SPI总线 */
    ret = spi_bus_initialize(SPI2_HOST, &spi_bus_conf, SPI_DMA_CH_AUTO);        /* SPI总线初始化 */
    ESP_ERROR_CHECK(ret);                                                       /* 校验参数值 */

    // Configure SPI device for LCD
    spi_device_interface_config_t devcfg = {
        .clock_speed_hz = 80 * 1000 * 1000,  // 80 MHz
        .mode = 0,                           // SPI mode 0
        .spics_io_num = SPI2_CS_GPIO_PIN,     // CS pin
        .queue_size = 15,                     // Transaction queue size
		.post_cb = spi_post_cb,
        .flags = 0,
        .command_bits = 0,
        .address_bits = 0,
        .dummy_bits = 0,
    };

    // Add SPI device
    ret = spi_bus_add_device(SPI2_HOST, &devcfg, spi_device_handle);
    ESP_ERROR_CHECK(ret);

}



/**
 * @brief       SPI发送命令
 * @param       handle : SPI句柄
 * @param       cmd    : 要发送命令
 * @retval      无
 */
void spi_write_cmd(spi_device_handle_t handle, uint8_t cmd)
{
    esp_err_t ret;
    spi_transaction_t t = {0};

    t.length = 8;                                       /* 要传输的位数 一个字节 8位 */
    t.tx_buffer = &cmd;                                 /* 将命令填充进去 */




    // ret = spi_device_polling_transmit(handle, &t);      /* 开始传输 */
    // ESP_ERROR_CHECK(ret);                               /* 一般不会有问题 */


	spi_transaction_t *ret_trans;
	ret = spi_device_queue_trans(handle, &t, portMAX_DELAY);
    ESP_ERROR_CHECK(ret);
	/* 等待传输完成 */
    ret = spi_device_get_trans_result(handle, &ret_trans, portMAX_DELAY);
    ESP_ERROR_CHECK(ret);
}

/**
 * @brief       SPI发送数据
 * @param       handle : SPI句柄
 * @param       data   : 要批量发送的数据
 * @param       len    : 要发送的数据长度 
 * @retval      无
 */
void spi_write_data(spi_device_handle_t handle, const uint8_t *data, int len)
{
    esp_err_t ret;
    spi_transaction_t t = {0};

    if (len == 0)
    {
        return;                                     /* 长度为0 没有数据要传输 */
    }

    t.length = len * 8;                             /* 要传输的位数 一个字节 8位 */
    t.tx_buffer = data;                             /* 将命令填充进去 */




	
    // ret = spi_device_polling_transmit(handle, &t);  /* 开始传输 */
    // ESP_ERROR_CHECK(ret);                           /* 一般不会有问题 */


	spi_transaction_t *ret_trans;
	
	// 异步入队，发送完成触发中断
    ret = spi_device_queue_trans(handle, &t, portMAX_DELAY);
    ESP_ERROR_CHECK(ret);
	/* 等待传输完成 */
    ret = spi_device_get_trans_result(handle, &ret_trans, portMAX_DELAY);
    ESP_ERROR_CHECK(ret);
}



/**
 * @brief       SPI处理数据
 * @param       handle       : SPI句柄
 * @param       data         : 要发送的数据 
 * @retval      t.rx_data[0] : 接收到的数据
 */
uint8_t spi_transfer_byte(spi_device_handle_t handle, uint8_t data)
{
    spi_transaction_t t;

    memset(&t, 0, sizeof(t));

    t.flags = SPI_TRANS_USE_TXDATA | SPI_TRANS_USE_RXDATA;
    t.length = 8;
    t.tx_data[0] = data;
    spi_device_transmit(handle, &t);

    return t.rx_data[0];
}
