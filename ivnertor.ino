#include <TFT_eSPI.h>
#include <SPI.h>
#include "grid_icon.h"
#include "house_icon.h"
#include "bat_icon.h"

TFT_eSPI tft = TFT_eSPI();

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
    tft.init();
    tft.setRotation(1);
    tft.fillScreen(TFT_BLACK);

    // Иконки
    tft.drawBitmap(128, 10, grid_icon, 64, 64, TFT_CYAN);
    tft.drawBitmap(256, 100, house_icon, 64, 64, TFT_YELLOW);
    tft.drawBitmap(0, 100, bat_icon, 64, 64, TFT_GREEN);

    // Подписи
    tft.setTextColor(TFT_CYAN, TFT_BLACK);
    tft.setTextSize(2);
    tft.setCursor(115, 10);
    tft.println("AC");

    tft.setTextColor(TFT_YELLOW, TFT_BLACK);
    tft.setCursor(230, 90);
    tft.println("Home");

    tft.setTextColor(TFT_GREEN, TFT_BLACK);
    tft.setCursor(55, 90);
    tft.println("Bat");
}

// ==============================================================

void loop() {
    flowACtoBat(TFT_GREEN, speedACBat);
    flowACtoHome(TFT_YELLOW, speedACHome);
    flowBatToHome(TFT_BLUE, speedBatHome);
}

// ==============================================================

void flowACtoBat(uint16_t color, int speed) {
    unsigned long currentMillis = millis();
    if (currentMillis - lastFlowTimeACBat >= speed) {
        lastFlowTimeACBat = currentMillis;

        if (flowHeight <= (ac_bat - bat_down)) {
            tft.fillRect(ac_bat - flowHeight, 32, 2, 2, color);
            flowHeight += 3;
        } 
        else if (flowWeight <= 50) {
            tft.fillRect(30, 32 + flowWeight, 2, 2, color);
            flowWeight += 3;
        } 
        else {
            // Очистка траектории
            tft.fillRect(27, 32, 105, 2, TFT_BLACK);
            tft.fillRect(30, 32, 2, 68, TFT_BLACK);
            flowHeight = 0;
            flowWeight = 0;
        }
    }
}

// ==============================================================

void flowACtoHome(uint16_t color, int speed) {
    unsigned long currentMillis = millis();
    if (currentMillis - lastFlowTimeACHome >= speed) {
        lastFlowTimeACHome = currentMillis;

        if (flowHeightToHome <= 86) {
            tft.fillRect(196 + flowHeightToHome, 32, 2, 2, color);
            flowHeightToHome += 3;
        } 
        else if (flowWeightToHome <= 50) {
            tft.fillRect(282, 32 + flowWeightToHome, 2, 2, color);
            flowWeightToHome += 3;
        } 
        else {
            // Очистка
            tft.fillRect(192, 32, 90, 2, TFT_BLACK);
            tft.fillRect(282, 32, 2, 58, TFT_BLACK);
            flowHeightToHome = 0;
            flowWeightToHome = 0;
        }
    }
}

// ==============================================================

void flowBatToHome(uint16_t color, int speed) {
    unsigned long currentMillis = millis();
    if (currentMillis - lastFlowTimeBatHome >= speed) {
        lastFlowTimeBatHome = currentMillis;

        if (flowHeightBatToHome <= 182) {
            tft.fillRect(64 + flowHeightBatToHome, 128, 2, 2, color);
            flowHeightBatToHome += 3;
        } 
        else {
            // Очистка линии
            tft.fillRect(64, 128, 182, 2, TFT_BLACK);
            flowHeightBatToHome = 0;
        }
    }
}
