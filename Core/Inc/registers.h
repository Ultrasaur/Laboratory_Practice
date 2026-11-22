#ifndef REGISTERS_H
#define REGISTERS_H

#include <stdint.h>

/* Регистры тактирования */
#define RCC_AHB1ENR (*(volatile uint32_t*)0x40023830UL)

/* Регистры порта D */
#define GPIOD_MODER   (*(volatile uint32_t*)0x40020C00UL)
#define GPIOD_OTYPER  (*(volatile uint32_t*)0x40020C04UL)
#define GPIOD_OSPEEDR (*(volatile uint32_t*)0x40020C08UL)
#define GPIOD_PUPDR   (*(volatile uint32_t*)0x40020C0CUL)
#define GPIOD_BSRR    (*(volatile uint32_t*)0x40020C18UL)

/* Регистры порта E */
#define GPIOE_MODER   (*(volatile uint32_t*)0x40021000UL)
#define GPIOE_OTYPER  (*(volatile uint32_t*)0x40021004UL)
#define GPIOE_OSPEEDR (*(volatile uint32_t*)0x40021008UL)
#define GPIOE_PUPDR   (*(volatile uint32_t*)0x4002100CUL)
#define GPIOE_BSRR    (*(volatile uint32_t*)0x40021018UL)

/* Регистры порта F */
#define GPIOF_MODER   (*(volatile uint32_t*)0x40021400UL)
#define GPIOF_PUPDR   (*(volatile uint32_t*)0x4002140CUL)
#define GPIOF_IDR     (*(volatile uint32_t*)0x40021410UL)

#endif
