#include <Servo.h>
#include "DHT.h"
#define dhtPin 7      //讀取DHT11 Data
#define dhtType DHT11 //選用DHT11   
const int buttonPin = 3; 

//------- PM2.5
int measurePin = 0; //Connect dust sensor to Arduino A0 pin
int ledPower = 2;   //Connect 3 led driver pins of dust sensor to Arduino D2
  
int samplingTime = 280;
int deltaTime = 40;
int sleepTime = 9680;
  
float voMeasured = 0;
float calcVoltage = 0;
float dustDensity = 0;
//-----------------


DHT dht(dhtPin, dhtType); // Initialize DHT sensor
Servo myservo; // 建立Servo物件，控制伺服馬達
void setup() {
  Serial.begin(9600);
  myservo.attach(9);
  dht.begin();//啟動DHT
  pinMode(buttonPin, INPUT);
  pinMode(ledPower,OUTPUT);//----PM2.5
}

void loop() {
   int buttonState = digitalRead(buttonPin);
  if (buttonState == HIGH) {
    Serial.println("Mode : Auto");
    //------PM2.5
     digitalWrite(ledPower,LOW); // PM2.5
      delayMicroseconds(samplingTime);// PM2.5
      voMeasured = analogRead(measurePin); // read the dust value
  
      delayMicroseconds(deltaTime);
     digitalWrite(ledPower,HIGH); // turn the LED off
      delayMicroseconds(sleepTime);
      calcVoltage = voMeasured * (5.0 / 1024.0);
      dustDensity = 0.17 * calcVoltage - 0.1;
      Serial.print("Raw Signal Value (0-1023): ");
      Serial.print(voMeasured);
      Serial.print(" - Voltage: ");
      Serial.print(calcVoltage);
      Serial.print(" - Dust Density: ");
      Serial.print(dustDensity * 1000); // 這裡將數值呈現改成較常用的單位( ug/m3 )
       Serial.println(" ug/m3 ");
        //------PM2.5

    
    int watersensor = analogRead(1 ); // 讀取可變電阻(數值介於 0 到 1023)
    Serial.print("watersensor:"); 
    Serial.println(watersensor);   //Wrote serial port
    float t = dht.readHumidity();//讀取攝氏溫度
    Serial.print("溼度: ");
    Serial.print(t);

      if(watersensor>150||t>82.00||dustDensity * 1000>100)
      {
         myservo.write(5); // 使用write，傳入角度，從0度轉到180度 
      }
      else{
          myservo.write(90);
      }
      delay(1000);
  } else {
    Serial.println("Mode : HandMove");
    int val = analogRead(5 ); // 讀取可變電阻(數值介於 0 到 1023)
    Serial.println(val);
    val = map(val, 0, 1023, 5, 179); // 把 0 – 1023 的數值按比例縮放為 0 – 180 的數值
    myservo.write(val); // 設定 Servo 旋轉角度
    delay(15); // 等待 Servo 旋轉指定的位置
  }
  
  
  /*int watersensor=analogRead(A1); // Incoming analog signal read and appointed sensor
  
*/
  
}
