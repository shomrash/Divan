// вкладка работы с bluetooth

#if (BT_MODE == 1)
#define PARSE_AMOUNT 4    // максимальное количество значений в массиве, который хотим получить
#define header '&'        // стартовый символ
#define divider '-'       // разделительный символ
#define ending ';'        // завершающий символ



int intData[PARSE_AMOUNT];     // массив численных значений после парсинга
bool recievedFlag;
bool parseStarted;
String runningText;
//булевские переменные, отвечающие за эффекты
bool breathFlag;
byte symmetryType;


void bluetoothRoutine() {
  parsing();                           // принимаем данные
  
	/*
	Режимы работы леда:
	0: статичный цвет. Берется из глобала
	1: плавный градиент
	2: рандомная смена цвета
	3: режим ментов
	4: бегущая радуга
	*/
  if (!parseStarted) {                // на время принятия данных матрицу не обновляем!
		if (changeTimer.isReady())
		{
			switch (thisMode){
				case 0:
					staticColor();
				break;
				case 1:
					colorChange();
				break;
				case 2:
					randomColor();
				break;
				case 3:
					policeMode();
				break;
				case 4:
					rainbowColor();
				break;
			}
			if (!(thisMode == 2 || thisMode == 3)) breathEffect();
			FastLED.show();
		}
  }
}

byte parse_index;
String string_convert = "";
enum modes {NORMAL, COLOR, TEXT} parseMode;


// ********************* ПРИНИМАЕМ ДАННЫЕ **********************
void parsing() {
  // ****************** ОБРАБОТКА *****************
  
  if (recievedFlag) {      // если получены данные
    recievedFlag = false;
    idleState = false;
    
	/*
	Команды управления: 
	&0-HEX_Color; - установка глобального цвета
	&1-INT_VAL; - установка яркости цвета
	&2-Mode-SubMode; - установка режима
	&3; - обновление ленты
  &4-setiings_num-val;
      0- сохранить настройки
      1- изменение цвета
				color_speed
			2-симметрия
				0-нет
				1-updown
				2-leftright
			3- переход через дыхание
				0-false
				1-true
	*/
	
    switch (intData[0]) {
      case 0: 
		//
        break;
			case 1:
        globalBrightness = intData[1];
				FastLED.setBrightness(globalBrightness);
				break;   
      case 2:
				thisMode = intData[1];
				subMode = intData[2];
				break;
      case 3:
        FastLED.show();
				break;  
			case 4:
				switch(intData[1]){
					case 0:
						break;
					case 1:
						colorTimer.setInterval(intData[2]);
					break;
					case 2:
						symmetryType = intData[2];
					break;
					case 3:
						breathFlag = intData[2];
					break;
				}
				break;      		
    }
  }

  // ****************** ПАРСИНГ *****************
  if (customSerial.available() > 0) {
    char incomingByte;
    if (parseMode == TEXT) {     // если нужно принять строку
      runningText = customSerial.readString();  // принимаем всю
      incomingByte = ending;              // сразу завершаем парс
      parseMode = NORMAL;
    } else {
      incomingByte = customSerial.read();        // обязательно ЧИТАЕМ входящий символ
    }
    if (parseStarted) {                         // если приняли начальный символ (парсинг разрешён)
      if (incomingByte != divider && incomingByte != ending) {   // если это не пробел И не конец
        string_convert += incomingByte;       // складываем в строку
      } else {                                // если это пробел или ; конец пакета
        if (parse_index == 0) {
          byte thisMode = string_convert.toInt();
          switch (thisMode){
            case 0: parseMode = COLOR;
              break;
            default: parseMode = NORMAL;
              break;
          }
        }

        if (parse_index == 1) {       // для второго (с нуля) символа в посылке
          if (parseMode == NORMAL) intData[parse_index] = string_convert.toInt();             // преобразуем строку в int и кладём в массив}
          //if (parseMode == COLOR) globalColor = strtol(&string_convert[0], NULL, 16);     // преобразуем строку HEX в цифру
          if (parseMode == COLOR) globalColor = (uint32_t)HEXtoInt(string_convert);     // преобразуем строку HEX в цифру
        } else {
          intData[parse_index] = string_convert.toInt();  // преобразуем строку в int и кладём в массив
        }
        string_convert = "";                  // очищаем строку
        parse_index++;                              // переходим к парсингу следующего элемента массива
      }
    }
    if (incomingByte == header) {             // если это $
      parseStarted = true;                      // поднимаем флаг, что можно парсить
      parse_index = 0;                              // сбрасываем индекс
      string_convert = "";                    // очищаем строку
    }
    if (incomingByte == ending) {             // если таки приняли ; - конец парсинга
      parseMode == NORMAL;
      parseStarted = false;                     // сброс
      recievedFlag = true;                    // флаг на принятие
    }
  }
}

// hex string to uint32_t
uint32_t HEXtoInt(String hexValue) {
  byte tens, ones, number1, number2, number3;
  tens = (hexValue[0] < '9') ? hexValue[0] - '0' : hexValue[0] - '7';
  ones = (hexValue[1] < '9') ? hexValue[1] - '0' : hexValue[1] - '7';
  number1 = (16 * tens) + ones;

  tens = (hexValue[2] < '9') ? hexValue[2] - '0' : hexValue[2] - '7';
  ones = (hexValue[3] < '9') ? hexValue[3] - '0' : hexValue[3] - '7';
  number2 = (16 * tens) + ones;

  tens = (hexValue[4] < '9') ? hexValue[4] - '0' : hexValue[4] - '7';
  ones = (hexValue[5] < '9') ? hexValue[5] - '0' : hexValue[5] - '7';
  number3 = (16 * tens) + ones;

  return ((uint32_t)number1 << 16 | (uint32_t)number2 << 8 | number3 << 0);
}

#elif (BT_MODE == 0)
void bluetoothRoutine() {
  return;
}
#endif
