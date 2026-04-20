#ifndef __SPI_H
#define __SPI_H
#ifdef __cplusplus
extern "C" {
#endif

#include <string.h>

#include "esp_log.h"
#include "driver/spi_master.h"
#include "driver/gpio.h"
#include "esp_attr.h"

/* 引脚定义 */
#define SPI2_MOSI_GPIO_PIN   GPIO_NUM_11         /* SPI2_MOSI */
#define SPI2_CLK_GPIO_PIN    GPIO_NUM_12         /* SPI2_CLK */
#define SPI2_MISO_GPIO_PIN   GPIO_NUM_13         /* SPI2_MISO */
#define SPI2_CS_GPIO_PIN      GPIO_NUM_6        /* Chip Select pin */

/* 函数声明 */
void spi_init(spi_device_handle_t *spi_device_handle);                              /* 初始化SPI2 */
void spi_write_cmd(spi_device_handle_t handle, uint8_t cmd);                       /* SPI发送命令 */
void spi_write_data(spi_device_handle_t handle, const uint8_t *data, int len);     /* SPI发送数据 */
uint8_t spi_transfer_byte(spi_device_handle_t handle, uint8_t byte);               /* SPI处理数据 */


extern void IRAM_ATTR spi_post_cb(spi_transaction_t *trans);

#ifdef __cplusplus
}
#endif
#endif // __SPI_H