 #ifndef__ST7789_H
 #define__ST7789_H
 #include"fonts.h"
 #include"main.h"
 /*chooseaHardwareSPIport touse.*/
 #defineST7789_SPI_PORThspi1
 externSPI_HandleTypeDefST7789_SPI_PORT;
 /*choosewhetheruseDMAornot*/
 //#defineUSE_DMA
 /*IfuneedCScontrol,commentbelow*/
 //#defineCFG_NO_CS
 /*Pinconnection*/
 #defineST7789_RST_PORTGPIOA
 #defineST7789_RST_PINGPIO_PIN_1
 #defineST7789_DC_PORTGPIOA
 #defineST7789_DC_PIN GPIO_PIN_2
 #ifndefCFG_NO_CS
 #defineST7789_CS_PORTGPIOC
 #defineST7789_CS_PIN GPIO_PIN_5
 #endif
 /*IfuneedBacklightcontrol,uncommentbelow*/
 //#defineBLK_PORT
 #defineST7789_BLK_PORTGPIOB
 //#defineBLK_PIN
#defineST7789_BLK_PINGPIO_PIN_0
 /*
 *Commentonetouseanother.
 *3parameterscanbechoosed
 *135x240(0.96inch)&240x240(1.3inch)&170x320(1.9inch)
 *X_SHIFT&Y_SHIFTareusedtoadaptdifferentdisplay'sresolution
 */
 /*Chooseatypeyouareusing*/
 //#defineUSING_135X240
 #defineUSING_240X300
 //#defineUSING_170X320
 /*Chooseadisplayrotationyouwant touse:(0-3)*/
 //#defineST7789_ROTATION0
 //#defineST7789_ROTATION1
 #defineST7789_ROTATION2 // useNormallyon240x240
 //#defineST7789_ROTATION3
 #ifdefUSING_135X240
 #ifST7789_ROTATION==0
 #defineST7789_WIDTH135
 #defineST7789_HEIGHT240
 #defineX_SHIFT53
 #defineY_SHIFT40
 #endif
 #ifST7789_ROTATION==1
 #defineST7789_WIDTH240
 #defineST7789_HEIGHT135
 #defineX_SHIFT40
 #defineY_SHIFT52
 #endif
 #ifST7789_ROTATION==2
 #defineST7789_WIDTH135
 #defineST7789_HEIGHT240
 #defineX_SHIFT52
 #defineY_SHIFT40
 #endif
 #ifST7789_ROTATION==3
 #defineST7789_WIDTH240
 #defineST7789_HEIGHT135
 #defineX_SHIFT40
 #defineY_SHIFT53
 #endif
 #endif
 #ifdefUSING_240X300
 #defineST7789_WIDTH240
 #defineST7789_HEIGHT300
 #ifST7789_ROTATION==0
 #defineX_SHIFT0
 #defineY_SHIFT80
 #elifST7789_ROTATION==1
 #defineX_SHIFT80
 #defineY_SHIFT0
 #elifST7789_ROTATION==2
 #defineX_SHIFT0
 #defineY_SHIFT0
 #elifST7789_ROTATION==3
 #defineX_SHIFT0
 #defineY_SHIFT0
 #endif
 #endif
#ifdefUSING_170X320
 #ifST7789_ROTATION==0
 #defineST7789_WIDTH170
 #defineST7789_HEIGHT320
 #defineX_SHIFT35
 #defineY_SHIFT0
 #endif
 #ifST7789_ROTATION==1
 #defineST7789_WIDTH320
 #defineST7789_HEIGHT170
 #defineX_SHIFT0
 #defineY_SHIFT35
 #endif
 #ifST7789_ROTATION==2
 #defineST7789_WIDTH170
 #defineST7789_HEIGHT320
 #defineX_SHIFT35
 #defineY_SHIFT0
 #endif
 #ifST7789_ROTATION==3
 #defineST7789_WIDTH320
 #defineST7789_HEIGHT170
 #defineX_SHIFT0
 #defineY_SHIFT35
 #endif
 #endif
 /**
 *Colorofpen
 *Ifyouwant touseanothercolor,youcanchooseoneinRGB565format.
 */
 #defineWHITE 0xFFFF
 #defineBLACK 0x0000
 #defineBLUE 0x001F
 #defineRED 0xF800
 #defineMAGENTA 0xF81F
 #defineGREEN 0x07E0
 #defineCYAN 0x7FFF
 #defineYELLOW 0xFFE0
 #defineGRAY 0X8430
 #defineBRED 0XF81F
 #defineGRED 0XFFE0
 #defineGBLUE 0X07FF
 #defineBROWN 0XBC40
 #defineBRRED 0XFC07
 #defineDARKBLUE 0X01CF
 #defineLIGHTBLUE 0X7D7C
 #defineGRAYBLUE 0X5458
 #defineLIGHTGREEN0X841F
 #defineLGRAY 0XC618
 #defineLGRAYBLUE 0XA651
 #defineLBBLUE 0X2B12
 /*ControlRegistersandconstantcodes*/
 #defineST7789_NOP 0x00
 #defineST7789_SWRESET0x01
 #defineST7789_RDDID 0x04
 #defineST7789_RDDST 0x09
 #defineST7789_SLPIN 0x10
 #defineST7789_SLPOUT0x11
 #defineST7789_PTLON 0x12
#defineST7789_NORON 0x13
 #defineST7789_INVOFF0x20
 #defineST7789_INVON 0x21
 #defineST7789_DISPOFF0x28
 #defineST7789_DISPON0x29
 #defineST7789_CASET 0x2A
 #defineST7789_RASET 0x2B
 #defineST7789_RAMWR 0x2C
 #defineST7789_RAMRD 0x2E
 #defineST7789_PTLAR 0x30
 #defineST7789_COLMOD0x3A
 #defineST7789_MADCTL0x36
 /**
 *MemoryDataAccessControlRegister(0x36H)
 *MAP: D7D6D5D4D3D2D1D0
 *param: MYMXMVMLRGBMH--
 *
 */
 /*PageAddressOrder('0':ToptoBottom, '1': theopposite)*/
 #defineST7789_MADCTL_MY0x80
 /*ColumnAddressOrder('0':Left toRight, '1': theopposite)*/
 #defineST7789_MADCTL_MX0x40
 /*Page/ColumnOrder('0'=NormalMode, '1'=ReverseMode)*/
 #defineST7789_MADCTL_MV0x20
 /*LineAddressOrder('0'=LCDRefreshToptoBottom, '1'=theopposite)*/
 #defineST7789_MADCTL_ML0x10
 /*RGB/BGROrder('0'=RGB, '1'=BGR)*/
 #defineST7789_MADCTL_RGB0x00
 #defineST7789_RDID1 0xDA
 #defineST7789_RDID2 0xDB
 #defineST7789_RDID3 0xDC
 #defineST7789_RDID4 0xDD
 /*Advancedoptions*/
 #defineST7789_COLOR_MODE_16bit0x55 // RGB565(16bit)
 //#defineST7789_COLOR_MODE_18bit0x66 // RGB666(18bit)
 /*Basicoperations*/
 #defineST7789_RST_Clr()HAL_GPIO_WritePin(ST7789_RST_PORT,ST7789_RST_PIN,
 GPIO_PIN_RESET)
 #defineST7789_RST_Set()HAL_GPIO_WritePin(ST7789_RST_PORT,ST7789_RST_PIN,
 GPIO_PIN_SET)
 #defineST7789_DC_Clr()HAL_GPIO_WritePin(ST7789_DC_PORT,ST7789_DC_PIN,
 GPIO_PIN_RESET)
 #defineST7789_DC_Set()HAL_GPIO_WritePin(ST7789_DC_PORT,ST7789_DC_PIN,
 GPIO_PIN_SET)
 #ifndefCFG_NO_CS
 #defineST7789_Select()HAL_GPIO_WritePin(ST7789_CS_PORT,ST7789_CS_PIN,
 GPIO_PIN_RESET)
 #defineST7789_UnSelect()HAL_GPIO_WritePin(ST7789_CS_PORT,ST7789_CS_PIN,
 GPIO_PIN_SET)
 #else
 #defineST7789_Select()asm("nop")
 #defineST7789_UnSelect()asm("nop")
 #endif
 #defineABS(x)((x)>0?(x):-(x))
 /*Basicfunctions.*/
 voidST7789_Init(void);
 voidST7789_SetRotation(uint8_tm);
 voidST7789_Fill_Color(uint16_tcolor);
 voidST7789_DrawPixel(uint16_tx,uint16_ty,uint16_tcolor);
voidST7789_Fill(uint16_txSta,uint16_tySta,uint16_txEnd,uint16_tyEnd,uint16_tcolor);
 voidST7789_DrawPixel_4px(uint16_tx,uint16_ty,uint16_tcolor);
 /*Graphical functions.*/
 voidST7789_DrawLine(uint16_tx1,uint16_ty1,uint16_tx2,uint16_ty2,uint16_tcolor);
 voidST7789_DrawRectangle(uint16_tx1,uint16_ty1,uint16_tx2,uint16_ty2,uint16_tcolor);
 voidST7789_DrawCircle(uint16_tx0,uint16_ty0,uint8_tr,uint16_tcolor);
 voidST7789_DrawImage(uint16_tx,uint16_ty,uint16_tw,uint16_th,constuint16_t*data);
 voidST7789_InvertColors(uint8_t invert);
 /*Text functions.*/
 voidST7789_WriteChar(uint16_tx,uint16_ty,charch,FontDef font,uint16_tcolor,uint16_t
 bgcolor);
 voidST7789_WriteString(uint16_tx,uint16_ty,constchar*str,FontDef font,uint16_tcolor,
 uint16_tbgcolor);
 /*ExtentedGraphical functions.*/
 voidST7789_DrawFilledRectangle(uint16_tx,uint16_ty,uint16_tw,uint16_th,uint16_tcolor);
 voidST7789_DrawTriangle(uint16_tx1,uint16_ty1,uint16_tx2,uint16_ty2,uint16_tx3,uint16_t
 y3,uint16_tcolor);
 voidST7789_DrawFilledTriangle(uint16_tx1,uint16_ty1,uint16_tx2,uint16_ty2,uint16_tx3,
 uint16_ty3,uint16_tcolor);
 voidST7789_DrawFilledCircle(int16_tx0, int16_ty0, int16_tr,uint16_tcolor);
 /*Commandfunctions*/
 voidST7789_TearEffect(uint8_t tear);
 /*Simpletest function.*/
 voidST7789_Test(void);
 #ifndefST7789_ROTATION
 #errorYoushouldat leastchooseadisplayrotation!
 #endif
 #endif
