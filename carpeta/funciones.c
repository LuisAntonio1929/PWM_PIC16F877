#include "source.h"
#include "funciones.h"

#define LCD_ENABLE_PIN  PIN_E0                                    ////
#define LCD_RS_PIN      PIN_A5                                    ////
#define LCD_RW_PIN      PIN_A2                                    ////
#define LCD_DATA4       PIN_D0                                    ////
#define LCD_DATA5       PIN_D1                                    ////
#define LCD_DATA6       PIN_D2                                    ////
#define LCD_DATA7       PIN_D3 

#include <lcd.c>

extern int modo;

void configuracion(){
   //Configuracion de pines
   lcd_init();
   delay_ms(10);
   trista |= 0b00000001;	//RA0 como entrada
   tristb |= 0b00000001;	//RB0 como entrada
   tristb &= 0b11110011;	//RB2 y RB3 como salidas
   tristc &= 0b11111011;	//RC2 como salida
   output_low(PIN_B2);
   output_low(PIN_B3);
   //Configuracion ADC
   ADCON0 = 0b01000001;		//Frecuencia de conversion Fosc/8, RA0 como canal analógico y activacion del módulo ADC
   ADCON1 = 0b00001110;		//Justificado a la izquierda y Ansel
   //Configuracion del CCP2 con prescaler de 1:8 y reloj de 4MHz para lanzar conversion ADC cada 100ms
   CCPR2L = 12500 & 0xFF;
   CCPR2H = 12500>>8;
   CCP2CON = 0b00001011;	//CCP2 con modo comparador con evento ADC
   T1CON = 0b00110001;		//Prescaler 1:8, TMR1 activado
   //Configuracion del CCP1 para prescaler de 1:1, reloj de 4MHz y modo PWM
   PR2 = 9;			//Frecuencia PWM de 100KHz
   CCP1CON = 0b00001111;	//Modo PWM
   T2CON = 0b00000100;		//Prescaler 1:1, postcaler 1:1, y TMR2 activado
   //Configuracion de las interrupciones
   OPTION_REG = 0b00000000;	//Desactivacion de las resistencias pull-ups, e interrupcion externa por flanco de bajada
   INTCON = 0b10010000;		//Activacion de las interrupciones globales y por interrupcion externa RB0
}

int dc, dc100, v, ccbits;
const int mask1 = 0b11001111, mask2 = 0b00000011;

void mensaje(){
   v = (ADRESH>>3)+(ADRESH>>4)+(ADRESH>>7);	//Operacion equivalente a ADRES*50/255
   if(modo==0){
      output_high(PIN_B3);
      lcd_gotoxy(1,1);
      lcd_putc("Modo Lineal  ");
      dc = (ADRESH>>3)+(ADRESH>>5);		//Operacion equivalente a ADRES*40/255
      dc100 = (ADRESH>>2)+(ADRESH>>3)+(ADRESH>>6);	//Operacion equivalente a ADRES*100/255
      ccbits = dc & mask2;
      ccbits = ccbits<<4;
      CCP1CON = (CCP1CON & mask1)|ccbits;
      CCPR1L = dc>>2;
   }else{
      output_low(PIN_B3);
      lcd_gotoxy(1,1);
      lcd_putc("Modo Centrado");
      int diferencia;
      if(ADRESH>=128){
	 output_high(PIN_B2);
	 diferencia = ADRESH-128;
      }else{
	 output_low(PIN_B2);
	 diferencia = 128-ADRESH;
      }
      dc = (diferencia>>2)+(diferencia>>4);			//Operacion equivalente a diferencia*40/128
      dc100 = (diferencia>>1)+(diferencia>>2)+(diferencia>>5);	//Operacion equivalente a diferencia*100/128
      ccbits = dc & mask2;
      ccbits = ccbits<<4;
      CCP1CON = (CCP1CON & mask1)|ccbits;
      CCPR1L = dc>>2;
   }
   lcd_gotoxy(1,2);
   lcd_putc("V: ");
   lcd_putc(v/10+'0');
   lcd_putc('.');
   lcd_putc(v%10+'0');
   printf(lcd_putc, "v D: %03d%%", dc100);
}

#INT_EXT
void interrupcion(){
   delay_ms(20);			//Filtro anti-rebotes
   if(!bit_test(PORTB,0)){
      modo++;
      modo%=2;
   }
}