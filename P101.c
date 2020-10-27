/*
PRACTICA 10 - “COMUNICACION SERIAL BIDIRECCIONAL”
EQUIPO V 
8H1 - ELECTRONICA DIGITAL V
DAVID MADRID NAPOLES 17100201
CARLOS CAMACHO MATA 17100057
HECTOR SAMUEL RIVERA RUIZ 17100287
03/11/2020
*/

#INCLUDE <16F887.H>
#USE DELAY(CLOCK=4000000)
#USE RS232(BAUD=9600,XMIT=PIN_C6,RCV=PIN_C7,BITS=8)
#FUSES INTRC_IO,NOWDT,NOPUT,MCLR,NOPROTECT,NOCPD,NOBROWNOUT
#FUSES NOIESO,NOFCMEN,NOLVP,NODEBUG,NOWRT,BORV21

#USE FAST_IO(A)
#USE FAST_IO(B)
#USE FAST_IO(C)
#USE FAST_IO(D)

#INCLUDE <LCD1.C>
INT Z = 0; //ETAPAS
INT I = 0, J = 0, AUX = 0, CUENTA = 0, L = 0;//VARIABLES PARA CUENTAS
INT R = 0; //RECIBIR
CHAR E; //ENVIAR
//VALOR REAL DE LA CUENTA
//INT RV [16] = {1, 2, 3, 100, 4, 5, 6, 100, 7, 8, 9, 100, 200, 0, 101, 100};
//VALOR DE CARACTERES
CHAR CV [16] = {'1', '2', '3', 'A', '4', '5', '6', 'B', '7', '8', '9', 'C', '*', '0', '#', 'D'};

VOID INICIO (){ //MUESTRA LOS MENSAJES DE INICIO
   LCD_GOTOXY(1,1);
   PRINTF(LCD_PUTC, "      CETI      \n     COLOMOS    ");         
   DELAY_MS(500);
   
   LCD_GOTOXY(1,1);
   PRINTF(LCD_PUTC, "TGO. EN CONTROL \nAUTOMATICO E INS");         
   DELAY_MS(500);
   
   LCD_GOTOXY(1,1);
   PRINTF(LCD_PUTC, "   ELECTRONICA  \n    DIGITAL V   ");         
   DELAY_MS(500);
   
   FOR(J = 0; J <= 2; J++){ // EN CADA ITERACION MUESTRA EL R Y N DE UN ALM
      SWITCH (J){
         CASE 0:
            LCD_GOTOXY(1,1);
            PRINTF(LCD_PUTC, "DAVID MADRID     \n17100201            ");
            BREAK;
               
         CASE 1:
            LCD_GOTOXY(1,1);
            PRINTF(LCD_PUTC, "HECTOR RUIZ     \n17100287          ");
            BREAK;
                     
         CASE 2:
            LCD_GOTOXY(1,1);
            PRINTF(LCD_PUTC, "CAMACHO MATA     \n17100057         ");
            BREAK;
            }
      DELAY_MS(200);
   }
   
   LCD_GOTOXY(1,1);
   PRINTF(LCD_PUTC, "PRACT 10 COM SER\nIAL BIDIRECCIONAL");         
   DELAY_MS(500);
}
VOID TECLADO (){ //SE ENCARGA DE DETECTAR PULSACIONES EN EL TECLADO
   AUX = 0XEF;
   CUENTA = 0; //LLEVARA LA CUENTA, DETECTANDO LA CUENTA PRESIONADO
   FOR(I=0;I<4;I++){//EN CADA ITERACION REVISA UNA FILA DIFERENTE
         
      AUX=AUX>>1; //DESPLAZA UN CERO A LA DERECHA
      OUTPUT_A((INPUT_A()&0XF0)|(AUX&0X0F));
         
      IF(INPUT(PIN_A7)==0) //REVISDA ESTADO COLUMNA A7
      BREAK;
      CUENTA++;
      IF(INPUT(PIN_A6)==0) //REVISDA ESTADO COLUMNA A6
      BREAK;
      CUENTA++;
      IF(INPUT(PIN_A5)==0) //REVISDA ESTADO COLUMNA A5
      BREAK;
      CUENTA++;
      IF(INPUT(PIN_A4)==0) //REVISDA ESTADO COLUMNA A4
      BREAK;
      CUENTA++;
         
      DELAY_MS(20); //TIEMPO DE ESPERA NECESARIO PARA DETECTAR CAMBIOS
   }
}

#INT_RDA
VOID RDA_isr( ){ //se recibe un dato de 8 bits por el pin RC7 vía comunicación serial.  
   R=GETC();  //SE GUARDA EL DATO EN R
   LCD_GOTOXY(1,1);
   PRINTF(LCD_PUTC, "RECIBIENDO VALOR\n       = %u      "R);
   DELAY_MS(2000);
   Z = 1; //PARA QUE INGRESE A ETAPA 1
}

VOID MAIN (){
   SET_TRIS_A(0XF0); //DECLARAR SALIDAS
   OUTPUT_A(INPUT_A()&0XF0); //LIMPIAR BITS DE SALIDA
   LCD_INIT(); //INICIAR LCD
   
   ENABLE_INTERRUPTS(GLOBAL); //Sirve para habilitar todas las interrupciones 
   ENABLE_INTERRUPTS(INT_RDA); //habilita interrupción por comunicación serial
   
   INICIO ();
   
   LCD_GOTOXY(1,1);
   PRINTF(LCD_PUTC, "PRESIONA ALGUNA \n     TECLA      ");
   
   WHILE(TRUE){
      
      WHILE(Z == 1){ //ETAPA 1
         LCD_GOTOXY(1,1);
         PRINTF(LCD_PUTC, "                \n                ");
         Z = 0; //SALE DE ETAPA 1
      }
      
      TECLADO (); //LEE EL TECLADO
      IF(CUENTA < 16){ //SI SE PRESIONA ALGUNA TECLA
         L = 1; //INDICA QUE YA SE EJECUTO ESTE PEDAZO DE CODIGO
         
         E = CV[CUENTA];
         LCD_GOTOXY(1,1);
         PRINTF(LCD_PUTC, "ENVIANDO VALOR =\n        %c       ", E);
         PUTC(E);
         DELAY_MS(2000);
      }
      IF(L!=0){ //SI YA SE EJECUTO (L) MUESTRA ESTE MENSAJE
         LCD_GOTOXY(1,1);
         PRINTF(LCD_PUTC, "  ULTIMA TECLA  \n PRESIONADA = %c ", E);
      }
   }
}
