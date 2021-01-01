/*Created by Ricardo Torres
 *CSU San Bernardino 
 *Final project for CSE 5410
 *Robot Car(Tiny Tank) is Designed to work with Arduino Uno R3.
 *ESP01_ESP8266 is used for wireless communication
 *between Arduino board and Phone Application.
 */

 /*Library used:
  * LiquidCrystal_74HC595 is used for LCD screen using a 
  * shift register 74HC595.
  * DHT is used for Humiture sensor. Sensor can give 
  * temperature as well as Humidity.
 */
#include <LiquidCrystal_74HC595.h>
#include <DHT.h>

//assign Humiture
#define DHTPIN 2 
#define DHTTYPE DHT11
DHT dht(DHTPIN, DHTTYPE);

//assigning spin of the motor
#define CW 0
#define CCW 1
//assigning motor
#define MOTOR_A 0
#define MOTOR_B 1

//bit shift register
int latchPin = 5;
int clockPin = 6;
int dataPin = 4;
#define DS 4
#define SHCP 6
#define STCP 5
#define RS 1
#define E 2
#define D4 3
#define D5 4
#define D6 5
#define D7 6

//Thermistor 
#define analogPin  A0 //the thermistor attach to 
#define beta 1090 //the beta of the thermistor
#define resistance 10 //the value of the pull-up resistor

//using attached LCD screen w/ bit shift
LiquidCrystal_74HC595 lcd(DS, SHCP, STCP, RS, E, D4, D5, D6, D7); //using 595 upshift

//board assignments for motor
const byte PWMA = 3;
const byte PWMB = 11;
const byte DIRA = 12;
const byte DIRB = 13;

//speed of motors
const byte spd = 205;

//chars used for incoming commands from Phone Application
char temp[50];
char compare;

//assigning fan a pin
int fan = 8;

void setup() 
{
  //set baudrate
  Serial.begin(115200);
  //shield setup for arduino motors
  setupArdumoto();
  //setup for LCD screen 16 characters long and 2 Lines
  lcd.begin(16,2); 
  lcd.clear();
  lcd.setCursor(0,0);
  lcd.print("Dir.: ");
  lcd.setCursor(0,1);
  lcd.print("Temp: ");
  pinMode(fan,OUTPUT);
  //initialize humiture
  dht.begin();
}

void loop() 
{
  String incoming;
  
 //reading phone commands to string incoming
  unsigned int length;
  do{
    if (Serial.available() > 0) 
  {
    reread:
    // read the incoming byte:
    incoming = Serial.readString();
    incoming.toCharArray(temp,50);
    length = incoming.length();
  }

//read driving direction to Serial monitor  
Serial.println("This is direction: ");
Serial.println(incoming);
compare = temp[length-1];
Serial.println(compare);
delay(1500);

//case statement to indicate what Tiny Tank 
//should do with incoming commands.
switch (compare)
{
  case 'f':
  {
  forward();
  break;
  }
 case 'b':
  {
  reverse();
  break;
  }
 case 'r':
  {
  turnRight();
  break;
  }
 case 'l':
  {
  turnLeft();
  break;
  }
 case 's':
  {
  stopArdumoto(MOTOR_A);
  stopArdumoto(MOTOR_B);
  lcd.setCursor(7,0);
  lcd.print("stop");
  break;
  }
 default:
  {
  delay(1500);
  goto reread;
  }
}
}while(compare != 's');


//tempReading();
//humitureReading();

/*
* This Set of code is to check the fucntions of driving.
*This is to verify what direction is left and Right and 
*driving functions operate as intended. Uncommment this 
*section to verify vehicle is operating normally. 
////////////////////////////////////////////////////////
forward();
delay(1500);
turnRight();
delay(1500);
turnLeft();
delay(1500);
reverse();
delay(1500);
stopArdumoto(MOTOR_A);
stopArdumoto(MOTOR_B);
delay(10000);
///////////////////////////////////////////////////////
//
*/
//digitalWrite(fan,HIGH);
//delay(2000);
//digitalWrite(fan,LOW);
}

//function for Humiture Sensor
void humitureReading()
{
  // Reading temperature or humidity takes about 250 milliseconds!
  // Sensor readings may also be up to 2 seconds 'old' (its a very slow sensor)
  float h = dht.readHumidity();
  // Read temperature as Celsius (the default)
  float t = dht.readTemperature();
  // Read temperature as Fahrenheit (isFahrenheit = true)
  float f = dht.readTemperature(true);

  // Check if any reads failed and exit early (to try again).
  if (isnan(h) || isnan(t) || isnan(f)) {
    Serial.println(F("Failed to read from DHT sensor!"));
    return;

  // Compute heat index in Fahrenheit (the default)
  float hif = dht.computeHeatIndex(f, h);
  // Compute heat index in Celsius (isFahreheit = false)
  float hic = dht.computeHeatIndex(t, h, false);

  Serial.print(F("Humidity: "));
  Serial.print(h);
  Serial.print(F("%  Temperature: "));
  Serial.print(t);
  Serial.print(F("°C "));
  Serial.print(f);
  Serial.print(F("°F  Heat index: "));
  Serial.print(hic);
  Serial.print(F("°C "));
  Serial.print(hif);
  Serial.println(F("°F"));
//set the cursor @ a particular position to print temperature
  lcd.setCursor(10,2);
  lcd.print(h);
}
}

//function to read temperature
void tempReading()
{
   long a =1023 - analogRead(analogPin); //read thermistor value
  Serial.print("Raw reading ");
  Serial.println(a);
  //the calculating formula of temperature
  float tempC = beta /(log((1025.0 * 10 / a - 10) / 10) + beta / 298.0) - 273.0;
  float tempF = 1.8*tempC + 32.0;
  Serial.print("Centigrade ");
  Serial.println(tempC);
  Serial.print("Fahrenheit ");
  Serial.println(tempF);
  Serial.println("");
  lcd.setCursor(6,2);
  lcd.print((int)tempF);
  lcd.print("F");
}

//intial setup of motor status
//called in initial setup only
void setupArdumoto()
{
  //set power and directions to output signals
  pinMode(PWMA, OUTPUT);
  pinMode(PWMB, OUTPUT);
  pinMode(DIRA, OUTPUT);
  pinMode(DIRB, OUTPUT);
// set all power and directions to off
  digitalWrite(PWMA, LOW);
  digitalWrite(PWMB, LOW);
  digitalWrite(DIRA, LOW);
  digitalWrite(DIRB, LOW);
}

//move robot forward
void forward()
{
  driveArdumoto(MOTOR_A, CW, spd);
  driveArdumoto(MOTOR_B, CW, spd);
    lcd.setCursor(6,0);
    Serial.println("forward"); 
  lcd.print("forward");
}

//move robot reverse
void reverse()
{
  driveArdumoto(MOTOR_A, CCW, spd);
  driveArdumoto(MOTOR_B, CCW, spd);
    lcd.setCursor(6,0);
    Serial.println("reverse");
  lcd.print("back     ");
}

//turn robot right
void turnRight()
{
  stopArdumoto(MOTOR_B);
  driveArdumoto(MOTOR_A, CW, spd);
    lcd.setCursor(6,0);
    Serial.println("Right");
  lcd.print("Right  ");
}

//turn robot left
void turnLeft()
{
  driveArdumoto(MOTOR_B, CW, spd);
  stopArdumoto(MOTOR_A);
    lcd.setCursor(6,0);
    Serial.println("Left");
  lcd.print("Left  ");
}

//assign robot direction and power
void driveArdumoto( byte motor, byte dir, byte spd)
{
  if (motor == MOTOR_A)
  {
    digitalWrite(DIRA, dir);
    analogWrite(PWMA, spd);
  }
  else if (motor == MOTOR_B)
  {
    digitalWrite(DIRB, dir);
    analogWrite(PWMB, spd);
  }
}

//stop motor
//stop needs to be called twice for full stop
//one time for each motor. 
void stopArdumoto(byte motor)
{
  driveArdumoto(motor,0,0);
}
