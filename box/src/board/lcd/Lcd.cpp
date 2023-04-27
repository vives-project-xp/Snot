#include "Lcd.h"
#include "mbed.h"

Lcd::Lcd(){
   backlight = new PwmOut(PTB19);
   backlight->period(0.001f);
   backlight->write(0.75f);
   lcd = new TextLCD(PTC5, PTC7, PTC0, PTC9, PTC8, PTC1, TextLCD::LCD20x4);
}

void Lcd::setEndScreen(){
    printf("All missions completed!\r\n");
    lcd->cls();
    lcd->locate(1, 0);
    lcd->printf("Missions complete!");
    lcd->locate(2, 1);
    lcd->printf("Congratulations!");
    lcd->locate(4, 3);
    lcd->printf("1Bix44C1EzY");
}

void Lcd::setStartScreen(){
    lcd->cls();
    lcd->locate(3, 1);
    lcd->printf("Vives Citygame");
    lcd->locate(3, 3);
    lcd->printf("Alpha Version!");
}

void Lcd::setMissionScreen(std::string description, int distance, int radius){
    lcd->cls();
    lcd->printf("%s", description.c_str());
    lcd->locate(0,3);
    lcd->printf("Dist: %im Rad:%im", distance, radius);
}

void Lcd::setMessageScreen(std::string message){
    lcd->cls();
    lcd->printf("%s", message.c_str());
}

void Lcd::setTime(int hours, int minutes, int seconds){
    //No implementation
}

void Lcd::eventHandler(char key){
    //No implementation
}

void Lcd::setStatusGps(int gpsFix){
    //No implementation
}
