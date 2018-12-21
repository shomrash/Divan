//вкладка режимов

void staticColor(){
	for (int i = 0; i< NUM_LEDS; i++){
		leds[i] = globalColor;
	}
}

int this_color = 0;
void colorChange(){
  if (colorTimer.isReady()){
    if (++this_color > 255) 
      this_color = 0;
    
    for (int i = 0; i < NUM_LEDS; i++) 
      nonSym(this_color, i);
  }
}

void randomColor(){
if (!breathFlag && colorTimer.isReady() || breathEffect()){
		switch (symmetryType){
			case 0:{
				this_color = random(0,255);
				for (int i = 0; i < NUM_LEDS; i++) 
					nonSym(this_color,i);
			}break;
			case 1:{
				updownSymmetry(random(0,255),random(0,255));
			}break;
			case 2:{
				leftrightSymmetry(random(0,255),random(0,255));
			}break;
		}
	}
}

bool policeDirection;
void policeMode(){
	if (!breathFlag && colorTimer.isReady() || breathEffect()){
		if (policeDirection) {
			leftrightSymmetry(240,0);
		}
		else {
			leftrightSymmetry(0,240);
		}
		policeDirection = !policeDirection;
	}
}

int RAINBOW_PERIOD = 3;
float RAINBOW_STEP_2 = 5.5;
float rainbow_steps;
void rainbowColor(){
	if (colorTimer.isReady()) {
		this_color += RAINBOW_PERIOD;
		if (this_color > 255) this_color = 0;
		if (this_color < 0) this_color = 255;
	}
	
	rainbow_steps = this_color;
	
	for (int i = 0; i < NUM_LEDS; i++) {
		nonSym((int)floor(rainbow_steps),i);
		rainbow_steps += RAINBOW_STEP_2;
		if (rainbow_steps > 255) rainbow_steps = 0;
		if (rainbow_steps < 0) rainbow_steps = 255;
	}
}

//Эффекты
bool brightnessDirection;
byte breathBrightness;
bool breathEffect(){
	if (!breathFlag) return false;
	int breathChange = globalBrightness/(changeTimer.getInterval()*breathTime);
	if (breathChange <1) breathChange = 1;
	if (brightnessDirection) {
		breathBrightness += breathChange;
		if (breathBrightness > globalBrightness - 1) {
			brightnessDirection = false;
		}
	} else {
		breathBrightness -= breathChange;
		if (breathBrightness < 1) {
			brightnessDirection = true;
		}
	}
	FastLED.setBrightness(breathBrightness);
	return (breathBrightness < 1);
}

void nonSym(int color, int led){
  leds[led] = CHSV(color, 255, globalBrightness);
}
void updownSymmetry(int upColor, int downColor){
	for (int i = 0; i < NUM_LEDS_SIDE; i++) 
      nonSym(downColor,i);
	for (int i = NUM_LEDS-NUM_LEDS_SIDE; i < NUM_LEDS; i++) 
      nonSym(downColor,i);
	for (int i = NUM_LEDS_SIDE; i < NUM_LEDS-NUM_LEDS_SIDE; i++) 
      nonSym(upColor,i);
}

void leftrightSymmetry(int leftColor, int rightColor){
	for (int i = 0; i < NUM_LEDS/2; i++) 
		nonSym(rightColor,i);
	for (int i = NUM_LEDS/2; i < NUM_LEDS; i++) 
		nonSym(leftColor,i);
}
