/*USERCODEBEGINHeader*/
 /**
 ******************************************************************************
 *@file :main.c
 *@brief :Mainprogrambody
 ******************************************************************************
 *@attention
 *
 *Copyright(c)2024STMicroelectronics.
 *All rightsreserved.
 *
 *ThissoftwareislicensedundertermsthatcanbefoundintheLICENSEfile
 *intherootdirectoryof thissoftwarecomponent.
 *IfnoLICENSEfilecomeswiththissoftware, it isprovidedAS-IS.
 *
 ******************************************************************************
 */
 /*USERCODEENDHeader*/
 /*Includes------------------------------------------------------------------*/
 #include"main.h"
 #include"cmsis_os.h"
 /*Privateincludes----------------------------------------------------------*/
 /*USERCODEBEGINIncludes*/
 #include"FreeRTOS.h"
 #include"task.h"
 #include"timers.h"
 #include"queue.h"
 #include"semphr.h"
 #include"event_groups.h"
#include"i2c_lcd.h"
 #include"stuffManage.h"
 /*USERCODEENDIncludes*/
 /*Privatetypedef-----------------------------------------------------------*/
 /*USERCODEBEGINPTD*/
 enumcuisine{
 SmallBites=31,
 AlmondWithCavior, //서브1
 SnowCrabAndPickledChrysanthemum, //서브1대체1
 AblaloneTaco, //서브1대체2
 HearthOvenGrilledHanwoo, //메인
EmberToastedAcornNoodle, //메인대체1
 BlackSesameSeaurchinNasturtium, //메인대체2
 BurdockTarteTatinWithSkinJuice, //서브2
 TilefishMustardBrassica, //서브2대체1
 fattyTuna, //서브2대체2
 SmallSweets
 }cuisine;
 typedefstruct{
 uint8_tcourseData[5][2];
 uint8_tchangeCourse[3];
 }user_data;
 /*USERCODEENDPTD*/
 /*Privatedefine------------------------------------------------------------*/
 /*USERCODEBEGINPD*/
 /*USERCODEENDPD*/
 /*Privatemacro-------------------------------------------------------------*/
 /*USERCODEBEGINPM*/
 /*USERCODEENDPM*/
 /*Privatevariables---------------------------------------------------------*/
 I2C_HandleTypeDefhi2c1;
 SPI_HandleTypeDefhspi1;
 UART_HandleTypeDefhuart2;
 /*USERCODEBEGINPV*/
 TaskHandle_tcopyHandler;
 TaskHandle_tcookHandler;
 TaskHandle_t lcdHandler;
 TaskHandle_tbuzzerHandler;
 SemaphoreHandle_tspiSem;
 SemaphoreHandle_tcookSem;
 SemaphoreHandle_t lcdSem;
 TimerHandle_tuserTimerHandler;
 /*USERCODEENDPV*/
 /*Privatefunctionprototypes-----------------------------------------------*/
 voidSystemClock_Config(void);
 staticvoidMX_GPIO_Init(void);
 staticvoidMX_SPI1_Init(void);
 staticvoidMX_USART2_UART_Init(void);
 staticvoidMX_I2C1_Init(void);
 voidStartdefaultTask(voidconst*argument);
 /*USERCODEBEGINPFP*/
voiddataInit(void);
 voidformatTime(uint8_t,char*);
 uint8_t findMaxIndex(uint8_tarr[],uint8_t);
 voidupdateUserTimer(TimerHandle_t);
 voidcopyTask(void*argument);
 voidcookTask(void*argument);
 voidlcdTask(void*argument);
 voidbuzzerTask(void*argument);
 voidUART_Print(constchar*message);
 /*USERCODEENDPFP*/
 /*Privateusercode---------------------------------------------------------*/
 /*USERCODEBEGIN0*/
 user_datauserData[4];
 uint8_trxBuffer[5];
 volatileboolstartSig=false;
 uint8_twhatCourse=0;
 chartimeStr1[6];
 chartimeStr2[6];
 chartimeStr3[6];
 chartimeStr4[6];
 uint8_tarrCourseTime[4];
 uint8_tarrCourseTime1[4];
 uint8_tarrCourseTime2[4];
 uint8_tarrCourseTime3[4];
 uint8_tmaxIndex;
 uint8_tmaxIndex1;
 uint8_tmaxIndex2;
 uint8_tmaxIndex3;
 uint8_tremainingStuff[30];
 volatileboolbuzzerSig=false;
 uint8_tresetCnt=0;
 volatilebool lcdState=false;
 uint8_t txData=0x30;
 /*USERCODEEND0*/
 /**
 *@brief Theapplicationentrypoint.
 *@retval int
 */
 intmain(void)
 {
 /*USERCODEBEGIN1*/
 /*USERCODEEND1*/
 /*MCUConfiguration--------------------------------------------------------*/
 /*Resetofallperipherals, InitializestheFlashinterfaceandtheSystick.*/
 HAL_Init();
 /*USERCODEBEGINInit*/
 /*USERCODEENDInit*/
 /*Configurethesystemclock*/
 SystemClock_Config();
 /*USERCODEBEGINSysInit*/
 /*USERCODEENDSysInit*/
/*Initializeallconfiguredperipherals*/
 MX_GPIO_Init();
 MX_SPI1_Init();
 MX_USART2_UART_Init();
 MX_I2C1_Init();
 /*USERCODEBEGIN2*/
 lcd_init();
 dataInit();
 HAL_SPI_Receive_IT(&hspi1,rxBuffer,sizeof(rxBuffer));
 /*USERCODEEND2*/
 /*USERCODEBEGINRTOS_MUTEX*/
 /*addmutexes, ...*/
 /*USERCODEENDRTOS_MUTEX*/
 /*USERCODEBEGINRTOS_SEMAPHORES*/
 /*addsemaphores, ...*/
 spiSem=xSemaphoreCreateBinary();
 cookSem=xSemaphoreCreateBinary();
 lcdSem=xSemaphoreCreateBinary();
 /*USERCODEENDRTOS_SEMAPHORES*/
 /*USERCODEBEGINRTOS_TIMERS*/
 /*start timers,addnewones, ...*/
 userTimerHandler=xTimerCreate("userTimer",pdMS_TO_TICKS(1000),pdTRUE,NULL,
 updateUserTimer);
 /*USERCODEENDRTOS_TIMERS*/
 /*USERCODEBEGINRTOS_QUEUES*/
 /*addqueues, ...*/
 /*USERCODEENDRTOS_QUEUES*/
 /*Createthethread(s)*/
 /*definitionandcreationofdeaultTask*/
 /*USERCODEBEGINRTOS_THREADS*/
 xTaskCreate(copyTask,"spiTask",128,NULL,3,&copyHandler);
 xTaskCreate(cookTask,"cookTask",128,NULL,2,&cookHandler);
 xTaskCreate(lcdTask,"lcdTask",128,NULL,1,&lcdHandler);
 xTaskCreate(buzzerTask,"buzzerTask",128,NULL,1,&buzzerHandler);
 /*addthreads, ...*/
 /*USERCODEENDRTOS_THREADS*/
 /*Startscheduler*/
 vTaskStartScheduler();
 /*Weshouldnevergethereascontrol isnowtakenbythescheduler*/
 /*Infiniteloop*/
 /*USERCODEBEGINWHILE*/
 while(1)
 {
 /*USERCODEENDWHILE*/
 /*USERCODEBEGIN3*/
 }
 /*USERCODEEND3*/
 }
 /**
 *@briefSystemClockConfiguration
 *@retvalNone
*/
 voidSystemClock_Config(void)
 {
 RCC_OscInitTypeDefRCC_OscInitStruct={0};
 RCC_ClkInitTypeDefRCC_ClkInitStruct={0};
 /**InitializestheRCCOscillatorsaccordingtothespecifiedparameters
 *intheRCC_OscInitTypeDefstructure.
 */
 RCC_OscInitStruct.OscillatorType=RCC_OSCILLATORTYPE_HSE;
 RCC_OscInitStruct.HSEState=RCC_HSE_BYPASS;
 RCC_OscInitStruct.HSEPredivValue=RCC_HSE_PREDIV_DIV1;
 RCC_OscInitStruct.HSIState=RCC_HSI_ON;
 RCC_OscInitStruct.PLL.PLLState=RCC_PLL_ON;
 RCC_OscInitStruct.PLL.PLLSource=RCC_PLLSOURCE_HSE;
 RCC_OscInitStruct.PLL.PLLMUL=RCC_PLL_MUL9;
 if(HAL_RCC_OscConfig(&RCC_OscInitStruct)!=HAL_OK)
 {
 Error_Handler();
 }
 /**InitializestheCPU,AHBandAPBbusesclocks
 */
 RCC_ClkInitStruct.ClockType=RCC_CLOCKTYPE_HCLK|RCC_CLOCKTYPE_SYSCLK
 |RCC_CLOCKTYPE_PCLK1|RCC_CLOCKTYPE_PCLK2;
 RCC_ClkInitStruct.SYSCLKSource=RCC_SYSCLKSOURCE_PLLCLK;
 RCC_ClkInitStruct.AHBCLKDivider=RCC_SYSCLK_DIV1;
 RCC_ClkInitStruct.APB1CLKDivider=RCC_HCLK_DIV2;
 RCC_ClkInitStruct.APB2CLKDivider=RCC_HCLK_DIV1;
 if(HAL_RCC_ClockConfig(&RCC_ClkInitStruct,FLASH_LATENCY_2)!=HAL_OK)
 {
 Error_Handler();
 }
 }
 /**
 *@brief I2C1InitializationFunction
 *@paramNone
 *@retvalNone
 */
 staticvoidMX_I2C1_Init(void)
 {
 /*USERCODEBEGINI2C1_Init0*/
 /*USERCODEENDI2C1_Init0*/
 /*USERCODEBEGINI2C1_Init1*/
 /*USERCODEENDI2C1_Init1*/
 hi2c1.Instance=I2C1;
 hi2c1.Init.ClockSpeed=100000;
 hi2c1.Init.DutyCycle=I2C_DUTYCYCLE_2;
 hi2c1.Init.OwnAddress1=0;
 hi2c1.Init.AddressingMode=I2C_ADDRESSINGMODE_7BIT;
 hi2c1.Init.DualAddressMode=I2C_DUALADDRESS_DISABLE;
 hi2c1.Init.OwnAddress2=0;
 hi2c1.Init.GeneralCallMode=I2C_GENERALCALL_DISABLE;
hi2c1.Init.NoStretchMode=I2C_NOSTRETCH_DISABLE;
 if(HAL_I2C_Init(&hi2c1)!=HAL_OK)
 {
 Error_Handler();
 }
 /*USERCODEBEGINI2C1_Init2*/
 /*USERCODEENDI2C1_Init2*/
 }
 /**
 *@briefSPI1InitializationFunction
 *@paramNone
 *@retvalNone
 */
 staticvoidMX_SPI1_Init(void)
 {
 /*USERCODEBEGINSPI1_Init0*/
 /*USERCODEENDSPI1_Init0*/
 /*USERCODEBEGINSPI1_Init1*/
 /*USERCODEENDSPI1_Init1*/
 /*SPI1parameterconfiguration*/
 hspi1.Instance=SPI1;
 hspi1.Init.Mode=SPI_MODE_SLAVE;
 hspi1.Init.Direction=SPI_DIRECTION_2LINES;
 hspi1.Init.DataSize=SPI_DATASIZE_8BIT;
 hspi1.Init.CLKPolarity=SPI_POLARITY_LOW;
 hspi1.Init.CLKPhase=SPI_PHASE_1EDGE;
 hspi1.Init.NSS=SPI_NSS_SOFT;
 hspi1.Init.FirstBit=SPI_FIRSTBIT_MSB;
 hspi1.Init.TIMode=SPI_TIMODE_DISABLE;
 hspi1.Init.CRCCalculation=SPI_CRCCALCULATION_DISABLE;
 hspi1.Init.CRCPolynomial=10;
 if(HAL_SPI_Init(&hspi1)!=HAL_OK)
 {
 Error_Handler();
 }
 /*USERCODEBEGINSPI1_Init2*/
 /*USERCODEENDSPI1_Init2*/
 }
 /**
 *@briefUSART2InitializationFunction
 *@paramNone
 *@retvalNone
 */
 staticvoidMX_USART2_UART_Init(void)
 {
 /*USERCODEBEGINUSART2_Init0*/
 /*USERCODEENDUSART2_Init0*/
 /*USERCODEBEGINUSART2_Init1*/
 /*USERCODEENDUSART2_Init1*/
 huart2.Instance=USART2;
 huart2.Init.BaudRate=115200;
huart2.Init.WordLength=UART_WORDLENGTH_8B;
 huart2.Init.StopBits=UART_STOPBITS_1;
 huart2.Init.Parity=UART_PARITY_NONE;
 huart2.Init.Mode=UART_MODE_TX_RX;
 huart2.Init.HwFlowCtl=UART_HWCONTROL_NONE;
 huart2.Init.OverSampling=UART_OVERSAMPLING_16;
 if(HAL_UART_Init(&huart2)!=HAL_OK)
 {
 Error_Handler();
 }
 /*USERCODEBEGINUSART2_Init2*/
 /*USERCODEENDUSART2_Init2*/
 }
 /**
 *@briefGPIOInitializationFunction
 *@paramNone
 *@retvalNone
 */
 staticvoidMX_GPIO_Init(void)
 {
 GPIO_InitTypeDefGPIO_InitStruct={0};
 /*USERCODEBEGINMX_GPIO_Init_1*/
 /*USERCODEENDMX_GPIO_Init_1*/
 /*GPIOPortsClockEnable*/
 __HAL_RCC_GPIOC_CLK_ENABLE();
 __HAL_RCC_GPIOD_CLK_ENABLE();
 __HAL_RCC_GPIOA_CLK_ENABLE();
 __HAL_RCC_GPIOB_CLK_ENABLE();
 /*ConfigureGPIOpinOutputLevel*/
 HAL_GPIO_WritePin(GPIOB,Buzzer_Pin|LED_Pin,GPIO_PIN_RESET);
 /*ConfigureGPIOpin:SS_Pin*/
 GPIO_InitStruct.Pin=SS_Pin;
 GPIO_InitStruct.Mode=GPIO_MODE_INPUT;
 GPIO_InitStruct.Pull=GPIO_NOPULL;
 HAL_GPIO_Init(SS_GPIO_Port,&GPIO_InitStruct);
 /*ConfigureGPIOpins:Buzzer_PinLED_Pin*/
 GPIO_InitStruct.Pin=Buzzer_Pin|LED_Pin;
 GPIO_InitStruct.Mode=GPIO_MODE_OUTPUT_PP;
 GPIO_InitStruct.Pull=GPIO_NOPULL;
 GPIO_InitStruct.Speed=GPIO_SPEED_FREQ_HIGH;
 HAL_GPIO_Init(GPIOB,&GPIO_InitStruct);
 /*USERCODEBEGINMX_GPIO_Init_2*/
 /*USERCODEENDMX_GPIO_Init_2*/
 }
 /*USERCODEBEGIN4*/
 voidUART_Print(constchar*message)
 {
 HAL_UART_Transmit(&huart2,(uint8_t*)message,strlen(message),HAL_MAX_DELAY);
 }
 voiddataInit()
 {
for(int i=0; i<4; i++)
 {
 userData[i].courseData[0][0]=SmallBites;
 userData[i].courseData[0][1]=5;
 userData[i].courseData[1][0]=AlmondWithCavior;
 userData[i].courseData[1][1]=10;
 userData[i].courseData[2][0]=HearthOvenGrilledHanwoo;
 userData[i].courseData[2][1]=15;
 userData[i].courseData[3][0]=BurdockTarteTatinWithSkinJuice;
 userData[i].courseData[3][1]=10;
 userData[i].courseData[4][0]=SmallSweets;
 userData[i].courseData[4][1]=15;
 }
 memset(remainingStuff,50,sizeof(remainingStuff));
 }
 voidupdateUserTimer(TimerHandle_txTimer)
 {
 //UART_Print("TimerStart!\r\n");
 uint8_t temp=0;
 lcd_clear();
 switch(whatCourse)
 {
 case1:
 //UART_Print("SUB1CourseTimerStart!\r\n");
 switch(maxIndex1)
 {
 case0:
 if(arrCourseTime1[maxIndex1]>0)
 {
 temp=arrCourseTime1[maxIndex1];
 lcd_put_cur(0,0);
 formatTime(arrCourseTime1[0], timeStr1);
 lcd_send_string(timeStr1);
 arrCourseTime1[maxIndex1]--;
 }
 if(temp<=arrCourseTime1[1]&&arrCourseTime1[1]>0)
 {
 lcd_put_cur(0,8);
 formatTime(arrCourseTime1[1], timeStr2);
 lcd_send_string(timeStr2);
 arrCourseTime1[1]--;
 }
 if(temp<=arrCourseTime1[2]&&arrCourseTime1[2]>0)
 {
 lcd_put_cur(1,0);
 formatTime(arrCourseTime1[2], timeStr3);
 lcd_send_string(timeStr3);
 arrCourseTime1[2]--;
 }
 if(temp<=arrCourseTime1[3]&&arrCourseTime1[3]>0)
 {
lcd_put_cur(1,8);
 formatTime(arrCourseTime1[3], timeStr4);
 lcd_send_string(timeStr4);
 arrCourseTime1[3]--;
 }
 break;
 case1:
 if(arrCourseTime1[maxIndex1]>0)
 {
 temp=arrCourseTime1[maxIndex1];
 lcd_put_cur(0,8);
 formatTime(arrCourseTime1[maxIndex1], timeStr2);
 lcd_send_string(timeStr2);
 arrCourseTime1[maxIndex1]--;
 }
 if(temp<=arrCourseTime1[0]&&arrCourseTime1[0]>0)
 {
 lcd_put_cur(0,0);
 formatTime(arrCourseTime1[0], timeStr1);
 lcd_send_string(timeStr1);
 arrCourseTime1[0]--;
 }
 if(temp<=arrCourseTime1[2]&&arrCourseTime1[2]>0)
 {
 lcd_put_cur(1,0);
 formatTime(arrCourseTime1[2], timeStr3);
 lcd_send_string(timeStr3);
 arrCourseTime1[2]--;
 }
 if(temp<=arrCourseTime1[3]&&arrCourseTime1[3]>0)
 {
 lcd_put_cur(1,8);
 formatTime(arrCourseTime1[3], timeStr4);
 lcd_send_string(timeStr4);
 arrCourseTime1[3]--;
 }
 break;
 case2:
 if(arrCourseTime1[maxIndex1]>0)
 {
 temp=arrCourseTime1[maxIndex1];
 lcd_put_cur(1,0);
 formatTime(arrCourseTime1[2], timeStr3);
 lcd_send_string(timeStr3);
 arrCourseTime1[maxIndex1]--;
 }
 if(temp<=arrCourseTime1[0]&&arrCourseTime1[0]>0)
 {
 lcd_put_cur(0,0);
 formatTime(arrCourseTime1[0], timeStr1);
 lcd_send_string(timeStr1);
arrCourseTime1[0]--;
 }
 if(temp<=arrCourseTime1[1]&&arrCourseTime1[1]>0)
 {
 lcd_put_cur(0,8);
 formatTime(arrCourseTime1[1], timeStr2);
 lcd_send_string(timeStr2);
 arrCourseTime1[1]--;
 }
 if(temp<=arrCourseTime1[3]&&arrCourseTime1[3]>0)
 {
 lcd_put_cur(1,8);
 formatTime(arrCourseTime1[3], timeStr4);
 lcd_send_string(timeStr4);
 arrCourseTime1[3]--;
 }
 break;
 case3:
 if(arrCourseTime1[maxIndex1]>0)
 {
 temp=arrCourseTime1[maxIndex1];
 lcd_put_cur(1,8);
 formatTime(arrCourseTime1[maxIndex1], timeStr4);
 lcd_send_string(timeStr4);
 arrCourseTime1[maxIndex1]--;
 }
 if(temp<=arrCourseTime1[0]&&arrCourseTime1[0]>0)
 {
 lcd_put_cur(0,0);
 formatTime(arrCourseTime1[0], timeStr1);
 lcd_send_string(timeStr1);
 arrCourseTime1[0]--;
 }
 if(temp<=arrCourseTime1[1]&&arrCourseTime1[1]>0)
 {
 lcd_put_cur(0,8);
 formatTime(arrCourseTime1[1], timeStr2);
 lcd_send_string(timeStr2);
 arrCourseTime1[1]--;
 }
 if(temp<=arrCourseTime1[2]&&arrCourseTime1[2]>0)
 {
 lcd_put_cur(1,0);
 formatTime(arrCourseTime1[2], timeStr3);
 lcd_send_string(timeStr3);
 arrCourseTime1[2]--;
 }
 break;
 }
 if(arrCourseTime1[0]==0&&arrCourseTime1[1]==0&&arrCourseTime1[2]==0
 &&arrCourseTime1[3]==0)
{
 lcd_clear();
 lcd_send_string("CourseFinish!");
 buzzerSig=true;
 xSemaphoreGive(lcdSem);
 //UART_Print("S/WTimer lcdSemGive!\r\n");
 xTimerStop(userTimerHandler,0);
 }
 break;
 case2:
 //UART_Print("MainCourseTimerStart!\r\n");
 switch(maxIndex2)
 {
 case0:
 if(arrCourseTime2[maxIndex2]>0)
 {
 temp=arrCourseTime2[maxIndex2];
 lcd_put_cur(0,0);
 formatTime(arrCourseTime2[maxIndex2], timeStr1);
 lcd_send_string(timeStr1);
 arrCourseTime2[maxIndex2]--;
 }
 if(temp<=arrCourseTime2[1]&&arrCourseTime2[1]>0)
 {
 lcd_put_cur(0,8);
 formatTime(arrCourseTime2[1], timeStr2);
 lcd_send_string(timeStr2);
 arrCourseTime2[1]--;
 }
 if(temp<=arrCourseTime2[2]&&arrCourseTime2[2]>0)
 {
 lcd_put_cur(1,0);
 formatTime(arrCourseTime2[2], timeStr3);
 lcd_send_string(timeStr3);
 arrCourseTime2[2]--;
 }
 if(temp<=arrCourseTime2[3]&&arrCourseTime2[3]>0)
 {
 lcd_put_cur(1,8);
 formatTime(arrCourseTime2[3], timeStr4);
 lcd_send_string(timeStr4);
 arrCourseTime2[3]--;
 }
 break;
 case1:
 if(arrCourseTime2[maxIndex2]>0)
 {
 temp=arrCourseTime2[maxIndex2];
 lcd_put_cur(0,8);
 formatTime(arrCourseTime2[maxIndex2], timeStr2);
 lcd_send_string(timeStr2);
arrCourseTime2[maxIndex2]--;
 }
 if(temp<=arrCourseTime2[0]&&arrCourseTime2[0]>0)
 {
 lcd_put_cur(0,0);
 formatTime(arrCourseTime2[0], timeStr1);
 lcd_send_string(timeStr1);
 arrCourseTime2[0]--;
 }
 if(temp<=arrCourseTime2[2]&&arrCourseTime2[2]>0)
 {
 lcd_put_cur(1,0);
 formatTime(arrCourseTime2[2], timeStr3);
 lcd_send_string(timeStr3);
 arrCourseTime2[2]--;
 }
 if(temp<=arrCourseTime2[3]&&arrCourseTime2[3]>0)
 {
 lcd_put_cur(1,8);
 formatTime(arrCourseTime2[3], timeStr4);
 lcd_send_string(timeStr4);
 arrCourseTime2[3]--;
 }
 break;
 case2:
 if(arrCourseTime2[maxIndex2]>0)
 {
 temp=arrCourseTime2[maxIndex2];
 lcd_put_cur(1,0);
 formatTime(arrCourseTime2[maxIndex2], timeStr3);
 lcd_send_string(timeStr3);
 arrCourseTime2[maxIndex2]--;
 }
 if(temp<=arrCourseTime2[0]&&arrCourseTime2[0]>0)
 {
 lcd_put_cur(0,0);
 formatTime(arrCourseTime2[0], timeStr1);
 lcd_send_string(timeStr1);
 arrCourseTime2[0]--;
 }
 if(temp<=arrCourseTime2[1]&&arrCourseTime2[1]>0)
 {
 lcd_put_cur(0,8);
 formatTime(arrCourseTime2[1], timeStr2);
 lcd_send_string(timeStr2);
 arrCourseTime2[1]--;
 }
 if(temp<=arrCourseTime2[3]&&arrCourseTime2[3]>0)
 {
 lcd_put_cur(1,8);
 formatTime(arrCourseTime2[3], timeStr4);
lcd_send_string(timeStr4);
 arrCourseTime2[3]--;
 }
 break;
 case3:
 if(arrCourseTime2[maxIndex2]>0)
 {
 temp=arrCourseTime2[maxIndex2];
 lcd_put_cur(1,8);
 formatTime(arrCourseTime2[maxIndex2], timeStr4);
 lcd_send_string(timeStr4);
 arrCourseTime2[maxIndex2]--;
 }
 if(temp<=arrCourseTime2[0]&&arrCourseTime2[0]>0)
 {
 lcd_put_cur(0,0);
 formatTime(arrCourseTime2[0], timeStr1);
 lcd_send_string(timeStr1);
 arrCourseTime2[0]--;
 }
 if(temp<=arrCourseTime2[1]&&arrCourseTime2[1]>0)
 {
 lcd_put_cur(0,8);
 formatTime(arrCourseTime2[1], timeStr2);
 lcd_send_string(timeStr2);
 arrCourseTime2[1]--;
 }
 if(temp<=arrCourseTime2[2]&&arrCourseTime2[2]>0)
 {
 lcd_put_cur(1,0);
 formatTime(arrCourseTime2[2], timeStr3);
 lcd_send_string(timeStr3);
 arrCourseTime2[2]--;
 }
 break;
 }
 if(arrCourseTime2[0]==0&&arrCourseTime2[1]==0&&arrCourseTime2[2]==0
 &&arrCourseTime2[3]==0)
 {
 lcd_clear();
 lcd_put_cur(0,0);
 lcd_send_string("CourseFinish!");
 buzzerSig=true;
 xSemaphoreGive(lcdSem);
 //UART_Print("S/WTimer lcdSemGive!\r\n");
 xTimerStop(userTimerHandler,0);
 }
 break;
 case3:
 //UART_Print("SUB2CourseTimerStart!");
 switch(maxIndex3)
{
 case0:
 if(arrCourseTime3[maxIndex3]>0)
 {
 temp=arrCourseTime3[maxIndex3];
 lcd_put_cur(0,0);
 formatTime(arrCourseTime3[maxIndex3], timeStr1);
 lcd_send_string(timeStr1);
 arrCourseTime3[maxIndex3]--;
 }
 if(temp<=arrCourseTime3[1]&&arrCourseTime3[1]>0)
 {
 lcd_put_cur(0,8);
 formatTime(arrCourseTime3[1], timeStr2);
 lcd_send_string(timeStr2);
 arrCourseTime3[1]--;
 }
 if(temp<=arrCourseTime3[2]&&arrCourseTime3[2]>0)
 {
 lcd_put_cur(1,0);
 formatTime(arrCourseTime3[2], timeStr3);
 lcd_send_string(timeStr3);
 arrCourseTime3[2]--;
 }
 if(temp<=arrCourseTime3[3]&&arrCourseTime3[3]>0)
 {
 lcd_put_cur(1,8);
 formatTime(arrCourseTime3[3], timeStr4);
 lcd_send_string(timeStr4);
 arrCourseTime3[3]--;
 }
 break;
 case1:
 if(arrCourseTime3[maxIndex3]>0)
 {
 temp=arrCourseTime3[maxIndex3];
 lcd_put_cur(0,8);
 formatTime(arrCourseTime3[maxIndex3], timeStr2);
 lcd_send_string(timeStr2);
 arrCourseTime3[maxIndex3]--;
 }
 if(temp<=arrCourseTime3[0]&&arrCourseTime3[0]>0)
 {
 lcd_put_cur(0,0);
 formatTime(arrCourseTime3[0], timeStr1);
 lcd_send_string(timeStr1);
 arrCourseTime3[0]--;
 }
 if(temp<=arrCourseTime3[2]&&arrCourseTime3[2]>0)
 {
 lcd_put_cur(1,0);
formatTime(arrCourseTime3[2], timeStr3);
 lcd_send_string(timeStr3);
 arrCourseTime3[2]--;
 }
 if(temp<=arrCourseTime3[3]&&arrCourseTime3[3]>0)
 {
 lcd_put_cur(1,8);
 formatTime(arrCourseTime3[3], timeStr4);
 lcd_send_string(timeStr4);
 arrCourseTime3[3]--;
 }
 break;
 case2:
 if(arrCourseTime3[maxIndex3]>0)
 {
 temp=arrCourseTime3[maxIndex3];
 lcd_put_cur(1,0);
 formatTime(arrCourseTime3[maxIndex3], timeStr3);
 lcd_send_string(timeStr3);
 arrCourseTime3[maxIndex3]--;
 }
 if(temp<=arrCourseTime3[0]&&arrCourseTime3[0]>0)
 {
 lcd_put_cur(0,0);
 formatTime(arrCourseTime3[0], timeStr1);
 lcd_send_string(timeStr1);
 arrCourseTime3[0]--;
 }
 if(temp<=arrCourseTime3[1]&&arrCourseTime3[1]>0)
 {
 lcd_put_cur(0,8);
 formatTime(arrCourseTime3[1], timeStr2);
 lcd_send_string(timeStr2);
 arrCourseTime3[1]--;
 }
 if(temp<=arrCourseTime3[3]&&arrCourseTime3[3]>0)
 {
 lcd_put_cur(1,8);
 formatTime(arrCourseTime3[3], timeStr4);
 lcd_send_string(timeStr4);
 arrCourseTime3[3]--;
 }
 break;
 case3:
 if(arrCourseTime3[maxIndex3]>0)
 {
 temp=arrCourseTime3[maxIndex3];
 lcd_put_cur(1,8);
 formatTime(arrCourseTime3[maxIndex3], timeStr4);
 lcd_send_string(timeStr4);
 arrCourseTime3[maxIndex3]--;
}
 if(temp<=arrCourseTime3[0]&&arrCourseTime3[0]>0)
 {
 lcd_put_cur(0,0);
 formatTime(arrCourseTime3[0], timeStr1);
 lcd_send_string(timeStr1);
 arrCourseTime3[0]--;
 }
 if(temp<=arrCourseTime3[1]&&arrCourseTime3[1]>0)
 {
 lcd_put_cur(0,8);
 formatTime(arrCourseTime3[1], timeStr2);
 lcd_send_string(timeStr2);
 arrCourseTime3[1]--;
 }
 if(temp<=arrCourseTime3[2]&&arrCourseTime3[2]>0)
 {
 lcd_put_cur(1,0);
 formatTime(arrCourseTime3[2], timeStr3);
 lcd_send_string(timeStr3);
 arrCourseTime3[2]--;
 }
 break;
 }
 if(arrCourseTime3[0]==0&&arrCourseTime3[1]==0&&arrCourseTime3[2]==0
 &&arrCourseTime3[3]==0)
 {
 lcd_clear();
 lcd_put_cur(0,0);
 lcd_send_string("CourseFinish!");
 buzzerSig=true;
 xSemaphoreGive(lcdSem);
 UART_Print("S/WTimer lcdSemGive!\r\n");
 xTimerStop(userTimerHandler,0);
 }
 break;
 default:
 //UART_Print("AppetizerorDesertCourseTimerStart!\r\n");
 lcd_clear();
 lcd_put_cur(0,0);
 formatTime(arrCourseTime[0], timeStr1);
 lcd_send_string(timeStr1);
 lcd_put_cur(0,8);
 formatTime(arrCourseTime[1], timeStr2);
 lcd_send_string(timeStr2);
 lcd_put_cur(1,0);
 formatTime(arrCourseTime[2], timeStr3);
 lcd_send_string(timeStr3);
 lcd_put_cur(1,8);
 formatTime(arrCourseTime[3], timeStr4);
 lcd_send_string(timeStr4);
for(int i=0; i<4; i++)
 arrCourseTime[i]--;
 if(arrCourseTime[0]==0&&arrCourseTime[1]==0&&arrCourseTime[2]==0&&
 arrCourseTime[3]==0)
 {
 lcd_clear();
 lcd_put_cur(0,0);
 lcd_send_string("CourseFinish!");
 buzzerSig=true;
 xSemaphoreGive(lcdSem);
 UART_Print("S/WTimer lcdSemGive!\r\n");
 xTimerStop(userTimerHandler,0);
 }
 break;
 }
 }
 voidformatTime(uint8_t time,char*str)
 {
 uint8_tminutes=(time/60);
 uint8_tseconds=(time/1);
 sprintf(str,"%02d:%02d",minutes,seconds);
 }
 uint8_t findMaxIndex(uint8_tarr[],uint8_tsize)
 {
 uint8_tmaxIndex=0;
 for(int i=1; i<size; i++)
 {
 if(arr[i]>arr[maxIndex])
 maxIndex=i;
 }
 returnmaxIndex;
 }
 voidHAL_SPI_RxCpltCallback(SPI_HandleTypeDef*hspi)
 {
 if(hspi->Instance==SPI1)
 {
 startSig=true;
 lcdState=true;
 //UART_Print("SPIrxBufferReceived!\r\n");
 BaseType_txHigherPriorityTaskWoken=pdFALSE;
 for(int i=0; i<5; i++)
 rxBuffer[i]=rxBuffer[i]-'0';
 HAL_SPI_Receive_IT(&hspi1,rxBuffer,sizeof(rxBuffer));
 xSemaphoreGiveFromISR(spiSem,&xHigherPriorityTaskWoken);
 //UART_Print("SPI_ISRspiSemGive!\r\n");
 portYIELD_FROM_ISR(xHigherPriorityTaskWoken);
 }
 }
 voidcopyTask(void*argument)
 {
 while(1)
{
 //UART_Print("copyTaskexcution!\r\n");
 if(xSemaphoreTake(spiSem,portMAX_DELAY)==pdTRUE)
 {
 //UART_Print("copyTaskspiSemTake!\r\n");
 memcpy(&whatCourse,&rxBuffer[0],sizeof(rxBuffer[0]));
 switch(whatCourse)
 {
 case1:
 for(int i=0; i<4; i++)
 memcpy(&userData[i].changeCourse[0],
 &rxBuffer[i+1],1);
 break;
 case2:
 for(int i=0; i<4; i++)
 memcpy(&userData[i].changeCourse[1],
 &rxBuffer[i+1],1);
 break;
 case3:
 for(int i=0; i<4; i++)
 memcpy(&userData[i].changeCourse[2],
 &rxBuffer[i+1],1);
 break;
 }
 xSemaphoreGive(cookSem);
 //UART_Print("copyTaskcookSemGive!\r\n");
 }
 vTaskDelay(500);
 }
 }
 voidcookTask(void*argument)
 {
 while(1)
 {
 if(startSig)
 {
 //UART_Print("cookTask(startSig==true)\r\n");
 if(xSemaphoreTake(cookSem,portMAX_DELAY)==pdTRUE)
 {
 switch(whatCourse)
 {
 case0:
 for(int i=0; i<6; i++)
 remainingStuff[i]--;
 printRemainingStuff(Seaweed);
 printRemainingStuff(Potato);
 printRemainingStuff(SalmonRoe);
 printRemainingStuff(Deodeok);
 printRemainingStuff(Radish);
 printRemainingStuff(RedCabbage);
 xSemaphoreGive(lcdSem);
//UART_Print("cookTasklcdSemGive(whatCourse==
 0)!\r\n");
 break;
 case1:
 for(int i=0; i<4; i++)
 {
 userData[i].courseData[1][0]+=
 userData[i].changeCourse[0];
 if(userData[i].courseData[1][0]==
 AlmondWithCavior)
 {
 for(int j=6; j<10; j++)
 remainingStuff[j]--;
 }
 elseif(userData[i].courseData[1][0]==
 SnowCrabAndPickledChrysanthemum)
 {
 userData[i].courseData[1][1]+=2;
 for(intk=10;k<13;k++)
 remainingStuff[k]--;
 }
 elseif(userData[i].courseData[1][0]==
 AblaloneTaco)
 {
 userData[i].courseData[1][1]-=2;
 remainingStuff[13]--;
 }
 }
 printRemainingStuff(Almond);
 printRemainingStuff(Cavior);
 printRemainingStuff(Tofu);
 printRemainingStuff(Egg);
 printRemainingStuff(Yuzu);
 printRemainingStuff(Snowcrab);
 printRemainingStuff(Beansprouts);
 printRemainingStuff(Ablalone);
 xSemaphoreGive(lcdSem);
 //UART_Print("cookTasklcdSemGive(whatCourse==
 1)!\r\n");
 break;
 case2:
 for(int i=0; i<4; i++)
 {
 userData[i].courseData[2][0]+=
 userData[i].changeCourse[1];
 if(userData[i].courseData[2][0]==
 HearthOvenGrilledHanwoo)
 {
 for(int j=14; j<17; j++)
 remainingStuff[j]--;
 }
elseif(userData[i].courseData[2][0]==
 EmberToastedAcornNoodle)
 {
 userData[i].courseData[2][1]-=2;
 for(intk=17;k<20;k++)
 remainingStuff[k]--;
 }
 elseif(userData[i].courseData[2][0]==
 BlackSesameSeaurchinNasturtium)
 {
 userData[i].courseData[2][1]-=2;
 remainingStuff[20]--;
 }
 }
 printRemainingStuff(KoreanBeef);
 printRemainingStuff(Apple);
 printRemainingStuff(Onion);
 printRemainingStuff(Acorn);
 printRemainingStuff(Truffle);
 printRemainingStuff(Noddle);
 printRemainingStuff(Sesame);
 xSemaphoreGive(lcdSem);
 //UART_Print("cookTasklcdSemGive(whatCourse==
 2)!\r\n");
 break;
 case3:
 for(int i=0; i<4; i++)
 {
 userData[i].courseData[3][0]+=
 userData[i].changeCourse[2];
 if(userData[i].courseData[3][0]==
 BurdockTarteTatinWithSkinJuice)
 remainingStuff[21]--;
 elseif(userData[i].courseData[3][0]==
 TilefishMustardBrassica)
 {
 userData[i].courseData[3][1]+=2;
 for(int j=22; j<25; j++)
 remainingStuff[j]--;
 }
 elseif(userData[i].courseData[3][0]==fattyTuna)
 {
 userData[i].courseData[3][1]-=2;
 remainingStuff[25]--;
 }
 }
 printRemainingStuff(Burdock);
 printRemainingStuff(Tilefish);
 printRemainingStuff(Mustard);
 printRemainingStuff(Brassica);
 printRemainingStuff(Tuna);
xSemaphoreGive(lcdSem);
 //UART_Print("cookTasklcdSemGive(whatCourse==
 3)!\r\n");
 break;
 case4:
 for(int i=26; i<30; i++)
 remainingStuff[i]--;
 printRemainingStuff(Tuna);
 printRemainingStuff(Hibiscus);
 printRemainingStuff(Peanut);
 printRemainingStuff(HoneyCookie);
 printRemainingStuff(Kombucha);
 xSemaphoreGive(lcdSem);
 //UART_Print("cookTasklcdSemGive(whatCourse==
 4)!\r\n");
 break;
 }
 }
 }
 vTaskDelay(500);
 }
 }
 voidlcdTask(void*argument)
 {
 while(1)
 {
 if(!startSig)
 {
 lcd_clear();
 lcd_put_cur(0,0);
 lcd_send_string("Seaweed");
 vTaskDelay(1000);
 lcd_clear();
 lcd_put_cur(0,0);
 lcd_send_string("Potato");
 vTaskDelay(1000);
 lcd_clear();
 lcd_put_cur(0,0);
 lcd_send_string("SalmonRoe");
 vTaskDelay(1000);
 lcd_clear();
 lcd_put_cur(0,0);
 lcd_send_string("Deodeok");
 vTaskDelay(1000);
 lcd_clear();
 lcd_put_cur(0,0);
 lcd_send_string("Radish");
 vTaskDelay(1000);
 lcd_put_cur(0,0);
 lcd_send_string("RedCabbage");
 vTaskDelay(1000);
}
 else
 {
 switch(whatCourse)
 {
 case0:
 if(xSemaphoreTake(lcdSem,portMAX_DELAY)==pdTRUE)
 {
 //UART_Print("lcdTasklcdSemTake(whatCourse==
 0)!\r\n");
 if(lcdState)
 {
 for(int i=0; i<4; i++)
 memcpy(&arrCourseTime[i],
 &userData[i].courseData[0][1],1);
 maxIndex=findMaxIndex(arrCourseTime,4);
 lcdState=false;
 xTimerStart(userTimerHandler,0);
 }
 else
 {
 lcd_clear();
 lcd_put_cur(0,0);
 lcd_send_string("Almond");
 lcd_put_cur(0,8);
 lcd_send_string("Cavior");
 lcd_put_cur(1,0);
 lcd_send_string("Tofu");
 lcd_put_cur(1,8);
 lcd_send_string("Egg");
 vTaskDelay(1000);
 lcd_clear();
 lcd_put_cur(0,0);
 lcd_send_string("Yuzu");
 lcd_put_cur(0,8);
 lcd_send_string("Snowcrab");
 lcd_put_cur(1,0);
 lcd_send_string("Beansprouts");
 vTaskDelay(1000);
 xSemaphoreGive(lcdSem);
 //UART_Print("lcdTasklcdSemGive(whatCourse
 ==0&&lcdState==false)!\r\n");
 }
 }
 break;
 case1:
 if(xSemaphoreTake(lcdSem,portMAX_DELAY)==pdTRUE)
 {
 //UART_Print("lcdTasklcdSemTake(whatCourse==
 1)!\r\n");
 if(lcdState)
{
 for(int i=0; i<4; i++)
 memcpy(&arrCourseTime1[i],
 &userData[i].courseData[1][1],1);
 maxIndex1=findMaxIndex(arrCourseTime1,4);
 lcdState=false;
 xTimerStart(userTimerHandler,0);
 }
 else
 {
 lcd_clear();
 lcd_put_cur(0,0);
 lcd_send_string("KoreanBeef");
 lcd_put_cur(1,0);
 lcd_send_string("Apple");
 lcd_put_cur(1,8);
 lcd_send_string("Onion");
 vTaskDelay(1000);
 lcd_clear();
 lcd_put_cur(0,0);
 lcd_send_string("Acorn");
 lcd_put_cur(0,8);
 lcd_send_string("Truffle");
 lcd_put_cur(1,0);
 lcd_send_string("Noddle");
 vTaskDelay(1000);
 lcd_clear();
 lcd_put_cur(0,0);
 lcd_send_string("Sesame");
 vTaskDelay(1000);
 xSemaphoreGive(lcdSem);
 //UART_Print("lcdTasklcdSemGive(whatCourse
 ==1&&lcdState==false)!\r\n");
 }
 }
 break;
 case2:
 if(xSemaphoreTake(lcdSem,portMAX_DELAY)==pdTRUE)
 {
 //UART_Print("lcdTasklcdSemTake(whatCourse==
 2)!\r\n");
 if(lcdState)
 {
 for(int i=0; i<4; i++)
 memcpy(&arrCourseTime2[i],
 &userData[i].courseData[2][1],1);
 maxIndex2=findMaxIndex(arrCourseTime2,4);
 lcdState=false;
 xTimerStart(userTimerHandler,0);
 }
 else
{
 lcd_clear();
 lcd_put_cur(0,0);
 lcd_send_string("Burdock");
 vTaskDelay(1000);
 lcd_clear();
 lcd_put_cur(0,0);
 lcd_send_string("Tilefish");
 lcd_put_cur(0,8);
 lcd_send_string("Mustard");
 lcd_put_cur(1,0);
 lcd_send_string("Brassica");
 vTaskDelay(1000);
 lcd_clear();
 lcd_put_cur(0,0);
 lcd_send_string("Tuna");
 vTaskDelay(1000);
 xSemaphoreGive(lcdSem);
 //UART_Print("lcdTasklcdSemGive(whatCourse
 ==2&&lcdState==false)!\r\n");
 }
 }
 break;
 case3:
 if(xSemaphoreTake(lcdSem,portMAX_DELAY)==pdTRUE)
 {
 //UART_Print("lcdTasklcdSemTake(whatCourse==
 3)!\r\n");
 if(lcdState)
 {
 for(int i=0; i<4; i++)
 memcpy(&arrCourseTime3[i],
 &userData[i].courseData[3][1],1);
 maxIndex3=findMaxIndex(arrCourseTime3,4);
 lcdState=false;
 xTimerStart(userTimerHandler,0);
 }
 else
 {
 lcd_clear();
 lcd_put_cur(0,0);
 lcd_send_string("Hibiscus");
 lcd_put_cur(1,0);
 lcd_send_string("Peanut");
 vTaskDelay(1000);
 lcd_clear();
 lcd_put_cur(0,0);
 lcd_send_string("HoneyCookie");
 lcd_put_cur(1,0);
 lcd_send_string("Kombucha");
 vTaskDelay(1000);
xSemaphoreGive(lcdSem);
 //UART_Print("lcdTasklcdSemGive(whatCourse
 ==3&&lcdState==false)!\r\n");
 }
 }
 break;
 case4:
 if(xSemaphoreTake(lcdSem,portMAX_DELAY)==pdTRUE)
 {
 if(lcdState)
 {
 //UART_Print("lcdTasklcdSemTake(whatCourse
 ==4)!\r\n");
 for(int i=0; i<4; i++)
 memcpy(&arrCourseTime[i],
 &userData[i].courseData[0][1],1);
 maxIndex=findMaxIndex(arrCourseTime,4);
 lcdState=false;
 xTimerStart(userTimerHandler,0);
 }
 }
 break;
 }
 }
 vTaskDelay(500);
 }
 }
 voidbuzzerTask(void*argument)
 {
 while(1)
 {
 //UART_Print("buzzerTaskexecution!\r\n");
 if(buzzerSig)
 {
 txData=0x31;
 HAL_SPI_Transmit(&hspi1,&txData,sizeof(txData),HAL_MAX_DELAY);
 //UART_Print("SPI txDatatransmit tomaster!\r\n");
 HAL_GPIO_WritePin(Buzzer_GPIO_Port,Buzzer_Pin,GPIO_PIN_SET);
 HAL_GPIO_WritePin(LED_GPIO_Port,LED_Pin,GPIO_PIN_SET);
 vTaskDelay(500);
 HAL_GPIO_WritePin(Buzzer_GPIO_Port,Buzzer_Pin,
 GPIO_PIN_RESET);
 HAL_GPIO_WritePin(LED_GPIO_Port,LED_Pin,GPIO_PIN_RESET);
 vTaskDelay(500);
 HAL_GPIO_WritePin(LED_GPIO_Port,LED_Pin,GPIO_PIN_SET);
 vTaskDelay(500);
 HAL_GPIO_WritePin(LED_GPIO_Port,LED_Pin,GPIO_PIN_RESET);
 vTaskDelay(500);
 HAL_GPIO_WritePin(LED_GPIO_Port,LED_Pin,GPIO_PIN_SET);
 vTaskDelay(500);
 HAL_GPIO_WritePin(LED_GPIO_Port,LED_Pin,GPIO_PIN_RESET);
vTaskDelay(500);
 txData=0x30;
 if(whatCourse>=4)
 {
 whatCourse=0;
 startSig=false;
 dataInit();
 }
 buzzerSig=!buzzerSig;
 }
 vTaskDelay(500);
 }
 }
 /*USERCODEEND4*/
 /*USERCODEBEGINHeader_StartdefaultTask*/
 /**
 *@brief FunctionimplementingthedeaultTaskthread.
 *@paramargument:Notused
 *@retvalNone
 */
 /*USERCODEENDHeader_StartdefaultTask*/
 /**
 *@brief Periodelapsedcallbackinnonblockingmode
 *@note ThisfunctioniscalledwhenTIM2interrupt tookplace, inside
 *HAL_TIM_IRQHandler(). Itmakesadirectcall toHAL_IncTick()toincrement
 *aglobalvariable"uwTick"usedasapplicationtimebase.
 *@paramhtim:TIMhandle
 *@retvalNone
 */
 voidHAL_TIM_PeriodElapsedCallback(TIM_HandleTypeDef*htim)
 {
 /*USERCODEBEGINCallback0*/
 /*USERCODEENDCallback0*/
 if(htim->Instance==TIM2){
 HAL_IncTick();
 }
 /*USERCODEBEGINCallback1*/
 /*USERCODEENDCallback1*/
 }
 /**
 *@brief Thisfunctionisexecutedincaseoferroroccurrence.
 *@retvalNone
 */
 voidError_Handler(void)
 {
 /*USERCODEBEGINError_Handler_Debug*/
 /*Usercanaddhisownimplementationtoreport theHALerrorreturnstate*/
 __disable_irq();
 while(1)
 {
 }
/*USERCODEENDError_Handler_Debug*/
 }
 #ifdef USE_FULL_ASSERT
 /**
 *@brief Reportsthenameof thesourcefileandthesourcelinenumber
 * wheretheassert_paramerrorhasoccurred.
 *@param file:pointertothesourcefilename
 *@param line:assert_paramerror linesourcenumber
 *@retvalNone
 */
 voidassert_failed(uint8_t*file,uint32_t line)
 {
 /*USERCODEBEGIN6*/
 /*Usercanaddhisownimplementationtoreport thefilenameandlinenumber,
 ex:printf("Wrongparametersvalue: file%sonline%d\r\n", file, line)*/
 /*USERCODEEND6*/
 }
 #endif/*USE_FULL_ASSERT*/
