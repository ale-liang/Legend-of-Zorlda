; LCD.s
; Student names: Zachary Chin and Alex Liang
; Last modification date: 4/15/2020

; Runs on LM4F120/TM4C123
; Use SSI0 to send an 8-bit code to the ST7735 160x128 pixel LCD.

; As part of Lab 7, students need to implement these LCD_WriteCommand and LCD_WriteData
; This driver assumes two low-level LCD functions

; Backlight (pin 10) connected to +3.3 V
; MISO (pin 9) unconnected
; SCK (pin 8) connected to PA2 (SSI0Clk)
; MOSI (pin 7) connected to PA5 (SSI0Tx)
; TFT_CS (pin 6) connected to PA3 (SSI0Fss)
; CARD_CS (pin 5) unconnected
; Data/Command (pin 4) connected to PA6 (GPIO)
; RESET (pin 3) connected to PA7 (GPIO)
; VCC (pin 2) connected to +3.3 V
; Gnd (pin 1) connected to ground

DC                      EQU   0x40004100
DC_COMMAND              EQU   0
DC_DATA                 EQU   0x40
SSI0_DR_R               EQU   0x40008008
SSI0_SR_R               EQU   0x4000800C
SSI_SR_RNE              EQU   0x00000004  ; SSI Receive FIFO Not Empty
SSI_SR_BSY              EQU   0x00000010  ; SSI Busy Bit
SSI_SR_TNF              EQU   0x00000002  ; SSI Transmit FIFO Not Full

      EXPORT   writecommand
      EXPORT   writedata

      AREA    |.text|, CODE, READONLY, ALIGN=2
      THUMB
      ALIGN

; The Data/Command pin must be valid when the eighth bit is
; sent.  The SSI module has hardware input and output FIFOs
; that are 8 locations deep.  Based on the observation that
; the LCD interface tends to send a few commands and then a
; lot of data, the FIFOs are not used when writing
; commands, and they are used when writing data.  This
; ensures that the Data/Command pin status matches the byte
; that is actually being transmitted.
; The write command operation waits until all data has been
; sent, configures the Data/Command pin for commands, sends
; the command, and then waits for the transmission to
; finish.
; The write data operation waits until there is room in the
; transmit FIFO, configures the Data/Command pin for data,
; and then adds the data to the transmit FIFO.
; NOTE: These functions will crash or stall indefinitely if
; the SSI0 module is not initialized and enabled.

; This is a helper function that sends an 8-bit command to the LCD.
; Input: R0  8-bit command to transmit
; Output: none
; Assumes: SSI0 and port A have already been initialized and enabled
writecommand
;; --UUU-- Code to write a command to the LCD
;1) Read SSI0_SR_R and check bit 4, 
;2) If bit 4 is high, loop back to step 1 (wait for BUSY bit to be low)
;3) Clear D/C=PA6 to zero
;4) Write the command to SSI0_DR_R
;5) Read SSI0_SR_R and check bit 4, 
;6) If bit 4 is high, loop back to step 5 (wait for BUSY bit to be low)
	PUSH{R4-R7}			;Save registers 4-7
	LDR R4, =SSI0_SR_R	;R4 holds the address of the status register
wcCheck1				
	LDR R5, [R4]		;grab the contents of the status register
	AND R5, #0x10		;isolate bit 4 
	CMP R5, #0			;compare it to 0
	BNE wcCheck1		;Loop back to wcCheck1 if it is high
	LDR R6, =DC			;R6 holds the address of DC (PortA Data Reg)
	LDR R7, [R6]		;R7 holds the contents of DC (PortA Data Reg)
	BIC R7, #0x40		;Clear DC=PA6 to zero
	STR R7, [R6]		;Store it back into DC
	LDR R6, =SSI0_DR_R	;R6 holds the address of the data register
	STRB R0, [R6]		;Store the 8-bit command in the data register
wcCheck2
	LDR R5, [R4]		;grab the contents of the status register
	AND R5, #0x10		;isolate bit 4
	CMP R5, #0			;compare it to 0
	BNE wcCheck2		;Loop back to wcCheck2 if it is high
    POP{R4-R7}			;Restore contents of registers 4-7
    BX  LR                          ;   return

; This is a helper function that sends an 8-bit data to the LCD.
; Input: R0  8-bit data to transmit
; Output: none
; Assumes: SSI0 and port A have already been initialized and enabled
writedata
;; --UUU-- Code to write data to the LCD
;1) Read SSI0_SR_R and check bit 1, 
;2) If bit 1 is low loop back to step 1 (wait for TNF bit to be high)
;3) Set D/C=PA6 to one
;4) Write the 8-bit data to SSI0_DR_R
	PUSH{R4-R7}			;Save registers 4-7
	LDR R4, =SSI0_SR_R	;R4 holds the address of the status register
wdCheck
	LDR R5, [R4]		;grab the contents of the status register
	AND R5, #0x2		;isolate bit 1 
	CMP R5, #0			;compare it to 0
	BEQ wdCheck			;Loop back to wdCheck if it is low
    LDR R6, =DC			;R6 holds the address of DC (PortA Data Reg)
	LDR R7, [R6]		;R7 holds the contents of DC (PortA Data Reg)
	ORR R7, #0x40		;Set DC=PA6 to 1
	STR R7, [R6]		;Store it back into DC
	LDR R6, =SSI0_DR_R	;R6 holds the address of the data register
	STRB R0, [R6]		;Store the 8-bit command in the data register
    POP{R4-R7}			;Restore contents of registers 4-7
    BX  LR                          ;   return


;***************************************************
; This is a library for the Adafruit 1.8" SPI display.
; This library works with the Adafruit 1.8" TFT Breakout w/SD card
; ----> http://www.adafruit.com/products/358
; as well as Adafruit raw 1.8" TFT display
; ----> http://www.adafruit.com/products/618
;
; Check out the links above for our tutorials and wiring diagrams
; These displays use SPI to communicate, 4 or 5 pins are required to
; interface (RST is optional)
; Adafruit invests time and resources providing this open source code,
; please support Adafruit and open-source hardware by purchasing
; products from Adafruit!
;
; Written by Limor Fried/Ladyada for Adafruit Industries.
; MIT license, all text above must be included in any redistribution
;****************************************************

    ALIGN          ; make sure the end of this section is aligned
    END            ; end of file
