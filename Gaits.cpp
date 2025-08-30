#include "Hexapod.h"
#include "Constants.h"

void Hexapod::Ripple(float Bez[][3], int Speed, int AdditionalDelay, int total, bool Strafe, float DelayPercent){




}
void Hexapod::Bi(){


}
void Hexapod::Wave(float Bez[][3], int Speed, int AdditionalDelay, int total, bool Strafe, float DelayPercent){
ServoManager Wave;
int angle = 0;
//FIX ANGLES
for(int leg = 0; leg < NUM_LEGS; leg++){
  Wave.MoveLeg(Bez,Wave.servo[leg], Speed, leg, angle, AdditionalDelay, total*DelayPercent*leg);

}


}

void Hexapod::TriPod(float Bez[][3], int Speed, int AdditionalDelay, int total, bool Strafe, float DelayPercent){
  ServoManager TP;
int Groups[2][3] = {{1,5,3}, {4,2,6}};
int angle = 0;

for(int i = 0; i < 3;i++){
  int leg = Groups[0][i];
    if(Strafe){
      angle = 90;
    }
    else{
      angle = TP.angle[i];
    }
  TP.MoveLeg(Bez,TP.servo[leg], Speed, leg-1, angle, AdditionalDelay, 0);
  
}
for(int j = 0; j < 3; j++){
  int leg = Groups[1][j];
   
    if(Strafe){
      angle = 90;
    }
    else{
      angle = TP.angle[j];
    }
  TP.MoveLeg(Bez,TP.servo[leg], Speed, leg-1, angle, AdditionalDelay, total*DelayPercent);
}

}
