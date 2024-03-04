/*include libraries*/
#include <LiquidCrystal.h>

/*defining some values*/
#define RS 11 // LCD
#define EN 10 // LCD
#define D4 5 // LCD
#define D5 4 // LCD
#define D6 3 // LCD
#define D7 2 // LCD
#define TRIG 13 // ultrasonic trigger
#define ECHO 12 // ultrasonic echo
#define MAX 334.5 // set maximum distance of ultrasonic range
#define MIN 2 // set minimum distance of ultrasonic range

/*declaration*/
float get_distance(int trig_pin,int echo_pin);

/*create lcd object*/
LiquidCrystal lcd(RS, EN, D4, D5, D6, D7);

/*setup code, to run once*/
void setup() {
  /*set pin modes*/
  pinMode (TRIG, OUTPUT);
  pinMode (ECHO, INPUT);

  /*start printing monitor*/
  lcd.begin (16,2);
  lcd.print (" Speed Detector ");
  // Serial.begin(9600);
}

/*main code, to run repeatedly*/
void loop() {
  long t[2];
  float d[2], velocity;
  int i;

  delay (50); // delay to finish process

  /*setting the cursor on the LCD*/
  lcd.setCursor (0,1);

  /*computing distance and time at 2 different points*/
  for (i = 0; i < 2; i++) {
    d[i] = get_distance(TRIG, ECHO);
    t[i] = millis() * 0.001; // t[i] in second
    if(d[i] < MIN || d[i] > MAX)
      lcd.print ("No object       "); // distance out of the range
  }

  velocity = (d[1] - d[0]) / (t[1] - t[0]);

  /*print velocity on the LCD*/
  lcd.print ("v = ");
  lcd.print (velocity);
  lcd.print (" m/s  ");
  // Serial.println (velocity);
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

  delay (50);
  digitalWrite (trig_pin, HIGH); // transmit waves
  delayMicroseconds (10);
  digitalWrite (trig_pin, LOW); // stop transmitting

  time = pulseIn (echo_pin, HIGH); // time of transmitting and recieving in microseconds
  distance = 0.0001715 * time ; // dist = 343 * (time / 2) * 10^-6

  return distance;
}