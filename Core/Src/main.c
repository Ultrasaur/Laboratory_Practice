#include "registers.h"

#define LED_COUNT             6U
#define BUTTON1_MASK          (1UL << 6)
#define BUTTON2_MASK          (1UL << 7)
#define BUTTON_MASK           (BUTTON1_MASK | BUTTON2_MASK)
#define DEBOUNCE_TIME_MS      20U

#define LED_PORT_D_MASK       0x70UL
#define LED_PORT_E_MASK       0x38UL

extern uint32_t SystemCoreClock;

static void initSysTick(void)
{
    SYST_RVR = (SystemCoreClock / 1000UL) - 1UL;
    SYST_CVR = 0UL;
    SYST_CSR = 0x05UL;
}

static void delay1ms(void)
{
    while ((SYST_CSR & (1UL << 16)) == 0UL)
    {
    }
}

static void initGPIO(void)
{
    uint32_t clockStatus;

    /* Включаем тактирование портов D, E и F */
    RCC_AHB1ENR |= 0x38UL;
    clockStatus = RCC_AHB1ENR;
    (void)clockStatus;

    /* Сбрасываем состояние светодиодов */
    GPIOD_BSRR = LED_PORT_D_MASK << 16;
    GPIOE_BSRR = LED_PORT_E_MASK << 16;

    /* Настраиваем PD4-PD6 как выходы */
    GPIOD_MODER = (GPIOD_MODER & ~0x00003F00UL) | 0x00001500UL;
    GPIOD_OTYPER &= ~LED_PORT_D_MASK;
    GPIOD_OSPEEDR &= ~0x00003F00UL;
    GPIOD_PUPDR &= ~0x00003F00UL;

    /* Настраиваем PE3-PE5 как выходы */
    GPIOE_MODER = (GPIOE_MODER & ~0x00000FC0UL) | 0x00000540UL;
    GPIOE_OTYPER &= ~LED_PORT_E_MASK;
    GPIOE_OSPEEDR &= ~0x00000FC0UL;
    GPIOE_PUPDR &= ~0x00000FC0UL;

    /* Настраиваем PF6 и PF7 как входы с подтяжкой к питанию */
    GPIOF_MODER &= ~0x0000F000UL;
    GPIOF_PUPDR = (GPIOF_PUPDR & ~0x0000F000UL) | 0x00005000UL;
}

static void setLeds(uint8_t ledCount)
{
    uint32_t portDSet = 0UL;
    uint32_t portESet = 0UL;

    if (ledCount >= 1U) portDSet |= (1UL << 4);
    if (ledCount >= 2U) portDSet |= (1UL << 5);
    if (ledCount >= 3U) portDSet |= (1UL << 6);

    if (ledCount >= 4U) portESet |= (1UL << 3);
    if (ledCount >= 5U) portESet |= (1UL << 4);
    if (ledCount >= 6U) portESet |= (1UL << 5);

    /* Сначала выключаем все светодиоды, затем включаем нужные */
    GPIOD_BSRR = (LED_PORT_D_MASK << 16) | portDSet;
    GPIOE_BSRR = (LED_PORT_E_MASK << 16) | portESet;
}

static uint8_t buttonPressed(uint32_t pinMask,
                             uint8_t *stableState,
                             uint8_t *counter)
{
    uint8_t currentState = (GPIOF_IDR & pinMask) != 0UL;

    if (currentState == *stableState)
    {
        *counter = 0U;
        return 0U;
    }

    if (*counter < DEBOUNCE_TIME_MS)
    {
        (*counter)++;
    }

    if (*counter >= DEBOUNCE_TIME_MS)
    {
        *stableState = currentState;
        *counter = 0U;

        if (currentState == 0U)
        {
            return 1U;
        }
    }

    return 0U;
}

int main(void)
{
    uint8_t groupSize = 1U;
    uint8_t ledCount = 0U;

    uint8_t button1State;
    uint8_t button2State;
    uint8_t button1Counter = 0U;
    uint8_t button2Counter = 0U;

    initGPIO();
    initSysTick();

    button1State = (GPIOF_IDR & BUTTON1_MASK) != 0UL;
    button2State = (GPIOF_IDR & BUTTON2_MASK) != 0UL;

    setLeds(ledCount);

    while (1)
    {
        delay1ms();

        if (buttonPressed(BUTTON1_MASK, &button1State, &button1Counter))
        {
            groupSize++;

            if (groupSize > LED_COUNT)
            {
                groupSize = 1U;
            }

            ledCount = 0U;
            setLeds(ledCount);
        }

        if (buttonPressed(BUTTON2_MASK, &button2State, &button2Counter))
        {
            if (ledCount == LED_COUNT)
            {
                ledCount = 0U;
            }
            else
            {
                ledCount += groupSize;

                if (ledCount > LED_COUNT)
                {
                    ledCount = LED_COUNT;
                }
            }

            setLeds(ledCount);
        }
    }
}
