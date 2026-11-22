#ifndef MAIN_H
#define MAIN_H

#include <stdint.h>

/* Количество светодиодов */
#define LED_COUNT                 6U

/* Настройка портов */
#define GPIO_CLOCK_ENABLE_MASK    0x38UL

#define GPIOD_LED_FIELDS_MASK     0x00003F00UL
#define GPIOD_LED_OUTPUT_MODE     0x00001500UL
#define GPIOE_LED_FIELDS_MASK     0x00000FC0UL
#define GPIOE_LED_OUTPUT_MODE     0x00000540UL

#define GPIOF_BUTTON_FIELDS_MASK  0x0000F000UL
#define GPIOF_BUTTON_PULLUP       0x00005000UL

/* Настройка системного таймера */
#define SYSTICK_TICK_RATE_HZ      1000UL
#define SYSTICK_CONTROL_VALUE     0x05UL
#define SYSTICK_COUNTFLAG_MASK    (1UL << 16)

/* Кнопки */
#define BUTTON1_MASK              (1UL << 6)
#define BUTTON2_MASK              (1UL << 7)
#define DEBOUNCE_TIME_MS          20U

/* Светодиоды */
#define LED_PORT_D_MASK           0x70UL
#define LED_PORT_E_MASK           0x38UL

void initGPIO(void);
void initSysTick(void);
void delay1ms(void);

#endif
