#include "Hexapod.h"
#include "Constants.h"
#include "functions.h"
//ADD A STARTING POINT SO LEGS CAN STAR SOMEWHERE ELSE WHEN STARTING TO MOVE, CURRENTLY SPEED DONT DO NOTHING
void ServoManager::MoveLeg(float FullBezierCurve[][3],  Servo Leg[3], int Speed, int legjoint, int angle, int AdditionalDelay, int Index){
  Bezier BezInst;
  int total = BezInst.total;// CHANGE WHEN IPN CHANGES

  //This will go through the whole bezier curve, change it into angles and then write it to the servos over a period of time
  int IKArray[3] = {90,90,90};
    int i = ((this->JointPositions[legjoint]) + Index) % total;  //change for ipn
    float newX = FullBezierCurve[i][0];
    float newZ = FullBezierCurve[i][2];
    if(angle != 0){
    newX = FullBezierCurve[i][0]*cos(angle*rad);
    newZ = FullBezierCurve[i][0]*sin(angle*rad) + FullBezierCurve[i][2];
    }
    InverseKinematics(newX, newZ, FullBezierCurve[i][1], IKArray);
      CheckInverse(IKArray);
      if(IKArray[0] <= 180 && IKArray[0]> 0){
        Leg[0].write(IKArray[0]);
        //Serial.print("THIS1: ");
        
        //Serial.println(IKArray[0]);
      }
      else{
        Serial.print("ERROR W/COXA: ");
        //Serial.println(IKArray[0]);
        if(IKArray[0] > 180){
          Leg[0].write(179);
        }
        else{
          Leg[0].write(1);
      }
      }
      if(IKArray[1] <= 180 && IKArray[1]> 0){
        Leg[1].write(IKArray[1]);
        //Serial.print("THIS2: ");
        //Serial.println(IKArray[1]);
      }
      else{
        Serial.print("ERROR W/FEMUR: ");
        //Serial.println(IKArray[1]);
        if(IKArray[1] > 180){
          Leg[1].write(179);
        }
        else{
          Leg[1].write(1);
        }
        }
      if(IKArray[2] <= 180 && IKArray[2]> 0){
        Leg[2].write(IKArray[2]);
        //Serial.print("THIS3: ");
        //Serial.println(IKArray[2]);
      }
      else{
        Serial.print("ERROR W/TIBIA: ");
        //Serial.println(IKArray[2]);
        if(IKArray[2] > 180){
          Leg[2].write(179);
        }
        else{
          Leg[2].write(1);
        }
      }

    Serial.print("IMPORTANT ");
    if(i> BezInst.EdgeIndex[0] && i< BezInst.EdgeIndex[1]){//in between edge0 and edge1
    delay(abs(Speed));
    }
    else{
    delay(abs(Speed)+AdditionalDelay);

    }
    if(Speed > 0){
    this->JointPositions[legjoint] = (i+1)%total;
    }
    else{
       this->JointPositions[legjoint] = (i-1+total)%total;
    };
    Serial.print(this->JointPositions[legjoint]);
    Serial.print(" NEW X: ");
    Serial.print(newX);
    Serial.print(" NEW Y: ");
    Serial.print(FullBezierCurve[i][1]);
    Serial.print(" NEW Z: ");
    Serial.println(newZ);
}

void ServoManager::SleepMode(){
  int Sleep[3] = {90, 100, 150};

  for(int i = 0; i< NUM_LEGS;i++){
    for(int j = 0; j< NUM_JOINTS; j++){
      servo[i][j].write(Sleep[j]);
    }
  }
}

void ServoManager::CalibrationMode(){
  int Calibration[3] = {90, 90, 0};

  for(int i = 0; i< NUM_LEGS;i++){
    for(int j = 0; j< NUM_JOINTS; j++){
      servo[i][j].write(Calibration[j]);
    }
  }
}
void ServoManager::IdleMode(){
//IDLE MODE SHOULD BE THREE PARTS, COXA, TIBIA, THEN FEMUR,
int Idle[3] = {0,0,0};
Bezier Bez;
InverseKinematics(0, 0, Bez.Z, Idle);
  for(int i = 0; i< NUM_LEGS;i++){
      this->servo[i][0].write(Idle[0]);
  }
  for(int i = 0; i< NUM_LEGS;i++){
      this->servo[i][2].write(Idle[2]);
  }
  for(int i = 0; i< NUM_LEGS;i++){
      this->servo[i][1].write(Idle[1]);
  }
}

void ServoManager::SetupServos(){
  for(int i = 0; i < NUM_LEGS; i++){
    for(int j = 0; j < NUM_JOINTS; j++){
  this->servo[i][j].attach(ServoPins[i][j], 500, 2400);
   delay(250);
    }
   
  }
}