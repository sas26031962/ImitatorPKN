/*
 Класс описания поведения светодиодов (дисплея)
 */
#ifndef C_DISPLAY
#define C_DISPLAY
 //========================================================================
 //---------------------------- ПОДКЛЮЧАЕМЫЕ ФАЙЛЫ ------------------------
 //========================================================================
#include "cLED.h"

 //========================================================================
 //-------------------------------- КОНСТАНТЫ -----------------------------
 //========================================================================
 

 
 //========================================================================
 //--------------------------- ОБЪЯВЛЕНИЕ КЛАССОВ -------------------------
 //========================================================================
 
class cDisplay{
 
 //------------------------------------------------------------------------
 // Атрибуты
 //------------------------------------------------------------------------
 public:
 static cLed Led1;  //
 static cLed Led2;  //
 static cLed Led3;  //
 static cLed Led4;  //
 static cLed Led5;  //
 static cLed Led6;  //
 static cLed Led7;  //
 static cLed Led8;  //
 static cLed Led9;  //
 static cLed Led10;  //

 static cLed Led11;  //
 static cLed Led12;  //
 static cLed Led13;  //
 static cLed Led14;  //
 static cLed Led15;  //
 static cLed Led16;  //
 static cLed Led17;  //
 static cLed Led18;  //
 static cLed Led19;  //
 static cLed Led20;  //

 static cLed Led21;  //
 static cLed Led22;  //
 static cLed Led23;  //
 static cLed Led24;  //
 static cLed Led25;  //
 static cLed Led26;  //
 static cLed Led27;  //
 static cLed Led28;  //
 static cLed Led29;  //
 static cLed Led30;  //

 static cLed Led31;  //
 static cLed Led32;  //
 static cLed Led33;  //
 static cLed Led34;  //
 static cLed Led35;  //
 static cLed Led36;  //
 static cLed Led37;  //
 static cLed Led38;  //
 static cLed Led39;  //
 static cLed Led40;  //

 static cLed Led41;  //
 static cLed Led42;  //
 static cLed Led43;  //
 static cLed Led44;  //
 static cLed Led45;  //
 static cLed Led46;  //
 static cLed Led47;  //

 //------------------------------------------------------------------------
 // Методы
 //------------------------------------------------------------------------
 protected:

 public:
 static void install();
 static void cDisplay::parseKeyCodeField( char c, bool state );

 void exec(); 
  
 //------------------------------------------------------------------------
 // Конструкторы
 //------------------------------------------------------------------------
 cDisplay();
 
};//End of class cDisplay
 //========================================================================
 //--------------------------- РЕАЛИЗАЦИЯ МЕТОДОВ -------------------------
 //========================================================================

//
// Конструктор
//
cDisplay::cDisplay(
    )
{
  //...
} 
//------------------------------------------------------------------
void cDisplay::install()
{
  Led1.install('A', LED1);
  Led2.install('B', LED2);
  Led3.install('C', LED3);
  Led4.install('D', LED4);
  Led5.install('E', LED5);
  Led6.install('F', LED6);
  Led7.install('G', LED7);
  Led8.install('H', LED8);
  Led9.install('I', LED9);
  Led10.install('J', LED10);

  Led11.install('K', LED11);
  Led12.install('L', LED2);
  Led13.install('M', LED3);
  Led14.install('N', LED4);
  Led15.install('O', LED5);
  Led16.install('P', LED6);
  Led17.install('Q', LED7);
  Led18.install('R', LED8);
  Led19.install('S', LED9);
  Led20.install('T', LED10);

  Led21.install('U', LED11);
  Led22.install('V', LED2);
  Led23.install('W', LED3);
  Led24.install('X', LED4);
  Led25.install('Y', LED5);
  Led26.install('Z', LED6);
  Led27.install('a', LED7);
  Led28.install('b', LED8);
  Led29.install('c', LED9);
  Led30.install('d', LED10);

  Led31.install('e', LED11);
  Led32.install('f', LED2);
  Led33.install('g', LED3);
  Led34.install('h', LED4);
  Led35.install('i', LED5);
  Led36.install('j', LED6);
  Led37.install('k', LED7);
  Led38.install('l', LED8);
  Led39.install('m', LED9);
  Led40.install('n', LED10);

  Led41.install('o', LED11);
  Led42.install('p', LED2);
  Led43.install('q', LED3);
  Led44.install('r', LED4);
  Led45.install('s', LED5);
  Led46.install('t', LED6);
  Led47.install('u', LED7);

  // Задержка
  //delay(300);
  /*
  // Погасить светодиоды
  Led1.darkness();
  Led2.darkness();
  Led3.darkness();
  Led4.darkness();
  Led5.darkness();
  Led6.darkness();
  Led7.darkness();
  Led8.darkness();
  Led9.darkness();
  Led10.darkness();
  Led11.darkness();
  */
}//End of void cDisplay::install()
//------------------------------------------------------------------
void cDisplay::exec()
{

}//End of void cDisplay::exec()

//
// Функция разбора принятых данных с учётом сдвига на два бита вправо
//

void cDisplay::parseKeyCodeField( char c, bool state )
{
      int x = (int)c / 4;// & 0x3f;
      switch(x)
      {
        case 1: Led1.setState(state); break;
        case 2: Led2.setState(state); break;
        case 3: Led3.setState(state); break;
        case 4: Led4.setState(state); break;
        case 5: Led5.setState(state); break;
        case 6: Led6.setState(state); break;
        case 7: Led7.setState(state); break;
        case 8: Led8.setState(state); break;
        case 9: Led9.setState(state); break;
        case 10: Led10.setState(state); break;
        
        case 11: Led11.setState(state); break;
        case 12: Led12.setState(state); break;
        case 13: Led13.setState(state); break;
        case 14: Led14.setState(state); break;
        case 15: Led15.setState(state); break;
        case 16: Led16.setState(state); break;
        case 17: Led17.setState(state); break;
        case 18: Led18.setState(state); break;
        case 19: Led19.setState(state); break;
        case 20: Led20.setState(state); break;
        
        case 21: Led21.setState(state); break;
        case 22: Led22.setState(state); break;
        case 23: Led23.setState(state); break;
        case 24: Led24.setState(state); break;
        case 25: Led25.setState(state); break;
        case 26: Led26.setState(state); break;
        case 27: Led27.setState(state); break;
        case 28: Led28.setState(state); break;
        case 29: Led29.setState(state); break;
        case 30: Led30.setState(state); break;
        
        case 31: Led31.setState(state); break;
        case 32: Led32.setState(state); break;
        case 33: Led33.setState(state); break;
        case 34: Led34.setState(state); break;
        case 35: Led35.setState(state); break;
        case 36: Led36.setState(state); break;
        case 37: Led37.setState(state); break;
        case 38: Led38.setState(state); break;
        case 39: Led39.setState(state); break;
        case 40: Led30.setState(state); break;
        
        case 41: Led41.setState(state); break;
        case 42: Led42.setState(state); break;
        case 43: Led43.setState(state); break;
        case 44: Led44.setState(state); break;
        case 45: Led45.setState(state); break;
        case 46: Led46.setState(state); break;
        case 47: Led47.setState(state); break;
        
      }//End switch switch(x)

}//End of void parseKeyCodeField( char c, bool state )

//--------------------------------------------------------------------------------
//----------------------------------ОБЪЯВЛЕНИЕ СТАТИЧЕСКИХ ПЕРЕМЕННЫХ-------------
//--------------------------------------------------------------------------------

cLed cDisplay::Led1;  //
cLed cDisplay::Led2;  //
cLed cDisplay::Led3;  //
cLed cDisplay::Led4;  //
cLed cDisplay::Led5;  //
cLed cDisplay::Led6;  //
cLed cDisplay::Led7;  //
cLed cDisplay::Led8;  //
cLed cDisplay::Led9;  //

cLed cDisplay::Led10;  //
cLed cDisplay::Led11;  //
cLed cDisplay::Led12;  //
cLed cDisplay::Led13;  //
cLed cDisplay::Led14;  //
cLed cDisplay::Led15;  //
cLed cDisplay::Led16;  //
cLed cDisplay::Led17;  //
cLed cDisplay::Led18;  //
cLed cDisplay::Led19;  //
cLed cDisplay::Led20;  //

cLed cDisplay::Led21;  //
cLed cDisplay::Led22;  //
cLed cDisplay::Led23;  //
cLed cDisplay::Led24;  //
cLed cDisplay::Led25;  //
cLed cDisplay::Led26;  //
cLed cDisplay::Led27;  //
cLed cDisplay::Led28;  //
cLed cDisplay::Led29;  //
cLed cDisplay::Led30;  //

cLed cDisplay::Led31;  //
cLed cDisplay::Led32;  //
cLed cDisplay::Led33;  //
cLed cDisplay::Led34;  //
cLed cDisplay::Led35;  //
cLed cDisplay::Led36;  //
cLed cDisplay::Led37;  //
cLed cDisplay::Led38;  //
cLed cDisplay::Led39;  //
cLed cDisplay::Led40;  //

cLed cDisplay::Led41;  //
cLed cDisplay::Led42;  //
cLed cDisplay::Led43;  //
cLed cDisplay::Led44;  //
cLed cDisplay::Led45;  //
cLed cDisplay::Led46;  //
cLed cDisplay::Led47;  //

//--------------------------------------------------------------------------------
#endif
