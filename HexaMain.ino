#include "Hexapod.h"
#include "Reciever.h"
#include "debug.h"


#include <SPI.h>
#include <RF24.h>
#include <nRF24L01.h>

ServoManager Servos;
Hexapod Hexa;
Bezier Bez;
Controller Control;

//LEG AND CURVE PROPERTIES
int leg = 0;
int angle = 0;
int strafeAngle = 0;
int Speed = 5;
int point1[3] = {0, -TIBIALENGTH/10, FEMURLENGTH/10};
int point2[3] = {0, (TIBIALENGTH+120)/10, FEMURLENGTH/10};
int point3[3] = {90, 145, 170};
float PushPercentage = 0.25;
int AdditionalDelay = 0.0;
int index = 0;
constexpr int interpolation = Bez.total;
float ArrayLine[interpolation][3];
void setup() {
  // put your setup code here, to run once:
  Serial.begin(115200);
  radiosetup();

  //SET INITIAL BEZIER
  Bez.Curve(Bez.FullBezierCurve, Bez.BezierPoints, Bez.IPN, Bez.Z);
  Bez.CheckCurveEdges();
  AdditionalDelay = Bez.SplitCurve(Speed, PushPercentage);
  
  //Servos.SetupServos();
  Servos.pwm.begin();
  Servos.pwm.setPWMFreq(60);
        Servos.servo[0][1].attach(4, SERVOMIN, SERVOMAX);
        Servos.servo[1][1].attach(5, SERVOMIN, SERVOMAX);
        Servos.servo[1][0].attach(6, SERVOMIN, SERVOMAX);
        //Servos.servo[0][2].attach(12, SERVOMIN, SERVOMAX);
Servos.offsetLegs(Bez.total, .5);
//Bez.Line(ArrayLine, point1, point2, interpolation, FEMURLENGTH);

      //Servos.servo[0][2].attach(12, SERVOMIN, SERVOMAX);

//Servos.Random(ArrayLine, point1, point2, 5, FEMURLENGTH);
//SETS ALL SERVOS TO VECTOR 3
for(int i = 0; i<NUM_LEGS;i++){
  Servos.setServos(i, point3);
}
}
/*
CURRENT NOTES: YOU NEED TO CHECK IF YOUR FSM WORKS
YOU NEED TO MAKE SURE YOUR RECIEVING INPUTS AND THAT NOTHING STEPS ON EACH OTHER
THINGS TO CHECK
FSM
UPDATE
CONTROLLER
HEXAPOD IN RELATION TO CONTROLLER
YOU NEED TO MAKE SURE THAT YOU DONT SET INDEX MULTIPLE TIMES
YOU NEED TO MAKE CONTROL BEABLE TO WORK WITH GAIT

*/
void loop() {
  //RECIEVE INPUTS
  if(radio.available()){
    radio.read(&Control.radiovals, sizeof(Control.radiovals));
  }
  //TURN INPUTS INTO DIRECTIONS
  Control.checkInputs();
  //Hexa.FSM(0, Servos, Control ); // Actual FSM
  //Hexa.update(Servos, Control); // What happens when in a certain State 
  //NOTE : FIGURE OUT IF ADDING A STRAFE ANGLE WILL CORRECTLY ALLOW IT TO STRAFE THE RIGHT WAY
  //NOTE : GAITS ARE THE SIMULTANEOUS CONFIGURATION AND MOVEMENT OF ALL SIX LEGS, WHAT IT SHOULD TURN INTO LATER.

  //DEBUG FUNCTIONS
      //CheckControllerVals(Control.radiovals);
      //checkAngles(angle, Control.rotationAngle, Control.totalAngle, Control.strafeAngle);
      //CheckServoJointPos(Servos.JointPositions);
      //CheckAdditionalDelay(Bez.EdgeIndex, Bez.total);


  //TURNING DIRECTIONS INTO MOVEMENT
      //Servos.Turn(Bez.FullBezierCurve, interpolation);
      //Servos.TriPod(Bez.FullBezierCurve, Speed, AdditionalDelay, Bez.total, 0, 0, Bez.EdgeIndex);
      //Servos.Ripple(Bez.FullBezierCurve, Speed, 0, Bez.total, 0, 0);

      //Servos.Random(ArrayLine, point1, point2, 5, FEMURLENGTH);
      //Servos.IdleMode();
  //USE THIS AS A LEG CHECK
      //Servos.MoveLeg(Bez.FullBezierCurve, Speed, 0, -50, 0, index, Bez.total);
}
