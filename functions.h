#ifndef FUNCTIONS_H
#define FUNCTIONS_H
#include "Constants.h"
//JoystickMapping
//PotentiometerMapping
//BezierCurveCalculations
void InverseKinematics(float X, float Y, float Z,int Array[3]){
  int FemurLength = FEMURLENGTH;
  int TibiaLength = TIBIALENGTH;
  int C = 90 - atan(X/Z)*(180/3.14);
  float H = sqrt( (Z*Z)+(X*X));
  float L = sqrt( (H*H)+(Y*Y));
  int B = 180 - (acos((FemurLength*FemurLength + TibiaLength*TibiaLength - L*L)/( 2*FemurLength*TibiaLength))* (180/3.14));
  float G = (acos((L*L + FemurLength*FemurLength - TibiaLength*TibiaLength)/ (2*L*FemurLength))*(180/3.14));
  float F = atan(Y/H)*(180/3.14);
  int A = (G + F);
  Array[0] = C; //Coxa
  Array[1] = A; //Femur
  Array[2] = B; //Tibia

  }

void CheckInverse(int IKArray[3]){

     
    Serial.print("A ");
    Serial.print(IKArray[0]);
    Serial.print(" B ");
    Serial.print(IKArray[1]);
    Serial.print(" C ");
    Serial.print(IKArray[2]);
    
}
  
#endif