#include "stdio.h"
#include "math.h"
#include "system_sam3x.h"
#include "at91sam3x8.h"
#include "core_cm3.h"
#include "display_keypad.h"
#include "easyWriting.h"
#include "temp.h"
#include "light.h"
#include "servo.h"
#include "initialize.h"
#include "UI.h"

int button = 0;
int lastButton;
int input = 12;
int unsigned menulvl = 0;

int j = 0;
int i = 0;

int counter = 0;
int logCounter = 0;
int servoCounter = 0;

void minOrMax(float temperature, int x){
  if(min[x]==0 && max[x]==0){
    min[x] = temperature;
    max[x] = temperature;
  }
  else if(temperature<min[x]){
    min[x] = temperature;
  }
  else if(temperature>max[x]){
    max[x] = temperature;
  }
}

void increaseAvg(float temperature, int x, int y){
  if(avg[x] == 0){
    avg[x] = temperature;
  }
  else{
    avg[x] = (avg[x]*(y-1) + temperature)/y;
  }
}

void SysTick_Handler(void){
  counter++;
  logCounter++;
  servoCounter++;
}

void main(){
  SystemInit();
  SysTick_Config(84000);
  initialize();
  Init_Display();
  while(1){
    if(counter>=500){
      startPulse();
      if(tempDone == 1){
        write_temp();
        if(j==1440){
          j = 0;
          i++;
        }
        if(i==7){
          i = 0;
        }
        if(logOn==1){
          if(speedyMode==1){
            theLog[i][j] = temp;
            minOrMax(temp, i);
            increaseAvg(temp, i, j);
            j++;
          }
          else if(logCounter>=6000){
            theLog[i][j] = temp;
            minOrMax(temp, i);
            j++;
            logCounter = 0;
          }
        }
        if(upperLimitSet == 1 && temp>=upperLimit){
          rowSelect(1);
          write(35, high);
        }
        else if(lowerLimitSet == 1 && temp<=lowerLimit){
          rowSelect(1);
          write(34, low);
        }
      }
      counter = 0;
    }
    
    button = buttonPressed();
    if(((button != 0 && menulvl == 0) || (button == 12 && menulvl == 1)) && button != lastButton){ //IF (button is pressed when in main-menu OR "#" is pressed when in sub-menu) AND button isn't same as last run
      input = button;
    }
    mainMenuSwitchCase(input);
    
    if(button==12 && button!=lastButton){
      clearRow(1);
    }
    lastButton = button;     //Save last button pressed so it doesn't repeat
  }
}
