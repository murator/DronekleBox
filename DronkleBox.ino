/*
***************************************************
-----STANDUINO-------DRONEKLE BOX------------------
***************************************************
 
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
 
Drone software based on Mozzi library,
written for the hacked fraAngelico synths
made at Steim, workshop 6-7.4.2013, Amsterdam Netherlands
5 oscilators connected in several ways 
for each one you can select the wavetable

ideas collected with workshop participants
workshop guided by Ondrej Merta and Vaclav Pelousek

www.standuino.eu
www.steim.org
 
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

*/

#include <MozziGuts.h>
#include <Oscil.h> // oscillator template
#include <LowPassFilter.h>
#include <mozzi_analog.h>

#include <tables/sin2048_int8.h> // sine table for oscillator
#include <tables/saw2048_int8.h> // sine table for oscillator
#include <tables/triangle2048_int8.h> // sine table for oscillator

// use: Oscil <table_size, update_rate> oscilName (wavetable)

Oscil <SIN2048_NUM_CELLS, AUDIO_RATE> aSin(SIN2048_DATA);
Oscil <SIN2048_NUM_CELLS, AUDIO_RATE> bSin(SAW2048_DATA);
Oscil <SIN2048_NUM_CELLS, AUDIO_RATE> cSin(TRIANGLE2048_DATA);
Oscil <SIN2048_NUM_CELLS, AUDIO_RATE> dSin(SAW2048_DATA);
Oscil <SIN2048_NUM_CELLS, AUDIO_RATE> eSin(SIN2048_DATA);

const char* waveTable[]={
SIN2048_DATA, SAW2048_DATA, TRIANGLE2048_DATA};
byte buttonPin[7]={
2,4,8,7,11,10,3};
boolean buttonState[7];
boolean justPressed[7];
boolean lastButtonState[7];

byte waveTableA,waveTableB,waveTableC,waveTableD,waveTableE;

// use #define for CONTROL_RATE, not a constant
#define CONTROL_RATE 32 // powers of 2 please
LowPassFilter lpf;

void setup(){
  startMozzi(CONTROL_RATE); // set a control rate of 64 (powers of 2 please)
  // set the frequency with an unsigned int or a float
  lpf.setResonance(200);
  //  setupFastAnalogRead();
  aSin.setFreq(float(440));
  Serial.begin(9600);
}

void updateControl(){

  ReadButtons();

  if(justPressed[0]){
    waveTableA++;
    if(waveTableA>2) waveTableA=0;
    aSin.setTable(waveTable[waveTableA]);
  }

  if(justPressed[1]){
    waveTableB++;
    if(waveTableB>2) waveTableB=0;
    bSin.setTable(waveTable[waveTableB]);
  }

  if(justPressed[2]) {
    waveTableC++;
    if(waveTableC>2) waveTableC=0;
    cSin.setTable(waveTable[waveTableC]);
  }

  if(justPressed[3]){
    waveTableD++;
    if(waveTableD>2) waveTableD=0;
    dSin.setTable(waveTable[waveTableD]);
  }

  if(justPressed[4]) {
    waveTableE++;
    if(waveTableE>2) waveTableE=0;
    eSin.setTable(waveTable[waveTableE]);
  }

  //random(100,1000);
  cSin.setFreq(float(random(100,analogRead(0))));
  bSin.setFreq(float(analogRead(1)));
  aSin.setFreq(float(analogRead(2)));
  //cSin.setFreq(float(analogRead(2)));
  dSin.setFreq(float(analogRead(3)+eSin.next()));
  eSin.next();
  eSin.next();
  eSin.next();
  eSin.next();
  eSin.setFreq(float(analogRead(4)));

  //  lpf.setCutoffFreq(analogRead(2)>>2);
  // put changing controls in here
}

int updateAudio(){
  return  ((aSin.next()*bSin.next())>>8 +dSin.next()+cSin.next())>>4; // return an int signal centred around 0
}

void loop(){
  audioHook(); // required here
}

void ReadButtons(){

  for(int i=0;i<7;i++){
    pinMode(buttonPin[i],INPUT);
    buttonState[i]=digitalRead(buttonPin[i]);
    justPressed[i]=false;
    if(buttonState[i]==true && lastButtonState[i]==false) justPressed[i]=true;
    lastButtonState[i]=buttonState[i];
    //debugRoutine
    if(justPressed[i]) Serial.println(i,DEC);
  }

}




