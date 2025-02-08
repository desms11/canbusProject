/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file           : main.c
  * @brief          : Main program body
  ******************************************************************************
  */
/* USER CODE END Header */

/* Includes ------------------------------------------------------------------*/
#include "main.h"
#include "stm32f1xx_hal.h"

/* Private variables ---------------------------------------------------------*/
CAN_HandleTypeDef hcan;
CAN_TxHeaderTypeDef TxHeader;
uint8_t TxData[8] = {0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00};
uint32_t TxMailbox;

/* Private function prototypes -----------------------------------------------*/
void SystemClock_Config(void);
static void MX_GPIO_Init(void);
static void MX_CAN_Init(void);
void transmit_can_message(void);

/* Private user code ---------------------------------------------------------*/
int main(void)
{
  HAL_Init();
  SystemClock_Config();
  MX_GPIO_Init();
  MX_CAN_Init();

  HAL_CAN_Start(&hcan);

  TxHeader.DLC = 8;  // Data Length Code (8 bytes)
  TxHeader.IDE = CAN_ID_STD;  // Standard ID
  TxHeader.RTR = CAN_RTR_DATA;  // Data Frame
  TxHeader.StdId = 0x123;  // CAN ID

  /* Infinite loop */
  while (1)
  {
    transmit_can_message();
    HAL_Delay(3000);  // Wait for 3 seconds
  }
}

void SystemClock_Config(void)
{
  // System clock configuration
}

static void MX_CAN_Init(void)
{
  hcan.Instance = CAN1;
  hcan.Init.Prescaler = 4;
  hcan.Init.Mode = CAN_MODE_NORMAL;
  hcan.Init.SyncJumpWidth = CAN_SJW_1TQ;
  hcan.Init.TimeSeg1 = CAN_BS1_6TQ;
  hcan.Init.TimeSeg2 = CAN_BS2_2TQ;
  hcan.Init.TimeTriggeredMode = DISABLE;
  hcan.Init.AutoBusOff = DISABLE;
  hcan.Init.AutoWakeUp = DISABLE;
  hcan.Init.AutoRetransmission = DISABLE;
  hcan.Init.ReceiveFifoLocked = DISABLE;
  hcan.Init.TransmitFifoPriority = DISABLE;

  if (HAL_CAN_Init(&hcan) != HAL_OK)
  {
    Error_Handler();
  }
}

static void MX_GPIO_Init(void)
{
  __HAL_RCC_GPIOD_CLK_ENABLE();
  __HAL_RCC_GPIOA_CLK_ENABLE();
}

void transmit_can_message(void)
{
  // Toggle TxData[1] between 0x00 and 0x11
  TxData[1] = (TxData[1] == 0x00) ? 0x11 : 0x00;

  // Transmit CAN message
  if (HAL_CAN_AddTxMessage(&hcan, &TxHeader, TxData, &TxMailbox) != HAL_OK)
  {
    // Transmission error
    Error_Handler();
  }
}

void Error_Handler(void)
{
  __disable_irq();
  while (1)
  {
  }
}
