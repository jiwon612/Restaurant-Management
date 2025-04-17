 #include"st7789.h"
 #ifdefUSE_DMA
 #include<string.h>
 uint16_tDMA_MIN_SIZE=16;
 /*Ifyou'reusingDMA, thenuneeda"framebuffer"tostoredatastobedisplayed.
 *IfyourMCUdon'thaveenoughRAM,pleaseavoidusingDMA(orset5to1).
 *AndifyourMCUhaveenoughRAM(evenlargerthanfull-framesize),
 *Thenyoucanspecifytheframebuffersizetothefull resolutionbelow.
 */
 #defineHOR_LEN 1 // Alsomindtheresolutionofyourscreen!
 uint8_tdisp_buf[ST7789_WIDTH*HOR_LEN];
#endif
 /**
 *@briefWritecommandtoST7789controller
 *@paramcmd->commandtowrite
 *@returnnone
 */
 staticvoidST7789_WriteCommand(uint8_tcmd)
 {
 ST7789_Select();
 ST7789_DC_Clr();
 HAL_SPI_Transmit(&ST7789_SPI_PORT,&cmd,sizeof(cmd),HAL_MAX_DELAY);
 ST7789_UnSelect();
 }
 /**
 *@briefWritedatatoST7789controller
 *@parambuff->pointerofdatabuffer
 *@parambuff_size->sizeof thedatabuffer
 *@returnnone
 */
 staticvoidST7789_WriteData(uint8_t*buff,size_tbuff_size)
 {
 ST7789_Select();
 ST7789_DC_Set();
 //splitdatainsmallchunksbecauseHALcan'tsendmorethan64Katonce
 while(buff_size>0){
 uint16_tchunk_size=buff_size>65535?65535:buff_size;
 #ifdefUSE_DMA
 if(DMA_MIN_SIZE<=buff_size)
 {
 HAL_SPI_Transmit_DMA(&ST7789_SPI_PORT,buff,chunk_size);
 while(ST7789_SPI_PORT.hdmatx->State!=
 HAL_DMA_STATE_READY)
 {}
 }
 else
 HAL_SPI_Transmit(&ST7789_SPI_PORT,buff,chunk_size,
 HAL_MAX_DELAY);
 #else
 HAL_SPI_Transmit(&ST7789_SPI_PORT,buff,chunk_size,
 HAL_MAX_DELAY);
 #endif
 buff+=chunk_size;
 buff_size-=chunk_size;
 }
 ST7789_UnSelect();
 }
 /**
 *@briefWritedatatoST7789controller,simplifyfor8bitdata.
 *data->datatowrite
 *@returnnone
 */
 staticvoidST7789_WriteSmallData(uint8_tdata)
 {
 ST7789_Select();
 ST7789_DC_Set();
 HAL_SPI_Transmit(&ST7789_SPI_PORT,&data,sizeof(data),HAL_MAX_DELAY);
 ST7789_UnSelect();
 }
 /**
*@briefSet therotationdirectionof thedisplay
 *@paramm->rotationparameter(pleaserefer it inst7789.h)
 *@returnnone
 */
 voidST7789_SetRotation(uint8_tm)
 {
 ST7789_WriteCommand(ST7789_MADCTL); //MADCTL
 switch(m){
 case0:
 ST7789_WriteSmallData(ST7789_MADCTL_MX|ST7789_MADCTL_MY|
 ST7789_MADCTL_RGB);
 break;
 case1:
 ST7789_WriteSmallData(ST7789_MADCTL_MY|ST7789_MADCTL_MV|
 ST7789_MADCTL_RGB);
 break;
 case2:
 ST7789_WriteSmallData(ST7789_MADCTL_RGB);
 break;
 case3:
 ST7789_WriteSmallData(ST7789_MADCTL_MX|ST7789_MADCTL_MV|
 ST7789_MADCTL_RGB);
 break;
 default:
 break;
 }
 }
 /**
 *@briefSetaddressofDisplayWindow
 *@paramxi&yi->coordinatesofwindow
 *@returnnone
 */
 staticvoidST7789_SetAddressWindow(uint16_tx0,uint16_ty0,uint16_tx1,uint16_ty1)
 {
 ST7789_Select();
 uint16_tx_start=x0+X_SHIFT,x_end=x1+X_SHIFT;
 uint16_ty_start=y0+Y_SHIFT,y_end=y1+Y_SHIFT;
 /*ColumnAddressset*/
 ST7789_WriteCommand(ST7789_CASET);
 {
 uint8_tdata[]={x_start>>8,x_start&0xFF,x_end>>8,x_end&0xFF};
 ST7789_WriteData(data,sizeof(data));
 }
 /*RowAddressset*/
 ST7789_WriteCommand(ST7789_RASET);
 {
 uint8_tdata[]={y_start>>8,y_start&0xFF,y_end>>8,y_end&0xFF};
 ST7789_WriteData(data,sizeof(data));
 }
 /*WritetoRAM*/
 ST7789_WriteCommand(ST7789_RAMWR);
 ST7789_UnSelect();
 }
 /**
 *@brief InitializeST7789controller
 *@paramnone
 *@returnnone
 */
voidST7789_Init(void)
 {
 #ifdefUSE_DMA
 memset(disp_buf,0,sizeof(disp_buf));
 #endif
 HAL_Delay(10);
 ST7789_RST_Clr();
 HAL_Delay(10);
 ST7789_RST_Set();
 HAL_Delay(20);
 ST7789_WriteCommand(ST7789_COLMOD); // Setcolormode
 ST7789_WriteSmallData(ST7789_COLOR_MODE_16bit);
 ST7789_WriteCommand(0xB2); // Porchcontrol
 {
 uint8_tdata[]={0x0C,0x0C,0x00,0x33,0x33};
 ST7789_WriteData(data,sizeof(data));
 }
 ST7789_SetRotation(ST7789_ROTATION); // MADCTL(DisplayRotation)
 /*InternalLCDVoltagegeneratorsettings*/
 ST7789_WriteCommand(0XB7); // GateControl
 ST7789_WriteSmallData(0x35); // Defaultvalue
 ST7789_WriteCommand(0xBB); // VCOMsetting
 ST7789_WriteSmallData(0x19); // 0.725v(default0.75vfor0x20)
 ST7789_WriteCommand(0xC0); // LCMCTRL
 ST7789_WriteSmallData(0x2C); // Defaultvalue
 ST7789_WriteCommand(0xC2); // VDVandVRHcommand
 Enable
 ST7789_WriteSmallData(0x01); // Defaultvalue
 ST7789_WriteCommand(0xC3); // VRHset
 ST7789_WriteSmallData(0x12); // +-4.45v(defalut+-4.1vfor0x0B)
 ST7789_WriteCommand(0xC4); // VDVset
 ST7789_WriteSmallData(0x20); // Defaultvalue
 ST7789_WriteCommand(0xC6); // Frameratecontrol in
 normalmode
 ST7789_WriteSmallData(0x0F); // Defaultvalue(60HZ)
 ST7789_WriteCommand(0xD0); // Powercontrol
 ST7789_WriteSmallData(0xA4); // Defaultvalue
 ST7789_WriteSmallData(0xA1); // Defaultvalue
 /****************Divisionline****************/
 ST7789_WriteCommand(0xE0);
 {
 uint8_tdata[]={0xD0,0x04,0x0D,0x11,0x13,0x2B,0x3F,0x54,0x4C,0x18,
 0x0D,0x0B,0x1F,0x23};
 ST7789_WriteData(data,sizeof(data));
 }
 ST7789_WriteCommand(0xE1);
 {
 uint8_tdata[]={0xD0,0x04,0x0C,0x11,0x13,0x2C,0x3F,0x44,0x51,0x2F,
 0x1F,0x1F,0x20,0x23};
 ST7789_WriteData(data,sizeof(data));
 }
 ST7789_WriteCommand(ST7789_INVON); // InversionON
 ST7789_WriteCommand(ST7789_SLPOUT); // Outofsleepmode
 ST7789_WriteCommand(ST7789_NORON); // NormalDisplayon
 ST7789_WriteCommand(ST7789_DISPON); // Mainscreenturnedon
 HAL_Delay(50);
 ST7789_Fill_Color(BLACK); // FillwithBlack.
 }
/**
 *@briefFill theDisplayWindowwithsinglecolor
 *@paramcolor->colortoFillwith
 *@returnnone
 */
 voidST7789_Fill_Color(uint16_tcolor)
 {
 uint16_t i;
 ST7789_SetAddressWindow(0,0,ST7789_WIDTH-1,ST7789_HEIGHT-1);
 ST7789_Select();
 #ifdefUSE_DMA
 for(i=0; i<ST7789_HEIGHT/HOR_LEN; i++)
 {
 memset(disp_buf,color,sizeof(disp_buf));
 ST7789_WriteData(disp_buf,sizeof(disp_buf));
 }
 #else
 uint16_t j;
 for(i=0; i<ST7789_WIDTH; i++)
 for(j=0; j<ST7789_HEIGHT; j++){
 uint8_tdata[]={color>>8,color&0xFF};
 ST7789_WriteData(data,sizeof(data));
 }
 #endif
 ST7789_UnSelect();
 }
 /**
 *@briefDrawaPixel
 *@paramx&y->coordinatetoDraw
 *@paramcolor->colorof thePixel
 *@returnnone
 */
 voidST7789_DrawPixel(uint16_tx,uint16_ty,uint16_tcolor)
 {
 if((x<0)||(x>=ST7789_WIDTH)||
 (y<0)||(y>=ST7789_HEIGHT)) return;
 ST7789_SetAddressWindow(x,y,x,y);
 uint8_tdata[]={color>>8,color&0xFF};
 ST7789_Select();
 ST7789_WriteData(data,sizeof(data));
 ST7789_UnSelect();
 }
 /**
 *@briefFillanAreawithsinglecolor
 *@paramxSta&ySta->coordinateof thestartpoint
 *@paramxEnd&yEnd->coordinateof theendpoint
 *@paramcolor->colortoFillwith
 *@returnnone
 */
 voidST7789_Fill(uint16_txSta,uint16_tySta,uint16_txEnd,uint16_tyEnd,uint16_tcolor)
 {
 if((xEnd<0)||(xEnd>=ST7789_WIDTH)||
 (yEnd<0)||(yEnd>=ST7789_HEIGHT)) return;
 ST7789_Select();
 uint16_t i, j;
 ST7789_SetAddressWindow(xSta,ySta,xEnd,yEnd);
 for(i=ySta; i<=yEnd; i++)
 for(j=xSta; j<=xEnd; j++){
uint8_tdata[]={color>>8,color&0xFF};
 ST7789_WriteData(data,sizeof(data));
 }
 ST7789_UnSelect();
 }
 /**
 *@briefDrawabigPixelatapoint
 *@paramx&y->coordinateof thepoint
 *@paramcolor->colorof thePixel
 *@returnnone
 */
 voidST7789_DrawPixel_4px(uint16_tx,uint16_ty,uint16_tcolor)
 {
 if((x<=0)||(x>ST7789_WIDTH)||
 (y<=0)||(y>ST7789_HEIGHT)) return;
 ST7789_Select();
 ST7789_Fill(x-1,y-1,x+1,y+1,color);
 ST7789_UnSelect();
 }
 /**
 *@briefDrawalinewithsinglecolor
 *@paramx1&y1->coordinateof thestartpoint
 *@paramx2&y2->coordinateof theendpoint
 *@paramcolor->colorof thelinetoDraw
 *@returnnone
 */
 voidST7789_DrawLine(uint16_tx0,uint16_ty0,uint16_tx1,uint16_ty1,
 uint16_tcolor){
 uint16_tswap;
 uint16_tsteep=ABS(y1-y0)>ABS(x1-x0);
 if(steep){
 swap=x0;
 x0=y0;
 y0=swap;
 swap=x1;
 x1=y1;
 y1=swap;
 //_swap_int16_t(x0,y0);
 //_swap_int16_t(x1,y1);
 }
 if(x0>x1){
 swap=x0;
 x0=x1;
 x1=swap;
 swap=y0;
 y0=y1;
 y1=swap;
 //_swap_int16_t(x0,x1);
 //_swap_int16_t(y0,y1);
 }
 int16_tdx,dy;
 dx=x1-x0;
 dy=ABS(y1-y0);
 int16_terr=dx/2;
 int16_tystep;
 if(y0<y1){
 ystep=1;
 }else{
 ystep=-1;
}
 for(;x0<=x1;x0++){
 if(steep){
 ST7789_DrawPixel(y0,x0,color);
 }else{
 ST7789_DrawPixel(x0,y0,color);
 }
 err-=dy;
 if(err<0){
 y0+=ystep;
 err+=dx;
 }
 }
 }
 /**
 *@briefDrawaRectanglewithsinglecolor
 *@paramxi&yi->2coordinatesof2toppoints.
 *@paramcolor->colorof theRectangleline
 *@returnnone
 */
 voidST7789_DrawRectangle(uint16_tx1,uint16_ty1,uint16_tx2,uint16_ty2,uint16_tcolor)
 {
 ST7789_Select();
 ST7789_DrawLine(x1,y1,x2,y1,color);
 ST7789_DrawLine(x1,y1,x1,y2,color);
 ST7789_DrawLine(x1,y2,x2,y2,color);
 ST7789_DrawLine(x2,y1,x2,y2,color);
 ST7789_UnSelect();
 }
 /**
 *@briefDrawacirclewithsinglecolor
 *@paramx0&y0->coordinateofcirclecenter
 *@paramr->radiusofcircle
 *@paramcolor->colorofcircleline
 *@returnnone
 */
 voidST7789_DrawCircle(uint16_tx0,uint16_ty0,uint8_tr,uint16_tcolor)
 {
 int16_t f=1-r;
 int16_tddF_x=1;
 int16_tddF_y=-2*r;
 int16_tx=0;
 int16_ty=r;
 ST7789_Select();
 ST7789_DrawPixel(x0,y0+r,color);
 ST7789_DrawPixel(x0,y0-r,color);
 ST7789_DrawPixel(x0+r,y0,color);
 ST7789_DrawPixel(x0-r,y0,color);
 while(x<y){
 if(f>=0){
 y--;
 ddF_y+=2;
 f+=ddF_y;
 }
 x++;
 ddF_x+=2;
 f+=ddF_x;
 ST7789_DrawPixel(x0+x,y0+y,color);
 ST7789_DrawPixel(x0-x,y0+y,color);
ST7789_DrawPixel(x0+x,y0-y,color);
 ST7789_DrawPixel(x0-x,y0-y,color);
 ST7789_DrawPixel(x0+y,y0+x,color);
 ST7789_DrawPixel(x0-y,y0+x,color);
 ST7789_DrawPixel(x0+y,y0-x,color);
 ST7789_DrawPixel(x0-y,y0-x,color);
 }
 ST7789_UnSelect();
 }
 /**
 *@briefDrawanImageonthescreen
 *@paramx&y->startpointof theImage
 *@paramw&h->width&heightof theImagetoDraw
 *@paramdata->pointerof theImagearray
 *@returnnone
 */
 voidST7789_DrawImage(uint16_tx,uint16_ty,uint16_tw,uint16_th,constuint16_t*data)
 {
 if((x>=ST7789_WIDTH)||(y>=ST7789_HEIGHT))
 return;
 if((x+w-1)>=ST7789_WIDTH)
 return;
 if((y+h-1)>=ST7789_HEIGHT)
 return;
 ST7789_Select();
 ST7789_SetAddressWindow(x,y,x+w-1,y+h-1);
 ST7789_WriteData((uint8_t*)data,sizeof(uint16_t)*w*h);
 ST7789_UnSelect();
 }
 /**
 *@brief InvertFullscreencolor
 *@paraminvert->Whethertoinvert
 *@returnnone
 */
 voidST7789_InvertColors(uint8_t invert)
 {
 ST7789_Select();
 ST7789_WriteCommand(invert?0x21/*INVON*/ :0x20/*INVOFF*/);
 ST7789_UnSelect();
 }
 /**
 *@briefWriteachar
 *@paramx&y->cursorof thestartpoint.
 *@paramch->chartowrite
 *@paramfont->fontstyleof thestring
 *@paramcolor->colorof thechar
 *@parambgcolor->backgroundcolorof thechar
 *@returnnone
 */
 voidST7789_WriteChar(uint16_tx,uint16_ty,charch,FontDef font,uint16_tcolor,uint16_t
 bgcolor)
 {
 uint32_t i,b, j;
 ST7789_Select();
 ST7789_SetAddressWindow(x,y,x+font.width-1,y+font.height-1);
 for(i=0; i<font.height; i++){
 b=font.data[(ch-32)*font.height+i];
 for(j=0; j<font.width; j++){
 if((b<<j)&0x8000){
uint8_tdata[]={color>>8,color&0xFF};
 ST7789_WriteData(data,sizeof(data));
 }
 else{
 uint8_tdata[]={bgcolor>>8,bgcolor&0xFF};
 ST7789_WriteData(data,sizeof(data));
 }
 }
 }
 ST7789_UnSelect();
 }
 /**
 *@briefWriteastring
 *@paramx&y->cursorof thestartpoint.
 *@paramstr->stringtowrite
 *@paramfont->fontstyleof thestring
 *@paramcolor->colorof thestring
 *@parambgcolor->backgroundcolorof thestring
 *@returnnone
 */
 voidST7789_WriteString(uint16_tx,uint16_ty,constchar*str,FontDef font,uint16_tcolor,
 uint16_tbgcolor)
 {
 ST7789_Select();
 while(*str){
 if(x+font.width>=ST7789_WIDTH){
 x=0;
 y+=font.height;
 if(y+font.height>=ST7789_HEIGHT){
 break;
 }
 if(*str==' '){
 //skipspacesinthebeginningof thenewline
 str++;
 continue;
 }
 }
 ST7789_WriteChar(x,y,*str, font,color,bgcolor);
 x+=font.width;
 str++;
 }
 ST7789_UnSelect();
 }
 /**
 *@briefDrawafilledRectanglewithsinglecolor
 *@paramx&y->coordinatesof thestartingpoint
 *@paramw&h->width&heightof theRectangle
 *@paramcolor->colorof theRectangle
 *@returnnone
 */
 voidST7789_DrawFilledRectangle(uint16_tx,uint16_ty,uint16_tw,uint16_th,uint16_tcolor)
 {
 ST7789_Select();
 uint8_t i;
 /*Checkinputparameters*/
 if(x>=ST7789_WIDTH||
 y>=ST7789_HEIGHT){
 /*Returnerror*/
 return;
}
 /*Checkwidthandheight*/
 if((x+w)>=ST7789_WIDTH){
 w=ST7789_WIDTH-x;
 }
 if((y+h)>=ST7789_HEIGHT){
 h=ST7789_HEIGHT-y;
 }
 /*Drawlines*/
 for(i=0; i<=h; i++){
 /*Drawlines*/
 ST7789_DrawLine(x,y+i,x+w,y+i,color);
 }
 ST7789_UnSelect();
 }
 /**
 *@briefDrawaTrianglewithsinglecolor
 *@paramxi&yi->3coordinatesof3toppoints.
 *@paramcolor->colorof thelines
 *@returnnone
 */
 voidST7789_DrawTriangle(uint16_tx1,uint16_ty1,uint16_tx2,uint16_ty2,uint16_tx3,uint16_t
 y3,uint16_tcolor)
 {
 ST7789_Select();
 /*Drawlines*/
 ST7789_DrawLine(x1,y1,x2,y2,color);
 ST7789_DrawLine(x2,y2,x3,y3,color);
 ST7789_DrawLine(x3,y3,x1,y1,color);
 ST7789_UnSelect();
 }
 /**
 *@briefDrawafilledTrianglewithsinglecolor
 *@paramxi&yi->3coordinatesof3toppoints.
 *@paramcolor->colorof thetriangle
 *@returnnone
 */
 voidST7789_DrawFilledTriangle(uint16_tx1,uint16_ty1,uint16_tx2,uint16_ty2,uint16_tx3,
 uint16_ty3,uint16_tcolor)
 {
 ST7789_Select();
 int16_tdeltax=0,deltay=0,x=0,y=0,xinc1=0,xinc2=0,
 yinc1=0,yinc2=0,den=0,num=0,numadd=0,numpixels=0,
 curpixel=0;
 deltax=ABS(x2-x1);
 deltay=ABS(y2-y1);
 x=x1;
 y=y1;
 if(x2>=x1){
 xinc1=1;
 xinc2=1;
 }
 else{
 xinc1=-1;
 xinc2=-1;
 }
 if(y2>=y1){
 yinc1=1;
 yinc2=1;
}
 else{
 yinc1=-1;
 yinc2=-1;
 }
 if(deltax>=deltay){
 xinc1=0;
 yinc2=0;
 den=deltax;
 num=deltax/2;
 numadd=deltay;
 numpixels=deltax;
 }
 else{
 xinc2=0;
 yinc1=0;
 den=deltay;
 num=deltay/2;
 numadd=deltax;
 numpixels=deltay;
 }
 for(curpixel=0;curpixel<=numpixels;curpixel++){
 ST7789_DrawLine(x,y,x3,y3,color);
 num+=numadd;
 if(num>=den){
 num-=den;
 x+=xinc1;
 y+=yinc1;
 }
 x+=xinc2;
 y+=yinc2;
 }
 ST7789_UnSelect();
 }
 /**
 *@briefDrawaFilledcirclewithsinglecolor
 *@paramx0&y0->coordinateofcirclecenter
 *@paramr->radiusofcircle
 *@paramcolor->colorofcircle
 *@returnnone
 */
 voidST7789_DrawFilledCircle(int16_tx0, int16_ty0, int16_tr,uint16_tcolor)
 {
 ST7789_Select();
 int16_t f=1-r;
 int16_tddF_x=1;
 int16_tddF_y=-2*r;
 int16_tx=0;
 int16_ty=r;
 ST7789_DrawPixel(x0,y0+r,color);
 ST7789_DrawPixel(x0,y0-r,color);
 ST7789_DrawPixel(x0+r,y0,color);
 ST7789_DrawPixel(x0-r,y0,color);
 ST7789_DrawLine(x0-r,y0,x0+r,y0,color);
 while(x<y){
 if(f>=0){
 y--;
 ddF_y+=2;
 f+=ddF_y;
}
 x++;
 ddF_x+=2;
 f+=ddF_x;
 ST7789_DrawLine(x0-x,y0+y,x0+x,y0+y,color);
 ST7789_DrawLine(x0+x,y0-y,x0-x,y0-y,color);
 ST7789_DrawLine(x0+y,y0+x,x0-y,y0+x,color);
 ST7789_DrawLine(x0+y,y0-x,x0-y,y0-x,color);
 }
 ST7789_UnSelect();
 }
 /**
 *@briefOpen/Closetearingeffect line
 *@paramtear->Whethertotear
 *@returnnone
 */
 voidST7789_TearEffect(uint8_t tear)
 {
 ST7789_Select();
 ST7789_WriteCommand(tear?0x35/*TEON*/ :0x34/*TEOFF*/);
 ST7789_UnSelect();
 }
 /**
 *@briefASimpletest functionforST7789
 *@paramnone
 *@returnnone
 */
 voidST7789_Test(void)
 {
 ST7789_Fill_Color(WHITE);
 HAL_Delay(1000);
 ST7789_WriteString(10,20,"SpeedTest",Font_11x18,RED,WHITE);
 HAL_Delay(1000);
 ST7789_Fill_Color(CYAN);
 HAL_Delay(500);
 ST7789_Fill_Color(RED);
 HAL_Delay(500);
 ST7789_Fill_Color(BLUE);
 HAL_Delay(500);
 ST7789_Fill_Color(GREEN);
 HAL_Delay(500);
 ST7789_Fill_Color(YELLOW);
 HAL_Delay(500);
 ST7789_Fill_Color(BROWN);
 HAL_Delay(500);
 ST7789_Fill_Color(DARKBLUE);
 HAL_Delay(500);
 ST7789_Fill_Color(MAGENTA);
 HAL_Delay(500);
 ST7789_Fill_Color(LIGHTGREEN);
 HAL_Delay(500);
 ST7789_Fill_Color(LGRAY);
 HAL_Delay(500);
 ST7789_Fill_Color(LBBLUE);
 HAL_Delay(500);
 ST7789_Fill_Color(WHITE);
 HAL_Delay(500);
 ST7789_WriteString(10,10,"Font test.",Font_16x26,GBLUE,WHITE);
 ST7789_WriteString(10,50,"HelloSteve!",Font_7x10,RED,WHITE);
ST7789_WriteString(10,75,"HelloSteve!",Font_11x18,YELLOW,WHITE);
 ST7789_WriteString(10,100,"HelloSteve!",Font_16x26,MAGENTA,WHITE);
 HAL_Delay(1000);
 ST7789_Fill_Color(RED);
 ST7789_WriteString(10,10,"Rect./Line.",Font_11x18,YELLOW,BLACK);
 ST7789_DrawRectangle(30,30,100,100,WHITE);
 HAL_Delay(1000);
 ST7789_Fill_Color(RED);
 ST7789_WriteString(10,10,"FilledRect.",Font_11x18,YELLOW,BLACK);
 ST7789_DrawFilledRectangle(30,30,50,50,WHITE);
 HAL_Delay(1000);
 ST7789_Fill_Color(RED);
 ST7789_WriteString(10,10,"Circle.",Font_11x18,YELLOW,BLACK);
 ST7789_DrawCircle(60,60,25,WHITE);
 HAL_Delay(1000);
 ST7789_Fill_Color(RED);
 ST7789_WriteString(10,10,"FilledCir.",Font_11x18,YELLOW,BLACK);
 ST7789_DrawFilledCircle(60,60,25,WHITE);
 HAL_Delay(1000);
 ST7789_Fill_Color(RED);
 ST7789_WriteString(10,10,"Triangle",Font_11x18,YELLOW,BLACK);
 ST7789_DrawTriangle(30,30,30,70,60,40,WHITE);
 HAL_Delay(1000);
 ST7789_Fill_Color(RED);
 ST7789_WriteString(10,10,"FilledTri",Font_11x18,YELLOW,BLACK);
 ST7789_DrawFilledTriangle(30,30,30,70,60,40,WHITE);
 HAL_Delay(1000);
 // IfFLASHcannotstorageanymoredatas,pleasedeletecodesbelow.
 ST7789_Fill_Color(WHITE);
 ST7789_DrawImage(0,40,128,128,(uint16_t*)saber);
 HAL_Delay(3000);
