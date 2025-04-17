#include "test.h"
 #include "OLED_1in3.h"
 int OLED_1in3_test(void)
 {
 printf("1.3inch OLED test demo\n");
 if(System_Init() != 0) {
 return-1;
 }
 printf("OLED Init...\r\n");
 OLED_1IN3_Init();
 Driver_Delay_ms(500);
 // 0.Create a new image cache
 UBYTE *BlackImage;
 UWORD Imagesize = ((OLED_1IN3_WIDTH%8==0)? (OLED_1IN3_WIDTH/8):
 (OLED_1IN3_WIDTH/8+1)) * OLED_1IN3_HEIGHT;
 if((BlackImage = (UBYTE *)malloc(Imagesize)) == NULL) {
 printf("Failed to apply for black memory...\r\n");
 return-1;
 }
 printf("Paint_NewImage\r\n");
 Paint_NewImage(BlackImage, OLED_1IN3_WIDTH, OLED_1IN3_HEIGHT, 90,
 BLACK);
 printf("Drawing\r\n");
 //1.Select Image
 Paint_SelectImage(BlackImage);
 Driver_Delay_ms(500);
 Paint_Clear(BLACK);
 while(1) {
 // mosoon
 printf("Drawing:page 1\r\n");
 OLED_DisplayWithText(gImage_1in3);
 Driver_Delay_ms(10000);
 Paint_Clear(BLACK);
 // menu
 printf("Drawing:page 2\r\n");
 OLED_DisplayWithText(gImage_menu);
 Driver_Delay_ms(10000);
 Paint_Clear(BLACK);
 }
 }
