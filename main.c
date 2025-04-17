/* USER CODE BEGIN Header */
 /**
 ******************************************************************************
 * @file
 : main.c
 * @brief
 : Main program body
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
 /* USER CODE ENDHeader */
 /* Includes------------------------------------------------------------------*/
 #include "main.h"
 #include "cmsis_os.h"
 /* Private includes----------------------------------------------------------*/
 /* USER CODE BEGIN Includes */
 #include <stdbool.h>
 #include <stdio.h>
 #include "string.h"
 #include "FreeRTOS.h"
 #include "task.h"
 //#include "timers.h"
 #include "queue.h"
 #include "semphr.h"
 //#include "event_groups.h"
 /* USER CODE ENDIncludes */
 /* Private typedef-----------------------------------------------------------*/
 /* USER CODE BEGIN PTD*/
 /* USER CODE ENDPTD*/
/* Private define------------------------------------------------------------*/
 /* USER CODE BEGIN PD*/
 #define NUM_LIST_SIZE 4
 /* USER CODE ENDPD*/
 /* Private macro-------------------------------------------------------------*/
 /* USER CODE BEGIN PM*/
 /* USER CODE ENDPM*/
 /* Private variables---------------------------------------------------------*/
 I2C_HandleTypeDef hi2c1;
 SPI_HandleTypeDef hspi1;
 SPI_HandleTypeDef hspi2;
 UART_HandleTypeDef huart2;
 osThreadId defaultTaskHandle;
 /* USER CODE BEGIN PV*/
 /* USER CODE ENDPV*/
 /* Private function prototypes-----------------------------------------------*/
 void SystemClock_Config(void);
 static void MX_GPIO_Init(void);
 static void MX_USART2_UART_Init(void);
 static void MX_I2C1_Init(void);
 static void MX_SPI1_Init(void);
 static void MX_SPI2_Init(void);
 void StartDefaultTask(void const * argument);
 /* USER CODE BEGIN PFP */
 bool password_mode = false;
 char password[5] = "0000";
 5바이트)
 uint8_t current_digit = 0;
 // 패스워드입력모드플래그
// 입력 중인패스워드문자열(null문자포함하여
// 현재 자릿수(0~3)
 int num_list[NUM_LIST_SIZE] = {12343, 43214, 88442, 12741};
 int num_list_size = NUM_LIST_SIZE;
 uint8_t required_person_count = 0; // 필요한 사람 수
uint32_t required_password = 0; // 필요한 비밀번호
bool access_granted = false;
 // 비밀번호가성공적으로입력되었는지여부
/* USER CODE ENDPFP*/
 /* Private user code---------------------------------------------------------*/
 /* USER CODE BEGIN 0*/
 volatile uint32_t person_count = 0;
 // 지나간사람수
GPIO_PinState pir_state_previous = GPIO_PIN_RESET; // PIR 센서 이전 상태
SemaphoreHandle_t xLcdMutex;
 //SemaphoreHandle_t SimpleMutex;
 TaskHandle_t PIRTaskHandle;
 TaskHandle_t HPT_Handler;
 TaskHandle_t LPT_Handler;
 //TaskHandle_t MPT_Handler;
 void PIR_Task(void *argument);
 void HPT_Task (void *argument);
 void LPT_Task (void *argument);
 //void MPT_Task (void *argument);
 /* USER CODE END0*/
 /**
 * @brief The application entry point.
 * @retval int
 */
 int main(void)
 {
 /* USER CODE BEGIN 1*/
 /* USER CODE END1*/
 /* MCU Configuration--------------------------------------------------------*/
 /* Reset of all peripherals, Initializes the Flash interface and the Systick. */
 HAL_Init();
 /* USER CODE BEGIN Init */
 /* USER CODE ENDInit */
 /* Configure the system clock */
SystemClock_Config();
 /* USER CODE BEGIN SysInit */
 /* USER CODE ENDSysInit */
 /* Initialize all configured peripherals */
 MX_GPIO_Init();
 MX_USART2_UART_Init();
 MX_I2C1_Init();
 MX_SPI1_Init();
 MX_SPI2_Init();
 /* USER CODE BEGIN 2*/
 xLcdMutex = xSemaphoreCreateMutex();
 //SimpleMutex = xSemaphoreCreateMutex();
 if( xLcdMutex != NULL)
 {
 HAL_UART_Transmit(&huart2, "Mutex Created\n\n", 15 , 1000);
 }
 // create tasks
 xTaskCreate(PIR_Task, "PIR_Task", 256, NULL, 3, &PIRTaskHandle);
 xTaskCreate(HPT_Task,"HPT", 256,NULL,2,&HPT_Handler);
 xTaskCreate(LPT_Task,"LPT", 256,NULL,1,&LPT_Handler);
 //xTaskCreate(MPT_Task,"MPT", 128,NULL,0,&MPT_Handler);
 vTaskStartScheduler();
 /* USER CODE END2*/
 /* USER CODE BEGIN RTOS_MUTEX */
 /* add mutexes, ... */
 /* USER CODE ENDRTOS_MUTEX*/
 /* USER CODE BEGIN RTOS_SEMAPHORES*/
 /* add semaphores, ... */
 /* USER CODE ENDRTOS_SEMAPHORES*/
 /* USER CODE BEGIN RTOS_TIMERS */
 /* start timers, add new ones, ... */
 /* USER CODE ENDRTOS_TIMERS */
 /* USER CODE BEGIN RTOS_QUEUES */
 /* add queues, ... */
/* USER CODE ENDRTOS_QUEUES*/
 /* Create the thread(s) */
 /* definition and creation of defaultTask */
 osThreadDef(defaultTask, StartDefaultTask, osPriorityNormal, 0, 128);
 defaultTaskHandle = osThreadCreate(osThread(defaultTask), NULL);
 /* USER CODE BEGIN RTOS_THREADS */
 /* add threads, ... */
 /* USER CODE ENDRTOS_THREADS*/
 /* Start scheduler */
 osKernelStart();
 /* We should never get here as control is now taken by the scheduler */
 /* Infinite loop */
 /* USER CODE BEGIN WHILE */
 while (1)
 {
 /* USER CODE ENDWHILE*/
 /* USER CODE BEGIN 3*/
 }
 /* USER CODE END3*/
 }
 /**
 * @brief System Clock Configuration
 * @retval None
 */
 void SystemClock_Config(void)
 {
 RCC_OscInitTypeDef RCC_OscInitStruct = {0};
 RCC_ClkInitTypeDef RCC_ClkInitStruct = {0};
 /** Initializes the RCC Oscillators according to the specified parameters
 * in the RCC_OscInitTypeDef structure.
 */
 RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_HSI;
 RCC_OscInitStruct.HSIState = RCC_HSI_ON;
 RCC_OscInitStruct.HSICalibrationValue = RCC_HSICALIBRATION_DEFAULT;
 RCC_OscInitStruct.PLL.PLLState = RCC_PLL_ON;
 RCC_OscInitStruct.PLL.PLLSource = RCC_PLLSOURCE_HSI_DIV2;
 RCC_OscInitStruct.PLL.PLLMUL = RCC_PLL_MUL16;
 if (HAL_RCC_OscConfig(&RCC_OscInitStruct) != HAL_OK)
{
 Error_Handler();
 }
 /** Initializes the CPU, AHB and APB buses clocks
 */
 RCC_ClkInitStruct.ClockType =
 RCC_CLOCKTYPE_HCLK|RCC_CLOCKTYPE_SYSCLK
 |RCC_CLOCKTYPE_PCLK1|RCC_CLOCKTYPE_PCLK2;
 RCC_ClkInitStruct.SYSCLKSource = RCC_SYSCLKSOURCE_PLLCLK;
 RCC_ClkInitStruct.AHBCLKDivider = RCC_SYSCLK_DIV1;
 RCC_ClkInitStruct.APB1CLKDivider = RCC_HCLK_DIV2;
 RCC_ClkInitStruct.APB2CLKDivider = RCC_HCLK_DIV1;
 if (HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_2) != HAL_OK)
 {
 Error_Handler();
 }
 }
 /**
 * @brief I2C1 Initialization Function
 * @param None
 * @retval None
 */
 static void MX_I2C1_Init(void)
 {
 /* USER CODE BEGIN I2C1_Init 0 */
 /* USER CODE ENDI2C1_Init 0 */
 /* USER CODE BEGIN I2C1_Init 1 */
 /* USER CODE ENDI2C1_Init 1 */
 hi2c1.Instance = I2C1;
 hi2c1.Init.ClockSpeed = 100000;
 hi2c1.Init.DutyCycle = I2C_DUTYCYCLE_2;
 hi2c1.Init.OwnAddress1 = 0;
 hi2c1.Init.AddressingMode = I2C_ADDRESSINGMODE_7BIT;
 hi2c1.Init.DualAddressMode = I2C_DUALADDRESS_DISABLE;
 hi2c1.Init.OwnAddress2 = 0;
 hi2c1.Init.GeneralCallMode = I2C_GENERALCALL_DISABLE;
 hi2c1.Init.NoStretchMode = I2C_NOSTRETCH_DISABLE;
 if (HAL_I2C_Init(&hi2c1) != HAL_OK)
 {
 Error_Handler();
}
 /* USER CODE BEGIN I2C1_Init 2 */
 /* USER CODE ENDI2C1_Init 2 */
 }
 /**
 * @brief SPI1 Initialization Function
 * @param None
 * @retval None
 */
 static void MX_SPI1_Init(void)
 {
 /* USER CODE BEGIN SPI1_Init 0 */
 /* USER CODE ENDSPI1_Init 0 */
 /* USER CODE BEGIN SPI1_Init 1 */
 /* USER CODE ENDSPI1_Init 1 */
 /* SPI1 parameter configuration*/
 hspi1.Instance = SPI1;
 hspi1.Init.Mode = SPI_MODE_MASTER;
 hspi1.Init.Direction = SPI_DIRECTION_2LINES;
 hspi1.Init.DataSize = SPI_DATASIZE_8BIT;
 hspi1.Init.CLKPolarity = SPI_POLARITY_LOW;
 hspi1.Init.CLKPhase = SPI_PHASE_1EDGE;
 hspi1.Init.NSS = SPI_NSS_SOFT;
 hspi1.Init.BaudRatePrescaler = SPI_BAUDRATEPRESCALER_64;
 hspi1.Init.FirstBit = SPI_FIRSTBIT_MSB;
 hspi1.Init.TIMode = SPI_TIMODE_DISABLE;
 hspi1.Init.CRCCalculation = SPI_CRCCALCULATION_DISABLE;
 hspi1.Init.CRCPolynomial = 10;
 if (HAL_SPI_Init(&hspi1) != HAL_OK)
 {
 Error_Handler();
 }
 /* USER CODE BEGIN SPI1_Init 2 */
 /* USER CODE ENDSPI1_Init 2 */
 }
 /**
 * @brief SPI2 Initialization Function
* @param None
 * @retval None
 */
 static void MX_SPI2_Init(void)
 {
 /* USER CODE BEGIN SPI2_Init 0 */
 /* USER CODE ENDSPI2_Init 0 */
 /* USER CODE BEGIN SPI2_Init 1 */
 /* USER CODE ENDSPI2_Init 1 */
 /* SPI2 parameter configuration*/
 hspi2.Instance = SPI2;
 hspi2.Init.Mode = SPI_MODE_MASTER;
 hspi2.Init.Direction = SPI_DIRECTION_2LINES;
 hspi2.Init.DataSize = SPI_DATASIZE_8BIT;
 hspi2.Init.CLKPolarity = SPI_POLARITY_LOW;
 hspi2.Init.CLKPhase = SPI_PHASE_1EDGE;
 hspi2.Init.NSS = SPI_NSS_SOFT;
 hspi2.Init.BaudRatePrescaler = SPI_BAUDRATEPRESCALER_2;
 hspi2.Init.FirstBit = SPI_FIRSTBIT_MSB;
 hspi2.Init.TIMode = SPI_TIMODE_DISABLE;
 hspi2.Init.CRCCalculation = SPI_CRCCALCULATION_DISABLE;
 hspi2.Init.CRCPolynomial = 10;
 if (HAL_SPI_Init(&hspi2) != HAL_OK)
 {
 Error_Handler();
 }
 /* USER CODE BEGIN SPI2_Init 2 */
 /* USER CODE ENDSPI2_Init 2 */
 }
 /**
 * @brief USART2 Initialization Function
 * @param None
 * @retval None
 */
 static void MX_USART2_UART_Init(void)
 {
 /* USER CODE BEGIN USART2_Init 0 */
 /* USER CODE ENDUSART2_Init 0 */
/* USER CODE BEGIN USART2_Init 1 */
 /* USER CODE ENDUSART2_Init 1 */
 huart2.Instance = USART2;
 huart2.Init.BaudRate = 115200;
 huart2.Init.WordLength = UART_WORDLENGTH_8B;
 huart2.Init.StopBits = UART_STOPBITS_1;
 huart2.Init.Parity = UART_PARITY_NONE;
 huart2.Init.Mode = UART_MODE_TX_RX;
 huart2.Init.HwFlowCtl = UART_HWCONTROL_NONE;
 huart2.Init.OverSampling = UART_OVERSAMPLING_16;
 if (HAL_UART_Init(&huart2) != HAL_OK)
 {
 Error_Handler();
 }
 /* USER CODE BEGIN USART2_Init 2 */
 /* USER CODE ENDUSART2_Init 2 */
 }
 /**
 * @brief GPIO Initialization Function
 * @param None
 * @retval None
 */
 static void MX_GPIO_Init(void)
 {
 GPIO_InitTypeDef GPIO_InitStruct = {0};
 /* USER CODE BEGIN MX_GPIO_Init_1 */
 /* USER CODE ENDMX_GPIO_Init_1 */
 /* GPIO Ports Clock Enable */
 __HAL_RCC_GPIOC_CLK_ENABLE();
 __HAL_RCC_GPIOD_CLK_ENABLE();
 __HAL_RCC_GPIOA_CLK_ENABLE();
 __HAL_RCC_GPIOB_CLK_ENABLE();
 /*Configure GPIO pin : B1_Pin */
 GPIO_InitStruct.Pin = B1_Pin;
 GPIO_InitStruct.Mode = GPIO_MODE_IT_RISING;
 GPIO_InitStruct.Pull = GPIO_NOPULL;
 HAL_GPIO_Init(B1_GPIO_Port, &GPIO_InitStruct);
 /*Configure GPIO pin : PA0 */
 GPIO_InitStruct.Pin = GPIO_PIN_0;
GPIO_InitStruct.Mode = GPIO_MODE_INPUT;
 GPIO_InitStruct.Pull = GPIO_NOPULL;
 HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);
 /*Configure GPIO pins : PB0 PB1 PB2 */
 GPIO_InitStruct.Pin = GPIO_PIN_0|GPIO_PIN_1|GPIO_PIN_2;
 GPIO_InitStruct.Mode = GPIO_MODE_INPUT;
 GPIO_InitStruct.Pull = GPIO_PULLUP; // 풀업 저항 사용
HAL_GPIO_Init(GPIOB, &GPIO_InitStruct);
 /* EXTI interrupt init*/
 HAL_NVIC_SetPriority(EXTI15_10_IRQn, 5, 0);
 HAL_NVIC_EnableIRQ(EXTI15_10_IRQn);
 /* USER CODE BEGIN MX_GPIO_Init_2 */
 /* USER CODE ENDMX_GPIO_Init_2 */
 }
 /* USER CODE BEGIN 4*/
 void RemoveFirstNumListElement()
 {
 if (num_list_size > 0)
 {
 for (int i = 0; i < num_list_size- 1; i++)
 {
 num_list[i] = num_list[i + 1];
 }
 num_list_size--;
 }
 }
 void UpdateRequiredInfo()
 {
 if (num_list_size > 0)
 {
 required_person_count = num_list[0] % 10; // 마지막 자리 숫자
required_password = num_list[0] / 10;
 }
 else
 {
 required_person_count = 0;
 required_password = 0;
 }
 }
 // 앞의 4자리숫자
void UpdatePasswordDisplay()
 {
 xSemaphoreTake(xLcdMutex, portMAX_DELAY);
lcd_put_cur(1, 0);
 lcd_send_string(password);
 xSemaphoreGive(xLcdMutex);
 }
 void HandlePasswordInput()
 {
 static uint32_t last_blink_time = 0;
 static bool blink_state = true;
 // 버튼의이전상태를저장하는변수(눌림->떼짐,떼짐->눌림)
 static GPIO_PinState left_button_prev = GPIO_PIN_SET;
 static GPIO_PinState center_button_prev = GPIO_PIN_SET;
 static GPIO_PinState confirm_button_prev = GPIO_PIN_SET;
 uint32_t current_time = xTaskGetTickCount();
 // 버튼 상태읽기
GPIO_PinState left_button = HAL_GPIO_ReadPin(GPIOB, GPIO_PIN_0);
 GPIO_PinState center_button = HAL_GPIO_ReadPin(GPIOB, GPIO_PIN_1);
 GPIO_PinState confirm_button = HAL_GPIO_ReadPin(GPIOB, GPIO_PIN_2);
 // 왼쪽 버튼처리
if (left_button == GPIO_PIN_RESET && left_button_prev == GPIO_PIN_SET)
 {
 vTaskDelay(pdMS_TO_TICKS(20));
 if (HAL_GPIO_ReadPin(GPIOB, GPIO_PIN_0) == GPIO_PIN_RESET)
 {
 password[current_digit]++;
 if (password[current_digit] > '9')
 password[current_digit] = '0';
 // LCD 업데이트
UpdatePasswordDisplay();
 }
 }
 left_button_prev = left_button;
 // 가운데버튼처리
if (center_button == GPIO_PIN_RESET && center_button_prev == GPIO_PIN_SET)
 {
 vTaskDelay(pdMS_TO_TICKS(20));
 if (HAL_GPIO_ReadPin(GPIOB, GPIO_PIN_1) == GPIO_PIN_RESET)
 {
 current_digit++;
 if (current_digit > 3)
current_digit = 0;
 }
 }
 center_button_prev = center_button;
 // 확인 버튼처리
if (confirm_button == GPIO_PIN_RESET && confirm_button_prev ==
 GPIO_PIN_SET)
 {
 vTaskDelay(pdMS_TO_TICKS(20));
 if (HAL_GPIO_ReadPin(GPIOB, GPIO_PIN_2) == GPIO_PIN_RESET)
 {
 // 입력된패스워드검증
uint32_t entered_password = atoi(password);
 if (entered_password == required_password)
 {
 // 패스워드일치
access_granted = true;
 // UART로 결과전송
char msg[50];
 snprintf(msg, sizeof(msg), "Password correct: %s\n", password);
 HAL_UART_Transmit(&huart2, (uint8_t *)msg, strlen(msg),
 HAL_MAX_DELAY);
 // 패스워드입력모드종료및초기화
password_mode = false;
 person_count = 0;
 current_digit = 0;
 strcpy(password, "0000");
 // num_list에서 첫 번째 요소 제거
RemoveFirstNumListElement();
 // 필요한정보업데이트
UpdateRequiredInfo();
 }
 else
 {
 // 패스워드불일치
char msg[50];
 snprintf(msg, sizeof(msg), "Wrong password: %s\n", password);
 HAL_UART_Transmit(&huart2, (uint8_t *)msg, strlen(msg),
 HAL_MAX_DELAY);
 // LCD에 "wrong password" 표시
xSemaphoreTake(xLcdMutex, portMAX_DELAY);
 lcd_clear();
 lcd_put_cur(0, 0);
 lcd_send_string("Wrong Password");
 xSemaphoreGive(xLcdMutex);
 vTaskDelay(pdMS_TO_TICKS(2000));
 // 패스워드입력화면으로복귀
xSemaphoreTake(xLcdMutex, portMAX_DELAY);
 lcd_clear();
 lcd_put_cur(0, 0);
 lcd_send_string("password");
 lcd_put_cur(1, 0);
 strcpy(password, "0000");
 lcd_send_string(password);
 xSemaphoreGive(xLcdMutex);
 current_digit = 0;
 }
 }
 }
 confirm_button_prev = confirm_button;
 // 깜빡임구현
if (current_time- last_blink_time >= pdMS_TO_TICKS(500))
 {
 blink_state = !blink_state;
 last_blink_time = current_time;
 char display_password[5];
 strcpy(display_password, password);
 if (blink_state)
 display_password[current_digit] = ' ';
 // LCD 업데이트
xSemaphoreTake(xLcdMutex, portMAX_DELAY);
 lcd_put_cur(1, 0);
 lcd_send_string(display_password);
 xSemaphoreGive(xLcdMutex);
 }
 }
 void PIR_Task(void *argument)
 {
 UpdateRequiredInfo();
uint8_t high_duration = 0; // HIGH 상태 지속 시간 카운트
while (1)
 {
 if (!password_mode && !access_granted)
 {
 // 사람 수감지로직
GPIO_PinState pir_state = HAL_GPIO_ReadPin(GPIOA, GPIO_PIN_0);
 if (pir_state == GPIO_PIN_SET)
 {
 high_duration++;
 if (high_duration >= 1 && pir_state_previous == GPIO_PIN_RESET)
 {
 person_count++; // 사람 수 증가
pir_state_previous = GPIO_PIN_SET;
 // UART로 결과전송
char msg[50];
 snprintf(msg, sizeof(msg), "Person detected! Count: %lu\n", person_count);
 HAL_UART_Transmit(&huart2, (uint8_t *)msg, strlen(msg),
 HAL_MAX_DELAY);
 // 필요한사람수에도달하면패스워드모드로전환
if (person_count >= required_person_count && required_person_count > 0)
 {
 password_mode = true;
 // LCD에 패스워드입력화면표시
xSemaphoreTake(xLcdMutex, portMAX_DELAY);
 HAL_UART_Transmit(&huart2, "task1 start\n", 15 , 1000);
 lcd_clear();
 lcd_put_cur(0, 0);
 lcd_send_string("password");
 lcd_put_cur(1, 0);
 strcpy(password, "0000");
 lcd_send_string(password);
 xSemaphoreGive(xLcdMutex);
 }
 }
 }
 else
 {
 high_duration = 0;
 pir_state_previous = GPIO_PIN_RESET;
 // 사람이감지되지않을때태스크블로킹
vTaskDelay(pdMS_TO_TICKS(100)); // 필요에 따라 지연 시간 조정
}
 }
 else if (password_mode)
 {
 // 패스워드입력모드
HandlePasswordInput();
 }
 else if (access_granted)
 {
 // 접근 허용된상태에서는대기
vTaskDelay(pdMS_TO_TICKS(100));
 }
 // 센서 상태를더자주확인
vTaskDelay(pdMS_TO_TICKS(10));
 }
 }
 void HPT_Task(void *argument)
 {
 uint8_t flag = 0;
 lcd_init();
 while (1)
 {
 if (access_granted)
 {
 ///////////////////////////////////통신 파트
flag = 1; // 손님이 왔음을 나타내는플래그
HAL_SPI_Transmit(&hspi1, &flag, 1, HAL_MAX_DELAY);
 /////////////////////////////////////
 OLED_1in3_c_test();
 /////////////////////////////////////////// oled
 xSemaphoreTake(xLcdMutex, portMAX_DELAY);
 HAL_UART_Transmit(&huart2, "task2 start\n", 15 , 1000);
 lcd_clear();
 lcd_put_cur(0, 0);
 lcd_send_string("WELCOME !");
 lcd_put_cur(1, 0);
 lcd_send_string("GO TO table 1");
 xSemaphoreGive(xLcdMutex);
// 환영 메시지를일정시간동안표시(예:5초)
 vTaskDelay(pdMS_TO_TICKS(5000));
 // LCD 초기화또는기본메시지표시
xSemaphoreTake(xLcdMutex, portMAX_DELAY);
 lcd_clear();
 lcd_put_cur(0, 0);
 lcd_send_string("waiting...");
 xSemaphoreGive(xLcdMutex);
 // 다음 그룹처리를위해플래그초기화
access_granted = false;
 // 필요한정보업데이트
UpdateRequiredInfo();
 }
 else
 {
 // 할 일이없을때태스크블로킹
vTaskDelay(pdMS_TO_TICKS(100));
 }
 }
 }
 void LPT_Task(void *argument)
 {
 const char *messages[][2] = {
 {"TODAY'S MENU", ""},
 {"Small Bites",""},
 {"Almond With", "Cavior"},
 {"Hearth Oven", "Grilled Hanwoo"},
 {"Burdock Tarte", "Tatin& SkinJuice"},
 {"Small Sweets","" }
 };
 const int num_messages = sizeof(messages) / sizeof(messages[0]);
 int current_message = 0;
 while (1)
 {
 // 높은 우선순위태스크들이실행중이지않을때실행
if (!password_mode && !access_granted)
 {
 xSemaphoreTake(xLcdMutex, portMAX_DELAY);
 HAL_UART_Transmit(&huart2, "task3 start\n", 15 , 1000);
 // LCD에 현재메시지표시
lcd_clear();
lcd_put_cur(0, 0);
 lcd_send_string(messages[current_message][0]); // 첫 번째 줄
lcd_put_cur(1, 0);
 lcd_send_string(messages[current_message][1]); // 두 번째 줄
xSemaphoreGive(xLcdMutex);
 // 다음 메시지로인덱스증가(순환)
 current_message = (current_message + 1) % num_messages;
 }
 // 3초 대기
vTaskDelay(pdMS_TO_TICKS(4000));
 }
 }
 /* USER CODE END4*/
 /* USER CODE BEGIN Header_StartDefaultTask */
 /**
 * @brief Function implementing the defaultTask thread.
 * @param argument: Not used
 * @retval None
 */
 /* USER CODE ENDHeader_StartDefaultTask */
 void StartDefaultTask(void const * argument)
 {
 /* USER CODE BEGIN 5*/
 /* Infinite loop */
 for(;;)
 {
 osDelay(1);
 }
 /* USER CODE END5*/
 }
 /**
 * @brief Period elapsed callback in non blocking mode
 * @note This function is called when TIM2 interrupt took place, inside
 * HAL_TIM_IRQHandler(). It makes a direct call to HAL_IncTick() to increment
 * a global variable "uwTick" used as application time base.
 * @param htim : TIM handle
 * @retval None
 */
 void HAL_TIM_PeriodElapsedCallback(TIM_HandleTypeDef *htim)
{
 /* USER CODE BEGIN Callback 0 */
 /* USER CODE ENDCallback 0 */
 if (htim->Instance == TIM2) {
 HAL_IncTick();
 }
 /* USER CODE BEGIN Callback 1 */
 /* USER CODE ENDCallback 1 */
 }
 /**
 * @brief This function is executed in case of error occurrence.
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
 /* USER CODE ENDError_Handler_Debug */
 }
 #ifdef USE_FULL_ASSERT
 /**
 * @brief Reports the name of the source file and the source line number
 *
 where the assert_param error has occurred.
 * @param file: pointer to the source file name
 * @param line: assert_param error line source number
 * @retval None
 */
 void assert_failed(uint8_t *file, uint32_t line)
 {
 /* USER CODE BEGIN 6*/
 /* User can add his own implementation to report the file name and line number,
 ex: printf("Wrong parameters value: file %s on line %d\r\n", file, line) */
 /* USER CODE END6*/
 }
 #endif /* USE_FULL_ASSERT */
