#include "Hexapod.h"
#include "Constants.h"
#include "functions.h"
//ADD A STARTING POINT SO LEGS CAN STAR SOMEWHERE ELSE WHEN STARTING TO MOVE, CURRENTLY SPEED DONT DO NOTHING
void ServoManager::MoveLeg(float FullBezierCurve[][3],int Speed, int legjoint, int angle, int AdditionalDelay, int Index, int total, int EdgeIndex[3]){
  Bezier BezInst;
  //int total = Bezier::total;// CHANGE WHEN IPN CHANGES
  //Serial.println("HERE");
  //This will go through the whole bezier curve, change it into angles and then write it to the servos over a period of time
  int IKArray[3] = {90,90,90};
    int i = ((this->JointPositions[legjoint])) % total;  //change for ipn
    float newX = FullBezierCurve[i][0];
    float newZ = FullBezierCurve[i][2];
    if(angle != 0){
    newX = FullBezierCurve[i][0]*cos(angle*rad);
    newZ = FullBezierCurve[i][0]*sin(angle*rad) + FullBezierCurve[i][2];
    }
    InverseKinematics(newX, FullBezierCurve[i][1], newZ, IKArray);
      //CheckInverse(IKArray);
      this->setServos(legjoint, IKArray);
    //Serial.println("IMPORTANT ");
    if(i> EdgeIndex[0] && i< EdgeIndex[1]){//in between edge0 and edge1
    delay(abs(Speed));
    Serial.println("IDIDIDNTO");
    }
    else{
      Serial.println("I DID DID");
      Serial.println(AdditionalDelay);
    delay(abs(Speed)+AdditionalDelay);

    }
    if(Speed > 0){
    this->JointPositions[legjoint] = (i+1)%total;
    }
    else{
       this->JointPositions[legjoint] = (i-1+total)%total;
    };

}

void ServoManager::SleepMode(){
  constexpr int interpolation = 5;
  float ArrayLine[interpolation][3];
  int point1[3] = {0, -TIBIALENGTH/10, FEMURLENGTH/10};
int point2[3] = {0, (TIBIALENGTH+120)/10, FEMURLENGTH/10};
  this->Random(ArrayLine, point1, point2, interpolation, FEMURLENGTH);
  delay(500);
  int Sleep[3] = {90, 145, 170};
  for(int i = 0; i< NUM_LEGS;i++){
    setServos(i, Sleep);
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
InverseKinematics(0, -TIBIALENGTH, FEMURLENGTH/2, Idle);

 // Serial.print("HERE");
  for(int i = 0; i< 6;i++){
      this->setServos(i, Idle);
      
}}

void ServoManager::Random(float ArrayLine[][3], int point1[3], int point2[3], int interpolation, int Axis){

  int EdgeTemp[3] = {0,0,0};
  for(int i = 0; i< interpolation;i++){
    Serial.print("ONE "); Serial.println(ArrayLine[i][0]); Serial.print("  "); Serial.println(i);
    Serial.print("TWO "); Serial.println(ArrayLine[i][1]);
    Serial.print("THREE "); Serial.println(ArrayLine[i][2]);
    for(int j = 0; j<NUM_LEGS;j++){
      delay(50);
      this->MoveLeg(ArrayLine, 30, j, 0, 0, i, interpolation, EdgeTemp);
      
    }
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
void ServoManager::Turn(float ArrayLine[][3], int interpolation){


  for(int i = 0; i< interpolation;i++){
    for(int j = 0; j<NUM_LEGS;j++){
      //this->MoveLeg(ArrayLine, 10, j, 0, 0, i, interpolation);
      
    }
  }
}
void ServoManager::setServos(int legjoint, int IKArray[]){
      //Serial.print("LEGJOINT: "); Serial.print(legjoint); Serial.println(" ");
      int TempArray[3] = {0,0,0};
      TempArray[0] = IKArray[0];
      TempArray[1] = IKArray[1];
      TempArray[2] = IKArray[2];

  for(int i = 0; i < 3 ;i++){
  if(this->ServoIDs[legjoint][i].type == Arduino){
      if(legjoint < 3){
        TempArray[i] = 180-IKArray[i];
        //Serial.print("MIRRORED");
        
        this->servo[legjoint-2][i].write(boundaries(TempArray[i],this->ServoIDs[legjoint][i].type));
      }
      else{
  
      this->servo[legjoint-2][i].write(boundaries(TempArray[i],this->ServoIDs[legjoint][i].type));

      }
      }
  else{

    if(legjoint <3 ){

        TempArray[i] = 180-IKArray[i];

         this->pwm.setPWM(this->ServoIDs[legjoint][i].pin, 0, boundaries(TempArray[i],this->ServoIDs[legjoint][i].type));
        //Serial.print(boundaries(IKArray[i],this->ServoIDs[legjoint][i].type));
    }
    else{
      //Serial.print("Before 1"); Serial.print(TempArray[0]);  Serial.print(" ");
  
         this->pwm.setPWM(this->ServoIDs[legjoint][i].pin, 0, boundaries(TempArray[i],this->ServoIDs[legjoint][i].type));
               //Serial.print("Before1 "); Serial.print(boundaries(TempArray[i],this->ServoIDs[legjoint][i].type));  Serial.print(" ");

        //Serial.print(boundaries(IKArray[i],this->ServoIDs[legjoint][i].type));
    }
      }
  }
}

void ServoManager::offsetLegs(int total, float delayPercentage){
  for(int j = 0; j<NUM_LEGS;j++){
      this->JointPositions[j] = 0;
  }
  for(int i = 0; i<NUM_LEGS;i++){
    if(i == 0){
  this->JointPositions[i] = 0;
    }
    else{
  this->JointPositions[i] = this->JointPositions[i-1] + total*delayPercentage;
    }
  }

}


