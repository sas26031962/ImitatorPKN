/*
 Класс управления коммуникацией по последовательному порту
 P - нажать на кнопку
 R - отпустить кнопку
 
 */
 //========================================================================
 //---------------------------- ПОДКЛЮЧАЕМЫЕ ФАЙЛЫ ------------------------
 //========================================================================
 #include "cSUO.h"
 //========================================================================
 //-------------------------------- КОНСТАНТЫ -----------------------------
 //========================================================================
 #define SINK_BUFFER_LENGTH 20
 
 #define SERIAL_SPEED 115200
 #define MARKER '{'
 #define SEARCH_MARKER_PHASE 0
 #define GET_FRAME_BODY_PHASE 1

 #define BUFFER_OVERFLOW_ERROR -1

 #define COMMAND_DISPLACEMENT 1
 #define COLON_DISPLACEMENT COMMAND_DISPLACEMENT+1
 #define PARAMETER_DISPLACEMENT COLON_DISPLACEMENT+1

 #define TITLE "Commander & Gunlayer Pult v1.0.0"

 //========================================================================
 //--------------------------- ОБЪЯВЛЕНИЕ КЛАССОВ -------------------------
 //========================================================================

class cCommunication{
 
 //------------------------------------------------------------------------
 // Атрибуты
 //------------------------------------------------------------------------
 private:
 public:
  int Phase = 0;//Фаза процесса коммуникации
                // 0 - поиск маркера
                // 1 - приём тела кадра
  char SinkBuffer[20] = {
    ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ',                   
    ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' '                   
    }; //Массив SINK_BUFFER_LENGTH байт
  int SinkCounter = 0;

  char x; //Текущий принятый байт

  String sParameter; //Строка, содержащая значение параметра
  int iParameter;
  
  bool IsRemoteControl;   //Флаг управления от удалённого терминала
  
 public:

 
 //------------------------------------------------------------------------
 // Методы
 //------------------------------------------------------------------------

 //Инициализация коммуникационного порта
 void install();
 
 //Реализация функциональности
 void operate();    

 //Реализация фазы поиска маркера
 void searchMarker();

 //Приём одного байта в буфер, возвращает принятый байт
 char sinkByte();

 //Эхо принятого пакета
 void echoFrame();

//-------------------------------------------------------------
// Функции управления
//-------------------------------------------------------------

 //Чтение параметра
 void getParameter();

 //Управление контроллером
 void driveController();

 //------------------------------------------------------------------------
 // Конструкторы
 //------------------------------------------------------------------------
 cCommunication();
 
};//End of class cCommunication

 //========================================================================
 //--------------------------- РЕАЛИЗАЦИЯ МЕТОДОВ -------------------------
 //========================================================================

//
// Конструктор
//
cCommunication::cCommunication() 
{
  
  this->Phase = 0;
  this->SinkCounter = 0;
  this->IsRemoteControl = false;
  
}//End of ctor 

//
// Инициализация коммуникационного порта
//
void cCommunication::install()
{
  Serial.begin(SERIAL_SPEED);
  while (!Serial) {;}
  
  Serial.println(TITLE);

}//End of void cCommunication::install()

//
//Реализация фазы поиска маркера
// 
//
void cCommunication::searchMarker()
{
  if(this->x == MARKER)
  {
    this->SinkCounter = 0;
    this->SinkBuffer[this->SinkCounter] = this->x;
    this->SinkCounter++;
    this->Phase = GET_FRAME_BODY_PHASE;
        
    //Serial.println("Marker detected, goto sink frame body");
  }      
}//End of void cCommunication::searchMarker()

//
// Приём одного байта в буфер, возвращает принятый байт
//
char cCommunication::sinkByte()
{
  this->SinkBuffer[this->SinkCounter] = this->x;
  this->SinkCounter++;// Проверка границы массива
  //Serial.print(this->x);//Эхо для проверки
  //Проверка границы массива
  if(this->SinkCounter == SINK_BUFFER_LENGTH)
  {
    this->Phase = 0;
    //Serial.println(); //Конец эха
    Serial.println("Right baund array error, goto SEARCH_MARKER phase");
    return this->x;
  }
  else
  {
    return BUFFER_OVERFLOW_ERROR;
  }
}//End of void cCommumnication::sinkByte()

//
//Эхо принятого пакета
//
void cCommunication::echoFrame()
{
  for(int i = 0; i < this->SinkCounter; i++)Serial.print(this->SinkBuffer[i]);
  Serial.println();
}
 
//Чтение параметра
void  cCommunication::getParameter()
{
  if(this->SinkBuffer[2]!= ':')
  {
    Serial.println("Colon missing error");
    this->Phase = SEARCH_MARKER_PHASE;
  }
  else
  {
    this->sParameter = "";
    int Pointer = 3;//Смещение для параметра
    while((this->SinkBuffer[Pointer] != '}')&&(this->SinkBuffer[Pointer] != ',')&&(Pointer < SINK_BUFFER_LENGTH - 3))
    {
      this->sParameter += this->SinkBuffer[Pointer];
      Pointer++;
    }
  }    
}//End of void  cCommunication::getParameter()



//
// Прямое управление контроллером
//
void cCommunication::driveController()
{
  if(this->sParameter == "reset")
  {
    Serial.println("Reset controller");
  }
  
}//End of void cCommunication::driveController()


//
// Реализация функциональности
//
void cCommunication::operate()
{
  if(Serial.available())
  {
    this->x = Serial.read();
    switch(this->Phase)
    {
      case 0: this->searchMarker(); break;

      case 1:
        this->sinkByte();
        if(this->x == '}')
        {
          //Serial.println("Sink frame complete, goto frame analisys");
          //Com.echoFrame();          
          
          //
          //... здесь анализ пакета
          //
          switch(this->SinkBuffer[1])
          {
            case 'P':// Нажатие на кнопку
            Serial.print("Press button command: ");
            this->getParameter();
            this->iParameter = this->sParameter.toInt();
            Serial.println(this->iParameter);
            // Выполнение команды
            cKeyboard::Key[this->iParameter].Current = true;
            break;
            //-------------------------------------
            case 'R':// Отпускание кнопки
            Serial.print("Release button command: ");
            this->getParameter();
            Serial.println(this->iParameter);
            // Выполнение команды
            if((this->iParameter > 0) && (this->iParameter < KEYS_ARRAY_LENGTH))
            {
              cKeyboard::Key[this->iParameter].Current = false;
            }
            else
            {
              Serial.println("Wrong parameter value!!!");
            }
            break;
            //-------------------------------------
            case 'S':// Управление СУО
            this->getParameter();
            // Выполнение команды
            if(this->sParameter == "on")
            {
              cSUO::IsSwitchOn = true;
              Serial.println("SUO had gotten SUO mount command");
            }
           if(this->sParameter == "off")
            {
              cSUO::IsSwitchOff = true;
              Serial.println("SUO had gotten SUO unmount command");
            }
            break;
            //-------------------------------------
            
            default:
            Serial.print("Unknown frame command: ");
            Serial.println(this->SinkBuffer[1]);
            break;  
          }//End of switch(SinkBuffer[1])
          
          this->Phase = SEARCH_MARKER_PHASE;
          //Serial.println("Frame analisys complete, goto SEARCH_MARKER phase");
          Serial.print(">");
        }
      break;

      default:
        this->Phase = SEARCH_MARKER_PHASE;
        Serial.println("Unknown command, goto SEARCH_MARKER phase");
      break;  
    }//End of switch(Com.Phase) 
    
  }//End of if(Serial.available())
}//End of void cCommunication::operate()

//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ 


