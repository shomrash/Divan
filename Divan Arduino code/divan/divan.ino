// ************************ Лента *************************
#define BRIGHTNESS 120      // стандартная маскимальная яркость (0-255)
#define CURRENT_LIMIT 750    // лимит по току в миллиамперах, автоматически управляет яркостью (пожалей свой блок питания!) 0 - выключить лимит


#define DEBUG 0

#if (DEBUG == 0)
#define NUM_LEDS 133    
#define NUM_LEDS_SIDE 27

#elif (DEBUG == 1)
#define NUM_LEDS 112
#define NUM_LEDS_SIDE 20

#endif


// ******************** ЭФФЕКТЫ И РЕЖИМЫ ********************

// ****************** ПИНЫ ПОДКЛЮЧЕНИЯ *******************
#define LED_PIN 6           // пин ленты

// **************Управление по бт *************
#define BT_MODE 1           // использовать блютус (0 нет, 1 да)
#include <SoftwareSerial.h> 
SoftwareSerial customSerial(9, 10); // RX, TX

// ******************************** ДЛЯ РАЗРАБОТЧИКОВ ********************************


#include "FastLED.h"
CRGB leds[NUM_LEDS];

boolean idleState = true;  // флаг холостого режима работы
int8_t thisMode = -1;
int8_t subMode = 0;

uint32_t globalColor = 0x1d1dd1;
int globalBrightness = BRIGHTNESS;
int breathTime = 5000;

#include "GyverTimer.h"
GTimer_ms changeTimer(10);
GTimer_ms colorTimer(100);

void setup() {
  #if (BT_MODE == 1)
    Serial.begin(9600); 
    customSerial.begin(9600);
  #endif
  
  pinMode(3, OUTPUT);
  pinMode(5, OUTPUT);
  digitalWrite(3, HIGH);
  digitalWrite(5, LOW);

  // настройки ленты
  FastLED.addLeds<WS2812, LED_PIN, GRB>(leds, NUM_LEDS).setCorrection( TypicalLEDStrip );
  FastLED.setBrightness(BRIGHTNESS);
  if (CURRENT_LIMIT > 0) FastLED.setMaxPowerInVoltsAndMilliamps(5, CURRENT_LIMIT);
  
	FastLED.clear();
  for (int i = 0; i< NUM_LEDS; i++){
		leds[i] = globalColor;
	}
	FastLED.show();
	delay(5000);
	FastLED.clear();
	FastLED.show();
}

void loop() {
  bluetoothRoutine();
}
