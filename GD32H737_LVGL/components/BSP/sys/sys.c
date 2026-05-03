#include "sys.h"

/**
 * @brief       Execute WFI instruction (enter low-power mode after execution, wait for interrupt wake-up)
 * @param       None
 * @retval      None
 */
void sys_wfi_set(void)
{
    __ASM volatile("wfi");
}

/**
 * @brief       Disable all interrupts (excluding fault and NMI interrupts)
 * @param       None
 * @retval      None
 */
void sys_intx_disable(void)
{
    __ASM volatile("cpsid i");
}

/**
 * @brief       Enable all interrupts
 * @param       None
 * @retval      None
 */
void sys_intx_enable(void)
{
    __ASM volatile("cpsie i");
}

/**
 * @brief       Set the main stack pointer address
 * @param       addr: Main stack top address
 * @retval      None
 */
void sys_msr_msp(uint32_t addr)
{
    __set_MSP(addr);        /* Set stack top address */
}

/**
 * @brief       System software reset
 * @param       None
 * @retval      None
 */
void sys_soft_reset(void)
{
    NVIC_SystemReset();
}

/**
 * @brief       Enable system cache
 * @param       None
 * @retval      None
 */
void sys_cache_enable(void)
{
    SCB_EnableICache();     /* Enable I-Cache */
    SCB_EnableDCache();     /* Enable D-Cache */
    SCB->CACR |= 1 << 2;    /* Force D-Cache write-through; avoid problems in actual use if disabled */
}

static uint32_t g_fac_us = 0;      /* Microsecond delay multiplier */

/**
 * @brief       Initialize delay function
 * @param       sysclk: System clock frequency (CPU frequency HCLK), unit: MHz
 * @retval      None
 */
void delay_init(uint16_t sysclk)
{

    SysTick->CTRL |= (1 << 2);                          /* Use system clock source for SYSTICK, frequency = HCLK */
    g_fac_us = sysclk ;                                 /* Basic time base for 1us, required for OS/non-OS use */
    SysTick->CTRL |= SysTick_CTRL_ENABLE_Msk;           /* Enable SYSTICK counter */
    SysTick->LOAD = 0XFFFFFF;                           /* Set maximum reload value (SYSTICK is 24-bit counter) */
}

/**
 * @brief       Delay for specified microseconds
 * @note        Use clock capture method for us delay, regardless of OS usage
 * @param       nus: Number of microseconds to delay
 * @note        nus range: 0 ~ (2^32 / g_fac_us) (g_fac_us equals system main frequency)
 * @retval      None
 */
void delay_us(uint32_t nus)
{
    uint32_t ticks;
    uint32_t told, tnow, tcnt = 0;
    uint32_t reload;
    reload = SysTick->LOAD;             /* Value of SYSTICK LOAD register */
    ticks = nus * g_fac_us;             /* Required counter ticks */

    told = SysTick->VAL;                /* Counter value at entry */
  
    while (1)
    {
        tnow = SysTick->VAL;

        if (tnow != told)
        {
            if (tnow < told)
            {
                tcnt += told - tnow;    /* SYSTICK is a down-counter */
            }
            else
            {
                tcnt += reload - tnow + told;
            }
            
            told = tnow;
            
            if (tcnt >= ticks) 
            {
                break;                  /* Exit when time reaches/exceeds the delay target */
            }
        }
    }
}

/**
 * @brief       Delay for specified milliseconds
 * @param       nms: Number of milliseconds to delay (0 < nms <= (2^32 / g_fac_us / 1000))
 * @retval      None
 */
void delay_ms(uint16_t nms)
{  
		uint32_t i;

		for (i=0; i<nms; i++)
		{
		   delay_us(1000);
		}
}
