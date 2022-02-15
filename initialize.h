void initialize(void){
  //-------------------------Display&Keypad-------------------------------------
  *AT91C_PIOD_PER = 0x1;     //Enable pin 25
  *AT91C_PIOC_PER = 0xff040; //Enable pin 38, 44-51
  *AT91C_PIOC_OER = 0x3c000; //Enable output pin 46-49
  
  *AT91C_PMC_PCER   = 0x6000;   //Clock enable port C & D
  *AT91C_PIOD_PER   = (1 << 2); //Enable reg. pin 27
  *AT91C_PIOC_PER   = 0x3BC;    //Enable reg. pin 34-37 & 39-41
  *AT91C_PIOD_PPUDR = (1 << 2); //Pullup disable pin 27
  *AT91C_PIOC_PPUDR = 0x3BC;    //pullup disable pin 34-37 & 39-41
  
  *AT91C_PIOD_OER = (1 << 2);   //Enable Output PIN27 
  *AT91C_PIOC_OER =  0x380;     //Enable Output PIN39-41
  *AT91C_PIOC_ODR =  0x3C;      //Disable Output PIN34-37

  //----------------------------Temp--------------------------------------------
  *AT91C_PMC_PCER = (1<<AT91C_ID_TC0);    //Enable Clock for TC0
  *AT91C_PMC_PCER = (1<<AT91C_ID_PIOB);   //Enable Clock for PIOB
  *AT91C_TC0_CMR  = (*AT91C_TC0_CMR | AT91C_TC_CLKS_TIMER_DIV1_CLOCK); //Select Timer_Clock1 as TCCLK
  *AT91C_TC0_CCR  = 0x5;            //Enable counter and sw_trigger for TC_CCR0
  *AT91C_TC0_CMR  = (*AT91C_TC0_CMR | AT91C_TC_LDRA_FALLING);    //Load counter to A when TIOA falling in (TC0_CMR0)
  *AT91C_TC0_CMR  = (*AT91C_TC0_CMR | AT91C_TC_LDRB_RISING);    //Load counter to B when TIOA rising in (TC0_CMR0)
   
  *AT91C_PIOB_PER   = AT91C_PIO_PB25;
  *AT91C_PIOB_OER   = AT91C_PIO_PB25;
  *AT91C_PIOB_SODR  = AT91C_PIO_PB25;
   
  NVIC_ClearPendingIRQ(TC0_IRQn);                       
  NVIC_SetPriority(TC0_IRQn, 1);
  NVIC_EnableIRQ(TC0_IRQn);
  
  //-----------------------------Light------------------------------------------
  *AT91C_ADCC_CWR  = (1<<24);
  *AT91C_PMC_PCER1 = (1<<5);               //Enable Clock for ADC
  *AT91C_PMC_PCER  = (1<<AT91C_ID_PIOA);   //Enable Clock for PIOA
  *AT91C_ADCC_MR   =  0x200;               //Set Prescale to 2 
  
  measureLightA5();
  measureLightA6();
   
  //------------------------------Servo-----------------------------------------
  *AT91C_PMC_PCER1      = (1<<4);               //Enable Clock for PWM
  *AT91C_PIOB_PDR       = AT91C_PIO_PB17;       //PB17 control
  *AT91C_PIOB_ABMR      = AT91C_PIO_PB17;       //Assign PB17 to peripheral B function
  *AT91C_PWMC_ENA       = AT91C_PWMC_CHID1;     //Enable CHID1
  *AT91C_PWMC_CH1_CMR   = 0x5;
  *AT91C_PWMC_CH1_CPRDR = 52500;                //Write a value to PWM_CPRD (20ms = 0.02/(32/84000000))
  *AT91C_PWMC_CH1_CDTYR = 1837;
  /*
  Minsta     = 1837
  Neutral    = 3937
  Högsta     = 6037
  */
}