int lightA5 = 0;
int lightA6 = 0;
int measureLightA6(void){
   *AT91C_ADCC_CHER = 0x2;            //Enable Ch 1 (A6=PA3=AD1=CH1)
   *AT91C_ADCC_CR   = 0x2;            //Start ADC
   
   while((*AT91C_ADCC_LCDR & 0xF000) != 0x1000){}  //Wait until channel 1 have converted
   lightA6 = (*AT91C_ADCC_LCDR & 0xFFF);     //Load converted data
   return lightA6;
}

int measureLightA5(void){
   *AT91C_ADCC_CHER = 0x4;            //Enable Ch 2 (A5=PA4=AD2=CH2)
   *AT91C_ADCC_CR   = 0x2;            //Start ADC
   
   while((*AT91C_ADCC_LCDR & 0xF000) != 0x2000){} //Wait until channel 2 have converted
   lightA5 = (*AT91C_ADCC_LCDR & 0xFFF);      //Load converted data
   return lightA5;
}

double conversion(int light){
   double k = 3.3/(double)0xfff;
   double voltLight = k * (double)light;
   return voltLight;
}

void writeLightVolt(void){
   Write_Data_2_Display(0x28);
   Write_Data_2_Display(0x00);
   Write_Command_2_Display(0x24);
   char lightsensor[14] = "Lightsensor: ";
   write(13, lightsensor);      //Skriver "Ljussensor: "
   //int volt = (int)(conversion((lightA6 + lightA5)/2)*1000);
   int volt = (int)(conversion((lightA6 + lightA5)/2)*1000);
   int volt1000 = volt/1000;
   int volt100 = volt/100 - volt1000*10; 
   int volt10 = volt/10 - volt1000*100 - volt100*10;
   int volt1 = volt - volt1000*1000 - volt100*100 - volt10*10;
   
   Write_Data_2_Display(0x10+volt1000);
   Write_Command_2_Display(0xc0);
   Write_Data_2_Display(0xe); //Write "."
   Write_Command_2_Display(0xc0);
   Write_Data_2_Display(0x10+volt100);
   Write_Command_2_Display(0xc0);
   Write_Data_2_Display(0x10+volt10);
   Write_Command_2_Display(0xc0);
   Write_Data_2_Display(0x10+volt1);
   Write_Command_2_Display(0xc0);
   /*Write "V"*/
   Write_Data_2_Display(0x00);
   Write_Command_2_Display(0xc0);
   Write_Data_2_Display(0x36);
   Write_Command_2_Display(0xc0);
}