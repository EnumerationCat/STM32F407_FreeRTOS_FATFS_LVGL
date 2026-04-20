#include "uart.h"



void usart_init(uint32_t baud_rate)
{

	uart_config_t uart_config = {
		.baud_rate = baud_rate,
		.data_bits = UART_DATA_8_BITS,
		.parity = UART_PARITY_DISABLE,
		.stop_bits = UART_STOP_BITS_1,
		.flow_ctrl = UART_HW_FLOWCTRL_DISABLE,
		.rx_flow_ctrl_thresh = 122,
		.source_clk = UART_SCLK_APB

	};

	
	
	 ESP_ERROR_CHECK(uart_param_config(USART_UX, &uart_config));

	 ESP_ERROR_CHECK(uart_set_pin(USART_UX, 
								  USART_TX_GPIO_PIN,
								  USART_RX_GPIO_PIN, 
								  UART_PIN_NO_CHANGE, 
								  UART_PIN_NO_CHANGE));

	 ESP_ERROR_CHECK(uart_driver_install(USART_UX,
										 RX_BUF_SIZE, 
										 TX_BUF_SIZE,
										 20, 
										 NULL, 
										 0));
}




unsigned char data[RX_BUF_SIZE] = {0};
uint8_t len = 0;


void uart_process(void)
{

	uart_get_buffered_data_len(USART_UX, (size_t*)&len);

		if(len > 0){

			
			memset(data, 0, RX_BUF_SIZE);
			uart_read_bytes(USART_UX, data, len, 100);

			uart_write_bytes(USART_UX, (const char*)data, len);

		}

}