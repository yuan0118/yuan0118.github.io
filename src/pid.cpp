#include "pid.h" // 引入 PID 頭檔

void PID::begin(float kp, float ki, float kd) { // 初始化函式
  setTunings(kp, ki, kd); // 設定初始參數
  _iTerm = 0; // 積分項清零
  _prevErr = 0; // 前誤差清零
  _dFilt = 0; // 微分濾波清零
  _lastT = millis(); // 記錄時間
}

void PID::setTunings(float kp, float ki, float kd) { // 更新參數
  _kp = kp; // 設定比例
  _ki = ki; // 設定積分
  _kd = kd; // 設定微分
}

float PID::compute(float error) { // 計算函式
  unsigned long now = millis(); // 取得現在時間
  float dt = (now - _lastT) / 1000.0f; // 轉換為秒
  _lastT = now; // 更新時間

  _iTerm += error * _ki * dt; // 積分累加
  _iTerm = constrain(_iTerm, I_MIN, I_MAX); // 積分限幅

  float deriv = (error - _prevErr) / dt; // 微分
  _dFilt = DERIV_LPF_ALPHA * deriv + (1 - DERIV_LPF_ALPHA) * _dFilt; // 低通濾波
  _prevErr = error; // 保存誤差

  float out = _kp * error + _iTerm + _kd * _dFilt; // PID 組合
  out = constrain(out, OUT_MIN, OUT_MAX); // 輸出限幅
  return out; // 返回結果
}
