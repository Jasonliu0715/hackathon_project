#include <Wire.h> 
#include <LiquidCrystal_I2C.h> 

/**
 * 4人抢答器系统 (实物版 - 无 LED)
 */

// --- 配置区 --- 
const int BTN_PINS[] = {2, 3, 4, 7}; // 按钮引脚 
const char* NAMES[] = {"GROUP 1", "GROUP 2", "GROUP 3", "GROUP 4"}; 

// 实物 LCD 通常地址为 0x27
LiquidCrystal_I2C lcd(0x27, 16, 2); 

bool fouled[4] = {false, false, false, false}; // 记录抢跑名单 
int winner = -1;                               // 记录最终赢家 
bool gameStarted = false; 

void setup() { 
  // 1. 初始化按钮引脚 (去除了 LED 相关代码)
  for(int i=0; i<4; i++) { 
    pinMode(BTN_PINS[i], INPUT_PULLUP); 
  } 

  lcd.init(); 
  lcd.backlight(); 
  lcd.clear(); 

  // 2. 5秒倒计时
  for (int i = 5; i > 0; i--) { 
    lcd.setCursor(0, 0); 
    lcd.print("   READY...     "); 
    lcd.setCursor(7, 1); 
    lcd.print(i); 
    
    unsigned long startTime = millis(); 
    while (millis() - startTime < 1000) { 
      for (int j = 0; j < 4; j++) { 
        if (!fouled[j] && digitalRead(BTN_PINS[j]) == LOW) { 
          fouled[j] = true; 
        } 
      } 
    } 
  } 

  // 3. 倒计时结束
  lcd.clear(); 
  lcd.setCursor(5, 0); 
  lcd.print("START!"); 
  gameStarted = true; 

  // 4. 抢答阶段
  while (winner == -1) { 
    for (int i = 0; i < 4; i++) { 
      if (!fouled[i] && digitalRead(BTN_PINS[i]) == LOW) { 
        winner = i; 
        break; 
      } 
    } 
    
    bool allFouled = true; 
    for(int k=0; k<4; k++) if(!fouled[k]) allFouled = false; 
    if(allFouled) break; 
  } 

  // 5. 显示结果 
  displayResults(); 
} 

void displayResults() { 
  // A. 显示赢家 (去除了 LED 点亮逻辑)
  lcd.clear(); 
  if (winner != -1) { 
    lcd.setCursor(0, 0); 
    lcd.print(">> WINNER! <<"); 
    lcd.setCursor(4, 1); 
    lcd.print(NAMES[winner]); 
    delay(4000); 
  } else { 
    lcd.setCursor(0, 0); 
    lcd.print("NO VALID WINNER"); 
    delay(3000); 
  } 

  // B. 公示抢跑名单 
  for (int i = 0; i < 4; i++) { 
    if (fouled[i]) { 
      lcd.clear(); 
      lcd.setCursor(0, 0); 
      lcd.print("EARLY PRESS!"); 
      lcd.setCursor(4, 1); 
      lcd.print(NAMES[i]); 
      delay(2000); 
    } 
  } 

  // C. 游戏结束状态 
  lcd.clear(); 
  lcd.setCursor(3, 0); 
  lcd.print("GAME OVER"); 
  lcd.setCursor(2, 1); 
  lcd.print("Press RESET"); 
} 

void loop() { }



