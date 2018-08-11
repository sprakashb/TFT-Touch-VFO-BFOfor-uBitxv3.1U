//-----------------------USER SELECTABLE DEFINITIONS---------------------------------------------------------------
//#define debug // when needed

// Various displays
//#define         elegoo923         // Joe's 923elegoo displ
//#define         MCUF0x154  // VU2SPF's test display
//#define         PL0x9341    // VU2SPF's Potential Lab display BUT syst
#define         VE0x7783    // Ventor Tech display SPB/ubitx
//#define           REJI5408      // Rejimon's display from Robodo detected as 5408 but works as 9320
//#define           IL9325      // Robodo 1/18
//#define     Sa35_9486   // sarmas 3.5 inch
//#define        pl0x2053  // poten lab 0x9341 now shows as 2053??
// Si5351 related
#define           si5351correction 0   // IF THERE IS ANY (check using calibrate program in the etherkit Si5351 library examples)


// pin allocations for ubitx digital interface 
#define TX_RX    14
#define CW_TONE  15
#define TX_LPF_A 16
#define TX_LPF_B 17
#define TX_LPF_C 18
#define CW_KEY   19     // 20,21 are SDA/SCL (rev order on ubitx interface board v1.0) so rest pins in sequence on Arduino Mega 

// B U T T O N S
// If external buttons are needed in parallel with TFT touch buttons then one cn connect required ones as per allocation below. 
// One can edit the pin numbers as per our convenience
// pin allocations for OPTIONAL input buttons (User selectable ), one pin of button goes to gnd and the other to following Arduino pin
// There are many unused pins still available for including many leftout functions, needing changes in program.
#define BandSelectUp    30  // band change button moves the band from lower to higher frequency side and rotates through all bands sequentally.(only increment)
#define BandSelectDn    31  // band change button moves the band from higher to lower frequency side and rotates through all bands sequentally.(only decrement)
#define MEMUp           32  // Changing memory channel from 1 to 100 in sequence (increment)
#define MEMDn           33  // Changing memory channel from 100 to 1 in sequence (decrement)

#define SideBandSelect 34  // USB or LSB 
#define SAVE          35  //for saving in memory the current parameters like freq, sideband, bfo's on VFO A/B or selected Memory channels
#define VFO           36  // Selecting A/B/M VFO's
#define STEPUp        37  // Selecting Step Size for incrementing frequency 1Hz to 1 MHz 
#define STEPDn        38  // decrease Step size from 1MHz to 1 Hz 
#define VtoMEM        39  // Copying currently selected VFO into currently defined Memory channel
#define MEMtoV        40  // copy from mem to current VFO

#define TxTmOut       42 // Activating Tx Time out 
#define SPLIT         43  // Split mode ON/OFF
#define F3            44
#define F4            42


// External encoder for frequency selection is connected on pins defined below.
#define ENCODER_A     67    // Encoder pin A is A13 on Mega
#define ENCODER_B     68    // Encoder pin B is A14 on Mega
#define ENCODER_BTN   37    // push button switch on encoder 


// How S-meter is connected and maximum range of display, The Smeter is not calibrated and one has to match it with known ones and set parameter below
#define SM_In        A12    // Analog Pin A12 for the S Meter function (66)
#define SM_speed 5    // speed of updating S meter
// to be set by trial

#define PM_In   A11  // Analog Pin A11 for Power meter FWD input (3.1cU)
#define PM_In_Ref A10  // Analog A10

// Due to variations in xtal freq of 5351, the actual frequency of various VFO/BFO may not be on the dot. By receiving station on a known freq (eg a net) find the difference (offset) and put below
// if there are offsets which are not same on each band the following offsets should help in setting exact frequency

//int32_t Offset_VFO=0, Offset_80m=0, Offset_40m=-1500, Offset_30m=0, Offset_20m=-1700, Offset_17m=0, Offset_15m=0,Offset_12m=0,Offset_10m=0;
// offsets for different bands could be -ve also & to be found by experimentation for each rig 



// TFT Hardware related (most mcufriend displays follow these pinouts, but better check)
#define LCD_CS      A3 // Chip Select goes to Analog 3    //**? Are these fixed
#define LCD_CD      A2 // Command/Data goes to Analog 2
#define LCD_WR      A1 // LCD Write goes to Analog 1
#define LCD_RD      A0 // LCD Read goes to Analog 0
#define LCD_RESET   A4 // Can alternately just connect to Arduino's reset pin



