#include "sonar.h" // 引入超音波頭檔

void Sonar::begin() { // 初始化函式
  pinMode(PIN_TRIG, OUTPUT); // Trig 腳輸出
  pinMode(PIN_ECHO, INPUT);  // Echo 腳輸入
  digitalWrite(PIN_TRIG, LOW); // 初始低電位
  _state = IDLE; // 狀態為待命
  _dist = -1; // 距離預設
}

void Sonar::start() { // 開始量測
  if (_state != IDLE) return; // 忙碌則返回
  _t = micros(); // 記錄時間
  _state = TRIG_HIGH; // 進入觸發高電位
}

bool Sonar::update() { // 更新狀態
  unsigned long now = micros(); // 取得現在時間
  switch (_state) { // 依狀態判斷
    case IDLE: // 待命
      break; // 無動作
    case TRIG_HIGH: // Trig 高電位準備
      if (now - _t >= 2) { // 維持 2 微秒
        digitalWrite(PIN_TRIG, HIGH); // 設為高
        _t = now; // 更新時間
        _state = WAIT_HIGH; // 下一狀態
      }
      break; // 跳出
    case WAIT_HIGH: // 等待 10 微秒
      if (now - _t >= 10) { // 維持 10 微秒
        digitalWrite(PIN_TRIG, LOW); // 拉低
        _t = now; // 更新時間
        _state = WAIT_RISE; // 等待 Echo 上升
      }
      break; // 跳出
    case WAIT_RISE: // 等待上升沿
      if (digitalRead(PIN_ECHO)) { // Echo 為高
        _echoStart = micros(); // 紀錄開始
        _state = WAIT_FALL; // 等待下降
      } else if (now - _t > ECHO_TIMEOUT * 1000) { // 逾時
        _dist = -1; // 無效距離
        _state = IDLE; // 回到待命
        return true; // 回傳完成
      }
      break; // 跳出
    case WAIT_FALL: // 等待下降沿
      if (!digitalRead(PIN_ECHO)) { // Echo 變低
        unsigned long dur = micros() - _echoStart; // 計算脈衝
        _dist = dur * 0.0343f / 2.0f; // 轉換為 cm
        _state = IDLE; // 回到待命
        return true; // 完成
      } else if (micros() - _echoStart > ECHO_TIMEOUT * 1000) { // 逾時
        _dist = -1; // 無效
        _state = IDLE; // 回待命
        return true; // 完成
      }
      break; // 跳出
  }
  return false; // 尚未完成
}

bool Sonar::isBusy() { // 是否忙碌
  return _state != IDLE; // 判斷狀態
}

float Sonar::getDistance() { // 取得距離
  return _dist; // 返回距離
}
