#include <Wire.h> //I2C Arduino Library

#define magAddress 0x1E //0011110b, I2C 7bit address of HMC5883

void setup() {
   //Size of input data unknown - change var sizes to save data
   unsigned int count = 0;
   //Store in x-y-z to avoid square root calculations until end
   unsigned long mFieldx = 0;
   unsigned long mFieldy = 0;
   unsigned long mFieldz = 0;
   unsigned int eFieldx = 0;
   unsigned int eFieldy = 0;
   unsigned int eFieldz = 0;
   //Initialize Serial and I2C communications
  Serial.begin(9600);
  Wire.begin();
  
  //Put the HMC5883 IC into the correct operating mode
 Wire.beginTransmission(magAddress); //open communication with HMC5883
 Wire.write(0x02); //select mode register
 Wire.write(0x00); //continuous measurement mode
 Wire.endTransmission();
}

void loop() {
   int x,y,z; //triple axis data

  //Tell the HMC5883L where to begin reading data
  Wire.beginTransmission(magAddress);
  Wire.write(0x03); //select register 3, X MSB register
  Wire.endTransmission();
  
 
 //Read data from each axis, 2 registers per axis
  Wire.requestFrom(magAddress, 6);
  if(6<=Wire.available()){
    x = Wire.read()<<8; //X msb
    x |= Wire.read(); //X lsb
    z = Wire.read()<<8; //Z msb
    z |= Wire.read(); //Z lsb
    y = Wire.read()<<8; //Y msb
    y |= Wire.read(); //Y lsb
  }
   
   //Uncomment if necessary
  /*
  //Print out values of each axis
  Serial.print("x: ");
  Serial.print(x);
  Serial.print("  y: ");
  Serial.print(y);
  Serial.print("  z: ");
  Serial.println(z);
  */
   
  //10 seconds approximately, based on a delay of 250ms per run
  if(count == 40){
     Serial.println(" ");
     Serial.println("5s Delay, turn on current now");
     Serial.println("Earth Ave Mag Field:");
     //get ave of earths mag field
     eFieldx/=count;
     eFieldy/=count;
     eFieldz/=count;
     Serial.print("x: ");
     Serial.print(eFieldx);
     Serial.print("  y: ");
     Serial.print(eFieldy);
     Serial.print("  z: ");
     Serial.println(eFieldz);
     //delay to turn on external magnetic field
     delay(5000);
     Serial.println("Calculating Ext Mag Field");
   }
   if(count >= 120){
      Serial.print("x: ");
      Serial.print(mFieldx/(count-40));
      Serial.print("  y: ");
      Serial.print(mFieldy/(count-40));
      Serial.print("  z: ");
      Serial.println(mFieldz/(count-40));
   }
   //Add adjusted mag field accounting for earths
   else if(count >= 40){
      mFieldx+=x-eFieldx;
      mFieldy+=y-eFieldy;
      mFieldz+=z-eFieldz;
   }
   else{
      eFieldx+=x;
      eFieldy+=y;
      eFieldz+=z;
   }
   count += 1;
   delay(250);
}
