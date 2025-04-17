/* USER CODE BEGIN Header */
 /**
 ****************************************************************************
 **
 * @file
 : main.c
 * @brief
 : Main program body
 ****************************************************************************
 **
 * @attention
 *
 * Copyright (c) 2024 STMicroelectronics.
*Allrightsreserved.
 *
 *ThissoftwareislicensedundertermsthatcanbefoundintheLICENSE
 file
 *intherootdirectoryofthissoftwarecomponent.
 *IfnoLICENSEfilecomeswiththissoftware,itisprovidedAS-IS.
 *
 ****************************************************************************
 **
 */
 /*USERCODEENDHeader*/
 /*Includes------------------------------------------------------------------*/
 #include"main.h"
 //#include"cmsis_os.h"
 /*Privateincludes----------------------------------------------------------*/
 /*USERCODEBEGINIncludes*/
 #include"FreeRTOS.h"
 #include"task.h"
 //#include"timers.h"
 #include"queue.h"
 #include"semphr.h"
 #include"stdbool.h"
 //#include"event_groups.h"
 #include"string.h"
 #include"stdio.h"
 #include"i2c-lcd.h"
 #defineSLAVE_ADDRESS_LCD0x4E//changethisaccordingtoursetup
 #defineMAX_GUESTS4
 #defineQUEUE_LENGTH10
 #defineQUEUE_ITEM_SIZEsizeof(uint8_t)
 /*USERCODEENDIncludes*/
 /*Privatetypedef-----------------------------------------------------------*/
 /*USERCODEBEGINPTD*/
 /*USERCODEENDPTD*/
 /*Privatedefine------------------------------------------------------------*/
 /*USERCODEBEGINPD*/
 /*USERCODEENDPD*/
 /*Privatemacro-------------------------------------------------------------*/
 /*USERCODEBEGINPM*/
 TaskHandle_tArrive_Handler; //Task1handler
 TaskHandle_tDecide_Handler; //Task2handler
 SemaphoreHandle_txSemaphore; //Semaphorefortasksynchronization
 SemaphoreHandle_tTaskFlowSemaphore;
 /*USERCODEENDPM*/
 /*Privatevariables---------------------------------------------------------*/
I2C_HandleTypeDefhi2c1;
 SPI_HandleTypeDefhspi1;
 SPI_HandleTypeDefhspi2;
 UART_HandleTypeDefhuart2;
 osThreadIddefaultTaskHandle;
 /*USERCODEBEGINPV*/
 #defineTASK1_PRIORITY3//Highestpriority
 #defineTASK2_PRIORITY2//Mediumpriority
 #defineTASK3_PRIORITY1//Lowestpriority
 //#defineEVENT_GUESTS_ARRIVED (1<<0)
 //#defineEVENT_MENU_SELECTED (1<<1)
 //#defineEVENT_GUESTS_LEAVING (1<<2)
 #defineDEBOUNCE_DELAY_MS200
 //Variablestostorethelastdebouncetimesforeachbutton
 uint32_tlast_debounce_time_B1=0;
 uint32_tlast_debounce_time_B2=0;
 uint32_tlast_debounce_time_B3=0;
 uint32_tlast_debounce_time_B4=0;
 intMENU_INDEX=0;
 /*GlobalVariables*/
 uint8_treset_data2=0x31;
 typedefstruct{
 uint8_tcourseSelections[4];//Selectionsfor4guests
 uint8_tspeed; //Speedvariable(1,2,or3)
 }CombinedData_t;
 _BoolB1_Flag=false;
 _BoolWait_Flag=false;
 _BoolEnter_Flag=true;
 _BoolSelect_Flag=false;
 //Task2Variables
 uint8_tCourse_Index=0; //Representsthecurrentcoursebeing
 selected(SUB1,etc.)
 uint8_tMenu_Buffer[5]={0}; //Buffertostorecourseorderand
 selections(Course_Index+4guestselections)
 intcurrent_Guest=0; //Trackswhichguestismakinga
 selection(0to3for4guests)
 uint8_tMenu_Selection=0; //Tracksthecurrentselection(0=
 original,1=alternative1,2=alternative2)
 uint8_tSTM1_Signal=0x00;
 constchar*courseNames[]={"Appetizer","Sub1","Main","Sub2",
 "Dessert"};
 constintselectableCourses[]={0,1,1,1,0};//0=notselectable,1=
 selectable
 volatileboolTask2Ready=false;//Task1signalsTask2onlyifTask2is
 ready
 volatileboolTask3Ready=false;//Task2signalsTask3onlyifTask3is
 ready
 volatileboolAllGuestsSelected=false;//TracksifTask2hascompleted
 guestselections
 volatileintcurrentCourse=0; //Tracksthecurrentcoursebeing
 processed
volatileboolAllCoursesDone=false;//Flagtoindicateallcourseshave
 beenprocessed
 charkey;
 uint8_tcheck_Num=0x00;
 uint8_tRx_Data=0x00;
 intspeed=0;
 _BoolSpeed_Flag=false;
 intcurrent_Index=1;
 intMenu_Select=0;
 intMenu_All_Index=0;
 uint8_ttransfer_Data[5]={0};
 intNum_People=0; //Counterforpeople
 uint8_tsend_msg[10]; //UARTmessagebuffer
 CombinedData_tcombinedData; //Shareddatastructurefor
 coursesandspeed
 uint8_tcourseSelections[4]={0}; //Arrayforcourseselections
 SemaphoreHandle_tDataMutex; //Mutextoprotectaccessto
 shareddata
 SemaphoreHandle_tTaskSyncSemaphore; //Binarysemaphorefortask
 synchronization
 QueueHandle_tCourseQueue; //Queueforstoringcourse
 selections
 SemaphoreHandle_tQueueMutex; //Mutextoprotectthequeue
 TaskHandle_tTask1Handle=NULL;
 TaskHandle_tTask2Handle=NULL;
 TaskHandle_tTask3Handle=NULL;
 SemaphoreHandle_tTask1ToTask2;//Binarysemaphore:Task1signalsTask2
 SemaphoreHandle_tTask2ToTask3;//Binarysemaphore:Task2signalsTask3
 SemaphoreHandle_tDataMutex; //Mutexforshareddata
 uint8_ttxBuffer=0xFF;//Dummydataformultiplebytes
 uint8_trxBuffer=0; //Bufferforreceiveddata
 //TaskPriorities
 #defineTASK1_PRIORITY3
 #defineTASK2_PRIORITY2
 #defineTASK3_PRIORITY1
 #defineMAX_DISH_NAME_LENGTH32
 //Defineastructureforadish
 typedefstruct
 {
 chardefaultDish[MAX_DISH_NAME_LENGTH]; //Defaultdishname
 charalternative1[MAX_DISH_NAME_LENGTH];//Alternativedish1
 charalternative2[MAX_DISH_NAME_LENGTH];//Alternativedish2
 }DishOptions;
 //Defineastructureforthecourse
 typedefstruct
 {
 charappetizer[MAX_DISH_NAME_LENGTH]; //Appetizer(fingerfood)
 DishOptionssub1; //Sub1
 DishOptionsmain; //Maincourse
 DishOptionssub2; //Sub2
 chardessert[MAX_DISH_NAME_LENGTH]; //Dessert
 }Course;
charkeypad;
 _BoolBut1_Flag=false;
 _BoolBut2_Flag=false;
 _BoolBut3_Flag=false;
 _BoolBut4_Flag=false;
 /*USERCODEENDPV*/
 /*Privatefunctionprototypes-----------------------------------------------*/
 voidSystemClock_Config(void);
 staticvoidMX_GPIO_Init(void);
 staticvoidMX_USART2_UART_Init(void);
 staticvoidMX_SPI1_Init(void);
 staticvoidMX_I2C1_Init(void);
 staticvoidMX_SPI2_Init(void);
 voidStartDefaultTask(voidconst*argument);
 /*USERCODEBEGINPFP*/
 voidSendUARTMessage(constchar*message);
 //charread_keypad(void);
 voidDisplayMenu(void);
 voidTask1(void*pvParameters);
 voidTask2(void*pvParameters);
 voidTask3(void*pvParameters);
 /*USERCODEENDPFP*/
 /*Privateusercode---------------------------------------------------------*/
 /*USERCODEBEGIN0*/
 /*USERCODEEND0*/
 /**
 *@brief Theapplicationentrypoint.
 *@retvalint
 */
 intmain(void)
 {
 /*USERCODEBEGIN1*/
 /*USERCODEEND1*/
 /*MCU
 Configuration--------------------------------------------------------*/
 /*Resetofallperipherals,InitializestheFlashinterfaceandthe
 Systick.*/
 HAL_Init();
 /*USERCODEBEGINInit*/
 /*USERCODEENDInit*/
 /*Configurethesystemclock*/
 SystemClock_Config();
 /*USERCODEBEGINSysInit*/
 /*USERCODEENDSysInit*/
 /*Initializeallconfiguredperipherals*/
 MX_GPIO_Init();
 MX_USART2_UART_Init();
 MX_SPI1_Init();
 MX_I2C1_Init();
 MX_SPI2_Init();
/*USERCODEBEGIN2*/
 //Initializerandomseed
 // srand(HAL_GetTick());
 // HAL_UART_Receive_IT(&huart2,&STM1_Signal,1);
 HAL_SPI_TransmitReceive_IT(&hspi1,&dummyData,&receivedData,1);
 //HAL_SPI_Receive_IT(&hspi1,pData,Size)
 //Otherinitializations...
 //CreateSemaphoreandMutex
 TaskFlowSemaphore=xSemaphoreCreateCounting(3,1);//Countingsemaphore
 DataMutex=xSemaphoreCreateMutex(); //Mutexforshared
 resources
 //CreateTasks
 xTaskCreate(Task1,"Task1",128,NULL,TASK1_PRIORITY,&Task1Handle);
 xTaskCreate(Task2,"Task2",128,NULL,TASK2_PRIORITY,&Task2Handle);
 xTaskCreate(Task3,"Task3",128,NULL,TASK3_PRIORITY,&Task3Handle);
 //StartScheduler
 vTaskStartScheduler();
 /*USERCODEEND2*/
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
 RCC_OscInitStruct.HSEState=RCC_HSE_ON;
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
 *@briefI2C1InitializationFunction
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
 hspi1.Init.Mode=SPI_MODE_MASTER;
 hspi1.Init.Direction=SPI_DIRECTION_2LINES;
 hspi1.Init.DataSize=SPI_DATASIZE_8BIT;
hspi1.Init.CLKPolarity=SPI_POLARITY_LOW;
 hspi1.Init.CLKPhase=SPI_PHASE_1EDGE;
 hspi1.Init.NSS=SPI_NSS_SOFT;
 hspi1.Init.BaudRatePrescaler=SPI_BAUDRATEPRESCALER_16;
 hspi1.Init.FirstBit=SPI_FIRSTBIT_MSB;
 hspi1.Init.TIMode=SPI_TIMODE_DISABLE;
 hspi1.Init.CRCCalculation=SPI_CRCCALCULATION_DISABLE;
 hspi1.Init.CRCPolynomial=10;
 if(HAL_SPI_Init(&hspi1)!=HAL_OK)
 {
 Error_Handler();
 }
 /*USERCODEBEGINSPI1_Init2*/
 // __HAL_SPI_ENABLE(&hspi1);
 //
 // uint8_treset_data=0x31;
 //// uint8_tdata2[2]={0x31,0x32};
 // HAL_GPIO_WritePin(SS_GPIO_Port,SS_Pin,GPIO_PIN_RESET);//CSLow
 // HAL_SPI_Transmit(&hspi1,&reset_data,1,1);
 // reset_data++;
 // HAL_SPI_Transmit(&hspi1,&reset_data,1,1);
 // reset_data++;
 // HAL_SPI_Transmit(&hspi1,&reset_data,1,1);
 // reset_data++;
 // HAL_SPI_Transmit(&hspi1,&reset_data,1,1);
 // reset_data++;
 // HAL_SPI_Transmit(&hspi1,&reset_data,1,1);
 // reset_data++;
 //
 // HAL_Delay(2000);
 //
 // HAL_SPI_Transmit(&hspi1,&reset_data,1,1);
 // reset_data++;
 // HAL_SPI_Transmit(&hspi1,&reset_data,1,1);
 // reset_data++;
 // HAL_SPI_Transmit(&hspi1,&reset_data,1,1);
 // reset_data++;
 // HAL_SPI_Transmit(&hspi1,&reset_data,1,1);
 // reset_data++;
 // HAL_SPI_Transmit(&hspi1,&reset_data,1,1);
 // reset_data++;
 // HAL_GPIO_WritePin(SS_GPIO_Port,SS_Pin,GPIO_PIN_RESET);//CSLow
 // HAL_GPIO_WritePin(SS_GPIO_Port,SS_Pin,GPIO_PIN_SET);
 // HAL_Delay(1);//CSHigh
 /*USERCODEENDSPI1_Init2*/
 }
 /**
 *@briefSPI2InitializationFunction
 *@paramNone
 *@retvalNone
 */
 staticvoidMX_SPI2_Init(void)
{
 /*USERCODEBEGINSPI2_Init0*/
 /*USERCODEENDSPI2_Init0*/
 /*USERCODEBEGINSPI2_Init1*/
 /*USERCODEENDSPI2_Init1*/
 /*SPI2parameterconfiguration*/
 hspi2.Instance=SPI2;
 hspi2.Init.Mode=SPI_MODE_MASTER;
 hspi2.Init.Direction=SPI_DIRECTION_2LINES;
 hspi2.Init.DataSize=SPI_DATASIZE_8BIT;
 hspi2.Init.CLKPolarity=SPI_POLARITY_LOW;
 hspi2.Init.CLKPhase=SPI_PHASE_1EDGE;
 hspi2.Init.NSS=SPI_NSS_SOFT;
 hspi2.Init.BaudRatePrescaler=SPI_BAUDRATEPRESCALER_2;
 hspi2.Init.FirstBit=SPI_FIRSTBIT_MSB;
 hspi2.Init.TIMode=SPI_TIMODE_DISABLE;
 hspi2.Init.CRCCalculation=SPI_CRCCALCULATION_DISABLE;
 hspi2.Init.CRCPolynomial=10;
 if(HAL_SPI_Init(&hspi2)!=HAL_OK)
 {
 Error_Handler();
 }
 /*USERCODEBEGINSPI2_Init2*/
 /*USERCODEENDSPI2_Init2*/
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
 HAL_GPIO_WritePin(SS_GPIO_Port,SS_Pin,GPIO_PIN_RESET);
 /*ConfigureGPIOpinOutputLevel*/
 HAL_GPIO_WritePin(GPIOA,OLED_DC_Pin|OLED_RST_Pin,GPIO_PIN_RESET);
 /*ConfigureGPIOpinOutputLevel*/
 HAL_GPIO_WritePin(OLED_CS_GPIO_Port,OLED_CS_Pin,GPIO_PIN_RESET);
 /*ConfigureGPIOpin:SS_Pin*/
 GPIO_InitStruct.Pin=SS_Pin;
 GPIO_InitStruct.Mode=GPIO_MODE_OUTPUT_PP;
 GPIO_InitStruct.Pull=GPIO_NOPULL;
 GPIO_InitStruct.Speed=GPIO_SPEED_FREQ_LOW;
 HAL_GPIO_Init(SS_GPIO_Port,&GPIO_InitStruct);
 /*ConfigureGPIOpin:Button5_Pin*/
 GPIO_InitStruct.Pin=Button5_Pin;
 GPIO_InitStruct.Mode=GPIO_MODE_IT_RISING;
 GPIO_InitStruct.Pull=GPIO_NOPULL;
 HAL_GPIO_Init(Button5_GPIO_Port,&GPIO_InitStruct);
 /*ConfigureGPIOpin:Button1_Pin*/
 GPIO_InitStruct.Pin=Button1_Pin;
 GPIO_InitStruct.Mode=GPIO_MODE_IT_FALLING;
 GPIO_InitStruct.Pull=GPIO_PULLUP;
 HAL_GPIO_Init(Button1_GPIO_Port,&GPIO_InitStruct);
 /*ConfigureGPIOpins:Button4_PinButton3_PinButton2_Pin*/
 GPIO_InitStruct.Pin=Button4_Pin|Button3_Pin|Button2_Pin;
 GPIO_InitStruct.Mode=GPIO_MODE_IT_RISING;
 GPIO_InitStruct.Pull=GPIO_NOPULL;
 HAL_GPIO_Init(GPIOC,&GPIO_InitStruct);
 /*ConfigureGPIOpins:OLED_DC_PinOLED_RST_Pin*/
 GPIO_InitStruct.Pin=OLED_DC_Pin|OLED_RST_Pin;
 GPIO_InitStruct.Mode=GPIO_MODE_OUTPUT_PP;
 GPIO_InitStruct.Pull=GPIO_NOPULL;
 GPIO_InitStruct.Speed=GPIO_SPEED_FREQ_LOW;
 HAL_GPIO_Init(GPIOA,&GPIO_InitStruct);
 /*ConfigureGPIOpin:OLED_CS_Pin*/
 GPIO_InitStruct.Pin=OLED_CS_Pin;
 GPIO_InitStruct.Mode=GPIO_MODE_OUTPUT_PP;
 GPIO_InitStruct.Pull=GPIO_NOPULL;
 GPIO_InitStruct.Speed=GPIO_SPEED_FREQ_LOW;
HAL_GPIO_Init(OLED_CS_GPIO_Port,&GPIO_InitStruct);
 /*EXTIinterruptinit*/
 HAL_NVIC_SetPriority(EXTI15_10_IRQn,5,0);
 HAL_NVIC_EnableIRQ(EXTI15_10_IRQn);
 /*USERCODEBEGINMX_GPIO_Init_2*/
 /*USERCODEENDMX_GPIO_Init_2*/
 }
 /*USERCODEBEGIN4*/
 voidHAL_UART_RxCpltCallback(UART_HandleTypeDef*huart){
 if(huart->Instance==USART2)
 {
 if(STM1_Signal==0x31){
 Num_People++;
 STM1_Signal=0x00;
 }
 HAL_UART_Receive_IT(&huart2,&STM1_Signal,1);
 }
 }
 voidHAL_SPI_RxCpltCallback(SPI_HandleTypeDef*hspi){
 if(hspi->Instance==SPI1){
 HAL_SPI_TransmitReceive_IT(&hspi1,&dummyData,&receivedData,
 1);
 }
 }
 voidHAL_GPIO_EXTI_Callback(uint16_tGPIO_Pin){
 uint32_tcurrent_time=HAL_GetTick();//Getcurrentsystemtimeinms
 //CheckifButton1ispressedtotrackguestarrivalordeparture
 /*
 BUTTON1:GuestWantToStopTheCourses
 BUTTON2:LINEAR++
 BUTTON3:LINEAR--
 BuTTON4;SELECTCHOICE
 */
 if(GPIO_Pin==Button1_Pin){
 if(Enter_Flag==true&&Num_People==4){
 Wait_Flag=true;
 Num_People--;
 }
 else{
 Wait_Flag=false;
 Num_People++;
 }
 }
 if(GPIO_Pin==Button2_Pin){
 if((current_time-last_debounce_time_B2)>DEBOUNCE_DELAY_MS){
 last_debounce_time_B2=current_time;//Updatedebouncetime
 Menu_Selection=(Menu_Selection+1)%3;//Cyclethrough
 options(0,1,2)
 }
 }
 //HandleButton3:Decreaseselection
 elseif(GPIO_Pin==Button3_Pin){
if((current_time-last_debounce_time_B3)>DEBOUNCE_DELAY_MS){
 last_debounce_time_B3=current_time;//Updatedebouncetime
 Menu_Selection=(Menu_Selection+2)%3;//Cyclebackwards
 throughoptions(0,1,2)
 }
 }
 //HandleButton4:Confirmselection
 elseif(GPIO_Pin==Button4_Pin){
 if((current_time-last_debounce_time_B4)>DEBOUNCE_DELAY_MS){
 last_debounce_time_B4=current_time;//Updatedebouncetime
 But4_Flag=true;//Settheflagtoconfirmselection
 current_Guest++;
 Menu_Buffer[current_Guest]=Menu_Selection+0x30;//Store
 selection
 if(current_Guest==4){
 currentCourse++;
 AllGuestsSelected=true;
 SendUARTMessage("AllGuestsareSelected!");
 }
 }
 elseif(AllGuestsSelected==true&&Speed_Flag==true){
 Speed_Flag=false;
 }
 }
 elseif(GPIO_Pin==Button5_Pin){
 Speed_Flag=true;
 speed=(speed+1)%3;
 }
 }
 voidRESET_FUNC(void){
 currentCourse=0;
 Num_People=0;
 Enter_Flag=false;
 Wait_Flag=false;
 current_Index=0;
 current_Guest=0;
 Menu_Selection=0;
 MENU_INDEX=0;
 }
 voidDisplayMenu(void){
 //MenuString
 constchar*menu=
 "------TODAYCOURSE------\n"
 "1.Appetizer:[CannotReplacement.]\n"
 " \"SmallBitesDishes\"\n"
 "2.SubDish_1:[ReplacementAvailable]\n"
 " OriginalCourse:\"AlmondWithCavior\"\n"
 " Replacement1:\"SnowCrabAndPickledChrysanthemum\"\n"
 " Replacement2:\"AblaloneTaco\"\n"
 "3.MainDish:[ReplacementAvailable]\n"
 " OriginalCourse:\"HearthOvenGrilledHanwoo\"\n"
 " Replacement1:\"EmberToastedAcornNoodle\"\n"
" Replacement2:\"BlackSesameSeaurchinNasturtium\"\n"
 "4.SubDish_2:[ReplacementAvailable]\n"
 " OriginalCourse:\"BurdockTarteTatinWithSkinJuice\"\n"
 " Replacement1:\"TilefishMustardBrassica\"\n"
 " Replacement2:\"FattyTuna\"\n"
 "5.Dessert:[CannotReplacement.]\n"
 " \"SmallSweets\"\n";
 //SendMenuviaUART
 HAL_UART_Transmit(&huart2,(uint8_t*)menu,strlen(menu),
 HAL_MAX_DELAY);
 }
 //UtilityfunctiontosendUARTmessages
 voidSendUARTMessage(constchar*message)
 {
 HAL_UART_Transmit(&huart2,(uint8_t*)message,strlen(message),
 HAL_MAX_DELAY);
 }
 voidLCD_Display_Menu(void){
 lcd_init();
 lcd_send_string("Today'sCourses");
 lcd_put_cur(1,0);
 lcd_send_string("EnjoyYourMeal");
 }
 voidLCD_Wait(void){
 lcd_init();
 lcd_send_string("SomeGuestsAre");
 lcd_put_cur(1,0);
 lcd_send_string("NeedSomeTime!");
 }
 voidLCD_NotEnoughGuests(void){
 lcd_init();
 lcd_send_string("C");
 lcd_put_cur(1,0);
 lcd_send_string("AllComing!");
 }
 voidLCD_EPP(void){
 lcd_init();
 lcd_send_string("CurrentCourse");
 lcd_put_cur(1,0);
 lcd_send_string("Eppitizer");
 }
 voidLCD_SUB1(void){
 lcd_init();
 lcd_send_string("CurrentCourse");
 lcd_put_cur(1,0);
 lcd_send_string("SUB1");
 }
 voidLCD_MAIN(void){
 lcd_init();
 lcd_send_string("CurrentCourse");
 lcd_put_cur(1,0);
 lcd_send_string("MAIN");
}
 voidLCD_SUB2(void){
 lcd_init();
 lcd_send_string("CurrentCourse");
 lcd_put_cur(1,0);
 lcd_send_string("SUB2");
 }
 voidLCD_DES(void){
 lcd_init();
 lcd_send_string("CurrentCourse");
 lcd_put_cur(1,0);
 lcd_send_string("DESSERT");
 }
 voidTask1(void*pvParameters){
 while(1){
 if(xSemaphoreTake(TaskFlowSemaphore,portMAX_DELAY)==pdTRUE){
 SendUARTMessage("Task1:Running...\n");
 //Task1CriticalSection
 if(xSemaphoreTake(DataMutex,pdMS_TO_TICKS(500))==pdTRUE){
 //CheckifWait_FlagistrueandNum_Peopleis3
 if(Wait_Flag==true&&Num_People==3){
 SendUARTMessage("Task1:SuspendingTask2andTask3.\n");
 vTaskSuspend(Task2Handle);//SuspendTask2
 vTaskSuspend(Task3Handle);//SuspendTask3
 }
 //WaituntilNum_Peoplereaches4
 while(Num_People<4){
 vTaskDelay(pdMS_TO_TICKS(500));
 }
 //ResetWait_Flagandresumetasks
 if(Wait_Flag==false&&Num_People==4){
 SendUARTMessage("Task1:ResumingTask2andTask3.\n");
 vTaskResume(Task2Handle);//ResumeTask2
 vTaskResume(Task3Handle);//ResumeTask3
 }
 xSemaphoreGive(DataMutex);//Releasemutex
 }
 //SignalTask2andAdjustPriority
 SendUARTMessage("Task1:SignalingTask2.\n");
 xSemaphoreGive(TaskFlowSemaphore);
 }
 vTaskDelay(pdMS_TO_TICKS(500));
 }
 }
 voidTask2(void*pvParameters){
 while(1){
 if(xSemaphoreTake(TaskFlowSemaphore,portMAX_DELAY)==pdTRUE){
 SendUARTMessage("Task2:Running...\n");
 //Task2CriticalSection
 if(xSemaphoreTake(DataMutex,pdMS_TO_TICKS(500))==pdTRUE){
 while(current_Guest<4){
 if(current_Guest==0){
Menu_Buffer[0]=currentCourse+0x30;
 But4_Flag=false;
 }
 vTaskDelay(pdMS_TO_TICKS(100));
 }
 xSemaphoreGive(DataMutex);//Releasemutex
 }
 //SignalTask3andAdjustPriority
 SendUARTMessage("Task2:SignalingTask3.\n");
 xSemaphoreGive(TaskFlowSemaphore);
 }
 vTaskDelay(pdMS_TO_TICKS(500));
 }
 }
 voidTask3(void*pvParameters){
 while(1){
 switch(MENU_INDEX){
 case0:
 LCD_EPP();
 break;
 case1:
 LCD_SUB1();
 break;
 case2:
 LCD_MAIN();
 break;
 case3:
 LCD_SUB2();
 break;
 case4:
 LCD_DES();
 break;
 }
 if(xSemaphoreTake(TaskFlowSemaphore,portMAX_DELAY)==pdTRUE){
 SendUARTMessage("Task3:Running...\n");
 //Task3CriticalSection
 if(xSemaphoreTake(DataMutex,pdMS_TO_TICKS(500))==pdTRUE){
 HAL_GPIO_WritePin(SS_GPIO_Port,SS_Pin,GPIO_PIN_RESET);
 for(inti=0;i<5;i++){
 HAL_SPI_Transmit(&hspi1,&Menu_Buffer[i],1,3);
 }
 //EndTransferData:STM2->STM3
 HAL_GPIO_WritePin(SS_GPIO_Port,SS_Pin,GPIO_PIN_SET);
 //StartReceiveData:STM2<-STM3
 // HAL_GPIO_WritePin(SS_GPIO_Port,SS_Pin,GPIO_PIN_RESET);
 memset(Menu_Buffer,0,sizeof(Menu_Buffer));
 current_Guest=0;//Resetguestcounter
 AllGuestsSelected=false;
 xSemaphoreGive(DataMutex);//Releasemutex
 }
 while(Speed_Flag==true){
 switch(speed){
case1:
 vTaskDelay(3000);
 break;
 case2:
 vTaskDelay(7000);
 break;
 case3:
 vTaskDelay(10000);
 break;
 }
 // }
 MENU_INDEX++;
 if(currentCourse==5){
 RESET_FUNC();
 }
 //SignalTask1andAdjustPriority
 SendUARTMessage("Task3:SignalingTask1.\n");
 xSemaphoreGive(TaskFlowSemaphore);
 }
 vTaskDelay(pdMS_TO_TICKS(500));
 }
 }
 /*USERCODEEND4*/
 /**
 *@brief Periodelapsedcallbackinnonblockingmode
 *@note Thisfunctioniscalled whenTIM1interrupttookplace,inside
 *HAL_TIM_IRQHandler().ItmakesadirectcalltoHAL_IncTick()to
 increment
 *aglobalvariable"uwTick"usedasapplicationtimebase.
 *@param htim:TIMhandle
 *@retvalNone
 */
 voidHAL_TIM_PeriodElapsedCallback(TIM_HandleTypeDef*htim)
 {
 /*USERCODEBEGINCallback0*/
 /*USERCODEENDCallback0*/
 if(htim->Instance==TIM1){
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
 /*UsercanaddhisownimplementationtoreporttheHALerrorreturnstate
 */
 __disable_irq();
while(1)
 {
 }
 /*USERCODEENDError_Handler_Debug*/
 }
 #ifdef USE_FULL_ASSERT
 /**
 *@brief Reportsthenameofthesourcefileandthesourcelinenumber
 * wheretheassert_paramerrorhasoccurred.
 *@param file:pointertothesourcefilename
 *@param line:assert_paramerrorlinesourcenumber
 *@retvalNone
 */
 voidassert_failed(uint8_t*file,uint32_tline)
 {
 /*USERCODEBEGIN6*/
 /*Usercanaddhisownimplementationtoreportthefilenameandline
 number,
 ex:printf("Wrongparametersvalue:file%sonline%d\r\n",file,line)
 */
 /*USERCODEEND6*/
 }
 #endif/*USE_FULL_ASSERT*/
