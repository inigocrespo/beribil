#include <IRremote.h>

//    The direction of the car's movement
//  ENA   ENB   IN1   IN2   IN3   IN4   Description
//  HIGH  HIGH  HIGH  LOW   LOW   HIGH  Car is runing forward
//  HIGH  HIGH  LOW   HIGH  HIGH  LOW   Car is runing back
//  HIGH  HIGH  LOW   HIGH  LOW   HIGH  Car is turning left
//  HIGH  HIGH  HIGH  LOW   HIGH  LOW   Car is turning right
//  HIGH  HIGH  LOW   LOW   LOW   LOW   Car is stoped
//  HIGH  HIGH  HIGH  HIGH  HIGH  HIGH  Car is stoped
//  LOW   LOW   N/A   N/A   N/A   N/A   Car is stoped


// L298n module IO Pin
#define ENA 5
#define ENB 6
#define IN1 7
#define IN2 8
#define IN3 9
#define IN4 11


// IR Codes
#define IR_F 16736925  // Forward
#define IR_B 16754775  // Back
#define IR_L 16720605  // Left
#define IR_R 16761405  // Right
#define IR_S 16712445  // Stop


#define IR_RECV_PIN  12

int elg_init_flag;

IRrecv irrecv(IR_RECV_PIN);
decode_results results;

void elg_set_forward() {
  digitalWrite(ENA, HIGH);
  digitalWrite(ENB, HIGH);
  digitalWrite(IN1, HIGH);
  digitalWrite(IN2, LOW);
  digitalWrite(IN3, LOW);
  digitalWrite(IN4, HIGH);
}

void elg_set_back() {
  digitalWrite(ENA, HIGH);
  digitalWrite(ENB, HIGH);
  digitalWrite(IN1, LOW);
  digitalWrite(IN2, HIGH);
  digitalWrite(IN3, HIGH);
  digitalWrite(IN4, LOW);
}

void elg_set_left() {
  digitalWrite(ENA, HIGH);
  digitalWrite(ENB, HIGH);
  digitalWrite(IN1, LOW);
  digitalWrite(IN2, HIGH);
  digitalWrite(IN3, LOW);
  digitalWrite(IN4, HIGH);
}

void elg_set_right() {
  digitalWrite(ENA, HIGH);
  digitalWrite(ENB, HIGH);
  digitalWrite(IN1, HIGH);
  digitalWrite(IN2, LOW);
  digitalWrite(IN3, HIGH);
  digitalWrite(IN4, LOW);
}

void elg_stop() {
  digitalWrite(ENA, LOW);
  digitalWrite(ENB, LOW);
}

void elg_forward_with_delay(int ms) {
  elg_set_forward();
  delay(ms);
}

void elg_back_with_delay(int ms) {
  elg_set_back();
  delay(ms);
}

void elg_left_with_delay(int ms) {
  elg_set_left();
  delay(ms);
}

void elg_right_ms_with_delay(int ms) {
  elg_set_right();
  delay(ms);
}

void elg_forward() {
  elg_set_forward();
}

void elg_back() {
  elg_set_back();
}

void elg_left() {
  elg_set_left();
}

void elg_right() {
  elg_set_right();
}

void elg_set_init_true() {
  elg_init_flag = 1;
}

void elg_set_init_false() {
  elg_init_flag = 0;
}

int elg_is_init_true() {
  return elg_init_flag;
}


void setup() {
  elg_set_init_true();

  Serial.begin(9600);

  pinMode(IN1, OUTPUT);
  pinMode(IN2, OUTPUT);
  pinMode(IN3, OUTPUT);
  pinMode(IN4, OUTPUT);
  pinMode(ENA, OUTPUT);
  pinMode(ENB, OUTPUT);

  elg_stop();
}

void elg_init() {
  for (int i = 0; i < 10; i++) {
    elg_forward_with_delay(300);
    elg_back_with_delay(300);
    elg_left_with_delay(300);
  }
  elg_stop();
}

void loop() {

  if (elg_is_init_true()) {
    elg_init();
    elg_set_init_false();
  }

  if (Serial.available()) {
    char val = Serial.read();
    switch (val) {
      case 'f':
        elg_forward();
        break;
      case 'b':
        elg_back();
        break;
      case 'l':
        elg_left();
        break;
      case 'r':
        elg_right();
        break;
      case 's':
        elg_stop();
        break;
      default:
        break;
    }
  }

  if (irrecv.decode(&results)) {
    unsigned long val = results.value;
    irrecv.resume();
    switch (val) {
      case IR_F:
        elg_forward();
        break;
      case IR_B:
        elg_back();
        break;
      case IR_L:
        elg_left();
        break;
      case IR_R:
        elg_right();
        break;
      case IR_S:
        elg_stop();
        break;
      default: 
        break;
    }
  }

}
