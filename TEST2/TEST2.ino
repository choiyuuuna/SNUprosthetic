#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>

#define OLED_ADDR   0x3C
Adafruit_SSD1306 display(-1);

unsigned long initial_time;
unsigned long final_time;

char a = 0;
long steady_state  = 0;

// motor actuating pins
int motor_Pin1_thumb = 5;   
int motor_Pin2_thumb = 6;  
int motor_Pin1_index = 7;  
int motor_Pin2_index = 8;   
int motor_Pin1_middle = 10;  
int motor_Pin2_middle = 16;  
int motor_Pin1_ring_little = 14;  
int motor_Pin2_ring_little = 15; 

// motor stroke pins
int stroke_thumb = A9;
int stroke_index = A0;
int stroke_middle = A1;
int stroke_ring = A3;
int stroke_little = A2;  

// variables and constants
int strokeValue_index = 0;
int strokeValue_middle = 0;
int strokeValue_ring = 0;
int strokeValue_little = 0;
int strokeValue_thumb = 0;

void setup() 
{
  Serial.begin(115200);
  Serial1.begin(115200);
  
  /* --------------- Display --------------- */
  display.begin(SSD1306_SWITCHCAPVCC, OLED_ADDR);
  display.clearDisplay();

  display.setTextSize(1.9);
  display.setTextColor(WHITE);
  display.setCursor(20,10);
  display.print("SNU");
  display.setCursor(20,20);
  display.print("OnePiece Hand");
  display.display();
  /* --------------------------------------- */
  
  pinMode(motor_Pin1_thumb, OUTPUT);
  pinMode(motor_Pin2_thumb, OUTPUT);
  pinMode(motor_Pin1_index, OUTPUT);
  pinMode(motor_Pin2_index, OUTPUT);
  pinMode(motor_Pin1_middle, OUTPUT);
  pinMode(motor_Pin2_middle, OUTPUT);
  pinMode(motor_Pin1_ring_little, OUTPUT);
  pinMode(motor_Pin2_ring_little, OUTPUT);

  int strokeValue_thumb = analogRead(stroke_thumb);
  int strokeValue_index = analogRead(stroke_index);
  int strokeValue_middle = analogRead(stroke_middle);
  int strokeValue_ring = analogRead(stroke_ring);
  int strokeValue_little = analogRead(stroke_little);

  full_extens();
  getInitialSteadyState();
}

void loop() 
{
  if(Serial1.available() > 0)
  {
    a = Serial1.read();
    Serial.println(a);
    Make_pattern();
  }
}

void Make_pattern()
{
  if (a == '1') 
  {
    /*
    display.clearDisplay();
    display.setTextSize(1.2);
    display.setTextColor(WHITE);
    display.setCursor(0,0.8);
    display.print("P >> Full Extension");
    display.display();
    */
    full_extens();
  }
  
  if (a == '2') 
  {
    /*
    display.clearDisplay();
    display.setTextSize(1.2);
    display.setTextColor(WHITE);
    display.setCursor(0,0.8);
    display.print("P >> Full Flexion");
    display.display();
    */
    grasp_wrap();
  }
  
  if (a == '3') 
  {
    /*
    display.clearDisplay();
    display.setTextSize(1.2);
    display.setTextColor(WHITE);
    display.setCursor(0,0.8);
    display.print("P >> Grasp Tripod");
    display.display();
    */
    full_extens();
  }
}

void thumb_extens() 
{
  while (1) 
  {
    strokeValue_thumb = analogRead(stroke_thumb);
    digitalWrite(motor_Pin1_thumb, 0);
    digitalWrite(motor_Pin2_thumb, 1);

    if (strokeValue_thumb >= 1020) 
    {
      digitalWrite(motor_Pin1_thumb, 0);
      digitalWrite(motor_Pin2_thumb, 0);
      break;
    }
  }
}

void thumb_flex(int K) 
{
  initial_time = millis();
  while (1) 
  {
    final_time = millis();
    strokeValue_thumb = analogRead(stroke_thumb);
    digitalWrite(motor_Pin1_thumb, 1);
    digitalWrite(motor_Pin2_thumb, 0);

    if (strokeValue_thumb <= K) 
    {
      digitalWrite(motor_Pin1_thumb, 0);
      digitalWrite(motor_Pin2_thumb, 0);
      break;
    }
    if (final_time - initial_time > 2500) 
    {
      break;
    }
  }
}

void index_extens() 
{
  while (1) 
  {
    strokeValue_index = analogRead(stroke_index);
    digitalWrite(motor_Pin1_index, 1);
    digitalWrite(motor_Pin2_index, 0);
    
    if (strokeValue_index >= 1000) 
    {
      digitalWrite(motor_Pin1_index, 0);
      digitalWrite(motor_Pin2_index, 0);
      break;
    }
  }
}

void index_flex(int K) 
{
  initial_time = millis();
  while (1) 
  {
    final_time = millis();
    strokeValue_index = analogRead(stroke_index);
    digitalWrite(motor_Pin1_index, 0);
    digitalWrite(motor_Pin2_index, 1);
  
    if (strokeValue_index <= K) 
    {
      digitalWrite(motor_Pin1_index, 0);
      digitalWrite(motor_Pin2_index, 0);
      break;
    }
    if (final_time - initial_time > 2500) 
    {
      break;
    }
  }
}

void middle_extens() 
{
  while (1) 
  {
    strokeValue_middle = analogRead(stroke_middle);
    digitalWrite(motor_Pin1_middle, 0);
    digitalWrite(motor_Pin2_middle, 1);
   
    if (strokeValue_middle >= 1020) 
    {
      digitalWrite(motor_Pin1_middle, 0);
      digitalWrite(motor_Pin2_middle, 0);
      break;
    }
  }
}

void middle_flex(int K) 
{
  initial_time = millis();
  while (1) 
  {
    final_time = millis();
    strokeValue_middle = analogRead(stroke_middle);
    digitalWrite(motor_Pin1_middle, 1);
    digitalWrite(motor_Pin2_middle, 0);

    if (strokeValue_middle <= K) 
    {
      digitalWrite(motor_Pin1_middle, 0);
      digitalWrite(motor_Pin2_middle, 0);
      break;
    }
    if (final_time - initial_time > 2500) 
    {
      break;
    }
  }
}

void ring_little_extens() 
{
  while (1) 
  {
    strokeValue_ring = analogRead(stroke_ring);
    strokeValue_little = analogRead(stroke_little);
    digitalWrite(motor_Pin1_ring_little, 1);
    digitalWrite(motor_Pin2_ring_little, 0);

    if (strokeValue_ring >= 1020) 
    {
      digitalWrite(motor_Pin1_ring_little, 0);
      digitalWrite(motor_Pin2_ring_little, 0);
      break;
    }
  }
}

void ring_little_flex(int K) 
{
  initial_time = millis();
  while (1) 
  {
    final_time = millis();
    strokeValue_ring = analogRead(stroke_ring);
    strokeValue_little = analogRead(stroke_little);
    digitalWrite(motor_Pin1_ring_little, 0);
    digitalWrite(motor_Pin2_ring_little, 1);
 
    if (strokeValue_little <= K) 
    {
      digitalWrite(motor_Pin1_ring_little, 0);
      digitalWrite(motor_Pin2_ring_little, 0);
      break;
    }
    if (final_time - initial_time > 2500) 
    {
      break;
    }
  }
}

void full_extens() 
{
  while (1) 
  {
    strokeValue_thumb = analogRead(stroke_thumb);
    strokeValue_index = analogRead(stroke_index);
    strokeValue_middle = analogRead(stroke_middle);
    strokeValue_ring = analogRead(stroke_ring);
    strokeValue_little = analogRead(stroke_little);
    digitalWrite(motor_Pin1_middle, 0);
    digitalWrite(motor_Pin2_middle, 1);
    digitalWrite(motor_Pin1_index, 1);
    digitalWrite(motor_Pin2_index, 0);
    digitalWrite(motor_Pin1_ring_little, 1);
    digitalWrite(motor_Pin2_ring_little, 0);
    digitalWrite(motor_Pin1_thumb, 0);
    digitalWrite(motor_Pin2_thumb, 1);

    if (strokeValue_middle >= 1020 && strokeValue_index >= 1010 && strokeValue_ring >= 1020 && strokeValue_little >= 1020 && strokeValue_thumb >= 1020) 
    {
      digitalWrite(motor_Pin1_middle, 0);
      digitalWrite(motor_Pin2_middle, 0);
      digitalWrite(motor_Pin1_index, 0);
      digitalWrite(motor_Pin2_index, 0);
      digitalWrite(motor_Pin1_ring_little, 0);
      digitalWrite(motor_Pin2_ring_little, 0);
      digitalWrite(motor_Pin1_thumb, 0);
      digitalWrite(motor_Pin2_thumb, 0);
      break;
    }
  }
}

void full_flex(int K) 
{
  while (1) 
  {
    strokeValue_index = analogRead(stroke_index);
    strokeValue_middle = analogRead(stroke_middle);
    strokeValue_ring = analogRead(stroke_ring);
    strokeValue_little = analogRead(stroke_little);
    digitalWrite(motor_Pin1_index, 0);
    digitalWrite(motor_Pin2_index, 1);
    digitalWrite(motor_Pin1_middle, 1);
    digitalWrite(motor_Pin2_middle, 0);
    digitalWrite(motor_Pin1_ring_little, 0);
    digitalWrite(motor_Pin2_ring_little, 1);
 
    if (strokeValue_index <= K) 
    {
      digitalWrite(motor_Pin1_index, 0);
      digitalWrite(motor_Pin2_index, 0);
      digitalWrite(motor_Pin1_middle, 0);
      digitalWrite(motor_Pin2_middle, 0);
      digitalWrite(motor_Pin1_ring_little, 0);
      digitalWrite(motor_Pin2_ring_little, 0);
      break;
    }
  }
}

void grasp_pinch() 
{
  initial_time = millis();
  while (1) 
  {
    final_time = millis();
    strokeValue_thumb = analogRead(stroke_thumb);
    strokeValue_index = analogRead(stroke_index);
    digitalWrite(motor_Pin1_thumb, 1);
    digitalWrite(motor_Pin2_thumb, 0);
    digitalWrite(motor_Pin1_index, 0);
    digitalWrite(motor_Pin2_index, 1);
   
    if (strokeValue_thumb <= 750) 
    {
      digitalWrite(motor_Pin1_thumb, 0);
      digitalWrite(motor_Pin2_thumb, 0);
      digitalWrite(motor_Pin1_index, 0);
      digitalWrite(motor_Pin2_index, 0);
      break;
    }
    if (final_time - initial_time > 2500) 
    {
      break;
    }
  }
  index_flex(250);
}

void grasp_tripod() 
{
  initial_time = millis();
  while (1) 
  {
    final_time = millis();
    strokeValue_thumb = analogRead(stroke_thumb);
    strokeValue_index = analogRead(stroke_index);
    strokeValue_middle = analogRead(stroke_middle);
    digitalWrite(motor_Pin1_thumb, 1);
    digitalWrite(motor_Pin2_thumb, 0);
    digitalWrite(motor_Pin1_index, 0);
    digitalWrite(motor_Pin2_index, 1);
    digitalWrite(motor_Pin1_middle, 1);
    digitalWrite(motor_Pin2_middle, 0);
  
    if (strokeValue_thumb <= 750) 
    {
      digitalWrite(motor_Pin1_thumb, 0);
      digitalWrite(motor_Pin2_thumb, 0);
      digitalWrite(motor_Pin1_middle, 0);
      digitalWrite(motor_Pin2_middle, 0);
      digitalWrite(motor_Pin1_index, 0);
      digitalWrite(motor_Pin2_index, 0);

      break;
    }
    if (final_time - initial_time > 2500) 
    {
      break;
    }
  }
  
  while (1) 
  {
    final_time = millis();
    strokeValue_index = analogRead(stroke_index);
    strokeValue_middle = analogRead(stroke_middle);
    digitalWrite(motor_Pin1_middle, 1);
    digitalWrite(motor_Pin2_middle, 0);
    digitalWrite(motor_Pin1_index, 0);
    digitalWrite(motor_Pin2_index, 1);

    if (strokeValue_index <= 350) 
    {
      digitalWrite(motor_Pin1_middle, 0);
      digitalWrite(motor_Pin2_middle, 0);
      digitalWrite(motor_Pin1_index, 0);
      digitalWrite(motor_Pin2_index, 0);
      break;
    }
    if (final_time - initial_time > 2500) 
    {
      break;
    }
  }
  middle_flex(200);
}

void grasp_wrap() 
{
  initial_time = millis();
  while (1) 
  {
    final_time = millis();
    strokeValue_index = analogRead(stroke_index);
    strokeValue_middle = analogRead(stroke_middle);
    strokeValue_ring = analogRead(stroke_ring);
    strokeValue_little = analogRead(stroke_little);
    digitalWrite(motor_Pin1_index, 0);
    digitalWrite(motor_Pin2_index, 1);
    digitalWrite(motor_Pin1_middle, 1);
    digitalWrite(motor_Pin2_middle, 0);
    digitalWrite(motor_Pin1_ring_little, 0);
    digitalWrite(motor_Pin2_ring_little, 1);

    if (strokeValue_index <= 300) 
    {
      digitalWrite(motor_Pin1_index, 0);
      digitalWrite(motor_Pin2_index, 0);
      digitalWrite(motor_Pin1_middle, 0);
      digitalWrite(motor_Pin2_middle, 0);
      digitalWrite(motor_Pin1_ring_little, 0);
      digitalWrite(motor_Pin2_ring_little, 0);
      break;
    }
    if (final_time - initial_time > 2500) 
    {
      break;
    }
  }
  
  while (1) 
  {
    final_time = millis();
    strokeValue_thumb = analogRead(stroke_thumb);
    strokeValue_index = analogRead(stroke_index);
    strokeValue_middle = analogRead(stroke_middle);
    strokeValue_ring = analogRead(stroke_ring);
    strokeValue_little = analogRead(stroke_little);
    digitalWrite(motor_Pin1_thumb, 1);
    digitalWrite(motor_Pin2_thumb, 0);
    if (strokeValue_thumb <= 750) 
    {
      digitalWrite(motor_Pin1_thumb, 0);
      digitalWrite(motor_Pin2_thumb, 0);
      break;
    }
    if (final_time - initial_time > 2500) 
    {
      break;
    }
  }
}

void IM_Flex() 
{
  initial_time = millis();
  while (1) 
  {
    final_time = millis();
    strokeValue_index = analogRead(stroke_index);
    strokeValue_middle = analogRead(stroke_middle);
    digitalWrite(motor_Pin1_index, 0);
    digitalWrite(motor_Pin2_index, 1);
    digitalWrite(motor_Pin1_middle, 1);
    digitalWrite(motor_Pin2_middle, 0);

    if (strokeValue_index <= 300) 
    {
      digitalWrite(motor_Pin1_index, 0);
      digitalWrite(motor_Pin2_index, 0);
      digitalWrite(motor_Pin1_middle, 0);
      digitalWrite(motor_Pin2_middle, 0);
      break;
    }
    if (final_time - initial_time > 2500) 
    {
      break;
    }
  }
}

void Digits_Flex() 
{
  initial_time = millis();
  while (1) 
  {
    final_time = millis();
    strokeValue_index = analogRead(stroke_index);
    strokeValue_middle = analogRead(stroke_middle);
    strokeValue_ring = analogRead(stroke_ring);
    strokeValue_little = analogRead(stroke_little);
    digitalWrite(motor_Pin1_index, 0);
    digitalWrite(motor_Pin2_index, 1);
    digitalWrite(motor_Pin1_middle, 1);
    digitalWrite(motor_Pin2_middle, 0);
    digitalWrite(motor_Pin1_ring_little, 0);
    digitalWrite(motor_Pin2_ring_little, 1);

    if (strokeValue_index <= 300) 
    {
      digitalWrite(motor_Pin1_index, 0);
      digitalWrite(motor_Pin2_index, 0);
      digitalWrite(motor_Pin1_middle, 0);
      digitalWrite(motor_Pin2_middle, 0);
      digitalWrite(motor_Pin1_ring_little, 0);
      digitalWrite(motor_Pin2_ring_little, 0);
      break;
    }
    if (final_time - initial_time > 2500) 
    {
      break;
    }
  }

}

void U_DF() 
{
  initial_time = millis();
  while (1) 
  {
    final_time = millis();
    strokeValue_index = analogRead(stroke_index);
    strokeValue_middle = analogRead(stroke_middle);
    digitalWrite(motor_Pin1_middle, 0);
    digitalWrite(motor_Pin2_middle, 1);
    digitalWrite(motor_Pin1_index, 1);
    digitalWrite(motor_Pin2_index, 0);

    if (strokeValue_middle >= 1020) 
    {
      digitalWrite(motor_Pin1_middle, 0);
      digitalWrite(motor_Pin2_middle, 0);
      digitalWrite(motor_Pin1_index, 0);
      digitalWrite(motor_Pin2_index, 0);
      break;
    }
    if (final_time - initial_time > 2500) 
    {
      break;
    }
  }
}

void grasp_IMThumb() 
{
  initial_time = millis();
  while (1) 
  {
    final_time = millis();
    strokeValue_thumb = analogRead(stroke_thumb);
    strokeValue_index = analogRead(stroke_index);
    strokeValue_middle = analogRead(stroke_middle);
    digitalWrite(motor_Pin1_thumb, 1);
    digitalWrite(motor_Pin2_thumb, 0);
    digitalWrite(motor_Pin1_middle, 1);
    digitalWrite(motor_Pin2_middle, 0);
    digitalWrite(motor_Pin1_index, 0);
    digitalWrite(motor_Pin2_index, 1);

    if (strokeValue_thumb <= 750) 
    {
      digitalWrite(motor_Pin1_thumb, 0);
      digitalWrite(motor_Pin2_thumb, 0);
      digitalWrite(motor_Pin1_middle, 0);
      digitalWrite(motor_Pin2_middle, 0);
      digitalWrite(motor_Pin1_index, 0);
      digitalWrite(motor_Pin2_index, 0);

      break;
    }
    if (final_time - initial_time > 2500) 
    {
      break;
    }
  }
  while (1) 
  {
    final_time = millis();
    strokeValue_index = analogRead(stroke_index);
    strokeValue_middle = analogRead(stroke_middle);
    digitalWrite(motor_Pin1_middle, 1);
    digitalWrite(motor_Pin2_middle, 0);
    digitalWrite(motor_Pin1_index, 0);
    digitalWrite(motor_Pin2_index, 1);

    if (strokeValue_index <= 300) 
    {
      digitalWrite(motor_Pin1_middle, 0);
      digitalWrite(motor_Pin2_middle, 0);
      digitalWrite(motor_Pin1_index, 0);
      digitalWrite(motor_Pin2_index, 0);
      break;
    }
    if (final_time - initial_time > 2500) 
    {
      break;
    }
  }
}

int getInitialSteadyState() 
{
  steady_state = 0;
  for (int i = 0; i < 5; ++i) 
  {
    for (int j = 0; j < 20; ++j) 
    {
      steady_state += analogRead(A6);
      delay(1);
    }
  }
}
