#include "source.h"
#include "funciones.h"

int modo = 0;

int main(){
   configuracion();
   mensaje();
   while(1){
      if(bit_test(PIR1,6)){	//Si hubo una conversión ADC
	 bit_clear(PIR1,6);	//Se limpia flag de conversion ADC
	 mensaje();
      }
   }
   return 0;
}
