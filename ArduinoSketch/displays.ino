void display_mem()
{
  tft.setCursor(memx + 75, memy + 5); //(185, 12);
  tft.setTextSize(3);
  tft.setTextColor(GREEN, GREY);
  if (memCh < 10)
    tft.print("0");
  tft.print(memCh);
  if (memCh < 100)
    tft.print(" ");
  if (!xch_M)
    read_ch();
  else
    xch_M = 0;

  if (vfo_M_sel)
  {
    vfo = vfo_M;
    bfo1 = bfo_M;
    display_bfo1();
    set_bfo1();
    sideband = sb_M;
    display_sideband();
    display_frequency();
  }
  else
    display_frequency2();
}

void display_mem_msg(String msg)
{
  tft.setCursor(memx + 10, memy + 5); //(185, 12);
  tft.setTextSize(3);
  tft.setTextColor(RED, GREY);
  tft.print(msg);
}

void displ_rx()
{
  tft.drawRoundRect(txrx, txry, txrwd, txrht, roundness, RED);  // TxRx box outline
  tft.fillRoundRect(txrx + 2, txry + 2, txrwd - 4, txrht - 4, roundness - 4, GREEN); //TxRx box
  tft.setCursor(txrx + 10, txry + 5);
  tft.setTextSize(3);
  tft.setTextColor(BLUE);
  tft.print("Rx");
}
void displ_tx()
{
  tft.drawRoundRect(txrx, txry, txrwd, txrht, roundness, RED);  // TxRx box outline
  tft.fillRoundRect(txrx + 2, txry + 2, txrwd - 4, txrht - 4, roundness - 4, RED); //TxRx box
  tft.setCursor(txrx + 10, txry + 5);
  tft.setTextSize(3);
  tft.setTextColor(BLUE);
  tft.print("Tx");
}

void display_vfo()
{
  tft.setCursor(vfoABMx, vfoABMy);    //(25, 50);
  tft.setTextSize(4);
  tft.setTextColor(WHITE, BLACK);
  old_band = bnd_count;

  if (vfo_M_sel)
    tft.print("M");  // Mem   ....
  else if (vfo_A_sel)
    tft.print("A");  // VFO A or B  ....
  else
    tft.print("B");  // VFO A or B  ....

  display_frequency2(); // 2nd line of display only when vfos changed
  set_band();  // select and display band according to frequency displayed
  display_band();
}


void display_frequency()
{
  tft.setTextSize(4);
  tft.setTextColor(WHITE, BLACK);
  tft.setCursor(frqx + 2, frqy + 8); //(70, 50);
  if (vfo < 10000000)
    tft.print(" ");
  tft.print(vfo / 1000.0, 3);
}

void display_frequency2()
{
  //other 2 vfo's displayed below
  tft.setTextSize(2);
  tft.setTextColor(WHITE, BLACK);
  tft.setCursor(frq2x1, frq2y);   //(25, 93);

  if (vfo_A_sel)
  {
    if (splitON)
      tft.setTextColor(RED, BLACK); //else use WHITE / BLACK
    tft.print("B ");
    tft.print(vfo_B / 1000.0, 3);
    tft.print(" ");
    tft.setCursor(frq2x2, frq2y);   // (170, 93);
    tft.setTextColor(WHITE, BLACK);
    tft.print("M ");
    tft.print(vfo_M / 1000.0, 3);
    tft.print(" "); // takes care of previous leftover digit
  }
  if (vfo_B_sel)
  {
    if (splitON)
      tft.setTextColor(GREEN, BLACK); //else use WHITE / BLACK
    tft.print("A ");
    tft.print(vfo_A / 1000.0, 3);
    tft.print(" ");
    tft.setCursor( frq2x2, frq2y);  //(170, 93);
    tft.setTextColor(WHITE, BLACK);
    tft.print("M ");
    tft.print(vfo_M / 1000.0, 3);
    tft.print(" ");
  }
  if (vfo_M_sel)
  {
    if (splitON)
      tft.setTextColor(GREEN, BLACK);// else use WHITE / BLACK
    tft.print("A ");
    tft.print(vfo_A / 1000.0, 3);
    tft.print(" ");
    tft.setCursor(frq2x2, frq2y);  //(170, 93);
    if (splitON)
      tft.setTextColor(RED, BLACK); //else use WHITE / BLACK
    tft.print("B ");
    tft.print(vfo_B / 1000.0, 3);
    tft.print(" ");
    tft.setTextColor(WHITE, BLACK);
  }
} // end of display_frequency2()

void set_band()       // from frequecy determine band and activate corresponding relay TBD
{
  for (int i = MAX_BANDS; i >= 0; i--)
  {
    if ((vfo >= F_MIN_T[i]) && (vfo <= F_MAX_T[i]))
    {
      bnd_count = i ;
      break;
    }
  }
  //  digitalWrite(band_cntrl[old_band], LOW);   // deactivate old band relay
  //  digitalWrite(band_cntrl[bnd_count], HIGH); // activate new selected band
}

void display_band()
{
  tft.setCursor(bandx + 2, bandy + 10); //22, 125);
  tft.setTextSize(2);
  tft.setTextColor(WHITE, GREY);
  //  changed_f = 1;           // ???? why here
  tft.print(B_NAME_T[bnd_count]);
}  // end of Display-band()

void change_band() {
  display_band();
  F_MIN = F_MIN_T[bnd_count];
  F_MAX = F_MAX_T[bnd_count];
  vfo = VFO_T[bnd_count];
  //  set_band();
  changed_f = 1;
}  // end of change_band()

// Displays the frequency change step
void display_step()
{ tft.setCursor(stpx + 3, stpy + 10); // (117, 125);
  tft.setTextSize(2);
  tft.setTextColor(WHITE, GREY);
  tft.print(step_sz_txt[step_index]);
}

void display_sideband() {
  tft.setCursor(sbx + 18, sby + 10); //(261, 125);
  tft.setTextSize(2);
  tft.setTextColor(WHITE, GREY);
  if (sideband == LSB)
  {
    tft.print("LSB");
  }
  else if (sideband == USB)
  {
    tft.print("USB");
  }
}

void update_row5()
{
  if (screen_no == 0)    // bfo1 and bfo2 in f2 and f4 buttons
  {
    // display bfo1 and bfo2
    tft.setTextSize(2);
    tft.setTextColor(WHITE, BLUE);
    tft.fillRoundRect(f1x + 2, f1y + 2, f1wd - 4, f1ht - 4, roundness - 4, BLUE); //F1
    tft.setCursor(f1x + 5, f1y + 10);
    tft.print("B1");
    tft.fillRoundRect(f2x + 2, f2y + 2, f2wd - 4, f2ht - 4, roundness - 4, GREY);
    tft.setTextColor(WHITE, GREY);
    display_bfo1();

    tft.fillRoundRect(f3x + 2, f3y + 2, f3wd - 4, f3ht - 4, roundness - 4, BLUE);
    tft.setTextColor(WHITE, BLUE);
    tft.setCursor(f3x + 5, f3y + 10);
    tft.print("B2");
    tft.fillRoundRect(f4x + 2, f4y + 2, f4wd - 4, f4ht - 4, roundness - 4, GREY);
    tft.setTextColor(WHITE, GREY);
    display_bfo2();
  }
  else if (screen_no == 1)
  {
    // display PTT Type and Tx Time out
    tft.setTextSize(2);
    tft.setTextColor(WHITE, BLUE);
    tft.fillRoundRect(f1x + 2, f1y + 2, f1wd - 4, f1ht - 4, roundness - 4, BLUE); //F1
    tft.setCursor(f1x + 5, f1y + 10);
    tft.print("PT");
    tft.setCursor(f2x + 15, f2y + 10);
    tft.fillRoundRect(f2x + 2, f2y + 2, f2wd - 4, f2ht - 4, roundness - 4, GREY);
    tft.setTextColor(WHITE, GREY);
    if (!active_PTT_in)
      tft.print("Toggle");
    else
      tft.print("Normal");

    if ( Tx_timeout_mode)
      tft.setTextColor(RED, GREY); // Activated then RED Fonts
    else
      tft.setTextColor(GREEN, GREY);

    tft.fillRoundRect(f3x + 2, f3y + 2, f3wd - 4, f3ht - 4, roundness - 4, GREY);
    tft.setCursor(f3x + 5, f3y + 10);
    tft.print("TO");
    tft.setCursor(f4x + 40, f4y + 10);
    tft.fillRoundRect(f4x + 2, f4y + 2, f4wd - 4, f4ht - 4, roundness - 4, GREY);
    // tft.setTextColor(RED, GREY);
    tft.print(Tx_timeout);
  }
  else if (screen_no == 2)   // band offset
  {
    tft.setTextSize(2);
    tft.setTextColor(WHITE, BLUE);
    tft.fillRoundRect(f1x + 2, f1y + 2, f1wd - 4, f1ht - 4, roundness - 4, BLUE); //F1
    tft.setCursor(f1x + 5, f1y + 8);
    tft.print("OF");
    tft.setCursor(f2x + 25, f2y + 10);
    tft.fillRoundRect(f2x + 2, f2y + 2, f2wd - 4, f2ht - 4, roundness - 4, GREY);
    tft.setTextColor(WHITE, GREY);
    tft.print(offsets[bnd_count]);

    tft.setCursor(f3x + 5, f3y + 10);
    tft.fillRoundRect(f3x + 2, f3y + 2, f3wd - 4, f3ht - 4, roundness - 4, BLUE);
    tft.setTextColor(WHITE, BLUE);
    tft.print("TS"); // Touch Sensitivity
    tft.setCursor(f4x + 40, f4y + 10);
    tft.fillRoundRect(f4x + 2, f4y + 2, f4wd - 4, f4ht - 4, roundness - 4, GREY);
    tft.setTextColor(WHITE, GREY);
    tft.print(ts_delay); // delay time between two touches
  }

  else if (screen_no == 3 && !txstatus)   // Smeter if Rx & Power meter value
  {
    tft.setTextSize(2);
    tft.setTextColor(WHITE, GREY);
    tft.fillRoundRect(f1x + 2, f1y + 2, f1wd - 4, f1ht - 4, roundness - 4, GREY); //F1
    tft.setCursor(f1x + 5, f1y + 8);
    tft.print("SL"); // Min value of S meter  (S meter Low)
    tft.setCursor(f2x + 25, f2y + 10);
    tft.fillRoundRect(f2x + 2, f2y + 2, f2wd - 4, f2ht - 4, roundness - 4, GREY);
    tft.setTextColor(WHITE, GREY);
    tft.print(SM_min); // Value for displaying Full scale from A12, Adjustable

    
    tft.setCursor(f3x + 5, f3y + 10);
    tft.fillRoundRect(f3x + 2, f3y + 2, f3wd - 4, f3ht - 4, roundness - 4, GREY);
    tft.setTextColor(WHITE, GREY);
    tft.print("SH"); // Max value of S Meter (S meter High)
    tft.setCursor(f4x + 40, f4y + 10);
    tft.fillRoundRect(f4x + 2, f4y + 2, f4wd - 4, f4ht - 4, roundness - 4, GREY);
    tft.setTextColor(WHITE, GREY);
    tft.print(SM_max); // S9 value ~65% of Full scale, Not adjustable
  }
  else if(screen_no == 3 and txstatus)  // if TX on Power meter
  {
    tft.setTextSize(2);
    tft.setTextColor(PINK, GREY);
    tft.fillRoundRect(f1x + 2, f1y + 2, f1wd - 4, f1ht - 4, roundness - 4, GREY); //F1
    tft.setCursor(f1x + 5, f1y + 8);
    tft.print("PL"); // Min value of Power meter  (P meter Low)
    tft.setCursor(f2x + 25, f2y + 10);
    tft.fillRoundRect(f2x + 2, f2y + 2, f2wd - 4, f2ht - 4, roundness - 4, GREY);
    tft.setTextColor(WHITE, GREY);
    tft.print(PM_min); // Value for displaying Full scale from A11, Adjustable

    
    tft.setCursor(f3x + 5, f3y + 10);
    tft.fillRoundRect(f3x + 2, f3y + 2, f3wd - 4, f3ht - 4, roundness - 4, GREY);
    tft.setTextColor(PINK, GREY);
    tft.print("PH"); // Max value of S Meter (P meter High)
    tft.setCursor(f4x + 40, f4y + 10);
    tft.fillRoundRect(f4x + 2, f4y + 2, f4wd - 4, f4ht - 4, roundness - 4, GREY);
    tft.setTextColor(WHITE, GREY);
    tft.print(PM_max); // P meter Max value
  }

}

void display_bfo1()    // bfo1 ver 3.1 at 5th row
{
  tft.setTextSize(2);
  tft.setTextColor(WHITE, GREY);
  tft.setCursor(f2x + 5, f2y + 10);
  if (bfo1 < 10000000)
    tft.print(" ");
  tft.print(bfo1);
}

void display_bfo2()
{
  tft.setTextSize(2);
  tft.setTextColor(WHITE, GREY);
  tft.setCursor(f4x + 5, f4y + 10);
  if (bfo2 < 10000000)
    tft.print(" ");
  tft.print(bfo2);
}

void display_msg(int xposn, String msg)
{ tft.setTextSize(2); // may setup some soft buttons here
  tft.setCursor(xposn, boty);  
  tft.setTextColor(WHITE, BLUE);
  tft.println(msg);
}
void debug_msg(int xposn, int msg)
{ tft.setTextSize(2); // may setp some soft buttons here
  tft.setCursor(xposn, boty);    //223);
  tft.setTextColor(WHITE, BLUE);
  tft.write(msg);
}

void displ_split_button()
{
  if (!splitON)
  {
    tft.drawRoundRect(splx, sply, splwd, splht, roundness, GREEN); // F3 button outline
    tft.fillRoundRect(splx + 2, sply + 2, splwd - 4, splht - 4, roundness - 4, PURPLE); //Split
    tft.setTextSize(2);
    tft.setTextColor(WHITE);
    tft.setCursor(splx + 10, sply + 10);
    tft.print("SPLIT");
    //  display_vfo();
    display_frequency2();
  }
  else   // under Split mode control
  {
    tft.drawRoundRect(splx, sply, splwd, splht, roundness, GREEN); // F3 button outline
    tft.fillRoundRect(splx + 2, sply + 2, splwd - 4, splht - 4, roundness - 4, YELLOW); //Split
    tft.setTextSize(2);
    tft.setTextColor(RED);
    tft.setCursor(splx + 10, sply + 10);
    tft.print("SPLIT");
    //  vfo_selA();
    //   display_vfo();
    display_frequency2();
  }
}

void displ_timeout_button()
{
  if (Tx_timeout_mode)   // button red
  {
    tft.drawRoundRect(f1x, f1y, f1wd, f1ht, roundness, WHITE); // TxTmO button outline TxTimeOut
    tft.fillRoundRect(f1x + 2, f1y + 2, f1wd - 4, f1ht - 4, roundness - 4, RED); //F1
    tft.setTextSize(2);
    tft.setTextColor(WHITE);
    tft.setCursor(f1x + 5, f1y + 8);
    tft.print("TxTmO");
    Tx_start_time = 0;    // timer acually starts by PTT in rx_tx_ptt() function
  }
  else     // button green when not in Tx timeout mode
  {
    tft.drawRoundRect(f1x, f1y, f1wd, f1ht, roundness, WHITE); // TxTmO button outline TxTimeOut
    tft.fillRoundRect(f1x + 2, f1y + 2, f1wd - 4, f1ht - 4, roundness - 4, GREEN); //F1
    tft.setTextSize(2);
    tft.setTextColor(BLUE);
    tft.setCursor(f1x + 5, f1y + 8);
    tft.print("TxTmO");
  }
}
