#pragma once // 防重複引用

#include <Arduino.h> // 引入 Arduino 標頭
#include "config.h" // 引入設定

class Sonar { // 超音波感測類別
public:
  void begin(); // 初始化
  void start(); // 開始一次量測
  bool update(); // 更新狀態，完成回傳 true
  bool isBusy(); // 是否忙碌
  float getDistance(); // 取得距離
private:
  enum {IDLE, TRIG_HIGH, WAIT_HIGH, WAIT_RISE, WAIT_FALL} _state; // 狀態
  unsigned long _t; // 時間記錄
  unsigned long _echoStart; // Echo 上升時間
  float _dist; // 最近距離
};
