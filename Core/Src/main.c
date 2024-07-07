/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file           : main.c
  * @brief          : Main program body
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
#include "main.h"
#include "cmsis_os.h"
#include "dcmi.h"
#include "dma.h"
#include "dma2d.h"
#include "ltdc.h"
#include "usart.h"
#include "gpio.h"
#include "fmc.h"
#include "stm32746g_discovery_camera.h"
#include "stm32746g_discovery_lcd.h"
#include "stm32746g_discovery_sdram.h"
#include "ov9655.h"
#include <stdio.h>  // 包含标准输入输出库头文件
#include <stdlib.h>
#include <string.h>
/* Private function prototypes -----------------------------------------------*/
void SystemClock_Config(void);
void MX_FREERTOS_Init(void);
/* USER CODE BEGIN PFP */
DMA2D_HandleTypeDef hdma2d_eval;
static void LCD_LL_ConvertLineToARGB8888(void *pSrc, void *pDst);
static void CPU_CACHE_Enable(void);
/* USER CODE END PFP */
int fputc(int ch, FILE *f)
{
    HAL_UART_Transmit(&huart1, (uint8_t *)&ch, 1, HAL_MAX_DELAY);
    return ch;
}
/* Private user code ---------------------------------------------------------*/
/* USER CODE BEGIN 0 */

/* USER CODE END 0 */

/**
  * @brief  The application entry point.
  * @retval int
  */
int main(void)
{
  /* USER CODE BEGIN 1 */
  CPU_CACHE_Enable();
  /* USER CODE END 1 */


  /* MCU Configuration--------------------------------------------------------*/

  /* Reset of all peripherals, Initializes the Flash interface and the Systick. */
  HAL_Init();

  /* USER CODE BEGIN Init */

  /* USER CODE END Init */

  /* Configure the system clock */
  SystemClock_Config();

  /* USER CODE BEGIN SysInit */

  /* USER CODE END SysInit */

  /* Initialize all configured peripherals */
  MX_GPIO_Init();
  MX_DMA_Init();
  MX_FMC_Init();
  MX_LTDC_Init();
  MX_DCMI_Init();
  MX_DMA2D_Init();
  MX_USART6_UART_Init();
  MX_USART1_UART_Init();
  /* USER CODE BEGIN 2 */
  BSP_SDRAM_Init();
  
	BSP_LCD_Init();
	BSP_LCD_LayerDefaultInit(0, LCD_FRAME_BUFFER);// 初始化LCD的第0层，用于摄像头数据显示
	BSP_LCD_SelectLayer(0);// 选择LTDC活动层
	BSP_LCD_DisplayOn();
  /* Clear the LCD */
	
  BSP_LCD_Clear(LCD_COLOR_WHITE);

  /* 设置透明度和文字颜色 */
  BSP_LCD_SetTransparency(0, 255); // 设置第0层的透明度为255（不透明）
  BSP_LCD_SetTextColor(LCD_COLOR_BLUE); // 设置文字颜色为蓝色

  /* 在屏幕中间显示文字 */
	BSP_LCD_DisplayStringAt(0, BSP_LCD_GetYSize() / 2 - 20, (uint8_t*)"Smart Lawn Sweeper", CENTER_MODE);


	  /* 延时2秒 */
  HAL_Delay(2000);
	
    /* Clear the LCD */
	BSP_LCD_Clear(LCD_COLOR_BLACK);
  /* USER CODE END 2 */
	BSP_SDRAM_Init();
	BSP_CAMERA_Init(CAMERA_R480x272);  // 使用480x272分辨率初始化摄像头

  /* USER CODE END 2 */

  /* Init scheduler */
  osKernelInitialize();  /* Call init function for freertos objects (in freertos.c) */
  MX_FREERTOS_Init();
  /* Start scheduler */
  osKernelStart();
  /* We should never get here as control is now taken by the scheduler */
  /* Infinite loop */
  /* USER CODE BEGIN WHILE */
  while (1)
  {
    /* USER CODE END WHILE */

    /* USER CODE BEGIN 3 */
  }
  /* USER CODE END 3 */
}

/**
  * @brief System Clock Configuration
  * @retval None
  */
void SystemClock_Config(void)
{
  RCC_OscInitTypeDef RCC_OscInitStruct = {0};
  RCC_ClkInitTypeDef RCC_ClkInitStruct = {0};

  /** Configure LSE Drive Capability
  */
  HAL_PWR_EnableBkUpAccess();

  /** Configure the main internal regulator output voltage
  */
  __HAL_RCC_PWR_CLK_ENABLE();
  __HAL_PWR_VOLTAGESCALING_CONFIG(PWR_REGULATOR_VOLTAGE_SCALE1);

  /** Initializes the RCC Oscillators according to the specified parameters
  * in the RCC_OscInitTypeDef structure.
  */
  RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_HSE;
  RCC_OscInitStruct.HSEState = RCC_HSE_ON;
  RCC_OscInitStruct.PLL.PLLState = RCC_PLL_ON;
  RCC_OscInitStruct.PLL.PLLSource = RCC_PLLSOURCE_HSE;
  RCC_OscInitStruct.PLL.PLLM = 25;
  RCC_OscInitStruct.PLL.PLLN = 432;
  RCC_OscInitStruct.PLL.PLLP = 2;
  RCC_OscInitStruct.PLL.PLLQ = 9;
  if (HAL_RCC_OscConfig(&RCC_OscInitStruct) != HAL_OK)
  {
    Error_Handler();
  }

  /** Activate the Over-Drive mode
  */
  if (HAL_PWREx_EnableOverDrive() != HAL_OK)
  {
    Error_Handler();
  }

  /** Initializes the CPU, AHB and APB buses clocks
  */
  RCC_ClkInitStruct.ClockType = RCC_CLOCKTYPE_HCLK|RCC_CLOCKTYPE_SYSCLK
                              |RCC_CLOCKTYPE_PCLK1|RCC_CLOCKTYPE_PCLK2;
  RCC_ClkInitStruct.SYSCLKSource = RCC_SYSCLKSOURCE_PLLCLK;
  RCC_ClkInitStruct.AHBCLKDivider = RCC_SYSCLK_DIV1;
  RCC_ClkInitStruct.APB1CLKDivider = RCC_HCLK_DIV4;
  RCC_ClkInitStruct.APB2CLKDivider = RCC_HCLK_DIV2;

  if (HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_7) != HAL_OK)
  {
    Error_Handler();
  }
}

/* USER CODE BEGIN 4 */
/*下面是摄像头以及显示的函数*/

void CPU_CACHE_Enable(void)
{
    /* 使能I-Cache */
    SCB_EnableICache();

    /* 使能D-Cache */
    SCB_EnableDCache();
}


static void LCD_LL_ConvertLineToARGB8888(void *pSrc, void *pDst)
{
    /* 使能DMA2D时钟 */
    __HAL_RCC_DMA2D_CLK_ENABLE();

    /* 配置DMA2D模式、颜色模式和输出偏移量 */
    hdma2d_eval.Init.Mode         = DMA2D_M2M_PFC;
    hdma2d_eval.Init.ColorMode    = DMA2D_ARGB8888;
    hdma2d_eval.Init.OutputOffset = 0;

    /* 前景层配置 */
    hdma2d_eval.LayerCfg[1].AlphaMode = DMA2D_NO_MODIF_ALPHA;
    hdma2d_eval.LayerCfg[1].InputAlpha = 0xFF;
    hdma2d_eval.LayerCfg[1].InputColorMode = CM_RGB565;
    hdma2d_eval.LayerCfg[1].InputOffset = 0;

    hdma2d_eval.Instance = DMA2D;

    /* 初始化DMA2D */
    if (HAL_DMA2D_Init(&hdma2d_eval) == HAL_OK)
    {
        if (HAL_DMA2D_ConfigLayer(&hdma2d_eval, 1) == HAL_OK)
        {
            if (HAL_DMA2D_Start(&hdma2d_eval, (uint32_t)pSrc, (uint32_t)pDst, xsize, 1) == HAL_OK)
            {
                /* 等待DMA传输完成 */
                HAL_DMA2D_PollForTransfer(&hdma2d_eval, 10);
            }
        }
    }
    else
    {
        /* DMA2D初始化失败 */
        while (1);
    }
}

void BSP_CAMERA_LineEventCallback(void)
{
    static uint32_t tmp = 0, tmp2 = 0, counter = 0;

    if (ysize > counter)
    {
        // 将摄像头捕获的行数据转换为 ARGB8888 格式并显示在 LCD 上
        LCD_LL_ConvertLineToARGB8888((uint32_t *)(CAMERA_FRAME_BUFFER + tmp), (uint32_t *)(LCD_FRAME_BUFFER + tmp2));
        
        
        tmp += xsize * sizeof(uint16_t);
        tmp2 += xsize * sizeof(uint32_t);
        counter++;
    }
    else
    {
        // 重置计数器和缓冲区偏移量
        tmp = 0;
        tmp2 = 0;
        counter = 0;

        if (bufferReady == 0)
        {
						memcpy((void*)image_data, (const void*)LCD_FRAME_BUFFER, xsize * ysize * sizeof(uint32_t));
            bufferReady = 1;
            osSemaphoreRelease(imageReadySemaphore);
        }
    }
}

/*
void BSP_CAMERA_LineEventCallback(void)
{
    static uint32_t tmp = 0, tmp2 = 0, counter = 0;

    if (ysize > counter)
    {
        // 计算目标行在垂直翻转后的目标位置
        uint32_t flipped_line = ysize - 1 - counter;

        // 将摄像头捕获的行数据转换为 ARGB8888 格式并显示在 LCD 上（垂直翻转行数据）
        LCD_LL_ConvertLineToARGB8888((uint32_t *)(CAMERA_FRAME_BUFFER + tmp), (uint32_t *)(LCD_FRAME_BUFFER + flipped_line * xsize * sizeof(uint32_t)));
        
        tmp += xsize * sizeof(uint16_t);
        tmp2 += xsize * sizeof(uint32_t);
        counter++;
    }
    else
    {
        // 重置计数器和缓冲区偏移量
        tmp = 0;
        tmp2 = 0;
        counter = 0;

        if (bufferReady == 0)
        {
            memcpy((void*)image_data, (const void*)LCD_FRAME_BUFFER, xsize * ysize * sizeof(uint32_t));
            bufferReady = 1;
            osSemaphoreRelease(imageReadySemaphore);
        }
    }
}

*/

/* USER CODE END 4 */

/**
  * @brief  Period elapsed callback in non blocking mode
  * @note   This function is called  when TIM6 interrupt took place, inside
  * HAL_TIM_IRQHandler(). It makes a direct call to HAL_IncTick() to increment
  * a global variable "uwTick" used as application time base.
  * @param  htim : TIM handle
  * @retval None
  */
void HAL_TIM_PeriodElapsedCallback(TIM_HandleTypeDef *htim)
{
  /* USER CODE BEGIN Callback 0 */

  /* USER CODE END Callback 0 */
  if (htim->Instance == TIM6) {
    HAL_IncTick();
  }
  /* USER CODE BEGIN Callback 1 */

  /* USER CODE END Callback 1 */
}

/**
  * @brief  This function is executed in case of error occurrence.
  * @retval None
  */
void Error_Handler(void)
{
  /* USER CODE BEGIN Error_Handler_Debug */
  /* User can add his own implementation to report the HAL error return state */
  __disable_irq();
  while (1)
  {
  }
  /* USER CODE END Error_Handler_Debug */
}

#ifdef  USE_FULL_ASSERT
/**
  * @brief  Reports the name of the source file and the source line number
  *         where the assert_param error has occurred.
  * @param  file: pointer to the source file name
  * @param  line: assert_param error line source number
  * @retval None
  */
void assert_failed(uint8_t *file, uint32_t line)
{
  /* USER CODE BEGIN 6 */
  /* User can add his own implementation to report the file name and line number,
     ex: printf("Wrong parameters value: file %s on line %d\r\n", file, line) */
  /* USER CODE END 6 */
}
#endif /* USE_FULL_ASSERT */
