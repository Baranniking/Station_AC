#include <TFT_eSPI.h>
#include <SPI.h>
#include <XPT2046_Touchscreen.h>
#include "grid_icon.h"
#include "house_icon.h"
#include "bat_icon.h"

#define TOUCH_CS 33
XPT2046_Touchscreen ts(TOUCH_CS);

TFT_eSPI tft = TFT_eSPI();

// флаги для меню
bool onGridToHome = false;
bool OnOffAC = false;
// переменные для цвета иконок
 int color_ac = TFT_RED;
 int color_bat = TFT_GREEN;
 int color_home =  TFT_GOLD;


// ------------------- КООРДИНАТЫ И ПАРАМЕТРЫ -------------------
int ac_bat = 120;   // длина шкалы от сети до аккум (горизонталь)
int ac_batW = 68;   // длина шкалы от сети до аккум (вертикаль)
int bat_down = 32;

// ------------------- АНИМАЦИОННЫЕ ПЕРЕМЕННЫЕ -------------------
int flowHeight = 0;
int flowWeight = 0;
int flowHeightToHome = 0;
int flowWeightToHome = 0;
int flowHeightBatToHome = 0;
int flowHeightBatToHomeEnd = 0;

bool flowBatToHomeEnd = false;
bool flowActoBatEnd = false;
bool flowACtoHomeEnd = false;

// ------------------- ТАЙМЕРЫ ДЛЯ millis() -------------------
unsigned long lastFlowTimeACBat = 0;
unsigned long lastFlowTimeACHome = 0;
unsigned long lastFlowTimeBatHome = 0;

// ------------------- СКОРОСТЬ АНИМАЦИИ (мс) -------------------
int speedACBat = 80;
int speedACHome = 60;
int speedBatHome = 38;

// ==============================================================

void setup() {
    Serial.begin(115200);
    
    ts.begin();
    ts.setRotation(1);

    tft.init();
    tft.setRotation(1);
    tft.fillScreen(TFT_BLACK);

 
}

// ==============================================================

void loop() {
    
 mainMenu(color_ac, color_bat, color_home);
 
 if(OnOffAC){
    color_ac = TFT_BLUE;
 }
 if(!OnOffAC){
    color_ac = TFT_RED;
 }
 if(onGridToHome && OnOffAC){
    flowACtoHome(TFT_RED, 100);
 }
 
}

// =============================================================




//------ Главное меню
    void mainMenu(int color_ac, int color_bat, int color_home){
    // Иконки
    tft.drawBitmap(128, 10, grid_icon, 64, 64, color_ac);
    tft.drawBitmap(256, 100, house_icon, 64, 64, color_home);
    tft.drawBitmap(0, 100, bat_icon, 64, 64, color_bat);

    // Подписи
    tft.setTextColor(color_ac, TFT_BLACK);
    tft.setTextSize(2);
    tft.setCursor(115, 10);
    tft.println("AC");

    tft.setTextColor(color_home, TFT_BLACK);
    tft.setCursor(230, 90);
    tft.println("Home");

    tft.setTextColor(color_bat, TFT_BLACK);
    tft.setCursor(55, 90);
    tft.println("Bat");
   
   // обработка касания
     TS_Point p;
     if(ts.touched()){ //определяем касание
     p = ts.getPoint(); //получаем координаты
     }
     if(p.z > 200){
        int x = map(p.x, 3600, 500, 0, 320);
        int y = map(p.y, 3500, 500, 0, 240);
    
        Serial.print("Touch at: ");
        Serial.print("x = ");
        Serial.print(x);
        Serial.print(", y = ");
        Serial.println(y);
     // ------обработка нажатия на иконку електростолб
    if(x >= 118 && x<= 200 && y >= 10 && y <= 80){ 
        OnOffAC = !OnOffAC;
        flowHeightToHome = 0; //сбро счетчика анимации перетока
        flowWeightToHome = 0;
         tft.fillRect(196, 32, 90, 2, TFT_BLACK);
         tft.fillRect(286, 32, 2, 50, TFT_BLACK);
       
     //-------обработка нажатия на иконку дом
    }
    if(x >= 250 && x <= 320 && y >= 90 && y <= 170){ 
        onGridToHome = !onGridToHome;
        flowHeightToHome = 0; //сбро счетчика анимации перетока
        flowWeightToHome = 0;
         tft.fillRect(196, 32, 90, 2, TFT_BLACK);
         tft.fillRect(286, 32, 2, 50, TFT_BLACK);
    }
 
    delay(300);
    }

    
}



// ==============================================================
//переток от сети к батареи
void flowACtoBat(uint16_t color, int speed) {
    unsigned long currentMillis = millis();
    if (currentMillis - lastFlowTimeACBat >= speed) {
        lastFlowTimeACBat = currentMillis;

        if (flowHeight <= (ac_bat - bat_down) && !flowActoBatEnd) {
            tft.fillRect(ac_bat - flowHeight, 32, 2, 2, color);
            flowHeight += 6;
        } 
        else if (flowWeight <= 50 && !flowActoBatEnd) {
            tft.fillRect(30, 32 + flowWeight, 2, 2, color);
            flowWeight += 6;
        } 
        else if (flowWeight >=50 && !flowActoBatEnd){
            flowHeight = 0;
            flowWeight = 0;
            flowActoBatEnd = true;
        }
        else if(flowHeight <= (ac_bat - bat_down) && flowActoBatEnd){       
             tft.fillRect(ac_bat - flowHeight, 32, 2, 2, TFT_RED);
            flowHeight += 6;
        } 
        else if(flowWeight <=50 && flowActoBatEnd){
            tft.fillRect(286, 32 + flowWeight, 2, 2, TFT_RED);
            flowWeight += 6;
        }
        else {
            flowHeight = 0;
            flowWeight = 0;
            flowActoBatEnd = false;
        }
        }
    }


// ==============================================================
//переток от сети к дома
void flowACtoHome(uint16_t color, int speed) {
    unsigned long currentMillis = millis();
    if (currentMillis - lastFlowTimeACHome >= speed) {
        lastFlowTimeACHome = currentMillis;

        if (flowHeightToHome <= 90 && !flowACtoHomeEnd) {
            tft.fillRect(196 + flowHeightToHome, 32, 2, 2, color);
            flowHeightToHome += 6;
        } 
        else if (flowWeightToHome <= 50 && !flowACtoHomeEnd) {
            tft.fillRect(286, 32 + flowWeightToHome, 2, 2, color);
            flowWeightToHome += 6;
        }
        else if (flowWeightToHome >= 50 && !flowACtoHomeEnd){
            flowHeightToHome = 0;
            flowWeightToHome = 0;
            flowACtoHomeEnd = true;
        }
        else if(flowHeightToHome <= 90 && flowACtoHomeEnd){
            tft.fillRect(196 + flowHeightToHome, 32, 2, 2, TFT_BLUE);
            flowHeightToHome += 6;
        }
        else if(flowWeightToHome <= 50 && flowACtoHomeEnd){
            tft.fillRect(286, 32 + flowWeightToHome, 2, 2, TFT_BLUE);
            flowWeightToHome += 6;
        }
        else {
            flowACtoHomeEnd = false;
            flowHeightToHome = 0;
            flowWeightToHome = 0;
        }
    }
}

// ==============================================================
//переток от батареи в дом
void flowBatToHome(uint16_t color, int speed) {
    unsigned long currentMillis = millis();
    if (currentMillis - lastFlowTimeBatHome >= speed) {
        lastFlowTimeBatHome = currentMillis;

        if (flowHeightBatToHome <= 182 && !flowBatToHomeEnd) {
            tft.fillRect(64 + flowHeightBatToHome, 128, 2, 2, color);
            flowHeightBatToHome += 6;
        } 
        else if(flowHeightBatToHome >=182 && !flowBatToHomeEnd){
            flowBatToHomeEnd = true;
            flowHeightBatToHome = 0;
        }
        else if (flowHeightBatToHome <= 182 && flowBatToHomeEnd) {
            // Очистка линии
            tft.fillRect(64 + flowHeightBatToHome, 128, 2, 2, TFT_GREEN);
            flowHeightBatToHome += 6;
        }
        else{
            flowBatToHomeEnd = false;
            flowHeightBatToHome = 0;
        }
    }
}

//================================================================

