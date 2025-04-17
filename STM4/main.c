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
 /*Privateincludes----------------------------------------------------------*/
 /*USERCODEBEGINIncludes*/
 #include"st7789.h"
 #include"image.h"
 /*USERCODEENDIncludes*/
/*Privatetypedef-----------------------------------------------------------*/
 /*USERCODEBEGINPTD*/
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
 /*USERCODEBEGINPV*/
 /*USERCODEENDPV*/
 /*Privatefunctionprototypes-----------------------------------------------*/
 voidSystemClock_Config(void);
 staticvoidMX_GPIO_Init(void);
 staticvoidMX_I2C1_Init(void);
 staticvoidMX_SPI1_Init(void);
 /*USERCODEBEGINPFP*/
 /*USERCODEENDPFP*/
 /*Privateusercode---------------------------------------------------------*/
 /*USERCODEBEGIN0*/
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
 MX_I2C1_Init();
 MX_SPI1_Init();
 /*USERCODEBEGIN2*/
 ST7789_Init();
 /*USERCODEEND2*/
 /*Infiniteloop*/
 /*USERCODEBEGINWHILE*/
 while(1)
{
 ST7789_DrawImage(0,40,240,240,(uint16_t*)SmallBites);
 HAL_Delay(1000);
 ST7789_Init();
 ST7789_DrawImage(0,40,240,240,(uint16_t*)AlmondWithCavior);
 HAL_Delay(1000);
 ST7789_Init();
 ST7789_DrawImage(0,40,240,240,(uint16_t*)SnowCrabAndPickledChrysanthemum);
 HAL_Delay(1000);
 ST7789_Init();
 ST7789_DrawImage(0,40,240,240,(uint16_t*)BurdockTarteTatinWithSkinJuice);
 HAL_Delay(1000);
 ST7789_Init();
 ST7789_DrawImage(0,40,240,240,(uint16_t*)EmberToastedAcornNoodle);
 HAL_Delay(1000);
 ST7789_Init();
 ST7789_DrawImage(0,40,240,240,(uint16_t*)HearthOvenGrilledHanwoo);
 HAL_Delay(1000);
 ST7789_Init();
 ST7789_DrawImage(0,40,240,240,(uint16_t*)TilefishMustardBrassica);
 HAL_Delay(1000);
 ST7789_Init();
 ST7789_DrawImage(0,40,240,240,(uint16_t*)SmallSweet1);
 HAL_Delay(1000);
 ST7789_Init();
 ST7789_DrawImage(0,40,240,240,(uint16_t*)anSungJae);
 HAL_Delay(3000);
 ST7789_Init();
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
 /**Configurethemaininternal regulatoroutputvoltage
 */
 __HAL_RCC_PWR_CLK_ENABLE();
 __HAL_PWR_VOLTAGESCALING_CONFIG(PWR_REGULATOR_VOLTAGE_SCALE1);
 /**InitializestheRCCOscillatorsaccordingtothespecifiedparameters
 *intheRCC_OscInitTypeDefstructure.
 */
 RCC_OscInitStruct.OscillatorType=RCC_OSCILLATORTYPE_HSE;
 RCC_OscInitStruct.HSEState=RCC_HSE_ON;
 RCC_OscInitStruct.PLL.PLLState=RCC_PLL_ON;
RCC_OscInitStruct.PLL.PLLSource=RCC_PLLSOURCE_HSE;
 RCC_OscInitStruct.PLL.PLLM=4;
 RCC_OscInitStruct.PLL.PLLN=168;
 RCC_OscInitStruct.PLL.PLLP=RCC_PLLP_DIV2;
 RCC_OscInitStruct.PLL.PLLQ=7;
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
 RCC_ClkInitStruct.APB1CLKDivider=RCC_HCLK_DIV4;
 RCC_ClkInitStruct.APB2CLKDivider=RCC_HCLK_DIV2;
 if(HAL_RCC_ClockConfig(&RCC_ClkInitStruct,FLASH_LATENCY_5)!=HAL_OK)
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
 hspi1.Init.Mode=SPI_MODE_MASTER;
 hspi1.Init.Direction=SPI_DIRECTION_2LINES;
 hspi1.Init.DataSize=SPI_DATASIZE_8BIT;
 hspi1.Init.CLKPolarity=SPI_POLARITY_LOW;
 hspi1.Init.CLKPhase=SPI_PHASE_1EDGE;
 hspi1.Init.NSS=SPI_NSS_SOFT;
 hspi1.Init.BaudRatePrescaler=SPI_BAUDRATEPRESCALER_4;
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
 __HAL_RCC_GPIOE_CLK_ENABLE();
 __HAL_RCC_GPIOC_CLK_ENABLE();
 __HAL_RCC_GPIOH_CLK_ENABLE();
 __HAL_RCC_GPIOA_CLK_ENABLE();
 __HAL_RCC_GPIOB_CLK_ENABLE();
 __HAL_RCC_GPIOD_CLK_ENABLE();
 /*ConfigureGPIOpinOutputLevel*/
 HAL_GPIO_WritePin(CS_I2C_SPI_GPIO_Port,CS_I2C_SPI_Pin,GPIO_PIN_RESET);
 /*ConfigureGPIOpinOutputLevel*/
 HAL_GPIO_WritePin(GPIOC,OTG_FS_PowerSwitchOn_Pin|CS_Pin,GPIO_PIN_SET);
 /*ConfigureGPIOpinOutputLevel*/
HAL_GPIO_WritePin(RES_GPIO_Port,RES_Pin,GPIO_PIN_SET);
 /*ConfigureGPIOpinOutputLevel*/
 HAL_GPIO_WritePin(DC_GPIO_Port,DC_Pin,GPIO_PIN_RESET);
 /*ConfigureGPIOpinOutputLevel*/
 HAL_GPIO_WritePin(BLK_GPIO_Port,BLK_Pin,GPIO_PIN_SET);
 /*ConfigureGPIOpinOutputLevel*/
 HAL_GPIO_WritePin(GPIOD,LD4_Pin|LD3_Pin|LD5_Pin|LD6_Pin
 |Audio_RST_Pin,GPIO_PIN_RESET);
 /*ConfigureGPIOpin:CS_I2C_SPI_Pin*/
 GPIO_InitStruct.Pin=CS_I2C_SPI_Pin;
 GPIO_InitStruct.Mode=GPIO_MODE_OUTPUT_PP;
 GPIO_InitStruct.Pull=GPIO_NOPULL;
 GPIO_InitStruct.Speed=GPIO_SPEED_FREQ_LOW;
 HAL_GPIO_Init(CS_I2C_SPI_GPIO_Port,&GPIO_InitStruct);
 /*ConfigureGPIOpin:OTG_FS_PowerSwitchOn_Pin*/
 GPIO_InitStruct.Pin=OTG_FS_PowerSwitchOn_Pin;
 GPIO_InitStruct.Mode=GPIO_MODE_OUTPUT_PP;
 GPIO_InitStruct.Pull=GPIO_NOPULL;
 GPIO_InitStruct.Speed=GPIO_SPEED_FREQ_LOW;
 HAL_GPIO_Init(OTG_FS_PowerSwitchOn_GPIO_Port,&GPIO_InitStruct);
 /*ConfigureGPIOpin:PDM_OUT_Pin*/
 GPIO_InitStruct.Pin=PDM_OUT_Pin;
 GPIO_InitStruct.Mode=GPIO_MODE_AF_PP;
 GPIO_InitStruct.Pull=GPIO_NOPULL;
 GPIO_InitStruct.Speed=GPIO_SPEED_FREQ_LOW;
 GPIO_InitStruct.Alternate=GPIO_AF5_SPI2;
 HAL_GPIO_Init(PDM_OUT_GPIO_Port,&GPIO_InitStruct);
 /*ConfigureGPIOpin:B1_Pin*/
 GPIO_InitStruct.Pin=B1_Pin;
 GPIO_InitStruct.Mode=GPIO_MODE_EVT_RISING;
 GPIO_InitStruct.Pull=GPIO_NOPULL;
 HAL_GPIO_Init(B1_GPIO_Port,&GPIO_InitStruct);
 /*ConfigureGPIOpins:RES_PinDC_Pin*/
 GPIO_InitStruct.Pin=RES_Pin|DC_Pin;
 GPIO_InitStruct.Mode=GPIO_MODE_OUTPUT_PP;
 GPIO_InitStruct.Pull=GPIO_NOPULL;
 GPIO_InitStruct.Speed=GPIO_SPEED_FREQ_VERY_HIGH;
 HAL_GPIO_Init(GPIOA,&GPIO_InitStruct);
 /*ConfigureGPIOpin: I2S3_WS_Pin*/
 GPIO_InitStruct.Pin=I2S3_WS_Pin;
 GPIO_InitStruct.Mode=GPIO_MODE_AF_PP;
 GPIO_InitStruct.Pull=GPIO_NOPULL;
 GPIO_InitStruct.Speed=GPIO_SPEED_FREQ_LOW;
 GPIO_InitStruct.Alternate=GPIO_AF6_SPI3;
 HAL_GPIO_Init(I2S3_WS_GPIO_Port,&GPIO_InitStruct);
 /*ConfigureGPIOpin:CS_Pin*/
 GPIO_InitStruct.Pin=CS_Pin;
 GPIO_InitStruct.Mode=GPIO_MODE_OUTPUT_PP;
 GPIO_InitStruct.Pull=GPIO_NOPULL;
 GPIO_InitStruct.Speed=GPIO_SPEED_FREQ_VERY_HIGH;
 HAL_GPIO_Init(CS_GPIO_Port,&GPIO_InitStruct);
/*ConfigureGPIOpin:BLK_Pin*/
 GPIO_InitStruct.Pin=BLK_Pin;
 GPIO_InitStruct.Mode=GPIO_MODE_OUTPUT_PP;
 GPIO_InitStruct.Pull=GPIO_NOPULL;
 GPIO_InitStruct.Speed=GPIO_SPEED_FREQ_VERY_HIGH;
 HAL_GPIO_Init(BLK_GPIO_Port,&GPIO_InitStruct);
 /*ConfigureGPIOpin:BOOT1_Pin*/
 GPIO_InitStruct.Pin=BOOT1_Pin;
 GPIO_InitStruct.Mode=GPIO_MODE_INPUT;
 GPIO_InitStruct.Pull=GPIO_NOPULL;
 HAL_GPIO_Init(BOOT1_GPIO_Port,&GPIO_InitStruct);
 /*ConfigureGPIOpin:CLK_IN_Pin*/
 GPIO_InitStruct.Pin=CLK_IN_Pin;
 GPIO_InitStruct.Mode=GPIO_MODE_AF_PP;
 GPIO_InitStruct.Pull=GPIO_NOPULL;
 GPIO_InitStruct.Speed=GPIO_SPEED_FREQ_LOW;
 GPIO_InitStruct.Alternate=GPIO_AF5_SPI2;
 HAL_GPIO_Init(CLK_IN_GPIO_Port,&GPIO_InitStruct);
 /*ConfigureGPIOpins:LD4_PinLD3_PinLD5_PinLD6_Pin
 Audio_RST_Pin*/
 GPIO_InitStruct.Pin=LD4_Pin|LD3_Pin|LD5_Pin|LD6_Pin
 |Audio_RST_Pin;
 GPIO_InitStruct.Mode=GPIO_MODE_OUTPUT_PP;
 GPIO_InitStruct.Pull=GPIO_NOPULL;
 GPIO_InitStruct.Speed=GPIO_SPEED_FREQ_LOW;
 HAL_GPIO_Init(GPIOD,&GPIO_InitStruct);
 /*ConfigureGPIOpins: I2S3_MCK_PinI2S3_SCK_PinI2S3_SD_Pin*/
 GPIO_InitStruct.Pin=I2S3_MCK_Pin|I2S3_SCK_Pin|I2S3_SD_Pin;
 GPIO_InitStruct.Mode=GPIO_MODE_AF_PP;
 GPIO_InitStruct.Pull=GPIO_NOPULL;
 GPIO_InitStruct.Speed=GPIO_SPEED_FREQ_LOW;
 GPIO_InitStruct.Alternate=GPIO_AF6_SPI3;
 HAL_GPIO_Init(GPIOC,&GPIO_InitStruct);
 /*ConfigureGPIOpin:VBUS_FS_Pin*/
 GPIO_InitStruct.Pin=VBUS_FS_Pin;
 GPIO_InitStruct.Mode=GPIO_MODE_INPUT;
 GPIO_InitStruct.Pull=GPIO_NOPULL;
 HAL_GPIO_Init(VBUS_FS_GPIO_Port,&GPIO_InitStruct);
 /*ConfigureGPIOpins:OTG_FS_ID_PinOTG_FS_DM_PinOTG_FS_DP_Pin*/
 GPIO_InitStruct.Pin=OTG_FS_ID_Pin|OTG_FS_DM_Pin|OTG_FS_DP_Pin;
 GPIO_InitStruct.Mode=GPIO_MODE_AF_PP;
 GPIO_InitStruct.Pull=GPIO_NOPULL;
 GPIO_InitStruct.Speed=GPIO_SPEED_FREQ_LOW;
 GPIO_InitStruct.Alternate=GPIO_AF10_OTG_FS;
 HAL_GPIO_Init(GPIOA,&GPIO_InitStruct);
 /*ConfigureGPIOpin:OTG_FS_OverCurrent_Pin*/
 GPIO_InitStruct.Pin=OTG_FS_OverCurrent_Pin;
 GPIO_InitStruct.Mode=GPIO_MODE_INPUT;
 GPIO_InitStruct.Pull=GPIO_NOPULL;
 HAL_GPIO_Init(OTG_FS_OverCurrent_GPIO_Port,&GPIO_InitStruct);
 /*ConfigureGPIOpin:MEMS_INT2_Pin*/
GPIO_InitStruct.Pin=MEMS_INT2_Pin;
 GPIO_InitStruct.Mode=GPIO_MODE_EVT_RISING;
 GPIO_InitStruct.Pull=GPIO_NOPULL;
 HAL_GPIO_Init(MEMS_INT2_GPIO_Port,&GPIO_InitStruct);
 /*USERCODEBEGINMX_GPIO_Init_2*/
 /*USERCODEENDMX_GPIO_Init_2*/
 }
 /*USERCODEBEGIN4*/
 /*USERCODEEND4*/
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
