# Real time clock display on LCD
Displaying real time clock and date on an LCD

## Used hardware componenets
- STM32F429ZI microcontroller
- LCD 16x2
- DS1307 RTC Module
- 10k ohm potentiometer

## Workflow
- The STM32 microcontroller communicates with the DS1307 via I2C in order to get the date and time information, then sends these information out via GPIO to the LCD to display it.
- Systick timer triggers an interrupt every 1 seconds, and it's handler is responsible for printing the date and time information on the LCD every 1 second.

## Notes
- Add the proper delay after executing LCD commands according to your microcontroller clock if my delays didn't work for you, here, HSI is used  which is 16MHz.
- Make sure you supply the ds1307 with typically 4.5~5.5V "from both sides" of the module and ground it from both sides also.
- If your LCD doesn't show the characters after running the code, try to modify the contrast of the screen with a potentiometer by connecting it to the VO pin on the LCD.

![IMG_7330](https://user-images.githubusercontent.com/29959479/194268192-6186ec24-fac6-4361-9618-d3c170031326.jpg)
