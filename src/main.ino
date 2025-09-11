#include "config.h" // 引入設定檔
#include "motor.h" // 引入馬達
#include "pid.h" // 引入 PID
#include "line_sensor.h" // 引入循跡
#include "sonar.h" // 引入超音波
#include "servo_radar.h" // 引入雷達

MotorDriver motor; // 建立馬達物件
PID pid; // 建立 PID 物件
LineSensor lineSensor; // 建立循跡感測
Sonar sonar; // 建立超音波
ServoRadar radar; // 建立雷達

RobotState state = STATE_STOP; // 初始狀態為停車
unsigned long loseT = 0; // 失線開始時間
unsigned long recoveryStart = 0; // 自救開始時間
unsigned long telT = 0; // 遙測時間戳

String cmd = ""; // 串口指令緩衝

void setup() { // 設定函式
  Serial.begin(115200); // 啟動串口
  motor.begin(); // 初始化馬達
  lineSensor.begin(); // 初始化循跡
  sonar.begin(); // 初始化超音波
  radar.begin(&sonar); // 初始化雷達
  pid.begin(KP, KI, KD); // 初始化 PID
  Serial.println("calibrating..."); // 顯示校準
  delay(3000); // 3 秒校準
  state = STATE_LINE_FOLLOW; // 進入循跡
}

void loop() { // 主迴圈
  unsigned long now = millis(); // 取得現在時間
  radar.update(); // 更新雷達
  uint8_t bits = lineSensor.readBits(); // 讀取循跡位元
  int error = lineSensor.computeError(); // 計算誤差
  bool allWhite = lineSensor.isAllWhite(); // 是否全白
  bool allBlack = lineSensor.isAllBlack(); // 是否全黑
  float dL = radar.distLeft(); // 左距
  float dC = radar.distCenter(); // 中距
  float dR = radar.distRight(); // 右距

  while (Serial.available()) { // 處理串口
    char c = Serial.read(); // 讀取字元
    if (c == '\n' || c == '\r') { // 判斷結束
      if (cmd.length()) { // 若有內容
        if (cmd.startsWith("kp=")) { KP = cmd.substring(3).toFloat(); pid.setTunings(KP, KI, KD); } // 設 kp
        else if (cmd.startsWith("ki=")) { KI = cmd.substring(3).toFloat(); pid.setTunings(KP, KI, KD); } // 設 ki
        else if (cmd.startsWith("kd=")) { KD = cmd.substring(3).toFloat(); pid.setTunings(KP, KI, KD); } // 設 kd
        else if (cmd.startsWith("base=")) { BASE_SPEED = cmd.substring(5).toInt(); } // 設基速
        else if (cmd == "go") { state = STATE_LINE_FOLLOW; } // 開始
        else if (cmd == "stop") { state = STATE_STOP; motor.brake(); } // 停止
        else if (cmd == "state?") { Serial.printf("state=%d\n", state); } // 查狀態
      }
      cmd = ""; // 清空指令
    } else { cmd += c; } // 累加字元
  }

  if (state == STATE_LINE_FOLLOW) { // 在循跡狀態
    if (dC > 0 && dC < D_STOP) { state = STATE_AVOID; } // 障礙優先
    else if (allWhite || allBlack) { // 失線
      if (loseT == 0) loseT = now; // 記錄開始
      else if (now - loseT > RECOVERY_T) { state = STATE_RECOVERY; recoveryStart = now; } // 進入自救
    } else loseT = 0; // 重置
  }
  if (state == STATE_AVOID) { // 避障狀態
    if (dC > D_STOP) state = STATE_LINE_FOLLOW; // 清場回循跡
  }
  if (state == STATE_RECOVERY) { // 自救狀態
    if (!(allWhite || allBlack)) { state = STATE_LINE_FOLLOW; loseT = 0; } // 找回線
    else if (now - recoveryStart > 2000) { state = STATE_STOP; motor.brake(); } // 超時停車
  }

  static float u = 0; // 控制量
  int L = 0; // 左輸出
  int R = 0; // 右輸出
  switch (state) { // 依狀態執行
    case STATE_LINE_FOLLOW: // 循跡
      u = pid.compute(error); // 計算 PID
      L = BASE_SPEED - (int)u; // 左速度
      R = BASE_SPEED + (int)u; // 右速度
      L = constrain(L, 0, 1023); // 限制範圍
      R = constrain(R, 0, 1023); // 限制範圍
      motor.drive(L, R); // 驅動
      break; // 結束
    case STATE_AVOID: // 避障
      if (dL > dR) motor.drive(-BASE_SPEED, BASE_SPEED); // 左轉
      else motor.drive(BASE_SPEED, -BASE_SPEED); // 右轉
      break; // 結束
    case STATE_RECOVERY: // 自救
      if (now - recoveryStart < 500) { // 前 0.5 秒
        motor.drive(-BASE_SPEED, -BASE_SPEED); // 後退
      } else { // 之後
        if (((now - recoveryStart) / 300) % 2 == 0) motor.drive(BASE_SPEED, -BASE_SPEED); // 左微掃
        else motor.drive(-BASE_SPEED, BASE_SPEED); // 右微掃
      }
      break; // 結束
    default: // 停車
      motor.brake(); // 煞車
      break; // 結束
  }
  if (now - telT >= TEL_INTERVAL) { // 遙測間隔
    telT = now; // 更新時間
    Serial.printf("state=%d bits=%02X err=%d u=%.1f L=%d R=%d dL=%.1f dC=%.1f dR=%.1f\n",
                  state, bits, error, u, L, R, dL, dC, dR); // 輸出
  }
}
