void dispPos()
{
  // all these coordinates and sizes need to be scaled for different size of displays automagically
  //  should determine these values by querying tft and touch - maybe in setup
  // 320 wd X 240  ht display

  roundness = 4;   // orig 14
  spacing = 3;
  buttonht = ht / 7; // 37;   general height of buttons 26
  buttonwd = ((wd - 3 * spacing) / 4); // max 4 buttons 

 
  // First row of buttons
  firstrowy = 3;   //ht/48 
  // VFO button related
  vfox =  spacing;   //wd /16 ;  // 20; top of screen
  vfoy = firstrowy;    //ht/48; // 5;
  vfowd = buttonwd ;  // wd/4.2; //  75;
  vfoht = buttonht;

  // MEM button related
  memx = vfox + vfowd + spacing;   //110; 5 px space
  memy = firstrowy;    //5;
  memwd = 2 * buttonwd;    //135; reduce spacing 5
  memht = buttonht;    //37;

  // Tx Rx box
  txrx = memx + memwd + spacing; // 260;
  txry = firstrowy;    //5;
  txrwd = buttonwd;    //50;
  txrht = buttonht;    //37;

    // Scan Down area
  scandnx = 35;
  scandny = firstrowy + buttonht + 5;
  scandnwd = 20;
  scandnht = 1.3 * buttonht;

  // frequeny box  Second Row
  frqx = scandnx + scandnwd + 5;
  frqy = firstrowy + buttonht + 5; // 45;
  frqwd = 3.0 * buttonwd;   //  3.0 was 3.5
  frqht = 1.3 * buttonht ;    //40;

  // Scan Up area
  scanupx = frqx + frqwd + 5;
  scanupy = scandny;
  scanupwd = scandnwd;
  scanupht = 1.3 * buttonht;

  vfoABMx = spacing;  // 25 where A/B or M is displayed
  vfoABMy = frqy + 8;

  frq2x1 = vfoABMx + spacing; // next line to display other freqs
  frq2x2 = vfoABMx + wd / 2 - 5; // x pos for 2nd freq display
  frq2y = vfoABMy + frqht + 1;   // second display of VFO /mem

  // band button  Third Row  : below freq2 display row
  bandx = spacing;    //20;
  bandy = frq2y + 20;     // 113; 2 for freq2 displ
  bandwd = 1.4 * buttonwd;    //89;
  bandht = buttonht;  //37;

  //step button
  stpx = bandx + bandwd + spacing;  // 114;
  stpy = bandy;     //113
  stpwd = 1.6 *  buttonwd  ;   //124;
  stpht = buttonht;   //37;
 
  // sideband button
  sbx = stpx + stpwd + spacing ;   //243;
  sby = bandy;         //113;
  sbwd =  buttonwd;  //69;
  sbht = buttonht;   //37;


  // modified Line 4:  V><M, SPLIT, SAVE : v 3.1 onwards
  
   // Previous screen Just display "<"
  prsnx = spacing;
  prsny = bandy + buttonht + 2;
  prsnwd = 30;
  prsnht = buttonht;

  // vfo < > mem button   Fourth Row
 
  vmx = prsnx + prsnwd + spacing;
  vmy = bandy + bandht + 2;
  vmwd =  buttonwd * 1.2;     // was 1.4 times
  vmht = buttonht;

  // SPLIT button
  splx = vmx + vmwd + spacing;
  sply = vmy;
  splwd = buttonwd;
  splht = buttonht;

  // save button
  svx = splx + splwd + spacing;
  svy = sply;
  svwd =  buttonwd;
  svht = buttonht;

   // Next screen button ">"
  nxsnx = svx + svwd + spacing;
  nxsny = svy;
  nxsnwd = 30;
  nxsnht = buttonht;


  // New line 5 :PrevScrn <, BFO1, BFO2 , NextScrn >:  v 3.1 onwards
 
   // F1 button 2 char msg button
  f1x = spacing;
  f1y = prsny + buttonht +2;
  f1wd = 0.5 * buttonwd;
  f1ht = buttonht; 

  // F2 button for bfo1 / PTT Type   etc variable params
  f2x = f1x + f1wd + spacing;
  f2y = f1y;
  f2wd = 1.5 * buttonwd; 
  f2ht = buttonht; 

  // F3 button for 2 char msg
  f3x = f2x + f2wd + spacing;
  f3y = f1y; // 192,
  f3wd = 0.5 * buttonwd;
  f3ht = buttonht; 

  //F4 button for bfo2 / Tx Timeout etc
  f4x = f3x + f3wd + spacing;
  f4y = f1y;        
  f4wd = 1.5 * buttonwd; 
  f4ht = buttonht; 

 

  // BOT MESSAGE STRIP /S Meter
  botx =  5;
  boty = f3y + f3ht; 
  botwd = wd - botx - 70; 
  botht = ht - (f3y + f3ht + 1); // 20

}

