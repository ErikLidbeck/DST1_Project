
float temp  = 0;
int tLow = 0;
int tempDone= 0;

void startPulse(void){
   *AT91C_PIOB_OER  = AT91C_PIO_PB25; //Output enable
   *AT91C_PIOB_CODR = AT91C_PIO_PB25; //Go low
   Delay(25);
   *AT91C_PIOB_SODR = AT91C_PIO_PB25;  //Go high
   *AT91C_PIOB_ODR  = AT91C_PIO_PB25;  //Output disable
   *AT91C_TC0_CCR   = AT91C_TC_SWTRG;  //Sw_reset
   
   *AT91C_TC0_SR;
   *AT91C_TC0_IER   = AT91C_TC_LDRBS; //Enable interrupt
}

void write_temp(void){
   Write_Data_2_Display(0x00);
   Write_Data_2_Display(0x00);
   Write_Command_2_Display(0x24);
   char tempsensor[13] = "Tempsensor: ";
   write(12, tempsensor);   //Skriver "Temperatur: "
   
   if(temp<0){
     Write_Data_2_Display(0xD); //Write "-"
     Write_Command_2_Display(0xc0);
   }
   else{
     Write_Data_2_Display(0xB); //Write "+"
     Write_Command_2_Display(0xc0);
   }
   
   int temp11 = (int)(temp*10);
   int temp10 = temp11/100;
   int temp01 = temp11/10 - temp10*10;
   int tempDec= temp11 - temp01*10 - temp10*100;
   Write_Data_2_Display(0x10+temp10);
   Write_Command_2_Display(0xc0);
   Write_Data_2_Display(0x10+temp01);
   Write_Command_2_Display(0xc0);
   Write_Data_2_Display(0xe); //Write "."
   Write_Command_2_Display(0xc0);
   Write_Data_2_Display(0x10 + tempDec);
   Write_Command_2_Display(0xc0);
   
   /*Write "C"*/
   Write_Data_2_Display(0x00);
   Write_Command_2_Display(0xc0);
   Write_Data_2_Display(0x23);
   Write_Command_2_Display(0xc0);
}

void TC0_Handler(void){
   tempDone = 1;
   *AT91C_TC0_IDR = AT91C_TC_LDRBS;     //Disable interrupt
   int RB = *AT91C_TC0_RB;
   int RA = *AT91C_TC0_RA;
   tLow = RB - RA;
   temp = (((tLow * (1 / 42.0)) / 5.0) - 273.15);
}