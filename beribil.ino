#include <IRremote.h>
#include <Servo.h>

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


#define IR_RECV_PIN 12

#define HEAD_PIN 3

#define HEAD_MIN_DEGREE_PULSE 700
#define HEAD_MAX_DEGREE_PULSE 2400

#define HEAD_STRAIGHT 90
#define HEAD_RIGTH_MAX 0
#define HEAD_LEFT_MAX 180
#define HEAD_STEP 20

int elg_init_flag;

IRrecv irrecv(IR_RECV_PIN);
decode_results results;

Servo elg_head;

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

void elg_set_head_straight() {
  elg_head.write(HEAD_STRAIGHT);
}

void elg_head_straight_with_delay(int ms) {
  elg_set_head_straight();
  delay(ms);
}

boolean elg_head_right() {
  int current = elg_head.read();
  int next = current - HEAD_STEP;

  boolean res = true;
  if (next <= HEAD_RIGTH_MAX) {
    next = HEAD_RIGTH_MAX;
    res = false;
  }
  elg_head.write(next);
  return res;
}

boolean elg_head_left() {
  int current = elg_head.read();
  int next = current + HEAD_STEP;

  boolean res = true;
  if (next >= HEAD_LEFT_MAX) {
    next = HEAD_LEFT_MAX;
    res = false;
  }
  elg_head.write(next);
  return res;
}

boolean elg_head_right_with_delay(int ms) {
  boolean res = elg_head_right();
  delay(ms);
  return res;
}

boolean elg_head_left_with_delay(int ms) {
  boolean res = elg_head_left();
  delay(ms);
  return res;
}

void elg_set_init_setup() {
  elg_init_flag = 1;
}

void elg_set_init_false() {
  elg_init_flag = 0;
}

int elg_is_init_true() {
  return elg_init_flag;
}

void elg_servo_setup() {
  pinMode(IN1, OUTPUT);
  pinMode(IN2, OUTPUT);
  pinMode(IN3, OUTPUT);
  pinMode(IN4, OUTPUT);
  pinMode(ENA, OUTPUT);
  pinMode(ENB, OUTPUT);
}

void elg_ble_setup() {
  Serial.begin(9600);
}

void elg_ir_setup() {
  irrecv.enableIRIn();
}

void elg_head_setup() {
  elg_head.attach(HEAD_PIN, HEAD_MIN_DEGREE_PULSE, HEAD_MAX_DEGREE_PULSE);
  elg_set_head_straight();
}

void setup() {
  elg_set_init_setup();
  elg_servo_setup();
  elg_ble_setup();
  elg_ir_setup();
  elg_head_setup();
  elg_stop();
}

void elg_init() {
  while (elg_head_right_with_delay(1000));
  elg_head_straight_with_delay(1000);
  while (elg_head_left_with_delay(1000));
  elg_head_straight_with_delay(1000);

  for (int i = 0; i < 5; i++) {
    elg_forward_with_delay(300);
    elg_back_with_delay(300);
    elg_left_with_delay(300);
  }
  elg_stop();
}

char* elg_read_ble() {

  return NULL;
}

void loop() {

  if (elg_is_init_true()) {
    elg_init();
    elg_set_init_false();
  }

  if (Serial.available() > 0) {
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
