extern unsigned int menulvl;
extern int button;
extern int input;
extern int servoCounter;
extern int lastButton;
extern int highAdress;

//-----------------PRINTABLE TEXT-----------------------------------------------
char menu[21]   = "---------MENU--------";
char logg[6]    = "1. Log";
char sun[11]    = "2. Find sun";
char alarm1[11] = "3. Alarm(+)";
char alarm2[11] = "4. Alarm(-)";
char logging[12]= "5. Logging: ";
char fast[14]   = "6. Fast mode: ";

char NA[13]     = "Not available";
char ON[3]      = "ON ";
char OFF[3]     = "OFF";

char logHeader[33]         = "Day       Min       Avg       Max";
char logDaysPrint[7][10] = {"Mon       ", "Tue       ", "Wen       ", "Thu       ", "Fri       ", "Sat       ", "Sun       "};

char retur[31]  = "To return to previous press '#'";
char degree[9]  = "Degrees: ";
char high[35]   = "Alarm for high temperature triggerd";
char low[34]    = "Alarm for low temperature triggerd";
char invalid[13]= "Invalid limit";
char deg[3];   // This will be used to write a variable which stores rotation

char alarmHeader[25] = "Choose which alarm to set";
char highAlarm[17]   = "1. Set high alarm";
char setHigh1[19]    = "High limit(0-125): ";
char setHigh2[19]    = "High limit(-40-0):-";
char lowAlarm[16]    = "2. Set low alarm";
char setLow1[18]     = "Low limit(0-125): ";
char setLow2[18]     = "Low limit(-40-0):-";
char inputDone[30]   = "When done with input press '*'";
char AlarmAt[27]     = "Alarm will be triggered at:";
char digitChar[1];    //Will be used to print a digit corresponding the button pressed
char numAsCharArr[3]; //Will be used to print the number used for alarm
//------------------------------------------------------------------------------
int menuPrinted = 0;  //'Flag' for if a menu is printed on screen

int logOn       = 0;  //'Flag' for if the log should log temp
int speedyMode  = 0;  //'Flag' for if fast mode is activated

float theLog[7][1440]; //The Log
float min[7];          //The min temps of 7 days
float max[7];          //The max temps of 7 days
float avg[7];          //The avg temps of 7 days
char tempAsString[10]; //Char array which will be used for temp

int input1      = 12;
int numOfDigits = 0;
int digit       = 0;
int num[3];
int number      = 0;

int rot      = 0;
int maxLight = 3000;
int sunDeg   = 0;
int A5orA6   = 0;        //0 for A5, 1 for A6

int upperLimitSet = 0;
int upperLimit    = 0;
int lowerLimitSet = 0;
int lowerLimit    = 0;

int digitArrayToNumber(int intArray[]){
  int number = 0;
  double x   = (double)1/(double)10;
  for(int i = numOfDigits-1 ; i>=0 ; i--){
    number = (int)((double)number + (double)intArray[i]*(x*(double)10));
    x = x*(double)10;
  }
  return number;
}

void printMenu(void){
  rowSelect(2);
  write(21, menu);
  rowSelect(3);
  write(6, logg);
  rowSelect(4);
  write(11, sun);
  rowSelect(5);
  write(11, alarm1);
  rowSelect(6);;
  write(11, alarm2);
  rowSelect(7);
  write(12, logging);
  if(logOn==1){
    write(3, ON);
  }else{write(3, OFF);}
  rowSelect(8);
  write(14, fast);
  if(speedyMode==1){
    write(3, ON);
  }else{write(3, OFF);}
}

void printAlarmMenu(void){
  rowSelect(2);
  write(25, alarmHeader);
  rowSelect(3);
  write(17, highAlarm);
  rowSelect(4);
  write(16, lowAlarm);
  rowSelect(15);
  write(31, retur);
}
//------------------This is for cases 3_1, 3_2, 4_1, 4_2------------------------
void alarmCase(int positiveAlarm, int highAlarm, char printSetAlarm[]){
  int length = 18;
  if(highAlarm==1){
    length = 19;
  }
  if(menulvl == 1){//If user arrived from former menu level, print. (Saves time so it interfere as little as possible)
    clearMenu();
    rowSelect(2);
    write(length, printSetAlarm);
    rowSelect(14);
    write(30, inputDone);
    rowSelect(15);
    write(31, retur);
  }
  if(((button!=0 && button!=10 && button!= 12 && menulvl==2))&& button != lastButton){
    if(button == 11){
      digit = 0;
    }
    else{digit = button;}
    Write_Data_2_Display(0x50+length+numOfDigits);
    Write_Data_2_Display(0x0);
    Write_Command_2_Display(0x24);
    sprintf(digitChar, "%d", digit);
    write(1, digitChar);
    if(numOfDigits<3){
      num[numOfDigits] = digit;
    }
    numOfDigits++;
  }
  else if(button == 10 && numOfDigits>0){
    number = digitArrayToNumber(num);
    if((number>125 && positiveAlarm==1) || (number<-40 && positiveAlarm==0)){
      write(13, invalid);
    }
    else if(highAlarm==1){
      if(positiveAlarm==0){
        number = 0 - number;
      }
      upperLimit = number;
      upperLimitSet = 1;
      rowSelect(3);
      write(27, AlarmAt);
      sprintf(numAsCharArr, "%3d", number);
      write(3, numAsCharArr);
    }
    else if(highAlarm==0){
      if(positiveAlarm==0){
        number = 0 - number;
      }
      lowerLimit = number;
      lowerLimitSet = 1;
      rowSelect(3);
      write(27, AlarmAt);
      sprintf(numAsCharArr, "%3d", number);
      write(3, numAsCharArr);
    }
  }
  menulvl = 2;
}
//------------------------------------------------------------------------------

void mainMenuSwitchCase(int inputt){
  switch(inputt){
    case 1:
      if(menulvl==0){
        clearMenu();
        write(33, logHeader);
        int k = 0;
        int highAdress = 0;
        for(int i = 3 ; i<10 ; i++){
          int rowColumn = rowSelect(i);
          write(10, logDaysPrint[k]);
          
          sprintf(tempAsString, "%.1f", min[k]);
          write(4, tempAsString);
          int inc = 0x2*0xA;
          movePointer(rowColumn, inc, highAdress);
          
          sprintf(tempAsString, "%.1f", avg[k]);
          write(4, tempAsString);
          
          inc = inc + 0xA;
          highAdress = movePointer(rowColumn, inc, highAdress);
          sprintf(tempAsString, "%.1f", max[k]);
          write(4, tempAsString);
          k++;
        }
        menulvl = 1;
        rowSelect(15);
        write(31, retur);
      }
      break;
    case 2:
      if(menulvl==0){
        clearMenu();
        menuPrinted = 0;
      }
      if(menuPrinted == 0){//If we've not found the sun and printed the result
        if(servoCounter >=5){//Need a pause so lightsensor can react
          servoCounter = 0;
          if(rot==180){
            if(A5orA6 == 1){//If A6 found the "sun" (180 - 360 degrees)
              turnServo(sunDeg-180);
            }
            else{
              turnServo(sunDeg);
            }
            sprintf(deg, "%3d", sunDeg);
            rowSelect(2);
            write(8, degree);
            write(3, deg);
            menuPrinted = 1;
          }
          else{
            lightA6 = measureLightA6();
            lightA5 = measureLightA5();
            if(lightA5 < maxLight){            //Bigger number from measureLight -> Darker
              maxLight = lightA5;
              sunDeg   = rot;
              A5orA6   = 0;
            }
            if(lightA6 < maxLight){
              maxLight = lightA6;
              sunDeg   = rot + 180;
              A5orA6   = 1;
            }
            turnServo(rot);
            rot++;
          }
        }
        menulvl = 1;
        rowSelect(15);
        write(31, retur);
      }
      break;
    case 3:
      if(menulvl==0){
        clearMenu();
        menuPrinted = 0;
      }
      if((button!=0 && menulvl==1) || (button==12 && menulvl==2)){
        input1 = button;
      }
      switch(input1){
        case 1:
          alarmCase(1, 1, setHigh1);          
          break;
        case 2:
          alarmCase(1, 0, setLow1);          
          break;
        case 12:
          if(menulvl != 1){
            clearMenu();
            menuPrinted = 0;
          }
          numOfDigits = 0;
          Write_Data_2_Display(0x50+19);
          Write_Data_2_Display(0x0);
          Write_Command_2_Display(0x24);
          menulvl = 1;
          if(menuPrinted == 0){
            printAlarmMenu();
            menuPrinted = 1;
          }
          break;
        default :
          menulvl = 1;
          if(menuPrinted == 0){
            printAlarmMenu();
            menuPrinted = 1;
          }
          break;
      }
      break;
    case 4:
      if(menulvl==0){
        clearMenu();
      }
      if((button!=0 && menulvl==1) || (button==12 && menulvl==2)){
        input1 = button;
      }
      switch(input1){
        case 1:
          alarmCase(0, 1, setHigh2);
          break;
        case 2:
          alarmCase(0, 0, setLow2);
          break;
        case 12:
          if(menulvl != 1){
            clearMenu();
            menuPrinted = 0;
          }
          numOfDigits = 0;
          Write_Data_2_Display(0x50+19);
          Write_Data_2_Display(0x0);
          Write_Command_2_Display(0x24);
          menulvl = 1;
          if(menuPrinted == 0){
            printAlarmMenu();
            menuPrinted = 1;
          }
          break;
        default :
          menulvl = 1;
          if(menuPrinted == 0){
            printAlarmMenu();
            menuPrinted = 1;
          }
          break;
      }
      break;
    case 5:
      if(logOn==0){
        logOn = 1;
      }else{logOn = 0;}
      menuPrinted = 0; //We want to reprint the menu so we set the check to 0;
      input = 12;
      break;
      
    case 6:
      if(speedyMode == 0){
        speedyMode = 1;
      }else{speedyMode = 0;}
      menuPrinted = 0;
      input = 12;
      break;
      
    case 12:
      if(menulvl != 0){
        clearMenu();
        printMenu();
      }
      else if(menuPrinted == 0){
        printMenu();
      }
      menulvl = 0;
      menuPrinted = 1;
      rot = 0;
      turnServo(rot);
      maxLight = 3000;
      sunDeg  = 0;
      break;
    default :
      menulvl = 0;
      printMenu();
      break;
  }
}

