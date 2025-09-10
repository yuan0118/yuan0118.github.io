#pragma once // 防重複引用

#include <Arduino.h> // 引入 Arduino 標頭
#include "config.h" // 引入設定

class MotorDriver { // 馬達驅動類別
public:
  void begin(); // 初始化 LEDC
  void drive(int left, int right); // 依左/右速度驅動
  void brake(); // 煞車
private:
  int compensate(int pwm); // 低速補償
};
