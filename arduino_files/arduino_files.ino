#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>

#define SCREEN_WIDTH 128 // OLED display width, in pixels
#define SCREEN_HEIGHT 64 // OLED display height, in pixels

#define OLED_RESET     -1 // Reset pin # (or -1 if sharing Arduino reset pin)
#define SCREEN_ADDRESS 0x3C ///< See datasheet for Address; 0x3D for 128x64, 0x3C for 128x32
#define UPDATE_INTERVAL_MS 1000
#define SECONDS_IN_A_DAY 86400
Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, OLED_RESET);
long current_seconds = -1;
int16_t degrees = 248;
int temperature;
int weathercode;
long last_weather_update = millis();;

unsigned long next_date_update = millis();
unsigned long displayStartTime = millis();
bool displayActive = true;



void setup()
{
  Serial.begin(9600);

  if (!display.begin(SSD1306_SWITCHCAPVCC, SCREEN_ADDRESS)) 
  {
    Serial.println(F("SSD1306 allocation failed"));
    for (;;); // Don't proceed, loop forever
  }
  display.display();

  // Clear the buffer
  display.clearDisplay();
  display.setTextColor(SSD1306_WHITE);
  Serial.write("ready");
  pinMode(A0, INPUT);
}

void loop() {
  if (Serial.available() > 0) {
    current_seconds = Serial.readStringUntil(',').toInt();
    temperature = Serial.readStringUntil(',').toInt();
    weathercode = Serial.readStringUntil(',').toInt();
    last_weather_update = millis();

    next_date_update = millis() + UPDATE_INTERVAL_MS;
  }

  unsigned long now = millis();
  if (current_seconds != -1 && next_date_update <= now) {
    next_date_update = now + UPDATE_INTERVAL_MS;
    current_seconds++;
    if (current_seconds >= SECONDS_IN_A_DAY) {
      current_seconds -= SECONDS_IN_A_DAY;
    }
  }

  int val = analogRead(A0);
  if (val > 20) {
    displayEverything();
    displayStartTime = millis();
    displayActive = true;
  }

  if (displayActive && (millis() - displayStartTime >= 5000)) {
    display.clearDisplay();
    display.display();
    Serial.println("closing down");
    displayActive = false; // Reset the display active state
  }
}

void displayEverything() {
  display.clearDisplay();
  drawTime(integersToFormated());
  drawTemp(String(temperature));
  drawWeather();
  display.display();
}

// TODO: Python in background
// TODO: Wind

void drawTime(String text) {
  display.setTextSize(4);
  display.setCursor(0, 0);
  display.println(text);
} 

void drawTemp(String temp) {
  display.setTextSize(2);
  display.cp437(true);
  display.setCursor(30, 40);
  display.println(temp);
  display.setCursor(30 + 12*temp.length(), 40);
  display.write(degrees);
  if (millis() - last_weather_update >= 1000*60*60) {
    display.write("     !");
  }
}

void drawWeather() {
  if (weathercode == 0) {drawClear();}
  else if (weathercode == 1) {drawSemiCloudy();}
  else if (weathercode == 2) {drawCloudy();}
  else if (weathercode == 3) {drawFog();}
  else if (weathercode == 4) {drawRain();}
  else if (weathercode == 5) {drawShowers();}
  else if (weathercode == 6) {drawSnow();}
  else if (weathercode == 7) {drawThunder();}
}

void drawClear() {
  display.drawCircle(14, 49, 9, SSD1306_WHITE);
}

void drawSemiCloudy() {
  // Sun
  display.drawCircle(14, 49, 9, SSD1306_WHITE);
  
  // Cloud
  display.fillCircle(12, 55, 2, SSD1306_WHITE);
  display.fillCircle(19, 55, 2, SSD1306_WHITE);
  display.fillCircle(15, 52, 2, SSD1306_WHITE);
  display.drawPixel(13, 56, SSD1306_WHITE);
  display.drawPixel(13, 57, SSD1306_WHITE);
  display.drawPixel(14, 57, SSD1306_WHITE);
  display.drawPixel(15, 57, SSD1306_WHITE);
  display.drawPixel(16, 57, SSD1306_WHITE);
  display.drawPixel(17, 57, SSD1306_WHITE);
  display.drawPixel(18, 57, SSD1306_WHITE);
  display.drawPixel(18, 56, SSD1306_WHITE);
  display.drawPixel(17, 56, SSD1306_WHITE);
  display.drawPixel(16, 56, SSD1306_WHITE);
  display.drawPixel(15, 56, SSD1306_WHITE);
  display.drawPixel(14, 56, SSD1306_WHITE);
  display.drawPixel(15, 55, SSD1306_WHITE);
  display.drawPixel(16, 55, SSD1306_WHITE);
}

void drawCloudy() {
  display.fillCircle(18, 53, 4, SSD1306_WHITE);
  display.fillCircle(10, 51, 4, SSD1306_WHITE);
  display.fillCircle(15, 47, 4, SSD1306_WHITE);
}

void drawFog() {
  display.drawLine(6, 56, 7, 56, SSD1306_WHITE);
  display.drawLine(10, 54, 11, 54, SSD1306_WHITE);
  display.drawLine(14, 56, 15, 56, SSD1306_WHITE);
  display.drawLine(18, 54, 19, 54, SSD1306_WHITE);
  display.drawLine(14, 52, 15, 52, SSD1306_WHITE);
  display.drawLine(6, 52, 7, 52, SSD1306_WHITE);
  display.drawLine(22, 52, 23, 52, SSD1306_WHITE);
  display.drawLine(18, 50, 19, 50, SSD1306_WHITE);
  display.drawLine(22, 48, 23, 48, SSD1306_WHITE);
  display.drawLine(14, 48, 15, 48, SSD1306_WHITE);
  display.drawLine(10, 50, 11, 50, SSD1306_WHITE);
  display.drawLine(6, 48, 7, 48, SSD1306_WHITE);
  display.drawLine(10, 46, 11, 46, SSD1306_WHITE);
  display.drawLine(18, 46, 19, 46, SSD1306_WHITE);
  display.drawLine(22, 44, 23, 44, SSD1306_WHITE);
  display.drawLine(14, 44, 15, 44, SSD1306_WHITE);
  display.drawLine(6, 44, 7, 44, SSD1306_WHITE);
  display.drawLine(10, 42, 11, 42, SSD1306_WHITE);
  display.drawLine(18, 42, 19, 42, SSD1306_WHITE);
  display.drawLine(22, 56, 23, 56, SSD1306_WHITE);
}

void drawShowers() {
  display.drawLine(10, 45, 9, 52, SSD1306_WHITE);
  display.drawLine(7, 51, 6, 56, SSD1306_WHITE);
  display.drawLine(11, 57, 12, 52, SSD1306_WHITE);
  display.drawLine(16, 56, 18, 50, SSD1306_WHITE);
  display.drawLine(20, 57, 21, 53, SSD1306_WHITE);
  display.drawLine(15, 49, 17, 44, SSD1306_WHITE);
  display.drawLine(19, 46, 22, 41, SSD1306_WHITE);
  display.drawLine(13, 46, 15, 41, SSD1306_WHITE);
  display.drawLine(6, 46, 8, 41, SSD1306_WHITE);
}

void drawRain() {
  display.drawLine(6, 56, 6, 54, SSD1306_WHITE);
  display.drawLine(16, 57, 16, 55, SSD1306_WHITE);
  display.drawLine(21, 55, 21, 53, SSD1306_WHITE);
  display.drawLine(18, 50, 18, 48, SSD1306_WHITE);
  display.drawLine(20, 45, 20, 43, SSD1306_WHITE);
  display.drawLine(6, 45, 6, 43, SSD1306_WHITE);
  display.drawLine(11, 55, 11, 52, SSD1306_WHITE);
  display.drawLine(13, 41, 13, 44, SSD1306_WHITE);
  display.drawLine(9, 49, 9, 47, SSD1306_WHITE);
}

void drawSnow() {
  display.drawLine(14, 39, 14, 59, SSD1306_WHITE);
  display.drawLine(6, 53, 22, 45, SSD1306_WHITE);
  display.drawLine(6, 45, 22, 54, SSD1306_WHITE);
  display.drawLine(10, 41, 14, 44, SSD1306_WHITE);
  display.drawLine(14, 44, 18, 41, SSD1306_WHITE);
  display.drawLine(19, 47, 20, 42, SSD1306_WHITE);
  display.drawLine(19, 47, 22, 49, SSD1306_WHITE);
  display.drawLine(18, 52, 22, 50, SSD1306_WHITE);
  display.drawLine(18, 52, 18, 56, SSD1306_WHITE);
  display.drawLine(14, 54, 17, 57, SSD1306_WHITE);
  display.drawLine(14, 54, 10, 57, SSD1306_WHITE);
  display.drawLine(9, 52, 9, 56, SSD1306_WHITE);
  display.drawLine(9, 52, 6, 49, SSD1306_WHITE);
  display.drawLine(9, 47, 6, 49, SSD1306_WHITE);
  display.drawLine(10, 47, 9, 42, SSD1306_WHITE);
}

void drawThunder() {
  display.fillTriangle(14, 48, 19, 48, 11, 55, SSD1306_WHITE);
  display.fillTriangle(11, 44, 8, 50, 15, 50, SSD1306_WHITE);
  display.fillTriangle(11, 44, 16, 44, 14, 48, SSD1306_WHITE);
}

String integersToFormated() {
  double hours_int = current_seconds/(60.0*60.0);
  int minutes_int = (hours_int - floor(hours_int))*60;
  int seconds_int = current_seconds % 60;

  String seconds_string = String(seconds_int);
  String minutes_string = String(minutes_int);
  String hours_string = String(int(hours_int));
  if (hours_string.length() == 1) {
    hours_string = "0" + hours_string;
  }
  if (minutes_string.length() == 1) {
    minutes_string = "0" + minutes_string;
  }
  return hours_string + ":" + minutes_string;
}

