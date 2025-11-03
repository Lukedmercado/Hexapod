#include "Hexapod.h"
#include "Constants.h"
static int Controller::radiovals [13];

  static int Controller::MoveForwardVal = 0;
  static bool Controller::MoveForward = false;
  static bool Controller::MoveBackward= false;

  static double Controller::strafeAngle = 0;
  static bool Controller::Strafe = false;

  static int Controller::rotationAngle = 0;
  static bool Controller::Rotation = false;

  static bool Controller::ChangeGaits = false;
  static bool Controller::gaitbuttonPressed = false;
  static bool Controller::statebuttonPressed = false;
    static bool Controller::gaitSetup = false;
  static double Controller::totalAngle = 0;

void Controller::checkInputs(){

  

  //create an inverse map of 10 to 100 for delay times of 60 to 20, 20 being the fastest, 

  //CURRENTLY this just tells me if I can move forward back or not at all.
  //FORWARD
  if(this->radiovals[0] > 2){
    //walkForwards
    this->MoveForwardVal = this->radiovals[0];
    this->MoveForward = true;
  }
  else if(this->radiovals[0] < -2){
    this->MoveForwardVal = this->radiovals[0];
    this->MoveBackward = true;
  }
  else{
    this->MoveForwardVal = 0;
    this->MoveForward = false;
    this->MoveBackward = false;
  }

//STRAFE
  if(this->radiovals[1] > 2 || this->radiovals[1] < -2){

    double rawStrafe = atan2(this->radiovals[1] , this->radiovals[0])/rad;
    //WELL FIND OUT IF THIS WORKS, WHEN WE STRAFE WHILE GOING BACKWARDS LOL
    if(this->radiovals[0]< 0){
      if(this->radiovals[1] < 0){
        rawStrafe +=180;
      }
      else{
        rawStrafe -=180;
      }

    }
    this->strafeAngle = rawStrafe;
    this->totalAngle = this->strafeAngle + this->rotationAngle;
    this->Strafe = true;
    //LIMIT THE ANGLE
  }
  else{
    this->strafeAngle = 0;
    this->Strafe = false;
    this->totalAngle = this->rotationAngle;
  }
  /*
  TO ROTATE
  if joystick value Y is over a certain value, then begin to rotate, rotate faster depending on how much of the joystick it is. 
  If it is already moving/strafing, what should change is that how much the angle is changing should change depending on how much the joystick is

  SIMPLE ROTATION: Left and right, which will allow it to turn infinitely from how its moving
  MORE COMPLEX ROTATION: STRAIGHT FORWARD NO ROTATION, VARIABLE ROTATION DEPENDING ON HOW FAR IT IS FROM THE LEFT/RIGHT

  take input from joystick on left or right, find how much it desires to turn based on yaxis of joystick
  create a map of joystick values to turning speed, this turning speed will be how mcuh the angle changes, it can subtract from the current strafe angle, or
  */

if(this->radiovals[2] > 20 || this->radiovals[2] < -20){
    //rotation speed calculated by radiovals2
    double rotationSpeed = ((this->radiovals[2])/100);
      this->rotationAngle += rotationSpeed;
      if(this->rotationAngle > 90){
        this->rotationAngle = 89;
      }
      if(this->rotationAngle < -90){
        this->rotationAngle = -89;
      }
    this->Rotation = true;
  }
  else if((this->radiovals[2] < 20 && this->radiovals[2] > -20) && this->Rotation == true){
    this->Rotation = false;

  }
//DO ONE FOR STATE CHANGE AS WELL
if(this->radiovals[7] == 1 && gaitbuttonPressed == false){
  this->gaitbuttonPressed = true;
  this->gaitSetup = true;
  this->EventType = (this->EventType+1)%Calibration;
}
else if(this->radiovals[7] == 1 && gaitbuttonPressed == true){
  this->gaitbuttonPressed = true;
  this->gaitSetup = false;
}
else{
  this->gaitbuttonPressed = false;
  this->gaitSetup = false;
}
//DO ONE FOR STATE CHANGE AS WELL


  //check first joystick for strafe values
  //check second joystick for rotation values
  //check buttons for gait switches


}

bool canMoveForward(){


}