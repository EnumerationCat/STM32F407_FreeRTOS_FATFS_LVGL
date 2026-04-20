#ifndef __UART_H
#define __UART_H

// C++ 兼容核心：判断是否为 C++ 编译器，若是则启用 extern "C"
#ifdef __cplusplus
extern "C" {
#endif

#include <string.h>
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "driver/uart.h"
#include "driver/uart_select.h"
#include "driver/gpio.h"

#define USART_UX      UART_NUM_0
#define USART_TX_GPIO_PIN GPIO_NUM_43
#define USART_RX_GPIO_PIN GPIO_NUM_44

#define RX_BUF_SIZE (1024)
#define TX_BUF_SIZE (1024)

// 函数声明（被 extern "C" 包裹，C++ 调用时不修改函数名）
void usart_init(uint32_t baud_rate);
void uart_process(void);

// 关闭 extern "C"
#ifdef __cplusplus
}
#endif

#endif // __UART_H