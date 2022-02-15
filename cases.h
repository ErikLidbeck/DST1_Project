void case1(void){
  
}

void case2(void){
  if(menulvl==0){
        clearMenu();
      }
      menulvl = 1;
      if(servoCounter >=5){
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
      rowSelect(15);
      write(31, retur);
}
void case3(void){
  if(menulvl==0){
        clearMenu();
      }
      if((button!=0 && menulvl==1) || (button==12 && menulvl==2)){
        input1 = button;
      }
      switch(input1){
        case 1:
          if(menulvl == 1){
            clearMenu();
          }
          rowSelect(2);
          write(19, setHigh1);
          
          if(((button!=0 && button!=10 && button!= 12 && menulvl==2))&& button != lastButton){
            if(button == 11){
              digit = 0;
            }
            else{digit = button;}
            Write_Data_2_Display(0x50+19+numOfDigits);
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
            if(number>125){
              write(13, invalid);
            }
            else{
              upperLimit = number;
              upperLimitSet = 1;
            }
          }
          
          menulvl = 2;
          rowSelect(15);
          write(31, retur);
          break;
          
        case 2:
          if(menulvl == 1){
            clearMenu();
          }
          
          rowSelect(2);
          write(18, setLow1);
          if(((button!=0 && button!=10 && button!= 12 && menulvl==2))&& button != lastButton){
            if(button == 11){
              digit = 0;
            }
            else{digit = button;}
            Write_Data_2_Display(0x50+18+numOfDigits);
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
            if(number>125){
              write(13, invalid);
            }
            else{
              lowerLimit = number;
              lowerLimitSet = 1;
            }
          }
          
          menulvl = 2;
          rowSelect(15);
          write(31, retur);
          break;
        case 12:
          if(menulvl != 1){
            clearMenu();
          }
          numOfDigits = 0;
          Write_Data_2_Display(0x50+19);
          Write_Data_2_Display(0x0);
          Write_Command_2_Display(0x24);
          menulvl = 1;
          printAlarmMenu();
          break;
        default :
          menulvl = 1;
          printAlarmMenu();
          break;
      }
}
void case4(void){
  if(menulvl==0){
        clearMenu();
      }
      if((button!=0 && menulvl==1) || (button==12 && menulvl==2)){
        input1 = button;
      }
      switch(input1){
        case 1:
          if(menulvl == 1){
            clearMenu();
          }
         
          rowSelect(2);
          write(19, setHigh2);
          
          if(((button!=0 && button!=10 && button!= 12 && menulvl==2))&& button != lastButton){
            if(button == 11){
              digit = 0;
            }
            else{digit = button;}
            Write_Data_2_Display(0x50+19+numOfDigits);
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
            number = 0-number;
            if(number<-40){
              write(13, invalid);
            }
            else{
              upperLimit = number;
              upperLimitSet = 1;
            }
          }
          
          menulvl = 2;
          rowSelect(15);
          write(31, retur);
          break;
          
        case 2:
          if(menulvl == 1){
            clearMenu();
          }
          
          rowSelect(2);
          write(18, setLow2);
          if(((button!=0 && button!=10 && button!= 12 && menulvl==2))&& button != lastButton){
            if(button == 11){
              digit = 0;
            }
            else{digit = button;}
            Write_Data_2_Display(0x50+18+numOfDigits);
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
            number = 0-number;
            if(number<-40){
              write(13, invalid);
            }
            else{
              lowerLimit = number;
              lowerLimitSet = 1;
            }
          }
          
          menulvl = 2;
          rowSelect(15);
          write(31, retur);
          break;
        case 12:
          if(menulvl != 1){
            clearMenu();
          }
          numOfDigits = 0;
          Write_Data_2_Display(0x50+19);
          Write_Data_2_Display(0x0);
          Write_Command_2_Display(0x24);
          menulvl = 1;
          printAlarmMenu();
          break;
        default :
          menulvl = 1;
          printAlarmMenu();
          break;
      }
}
void case5(void){
  
}