# ESP32-S3 自走車循跡避障專案

此專案實作以 ESP32-S3 為核心的自走車，可進行循跡與避障。
程式可於 Arduino IDE 2.3.6 搭配 ESP32 Arduino Core 3.3.0 編譯。

## 硬體接線

| 模組 | 腳位 |
| --- | --- |
| L9110S 右 IN1 | GPIO10 |
| L9110S 右 IN2 | GPIO11 |
| L9110S 左 IN1 | GPIO12 |
| L9110S 左 IN2 | GPIO13 |
| TRCT S1~S5 | GPIO4,5,6,7,15 |
| SG90 伺服 | GPIO21 |
| HC-SR04 Trig | GPIO16 |
| HC-SR04 Echo | GPIO17 |

> ⚠️ HC-SR04 Echo 輸出為 5V，需經分壓後再接入 ESP32-S3。

## 程式架構

- `src/main.ino` 主程式與狀態機
- `src/config.h` 參數與腳位設定
- `src/motor.*` L9110S 控制
- `src/pid.*` PID 控制器
- `src/line_sensor.*` 五路循跡感測
- `src/sonar.*` 超音波非阻塞量測
- `src/servo_radar.*` 伺服雷達掃描

## 套件安裝

- [ESP32Servo](https://github.com/madhephaestus/ESP32Servo)（測試版本 0.13.0）

## 編譯

1. 於 Arduino IDE 選擇 **ESP32-S3 Dev Module**
2. 安裝 ESP32 Arduino Core 3.3.0
3. 導入本專案後即可編譯上傳

## 串口指令

- `kp=...` / `ki=...` / `kd=...` 調整 PID 參數
- `base=...` 調整基礎速度
- `go` 開始行駛
- `stop` 停止
- `state?` 查詢目前狀態
