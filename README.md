# RTC-Display-on-LCD
Displaying real time clock and date on an LCD

## Used hardware componenets
- STM32F429ZI microcontroller
- LCD 16x2
- DS1307 RTC Module
- 10k ohm potentiometer

## Workflow
The STM32 microcontroller communicates with the DS1307 via I2C in order to get the date and time information, then sends these information out via GPIO to the LCD to display it.

## Notes on hardware interfacing
- Make sure you supply the ds1307 with typically 4.5~5.5V "from both sides" of the module and ground it from both sides also.
- If your LCD doesn't show the characters after running the code, try to modify the contrast of the screen with a potentiometer by connecting it to the VO pin on the LCD.
