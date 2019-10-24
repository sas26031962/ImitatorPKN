/*
 Класс описания поведения клавиатуры
 */
#ifndef C_KEYBOARD
#define C_KEYBOARD
 //========================================================================
 //---------------------------- ПОДКЛЮЧАЕМЫЕ ФАЙЛЫ ------------------------
 //========================================================================
#include "cKeyX.h"

 //========================================================================
 //-------------------------------- КОНСТАНТЫ -----------------------------
 //========================================================================
#define KEYS_ARRAY_LENGTH 40
 
 //========================================================================
 //--------------------------- ОБЪЯВЛЕНИЕ КЛАССОВ -------------------------
 //========================================================================
 
class cKeyboard{
 
 //------------------------------------------------------------------------
 // Атрибуты
 //------------------------------------------------------------------------
 public:
 
 static cKeyX Key[KEYS_ARRAY_LENGTH]; 

 //------------------------------------------------------------------------
 // Методы
 //------------------------------------------------------------------------
 protected:
 static void execKey(int n);

 public:
 static void install();

 void exec(); 
  
 //------------------------------------------------------------------------
 // Конструкторы
 //------------------------------------------------------------------------
 cKeyboard();
 
};//End of class cKeyboard

 //========================================================================
 //--------------------------- РЕАЛИЗАЦИЯ МЕТОДОВ -------------------------
 //========================================================================

//
// Конструктор
//
cKeyboard::cKeyboard(
    )
{
  //...
} 

void cKeyboard::install()
{
  /*
   * Инициализация выводов не проводится, так как 
   * используется управление кнопками по последовательному порту
   */
  for(int i = 0; i < KEYS_ARRAY_LENGTH; i++)
  {
    Key[i].install(i); 
  }
}//End of void cKeyboard::install()
//------------------------------------------------------------------
void cKeyboard::execKey(int n)
{
  if(cKeyboard::Key[n].checkEvent())
  {
    //--- Отправка сообщения об изменении состояния клавиши ---
    // Дескриптор исходящего пакета
    if(cKeyboard::Key[n].getValue() == 0)
       cEthernet::releaseBaseProtocol(102);
    else
       cEthernet::releaseBaseProtocol(103);
    // Код клавиши   
    cEthernet::AnswerBuffer[ 1 + BASE_HEADER_LENGTH ] = n;//Код клавиши                   
    // длина исходящего пакета  
    cEthernet::AnswerLength = 2 + BASE_HEADER_LENGTH;                      
    
    // Передача 
      cEthernet::sendAnswer();

  }
}//End of void cKeyboard::execKey(n)
//------------------------------------------------------------------

void cKeyboard::exec()
{
  for(int i = 0; i < KEYS_ARRAY_LENGTH; i++)
  {
    cKeyboard::Key[i].operate();
    cKeyboard::execKey(i);
  }

}//End of void cKeyboard::exec()


//--------------------------------------------------------------------------------
//----------------------------------ОБЪЯВЛЕНИЕ СТАТИЧЕСКИХ ПЕРЕМЕННЫХ-------------
//--------------------------------------------------------------------------------
cKeyX cKeyboard::Key[KEYS_ARRAY_LENGTH]; 


//--------------------------------------------------------------------------------
#endif