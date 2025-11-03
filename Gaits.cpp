#include "Hexapod.h"
#include "Constants.h"

void ServoManager::Ripple(float Bez[][3], int Speed, int AdditionalDelay, int total, bool Strafe, float DelayPercent){

for(int i = 0; i< NUM_LEGS; i++){
  //this->MoveLeg(Bez, Speed, i, this->Angles[i], AdditionalDelay, 0, total);

}


}
void ServoManager::Bi(){


}
void ServoManager::Wave(float Bez[][3], int Speed, int AdditionalDelay, int total, bool Strafe, float DelayPercent){

int angle = 0;
//FIX ANGLES
for(int leg = 0; leg < NUM_LEGS; leg++){
  //Wave.MoveLeg(Bez, Speed, leg, angle, AdditionalDelay, total*DelayPercent*leg);

}


}

void ServoManager::TriPod(float Bez[][3], int Speed, int AdditionalDelay, int total, bool Strafe, float DelayPercent, int EdgeIndex[3]){

    for(int i = 0; i < NUM_LEGS; i++){
  this->MoveLeg(Bez, Speed, i, this->Angles[i],AdditionalDelay, 0, total, EdgeIndex);
    }
  
}
