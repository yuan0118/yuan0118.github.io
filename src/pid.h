#pragma once // 防重複引用

#include <Arduino.h> // 引入 Arduino 標頭
#include "config.h" // 引入設定

class PID { // PID 類別
public:
  void begin(float kp, float ki, float kd); // 初始化參數
  void setTunings(float kp, float ki, float kd); // 更新參數
  float compute(float error); // 計算 PID 輸出
private:
  float _kp; // 比例增益
  float _ki; // 積分增益
  float _kd; // 微分增益
  float _iTerm; // 積分項
  float _prevErr; // 前一次誤差
  float _dFilt; // 微分濾波值
  unsigned long _lastT; // 上次時間
};
