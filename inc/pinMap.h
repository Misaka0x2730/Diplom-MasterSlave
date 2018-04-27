#ifndef INC_PINMAP_H_
#define INC_PINMAP_H_

/*
 * Format:
 * High: port number
 * 0 - A
 * 1 - B
 * 2 - C
 * 3 - D
 * 4 - E
 * Low: pin number
 * 0  - 0
 * 1  - 1
 * 2  - 2
 * 3  - 3
 * 4  - 4
 * 5  - 5
 * 6  - 6
 * 7  - 7
 * 8  - 8
 * 9  - 9
 * 10 - A
 * 11 - B
 * 12 - C
 * 13 - D
 * 14 - E
 * 15 - F
*/

#define PIN_LED_1		0x0C
#define PIN_LED_2		0x0B
#define PIN_LED_3		0x29
#define PIN_LED_4		0x28
#define PIN_LED_5		0x27
#define PIN_LED_6		0x26
#define PIN_LED_7		0x3F
#define PIN_LED_8		0x3E
#define PIN_LED_9		0x3A
#define PIN_LED_10		0x39
#define PIN_LED_11		0x38
#define PIN_LED_12		0x1F
#define PIN_LED_13		0x1E
#define PIN_LED_14		0x1D
#define PIN_LED_15		0x1C
#define PIN_OC_1		0x4E
#define PIN_OC_2		0x4D
#define PIN_OC_3		0x4C
#define PIN_OC_4		0x4B
#define PIN_OC_5		0x4A
#define PIN_OC_6		0x49
#define PIN_OC_7		0x48
#define PIN_BUZZER		0x41
#define PIN_ANALOG_1	0x03
#define PIN_ANALOG_2	0x04
#define PIN_ANALOG_3	0x05
#define PIN_ANALOG_4	0x06
#define PIN_ANALOG_5	0x07

#define	PIN_RS485_TXD	0x1A
#define PIN_RS485_RXD	0x1B
#define PIN_RS485_MODE	0x4F

#define PIN_I2C_SDA		0x19
#define PIN_I2C_SCL		0x18

#define PIN_LCD_RS		0x17
#define PIN_LCD_CS		0x16
#define PIN_LCD_MOSI	0x15
#define PIN_LCD_MISO	0x14
#define PIN_LCD_SCK		0x13
#define PIN_LCD_RST		0x37
#define PIN_LCD_BL		0x36

#define PIN_BUTTON_ENTER	0x42
#define PIN_BUTTON_LEFT		0x46
#define PIN_BUTTON_RIGHT	0x45
#define PIN_BUTTON_UP		0x44
#define PIN_BUTTON_DOWN		0x43

#endif /* INC_PINMAP_H_ */
