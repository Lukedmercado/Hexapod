#ifndef RECIEVER_H
#define RECIEVER_H

#include <SPI.h>
#include <RF24.h>
#include <nRF24L01.h>

RF24 radio(7,8);
const byte address[5] = "12345";

void radiosetup(){
const byte address[5] = "12345";
   radio.begin();
  radio.openReadingPipe(0, address);
  radio.setPALevel(RF24_PA_MIN);
  radio.startListening();

}

void radiorecieve(int array[]){
  int txt[13] = {0,0,0,0,0,0,0,0,0,0,0,0,0};
  if(radio.available()){
    radio.read(&txt, sizeof(txt));
}
for(int i = 0; i < 13; i++){
  array[i] = txt[i];
}
}


#endif