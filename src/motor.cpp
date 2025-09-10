#include "motor.h" // 引入馬達頭檔

void MotorDriver::begin() { // 初始化函式
  ledcAttach(PIN_R_IN1, CH_R_IN1); // 右 IN1 附加 LEDC 通道
  ledcAttach(PIN_R_IN2, CH_R_IN2); // 右 IN2 附加 LEDC 通道
  ledcAttach(PIN_L_IN1, CH_L_IN1); // 左 IN1 附加 LEDC 通道
  ledcAttach(PIN_L_IN2, CH_L_IN2); // 左 IN2 附加 LEDC 通道
  ledcChangeFrequency(CH_R_IN1, PWM_FREQ, PWM_RES); // 設定通道頻率與解析度
  ledcChangeFrequency(CH_R_IN2, PWM_FREQ, PWM_RES); // 同上
  ledcChangeFrequency(CH_L_IN1, PWM_FREQ, PWM_RES); // 同上
  ledcChangeFrequency(CH_L_IN2, PWM_FREQ, PWM_RES); // 同上
}

void MotorDriver::drive(int left, int right) { // 馬達驅動
  left = constrain(left, -1023, 1023); // 限制左輸入
  right = constrain(right, -1023, 1023); // 限制右輸入
  int l = compensate(abs(left)); // 左輪補償
  int r = compensate(abs(right)); // 右輪補償
  if (left >= 0) { // 左輪前進
    ledcWrite(CH_L_IN1, l); // IN1 輸出 PWM
    ledcWrite(CH_L_IN2, 0); // IN2 關閉
  } else { // 左輪後退
    ledcWrite(CH_L_IN1, 0); // IN1 關閉
    ledcWrite(CH_L_IN2, l); // IN2 輸出 PWM
  }
  if (right >= 0) { // 右輪前進
    ledcWrite(CH_R_IN1, r); // IN1 輸出 PWM
    ledcWrite(CH_R_IN2, 0); // IN2 關閉
  } else { // 右輪後退
    ledcWrite(CH_R_IN1, 0); // IN1 關閉
    ledcWrite(CH_R_IN2, r); // IN2 輸出 PWM
  }
}

void MotorDriver::brake() { // 煞車函式
  ledcWrite(CH_L_IN1, 0); // 左 IN1 關閉
  ledcWrite(CH_L_IN2, 0); // 左 IN2 關閉
  ledcWrite(CH_R_IN1, 0); // 右 IN1 關閉
  ledcWrite(CH_R_IN2, 0); // 右 IN2 關閉
}

int MotorDriver::compensate(int pwm) { // 低速補償
  if (pwm == 0) return 0; // 若為零直接返回
  const int MIN_DUTY = 150; // 最小啟轉占空比
  return max(pwm, MIN_DUTY); // 取較大值
}
