#include "main.h"
#include "registers.h"

extern uint32_t SystemCoreClock;

void initGPIO(void)
{
    uint32_t clockStatus;

    /* Включаем тактирование портов D, E и F */
    RCC_AHB1ENR |= GPIO_CLOCK_ENABLE_MASK;
    clockStatus = RCC_AHB1ENR;
    (void)clockStatus;

    /* Сбрасываем состояние светодиодов */
    GPIOD_BSRR = LED_PORT_D_MASK << 16;
    GPIOE_BSRR = LED_PORT_E_MASK << 16;

    /* Настраиваем PD4-PD6 как выходы */
    GPIOD_MODER = (GPIOD_MODER & ~GPIOD_LED_FIELDS_MASK) | GPIOD_LED_OUTPUT_MODE;
    GPIOD_OTYPER &= ~LED_PORT_D_MASK;
    GPIOD_OSPEEDR &= ~GPIOD_LED_FIELDS_MASK;
    GPIOD_PUPDR &= ~GPIOD_LED_FIELDS_MASK;

    /* Настраиваем PE3-PE5 как выходы */
    GPIOE_MODER = (GPIOE_MODER & ~GPIOE_LED_FIELDS_MASK) | GPIOE_LED_OUTPUT_MODE;
    GPIOE_OTYPER &= ~LED_PORT_E_MASK;
    GPIOE_OSPEEDR &= ~GPIOE_LED_FIELDS_MASK;
    GPIOE_PUPDR &= ~GPIOE_LED_FIELDS_MASK;

    /* Настраиваем PF6 и PF7 как входы с подтяжкой к питанию */
    GPIOF_MODER &= ~GPIOF_BUTTON_FIELDS_MASK;
    GPIOF_PUPDR = (GPIOF_PUPDR & ~GPIOF_BUTTON_FIELDS_MASK) | GPIOF_BUTTON_PULLUP;
}

void initSysTick(void)
{
    SYST_RVR = (SystemCoreClock / SYSTICK_TICK_RATE_HZ) - 1UL;
    SYST_CVR = 0UL;
    SYST_CSR = SYSTICK_CONTROL_VALUE;
}

void delay1ms(void)
{
    while ((SYST_CSR & SYSTICK_COUNTFLAG_MASK) == 0UL)
    {
    }
}
