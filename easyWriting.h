
int rowSelect(int i){
  int overFF = 0;
  if(i<16){
    if(i>6 && i <13){
      overFF = 1;
    }
    else if(i>12){
      overFF = 2;
    }
    int columns[16] = {0x0, 0x28, 0x50, 0x78, 0xA0, 0xC8, 0xF0, 0x18, 0x40, 0x68, 0x90, 0xB8, 0xE0, 0x8, 0x30, 0x58};
    Write_Data_2_Display(columns[i]);
    Write_Data_2_Display(overFF);
    Write_Command_2_Display(0x24);
    return columns[i];
  }
  return 0;
}

int movePointer(int start, int increment, int highAdress){
  int lowAdress = 0;
  lowAdress = start + increment;
  while(lowAdress>0xFF){
    if(lowAdress>0xFF){
      lowAdress = lowAdress - 0xFF - 0x1;
      highAdress++;
    }
  }
  Write_Data_2_Display(lowAdress);
  Write_Data_2_Display(highAdress);
  Write_Command_2_Display(0x24);
  return highAdress;
}

void clearMenu(void){
  rowSelect(2);
  for(int i=0; i<560; i++){           //Clear screan from 3rd row 
    Write_Data_2_Display(0x00);
    Write_Command_2_Display(0xc0);
  }
  rowSelect(2);
}

void clearRow(int row){
  rowSelect(row);
  for(int i = 0 ; i<0x28 ; i++){
    Write_Data_2_Display(0x00);
    Write_Command_2_Display(0xc0);
  }
}

void write(int x, char word[]){
  int aChar = 0;
  for(int i=0; i<x; i++){
    if(word[i] != '\0'){
      aChar = (int)word[i]-0x20;
      Write_Data_2_Display(aChar);
      Write_Command_2_Display(0xc0);
    }
  }
}
