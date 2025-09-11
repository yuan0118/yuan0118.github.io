#pragma once // 防重複引用

#include <Arduino.h> // 引入 Arduino 標頭
#include "config.h" // 引入設定

class LineSensor { // 線感測類別
public:
  void begin(); // 初始化腳位
  uint8_t readBits(); // 讀取五路狀態
  int computeError(); // 計算權重誤差
  bool isAllWhite(); // 是否全白
  bool isAllBlack(); // 是否全黑
private:
  int _pins[5] = {PIN_S1, PIN_S2, PIN_S3, PIN_S4, PIN_S5}; // 腳位陣列
};
