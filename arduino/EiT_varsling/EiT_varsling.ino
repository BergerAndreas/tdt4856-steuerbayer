#define VIBRATOR  3
#define SPEAKER   5
#define LED_POWER 6
#define LED1 7
#define LED2 8
#define LED3 9


int BassTab[]={1911,1702,1516,1431,1275,1136,1012};//bass 1~7


void setup()
{
  // Initialize vibrator
  pinMode(VIBRATOR, OUTPUT);

  // Initialize speaker
  pinMode(SPEAKER, OUTPUT);
  digitalWrite(SPEAKER,LOW);

  // Initialize power LED
  pinMode(LED_POWER, OUTPUT);
  digitalWrite(LED_POWER, HIGH);

  // Initialize LED1-3
  pinMode(LED1, OUTPUT);
  //digitalWrite(LED1, HIGH);
  
  pinMode(LED2, OUTPUT);
  digitalWrite(LED2, HIGH);
  
  pinMode(LED3, OUTPUT);
  //digitalWrite(LED3, HIGH);
}

void loop()
{

  /*sound bass 1~7*/
  //play_sound(7);

  //vibrate(200,100);

  //blink_leds();
  //digitalWrite(VIBRATOR, HIGH);
  //blink_simultaneously(100,50);
  //digitalWrite(VIBRATOR, LOW);
  digitalWrite(LED2, HIGH);
  delay(500);
  digitalWrite(LED2, LOW);
  delay(200);
}

void play_sound(int sound)
{
    for(int i=0;i<100;i++)
  {
      digitalWrite(SPEAKER,HIGH);
      delayMicroseconds(BassTab[sound-1]);
      digitalWrite(SPEAKER,LOW);
      delayMicroseconds(BassTab[sound-1]);
      //delay(500);

  }
}

void vibrate(int time_on, int time_off)
{
  digitalWrite(VIBRATOR, HIGH);
  delay(time_on);
  digitalWrite(VIBRATOR, LOW);
  delay(time_off);
  digitalWrite(VIBRATOR, HIGH);
  delay(time_on);
  digitalWrite(VIBRATOR, LOW);
}

void blink_leds()
{
  digitalWrite(LED1, HIGH);
  delay(100);
  digitalWrite(LED1, LOW);
  
  digitalWrite(LED2, HIGH);
  delay(100);
  digitalWrite(LED2, LOW);

  digitalWrite(LED3, HIGH);
  delay(100);
  digitalWrite(LED3, LOW);
}

void blink_simultaneously(int time_on, int time_off)
{
  for(int i=0;i<5;i++)
  {
    digitalWrite(LED1, HIGH);
    delay(time_on);
    digitalWrite(LED1, LOW);
    delay(time_off);
  }
  
}
