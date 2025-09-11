#include "line_sensor.h" // 引入線感測頭檔

void LineSensor::begin() { // 初始化函式
  for (int i = 0; i < 5; i++) { // 迴圈設定腳位
    pinMode(_pins[i], INPUT); // 設定為輸入
  }
}

uint8_t LineSensor::readBits() { // 讀取感測值
  uint8_t bits = 0; // 初始位元
  for (int i = 0; i < 5; i++) { // 逐一讀取
    bits |= (!digitalRead(_pins[i]) << i); // 低電位視為線，放入對應位
  }
  return bits; // 返回位元
}

int LineSensor::computeError() { // 計算誤差
  int weights[5] = {-2, -1, 0, 1, 2}; // 權重陣列
  int sum = 0; // 加權總和
  int cnt = 0; // 線數量
  for (int i = 0; i < 5; i++) { // 逐一檢查
    int val = !digitalRead(_pins[i]); // 取得線狀態
    if (val) { // 若偵測到線
      sum += weights[i]; // 加權累加
      cnt++; // 計數
    }
  }
  if (cnt == 0) return 0; // 若無線回傳 0
  return sum / cnt; // 返回平均
}

bool LineSensor::isAllWhite() { // 判斷全白
  for (int i = 0; i < 5; i++) { // 逐一檢查
    if (!digitalRead(_pins[i])) return false; // 有線則非全白
  }
  return true; // 全白
}

bool LineSensor::isAllBlack() { // 判斷全黑
  for (int i = 0; i < 5; i++) { // 逐一檢查
    if (digitalRead(_pins[i])) return false; // 有白則非全黑
  }
  return true; // 全黑
}
