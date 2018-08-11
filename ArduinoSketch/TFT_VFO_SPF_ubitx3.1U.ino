/* Arduino Sketch to control Si5351 for generating VFO and BFO signals for uBitx or any multiband transceiver
    Uses MCUFRIEND 2.4 inch display with touch screen for controlling all functions. The display and a extension board sit piggy back on the Arduino Mega board.
    SP Bhatnagar VU2SPF ,  vu2spf@gmail.com
    with
    Joe Basque VE1BWV, joeman2116@gmail.com
  Released under GNU GPL3.0
  The Author VU2SPF respectfully and humbly acknowledes the fine contributions of authors of various libraries and other similar projects for publishing their
  code for the benefit of HAMS and other electronic enthusiasts. Ideas from many of these may reflect in this work. There is inspiration from OM Rob Lae g8vlq in initial screen
  layout. Heartfelt Thanks to all users and testers for encouragement.
  There are lot of possible additions / upgrades and improvements. Please suggest or correct and publish your changes for the benefit of all HAMS.
*/


//#define Ver "VU2SPF"   // if the Call is to be displayed on right hand lower corner uncomment this line and comment out next line (by putting // at the beginning of line)
#define Ver "3.1bU"   // cleaned up TFT config etc
//#define Ver "3.1aU"  // screens for  o/p power and screen schanging buttons available during Tx for adjustment
//#define Ver "3.1U"  // New screens for setup / adjusting parameters (Joe's suggestion)
//#define Ver "2.9cU"  // 2.9cU - SPLIT related enhancements; display in freq2 /info colour coded in Split mode, also added individual band offsets, set upper limit to smeter
// Separated user defined variables in userdefs.h tab, defined more hardware buttons (details in userdefs.h),
//#define Ver "2.9bU"  // 2.9bU - momentary CAT PTT problem corrected (PTT_by_CAT)
//#define Ver "2.9a"  // 2.9a - Up/Dn Scan buttons added (U missed in name)
//#define Ver "2.9U"    // Split Freq mode added VFO A & B used for Rx and Tx respectively
//#define Ver "2.8U"    // CAT functionality
//#define Ver "2.7U"    // Band select synced with sidebands, increased drive levels to 6 mA for all clocks
//#define Ver "2.6U"    // Added Active PTT functionality
//#define Ver "2.5U"  // uBitx version.  Uses all 3 clocks of Si5351
//uBitx needs all 3 clocks from Si5351 - Clock0 = Fixed 12MHz (BFO2), Clock1 = 33 (USB) or 57 (LSB) MHz (BFO1) and Clock2 = 45-75 MHz (VFO)

#include <EEPROM.h>
#include "EEPROMAnything.h"
#include <Wire.h>
#include <avr/io.h>
#include "userdefs.h"       // user defined papameters to control rig - to be edited by users

//--------------------Installable Libraries-----------------------------------------------------------------------
#include <Rotary.h>          // https://github.com/brianlow/Rotary 
#include <si5351.h>          // https://github.com/etherkit/Si5351Arduino
#include <Adafruit_GFX.h>    // Core graphics library located at adafuit website  https://github.com/adafruit/Adafruit-GFX-Library
#include <MCUFRIEND_kbv.h>   // https://github.com/prenticedavid/MCUFRIEND_kbv
#include "TouchScreen.h"     // https://github.com/adafruit/Touch-Screen-Library

//-------------------------------------------------------------------------------------
//function prototypes
void dispPos();
void setup_vfo_screen();
void display_msg(int xposn, String msg);
void init_eprom();
void read_eprom();
void set_vfo();
void set_bfo1();
void display_frequency();
void display_vfo();
void display_band();
void display_step();
void display_sideband();
void display_mem();
void display_bfo1();
void set_band();
void band_incr();
void band_decr();
int get_button(int x);
void step_decr();
void step_incr();
void save_frequency();
void displ_rx();
void vfo_sel();
void ptt_ON();
void ptt_OFF();
void toggle_ptt();
void sideband_chg();
void vfo_to_mem();
void mem_decr();
void mem_incr();
void mem_to_vfo();
void bfo1_decr();
void bfo1_incr();
void bfo2_decr();
void bfo2_incr();
void save();
void bfo2_decr();
void bfo2_incr();
void read_ch();
void set_bfo2();
void write_ch();
void write_vfo_A();
void write_vfo_B();
//void write_vfo_M();
void displ_timeout_button();
void displ_split_button();
void scan_dn();
void scan_dn();
void scan_up();
void update_row5();
void check_CAT();

//------- TFT Display related----
// Assign human-readable names to some common 16-bit color values: // EXTRA Color definitions thanks Joe Basque
#define BLACK       0x0000      /*   0,   0,   0 */
#define LIGHTGREY   0xC618      /* 192, 192, 192 */
#define GREY        0x7BEF      /* 128, 128, 128 */
#define DARKGREY    0x7BEF      /* 128, 128, 128 */
#define BLUE        0x001F      /*   0,   0, 255 */
#define NAVY        0x000F      /*   0,   0, 128 */
#define RED         0xF800      /* 255,   0,   0 */
#define MAROON      0x7800      /* 128,   0,   0 */
#define PURPLE      0x780F      /* 128,   0, 128 */
#define YELLOW      0xFFE0      /* 255, 255,   0 */
#define WHITE       0xFFFF      /* 255, 255, 255 */
#define PINK        0xF81F
#define ORANGE      0xFD20      /* 255, 165,   0 */
#define GREEN       0x07E0
#define DARKGREEN   0x03E0      /*   0, 128,   0 */
#define OLIVE       0x7BE0      /* 128, 128,   0 */
#define GREENYELLOW 0xAFE5      /* 173, 255,  47 */
#define CYAN        0x07FF
#define DARKCYAN    0x03EF      /*   0, 128, 128 */
#define MAGENTA     0xF81F


// most mcufriend shields use these pins and Portrait mode:      // **? can we auto define these pins
uint8_t YP;  // must be an analog pin, use "An" notation!
uint8_t XM;  // must be an analog pin, use "An" notation!
uint8_t YM;   // can be a digital pin
uint8_t XP;   // can be a digital pin

//Touch coordinates determined by one of the sample programs provided with touch screen library
uint16_t TS_LEFT; // Touch Screen Left edge
uint16_t TS_RT;    // Touch Screen right edge
uint16_t TS_TOP;   // Touch Screen Top edge
uint16_t TS_BOT;   // Touch Screen Bottom edge

//--------------------------------------------------------------------------------------

Si5351 si5351;
Rotary r = Rotary(ENCODER_A, ENCODER_B);

MCUFRIEND_kbv tft;
TouchScreen ts = TouchScreen(XP, YP, XM, YM, 300);
TSPoint tp;
#define MINPRESSURE 20
#define MAXPRESSURE 1000
//--------------------------------------------------------------------------------------
boolean txstatus = false;     // Rx = False Tx = True
uint32_t bfo_A, bfo_B, bfo_M;  //The bfo frequency is added to or subtracted from the vfo frequency in the "Loop()"
uint32_t bfo2 = 11993900L;  // Fixed 12 MHz BFO2 Farhans 11996500 this value found by test
uint32_t bfo1, bfo1_USB = 56995000L , bfo1_LSB = 32994000L;   // Initial Values of BFO1 for USB or LSB
uint32_t vfo , vfo_tx; // vfo is displayed freq, vfo_tx is actual vfo on clock2 of Si5351
uint32_t vfo_A = 7050000L, vfo_B = 7130000L, vfo_M = 14000000L ; // temp values // either vfo A or B or mem channel is selected for output at any time
boolean vfo_A_sel = true, vfo_B_sel = false, vfo_M_sel = false; // true for vfo selected
boolean changed_f = 0;      // indicating need for updating display
boolean xch_M = 0; // flag for xchged mem in V > M
uint16_t sideband, sb_A, sb_B, sb_M;
uint16_t LSB = 1, USB = 2;
// display step size and radix
String step_sz_txt[] = {"   1Hz ", "   10Hz ", "  100Hz ", "   1kHz ", "  10kHz ", "  100kHz", "   1MHz "};
uint32_t step_sz[] = {    1,          10,        100,        1000,     10000,      100000,     1000000};
int step_index = 3;
uint32_t radix = 1000;  //start step size - change to suit

// If the touch on touch screen is producing multiple touch effect increase this delay
uint16_t ts_delay = 80;    // delay between touch button to reduce sensitivity

// Transmit time out in seconds for reducing heating of the finals
uint16_t Tx_timeout = 20; // time in sec upto which Tx works continuosly then goes to Rx
bool Tx_timeout_mode = false;
uint16_t max_timeout = 60; // Max value of timeout in sec  decided by finals and their cooling

// PTT has two modes
// **** Only Toggle PTT in normal mode / in active_ptt mode PTT remains LOW during Tx
#define PTT_Input     26    // Normal is Toggle PTT (press briefly to go from Rx to Tx & vice-versa)
bool active_PTT_in = false; // if PTT remains continuously low during QSO make it true else false means "toggle PTT" on active low


uint8_t magic_no = 04; // used for checking the initialization of eprom or to reinitialize change this no.
// If mem not initialized there may be garbled display. In that case simply change this number and reload the prog

//------------EPROM memory related -----------------------------------------------------------
uint16_t max_memory_ch = 100;  // each ch takes 10-4=6 bytes *ver 3.1 shifted bfo1 to eprom
struct allinfo {
  uint32_t s_vfo;
  //uint32_t s_bfo;
  uint16_t s_sb;
} Allinfo;  // complete description of channel saved in mem

// Eprom content sequence: allinfo for vfoA, vfoB, mem1,2,3...
uint16_t address;  // temp address used by fns
allinfo ch_info;    // local copy
unsigned int memCh = 1;  // memory  channel  number (1,2,3...100) now.  Try names..??
int32_t Offset_VFO = 0, Offset_80m = 0, Offset_40m = 0, Offset_30m = 0, Offset_20m = 0, Offset_17m = 0, Offset_15m = 0, Offset_12m = 0, Offset_10m = 0; //in mem at 8-11, 12-15, etc...

int bnd_count = 2, old_band; // bnd_count=2 means 40m
int MAX_BANDS = 9;
// Band Limits and frequencies and their corresponding display on band button
volatile uint32_t F_MIN_T[9] = {100000UL,  3500000UL, 7000000UL, 10100000UL, 14000000UL, 18068000UL, 21000000UL, 24890000UL, 28000000UL};
volatile uint32_t  F_MAX_T[9] = {75000000UL,  3800000UL, 7200000UL, 10150000UL, 14350000UL, 18168000UL, 21450000UL, 24990000UL, 29700000UL};
String  B_NAME_T[] = {"  VFO", "  80m", "  40m", "  30m", "  20m", "  17m", "  15m", "  12m", "  10m" };
volatile uint32_t  VFO_T[9] = {9500000UL, 3670000UL, 7100000UL, 10120000UL, 14200000UL, 18105000UL, 21200000UL, 24925000UL, 28500000UL};
volatile int32_t offsets[9] = { Offset_VFO , Offset_80m , Offset_40m , Offset_30m , Offset_20m , Offset_17m , Offset_15m , Offset_12m , Offset_10m };
// offsets for different bands could be -ve also & to be found by experimentation for each rig
unsigned long F_MIN, F_MAX;
uint16_t SM_min = 100, SM_max = 1023 ; //  depending on input to Smeter input Analog pin 55 (A12) this should be adjsuted (by comparison or by ear)
uint16_t PM_min = 100, PM_max = 1023; // Power output
// -- eprom mem map in Ver 3.1 the beginning of mem used to save info from on screen adjustments. VFO infos after mem addr 100
uint16_t magic_no_address = 0;
uint16_t bfo1_USB_address = magic_no_address + sizeof(magic_no);
uint16_t bfo1_LSB_address = bfo1_USB_address + sizeof(bfo1_USB);
uint16_t bfo2_address = bfo1_LSB_address + sizeof(bfo1_LSB);
uint16_t PTT_type_address = bfo2_address + sizeof(bfo2);   // active_PTT_in (TRUE/FALSE)
uint16_t Tx_timeout_mode_address = PTT_type_address + sizeof(active_PTT_in); // Tx_timeout_mode ON/OFF
uint16_t TxTmO_Time_address = Tx_timeout_mode_address + sizeof(Tx_timeout_mode);  // time for Tx Time out in sec
uint16_t touch_sens_address = TxTmO_Time_address + sizeof(Tx_timeout);  // ts_delay for adjusting touch sensitivity
uint16_t offsets_base_address = touch_sens_address + sizeof(ts_delay); // all offsets for each band start here
uint16_t sm_min_address = offsets_base_address + sizeof(offsets); // min value for s meter
uint16_t sm_max_address = sm_min_address + sizeof(SM_min); // max value for s meter

uint16_t eprom_base_addr = 100;  //offsets_base_address + sizeof(offsets[]) ;   // from here all VFO details would be stored
//----------------------------------------------------------------------------------------------

unsigned long Tx_start_time = 0, rem_time = 0;
int diff;

uint16_t xpos, ypos;  //screen coordinates

uint16_t identifier;        // TFT identifier : can be found from the example programs in the TFT library
int wd;                     //= tft.width();
int ht;                     //= tft.height();
int displ_rotn = 1;         // 0 - normal, 1 - 90deg rotn (landscape), 2 - reverse, 3-rev LS  ** if the touch buttons do not match try changing here

// individual button x,y coordinates, height and width, some common params,
uint16_t buttonht, buttonwd, smwd, smx, smy, smht, firstrowy, vfox, vfoy, vfowd, vfoht;
uint16_t memx, memy, memwd, memht, txrx, txry, txrwd, txrht, frqx, frqy, frqwd, frqht;
uint16_t vfoABMx, vfoABMy, frq2x1, frq2x2, frq2y, bandx, bandy, bandwd, bandht;
uint16_t stpx, stpy, stpwd, stpht, sbx, sby, sbwd, sbht, vmx, vmy, vmwd, vmht, splx, sply, splwd, splht, svx, svy, svwd, svht;
uint16_t prsnx, prsny, prsnwd, prsnht,  nxsnx, nxsny, nxsnwd, nxsnht; // prev & nxt screen arrows
uint16_t f1x, f1y, f1wd, f1ht, f2x, f2y, f2wd, f2ht; // F1 to F4 used for various adjustable parameters on line 5
uint16_t f3x, f3y, f3wd, f3ht, f4x, f4y, f4wd, f4ht;
// bfo1x, bfo1y, bfo1wd, bfo1ht, bfo2x, bfo2y, bfo2wd, bfo2ht, , f3x, f3y, f3wd, f3ht
uint16_t botx, boty, botwd, botht;
uint16_t roundness;  // box edge roundness
uint16_t spacing;      // between buttons on same row
//scan buttons
uint16_t scandnx, scandny, scandnwd, scandnht, scanupx, scanupy, scanupwd, scanupht;
bool in_scan_dn = false, in_scan_up = false; // flag to indicate freq scan

//-------------------- S Meter -------------------------------------------------------------------------
unsigned int Sval, Sens, Ssamp = 0;   // s meter related
unsigned long Savg;
//----------------------CAT relted-------------
long prev_CAT_rd = 0l;  // time when last command was received
byte CAT_buff[5] = {0x0, 0x0, 0x0, 0x0, 0x0};
int CAT_buff_ptr = 0;
byte i;
bool checkingCAT = 0; // when CAT is still being received
bool CAT_ctrl = 0; // Under CAT control or not
bool PTT_by_CAT = false;

///---------- etc
bool PTT_by_touch = false; //flag for PTT from touch screen
bool splitON = 0; // Is split freq operation between VFO A and B on?
int screen_no = 0; // screen_no 0 = main, 1 = PTT Setup , 2 = offsets, 3 = Smeter/O/P Power meter displays
//As per Joes suggestion ver 3.1U onwards
int max_screen = 3; // last screen


// Fixed main display  - VFO, Mem, Tx/Rx : VfoDispl, Dn, Freq, Up : Band, Step, SideBand : New Row 4: V><M, SPLIT, SAVE,
// in fourth row the end buttons PrevScrn <,  NextScrn > should be available on Tx for - adjusting Power meter, activating Tx time out
// Variable Fifth Row  differently arranged starting from v 3.1 // adjustable variables / active buttons are white fonts on grey background
// screen 0 : Variable Row 5 :  BFO1, BFO2 : (indicated as B1 and B2)
// screen 1 - PTT setup :   PTT Type, TxTimeOut on/off and its value (Indicated as PT and TO)
// screen 2 - offsets  adjustable offset in Hz for the currently selected band, Touch Sensitivity (Indicated as OF and TS)
// screen 3 - S meter Low (SL) and High (SH) capture buttons and their adjustable values
// screen 4 - Power meter Low (PL) and Hi (PH) capture buttons and their adjustable values
/**************************************/
/* Interrupt service routine for      */
/* encoder frequency change           */
/**************************************/
ISR(PCINT2_vect)
{
  unsigned char result = r.process();
  if (result && !txstatus)      // lock freq during transmit
  {
    if (result == DIR_CW)
      vfo += radix;
    else if (result == DIR_CCW)
      vfo -= radix;
    changed_f = 1;
  }
}
//-------------
void setup()
{
  Serial.begin(38400, SERIAL_8N1);  // 2 stop bits hamlib need - needed for CAT //
  // CAT_get_freq();
  Serial.flush();

  identifier = tft.readID(); // get display ID.

  //#ifdef Sa35_9486
  if ((identifier == 0x9486) || (identifier == 0x7783))   // identify display driver chip (known from mcufriend test progs)
    // spf Ventor's display ST7783,   // spf Ventor's display ST7783,
  {
    TS_LEFT = 50; TS_RT  =  890; TS_TOP = 880; TS_BOT = 100;
    YP = A2; XM = A1; YM = 6;  XP = 7;
  }

  if ((identifier == 0x154) || (identifier == 0x9325))
    // spf robodo display
  {
    TS_LEFT = 950;  TS_RT  = 120;  TS_TOP = 920;  TS_BOT = 120;
    YP = A1;  XM = A2;   YM = 7;   XP = 6;
  }

  if ((identifier == 0x2053) || (identifier == 0x9341))
  {
    TS_LEFT = 950 ; TS_RT  = 120; TS_TOP = 120; TS_BOT = 920;
    YP = A1; XM = A2; YM = 7;  XP = 6;
  }

  if (identifier == 0x9320) // Joe's 2.8 DISPLAY blueg5/5elegoo detected as 0x9341 but with settings below
    // Rejimon's 2.4 DISPLAY from Robodo detected as 5408 but works as 9320
  {
    TS_LEFT = 74; TS_RT  = 906; TS_TOP = 117 ; TS_BOT = 923;
    YP = A3; XM = A2; YM = 9; XP = 8;
  }


  ts = TouchScreen(XP, YP, XM, YM, 300);
  tft.begin(identifier);        // setup to use driver
  wd = tft.width();
  ht = tft.height();
  tft.setRotation(displ_rotn); // LS

  if (displ_rotn == 1 || displ_rotn == 3) // exchange the width and height
  {
    int temp = wd; wd = ht; ht = temp;
  }
  dispPos();

  Wire.begin();   // for si5351
  // welcome_screen();
  setup_vfo_screen(); // create main VFO screen

  if (EEPROM.read(magic_no_address) != magic_no)
    init_eprom();  // if blank eeprom or changed/new magic_no for reinit

  read_eprom(); // get infos of VFO A & B (and 1st memory channel only??)
  vfo = vfo_A;   // then display and use VFO A
  vfo_A_sel = 1;
  bfo1 = bfo_A;
  sideband = sb_A;



  // Ports Init
  pinMode(TX_RX, OUTPUT); // Rx mode D14
  digitalWrite(TX_RX, 0);
  pinMode(CW_TONE, OUTPUT); // d15
  digitalWrite(CW_TONE, 0);
  pinMode(CW_KEY, OUTPUT);  //d19
  digitalWrite(CW_KEY, 0);

  // Filters
  pinMode(TX_LPF_A, OUTPUT);  //d16
  pinMode(TX_LPF_B , OUTPUT);  //d17
  pinMode(TX_LPF_C, OUTPUT);  //d18
  digitalWrite(TX_LPF_A, 0); // All filters off only 30MHz LPF for Rx
  digitalWrite(TX_LPF_B, 0);
  digitalWrite(TX_LPF_C, 0);

  pinMode(ENCODER_BTN, INPUT_PULLUP);     // ? pushbutton setup
  pinMode(BandSelectUp, INPUT_PULLUP);     // band pushbutton setup
  pinMode(BandSelectDn, INPUT_PULLUP);     // band pushbutton setup
  pinMode(MEMUp, INPUT_PULLUP);
  pinMode(MEMDn, INPUT_PULLUP);

  pinMode(SideBandSelect, INPUT_PULLUP);     //sideband pushbutton setup
  pinMode(VFO, INPUT_PULLUP);     //VFO pushbutton setup
  pinMode(VtoMEM, INPUT_PULLUP);     //VtoM pushbutton setup
  pinMode(MEMtoV, INPUT_PULLUP);
  pinMode(STEPUp, INPUT_PULLUP);     //Step pushbutton setup
  pinMode(STEPDn, INPUT_PULLUP);     //Step pushbutton setup
  pinMode(TxTmOut, INPUT_PULLUP);
  pinMode(SPLIT, INPUT_PULLUP);

  pinMode(PTT_Input, INPUT_PULLUP); // PTT Button toggle type  d26
  digitalWrite(PTT_Input, HIGH); // temporary for test

  //Initialise si5351
  //  si5351.set_correction(00); //. There is a calibration sketch in File/Examples/si5351Arduino-Jason; was 140
  //where one can determine the correction by using the serial monitor.

  //initialize the Si5351
  si5351.init(SI5351_CRYSTAL_LOAD_8PF, 0, si5351correction); //If using a 27Mhz crystal, put in 27000000 instead of 0
  // 0 is the default crystal frequency of 25Mhz.

  si5351.set_pll(SI5351_PLL_FIXED, SI5351_PLLA);

  si5351.drive_strength(SI5351_CLK0, SI5351_DRIVE_6MA); //  this is 11dBm  // you can set this to 2MA, 4MA, 6MA or 8MA
  si5351.drive_strength(SI5351_CLK1, SI5351_DRIVE_6MA); //be careful though - measure into 50ohms
  si5351.drive_strength(SI5351_CLK2, SI5351_DRIVE_6MA); //


  si5351.set_freq(bfo2 * SI5351_FREQ_MULT, SI5351_CLK0); // 12 MHZ which remains fixed
  set_bfo1();  // adjust bfo1 and vfo_tx
  //si5351.set_freq(((vfo_tx + if_offset)* SI5351_FREQ_MULT), SI5351_CLK2); // 45 to 75 MHz

  //Tx_timeout = Tx_timeout * 1000; //in ms
  //max_timeout = max_timeout * 1000;
  //----------
  PCICR |= (1 << PCIE2);           // Enable pin change interrupt for the encoder
  PCMSK2 |= (1 << PCINT21) | (1 << PCINT22); // MEGA interrupt pins mapped to A14 A13
  sei();   // start interrupts

  //----------
  set_band();
  display_frequency();  // Update the local display at power on
  display_vfo();
  display_band();       // with values
  display_step();
  display_sideband();
  display_mem();
  display_bfo1();



} // end of setup() //

//--------------------------------------

void loop()
{
  if (CAT_ctrl) return;   // if in cat control go back


  // Update the display if the frequency changed
  if (changed_f)
  {
    //  set_vfo();
    display_vfo();
    set_bfo1();
    display_frequency();
    set_band();
    display_band();
    display_sideband();
    save_frequency();
    changed_f = 0;
    return;
  }


  if (Tx_timeout_mode)
  {
    rem_time = (millis() - Tx_start_time) / 1000; // remaining time
    if  (txstatus)
    {
      if ( rem_time >= Tx_timeout)   // time over
      {
        digitalWrite(TX_RX, LOW);    // disable Tx
        displ_rx();
        txstatus = false;
      }
      else
      {
        tft.setCursor(txrx + 45, txry + 5);  // else display the remaining time
        tft.setTextSize(2);
        tft.setTextColor(WHITE, RED);
        diff = (Tx_timeout - rem_time) ;
        if (diff <= 9)
          tft.print(" ");
        tft.print(diff);
      }
    }
  }
  //-----------------------------------
  // External button controls


  //#### Toggle PTT button
  if (!PTT_by_CAT)   // detect button press if not done by CAT - else if PTT is put on by CAT the open PTT
    // was causing immediate PTT_off. PTT_by_CAT is set/reset from CAT PTT controls
  {
    // for uBitx PTT   - touch ptt is always toggle - first touch on second off
    if (active_PTT_in && !PTT_by_touch && bnd_count > 0)     // i.e. Normal type PTT we call active PTT
    {
      if (digitalRead(PTT_Input) == LOW )     // only if not in Tx already
        ptt_ON();
      else if (txstatus)    // only if in Tx
        ptt_OFF();
    }

    else      // Toggle type PTT
    {
      if (digitalRead(PTT_Input) == LOW && bnd_count > 0)
      {
        if (get_button(PTT_Input))  // get_button returns 1 if pressed for 100 ms+
          toggle_ptt();
      }
    }
  }
  //------ except Tx/Rx touch buttons all buttons locked during Tx
  if (!txstatus)    // only if not in Tx
  {
    //##### Cycle through VFOs A/B/M
    { if (digitalRead(VFO) == LOW )
        if (get_button(VFO))
          vfo_sel();
    }
    //##### Band Select
    { if (digitalRead(BandSelectUp) == LOW )
        if (get_button(BandSelectUp))
          band_incr();
    }
    { if (digitalRead(BandSelectDn) == LOW )
        if (get_button(BandSelectDn))
          band_decr();
    }
    //##### Step Size
    { if (digitalRead(STEPUp) == LOW )
        if (get_button(STEPUp))
          step_incr();
    }
    { if (digitalRead(STEPDn) == LOW )
        if (get_button(STEPDn))
          step_decr();
    }
    //####
    { if (digitalRead(SideBandSelect) == LOW )
        if (get_button(SideBandSelect))
          sideband_chg();
    }

    //#### copying from Mem to VFO or vice-versa
    { if (digitalRead(VtoMEM) == LOW )
        if (get_button(VtoMEM))
          vfo_to_mem();
    }
    { if (digitalRead(MEMtoV) == LOW )
        if (get_button(MEMtoV))
          mem_to_vfo();
    }
    //#### Memory channel incr / decr
    { if (digitalRead(MEMUp) == LOW )
        if (get_button(MEMUp))
          mem_incr();
    }
    { if (digitalRead(MEMDn) == LOW )
        if (get_button(MEMDn))
          mem_decr();
    }
    { if (digitalRead(TxTmOut) == LOW )
        if (get_button(TxTmOut))
          displ_timeout_button();
    }
    { if (digitalRead(SPLIT) == LOW )
        if (get_button(SPLIT))
          displ_split_button();
    }
  }
  //----------------------------------------------
  //$$$$  for Touch Screen input control

  tp = ts.getPoint();
  pinMode(XM, OUTPUT);
  pinMode(YP, OUTPUT);
  pinMode(XP, OUTPUT);
  pinMode(YM, OUTPUT);
  delay(ts_delay);     // delay between two touches to reduce sensitivity

  if (tp.z > MINPRESSURE && tp.z < MAXPRESSURE)
  {
    //#if defined (MCUF0x154) || defined(PL0x9341) || defined (VE0x7783) || defined (pl0x2053)
    xpos = map(tp.x, TS_LEFT, TS_RT, 0, tft.width());
    ypos = map(tp.y, TS_TOP, TS_BOT, 0, tft.height());
    //#endif

    /*
      #if defined elegoo923 || defined (IL9325)
        xpos = map(tp.y, TS_LEFT, TS_RT, 0, tft.width());
        ypos = map(tp.x, TS_TOP, TS_BOT, 0, tft.height());
      #endif*/

    // Rx/Tx PTT touch button or PTT_Input
    if (ypos > firstrowy && ypos < firstrowy + buttonht)  // first row of buttons
    {
      if (xpos > txrx  && xpos < txrx + txrwd - 2  && (bnd_count > 0) ) // toggle between Rx & Tx, TX_RX (D14) goes Hi on Tx
      {
        toggle_ptt();
        PTT_by_touch = !PTT_by_touch;
        delay(50);
      }
    }

    if (!txstatus)    // only if not in Tx should any button be recognized for change
    {
      if (ypos > firstrowy && ypos < firstrowy + buttonht)  // first row of buttons (orig 5,42)
      {
        // VFO Button:  cycle VFO A/B/M in sequence when VFO button is touched
        if (xpos > vfox && xpos < vfox + vfowd ) // change VFO
          vfo_sel();

        // MEM Ch change Button
        // Left half button decreases channel no
        else if (xpos > memx && xpos < memx + buttonwd - 2 ) // decrease channel
          mem_decr();   // decrease memory channel number

        // right half buttton increases ch no
        else if (xpos > memx + buttonwd + 2 && xpos < memx + 2 * buttonwd ) // increase channel
          mem_incr();   // increase mem ch no
        return;
      }
      // First row end

      // Freq Change Second Row touch  Button
      if (ypos > frqy && ypos < frqy + frqht && !txstatus) //  lock freq change during transmit
      {
        if (xpos > scandnx && xpos < (scandnx + scandnwd))
        {
          in_scan_dn = true;
          scan_dn();
          return;
        }
        if (xpos > frqx && xpos < (frqx + frqwd / 2) - 2 ) // Left half button decreases frq by step size
        {
          in_scan_dn = false;
          vfo = vfo - radix;
          changed_f = 1;
          save_frequency();   // added 7/7/17
          return;
        }

        if (xpos > (frqx + frqwd / 2) + 2 && xpos < frqx + frqwd) // Right half button increases freq by step size
        {
          in_scan_up = false;
          vfo = vfo + radix;
          changed_f = 1;
          save_frequency();   // added 7/7/17
          return;
        }

        if (xpos > scanupx && xpos < (scanupx + scanupwd))
        {
          in_scan_up = true;
          scan_up();
          return;
        }
      }  // Freq Button/ Second Row end

      // Third Row  Band Change Button
      if (ypos > bandy && ypos < bandy + bandht)
      {
        if (xpos > bandx && xpos < (bandx + bandwd / 2) - 2 ) // Left half button decreases band(20,65)
          band_decr();

        else if (xpos > (bandx + bandwd / 2) + 2 && xpos < bandx + bandwd ) // Right half button increases band (67,115)
          band_incr();


        // Third Row Step Size change  Button
        //  left half of step button decreases step size
        else if (xpos > stpx && xpos < (stpx + stpwd / 2) - 2 )
          step_decr();

        //  right half of step button increases step size
        else if (xpos > (stpx + stpwd / 2) + 2 && xpos < stpx + stpwd )
          step_incr();


        /// Third Row side band flip flop between LSB & USB   (others may be added if hardware permits)
        else if (xpos > sbx && xpos < sbx + sbwd )
          sideband_chg();
        return;
      } // Third row end

      // Fourth row Central part no change during Tx (new from ver 3.1)
      if (ypos > vmy && ypos < vmy + vmht) //  fourth row
      {
        // Fourth Row VFO < > Mem switch nothing saved on EEPROM unless SAVE button pressed
        if (xpos > vmx && xpos < (vmx + vmwd / 2) - 2 ) //left half VFO -> MEM
          // currently selected VFO stored on currently selected mem ch (not in EEPROM which is by SAVE button)
          vfo_to_mem();

        else if (xpos > (vmx + vmwd / 2) + 2 && xpos < vmx + vmwd ) //right half VFO <- MEM
          mem_to_vfo();

        else if (xpos > splx && xpos < (splx + splwd)) // Split freq Control
        {
          splitON = !splitON;
          displ_split_button();
        }

        // Fourth Row SAVE Button
        else if (xpos > svx && xpos < svx + svwd ) // Save "current" Vfo/Mem and all other parameters on eeprom
          save();


      } // Fourth Row entral part  end
    } // all above buttons not active during Tx

    // Fixed main display  - VFO, Mem, Tx/Rx : VfoDispl, Dn, Freq, Up : Band, Step, SideBand : New Row 4: V><M, SPLIT, SAVE,
    // in fourth row the end buttons PrevScrn <,  NextScrn > should be available on Tx for adjusting Power meter, activating Tx time out
    // Variable Fifth Row  differently arranged starting from v 3.1 // adjustable variables / active buttons are white fonts on grey background
    // screen 0 : Variable Row 5 :  BFO1, BFO2 : (indicated as B1 and B2)
    // screen 1 - PTT setup :   PTT Type, TxTimeOut on/off and its value (Indicated as PT and TO)
    // screen 2 - offsets  adjustable offset in Hz for the currently selected band, Touch Sensitivity (Indicated as OF and TS)
    // screen 3 - S meter Low (SL) and High (SH) capture buttons and their adjustable values
    // screen 4 - Power meter Low (PL) and Hi (PH) capture buttons and their adjustable values

    // Fourth row  two End buttons <- & -> new from ver 3.1 active even during Tx
    if (ypos > vmy && ypos < vmy + vmht) //  fourth row
    {
      if (xpos > prsnx && xpos < (prsnx + prsnwd) ) // previous screen button "<-" changes 5th row displays
      {
        screen_no --;
        if (screen_no < 0)
          screen_no = max_screen;
        update_row5();
      }
      else if (xpos > nxsnx && xpos < (nxsnx + nxsnwd) ) // next screen button "->" : affects 5th row displays
      {
        screen_no ++;
        if (screen_no > max_screen)
          screen_no = 0 ;
        update_row5();
      }
    }

    // One of the screen for Tx power needs to be active when transmitting therefore we have to handle each screen separately
    if (ypos > f1y && ypos < f1y + f1ht) //  fifth row
    {
      if (screen_no == 0 && !txstatus)     // bfo1 / 2 change only when in Receive mode
      {
        // left half button decreases bfo freq
        if (xpos > f2x && xpos < (f2x + f2wd / 2) - 2 ) // decrease freq
          bfo1_decr();
        // right half button increases bfo freq
        else if (xpos > (f2x + f2wd / 2) + 2 && xpos < f2x + f2wd ) // increase freq  (175,235)
          bfo1_incr();

        if (xpos > f4x && xpos < (f4x + f4wd / 2) - 2 ) // decrease freq
          bfo2_decr();
        // right half button increases bfo freq
        else if (xpos > (f4x + f4wd / 2) + 2 && xpos < f4x + f4wd ) // increase freq  (175,235)
          bfo2_incr();
      }


      else if (screen_no == 1 && !txstatus)  // change allowed in Rx mode only
      {
        if ( xpos > f2x && xpos < (f2x + f2wd)) // touch on button to toggle
          active_PTT_in = ! active_PTT_in;

        else if (xpos > f3x && xpos < f3x + f3wd)    // touch on TO buttom
          Tx_timeout_mode = !Tx_timeout_mode;

        else if (xpos > f4x && xpos < (f4x + f4wd / 2) - 2) // Tx time out button RED / Green, Mode activated, timer starts on PTT
        {
          Tx_timeout = Tx_timeout - 1;   // decrease Time out by 1 sec
          if (Tx_timeout <= 0)
            Tx_timeout = 1; // min 1 sec
        }

        else if ((  xpos > (f4x + f4wd / 2) + 2 ) && xpos < f4x + f4wd) // increase Time out
        {
          Tx_timeout = Tx_timeout + 1;
          if (Tx_timeout > max_timeout)  // max timeout defined in used defs
            Tx_timeout = max_timeout;
        }
      }


      else if (screen_no == 2 && !txstatus)  // only change in Rx mode
      {
        if (xpos > f2x && xpos < (f2x + f2wd / 2) - 2 ) // decrease offset for currently displayed band
        {
          offsets[bnd_count] = offsets[bnd_count] - radix;
          set_vfo();
        }
        else if (xpos > (f2x + f2wd / 2) + 2 && xpos < f2x + f2wd ) // increase offset for currently displayed band
        {
          offsets[bnd_count] = offsets[bnd_count] + radix;
          set_vfo();
        }

        else if (xpos > f4x && xpos < (f4x + f4wd / 2) - 2) // ts_delay decrement
        {
          ts_delay--;   // decrease delay Time
          if (ts_delay < 0)
            ts_delay = 80;   // typical val 80 ms
        }

        else if ((xpos > (f4x + f4wd / 2) + 2 ) && xpos < f4x + f4wd) // increase Time out
        {
          ts_delay++;  // no upper limit
        }
      }

      else if (screen_no == 3)    // should change even during Tx
      {
        if (xpos > f1x && xpos < f1x + f1wd)    // touch on Min  button Capture current SM value as min value
          if ( !txstatus)
            SM_min = analogRead(SM_In);
          else
            PM_min = analogRead(PM_In);

        if (xpos > f2x && xpos < (f2x + f2wd / 2) - 2 ) // decrease min value
        {
          if ( !txstatus)
            SM_min =   SM_min - radix;
          else
            PM_min = PM_min - radix;
        }
        else if (xpos > (f2x + f2wd / 2) + 2 && xpos < f2x + f2wd ) // increase min value
        {
          if ( !txstatus)
            SM_min =   SM_min + radix;
          else
            PM_min = PM_min + radix;
        }

        if (xpos > f3x && xpos < f3x + f3wd)    // touch on Min  button Capture current SM value as min value
          if ( !txstatus)
            SM_max = analogRead(SM_In);
          else
            PM_max = analogRead(PM_In);

        else if (xpos > f4x && xpos < (f4x + f4wd / 2) - 2) // decrease max value
        {
          if ( !txstatus)
            SM_max =   SM_max - radix;
          else
            PM_max = PM_max - radix;
        }
        else if ((xpos > (f4x + f4wd / 2) + 2 ) && xpos < f4x + f4wd) // increase Time out
        {
          if ( !txstatus)
            SM_max =   SM_max + radix;
          else
            PM_max = PM_max + radix;
        }
      }
    }

    update_row5();
  }



  //=====================
  //$$$$ S Meter display Take average and display after Ssamp no of samples

  Ssamp++; // sample no
  if (!txstatus)
    Sval = analogRead(SM_In);   // read s-meter
  else
    Sval = analogRead(PM_In);  // read Power meter

  Savg = (Savg + Sval );
  if (Ssamp >= SM_speed)     // calc & display every few samples (SM_speed 2-10)
  {
    Savg = Savg / Ssamp;
    {
      Sens = map(Savg, SM_min, SM_max, 0, wd - 70); // play with SM_FullScale as per input to SM_In 70 for ver displ
      tft.fillRect(botx + 2, boty + 3, botwd - 8, botht - 5, BLACK);

      if (!txstatus)
        tft.fillRect(botx + 2, boty + 3, Sens - 4, botht - 5, GREEN);  // display S-meter
      else
        tft.fillRect(botx + 2, boty + 3, Sens - 4, botht - 5, PINK);  // display Power
    }
    Ssamp = 0;
    Savg = 0;
    tft.setTextColor(WHITE, BLUE);  // ver info
  }
  check_CAT();
}    // end of loop()


