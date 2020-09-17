; Print.s
; Student names: Zachary Chin and Alex Liang
; Last modification date: 4/15/2020
; Runs on LM4F120 or TM4C123
; EE319K lab 7 device driver for any LCD
;
; As part of Lab 7, students need to implement these LCD_OutDec and LCD_OutFix
; This driver assumes two low-level LCD functions
; ST7735_OutChar   outputs a single 8-bit ASCII character
; ST7735_OutString outputs a null-terminated string 
	
    IMPORT   ST7735_OutChar
    IMPORT   ST7735_OutString
    EXPORT   LCD_OutDec
    EXPORT   LCD_OutFix

    AREA    |.text|, CODE, READONLY, ALIGN=2
    THUMB
		

;-----------------------LCD_OutDec-----------------------
; Output a 32-bit number in unsigned decimal format
; Input: R0 (call by value) 32-bit unsigned number
; Output: none
; Invariables: This function must not permanently modify registers R4 to R11
; Lab 7 requirement is for at least one local variable on the stack with symbolic binding
numToAscii	EQU 0x30		;Any number plus 0x30 = their ascii equivalent
;Binding for local variable numDec
numDec 	EQU 0				;numDec holds the input number to be used to print
LCD_OutDec
	 PUSH{R4, LR}			;Save registers
	 SUB SP, #8				;Allocate space on the stack for numDec (and an extra 4 bytes for alignment)
	 STR R0, [SP,#numDec]	;Store the input in numDec
	 CMP R0, #10			;BASE CASE if <10 then branch to the base case
	 BLO baseCase
	 MOV R4, #10			;R4 holds the value #10
	 UDIV R0, R4			;Divide the num by 10
	 BL  LCD_OutDec			;Recursive call, causing the numbers to be printed starting from the MS digit
	 LDR R0, [SP,#numDec]	;After the recursive call finishes, grab the number again from numDec
	 BL  mod10				;Isolate the LS digit
baseCase
	 LDR R4, =numToAscii	;Grab the value to convert to ascii
	 ADD R0, R4				;Add the digit to print and convert it to ascii
	 BL  ST7735_OutChar		;Print the digit
	 ADD SP, #8				;Deallocate the local variable
	 POP{R4,LR}				;Restore the registers
     BX  LR					;Exit the subroutine
;* * * * * * * * End of LCD_OutDec * * * * * * * *


; -----------------------mod10----------------------
; Divides the input by 10 and returns the remainder
; Inputs:  R0 is an unsigned 32-bit number
; Outputs: R0 holds the remainder
mod10
	 PUSH{R4,R5}		;Save registers used
	 MOV R5, #10		;R5 holds the value #10
	 UDIV R4, R0, R5	;Divide the input by 10 and store in R4
	 MUL R4, R5			;Multiply the quotient by 10
	 SUB R0, R0, R4		;Subtract the original num by the product and store it in R0
	 POP{R4,R5}			;Restore registers
	 BX LR				;Return
	 
; -----------------------LCD _OutFix----------------------
; Output characters to LCD display in fixed-point format
; unsigned decimal, resolution 0.01, range 0.00 to 9.99
; Inputs:  R0 is an unsigned 32-bit number
; Outputs: none
; E.g., R0=0,    then output "0.00 "
;       R0=3,    then output "0.003 "
;       R0=89,   then output "0.89 "
;       R0=123,  then output "1.23 "
;       R0=999,  then output "9.99 "
;       R0>999,  then output "*.** "
; Invariables: This function must not permanently modify registers R4 to R11
; Lab 7 requirement is for at least one local variable on the stack with symbolic binding

maximum 	EQU 999		;The maximum value before the input is invalid
asterisk	EQU 0x2A	;The ascii code for "*"
period		EQU 0x2E	;The ascii code for "."
delta		EQU 100		;The fixed number delta value
;Binding for local variables numFix and count
numFix 		EQU 0		;numFix holds the digits left to be printed
count		EQU	4		;count keeps track of the current digit being printed
LCD_OutFix
	 PUSH{R4-R6,LR}			;Save registers
	 SUB SP, #8				;Allocation for local variables
	 ;Check for out of range
	 LDR R4, =maximum		;R4 holds 999
	 CMP R0, R4				;Compare R0 and R4
	 BHI outOfRange			;Branch if parameter > 999
	 STR R0, [SP,#numFix]	;Store the number in numFix
	 MOV R4, #1				;R4 holds the value #1
	 STR R4, [SP,#count]	;count is initialized to 1
outFixLoop
	 LDR R4, [SP,#count]	;Load the value of count
	 CMP R4, #2				;Check if count is 2
	 BEQ isPeriod			;If =2, branch to print a "."
	 LDR R5, [SP,#numFix]	;Load the remaining digits to be printed
	 LDR R0, =delta			;R0 holds the value #100
	 UDIV R0, R5, R0		;Isolate the hundredths digit to be printed
	 LDR R6, =numToAscii	;R6 holds the value to convert a number to ascii
	 ADD R0, R6				;Convert the digit to ascii
	 BL ST7735_OutChar		;Print the digit in R0
	 MOV R0, R5				;R0 holds the previous number before division
	 BL mod100				;Isolate the tenths and ones digits
	 MOV R5, #10			;R5 holds the value #10
	 MUL R0, R5				;Multiply the isolated digits by 10 to print out next
	 STR R0, [SP,#numFix]	;Store the new number in numFix
	 ADD R4, #1				;Increment count by 1
	 STR R4, [SP,#count]	;Store the new value in count
	 CMP R4, #5				;Compare it to 5
	 BEQ exit				;If 5, the number has been printed, exit the loop
	 B outFixLoop			;Else loop back to outFixLoop
isPeriod					;Print out a decimal point (period)
	 LDR R0, =period		;R0 holds the ascii value for "."
	 BL  ST7735_OutChar		;Print it
	 ADD R4, #1				;Increment count
	 STR R4, [SP,#count]	;Store the new value in count
	 B outFixLoop			;Enter back into the loop
outOfRange					;If invalid, print "*.**"
	 LDR R4, =asterisk		;R4 holds the ascii value for "*"
	 MOV R0, R4				;Put "*" in R0 to print
	 BL  ST7735_OutChar		;Print
	 LDR R0, =period		;R0 holds the value for period
	 BL  ST7735_OutChar		;Print
	 MOV R0, R4				;R0 holds "*"
	 BL  ST7735_OutChar		;Print
	 MOV R0, R4				;R0 holds "*"
	 BL  ST7735_OutChar		;Print
exit
	 ADD SP, #8				;Deallocate the local variables
	 POP{R4-R6,LR}			;Restore registers
     BX   LR				;Return
 
     ALIGN
;* * * * * * * * End of LCD_OutFix * * * * * * * *

; -----------------------mod100----------------------
; Divides the input by 100 and returns the remainder
; Inputs:  R0 is an unsigned 32-bit number
; Outputs: R0 holds the remainder
mod100
	 PUSH{R4,R5}		;Save registers
	 MOV R5, #100		;R5 holds the value #100
	 UDIV R4, R0, R5	;Divide the input num by 100 and store in R4
	 MUL R4, R5			;Multiply the quotient by 100
	 SUB R0, R0, R4		;Subtract the product from the input num and store in R0
	 POP{R4,R5}			;Restore registers
	 BX LR				;Return

     ALIGN          ; make sure the end of this section is aligned
     END            ; end of file
