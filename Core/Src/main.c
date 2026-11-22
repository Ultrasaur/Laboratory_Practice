#include "main.h"
#include "registers.h"

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
