This is the STM32 source codes for the radar speed detector. The input signal will be filtered using a bandpass filter before sending into the STM32.
There are two methods that can be used to determine the frequency of the input signal and the corresponding speed, which are the FFT and the comparator methods.
The freqeuncy and the speed computed will then display on the LCD display and also the two seven-segment LED displays.
To display on the seven-segment LED display, the computed speed is converted into BCD and sent to the Xilinx CPLD.
The control circuit constructed using the Xilinx ISE can decode the BCD to control the seven-segment LED display.
![photo_2025-04-20_00-43-36](https://github.com/user-attachments/assets/46848da5-c938-4986-85d4-0ca7a6e87054)
