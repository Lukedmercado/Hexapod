#include "Hexapod.h"
#include "Constants.h"

void Bezier::Bez(float CurveArray[][3], int p[], int q[], int s[], int interpolation, int Z){
  float t = 0.0;
  float tstep = 1.0/interpolation;
  for(int i = 0; i < interpolation; i++){
  float a0 = q[0] + (1-t)*(p[0]-q[0]);
  float a1 = q[1] + (1-t)*(p[1]-q[1]);
  float a2 = q[0] + t*(s[0] - q[0]);
  float a3 = q[1] + t*(s[1] - q[1]);
  float BL0 = a0 + t*(a2-a0);
  float BL1 = a1 + t*(a3-a1);
  CurveArray[i][0] = BL0;
  CurveArray[i][1] = BL1;
  CurveArray[i][2] = Z;

  t += tstep;
  }
}
void Bezier::Line(float CurveArray[][3], int point1[], int point2[], int interpolation, int Z){
  float t = 0.0;
  float tstep = 1.0/interpolation;

  for(int i = 0; i < interpolation; i++){
    float a0 = t*point2[0] + (1-t)*point1[0]; 
    float a1 = t*point2[1] + (1-t)*point1[1];
    
    t+= tstep;
  CurveArray[i][0] = a0;
  CurveArray[i][1] = a1;
  CurveArray[i][2] = Z;
  }

}
//int point1[], int point2[], used for line, but bezier points hold it
void Bezier::Curve(float CurveArray[][3], int BezierPoints[8][2], int IPN[4], int Z){
  float(*Point)[3] = CurveArray;

  //YOU NEED TO CHANGE ALL THE BEZIER POINTS

  Bez(CurveArray, BezierPoints[0], BezierPoints[1], BezierPoints[2], IPN[0], Z);
  Point+=IPN[0];
  Line(Point, BezierPoints[2], BezierPoints[3], IPN[1], Z);
  Point+=IPN[1];
  Bez(Point, BezierPoints[3], BezierPoints[4], BezierPoints[5], IPN[2], Z); // right bezier
  Point+=IPN[2];
  Bez(Point, BezierPoints[5], BezierPoints[6], BezierPoints[7], IPN[3], Z ); //top bezier
  Point+=IPN[3];
  

}

/*
STEPS
MAKE BEZIER CURVE
CHECK CURVE EDGES
SPLIT THE CURVE
CHOOSE GAIT
MOVE THE LEG
*/
void Bezier::CheckCurveEdges(){
    // go through curve and find most negative, x, most postive x, and most positive 
    int BezArray[3] = {0,0,0};
    for(int i = 0; i < total; i++){
      if(this->FullBezierCurve[i][0] < BezArray[0]){ //checks for most negative x
        BezArray[0] = this->FullBezierCurve[i][0];
        this->EdgeIndex[0] = i;
      }
      if(this->FullBezierCurve[i][0] > BezArray[1]){ //checks for most postitive x
        BezArray[1] = this->FullBezierCurve[i][0];
        this->EdgeIndex[1] = i;
      }
      if(this->FullBezierCurve[i][1] > BezArray[2]){ //checks for most positive y
        BezArray[2] = this->FullBezierCurve[i][1];
        this->EdgeIndex[2] = i;
      }
    }
  }

  int Bezier::SplitCurve(int speed, float pushPercent){ 
    //push percentage tells u how long u want it on the ground, speed tells u how fast it is on the ground and will add delay to the second half
    int OneHalf = abs(this->EdgeIndex[0] - this->EdgeIndex[1]);//this should be bottom half
    int TwoHalf = this->total - TwoHalf;  //this should be top half
    //Top half should be whichever half has the most positive y value(should be 2nd half)
    
    //TWO WAYS OF DOING IT -> USE THE SPEED GIVEN AND FIND THE TOP HALF, THEN TURN THAT INTO A PERCENTAGE TO FIND THE PERCENTAGE BOTTOM HALF AND IT'S NECCESESARY SPEED
    //USE SPEED AT THE PERCENTAGE ON THE GROUND, AND MAKE THE TOP HALF WORK TO THIS. (This ideas better
    int FirstTime = OneHalf*abs(speed);
    int AdditionalDelay = (FirstTime*(1-pushPercent))/(pushPercent*TwoHalf); //speed
    return AdditionalDelay;
  }