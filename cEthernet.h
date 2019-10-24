/*
 Класс реализации протокола информационного обмена
 */
#ifndef C_ETHERNET
#define C_ETHERNET

 //========================================================================
 //---------------------------- ПОДКЛЮЧАЕМЫЕ ФАЙЛЫ ------------------------
 //========================================================================
  #include <SPI.h>         
  #include <Ethernet.h>
  #include <EthernetUdp.h>     
 
 //========================================================================
 //-------------------------------- КОНСТАНТЫ -----------------------------
 //========================================================================
 #define DESTINATION_PORT 51075
 #define LOCAL_PORT 51075

 //========================================================================
 //--------------------------- ОБЪЯВЛЕНИЕ КЛАССОВ -------------------------
 //========================================================================
 
class cEthernet{
 
 //------------------------------------------------------------------------
 // Атрибуты
 //------------------------------------------------------------------------
 public:
 static int   PacketCounter;                    // Счётчик переданных пакетов
 static char  packetBuffer[UDP_BUFFER_SIZE];    // буфер для хранения входящего пакета,
 static char  AnswerBuffer[UDP_BUFFER_SIZE];    // буфер для передачи в ответ
 static int   AnswerLength;                     // длина исходящего пакета

 static unsigned int destinationPort;           //Удалённый порт ЦВС     
 static unsigned int localPort;                 //Локальный порт пульта      
 static IPAddress remote_ip;//(192, 168, 100, 2);  //Удалённый IP адрес ЦВС 
 static IPAddress local_ip;//(192, 168, 100, 75);  //Локальный IP адрес пульта 

 //
 // Экземпляр класса для реализации передачи и приёма информации по протоколу UDP
 //
 static EthernetUDP Udp;

 // 
 // Параметры принятого пакета
 //
 static int packetSize;             // размер принятого пакета

 
 //------------------------------------------------------------------------
 // Методы
 //------------------------------------------------------------------------
 
 static void releaseBaseProtocol(int descriptor);          
 static void sendAnswer();
 static void installEthernet();          
 
 //------------------------------------------------------------------------
 // Конструкторы
 //------------------------------------------------------------------------
 cEthernet();
 
};//End of class cEthernet

 //========================================================================
 //--------------------------- РЕАЛИЗАЦИЯ МЕТОДОВ -------------------------
 //========================================================================

//
// Конструктор
//
cEthernet::cEthernet() 
{
  //...
} 

//
// Реализация базового протокола
//
void cEthernet::releaseBaseProtocol(int descriptor)
{
      cEthernet::PacketCounter++;                      // Подсчёт числа пакетов
      AnswerBuffer[0] = descriptor & 0x00ff;           // Дескриптор исходящего пакета младший байт
      AnswerBuffer[1] = (descriptor & 0xff00) >> 8;    // Дескриптор исходящего пакета старший байт
      AnswerBuffer[2] = PacketCounter & 0x00ff;        // Счётчик пакетов младший байт
      AnswerBuffer[3] = (PacketCounter & 0xff00) >> 8; // Счётчик пакетов старший байт
                                                       // 
      AnswerBuffer[4] = 0;                             // Текущее время (равно 0) 8 байт
      AnswerBuffer[5] = 0;                             // 
      AnswerBuffer[6] = 0;                             // 
      AnswerBuffer[7] = 0;                             // 
      AnswerBuffer[8] = 0;                             // 
      AnswerBuffer[9] = 0;                             // 
      AnswerBuffer[10] = 0;                            // 
      AnswerBuffer[11] = 0;                            // 

}//End of void cEthernetreleaseBaseProtocol(char buf, descriptor) 

//
// Передача сообщения
//
void cEthernet::sendAnswer()
{
  
    cEthernet::Udp.beginPacket(cEthernet::remote_ip, cEthernet::destinationPort);// Инициализация UDP на передачу
    cEthernet::Udp.write( cEthernet::AnswerBuffer, cEthernet::AnswerLength);// Передача ответа
    cEthernet::Udp.endPacket();// Закрытие UDP канала на передачу
  
}//End of void cEthernet::sendAnswer()

//
// Начальная установка
//
void cEthernet::installEthernet()
{
 //
 // MAC адрес устройства
 //
 byte mac[] = {
    0xDE, 0xAD, 0xBE, 0xEF, 0xFE, 0xEE
  };
  Ethernet.begin(mac, cEthernet::local_ip);      //Запуск Ethernet
  cEthernet::Udp.begin(cEthernet::localPort);    // Запуск UDP на приём
  
}//End of void cEthernet::installEthernet()

//--------------------------------------------------------------------------------
//----------------------------------ОБЪЯВЛЕНИЕ СТАТИЧЕСКИХ ПЕРЕМЕННЫХ-------------
//--------------------------------------------------------------------------------
int   cEthernet::PacketCounter = 0;                    // Счётчик переданных пакетов
char  cEthernet::packetBuffer[UDP_BUFFER_SIZE];        // буфер для хранения входящего пакета,
char  cEthernet::AnswerBuffer[UDP_BUFFER_SIZE];        // буфер для передачи в ответ
int   cEthernet::AnswerLength = 0;                     // длина исходящего пакета

unsigned int cEthernet::destinationPort = DESTINATION_PORT; //Удалённый порт ЦВС     
unsigned int cEthernet::localPort = LOCAL_PORT;             //Локальный порт пульта      

IPAddress cEthernet::remote_ip(192, 168, 100, 1);      //Удалённый IP адрес ЦВС 
IPAddress cEthernet::local_ip(192, 168, 100, 75);      //Локальный IP адрес пульта

 //
 // Экземпляр класса для реализации передачи и приёма информации по протоколу UDP
 //
 EthernetUDP cEthernet::Udp;

 int cEthernet::packetSize;                           // размер принятого пакета

#endif


