#include <16f877.h>

#use delay(clock=4M)

#fuses xt
#fuses nobrownout
#fuses nowdt
#fuses noprotect
#fuses put

#byte trista = 0x85
#byte tristb = 0x86
#byte tristc = 0x87
#byte portb = 0x06
#byte CCPR1L = 0x15
#byte CCPR1H = 0x16
#byte CCP1CON = 0x17
#byte CCPR2L = 0x1B
#byte CCPR2H = 0x1C
#byte CCP2CON = 0x1D
#byte ADCON0 = 0x1F
#byte ADCON1 = 0x9F
#byte ADRESH = 0x1E
#byte ADRESL = 0x9E
#byte T1CON = 0x10
#byte OPTION_REG = 0x181
#byte INTCON = 0x0B
#byte T2CON = 0x12
#byte PR2 = 0x92
#byte PIR1 = 0x0C