/*include libraries*/
#include <LiquidCrystal.h>

/*defining some values*/
#define RS 11 // LCD Register
#define EN 10 // LCD Enable
#define D4 5 // LCD data line 1
#define D5 4 // LCD data line 2
#define D6 3 // LCD data line 3
#define D7 2 // LCD data line 4
#define TRIG 13 // ultrasonic trigger
#define ECHO 12 // ultrasonic echo
#define MAX 3.33 // set maximum distance of ultrasonic range
#define MIN 0.03 // set minimum distance of ultrasonic range

/*functions declaration*/
float get_distance(int trig_pin, int echo_pin);

/*global variables*/
float d[2]; // an array of size 2 to store distances
float t[2]; // an array of size 2 to store times
int i; // an iterating variable
bool range; // a flag to the distance range

/*create a variable to control the LCD*/
LiquidCrystal lcd(RS, EN, D4, D5, D6, D7);

/*setup code, to run once*/
void setup() {
  /*set pin modes*/
  pinMode(TRIG, OUTPUT);
  pinMode(ECHO, INPUT);

  /*initializing lcd*/
  lcd.begin(16,2);
  lcd.print("VelocityDetector");
  
  /*initializing serial monitor*/
  //Serial.begin(9600);
}

/*main code, to run repeatedly*/
void loop() {
  delay(100); // delay to finish processing

  /*reset the flag*/
  range = true;

  /*setting the cursor on the LCD*/
  lcd.setCursor(0,1);

  /*computing distance and time at 2 different points*/
  for (i = 0; i < 2; i++) {
    d[i] = get_distance(TRIG, ECHO);
    t[i] = millis() * 0.001; // t[i] in second
    if(d[i] < MIN || d[i] > MAX)
      range = false;
  }

  if (!range)
    /*object out of range*/
    lcd.print("No Object       ");
  else {
    /*calculate and print the velocity on the LCD*/
    float velocity = (d[1] - d[0]) / (t[1] - t[0]);
    lcd.print("v = ");
    lcd.print(velocity);
    lcd.print(" m/s  ");

    /*printing on serial monitor*/
    //Serial.print("v = ");
    //Serial.println(velocity);
  }
}

/**
 * get_distance - A function that gets distance of an object using ultrasonic
 *
 * @trig_pin: The pin connected to the ultrasonic trigger
 * @echo_pin: The pin connected to the ultrasonic echo
 *
 * Return: The distance in meter
 */
float get_distance(int trig_pin, int echo_pin) {
  float distance, time;
  int temp;

  delay(50);
  digitalWrite(trig_pin, HIGH); // transmit waves
  delayMicroseconds(10);
  digitalWrite(trig_pin, LOW); // stop transmitting

  time = pulseIn(echo_pin, HIGH); // time of transmitting and recieving in microseconds
  distance = 0.0001715 * time ; // dist = 343 * (time / 2) * 10^-6

  /*cast distance as float with only 2 decimals after the point*/
  temp = distance * 100; // storing in integer the 2 decimals
  distance = temp / 100.0;

  return distance;
}