
int value = 0;
//int button = 0;
//int check_button = 0;

void clearScreen(void);

int buttonPressed(void){
  value = 0;
  *AT91C_PIOD_CODR = (1 << 2);  //Clear EO KEY BUS (active low) (pin 27)
  *AT91C_PIOC_OER =  0x380;     //Make all Column pin as output 
  *AT91C_PIOC_SODR = 0x380;     //Make all Column pins high (pin 39-41)
  for(int i = 0; i < 3;i++){    //Loop Column
    *AT91C_PIOC_CODR = (1 << i+7);    //Clear one column at the time
    for(int j = 0; j < 4;j++){        //Loop row
      if((~*AT91C_PIOC_PDSR & (1 << j+2)) == (1 << j+2)){   //Read row and check if bit is zero 
        value = j*3+i+1;
      } 
    }//end loop Row
    *AT91C_PIOC_SODR = (1 << i+7);    //Set the column again 
  } //End loop Column 
  *AT91C_PIOC_ODR = 0x380;    //Make all Column pin as input 
  return value;
}

void Delay(int Value){
  for(int i=0;i<Value;i++)
    asm("nop");
}
unsigned char Read_Status_Display(void){
  unsigned char Temp;
  *AT91C_PIOC_ODR  = 0x3fc;    //make databus as input
  *AT91C_PIOC_ODR  = (1<<13);  //Set dir as input (74chip, 1 = input)
  *AT91C_PIOC_OER  = (1<<12);
  *AT91C_PIOC_CODR = (1<<12);  //Clear/enable output (74chip 0 = enable)
  *AT91C_PIOC_SODR = (1<<17);  //Set C/D
  *AT91C_PIOC_CODR = (1<<16);  //Clear chip select display
  *AT91C_PIOC_CODR = (1<<15);  //Clear read display
  Delay(15);                //Make a Delay
  Temp = *AT91C_PIOC_PDSR;     //Read data bus and save it in temp
  *AT91C_PIOC_SODR = (1<<16);  //Set chip select display
  *AT91C_PIOC_SODR = (1<<15);  //Set read display
  *AT91C_PIOC_ODR  = (1<<12);  //Disable output (74chip)
  *AT91C_PIOC_OER  = (1<<13);  //Set dir as output (74chip)
  return Temp;
}
void Write_Command_2_Display(unsigned char Command){
  while((~Read_Status_Display() & 0xC) == 0xC){//Wait until Read_Status_Display returns an OK
    asm("nop");
  }
  *AT91C_PIOC_CODR = 0x3fc;          //Clear databus
  *AT91C_PIOC_SODR = (Command << 2); //Set Command to databus
  *AT91C_PIOC_OER  = (1<<13);        //Set dir as output (74chip)
  *AT91C_PIOC_OER  = (1<<12);        //Enable output (74chip)
  *AT91C_PIOC_OER  = 0x3fc;          //Set databus as output
  *AT91C_PIOC_SODR = (1<<17);        //Set C/D signal High (1 = Command)
  *AT91C_PIOC_CODR = (1<<16);        //Clear chip select display
  *AT91C_PIOC_CODR = (1<<14);        //Clear write display
  Delay(15);                         //Make a Delay
  *AT91C_PIOC_SODR = (1<<16);        //Set chip enable display
  *AT91C_PIOC_SODR = (1<<14);        //Set write display
  *AT91C_PIOC_ODR  = (1<<12);        //Disable output (74chip)
  *AT91C_PIOC_ODR  = 0x3fc;          //Make databus as input
}
void Write_Data_2_Display(unsigned char Data){
  while((~Read_Status_Display() & 0xC) == 0xC){//Wait until Read_Status_Display returns an OK
    asm("nop");
  }
  *AT91C_PIOC_CODR = 0x3fc;           //Clear databus
  *AT91C_PIOC_SODR = (Data << 2);     //Set Data to databus
  *AT91C_PIOC_OER  = (1<<13);         //Set dir as output (74chip)
  *AT91C_PIOC_OER  = (1<<12);         //Enable output (74chip)
  *AT91C_PIOC_OER  = 0x3fc;           //Set databus as output
  *AT91C_PIOC_CODR = (1<<17);         //Clear C/D signal High (0 = Data)
  *AT91C_PIOC_CODR = (1<<16);         //Clear chip select display
  *AT91C_PIOC_CODR = (1<<14);         //Clear write display
  Delay(15);                           //Make a Delay
  *AT91C_PIOC_SODR = (1<<16);         //Set chip enable display
  *AT91C_PIOC_SODR = (1<<14);         //Set write display
  *AT91C_PIOC_ODR  = (1<<12);         //Disable output (74chip
  *AT91C_PIOC_ODR  = 0x3fc;           //Make databus as input
}

void Init_Display(void){
  *AT91C_PIOC_CODR = 0x1;//Clear Reset display
  Delay(150);
  *AT91C_PIOC_SODR = 0x1;//Set Reset display
  
  Write_Data_2_Display(0x00);
  Write_Data_2_Display(0x00);
  Write_Command_2_Display(0x40);//Set text home address
  Write_Data_2_Display(0x00);
  Write_Data_2_Display(0x40);
  Write_Command_2_Display(0x42); //Set graphic home address
  Write_Data_2_Display(0x28); //0x28
  Write_Data_2_Display(0x00);
  Write_Command_2_Display(0x41); // Set text area
  Write_Data_2_Display(0x28);
  Write_Data_2_Display(0x00);
  Write_Command_2_Display(0x43); // Set graphic area
  Write_Command_2_Display(0x80); // text mode
  Write_Command_2_Display(0x94); // Text on graphic off
  
  
  clearScreen();
}

void clearScreen(void){
  Write_Data_2_Display(0x00);
  Write_Data_2_Display(0x00);
  Write_Command_2_Display(0x24);
  for(int i=0; i<640; i++)       //Clear screan
  {      
    Write_Data_2_Display(0x00);
    Write_Command_2_Display(0xc0);
  }
  Write_Data_2_Display(0x00);
  Write_Data_2_Display(0x00);
  Write_Command_2_Display(0x24);
}

void Write_Keypad_Display(unsigned int Button){
  //write *
  if(Button == 10){
    Write_Data_2_Display(0xA);
    Write_Command_2_Display(0xc0);
  }
  //Write 0
  else if(Button == 11){
    Write_Data_2_Display(0x10);
    Write_Command_2_Display(0xc0);
  }
  //Write #
  else if(Button == 12){
    Write_Data_2_Display(0x3);
    Write_Command_2_Display(0xc0);
  }
  else{
    Write_Data_2_Display(0x10+Button);
    Write_Command_2_Display(0xc0);
  }
}