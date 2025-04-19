#include "main.h"
#include <math.h>
#include <stdio.h>
#include "LCD.h"
#include "ADC.h"

int uart_buf_len4;
char uart_buf4[50];

// Convert normal integer into BCD (binary coded decimal)
uint8_t int_to_bcd(uint16_t number, uint8_t bcd) {
	// ======= Convert the number directly to BCD =======
	uint8_t tens = (number / 10) & 0x0F;  // Get tens digit (4)
	uint8_t ones = (number % 10) & 0x0F;  // Get ones digit (2)
	bcd = (tens << 4) | ones;             // Pack into 8-bit BCD (0100 0010)
    return (bcd);
}

// BCD Processing function
void bcdProcessing(uint16_t number, UART_HandleTypeDef* huart2, UART_HandleTypeDef* huart3) {
    uint8_t bcd=0;

    // Convert number into BCD
    bcd = int_to_bcd(number,bcd);

    // Send the BCD over RS485    HAL_GPIO_WritePin(GPIOC, GPIO_PIN_11, GPIO_PIN_SET); // DE HIGH - Enable Transmit
    HAL_UART_Transmit(huart3, &bcd, 1, HAL_MAX_DELAY); // Send 1 byte
    HAL_GPIO_WritePin(GPIOC, GPIO_PIN_11, GPIO_PIN_RESET); // DE LOW - Disable Transmit;

    // ======= Print BCD value on debug UART =======
    uart_buf_len4 = sprintf(uart_buf4, "\nBCD (hex): 0x%02X\n", bcd);
    HAL_UART_Transmit(huart2, (uint8_t *)uart_buf4, uart_buf_len4, 100);
    HAL_Delay(1000); // Small delay

}
