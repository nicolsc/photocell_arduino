#include "AtmSIGFOXshd.h"
SIGFOXshield SIGsh;          // load class SIGFOXshield


#define PHOTO_PIN A0

#define DEFAULT_SAMPLE_RATE 600000 //default 10 mins

#define DARK_THRESHOLD 10
#define DIM_THRESHOLD 200
#define LIGHT_THRESHOLD 500
#define BRIGHT_THRESHOLD 800

int brightness;
long sampleRate = DEFAULT_SAMPLE_RATE;
int temperature;

unsigned char sigfoxMsg[3];

void setup(void) {
  Serial.begin(9600);   
}
     
void loop(void) {
  brightness = analogRead(PHOTO_PIN); 
  Serial.print(brightness);
  if (brightness < DARK_THRESHOLD) {
    Serial.println(" - Dark");
  }
  else if (brightness < DIM_THRESHOLD) {
    Serial.println(" - Dim");
  }
  else if (brightness < LIGHT_THRESHOLD) {
    Serial.println(" - Light");
  }
  else if (brightness < BRIGHT_THRESHOLD) {
    Serial.println(" - Bright");
  } 
  else {
    Serial.println(" - Very bright");
  }
  
  SIGsh.SIGFOXon();                // switch on SIGFOX module and temp. module
  SIGsh.TempOn();
  temperature = (int)SIGsh.readTemp();
      SIGsh.TempOff();   
  
  sigfoxMsg[0] = temperature;
  sigfoxMsg[1] = highByte(brightness);
  sigfoxMsg[2] = lowByte(brightness);
  
  int ret=SIGsh.sendMessage(sigfoxMsg,sizeof(sigfoxMsg));     // send buffer to SIGFOX network
    Serial.println(SIGsh.getStatusSig());
    if (ret>0) Serial.println("No transmission!");
    
  
  Serial.print("Next sample in ");
  Serial.print(sampleRate);
  Serial.println("ms");


    
  SIGsh.SIGFOXoff();       
  delay(sampleRate);
  
  

}
