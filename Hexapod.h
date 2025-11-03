#ifndef HEXAPOD_H
#define HEXAPOD_H
#include <Arduino.h>
#include <Servo.h>
#include <Wire.h>
#include <Adafruit_PWMServoDriver.h>
#include "Constants.h"

enum MotorController {Arduino, PCA};
struct Motor{
  MotorController type;
  int pin;

};

enum States{
Idle,
Sleeping,
Walking,
Stationary,
Calibration,
};

class ServoManager{
  public:

  int ServoMinPWM = 500;
  int ServoMaxPWM = 2400;
  int ServoMin = 0;
  int ServoMax = 180;
  int angle[3] = {40, 0, -40};
  //I can make a 2D array of servos to manage it, currently not a priority, ALSO SET PINS AS NEEDED
  Servo servo[NUM_LEGS/2][NUM_JOINTS];
  Adafruit_PWMServoDriver pwm = Adafruit_PWMServoDriver();
  int JointPositions[NUM_LEGS] = 
  {0,0,0,0,0,0};

  /*
  HELLA INCLUDE MINS AND MAX'S
  /|\ = where they are on the curves, note for where you want to implement changing the bezier curve.
  Leg 1 = FrontLeft /
  Leg 2 = MidLeft |
  Leg 3 = BackLeft  \
  Leg 4 = FrontRight  \
  Leg 5 = MidRight  |
  Leg 6 = BackRight /
  Joint 1 = Coxa
  Joint 2 = Femur
  Joint 3 = Tibia

  */
  int ServoPins[NUM_LEGS][NUM_JOINTS] =
  { {0,1,2},  //pca
    {3,4,5},
    {6,7,8},
    {13,12,10}, //arduino
    {9,8,7},
   {6,5,4}
  };

  Motor ServoIDs [NUM_LEGS][NUM_JOINTS]=
  {{{PCA, 0}, {PCA, 1}, {PCA, 2}},
   {{PCA, 5}, {PCA, 4}, {PCA, 3}}, //switched on robot, so switched on code
   {{PCA, 6}, {Arduino, 4}, {PCA, 8}}, //PCA 7 BROKEN
   {{Arduino, 5},{Arduino, 6},{PCA, 15}},
   {{PCA, 12},{PCA, 13},{PCA, 14}}, //coxa in the middle
    {{PCA, 9},{PCA, 10},{PCA, 11}}};

  int Angles[NUM_LEGS] = {50,0,-50,-50,0,50}; //idek yet
  

  void SetupServos();
  void MoveLeg(float FulljierCurve[][3], int Speed, int legjoint, int angle, int AdditionalDelay, int index, int total, int EdgeIndex[3]);
  void CalibrateLeg(int ServoPin);
  void SleepMode();
  void CalibrationMode();
  void IdleMode();
  void Random(float ArrayLine[][3], int point1[3], int point2[3], int interpolation, int Axis);
  void setServos(int legjoint, int IKArray[]);
  void offsetLegs(int total, float delayPercentage);
void Ripple(float Bez[][3], int Speed, int AdditionalDelay, int total, bool Strafe, float DelayPercent);
void Bi();
void Wave(float Bez[][3], int Speed, int AdditionalDelay, int total, bool Strafe, float DelayPercent);
void TriPod(float Bez[][3], int Speed, int AdditionalDelay, int total, bool Strafe, float DelayPercent, int EdgeIndex[3]);
void Turn(float ArrayLine[][3], int interpolation);
  //functions 
};
class Controller{
  public:
  static int radiovals [13];

  static int MoveForwardVal;
  static bool MoveForward;
  static bool MoveBackward;

  static double strafeAngle;
  static bool Strafe;

  static int rotationAngle;
  static bool Rotation;

  static bool ChangeGaits;
  static bool gaitbuttonPressed;
  static bool statebuttonPressed;
  static bool gaitSetup;
  
  static double totalAngle;
  int EventType = Idle;
  void checkInputs();
  bool canMoveForward();

};
class Hexapod{
public:
  


enum Gaits{
RippleGait,
BiGait,
WaveGait,
TriPodGait
};


bool gaitChanged = false;

States current_state = Sleeping;
Gaits gaitArray[4] = { RippleGait, BiGait, WaveGait, TriPodGait};
Gaits current_gait = RippleGait;
void FSM(ServoManager StateControl, Controller ControlInst);
void update(ServoManager StateControl, Controller ControlInst );
void walkFSM(Gaits blah, int direction, bool Strafe, ServoManager StateControl, Controller ControlInst);
//GAITS
void changeGaits(bool gaitbuttonPressed);



};

//MAKING BEZIER A CLASS TO FIRST FIND THE ORIGINAL BEZIER CURVE, THEN FOR ANALYSIS BETWEEN POINTS WHEN DOING AVOIDANCE
class Bezier{
  public:
  static constexpr int IPN[4] = {3, 5, 3, 5};   //
  
  
  static const int total = IPN[0]+IPN[1]+IPN[2]+IPN[3];  
  float FullBezierCurve[total][3];
  int EdgeIndex[3] = {0,0,0};
  
  /*
  int BezierPoints[3][3][2] = 
  {{{-7,7},{-11,4},{-7,4}},
  {{7,7},{0,14},{-7,7}},
  {{7,7},{11,4},{7,4}}};
  */
  //WHEN CHANGING THIS, MAKE SURE TO SHRINK THE POINTS TO GIVE IT ROOM TO MOVE
  int yoff = -7*10; //Y Offset: UP and DOWN
  int Z = 40; //IN and OUT
  int Range = 25; // Outside Domain
  int Domain = 70; // Bezier Top Domain
  int Floor = 0;  // Bezier Bottom Domain
  int inX = Range - 10; // extra Bezier Point coord X
  int outY = Floor + 20;// extra Bezier Point coord Y
  int BezierPoints[8][2] = { {-inX,outY+yoff} ,{-Range,Floor+yoff}, {-inX,outY+yoff}, {inX, Floor+yoff}, {Range,Floor+yoff}, {inX,outY+yoff}, {0,Domain+yoff}, {-inX,outY+yoff}};

/*
BEZIER CURVES
{ {-5,3+yoff} ,{-8,0+yoff}, {-5,0+yoff}, {5,0+yoff}, {8,0+yoff}, {5,3+yoff}, {0,10+yoff}, {-5,4+yoff}};
{ {-7,7}, {-11,4}, {-7,4}, {7,4}, {11,4}, 7,7}, {0,14} + {-7,7} }
*/

  void Curve(float CurveArray[][3], int BezierPoints[8][2], int IPN[4], int Z);
  void Bez(float CurveArray[][3], int p[], int q[], int s[], int interpolation, int Z);
  void Line(float CurveArray[][3], int point1[], int point2[], int interpolation, int Z);
  void CheckCurveEdges();
  int SplitCurve(int speed, float PushPercentage);
};


#endif

