/******************************include_libraries******************************/
#include <avr/io.h>
#include <avr/interrupt.h>

/********************************define_macros********************************/
#define LDR_PIN A5
#define RESISTANCE_VALUE 560
#define INPUT_VOLTAGE 500UL
#define LED1_PIN 6
#define LED2_PIN 7
#define BUZZER_PIN 8
#define Echo_Pin 10
#define Trig_Pin 11
#define Box_Distance 20
#define SpeedOfSound 343
#define Conversion_Factor 0.0001

/********************************global_variables*****************************/
int Counter;
int flag;
int is_picked_flag;

/***********************************setup*************************************/
void setup() {
	//Enabling OVF Interrupt
	TIMSK1 |= (1 << TOIE1);
  //Setting Initial Value For Timer Register
  TCNT1 = 46272;
  //Enabling Global Interrupt
  sei();

  pinMode(LDR_PIN, INPUT);
  digitalWrite(LDR_PIN, LOW);
  pinMode(BUZZER_PIN, OUTPUT);
  digitalWrite(BUZZER_PIN, LOW);
  pinMode(LED1_PIN, OUTPUT);
  digitalWrite(LED1_PIN, LOW);
  pinMode(LED2_PIN, OUTPUT);
  digitalWrite(LED2_PIN, LOW);
  pinMode(Echo_Pin, INPUT);
  pinMode(Trig_Pin, OUTPUT);
  digitalWrite(Trig_Pin, LOW);

  //Setting PreScaler To 8
  TCCR1B |= (1 << CS11);
}

/***********************************loop**************************************/
void loop() {
  /**
   * Applying Pulse Of 12 microsec To Trig Pin To Send Wave and
   * Setting ECHO Pin High
   *
   * Measuring Time Required To Return The Wave back To ECHO Pin
   *
   * Calculating Distance In cm Between UltraSonic And Object
   */
  delay(50);
  digitalWrite(Trig_Pin, HIGH);
  delayMicroseconds(10);
  digitalWrite(Trig_Pin, LOW);
  float time_Returned = pulseIn(Echo_Pin, HIGH);
  float distance = Get_Distance(time_Returned / 2);    

	/**
	 * Reading Digital Value From ADC Covnersion
	 *
	 * Converting Value To Analogue In Order To Obtain LDR Value
	 *	using Voltage Divider Equation
	 */
	int digital_value = analogRead(LDR_PIN);
	int input_analog_value = (digital_value * INPUT_VOLTAGE) / 1024;
	unsigned int LDR_VALUE = (INPUT_VOLTAGE * RESISTANCE_VALUE -
                  input_analog_value * RESISTANCE_VALUE) / input_analog_value;
 
	/**
	 * If Calculated Resistance Value greater than 1500 (i.e No Light Or Dark)
	 * And an Object Was Placed Turn ON UV-Light For 5 Seconds
	 * After Which a Buzzer Sound Is Generated
	 *
	 * else if Container is Opened Turn OFF UV-Light
	 */
	if (LDR_VALUE > 1500 && distance < Box_Distance) {
		if (is_picked_flag) {
	  	digitalWrite(LED1_PIN, HIGH);
	    digitalWrite(LED2_PIN, HIGH);
	    if (flag == 1){
     		Buzzer_On();
	      digitalWrite(LED1_PIN, LOW);
	      digitalWrite(LED2_PIN, LOW);
	    }
	  }
	} else {
		is_picked_flag = 1;
		Counter = 0;
	  flag = 0;
	  TCNT1 = 46272;
	  digitalWrite(LED1_PIN, LOW);
	  digitalWrite(LED2_PIN, LOW);
	}
}

/*******************************used_functions********************************/
/**
 * Get_Distance - Calculates Distance between ultrasonic and object
 * @time: Time Obtained From UltraSonic Sensor
 *
 * Return: Distance in centimeters
 */
float Get_Distance(float time) {
	return (SpeedOfSound * time * Conversion_Factor);
}

/**
 * Buzzer_On - Turn On Buzzer
 */
void Buzzer_On(void) {
  for (int i = 0; i < 2; ++i) {
  	digitalWrite(BUZZER_PIN, HIGH);
  	delay(500);
  	digitalWrite(BUZZER_PIN, LOW);
  	delay(500);
  }
  flag = 0;
	is_picked_flag = 0;
}

/**
 * ISR - Interrupt Service Routine
 * Function Which is Automatically Called Upon OVF Of Timer1 Register
 *
 * @TIMER1_OVF_vect: Timer1 Interrupt Vector
 */
ISR(TIMER1_OVF_vect) {
  Counter++;
  if (Counter == 2500) {
    // Sterilization Succeeded With No Interruption
    // i.e Box Wasnt Opened through The past 5 sec
    flag = 1;
    Counter = 0;
    TCNT1 = 46272;
  }
}

/*****************************************************************************/