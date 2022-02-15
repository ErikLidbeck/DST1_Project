
/*
Minsta deg = 1837
Neutral    = 3937
Högsta     = 6037
*/

void turnServo(int Deg){
//   if(Deg != 0){
  if(Deg>180){
    *AT91C_PWMC_CH1_CDTYUPDR = 1837;
  }
  *AT91C_PWMC_CH1_CDTYUPDR = 1837+(28*(Deg));
//    if(Deg == 100){
//        *AT91C_PWMC_CH1_CDTYUPDR = 3937;
//    }
//  }
}

