#include "beep.h"

/**
 * @brief  ³õÊ¼»¯·äÃùÆ÷
 * @param  beep: ·äÃùÆ÷½á¹¹ÌåÖ¸Õë
 * @param  GPIOx: GPIO¶Ë¿Ú
 * @param  GPIO_Pin: GPIOÒı½Å
 * @retval ÎŞ
 */
void Beep_Init(Beep_TypeDef* beep, GPIO_TypeDef* GPIOx, uint16_t GPIO_Pin) {
    GPIO_InitTypeDef GPIO_InitStructure;
    uint32_t RCC_APB2Periph_GPIOx = 0;
    
    // ¸ù¾İGPIO¶Ë¿ÚÈ·¶¨RCCÊ±ÖÓ
    if (GPIOx == GPIOA) {
        RCC_APB2Periph_GPIOx = RCC_APB2Periph_GPIOA;
    } else if (GPIOx == GPIOB) {
        RCC_APB2Periph_GPIOx = RCC_APB2Periph_GPIOB;
    } else if (GPIOx == GPIOC) {
        RCC_APB2Periph_GPIOx = RCC_APB2Periph_GPIOC;
    } else if (GPIOx == GPIOD) {
        RCC_APB2Periph_GPIOx = RCC_APB2Periph_GPIOD;
    } else if (GPIOx == GPIOE) {
        RCC_APB2Periph_GPIOx = RCC_APB2Periph_GPIOE;
    }
    
    // Ê¹ÄÜGPIOÊ±ÖÓ
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOx, ENABLE);
    
    // ÅäÖÃGPIOÎªÍÆÍìÊä³ö
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_Init(GPIOx, &GPIO_InitStructure);
    
    // ³õÊ¼»¯·äÃùÆ÷½á¹¹Ìå
    beep->GPIOx = GPIOx;
    beep->GPIO_Pin = GPIO_Pin;
    beep->state = BEEP_OFF;
    
    // Ä¬ÈÏ¹Ø±Õ·äÃùÆ÷
    GPIO_SetBits(GPIOx, GPIO_Pin);
}

/**
 * @brief  ´ò¿ª·äÃùÆ÷
 * @param  beep: ·äÃùÆ÷½á¹¹ÌåÖ¸Õë
 * @retval ÎŞ
 */
void Beep_On(Beep_TypeDef* beep) {
    GPIO_ResetBits(beep->GPIOx, beep->GPIO_Pin);
    beep->state = BEEP_ON;
}

/**
 * @brief  ¹Ø±Õ·äÃùÆ÷
 * @param  beep: ·äÃùÆ÷½á¹¹ÌåÖ¸Õë
 * @retval ÎŞ
 */
void Beep_Off(Beep_TypeDef* beep) {
    GPIO_SetBits(beep->GPIOx, beep->GPIO_Pin);
    beep->state = BEEP_OFF;
}

/**
 * @brief  ÇĞ»»·äÃùÆ÷×´Ì¬
 * @param  beep: ·äÃùÆ÷½á¹¹ÌåÖ¸Õë
 * @retval ÎŞ
 */
void Beep_Toggle(Beep_TypeDef* beep) {
    if (beep->state == BEEP_ON) {
        Beep_Off(beep);
    } else {
        Beep_On(beep);
    }
}

/**
 * @brief  »ñÈ¡·äÃùÆ÷×´Ì¬
 * @param  beep: ·äÃùÆ÷½á¹¹ÌåÖ¸Õë
 * @retval ·äÃùÆ÷×´Ì¬
 */
Beep_State Beep_GetState(Beep_TypeDef* beep) {
    return beep->state;
}
