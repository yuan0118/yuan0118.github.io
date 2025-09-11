#pragma once // 防重複引用

#include <Arduino.h> // 引入 Arduino 標頭
#include <ESP32Servo.h> // 引入伺服庫
#include "config.h" // 引入設定
#include "sonar.h" // 引入超音波

class ServoRadar { // 伺服雷達類別
public:
  void begin(Sonar* s); // 初始化並綁定超音波
  void update(); // 更新掃描
  float distLeft(); // 左側距離
  float distCenter(); // 中間距離
  float distRight(); // 右側距離
private:
  Servo _servo; // 伺服物件
  Sonar* _sonar; // 超音波指標
  int _angle; // 當前角度
  int _dir; // 掃描方向
  unsigned long _lastMove; // 上次移動時間
  static const int N = 3; // 每點取樣次數
  float _samples[N]; // 取樣暫存
  int _idx; // 取樣索引
  float _dL, _dC, _dR; // 三扇區距離
};
