#define VIBRATOR  A5
#define SPEAKER   3


#define LED_GREEN 8
#define LED1 9
#define LED2 10
#define LED3 11
#define LED4 12



int BassTab[] = {1911, 1702, 1516, 1431, 1275, 1136, 1012}; //bass 1~7


void setup()
{

  // Initialize Serial port
  Serial.begin(9600);
  
  // Initialize vibrator
  initialize_vibrator();

  // Initialize speaker
  initialize_speaker();

  // Initialize LEDS
  initialize_leds();
}

void loop()
{
  /*
  MAIN PROGRAM
  When the serial port receives an 1, the alert system will start.
  The sequenece will result in 6 blinks and 6 sound signals. The vibrator will vibrate in sync with the sound.
  */
  
  
  // Send data only when you receive data
  if (Serial.available() > 0) {
    trigger_signal = Serial.read();
  }

  if (trigger_signal == 1) {
    for (int i = 1; i <= 6; i++) {
    tone(SPEAKER, 1000, 200);
    digitalWrite(VIBRATOR, HIGH);
    if (i <= 3) {
      blink_leds_sequence();
    }
    else {
      blink_leds(400);
    }
    digitalWrite(VIBRATOR, LOW);
    delay(200);
    }  
  }
  
}

void initialize_leds()
{
  /*
  Initialize all LEDS by defining the pinmode on the respective digital output
  */
  pinMode(LED1, OUTPUT);
  digitalWrite(LED1, LOW);

  pinMode(LED2, OUTPUT);
  digitalWrite(LED2, LOW);

  pinMode(LED3, OUTPUT);
  digitalWrite(LED3, LOW);

  pinMode(LED4, OUTPUT);
  digitalWrite(LED4, LOW);

  pinMode(LED_GREEN, OUTPUT);
  digitalWrite(LED_GREEN, HIGH);
}

void initialize_vibrator()
{
  /*
  Initialize pinmode for the output pin for the vibrator.
  */
  pinMode(VIBRATOR, OUTPUT);
  digitalWrite(VIBRATOR, LOW);
}

void initialize_speaker()
{
  /*
  Initialize pinmode for the output pin for the speaker.
  */
  pinMode(SPEAKER, OUTPUT);
  digitalWrite(SPEAKER, LOW);
}

void blink_leds_sequence()
{
  /*
  Blink all red LEDs in a sequence where one LED blink after the other
  */
  digitalWrite(LED1, HIGH);
  delay(80);
  digitalWrite(LED1, LOW);
  digitalWrite(LED2, HIGH);
  delay(80);
  digitalWrite(LED2, LOW);
  digitalWrite(LED3, HIGH);
  delay(80);
  digitalWrite(LED3, LOW);
  digitalWrite(LED4, HIGH);
  delay(80);
  digitalWrite(LED4, LOW);
}

void blink_leds(int on_time)
{
  /* 
  Blink all simultaneously once with an ON-time as the input

  Args:
    int on_time: An integer value which indicates how long the LEDS are turned on
  */
  digitalWrite(LED1, HIGH);
  digitalWrite(LED2, HIGH);
  digitalWrite(LED3, HIGH);
  digitalWrite(LED4, HIGH);
  delay(on_time);
  digitalWrite(LED1, LOW);
  digitalWrite(LED2, LOW);
  digitalWrite(LED3, LOW);
  digitalWrite(LED4, LOW);
}
