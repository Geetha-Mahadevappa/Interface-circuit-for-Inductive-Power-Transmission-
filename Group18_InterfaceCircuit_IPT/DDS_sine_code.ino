// Include the library code
#include <EEPROM.h>

//Setup some items
#define W_CLK 8   // Pin 8 - connect to AD9850 module word load clock pin (CLK)
#define FQ_UD 9   // Pin 9 - connect to freq update pin (FQ)
#define DATA 11   // Pin 11 - connect to serial data load pin (DATA)
#define RESET 10  // Pin 10 - connect to reset pin (RST) 
#define pulseHigh(pin) {digitalWrite(pin, HIGH); digitalWrite(pin, LOW); }
double rx=0000000; // Base (starting) frequency of VFO.  This only loads once.  To force load again see ForceFreq variable below.
double rx2=1; // variable to hold the updated frequency
int_fast32_t iffreq = 0000000; // Intermediate Frequency - Amount to subtract from base frequency
int buttonstate = 0;
int buttonstate2 = 0;
int GoIF = 1;
byte ones,tens,hundreds,thousands,tenthousands,hundredthousands,millions ;  //Placeholders
String freq; // string to hold the frequency
int_fast32_t timepassed = millis(); // int to hold the arduino miilis since startup
int memstatus = 1;  // value to notify if memory is current or old. 0=old, 1=current
int ForceFreq = 1;  // Change this to 0 after you upload and run a working sketch to activate the EEPROM memory.  YOU MUST PUT THIS BACK TO 0 AND UPLOAD THE SKETCH AGAIN AFTER STARTING FREQUENCY IS SET!

void setup() {
  pinMode(A0,INPUT); // Connect to a button that goes to GND on push
  pinMode(A5,INPUT); // IF sense **********************************************
  digitalWrite(A0,HIGH);
  digitalWrite(A5,HIGH);
  PCICR |= (1 << PCIE2);
  PCMSK2 |= (1 << PCINT18) | (1 << PCINT19);
  sei();
  pinMode(FQ_UD, OUTPUT);
  pinMode(W_CLK, OUTPUT);
  pinMode(DATA, OUTPUT);
  pinMode(RESET, OUTPUT); 
  pulseHigh(RESET);
  pulseHigh(W_CLK);
  pulseHigh(FQ_UD);  // this pulse enables serial mode on the AD9850 - Datasheet page 12
 
  // Load the stored frequency  
  if (ForceFreq == 0) {
    freq = String(EEPROM.read(0))+String(EEPROM.read(1))+String(EEPROM.read(2))+String(EEPROM.read(3))+String(EEPROM.read(4))+String(EEPROM.read(5))+String(EEPROM.read(6));
    rx = freq.toInt();  
  }
  rx=1000000;
}

void loop() {    
  // Update the display and frequency if the new Freq NEQ the old Freq  
  if (rx != rx2){
    sendFrequency(rx);
    rx2 = rx;        
  }
    
  // Check for PIN low to drive IF offset Freq
  buttonstate = digitalRead(A5);  
    if (buttonstate != buttonstate2){
        if(buttonstate == LOW) {
              GoIF = 0; 
              buttonstate2 = buttonstate; 
              sendFrequency(rx);         
        }
        else{
            GoIF = 1;
            buttonstate2 = buttonstate;
            sendFrequency(rx);       
        };
    };
    
    // Write the frequency to memory if not stored and 2 seconds have passed since the last frequency change.
    if(memstatus == 0){   
      if(timepassed+2000 < millis()){
        storeMEM();
      }
    }    
 }

// frequency calc from datasheet page 8 = <sys clock> * <frequency tuning word>/2^32
void sendFrequency(double frequency) {  
  if (GoIF == 1){frequency=frequency-iffreq;}; //If pin = low, subtract the IF frequency.
  int32_t freq = frequency * 4294967295/125000000;  // note 125 MHz clock on 9850.  'slight' tuning variations can be done by adjusting the clock frequency
  for (int b=0; b<4; b++, freq>>=8) {
    tfr_byte(freq & 0xFF);
  }
  tfr_byte(0x000);   // Final control byte, all 0 for 9850 chip
  pulseHigh(FQ_UD);  // Done!  Should see output
}

// transfers a byte, a bit at a time, LSB first to the 9850 via serial DATA line
void tfr_byte(byte data)
{
  for (int i=0; i<8; i++, data>>=1) {
    digitalWrite(DATA, data & 0x01);
    pulseHigh(W_CLK);   //after each bit sent, CLK is pulsed high
  }
}

void storeMEM(){
  //Write each frequency section to a EEPROM slot
   EEPROM.write(0,millions);
   EEPROM.write(1,hundredthousands);
   EEPROM.write(2,tenthousands);
   EEPROM.write(3,thousands);
   EEPROM.write(4,hundreds);       
   EEPROM.write(5,tens);
   EEPROM.write(6,ones);   
   memstatus = 1;  // Let program know memory has been written
};
