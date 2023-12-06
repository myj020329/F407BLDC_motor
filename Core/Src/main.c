/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file           : main.c
  * @brief          : Main program body
  ******************************************************************************
  * @attention
  *
  * Copyright (c) 2023 STMicroelectronics.
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
#include "tim.h"
#include "usart.h"
#include "gpio.h"

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */
#include "hall.h"
#include "motor.h"
#include "stdio.h"
#include <math.h>

#define TIMECNT   800

uint8_t i = 0;

float MotorSpeed  = 0.0f ;// ���ת��,������ռ�ձ�,
_Bool isTimeUp    = 0;     // ��ʱ���
uint32_t timeTick = 0;   // ��ʱ���
float Speed_hz    = 0 ;

extern MotorSta_Typedef Motor_State; // ���ʹ��״̬
extern MotorDir_Typedef Motor_Dir;  // ������� ,˳ʱ��
extern float PWM_Duty;        // 25%ռ�ձ�

extern MotorDir_Typedef RT_hallDir; // ����˳��õ��ĵ��ת������


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

/* USER CODE BEGIN PV */

/* USER CODE END PV */

/* Private function prototypes -----------------------------------------------*/
void SystemClock_Config(void);
/* USER CODE BEGIN PFP */

/* USER CODE END PFP */

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
  MX_TIM5_Init();
  MX_TIM8_Init();
  MX_TIM6_Init();
  MX_USART1_UART_Init();
  /* USER CODE BEGIN 2 */
    Motor_Dir = MOTOR_DIR_CW;
//	MotorSpeed = 0.50f;
//    BLDCMotor_SetSpeed(MotorSpeed);
//    BLDCMotor_Start();
  /* USER CODE END 2 */

  /* Infinite loop */
  /* USER CODE BEGIN WHILE */
  while (1)
  {
    /* USER CODE END WHILE */

    /* USER CODE BEGIN 3 */
	   //�����������ռ�ձ�15%
	if(Key_Scan(KEY1_GPIO_Port,KEY1_Pin) == KEY_ON)
	{
	  MotorSpeed = 0.15f;
      BLDCMotor_SetSpeed(MotorSpeed);
      BLDCMotor_Start();
      printf("����KEY1�������\n");
	}
	
	//����ռ�ձ�
	if( Key_Scan(KEY2_GPIO_Port,KEY2_Pin) == KEY_ON)
	{
		MotorSpeed += 0.05f;
      BLDCMotor_SetSpeed(MotorSpeed);
      printf("�������\n");
	}
	
	//��Сռ�ձ�
	if( Key_Scan(KEY3_GPIO_Port,KEY3_Pin) == KEY_ON)
	{
		 MotorSpeed -= 0.05f;
      BLDCMotor_SetSpeed(MotorSpeed);
      printf("�������\n");
	}
	
	//���Ƶ������
	if( Key_Scan(KEY4_GPIO_Port,KEY4_Pin) == KEY_ON)
	{
		/* ������ֹͣ,Ȼ�������� */
      if(Motor_State == MOTOR_DISABLE)
      {
        if(Motor_Dir == MOTOR_DIR_CW)
          Motor_Dir = MOTOR_DIR_CCW;
        else 
          Motor_Dir = MOTOR_DIR_CW;
        printf("��Ҫ�����������\n");
      }
      else
        printf("�����������,������ֹͣ\n");
	}
	
	
	//ֹͣ���
	if( Key_Scan(KEY5_GPIO_Port,KEY5_Pin) == KEY_ON)
	{
	  BLDCMotor_braking_LowerShort();
      HAL_Delay(100);// ��100ms, ���ͣ����֮���ȡ��ɲ������
      BLDCMotor_unbraking_LS();
      printf("ɲ��ֹͣ\n");
	}
	
//	if( isTimeUp )
//    {
//      /* ����õ��Ļ����ź�Ƶ�� */
//      if( RT_hallcomp == 0 )
//        Speed_hz = 0;
//      else
//        Speed_hz = (float)(HALL_TIM_FREQ)/(float)(RT_hallcomp);
//      RT_hallcomp = 0;
//      
//      /* �����źŵ�Ƶ��,ת��rps,ת��rpm */
//      printf("%.3f Hz, %.2f RPS, %.2fRPM\n",Speed_hz, Speed_hz/24, (Speed_hz/24)*60);

//      isTimeUp = 0;
//      timeTick = TIMECNT;
//    }
	
	if( isTimeUp )
    {
      uint32_t tmpCC = 0;
      if(RT_hallcnt == 0) // �������Ϊ0
      {
        Speed_hz = 0;
      }
      else 
      {
        tmpCC = RT_hallcomp / RT_hallcnt; // tmpCC:���β���֮��Ĳ���ֵ,
        Speed_hz = (float)HALL_TIM_FREQ/(float)(tmpCC);
      }
      RT_hallcomp = 0;
      RT_hallcnt  = 0;
      /* �����źŵ�Ƶ��,ת��rps,ת��rpm */
      if(RT_hallDir == MOTOR_DIR_CW)
        Speed_hz = fabs(Speed_hz);
      else
        Speed_hz = -fabs(Speed_hz);
      /* δ���κ��˲����ٶ�ֵ */
      printf("%.3f Hz, %.2f RPS, %.2fRPM\n", Speed_hz, Speed_hz/24, (Speed_hz/24)*60);
      
      isTimeUp = 0;
      timeTick = TIMECNT;
    }
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

  /** Configure the main internal regulator output voltage
  */
  __HAL_RCC_PWR_CLK_ENABLE();
  __HAL_PWR_VOLTAGESCALING_CONFIG(PWR_REGULATOR_VOLTAGE_SCALE1);

  /** Initializes the RCC Oscillators according to the specified parameters
  * in the RCC_OscInitTypeDef structure.
  */
  RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_HSI;
  RCC_OscInitStruct.HSIState = RCC_HSI_ON;
  RCC_OscInitStruct.HSICalibrationValue = RCC_HSICALIBRATION_DEFAULT;
  RCC_OscInitStruct.PLL.PLLState = RCC_PLL_ON;
  RCC_OscInitStruct.PLL.PLLSource = RCC_PLLSOURCE_HSI;
  RCC_OscInitStruct.PLL.PLLM = 8;
  RCC_OscInitStruct.PLL.PLLN = 168;
  RCC_OscInitStruct.PLL.PLLP = RCC_PLLP_DIV2;
  RCC_OscInitStruct.PLL.PLLQ = 4;
  if (HAL_RCC_OscConfig(&RCC_OscInitStruct) != HAL_OK)
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

  if (HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_5) != HAL_OK)
  {
    Error_Handler();
  }
  HAL_RCC_EnableCSS();                                            // ʹ��CSS���ܣ�����ʹ���ⲿ�����ڲ�ʱ��ԴΪ����
  // HAL_RCC_GetHCLKFreq()/1000    1ms�ж�һ��
  // HAL_RCC_GetHCLKFreq()/100000	 10us�ж�һ��
  // HAL_RCC_GetHCLKFreq()/1000000 1us�ж�һ��
  HAL_SYSTICK_Config(HAL_RCC_GetHCLKFreq() / 1000);               // ���ò�����ϵͳ�δ�ʱ��
  /* ϵͳ�δ�ʱ��ʱ��Դ */
  HAL_SYSTICK_CLKSourceConfig(SYSTICK_CLKSOURCE_HCLK);
  /* ϵͳ�δ�ʱ���ж����ȼ����� */
  HAL_NVIC_SetPriority(SysTick_IRQn, 0, 0);
}

/* USER CODE BEGIN 4 */
void HAL_SYSTICK_Callback()
{
  if(timeTick != 0)
    timeTick--;
  if(timeTick == 0)
  {
    isTimeUp = 1;
  }
}
/* USER CODE END 4 */

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