#include "Hexapod.h"
#include "Constants.h"
//PUSH %, and %difference walking
void Hexapod::FSM(ServoManager StateControl, Controller ControlInst) {
 /*
  THIS FSM WILL ALLOW YOU TO CREATE NEW STATES THAT CAN GO IN AND OUT OF EACH OTHER DEPENDING
  I CAN ALSO PUT OTHER CLASS FUNCTIONS IN HERE
  */
  int event = ControlInst.EventType;
  //WHERE I WANT TO WALK, I PUT WHAT PUSH PERCENTAGE, WHAT 
  switch(event){
    case Idle: // if event == idle
    if(current_state != event){ // if its not already idle
      if(event == Calibration){ // and it is calibration
        //PUT INTO SLEEP FIRST THEN CALIBRATION
        current_state = Sleeping;
      }
      else{ //from anyother state, you can make it idle
      current_state = event;
      }
    }
    break;
    case Sleeping:
    if(current_state != event){ // if not already sleeping
      if(event == Walking || event == Stationary){  // if I want to walk or am in stationary
        current_state = Idle;
      }
      else{
        current_state = event;  //else from any other state, make it sleep
      }
    }
    break;
    case Walking:
    if(current_state != event){ // if its not, make it walk, if it is, then donothing
      if(event == Sleeping || Calibration){
        current_state = Idle;
      }
      else{
      current_state = event;
      }
    }
    break;
    case Stationary:
    if(current_state != event){// if sleeping, move into idle before doing anything
      if(event == Sleeping || Calibration){
        current_state = Idle;
      }
      else{
      current_state = event;
      }
    }
    break;
    case Calibration:
    if(current_state != event){
      if(event == Walking || Stationary){
        current_state = Sleeping;
      }
      else{
      current_state = event;
      }
    }
    break;
  }
}

void Hexapod::update(ServoManager StateControl, Controller ControlInst) {

  Controller Control;
    switch(this->current_state){
case Idle:
//PICK A STANCE FOR IDLE MODE
StateControl.IdleMode();
break;
case Sleeping:
//THIS SHOULD BE EVERYTHING ALLTHE WAY DOWN, COXA = 90, FEMUR = (#), THEN TIBIA = 180
StateControl.SleepMode();
break;

case Walking:

this->changeGaits(Control.gaitbuttonPressed);
if(Control.MoveForward){
  if(Control.MoveForwardVal > 0){
    //FORWARD
    this->walkFSM(this->current_gait, 1, false, StateControl, ControlInst);
  }
  else if(Control.MoveForwardVal < 0){
    //BACKWARD
    this->walkFSM(this->current_gait, -1, false, StateControl, ControlInst);
  }
}
else if(!Control.MoveForward){
  if(Control.Strafe && Control.strafeAngle > 2){
    //STRAFE LEFT
     this->walkFSM(this->current_gait, 1, Control.Strafe, StateControl, ControlInst);
  }
   else if(Control.Strafe && Control.strafeAngle < 2){
    //STRAFE RIGHT
    this->walkFSM(this->current_gait, -1, Control.Strafe, StateControl, ControlInst);
   }
}
//FSM + BUTTON TO CHANGE WALKING, GAIT CHANGE ONLY POSSIBLE DURING IDLE???
break;

case Stationary:
//THIS SHOULD BE REALLY COOL, STAYS STILL AND CAN EITHER MOVE LEGS, EMOTE, CROUCH, BACK 

break;
case Calibration:
//BE IN SLEEP MODE AND CALIBRATION TO GET CERTAIN VALUES
break;

}
}

void Hexapod::walkFSM(Gaits Gait, int direction, bool Strafe, ServoManager StateControl, Controller ControlInst){
  //YOU NEED TO SET YOUR INDEX FOR EACH ONLY ONCE THO
  Bezier Bez;
  switch(Gait){
    case TriPodGait:
      int Speed1 = 10;
      int PushPercentage1 = 0;
      float delayPercent1 = 0.5;
      if(ControlInst.gaitSetup = true){
      StateControl.offsetLegs(Bez.total, delayPercent1);
      }
      int delay1 = Bez.SplitCurve(Speed1, PushPercentage1);
      //TriPod(Bez.FullBezierCurve, Speed1*direction, delay1, Bez.total, Strafe, delayPercent1);
    case RippleGait:
      int Speed2 = 12;
      int PushPercentage2 = 0;
      float delayPercent2 = 0.15;
           if(ControlInst.gaitSetup = true){
      StateControl.offsetLegs(Bez.total, delayPercent2);
      }
      int delay2 = Bez.SplitCurve(Speed2, PushPercentage2);
      //Ripple(Bez.FullBezierCurve, Speed2*direction, delay2, Bez.total, Strafe, delayPercent2);
    case WaveGait:
    int Speed3 = 5;
      int PushPercentage3 = 0;
      float delayPercent3 = .20;
           if(ControlInst.gaitSetup = true){
      StateControl.offsetLegs(Bez.total, delayPercent3);
      }
      int delay3 = Bez.SplitCurve(Speed3, PushPercentage3);
      //Wave(Bez.FullBezierCurve, Speed2*direction, delay2, Bez.total, Strafe, delayPercent3);
  }
}

void Hexapod::changeGaits(bool gaitbuttonPressed){
  if(gaitbuttonPressed && !this->gaitChanged){
    this->gaitChanged = true;
    this->current_gait = gaitArray[(this->current_gait+1)%this->TriPodGait];//should circle an array using gaits + enums
  }
  else if(gaitbuttonPressed && this->gaitChanged){
    this->gaitChanged = true;
  }
  else{
    this->gaitChanged = false;
  }
}

