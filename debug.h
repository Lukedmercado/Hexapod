#ifndef DEBUG_H
#define DEBUG_H

void CheckBezier(){
  Bezier Bez;
  for(int i = 0; i < Bez.total; i++){
      
      Serial.print(Bez.FullBezierCurve[i][0]);
      Serial.print(" ");
      Serial.print(Bez.FullBezierCurve[i][1]);
      Serial.print(" ");
      Serial.print(Bez.FullBezierCurve[i][2]);
      Serial.println(" ");
      
      delay(500);

  }
}
void checkAngles(int angle, double rotation, double total, double Strafe){
   Serial.print("STARTING ANGLE: ");
    Serial.print(angle);
    Serial.print(" CONTROLLED ANGLE: ");
    Serial.print(total);
    Serial.print(" ROTATION ANGLE: ");
     Serial.print(rotation);
    Serial.print(" STRAFE ANGLE: ");
    Serial.println(Strafe);
}
void CheckControllerVals(int radiovals[]){
  Serial.print("X VAL: ");
  Serial.print(radiovals[0]);
  Serial.print(" Y VAL: ");
    Serial.println(radiovals[1]);

  
}

  
#endif