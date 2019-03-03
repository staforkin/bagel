
//           7    22    7
// 1      15,15,10,10,32,12,11      1
//            7   1   7
// [0 - 52] - контур
// [53 - 157] - буквы
// [8 - 29] - крендель
// https://www.tweaking4all.com/hardware/arduino/adruino-led-strip-effects/
#define NUM_LEDS 158
#define LED_COUNT 158
#include "FastLED.h"
#define LED_PIN 13
#define BTN_PIN 12

#define MODE 0        // режим при запуске
#define MAIN_LOOP 5   // период основного цикла отрисовки (по умолчанию 5)
#define MODE_AMOUNT 7 // количество режимов

#include "GyverButton.h"
GButton button(BTN_PIN);

byte counter;
byte currentMode = MODE;
boolean ONstate = true;
unsigned long main_timer;
boolean fadeIn = true;

// ---------------СЛУЖЕБНЫЕ ПЕРЕМЕННЫЕ-----------------
int BOTTOM_INDEX = 0;        // светодиод начала отсчёта
int TOP_INDEX = int(NUM_LEDS / 2);
int EVENODD = NUM_LEDS % 2;
struct CRGB leds[NUM_LEDS];
int ledsX[NUM_LEDS][3];     //-ARRAY FOR COPYING WHATS IN THE LED STRIP CURRENTLY (FOR CELL-AUTOMATA, MARCH, ETC)

int thisdelay = 20;          //-FX LOOPS DELAY VAR
int thisstep = 10;           //-FX LOOPS DELAY VAR
int thishue = 220;             //-FX LOOPS DELAY VAR
int thissat = 255;           //-FX LOOPS DELAY VAR

int thisindex = 0;
int thisRED = 0;
int thisGRN = 0;
int thisBLU = 0;

int idex = 0;                //-LED INDEX (0 to LED_COUNT-1
int ihue = 0;                //-HUE (0-255)
int ibright = 0;             //-BRIGHTNESS (0-255)
int isat = 0;                //-SATURATION (0-255)
int bouncedirection = 0;     //-SWITCH FOR COLOR BOUNCE (0-1)
float tcount = 0.0;          //-INC VAR FOR SIN LOOPS
int lcount = 0;              //-ANOTHER COUNTING VAR
// ---------------СЛУЖЕБНЫЕ ПЕРЕМЕННЫЕ-----------------

void setup()
{
  Serial.begin(9600);
  delay(3000);
  FastLED.addLeds<WS2811, LED_PIN, GRB>(leds, NUM_LEDS).setCorrection(TypicalLEDStrip);
  FastLED.setBrightness(80);
  pinMode(LED_PIN, OUTPUT);
  button.setTimeout(300);
}
void loop()
{
  buttonTick();
  mainLoop();
}

void mainLoop()
{
  if (ONstate)
  {
    if (millis() - main_timer > MAIN_LOOP)
    {
      switch (currentMode)
      {
      case 0: // RAINBOW
        fullRainbow(10);
        break;
      case 1: // random_burst
        random_burst();
        break;
      case 2: // WHITE
        setAll(255, 255, 255);
        break;
      case 3: // rainbow letters, white contour
        lettersRainbow();
        break;
      case 4: //fade in/out
        rainbow_fade();
        break;
      case 5: // rainbow loop
        rainbow_loop(5);
        break;
      case 6:
        color_bounceFADE();
        break;
      }
    }
  }
}

