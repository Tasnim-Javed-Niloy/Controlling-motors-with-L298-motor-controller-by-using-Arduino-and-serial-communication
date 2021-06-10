/*
 * M. Tasnim Javed Niloy
 * 10_June_2021 8:41pm
 * Testing motor controller with arduino and I2C communication.
 * Serial data is transmitted from laptop to arduino through data cable of arduino.
 * "$000255" message is for forward direction and "$000000" is for backward direction.
 * 
 * This is a test run. Four motors can be controlled with this code. Actually it's two motors.
 * 2 motors for left side and another 2 for right side.
 * Tattu Fun Fly 4S 100C 14.8V 22.94wh 1550 mAh lithium polymer battery is used.
 * Heat sink of the L298 motor controlled was connected to the ground.
 * Without being grounded the motor was not running at all (unknown cause).
 */

// Motor A connections
int enA = 9;
int in1 = 8;
int in2 = 7;
// Motor B connections
int enB = 3;
int in3 = 4;
int in4 = 2;

#define numOfValsRec 2       // Defining how many values will be received.
#define digitsPerValRec 3    // Defining how many digits each value will carry.

int valsRec[numOfValsRec];
int stringLength = numOfValsRec * digitsPerValRec + 1;  // Length of the full message that will be received.
int counter = 0;
bool counterStart = false;
String receivedString;

void receiveData()
{
  while (Serial.available())
  {
    char c = Serial.read();
    if (c=='$')
    {
      counterStart = true;   // If the message contains "$" sign on the 1st position, the message will be received.
    }
    if (counterStart)
    {
      if (counter < stringLength)
      {
        receivedString = String(receivedString+c);
        counter++;
      }
      if (counter >= stringLength)
      {
        for (int i=0; i<numOfValsRec; i++)
        {
          int num = (i*digitsPerValRec)+1;
          valsRec[i] = receivedString.substring(num,num+digitsPerValRec).toInt();
        }
        receivedString = "";
        counter = 0;
        counterStart = false;
      }
    }
  }
}

void setup() {
  // Set all the motor control pins to outputs
  pinMode(enA, OUTPUT);
  pinMode(in1, OUTPUT);
  pinMode(in2, OUTPUT);
  pinMode(enB, OUTPUT);
  pinMode(in3, OUTPUT);
  pinMode(in4, OUTPUT);
  
  // Turn off motors - Initial state
  digitalWrite(in1, LOW);
  digitalWrite(in2, LOW);
  digitalWrite(in3, LOW);
  digitalWrite(in4, LOW);

  Serial.begin(9600);   // Must include this line or motor can't be controlled through serial data.

}

void loop() {
  receiveData();
  if (valsRec[1] == 255){
    // Setting this speed value.
    analogWrite(enA, 255);
    analogWrite(enB, 255);
    
    // Setting direction to forward.
    digitalWrite(in1, HIGH);
    digitalWrite(in2, LOW);
    digitalWrite(in3, HIGH);
    digitalWrite(in4, LOW);
  }
  else {
    // Setting this speed value.
    analogWrite(enA, 255);
    analogWrite(enB, 255);
    
    // Setting direction to backward.
    digitalWrite(in1, LOW);
    digitalWrite(in2, HIGH);
    digitalWrite(in3, LOW);
    digitalWrite(in4, HIGH);
        
  }
////  CSA = (analogRead(A0)/1024)*5000; // reading in milivolt
//  CSA = analogRead(A0);
//  CSA = CSA/1.5; // I = V/R
//  Serial.print("Motor A Current= ");
//  Serial.print(CSA);
//  Serial.println(" ");
}
