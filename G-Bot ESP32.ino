#include <WiFi.h>
#include <WebSocketServer.h>
#include <string.h>;

#define PIN_AIN1  27 // ESP32 pin GIOP27 connected to the AIN1 pin L298N
#define PIN_AIN2  14 // ESP32 pin GIOP14 connected to the AIN2 pin L298N
#define PIN_AENA  25 // ESP32 pin GIOP25 connected to the AEN1 pin L298N
#define PIN_BIN1  12 // ESP32 pin GIOP12 connected to the BIN1 pin L298N
#define PIN_BIN2  15 // ESP32 pin GIOP35 connected to the BIN2 pin L298N
#define PIN_BENA  26 // ESP32 pin GIOP26 connected to the BEN1 pin L298N
#define echoPin   22
#define trigPin   23
 

#define PWM1_Ch     0              // Setting 0th PWM channel for the first motor
#define PWM2_Ch     1              // Setting 1st PWM channel for the second motor
#define PWM1_Res    10             // Setting SAR ADC resolution for channel 1 to 10
#define PWM1_Freq   1000           // Setting the PWM channel 1 frequency to 1000
#define PWM2_Res    10             // Setting SAR ADC resolution for channel 2 to 10
#define PWM2_Freq   1000           // Setting the PWM channel 2 frequency to 1000

int PWM1_DutyCycle=250;           // Setting default PWM channel 1 duty cycle to 250
int PWM2_DutyCycle=250;           // Setting default PWM channel 2 duty cycle to 250

long duration;
int distance2; 
String message;
long duration1;
int distance1; 
String message1;
String mg = "Message@";

 
WiFiServer server(80);
WebSocketServer webSocketServer;
 
// Replace with your network credentials
const char* ssid = "GBOT";               // Creating SSID for access point
const char* password = "123456789";      // Creating password for the access point



// the code in setup function runs only one time when ESP32 starts
void setup() {
  // initialize digital pins as outputs.
 
  pinMode(PIN_AIN1, OUTPUT);
  pinMode(PIN_AIN2, OUTPUT);
  pinMode(PIN_AENA, OUTPUT);
  pinMode(PIN_BIN1, OUTPUT);
  pinMode(PIN_BIN2, OUTPUT);
  pinMode(PIN_BENA, OUTPUT);

  // Setting up analog pins with the respective PWM channels
  ledcAttachPin(PIN_AENA, PWM1_Ch);
  ledcSetup(PWM1_Ch, PWM1_Freq, PWM1_Res);
  ledcAttachPin(PIN_BENA, PWM2_Ch);
  ledcSetup(PWM2_Ch, PWM2_Freq, PWM2_Res);
  
  Serial.begin(115200);                       // Serial Begin at 115200 baud rate
  
  delay(4000);
 
  WiFi.softAP(ssid, password);              // Creating access point
  Serial.println(WiFi.softAPIP());          // Printing the IP address of ESP32 which will be used to connect to it
 
  server.begin();      // Server created
  delay(100);
}

// the code in loop function is executed repeatedly infinitely
void loop() {

  /////////////////////////////////////////////////
  // Activating ultrasonic sensor to send and receive pulses for distance value
  digitalWrite(trigPin,LOW);
  delayMicroseconds(2);
  digitalWrite(trigPin,HIGH);
  delayMicroseconds(10);
  digitalWrite(trigPin,LOW);

  duration=pulseIn(echoPin,HIGH); // time for the return of signal
  distance2=(duration*0.034/2);   // distance derived from time
  message = (String)distance2;
  /////////////////////////////////////////////////

  ledcWrite(PWM1_Ch, PWM1_DutyCycle);
  ledcWrite(PWM2_Ch, PWM2_DutyCycle);
  String data;  
  int d;

  WiFiClient client = server.available();  // creating a client when available
 
  if (client.connected() && webSocketServer.handshake(client)) {  // When client is connected and handshake is successful
 
    //////////////////////////////////////////////
    // Data Transfer
    while (client.connected()) {
 
      data = webSocketServer.getData();

      if(data.length()>0){
          d = data.toInt();
          Serial.println("Data:");
          Serial.println(data);

    /////////////////////////////////////////////


////////////////////////////////////////////////////////////////////////////////////////////////
// Can move anywhere no objects detected
////////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////
//Integer  Corresponding Robot Motion
/////       0     Stop
/////       1     Forward
/////       2   Forward Fast
/////       3   Backward
/////       4   Backward fast
/////       5   Left
/////       6   Big Left
/////       7   Right
/////       8   Big Right
/////       9   Forward motion while making a left turn
/////      10    Fast Forward motion while making a big left turn
/////      11    Backward motion while making a left turn
/////      12    Fast Backward motion while making a big left turn
/////      13    Backward motion while making a right turn
/////      14    Fast Backward motion while making a big right turn
/////      15    Forward motion while making a right turn
/////      16    Fast Forward motion while making a big right turn
/////      17    Forward motion while making a big left turn
/////      18    Backward motion while making a big left turn
/////      19    Fast Backward motion while making a left turn
/////      20    Fast Forward motion while making a left turn
/////      21    Forward motion while making a big right turn
/////      22    Backward motion while making a big right turn

          if(distance2 > 40){

              if(d == 0){
                  digitalWrite(PIN_AIN1, LOW); 
                  digitalWrite(PIN_AIN2, LOW);  
                  digitalWrite(PIN_BIN1, LOW);
                  digitalWrite(PIN_BIN2, LOW);  
              }

              if(d == 1){
                  PWM1_DutyCycle=300;
                  PWM2_DutyCycle=300;
                  ledcWrite(PWM1_Ch, PWM1_DutyCycle);
                  ledcWrite(PWM2_Ch, PWM2_DutyCycle);
                  digitalWrite(PIN_AIN1,LOW);
                  digitalWrite(PIN_AIN2,HIGH);
                  digitalWrite(PIN_BIN1,HIGH);
                  digitalWrite(PIN_BIN2,LOW);
            }

            if(d == 2){
                int PWM1_DutyCycle=350;
                int PWM2_DutyCycle=350;
                ledcWrite(PWM1_Ch, PWM1_DutyCycle);
                ledcWrite(PWM2_Ch, PWM2_DutyCycle);
                digitalWrite(PIN_AIN1,LOW);
                digitalWrite(PIN_AIN2,HIGH);
                digitalWrite(PIN_BIN1,HIGH);
                digitalWrite(PIN_BIN2,LOW);
            }
  
            if(d == 3){
                int PWM1_DutyCycle=300;
                int PWM2_DutyCycle=300;
                ledcWrite(PWM1_Ch, PWM1_DutyCycle);
                ledcWrite(PWM2_Ch, PWM2_DutyCycle);
                digitalWrite(PIN_AIN1,HIGH);
                digitalWrite(PIN_AIN2,LOW);
                digitalWrite(PIN_BIN1,LOW);
                digitalWrite(PIN_BIN2,HIGH);
            }
  
            if(d == 4){
                int PWM1_DutyCycle=350;
                int PWM2_DutyCycle=350;
                ledcWrite(PWM1_Ch, PWM1_DutyCycle);
                ledcWrite(PWM2_Ch, PWM2_DutyCycle);
                digitalWrite(PIN_AIN1,HIGH);
                digitalWrite(PIN_AIN2,LOW);
                digitalWrite(PIN_BIN1,LOW);
                digitalWrite(PIN_BIN2,HIGH);
            }
  
            if(d == 5){
                int PWM1_DutyCycle=300;
                int PWM2_DutyCycle=0;
                ledcWrite(PWM1_Ch, PWM1_DutyCycle);
                ledcWrite(PWM2_Ch, PWM2_DutyCycle);
                digitalWrite(PIN_AIN1,LOW);
                digitalWrite(PIN_AIN2,HIGH);
                digitalWrite(PIN_BIN1,HIGH);
                digitalWrite(PIN_BIN2,LOW);
            }
  
            if(d == 6){
                int PWM1_DutyCycle=350;
                int PWM2_DutyCycle=0;
                ledcWrite(PWM1_Ch, PWM1_DutyCycle);
                ledcWrite(PWM2_Ch, PWM2_DutyCycle);
                digitalWrite(PIN_AIN1,LOW);
                digitalWrite(PIN_AIN2,HIGH);
                digitalWrite(PIN_BIN1,HIGH);
                digitalWrite(PIN_BIN2,LOW);
            }

  
            if(d == 7){
                int PWM1_DutyCycle=0;
                int PWM2_DutyCycle=300;
                ledcWrite(PWM1_Ch, PWM1_DutyCycle);
                ledcWrite(PWM2_Ch, PWM2_DutyCycle);
                digitalWrite(PIN_AIN1,LOW);
                digitalWrite(PIN_AIN2,HIGH);
                digitalWrite(PIN_BIN1,HIGH);
                digitalWrite(PIN_BIN2,LOW);
            }

  
            if(d == 8){
                int PWM1_DutyCycle=0;
                int PWM2_DutyCycle=350;
                ledcWrite(PWM1_Ch, PWM1_DutyCycle);
                ledcWrite(PWM2_Ch, PWM2_DutyCycle);
                digitalWrite(PIN_AIN1,LOW);
                digitalWrite(PIN_AIN2,HIGH);
                digitalWrite(PIN_BIN1,HIGH);
                digitalWrite(PIN_BIN2,LOW);
            }
  
            if(d == 9){
                int PWM1_DutyCycle=300;
                int PWM2_DutyCycle=150;
                ledcWrite(PWM1_Ch, PWM1_DutyCycle);
                ledcWrite(PWM2_Ch, PWM2_DutyCycle);
                digitalWrite(PIN_AIN1,LOW);
                digitalWrite(PIN_AIN2,HIGH);
                digitalWrite(PIN_BIN1,HIGH);
                digitalWrite(PIN_BIN2,LOW);
            }
            
            if(d == 10){
                int PWM1_DutyCycle=350;
                int PWM2_DutyCycle=150;
                ledcWrite(PWM1_Ch, PWM1_DutyCycle);
                ledcWrite(PWM2_Ch, PWM2_DutyCycle);
                digitalWrite(PIN_AIN1,LOW);
                digitalWrite(PIN_AIN2,HIGH);
                digitalWrite(PIN_BIN1,HIGH);
                digitalWrite(PIN_BIN2,LOW);
            }
  
            if(d == 11){
                int PWM1_DutyCycle=300;
                int PWM2_DutyCycle=150;
                ledcWrite(PWM1_Ch, PWM1_DutyCycle);
                ledcWrite(PWM2_Ch, PWM2_DutyCycle);
                digitalWrite(PIN_AIN1,HIGH);
                digitalWrite(PIN_AIN2,LOW);
                digitalWrite(PIN_BIN1,LOW);
                digitalWrite(PIN_BIN2,HIGH);
            }
  
            if(d == 12){
                int PWM1_DutyCycle=350;
                int PWM2_DutyCycle=150;
                ledcWrite(PWM1_Ch, PWM1_DutyCycle);
                ledcWrite(PWM2_Ch, PWM2_DutyCycle);
                digitalWrite(PIN_AIN1,HIGH);
                digitalWrite(PIN_AIN2,LOW);
                digitalWrite(PIN_BIN1,LOW);
                digitalWrite(PIN_BIN2,HIGH);
            }
  
            if(d == 13){
                int PWM1_DutyCycle=150;
                int PWM2_DutyCycle=300;
                ledcWrite(PWM1_Ch, PWM1_DutyCycle);
                ledcWrite(PWM2_Ch, PWM2_DutyCycle);
                digitalWrite(PIN_AIN1,HIGH);
                digitalWrite(PIN_AIN2,LOW);
                digitalWrite(PIN_BIN1,LOW);
                digitalWrite(PIN_BIN2,HIGH);
            }
  
            if(d == 14){
                int PWM1_DutyCycle=150;
                int PWM2_DutyCycle=350;
                ledcWrite(PWM1_Ch, PWM1_DutyCycle);
                ledcWrite(PWM2_Ch, PWM2_DutyCycle);
                digitalWrite(PIN_AIN1,HIGH);
                digitalWrite(PIN_AIN2,LOW);
                digitalWrite(PIN_BIN1,LOW);
                digitalWrite(PIN_BIN2,HIGH);
            }
            
            if(d == 15){
                int PWM1_DutyCycle=150;
                int PWM2_DutyCycle=300;
                ledcWrite(PWM1_Ch, PWM1_DutyCycle);
                ledcWrite(PWM2_Ch, PWM2_DutyCycle);
                digitalWrite(PIN_AIN1,LOW);
                digitalWrite(PIN_AIN2,HIGH);
                digitalWrite(PIN_BIN1,HIGH);
                digitalWrite(PIN_BIN2,LOW);
            }
  
            if(d == 16){
                int PWM1_DutyCycle=150;
                int PWM2_DutyCycle=350;
                ledcWrite(PWM1_Ch, PWM1_DutyCycle);
                ledcWrite(PWM2_Ch, PWM2_DutyCycle);
                digitalWrite(PIN_AIN1,LOW);
                digitalWrite(PIN_AIN2,HIGH);
                digitalWrite(PIN_BIN1,HIGH);
                digitalWrite(PIN_BIN2,LOW);
            }
  
            if(d == 17){
                int PWM1_DutyCycle=300;
                int PWM2_DutyCycle=150;
                ledcWrite(PWM1_Ch, PWM1_DutyCycle);
                ledcWrite(PWM2_Ch, PWM2_DutyCycle);
                digitalWrite(PIN_AIN1,LOW);
                digitalWrite(PIN_AIN2,HIGH);
                digitalWrite(PIN_BIN1,HIGH);
                digitalWrite(PIN_BIN2,LOW);
            }
  
            if(d == 18){
                int PWM1_DutyCycle=300;
                int PWM2_DutyCycle=150;
                ledcWrite(PWM1_Ch, PWM1_DutyCycle);
                ledcWrite(PWM2_Ch, PWM2_DutyCycle);
                digitalWrite(PIN_AIN1,HIGH);
                digitalWrite(PIN_AIN2,LOW);
                digitalWrite(PIN_BIN1,LOW);
                digitalWrite(PIN_BIN2,HIGH);
            }
  
            if(d == 19){
                int PWM1_DutyCycle=350;
                int PWM2_DutyCycle=150;
                Serial.print("DC1 : ");
                Serial.print(PWM1_DutyCycle);
                Serial.print("DC2 : ");
                Serial.print(PWM2_DutyCycle);
                ledcWrite(PWM1_Ch, PWM1_DutyCycle);
                ledcWrite(PWM2_Ch, PWM2_DutyCycle);
                digitalWrite(PIN_AIN1,LOW);
                digitalWrite(PIN_AIN2,HIGH);
                digitalWrite(PIN_BIN1,HIGH);
                digitalWrite(PIN_BIN2,LOW);
            }
  
            if(d == 20){
                int PWM1_DutyCycle=300;
                int PWM2_DutyCycle=150;
                ledcWrite(PWM1_Ch, PWM1_DutyCycle);
                ledcWrite(PWM2_Ch, PWM2_DutyCycle);
                digitalWrite(PIN_AIN1,LOW);
                digitalWrite(PIN_AIN2,HIGH);
                digitalWrite(PIN_BIN1,HIGH);
                digitalWrite(PIN_BIN2,LOW);
            }
            
            if(d == 21){
                int PWM1_DutyCycle=150;
                int PWM2_DutyCycle=350;
                ledcWrite(PWM1_Ch, PWM1_DutyCycle);
                ledcWrite(PWM2_Ch, PWM2_DutyCycle);
                digitalWrite(PIN_AIN1,LOW);
                digitalWrite(PIN_AIN2,HIGH);
                digitalWrite(PIN_BIN1,HIGH);
                digitalWrite(PIN_BIN2,LOW);
            }
            
            if(d == 22){
                int PWM1_DutyCycle=150;
                int PWM2_DutyCycle=350;
                ledcWrite(PWM1_Ch, PWM1_DutyCycle);
                ledcWrite(PWM2_Ch, PWM2_DutyCycle);
                digitalWrite(PIN_AIN1,HIGH);
                digitalWrite(PIN_AIN2,LOW);
                digitalWrite(PIN_BIN1,LOW);
                digitalWrite(PIN_BIN2,HIGH);
            }
        }

////////////////////////////////////////////////////////////////////////////////////////////////
// Object detected ahead but can move in reverse
////////////////////////////////////////////////////////////////////////////////////////////////
        

       else if(distance2<40){

          digitalWrite(PIN_AIN1, LOW); // control the motor's direction in clockwise
          digitalWrite(PIN_AIN2, LOW);  // control the motor's direction in clockwise
          digitalWrite(PIN_BIN1, LOW); // control the motor's direction in clockwise
          digitalWrite(PIN_BIN2, LOW);  // control the motor's direction in clockwise
        
        
          if(d == 0){
                  digitalWrite(PIN_AIN1, LOW); // control the motor's direction in clockwise
                  digitalWrite(PIN_AIN2, LOW);  // control the motor's direction in clockwise
                  digitalWrite(PIN_BIN1, LOW); // control the motor's direction in clockwise
                  digitalWrite(PIN_BIN2, LOW);  // control the motor's direction in clockwise  
              }
              
              if(d == 3){
                int PWM1_DutyCycle=300;
                int PWM2_DutyCycle=300;
                ledcWrite(PWM1_Ch, PWM1_DutyCycle);
                ledcWrite(PWM2_Ch, PWM2_DutyCycle);
                digitalWrite(PIN_AIN1,HIGH);
                digitalWrite(PIN_AIN2,LOW);
                digitalWrite(PIN_BIN1,LOW);
                digitalWrite(PIN_BIN2,HIGH);
            }
  
            if(d == 4){
                int PWM1_DutyCycle=350;
                int PWM2_DutyCycle=350;
                ledcWrite(PWM1_Ch, PWM1_DutyCycle);
                ledcWrite(PWM2_Ch, PWM2_DutyCycle);
                digitalWrite(PIN_AIN1,HIGH);
                digitalWrite(PIN_AIN2,LOW);
                digitalWrite(PIN_BIN1,LOW);
                digitalWrite(PIN_BIN2,HIGH);
            }

            if(d == 11){
                int PWM1_DutyCycle=300;
                int PWM2_DutyCycle=150;
                Serial.print("DC1 : ");
                Serial.print(PWM1_DutyCycle);
                Serial.print("DC2 : ");
                Serial.print(PWM2_DutyCycle);
                ledcWrite(PWM1_Ch, PWM1_DutyCycle);
                ledcWrite(PWM2_Ch, PWM2_DutyCycle);
                digitalWrite(PIN_AIN1,HIGH);
                digitalWrite(PIN_AIN2,LOW);
                digitalWrite(PIN_BIN1,LOW);
                digitalWrite(PIN_BIN2,HIGH);
            }
  
            if(d == 12){
                int PWM1_DutyCycle=350;
                int PWM2_DutyCycle=150;
                ledcWrite(PWM1_Ch, PWM1_DutyCycle);
                ledcWrite(PWM2_Ch, PWM2_DutyCycle);
                digitalWrite(PIN_AIN1,HIGH);
                digitalWrite(PIN_AIN2,LOW);
                digitalWrite(PIN_BIN1,LOW);
                digitalWrite(PIN_BIN2,HIGH);
            }
  
            if(d == 13){
                int PWM1_DutyCycle=150;
                int PWM2_DutyCycle=300;
                ledcWrite(PWM1_Ch, PWM1_DutyCycle);
                ledcWrite(PWM2_Ch, PWM2_DutyCycle);
                digitalWrite(PIN_AIN1,HIGH);
                digitalWrite(PIN_AIN2,LOW);
                digitalWrite(PIN_BIN1,LOW);
                digitalWrite(PIN_BIN2,HIGH);
            }
  
            if(d == 14){
                int PWM1_DutyCycle=150;
                int PWM2_DutyCycle=350;
                ledcWrite(PWM1_Ch, PWM1_DutyCycle);
                ledcWrite(PWM2_Ch, PWM2_DutyCycle);
                digitalWrite(PIN_AIN1,HIGH);
                digitalWrite(PIN_AIN2,LOW);
                digitalWrite(PIN_BIN1,LOW);
                digitalWrite(PIN_BIN2,HIGH);
            }
            if(d == 18){
                int PWM1_DutyCycle=300;
                int PWM2_DutyCycle=150;
                ledcWrite(PWM1_Ch, PWM1_DutyCycle);
                ledcWrite(PWM2_Ch, PWM2_DutyCycle);
                digitalWrite(PIN_AIN1,HIGH);
                digitalWrite(PIN_AIN2,LOW);
                digitalWrite(PIN_BIN1,LOW);
                digitalWrite(PIN_BIN2,HIGH);
            }
  
            if(d == 19){
                int PWM1_DutyCycle=350;
                int PWM2_DutyCycle=150;
                ledcWrite(PWM1_Ch, PWM1_DutyCycle);
                ledcWrite(PWM2_Ch, PWM2_DutyCycle);
                digitalWrite(PIN_AIN1,LOW);
                digitalWrite(PIN_AIN2,HIGH);
                digitalWrite(PIN_BIN1,HIGH);
                digitalWrite(PIN_BIN2,LOW);
            }

            if(d == 22){
                int PWM1_DutyCycle=150;
                int PWM2_DutyCycle=350;
                ledcWrite(PWM1_Ch, PWM1_DutyCycle);
                ledcWrite(PWM2_Ch, PWM2_DutyCycle);
                digitalWrite(PIN_AIN1,HIGH);
                digitalWrite(PIN_AIN2,LOW);
                digitalWrite(PIN_BIN1,LOW);
                digitalWrite(PIN_BIN2,HIGH);
            }
    }
      
////////////////////////////////////////////////////////////////////////////////////////////////
//Object Detected on front and back
////////////////////////////////////////////////////////////////////////////////////////////////

      else{
            if(d == 0){
                  digitalWrite(PIN_AIN1, LOW); // control the motor's direction in clockwise
                  digitalWrite(PIN_AIN2, LOW);  // control the motor's direction in clockwise
                  digitalWrite(PIN_BIN1, LOW); // control the motor's direction in clockwise
                  digitalWrite(PIN_BIN2, LOW);  // control the motor's direction in clockwise 
              }
        }
    }

////////////////////////////////////////////////////////////////////////////////////////////////
//Message Sending
////////////////////////////////////////////////////////////////////////////////////////////////
        
        if (message.length() > 0) {
           //Serial.println(message);
           webSocketServer.sendData(mg + message);
        }
        delay(10); // Delay needed for receiving the data correctly  
 }
 
   Serial.println("The client disconnected");
}
}
