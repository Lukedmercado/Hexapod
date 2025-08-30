#ifndef HEXAPOD_H
#define HEXAPOD_H
#include <Arduino.h>
#include <Servo.h>
#include "Constants.h"
class Hexapod{
public:
enum States{
Idle,
Sleeping,
Walking,
Stationary,
Calibration,
};

enum Gaits{
RippleGait,
BiGait,
WaveGait,
TriPodGait
};

int TibiaLength = 119;
int FemurLength = 90;
bool gaitChanged = false;
States current_state = Sleeping;
Gaits gaitArray[4] = { RippleGait, BiGait, WaveGait, TriPodGait};
Gaits current_gait = RippleGait;
void FSM(int event);
void update(bool gaitbuttonPressed);
void walkFSM(Gaits blah, int direction, bool Strafe);
//GAITS


void Ripple(float Bez[][3], int Speed, int AdditionalDelay, int total, bool Strafe, float DelayPercent);
void Bi();
void Wave(float Bez[][3], int Speed, int AdditionalDelay, int total, bool Strafe, float DelayPercent);
void TriPod(float Bez[][3], int Speed, int AdditionalDelay, int total, bool Strafe, float DelayPercent);
void changeGaits(bool gaitbuttonPressed);

};


class ServoManager{
  public:
  int ServoMinPWM = 500;
  int ServoMaxPWM = 2400;
  int ServoMin = 0;
  int ServoMax = 180;
  int angle[3] = {40, 0, -40};
  //I can make a 2D array of servos to manage it, currently not a priority, ALSO SET PINS AS NEEDED
  Servo servo[NUM_LEGS][NUM_JOINTS];
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
  { {10,11,5},
    {10,11,5},
    {10,11,5},
    {10,11,5},
    {10,11,5},
   {10,11,5}
  };

  int Angles[NUM_LEGS] = {0,0,0,0,0,0}; //idek yet
  

  void SetupServos();
  void MoveLeg(float FullBezierCurve[][3], Servo Leg[3], int Speed, int legjoint, int angle, int AdditionalDelay, int index);
  void CalibrateLeg(int ServoPin);
  void SleepMode();
  void CalibrationMode();
  void IdleMode();

  //functions 
};
//MAKING BEZIER A CLASS TO FIRST FIND THE ORIGINAL BEZIER CURVE, THEN FOR ANALYSIS BETWEEN POINTS WHEN DOING AVOIDANCE
class Bezier{
  public:
  static constexpr int IPN[4] = {10, 20, 10, 20};
  
  
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
  int yoff = 2; //WEIRD, OPPOSITE UP AND DOWN
  int Z = 100; // inwards and outwards
  int BezierPoints[8][2] = { {-3,2+yoff} ,{-4,0+yoff}, {-3,0+yoff}, {3,0+yoff}, {4,0+yoff}, {3,2+yoff}, {0,7+yoff}, {-3,2+yoff}};

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
  
  static double totalAngle;

  void checkInputs();
  bool canMoveForward();

};
#endif

