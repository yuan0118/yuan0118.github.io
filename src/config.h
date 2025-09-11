#pragma once // 防重複引用

#include <Arduino.h> // 引入 Arduino 標頭

// L9110S 馬達驅動腳位
const int PIN_R_IN1 = 10; // 右輪 IN1
const int PIN_R_IN2 = 11; // 右輪 IN2
const int PIN_L_IN1 = 12; // 左輪 IN1
const int PIN_L_IN2 = 13; // 左輪 IN2

// LEDC 通道設定
const int CH_R_IN1 = 0; // LEDC 通道 0
const int CH_R_IN2 = 1; // LEDC 通道 1
const int CH_L_IN1 = 2; // LEDC 通道 2
const int CH_L_IN2 = 3; // LEDC 通道 3

// TRCT 五路循跡感測腳位
const int PIN_S1 = 4;  // 感測器 1
const int PIN_S2 = 5;  // 感測器 2
const int PIN_S3 = 6;  // 感測器 3
const int PIN_S4 = 7;  // 感測器 4
const int PIN_S5 = 15; // 感測器 5

// 伺服與超音波腳位
const int PIN_SERVO = 21; // SG90 伺服
const int PIN_TRIG  = 16; // HC-SR04 Trig
const int PIN_ECHO  = 17; // HC-SR04 Echo

// PWM 參數
const int PWM_FREQ = 15000; // 馬達 PWM 頻率 15kHz
const int PWM_RES  = 10;    // PWM 解析度 10-bit

// 基礎速度與 PID 參數
int BASE_SPEED = 460;       // 巡航基礎速度，可串口調整
float KP = 1.10f;           // 比例增益
float KI = 0.02f;           // 積分增益
float KD = 10.0f;           // 微分增益
const int OUT_MIN = -500;   // 控制輸出下限
const int OUT_MAX =  500;   // 控制輸出上限
const int I_MIN = -220;     // 積分下限
const int I_MAX =  220;     // 積分上限
const float DERIV_LPF_ALPHA = 0.25f; // 微分低通係數

// 其他參數
const unsigned long RECOVERY_T = 350; // 失線判定時間 ms
const float D_STOP = 18.0f;            // 停止距離 cm
const int RADAR_MIN = 40;              // 雷達掃描最小角度
const int RADAR_MAX = 140;             // 雷達掃描最大角度
const int RADAR_STEP = 10;             // 雷達掃描步進角度
const unsigned long SWEEP_T = 25;      // 每步掃描時間 ms
const unsigned long ECHO_TIMEOUT = 35; // 超音波回應逾時 ms
const unsigned long TEL_INTERVAL = 100; // 遙測輸出間隔 ms

// 狀態列舉
enum RobotState {
  STATE_LINE_FOLLOW, // 循跡狀態
  STATE_AVOID,       // 避障狀態
  STATE_RECOVERY,    // 自救狀態
  STATE_STOP         // 停車狀態
};
