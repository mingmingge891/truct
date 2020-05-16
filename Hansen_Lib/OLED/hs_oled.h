#ifndef HS_OLED_H
#define HS_OLED_H

#ifdef __cplusplus
extern "C"
{
#endif
 
/**************************************************************************************************
 *                                         CONSTANTS
 **************************************************************************************************/

/* These are used to specify which line the text will be printed */
#define HAL_LCD_LINE_1      0x01
#define HAL_LCD_LINE_2      0x02
/*
   This to support LCD with extended number of lines (more than 2).
   Don't use these if LCD doesn't support more than 2 lines
*/
#define HAL_LCD_LINE_3      0x03
#define HAL_LCD_LINE_4      0x04
#define HAL_LCD_LINE_5      0x05
#define HAL_LCD_LINE_6      0x06
#define HAL_LCD_LINE_7      0x07
#define HAL_LCD_LINE_8      0x08
 
/**************************************************************************************************
 *                                          MACROS
 **************************************************************************************************/


/**************************************************************************************************
 *                                         TYPEDEFS
 **************************************************************************************************/


/**************************************************************************************************
 *                                     GLOBAL VARIABLES
 **************************************************************************************************/
extern const unsigned char F6x8[92][6];

/**************************************************************************************************
 *                                     FUNCTIONS - API
 **************************************************************************************************/

/*
 * Initialize LCD Service
 */
extern void lcd_hw_init(void);

/*
 * Write a string to the LCD
 */
extern void lcd_write_string( char *str, uint8_t option);

/*
 * Write a value to the LCD
 */
extern void HalLcdWriteValue ( uint32_t value, const uint8_t radix, uint8_t option);

/*
 * Write a value to the LCD
 */
extern void HalLcdWriteScreen( char *line1, char *line2 );

/*
 * Write a string followed by a value to the LCD
 */
extern void HalLcdWriteStringValue( char *title, uint16_t value, uint8_t format, uint8_t line );

/*
 * Write a string followed by 2 values to the LCD
 */
extern void HalLcdWriteStringValueValue( char *title, uint16_t value1, uint8_t format1, uint16_t value2, uint8_t format2, uint8_t line );

/*
 * Write a percentage bar to the LCD
 */
extern void HalLcdDisplayPercentBar( char *title, uint8_t value );
/*
 * Write a
 */
extern void HalLcdWritelrValue(uint32_t value1,uint8_t option);
/**

*/
void HalLCDFillLine(uint8_t start_option,uint8_t end_option,uint8_t value);

void LCD_Fill(unsigned char bmp_dat);

void lcd_printf(char *Data,...);

extern void LCD_SetPoint(uint8_t x,uint8_t y,uint8_t data1);

extern void oled_fill(unsigned char bmp_dat);

extern void OLED_Refresh_Gram(void);

extern void OLED_Clear(void);

extern void OLED_Fill(u8 x1,u8 y1,u8 x2,u8 y2,u8 dot);

extern uint32_t OLED_RedaPoinr(uint8_t x,uint8_t y);

extern void OLED_Refresh_Gram(void);

/**************************************************************************************************
**************************************************************************************************/

#ifdef __cplusplus
}
#endif

#endif
