/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * File Name          : freertos.c
  * Description        : Code for freertos applications
  ******************************************************************************
  * @attention
  *
  * Copyright (c) 2024 STMicroelectronics.
  * All rights reserved.
  *
  * This software is licensed under terms that can be found in the LICENSE file
  * in the root directory of this software component.
  * If no LICENSE file comes with this software, it is provided AS-IS.
  *
  ******************************************************************************
  */
/* USER CODE END Header */

/* Includes ------------------------------------------------------------------*/
#include "FreeRTOS.h"
#include "task.h"
#include "main.h"
#include "cmsis_os.h"

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */
#include "usart.h"
#include "MyUDP.h"

/* USER CODE END Includes */

/* Private typedef -----------------------------------------------------------*/
/* USER CODE BEGIN PTD */

/* USER CODE END PTD */

/* Private define ------------------------------------------------------------*/
/* USER CODE BEGIN PD */

/* USER CODE END PD */

/* Private macro -------------------------------------------------------------*/
/* USER CODE BEGIN PM */

/* USER CODE END PM */

/* Private variables ---------------------------------------------------------*/
/* USER CODE BEGIN Variables */

/* USER CODE END Variables */
/* Definitions for defaultTask */
osThreadId_t defaultTaskHandle;
const osThreadAttr_t defaultTask_attributes = {
  .name = "defaultTask",
  .stack_size = 128 * 4,
  .priority = (osPriority_t) osPriorityNormal,
};

/* Private function prototypes -----------------------------------------------*/
/* USER CODE BEGIN FunctionPrototypes */
void task0(void);
void w5500_task(void);
/* USER CODE END FunctionPrototypes */

void StartDefaultTask(void *argument);

void MX_FREERTOS_Init(void); /* (MISRA C 2004 rule 8.1) */

/**
  * @brief  FreeRTOS initialization
  * @param  None
  * @retval None
  */
void MX_FREERTOS_Init(void) {
  /* USER CODE BEGIN Init */

  /* USER CODE END Init */

  /* USER CODE BEGIN RTOS_MUTEX */
  /* add mutexes, ... */
  /* USER CODE END RTOS_MUTEX */

  /* USER CODE BEGIN RTOS_SEMAPHORES */
  /* add semaphores, ... */
  /* USER CODE END RTOS_SEMAPHORES */

  /* USER CODE BEGIN RTOS_TIMERS */
  /* start timers, add new ones, ... */
  /* USER CODE END RTOS_TIMERS */

  /* USER CODE BEGIN RTOS_QUEUES */
  /* add queues, ... */
  /* USER CODE END RTOS_QUEUES */

  /* Create the thread(s) */
  /* creation of defaultTask */
  defaultTaskHandle = osThreadNew(StartDefaultTask, NULL, &defaultTask_attributes);

  /* USER CODE BEGIN RTOS_THREADS */
  /* add threads, ... */
  xTaskCreate(task0, "task0", 128, NULL, osPriorityNormal, NULL);
  xTaskCreate(w5500_main, "task1", 128, NULL, osPriorityNormal, NULL);
  /* USER CODE END RTOS_THREADS */

  /* USER CODE BEGIN RTOS_EVENTS */
  /* add events, ... */
  /* USER CODE END RTOS_EVENTS */

}

/* USER CODE BEGIN Header_StartDefaultTask */
/**
  * @brief  Function implementing the defaultTask thread.
  * @param  argument: Not used
  * @retval None
  */
/* USER CODE END Header_StartDefaultTask */
void StartDefaultTask(void *argument)
{
  /* USER CODE BEGIN StartDefaultTask */
  /* Infinite loop */
  for(;;)
  {
    HAL_GPIO_TogglePin(GPIOB, GPIO_PIN_0);
    osDelay(1000);
  }
  /* USER CODE END StartDefaultTask */
}

/* Private application code --------------------------------------------------*/
/* USER CODE BEGIN Application */
void task0(void){
  char str[] = "hello dap!!\n";
  for(;;){
    HAL_UART_Transmit(&huart1, str, sizeof(str)/sizeof(str[0]), HAL_MAX_DELAY);
    vTaskDelay(1000);
  }
}
#define UDP 0
#define UDP1 1
#define TCPS 2
#define TCPS1 3
#define TCPC  4
#define TCPC1 5

void w5500_task(void){
  char str[] = "hello w5500\n";
  char udp[] = "this is udp\n";
  char udp1[] = "this is udp1\n";
  char tcps[] = "this is tcp server\n";
  char tcps1[] = "this is tcp server 1\n";
  char tcpc[] = "this is tcp client\n";
  char tcpc1[] = "this is tcp client1\n";
  int select = 3; // 1:udp1 0:udp
  int procotol = TCPS;
  
  UDPinit();
  
  while(1){

    switch(procotol){
      case UDP:
        HAL_UART_Transmit(&huart1, udp, sizeof(udp)/sizeof(udp[0]), HAL_MAX_DELAY);
        do_udp();
        break;
      case UDP1:
        HAL_UART_Transmit(&huart1, udp1, sizeof(udp1)/sizeof(udp1[0]), HAL_MAX_DELAY);
        do_udp1();
        break;
      case TCPS:
        HAL_UART_Transmit(&huart1, tcps, sizeof(tcps)/sizeof(tcps[0]), HAL_MAX_DELAY);
        do_tcp_server();
        break;
      case TCPS1:
        HAL_UART_Transmit(&huart1, tcps1, sizeof(tcps1)/sizeof(tcps1[0]), HAL_MAX_DELAY);
        do_tcp_server1();
        break;
      case TCPC:
        HAL_UART_Transmit(&huart1, tcpc, sizeof(tcpc)/sizeof(tcpc[0]), HAL_MAX_DELAY);
        do_tcp_client();
        break;
      case TCPC1:
        HAL_UART_Transmit(&huart1, tcpc1, sizeof(tcpc1)/sizeof(tcpc1[0]), HAL_MAX_DELAY);
        do_tcp_client1();
        vTaskDelay(1000);
        break;
    }

    // vTaskDelay(500);

  }
}
/* USER CODE END Application */

