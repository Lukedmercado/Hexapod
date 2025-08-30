#include "Hexapod.h"
#include "Reciever.h"
#include "debug.h"


#include <SPI.h>
#include <RF24.h>
#include <nRF24L01.h>
Hexapod Hexa;
ServoManager Servos;
Bezier Bez;
Controller Control;

//LEG AND CURVE PROPERTIES
int leg = 3;
int angle = 0;
int strafeAngle = 0;
int Speed = 33;
float PushPercentage = 0.55;
int AdditionalDelay = 0;
int index = 0;

void setup() {
  // put your setup code here, to run once:
  Serial.begin(115200);
  radiosetup();
  Bez.Curve(Bez.FullBezierCurve, Bez.BezierPoints, Bez.IPN, Bez.Z);
  AdditionalDelay = Bez.SplitCurve(Speed, PushPercentage);
  //Servos.SetupServos();
  Servos.servo[3][0].attach(10, 550, 2400);
  Servos.servo[3][1].attach(11, 550, 2400);
  Servos.servo[3][2].attach(5, 550, 2400);
  
}

void loop() {
  //RECIEVE INPUTS
  if(radio.available()){
    radio.read(&Control.radiovals, sizeof(Control.radiovals));
  }
  //TURN INPUTS INTO DIRECTIONS
  Control.checkInputs();

  //NOTE : FIGURE OUT IF ADDING A STRAFE ANGLE WILL CORRECTLY ALLOW IT TO STRAFE THE RIGHT WAY
  //NOTE : GAITS ARE THE SIMULTANEOUS CONFIGURATION AND MOVEMENT OF ALL SIX LEGS, WHAT IT SHOULD TURN INTO LATER.

  //DEBUG FUNCTIONS
    //CheckControllerVals(Control.radiovals);
    //checkAngles(angle, Control.rotationAngle, Control.totalAngle, Control.strafeAngle);
  //TURNING DIRECTIONS INTO MOVEMENT

//PART UNDERNEATH SHOULD BE BUILT INTO HEXAPOD.UPDATE
  if(Control.MoveForward){
    Serial.print("Forward");
    Servos.MoveLeg(Bez.FullBezierCurve,Servos.servo[leg], 60, leg, angle+Control.totalAngle, AdditionalDelay, index);
  }
  else if(Control.MoveBackward){
    Serial.print("Backward ");
    //WalkBackwards
    Servos.MoveLeg(Bez.FullBezierCurve,Servos.servo[leg], -60, leg, angle+Control.totalAngle, AdditionalDelay, index);
  }
  else if(!Control.MoveForward){
    if(Control.Strafe && Control.strafeAngle > 2){
      Servos.MoveLeg(Bez.FullBezierCurve,Servos.servo[leg], 60, leg, 90, AdditionalDelay, index);
    }
    else if(Control.Strafe && Control.strafeAngle < 2){
      Servos.MoveLeg(Bez.FullBezierCurve,Servos.servo[leg], 60, leg, -90, AdditionalDelay, index);
    }
  } 
//Hexa.changeGaits(Control.gaitbuttonPressed); NOT NEEDED ANYMORE

//Control.FSM, should be part above

//Hexa.FSM(0); // Actual FSM
//Hexa.update(Control.gaitbuttonPressed); // What happens when in a certain State


}
