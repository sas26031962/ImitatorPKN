/*
 Класс описания поведения пульта
 */
#ifndef C_PULT
#define C_PULT
 //========================================================================
 //---------------------------- ПОДКЛЮЧАЕМЫЕ ФАЙЛЫ ------------------------
 //========================================================================

 //========================================================================
 //-------------------------------- КОНСТАНТЫ -----------------------------
 //========================================================================
 //
 // Таблица дескрипторов входящих сообщений
 //
 #define DESCRIPTOR_INPUT_ON_LED 1            // Включить индикатор с заданным номером 
 #define DESCRIPTOR_INPUT_OFF_LED 2           // Выключить индикатор с заданным номером 
 #define DESCRIPTOR_INPUT_SET_PARAMETER 3     // Установить значение параметра пульта 
 #define DESCRIPTOR_INPUT_GET_PARAMETER 4     // Запросить значение параметра пульта
 #define DESCRIPTOR_INPUT_GET_SHORT_STATE 5   // Запросить состояние пульта
 #define DESCRIPTOR_INPUT_DIAGNOSTICS 6       // Запустить диагностику пульта
 #define DESCRIPTOR_INPUT_SET_RELAY_STATE 7   // Задать состояние реле "Выбор К/Н"
 #define DESCRIPTOR_INPUT_GET_RELAY_STATE 8   // Запросить состояние реле "Выбор К/Н"
 #define DESCRIPTOR_INPUT_GET_FULL_STATE 9    // Запросить расширенное состояние пульта
 #define DESCRIPTOR_INPUT_GET_VERSION 10      // Запросить версию программного обеспечения пульта

 //
 // Таблица дескрипторов исходящих сообщений
 //
 #define  DESCRIPTOR_OUTPUT_SHORT_STATE 101   // Короткое состояние пульта 
 #define  DESCRIPTOR_OUTPUT_WIDE_STATE 110    // Расширенное состояние пульта 
 #define  DESCRIPTOR_OUTPUT_LED_IS_ON 106     // Табло включено 
 #define  DESCRIPTOR_OUTPUT_LED_IS_OFF 107    // Табло выключено 
 #define  DESCRIPTOR_OUTPUT_PARAMETER 104     // Значение параметра 
 #define  DESCRIPTOR_OUTPUT_DIAGNOSTIC 105    // Результат диагностики 
 #define  DESCRIPTOR_OUTPUT_RELAY_STATE 109   // Состояние реле "Выбор К/Н"
 #define  DESCRIPTOR_OUTPUT_VERSION 111       // Версия программного обеспечения пульта


 //
 // Общие константы
 //

 //========================================================================
 //--------------------------- ОБЪЯВЛЕНИЕ КЛАССОВ -------------------------
 //========================================================================
 
class cPult{
 
 //------------------------------------------------------------------------
 // Атрибуты
 //------------------------------------------------------------------------
 public:
 //
 // Параметры пульта
 //
 static char ParameterDebounce;              // Параметр для подавления дребезга
 static char ErrorCode;                      // Значение параметра ошибки
 static char Version[6];                     // Версия
 static char Result[3];                      // Результат выполнения диагностики
 
 static cCommunication Com;                  //Коммуникационная составляющая

 // Звуковой сигнал 
 cBuzer Buzer;
 
 // Светодиоды
 cDisplay Dis;
 
 // Кнопки
 cKeyboard Kbd;
 
 // СУО

 cSUO Suo;

 // Реле "Командир/Наводчик"

 cRelay Relay;
  
 //------------------------------------------------------------------------
 // Методы
 //------------------------------------------------------------------------
 protected:

 public:
 void install();

 void exec(); 

 void view();

 //------------------------------------------------------------------------
 // Конструкторы
 //------------------------------------------------------------------------
 cPult();
 
};//End of class cPult
 //========================================================================
 //--------------------------- РЕАЛИЗАЦИЯ МЕТОДОВ -------------------------
 //========================================================================

//
// Конструктор
//
cPult::cPult(
    )
{
  //...
} 
//------------------------------------------------------------------
void cPult::install()
{

  //
  // Запуск последовательного порта для мониторинга
  //
  cPult::Com.install();

  //
  // Инициализация сети Ethernet
  //
  cEthernet::installEthernet();


  // Кнопки
  cKeyboard::install();

  // Светодиоды
  cDisplay::install();
  
  // Инициализация звукового сигнала
  Buzer.install('L', PIN_PIEZO);
  Buzer.beep();

}//End of void cPult::install()
//-------------------------------------------------------------------
void cPult::exec()
{
  //------------------------ Реализация функции СУО -----------------
  this->Suo.operate();
  //----- Передача сообщения о готовности пульта после включения ----
  cReady::exec();
  //------------- Процесс работы с последовательным портом ----------
  cPult::Com.operate();
  //-------------------------- Процесс чтения кнопок ----------------
  Kbd.exec();
  //-----------------------------------------------------------------
  // 
  // Разбор принятого пакета
  //
  cEthernet::packetSize = cEthernet::Udp.parsePacket();
  
  if (cEthernet::packetSize > 0) 
  {
    
    cEthernet::Udp.read(cEthernet::packetBuffer,32);

    view();
    //Serial.print("Sink descriptor: ");
    //Serial.println(cEthernet::packetBuffer[0]);
    //
    //Декодирование принятого сообщения
    //
    
    switch(cEthernet::packetBuffer[0])
    {
      
      //--- Передача версии программного обеспечения ---------------------------------
      case DESCRIPTOR_INPUT_GET_VERSION:
        cEthernet::releaseBaseProtocol(DESCRIPTOR_OUTPUT_VERSION);  // реализация базового протокола
        cEthernet::AnswerBuffer[1 + BASE_HEADER_LENGTH] = cPult::Version[0];   // Номер версии 1
        cEthernet::AnswerBuffer[2 + BASE_HEADER_LENGTH] = cPult::Version[1];   // Номер версии 1
        cEthernet::AnswerBuffer[3 + BASE_HEADER_LENGTH] = cPult::Version[2];   // Номер версии 1
        cEthernet::AnswerBuffer[4 + BASE_HEADER_LENGTH] = cPult::Version[3];   // Дата выпуска версии день
        cEthernet::AnswerBuffer[5 + BASE_HEADER_LENGTH] = cPult::Version[4];   // Дата выпуска версии месяц
        cEthernet::AnswerBuffer[6 + BASE_HEADER_LENGTH] = cPult::Version[5];   // Дата выпуска версии год
        
        cEthernet::AnswerLength = 7 + BASE_HEADER_LENGTH;           // длина исходящего пакета
    
        // Передача ответа: в качестве адреса и номера порта берутся данные 
        cEthernet::sendAnswer();

      break;
      
      //--------------------------------------------------------
      //--- Включить индикатор с заданным номером -------
      case DESCRIPTOR_INPUT_ON_LED:
        cDisplay::parseKeyCodeField( cEthernet::packetBuffer[1 + BASE_HEADER_LENGTH], HIGH);
      
        cEthernet::releaseBaseProtocol(DESCRIPTOR_OUTPUT_LED_IS_ON);    // Реализация базового протокола
        cEthernet::AnswerBuffer[1 + BASE_HEADER_LENGTH] = 
        cEthernet::packetBuffer[1+ BASE_HEADER_LENGTH];                      // Код элемента индикации
        cEthernet::AnswerLength = 2 + BASE_HEADER_LENGTH;           // длина исходящего пакета
    
        // Передача ответа: в качестве адреса и номера порта берутся данные 
        cEthernet::sendAnswer();
      break;
      //--------------------------------------------------------
      //--- Выключить индикатор с заданным номером ------
      case DESCRIPTOR_INPUT_OFF_LED:
        cDisplay::parseKeyCodeField( cEthernet::packetBuffer[1 + BASE_HEADER_LENGTH], LOW);
      
        cEthernet::releaseBaseProtocol(DESCRIPTOR_OUTPUT_LED_IS_OFF);   // Реализация базового протокола
        cEthernet::AnswerBuffer[1  + BASE_HEADER_LENGTH] = 
          cEthernet::packetBuffer[1+ BASE_HEADER_LENGTH];                      // Код элемента индикации
        cEthernet::AnswerLength = 2 + BASE_HEADER_LENGTH;           // длина исходящего пакета
    
        // Передача ответа: в качестве адреса и номера порта берутся данные 
        cEthernet::sendAnswer();
      break;
      //--------------------------------------------------------
      
      //--- Установить значение параметра пульта --------
      case DESCRIPTOR_INPUT_SET_PARAMETER:
        cPult::ParameterDebounce = cEthernet::packetBuffer[1 + BASE_HEADER_LENGTH]; //Установка значения параметра пульта
                                                                  //
        cEthernet::releaseBaseProtocol(DESCRIPTOR_OUTPUT_PARAMETER);   // реализация базового протокола
        cEthernet::AnswerBuffer[1 + BASE_HEADER_LENGTH] = cPult::ParameterDebounce;   // Значение параметра
        cEthernet::AnswerLength = 2  + BASE_HEADER_LENGTH;          // длина исходящего пакета
    
        // Передача ответа: в качестве адреса и номера порта берутся данные 
        cEthernet::sendAnswer();
      break;
      
      //--------------------------------------------------------
      
      //--- Запросить значение параметра пульта ---------
      case DESCRIPTOR_INPUT_GET_PARAMETER:
        cEthernet::releaseBaseProtocol(DESCRIPTOR_OUTPUT_PARAMETER);   // реализация базового протокола
        cEthernet::AnswerBuffer[1 + BASE_HEADER_LENGTH] = cPult::ParameterDebounce;   // Значение параметра
        cEthernet::AnswerLength = 2 + BASE_HEADER_LENGTH;           // длина исходящего пакета
    
        // Передача ответа: в качестве адреса и номера порта берутся данные 
        cEthernet::sendAnswer();
      break;
      
      //--------------------------------------------------------
      
      //--- Задать состояние реле "Командир/Наводчик" ---------
      case DESCRIPTOR_INPUT_SET_RELAY_STATE:
        if(cEthernet::packetBuffer[1 + BASE_HEADER_LENGTH] > 0) 
        {
          Relay.IsOn = true;   
        }
        else
        {
           Relay.IsOn = false;   
        }
        cEthernet::releaseBaseProtocol(DESCRIPTOR_OUTPUT_RELAY_STATE);   // реализация базового протокола
        cEthernet::AnswerBuffer[1 + BASE_HEADER_LENGTH] = Relay.IsOn;        // Значение параметра
        cEthernet::AnswerLength = 2 + BASE_HEADER_LENGTH;           // длина исходящего пакета
    
        // Передача ответа: в качестве адреса и номера порта берутся данные 
        cEthernet::sendAnswer();
      break;
      
      //--------------------------------------------------------
      
      //--- Запросить состояние реле "Командир/Наводчик" ---------
      case DESCRIPTOR_INPUT_GET_RELAY_STATE:

        cEthernet::releaseBaseProtocol(DESCRIPTOR_OUTPUT_RELAY_STATE);   // реализация базового протокола
        cEthernet::AnswerBuffer[1 + BASE_HEADER_LENGTH] = Relay.IsOn;        // Значение параметра
        cEthernet::AnswerLength = 2 + BASE_HEADER_LENGTH;           // длина исходящего пакета
    
        // Передача ответа: в качестве адреса и номера порта берутся данные 
        cEthernet::sendAnswer();
      break;
      
      //--------------------------------------------------------
      
      //--- Запросить состояние состояние пульта ---------------
      case DESCRIPTOR_INPUT_GET_SHORT_STATE:

        cEthernet::releaseBaseProtocol(DESCRIPTOR_OUTPUT_SHORT_STATE);   // реализация базового протокола
        cEthernet::AnswerBuffer[1 + BASE_HEADER_LENGTH] = Suo.getState();// Значение параметра
        cEthernet::AnswerLength = 2 + BASE_HEADER_LENGTH;                // длина исходящего пакета
    
        // Передача ответа: в качестве адреса и номера порта берутся данные 
        cEthernet::sendAnswer();
      break;
      
      //--------------------------------------------------------
      
      //--- Запросить расширенное состояние пульта -------------------
      case DESCRIPTOR_INPUT_GET_FULL_STATE:
      cEthernet::releaseBaseProtocol(DESCRIPTOR_OUTPUT_WIDE_STATE);                      // реализация базового протокола

      cEthernet::AnswerBuffer[1 + BASE_HEADER_LENGTH]  = Suo.getState();                 // Состояние СУО
      
      for(int i = 0; i < KEYS_ARRAY_LENGTH; i++)                                         // Состояние кнопок
      {
        cEthernet::AnswerBuffer[2 + BASE_HEADER_LENGTH + i]  = cKeyboard::Key[i].getValue();     
      }
                                                                                         
      cEthernet::AnswerBuffer[2 + BASE_HEADER_LENGTH + KEYS_ARRAY_LENGTH + 0] = cDisplay::Led1.getState();        // Состояние табло "ВКЛ"
      cEthernet::AnswerBuffer[2 + BASE_HEADER_LENGTH + KEYS_ARRAY_LENGTH + 1] = cDisplay::Led2.getState();        // Состояние элемента индикации "CТОПОР ОРУДИЯ"
      cEthernet::AnswerBuffer[2 + BASE_HEADER_LENGTH + KEYS_ARRAY_LENGTH + 2] = cDisplay::Led3.getState();        // Состояние элемента индикации "ПОХОДН"
      cEthernet::AnswerBuffer[2 + BASE_HEADER_LENGTH + KEYS_ARRAY_LENGTH + 3] = cDisplay::Led4.getState();        // Состояние элемента индикации "КОНТРР"
      cEthernet::AnswerBuffer[2 + BASE_HEADER_LENGTH + KEYS_ARRAY_LENGTH + 4] = cDisplay::Led5.getState();        // Состояние элемента индикации "БОЕВОЙ"
      cEthernet::AnswerBuffer[2 + BASE_HEADER_LENGTH + KEYS_ARRAY_LENGTH + 5] = cDisplay::Led6.getState();        // Состояние элемента индикации "ДЕЖУР"
      cEthernet::AnswerBuffer[2 + BASE_HEADER_LENGTH + KEYS_ARRAY_LENGTH + 6] = cDisplay::Led7.getState();        // Резерв для элемента индикации "БОЕПР"
      cEthernet::AnswerBuffer[2 + BASE_HEADER_LENGTH + KEYS_ARRAY_LENGTH + 7] = cDisplay::Led8.getState();        // Резерв для элемента индикации "ТРЕНИР"
      cEthernet::AnswerBuffer[2 + BASE_HEADER_LENGTH + KEYS_ARRAY_LENGTH + 8] = cDisplay::Led9.getState();        // Состояние элемента индикации "ВЕДУЩАЯ"
      cEthernet::AnswerBuffer[2 + BASE_HEADER_LENGTH + KEYS_ARRAY_LENGTH + 9] = cDisplay::Led10.getState();       // Состояние элемента индикации "ВЕДОМАЯ"
      cEthernet::AnswerBuffer[2 + BASE_HEADER_LENGTH + KEYS_ARRAY_LENGTH + 10] = cDisplay::Led11.getState();      // Состояние элемента индикации "АВТОНОМ"
      cEthernet::AnswerBuffer[2 + BASE_HEADER_LENGTH + KEYS_ARRAY_LENGTH + 11] = cDisplay::Led12.getState();      // Состояние элемента индикации "РАДИОМОЛЧАНИЕ"
      cEthernet::AnswerBuffer[2 + BASE_HEADER_LENGTH + KEYS_ARRAY_LENGTH + 12] = cDisplay::Led13.getState();      // Состояние элемента индикации "АВТО ПОИСК ПК"
      cEthernet::AnswerBuffer[2 + BASE_HEADER_LENGTH + KEYS_ARRAY_LENGTH + 13] = cDisplay::Led14.getState();      // Состояние табло "ПАВ ПОИСК ПК"
      cEthernet::AnswerBuffer[2 + BASE_HEADER_LENGTH + KEYS_ARRAY_LENGTH + 14] = cDisplay::Led15.getState();      // Состояние элемента индикации "ОБОГРЕВ ПК"
      cEthernet::AnswerBuffer[2 + BASE_HEADER_LENGTH + KEYS_ARRAY_LENGTH + 15] = cDisplay::Led16.getState();      // Состояние элемента индикации "ОЧИСТКА ПК"
      cEthernet::AnswerBuffer[2 + BASE_HEADER_LENGTH + KEYS_ARRAY_LENGTH + 16] = cDisplay::Led17.getState();      // Состояние элемента индикации "ОЭС ВКЛ ПК"
      cEthernet::AnswerBuffer[2 + BASE_HEADER_LENGTH + KEYS_ARRAY_LENGTH + 17] = cDisplay::Led18.getState();      // Состояние элемента индикации "ТПВ МАТРИЦА ПК"
      cEthernet::AnswerBuffer[2 + BASE_HEADER_LENGTH + KEYS_ARRAY_LENGTH + 18] = cDisplay::Led19.getState();      // Состояние элемента индикации "АВТО ПОИСК ПН"
      cEthernet::AnswerBuffer[2 + BASE_HEADER_LENGTH + KEYS_ARRAY_LENGTH + 19] = cDisplay::Led20.getState();      // Резерв для элемента индикации "ПАВ ПОИСК ПН"
      cEthernet::AnswerBuffer[2 + BASE_HEADER_LENGTH + KEYS_ARRAY_LENGTH + 20] = cDisplay::Led21.getState();      // Резерв для элемента индикации "ОБОГРЕВ ПН"
      cEthernet::AnswerBuffer[2 + BASE_HEADER_LENGTH + KEYS_ARRAY_LENGTH + 21] = cDisplay::Led22.getState();      // Состояние элемента индикации "ОЧИСТКА ПН"
      cEthernet::AnswerBuffer[2 + BASE_HEADER_LENGTH + KEYS_ARRAY_LENGTH + 22] = cDisplay::Led23.getState();      // Состояние элемента индикации "ОЭС ВКЛ ПН"
      cEthernet::AnswerBuffer[2 + BASE_HEADER_LENGTH + KEYS_ARRAY_LENGTH + 23] = cDisplay::Led24.getState();      // Состояние элемента индикации "ТПВ МАТРИЦА ПН"
      
      cEthernet::AnswerBuffer[2 + BASE_HEADER_LENGTH + KEYS_ARRAY_LENGTH + 24] = cDisplay::Led25.getState();      // Состояние клавиши "СОСТОЯНИЕ табло РЕЗЕРВ1"
      cEthernet::AnswerBuffer[2 + BASE_HEADER_LENGTH + KEYS_ARRAY_LENGTH + 25] = cDisplay::Led26.getState();      // Состояние клавшиши "СОСТОЯНИЕ табло РЕЗЕРВ2"

      cEthernet::AnswerBuffer[2 + BASE_HEADER_LENGTH + KEYS_ARRAY_LENGTH + 26] = cDisplay::Led27.getState();      // Состояние клавиши "ОЭС КОМ"
      cEthernet::AnswerBuffer[2 + BASE_HEADER_LENGTH + KEYS_ARRAY_LENGTH + 27] = cDisplay::Led28.getState();      // Состояние элемента индикации "ОЭС НАВ"
      cEthernet::AnswerBuffer[2 + BASE_HEADER_LENGTH + KEYS_ARRAY_LENGTH + 28] = cDisplay::Led29.getState();      // Состояние элемента индикации "ОТКАЗ"
      cEthernet::AnswerBuffer[2 + BASE_HEADER_LENGTH + KEYS_ARRAY_LENGTH + 29] = cDisplay::Led30.getState();      // Состояние элемента индикации "ПОРАЖ"
      cEthernet::AnswerBuffer[2 + BASE_HEADER_LENGTH + KEYS_ARRAY_LENGTH + 30] = cDisplay::Led31.getState();      // Состояние элемента индикации "ПОВТОР"
      cEthernet::AnswerBuffer[2 + BASE_HEADER_LENGTH + KEYS_ARRAY_LENGTH + 31] = cDisplay::Led32.getState();      // Состояние элемента индикации "ПРОМАХ"
      cEthernet::AnswerBuffer[2 + BASE_HEADER_LENGTH + KEYS_ARRAY_LENGTH + 32] = cDisplay::Led33.getState();      // Резерв для элемента индикации "АВТ"
      cEthernet::AnswerBuffer[2 + BASE_HEADER_LENGTH + KEYS_ARRAY_LENGTH + 33] = cDisplay::Led34.getState();      // Резерв для элемента индикации "ПАВ"
      cEthernet::AnswerBuffer[2 + BASE_HEADER_LENGTH + KEYS_ARRAY_LENGTH + 34] = cDisplay::Led35.getState();      // Состояние элемента индикации "ОТКЛ НРЗ"
      cEthernet::AnswerBuffer[2 + BASE_HEADER_LENGTH + KEYS_ARRAY_LENGTH + 35] = cDisplay::Led36.getState();      // Состояние элемента индикации "АВТОМ(РУЧНОЙ)"
      cEthernet::AnswerBuffer[2 + BASE_HEADER_LENGTH + KEYS_ARRAY_LENGTH + 36] = cDisplay::Led37.getState();      // Состояние элемента индикации "КОНТР ИНД"
      cEthernet::AnswerBuffer[2 + BASE_HEADER_LENGTH + KEYS_ARRAY_LENGTH + 37] = cDisplay::Led38.getState();      // Состояние элемента индикации "ГОТОВ"
      cEthernet::AnswerBuffer[2 + BASE_HEADER_LENGTH + KEYS_ARRAY_LENGTH + 38] = cDisplay::Led39.getState();      // Состояние элемента индикации "БЛОКИР"
      cEthernet::AnswerBuffer[2 + BASE_HEADER_LENGTH + KEYS_ARRAY_LENGTH + 39] = cDisplay::Led40.getState();      // Состояние табло              "КТН"
      cEthernet::AnswerBuffer[2 + BASE_HEADER_LENGTH + KEYS_ARRAY_LENGTH + 40] = cDisplay::Led41.getState();      // Состояние элемента индикации "КУМЗ"
      cEthernet::AnswerBuffer[2 + BASE_HEADER_LENGTH + KEYS_ARRAY_LENGTH + 41] = cDisplay::Led42.getState();      // Состояние элемента индикации "АЗП"
      cEthernet::AnswerBuffer[2 + BASE_HEADER_LENGTH + KEYS_ARRAY_LENGTH + 42] = cDisplay::Led43.getState();      // Состояние элемента индикации "ПУЛЕМЁТ"
      cEthernet::AnswerBuffer[2 + BASE_HEADER_LENGTH + KEYS_ARRAY_LENGTH + 43] = cDisplay::Led44.getState();      // Состояние элемента индикации "ПНС"
      cEthernet::AnswerBuffer[2 + BASE_HEADER_LENGTH + KEYS_ARRAY_LENGTH + 44] = cDisplay::Led45.getState();      // Состояние элемента индикации "ТАБЛО ТУМБЛЕРА ВКЛ ПНС"
      cEthernet::AnswerBuffer[2 + BASE_HEADER_LENGTH + KEYS_ARRAY_LENGTH + 45] = cDisplay::Led46.getState();      // Состояние для элемента индикации "ТАБЛО ТУМБЛЕРА МПиЭ"
      cEthernet::AnswerBuffer[2 + BASE_HEADER_LENGTH + KEYS_ARRAY_LENGTH + 46] = cDisplay::Led47.getState();      // Состояние для элемента индикации "ВНИМАНИЕ"
      
      cEthernet::AnswerBuffer[2 + BASE_HEADER_LENGTH + KEYS_ARRAY_LENGTH + 47] = 4;                               // Состояние для элемента индикации "ГОТОВ ШАССИ" 
      cEthernet::AnswerBuffer[2 + BASE_HEADER_LENGTH + KEYS_ARRAY_LENGTH + 48] = 4;                               // Состояние для элемента индикации "КОМ" 
      cEthernet::AnswerBuffer[2 + BASE_HEADER_LENGTH + KEYS_ARRAY_LENGTH + 49] = 4;                               // Состояние для элемента индикации "НАВ" 
      cEthernet::AnswerBuffer[2 + BASE_HEADER_LENGTH + KEYS_ARRAY_LENGTH + 50] = cPult::ParameterDebounce;        // Значение параметра подавления дребезга
      cEthernet::AnswerBuffer[2 + BASE_HEADER_LENGTH + KEYS_ARRAY_LENGTH + 51] = cPult::ErrorCode;                // Значение параметра "код ошибки"
      cEthernet::AnswerBuffer[2 + BASE_HEADER_LENGTH + KEYS_ARRAY_LENGTH + 52] = (byte)Relay.IsOn;                // Состояние реле "ВЫБОР К/Н"

      //cEthernet::AnswerBuffer[2 + BASE_HEADER_LENGTH + KEYS_ARRAY_LENGTH + 53] = cPult::Version[0];              // Версия ПО пульта цифра 1
      //cEthernet::AnswerBuffer[2 + BASE_HEADER_LENGTH + KEYS_ARRAY_LENGTH + 54] = cPult::Version[1];              // Версия ПО пульта цифра 2
      //cEthernet::AnswerBuffer[2 + BASE_HEADER_LENGTH + KEYS_ARRAY_LENGTH + 55] = cPult::Version[2];              // Версия ПО пульта цифра 3
     
      cEthernet::AnswerLength = 95 + BASE_HEADER_LENGTH;                                                         // длина исходящего пакета
    
      // Передача ответа: в качестве адреса и номера порта берутся данные 
      cEthernet::sendAnswer();
      break;
      
      //--------------------------------------------------------
      
      //--- Команда диагностики пульта ------------------
      case DESCRIPTOR_INPUT_DIAGNOSTICS:

      //
      //... здесь должен помещаться анализ команды диагностики
      //
      cEthernet::releaseBaseProtocol(DESCRIPTOR_OUTPUT_DIAGNOSTIC);                                       // реализация базового протокола
      cEthernet::AnswerBuffer[1 + BASE_HEADER_LENGTH] = cEthernet::packetBuffer[1 + BASE_HEADER_LENGTH];  // Чтение номера команды диагностики                 // Номер выполненной команды диагностики
      cEthernet::AnswerBuffer[2 + BASE_HEADER_LENGTH] = cPult::Result[0];  // Параметр 1 результата
      cEthernet::AnswerBuffer[3 + BASE_HEADER_LENGTH] = cPult::Result[1];  // Параметр 2 результата
      cEthernet::AnswerBuffer[4 + BASE_HEADER_LENGTH] = cPult::Result[2];  // Параметр 3 результата
      
      cEthernet::AnswerLength = 5 + BASE_HEADER_LENGTH;           // длина исходящего пакета
    
      // Передача ответа
      cEthernet::sendAnswer();
      
      break;
      
      //--------------------------------------------------------

      default:
      break;
       
    }//End of switch(switch(cEthernet::packetBuffer[0]))
    
    // Очистка буфера входящих пакетов
    for(int i = 0; i < 8; i++) cEthernet::packetBuffer[i] = ' ';
  
  }//End of if (cEthernet::packetSize > 0)
  
}//End of void cPult::exec()

//--------------------------------------------------------------------------------
void cPult::view()
{
    String s;
    int color;
    int number;
    
    switch(cEthernet::packetBuffer[0])
    {
      //--------------------------------------------------------
      //--- Включить индикатор с заданным номером -------
      case DESCRIPTOR_INPUT_ON_LED:
        s = "Light on LED: ";
        color = cEthernet::packetBuffer[1+ BASE_HEADER_LENGTH] & 0x3;
        number = (int)cEthernet::packetBuffer[1+ BASE_HEADER_LENGTH] / 4;
        s += String(number, DEC);
        s += " color: ";
        s += String(color, DEC);
        Serial.println(s);
      break;
      //--------------------------------------------------------
      //--- Выключить индикатор с заданным номером ------
      case DESCRIPTOR_INPUT_OFF_LED:
        s = "Light off LED: ";
        color = cEthernet::packetBuffer[1+ BASE_HEADER_LENGTH] & 0x3;
        number = (int)cEthernet::packetBuffer[1+ BASE_HEADER_LENGTH] / 4;
        s += String(number, DEC);
        s += " color: ";
        s += String(color, BIN);
      Serial.println(s);
      break;
      //--------------------------------------------------------
      //--- Установить значение параметра пульта --------
      case DESCRIPTOR_INPUT_SET_PARAMETER:
        s = "Set parameter: ";
        number = cEthernet::packetBuffer[1+ BASE_HEADER_LENGTH];
        s += String(number, DEC);
        Serial.println(s);
      break;
      //--------------------------------------------------------
      //--- Запросить значение параметра пульта ---------
      case DESCRIPTOR_INPUT_GET_PARAMETER:
        s = "Get parameter: ";
        number = (int)cPult::ParameterDebounce;
        s += String(number, DEC);
        Serial.println(s);
      break;
     
      //--------------------------------------------------------
      //--- Задать состояние реле "Командир/Наводчик" ---------
      case DESCRIPTOR_INPUT_SET_RELAY_STATE:
        s = "Set relay C/G: ";
        number = (int)Relay.IsOn;
        s += String(number, DEC);
        Serial.println(s);
      break;
      
      //--------------------------------------------------------
      //--- Зпросить состояние реле "Командир/Наводчик" ---------
      case DESCRIPTOR_INPUT_GET_RELAY_STATE:
        s = "Get relay C/G: ";
        number = (int)Relay.IsOn;
        s += String(number, DEC);
        Serial.println(s);
      break;
      
      //--------------------------------------------------------
      //--- Запросить состояние состояние пульта ---------------
      case DESCRIPTOR_INPUT_GET_SHORT_STATE:
        s = "Get short SUO state: ";
        number = (int)Suo.getState();
        s += String(number, DEC);
        Serial.println(s);
      break;
      
      //--------------------------------------------------------
      case DESCRIPTOR_INPUT_GET_FULL_STATE:
        s = "Get wide SUO state: ";
        number = 94 + BASE_HEADER_LENGTH;
        s += String(number, DEC);
        s += " bytes";
        Serial.println(s);
      break;
      //--------------------------------------------------------
       //--- Команда диагностики пульта ------------------
      case DESCRIPTOR_INPUT_DIAGNOSTICS:

      //
      //... здесь должен помещаться анализ команды диагностики
      //
        s = "Start diagnostics: ";
        number = cEthernet::packetBuffer[1 + BASE_HEADER_LENGTH];
        s += String(number, DEC);
        s += " result: ";
        s += String(cPult::Result[0], DEC);
        s += ", "; 
        s += String(cPult::Result[0], DEC);
        s += ", "; 
        s += String(cPult::Result[0], DEC);
        
        Serial.println(s);
      break;
      //--------------------------------------------------------

      default:
      break;
       
    }//End of switch(switch(cEthernet::packetBuffer[0]))
  
}//End of void cPult::view()
//--------------------------------------------------------------------------------

//--------------------------------------------------------------------------------
//----------------------------------ОБЪЯВЛЕНИЕ СТАТИЧЕСКИХ ПЕРЕМЕННЫХ-------------
//--------------------------------------------------------------------------------
//
// Параметры пульта
//
char cPult::ParameterDebounce = 200;              // Параметр для подавления дребезга
char cPult::ErrorCode = 0;                        // Значение параметра ошибки
char cPult::Version[6] = {1, 0, 0, 12, 12, 17};   // Версия

char cPult::Result[3] = {0, 0, 0};                // Результат выполнения диагностики

cCommunication cPult::Com;                        //Коммуникационная составляющая

//--------------------------------------------------------------------------------
#endif
