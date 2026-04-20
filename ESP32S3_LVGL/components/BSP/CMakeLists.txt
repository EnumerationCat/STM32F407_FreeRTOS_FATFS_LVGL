set(src_dirs led/led.c
			 uart/uart.c
			 esptim/esptim.c
			 gptim/gptim.c
			 spi/spi.c

			 lcd/lcd.c
)

set(include_dirs led
				 uart
				 esptim
				 gptim
				 spi
				 lcd
)

set(requires driver
			 esp_timer
)

idf_component_register(SRCS ${src_dirs}
						   INCLUDE_DIRS ${include_dirs}
						   REQUIRES ${requires}
)


component_compile_options(-ffast-math -O3 -Wno-error=format=-Wno-format)