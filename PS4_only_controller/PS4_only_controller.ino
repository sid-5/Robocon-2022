#include <PS4Controller.h>



void setup() {
 
  Serial.begin(115200);
 //54:8d:5a:0b:66:1f
 //30:94:35:32:df:3e
  PS4.begin("54:8d:5a:0b:66:1f");
  Serial.println("Ready.");
  
   
}
int data[3] = {};
void loop() {
  
  if(PS4.isConnected()) {
    if(PS4.Triangle() && PS4.Circle()){
       data[0] = 20;
      data[1]= 0;
      data[2] = 0;
      Serial.write(data[0]);
      Serial.write(data[1]);
      Serial.write(data[2]);
      
    }
    else if(PS4.Triangle() && PS4.Square()){
       data[0] = 21;
      data[1]= 0;
      data[2] = 0;
      Serial.write(data[0]);
      Serial.write(data[1]);
      Serial.write(data[2]);
      
    }else if(PS4.Cross() && PS4.Square()){
       data[0] = 22;
      data[1]= 0;
      data[2] = 0;
      Serial.write(data[0]);
      Serial.write(data[1]);
      Serial.write(data[2]);
  
    }else if(PS4.Cross() && PS4.Circle()){
       data[0] = 23;
      data[1]= 0;
      data[2] = 0;
      Serial.write(data[0]);
      Serial.write(data[1]);
      Serial.write(data[2]);
      
    }
    else if (PS4.Up() && !PS4.Circle()){
      data[0] = 1;
      data[1]= 0;
      data[2] = 0;
      Serial.write(data[0]);
      Serial.write(data[1]);
      Serial.write(data[2]);
      
    }

    else if (PS4.Down()){
      data[0] = 2;
      data[1]= 0;
      data[2] = 0;
      Serial.write(data[0]);
      Serial.write(data[1]);
      Serial.write(data[2]);

    }

    else if (PS4.Left()){
      data[0] = 3;
      data[1]= 0;
      data[2] = 0;
      Serial.write(data[0]);
      Serial.write(data[1]);
      Serial.write(data[2]);

    }

    else if (PS4.Right()){
      data[0] = 4;
      data[1]= 0;
      data[2] = 0;
      Serial.write(data[0]);
      Serial.write(data[1]);
      Serial.write(data[2]);

    }

    else if (PS4.Square()){
      data[0] = 5;
      data[1]= 0;
      data[2] = 0;
      Serial.write(data[0]);
      Serial.write(data[1]);
      Serial.write(data[2]);
     
    }

    else if (PS4.Circle()){ //servo
      data[0] = 6;
      data[1]= 0;
      data[2] = 0;
      Serial.write(data[0]);
      Serial.write(data[1]);
      Serial.write(data[2]);
  
    }
    else if (PS4.L1()){
    data[0] = 7;
      data[1]= 0;
      data[2] = 0;
      Serial.write(data[0]);
      Serial.write(data[1]);
      Serial.write(data[2]);

  }
  else if (PS4.L2()){
    data[0] = 8;
      data[1]= 0;
      data[2] = 0;
      Serial.write(data[0]);
      Serial.write(data[1]);
      Serial.write(data[2]);
 
    
  }
  else if (PS4.R1()){
    data[0] = 9;
      data[1]= 0;
      data[2] = 0;
      Serial.write(data[0]);
      Serial.write(data[1]);
      Serial.write(data[2]);
    
  }else if (PS4.R2()){
    data[0] = 10;
      data[1]= 0;
      data[2] = 0;
      Serial.write(data[0]);
      Serial.write(data[1]);
      Serial.write(data[2]);
  }else if(PS4.Triangle()){
    data[0] = 11;
      data[1]= 0;
      data[2] = 0;
      Serial.write(data[0]);
      Serial.write(data[1]);
      Serial.write(data[2]);
   
  }else if(PS4.Cross()){
    data[0] = 12;
      data[1]= 0;
      data[2] = 0;
      Serial.write(data[0]);
      Serial.write(data[1]);
      Serial.write(data[2]);
      
  }else if(PS4.Touchpad()){
 data[0] = 13;
      data[1]= 0;
      data[2] = 0;
      Serial.write(data[0]);
      Serial.write(data[1]);
      Serial.write(data[2]);
    
  }
      data[0] = 0;
      data[1]= 0;
      data[2] = 0;
      Serial.write(data[0]);
      Serial.write(data[1]);
      Serial.write(data[2]);
      
    
  }
 
}
