#include "servo_radar.h" // 引入雷達頭檔

void ServoRadar::begin(Sonar* s) { // 初始化
  _sonar = s; // 保存超音波指標
  _servo.attach(PIN_SERVO); // 附加伺服
  _angle = RADAR_MIN; // 初始角度
  _servo.write(_angle); // 寫入角度
  _dir = RADAR_STEP; // 掃描方向
  _lastMove = millis(); // 記錄時間
  _idx = 0; // 取樣索引歸零
  _dL = _dC = _dR = -1; // 距離預設
}

void ServoRadar::update() { // 更新函式
  bool done = _sonar->update(); // 更新超音波並取得完成狀態
  if (!_sonar->isBusy() && _idx < N) { // 若可開始量測
    _sonar->start(); // 觸發量測
  }
  if (done) { // 若量測完成
    _samples[_idx++] = _sonar->getDistance(); // 保存距離
  }
  if (_idx >= N) { // 已收集足夠樣本
    float tmp[N]; // 暫存排序
    for (int i = 0; i < N; i++) tmp[i] = _samples[i]; // 複製
    for (int i = 0; i < N - 1; i++) { // 簡單排序
      for (int j = i + 1; j < N; j++) {
        if (tmp[i] > tmp[j]) { float t = tmp[i]; tmp[i] = tmp[j]; tmp[j] = t; } // 交換
      }
    }
    float med = tmp[N / 2]; // 中位數
    if (_angle < 80) _dR = med; // 右扇區
    else if (_angle > 100) _dL = med; // 左扇區
    else _dC = med; // 中央
    _idx = 0; // 重置索引
    unsigned long now = millis(); // 現在時間
    if (now - _lastMove >= SWEEP_T) { // 檢查步進時間
      _angle += _dir; // 角度前進
      if (_angle >= RADAR_MAX) { _angle = RADAR_MAX; _dir = -RADAR_STEP; } // 到達極限反向
      if (_angle <= RADAR_MIN) { _angle = RADAR_MIN; _dir = RADAR_STEP; } // 另一極限
      _servo.write(_angle); // 寫入角度
      _lastMove = now; // 更新時間
    }
  }
}

float ServoRadar::distLeft() { // 取得左距
  return _dL; // 返回
}

float ServoRadar::distCenter() { // 取得中距
  return _dC; // 返回
}

float ServoRadar::distRight() { // 取得右距
  return _dR; // 返回
}
