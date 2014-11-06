#include <arduino/Arduino.h>
#include <comm/arduino_mavlink_packet_handler.hpp>
#include <comm/custom.h>
#include <actuator/motor.h>

int timer1_counter;
const size_t pwm_pin = 11;
const size_t dir_pin = 12; 
const float outmax = 100.0; 
const float outmin = -100.0; 

const int numerator= 24; // for slave with 13 ppr encoder
const int denominator= 5; // for slave with 13 ppr encoder
// const int numerator= 600; // for slave with 3 ppr encoder
// const int denominator= 29; // for slave with 3 ppr encoder


const int encoder_out_a_pin = 2;
const int encoder_out_b_pin = 3;
const int encoder_resolution = 360; //Only needed for Initialization, not used unless .rot() is called

int received_speed;
float speed;

trui::Motor motor(pwm_pin, dir_pin, encoder_out_a_pin, encoder_out_b_pin, encoder_resolution, outmax, outmin, numerator, denominator);

void setup()
{
  
  // initialize timer1 
  noInterrupts();           // disable all interrupts
  TCCR1A = 0;      // set entire TCCR1A register to 0
  TCCR1B = 0;     // same for TCCR1B
 
  // set compare match register to desired timer count:
  OCR1A = 780; // 780 for 50ms
  // target time = timer resolution * (timer counts + 1)
  // (timer counts + 1) = target time / timer resolution
  // (timer counts + 1) = 50.10^-3 / (1 / (16.10^6 / 1024))

  // turn on CTC mode:
  TCCR1B |= (1 << WGM12);
  // Set CS10 and CS12 bits for 1024 prescaler:
  TCCR1B |= (1 << CS10);
  TCCR1B |= (1 << CS12);
  // enable timer compare interrupt:
  TIMSK1 |= (1 << OCIE1A);
  interrupts();             // enable all interrupts
}

ISR(TIMER1_COMPA_vect)        // interrupt service routine 
{
  //interrupts();
  motor.set_speed(speed);
}

int main() {
  init();// this needs to be called before setup() or some functions won't work there
  setup();
  motor.setup();

  Serial.begin(9600);

  long timeNow = 0, timeOld = 0;
  int incoming_byte = 0;
  uint8_t buffer[7];

  while (true) {

    //1. Wait msg from master

    while(true){
      if(Serial.available() >= 7) {
        buffer[0]= Serial.read();
        if (buffer[0] == 0xCE){
          for (int i=1; i<7; i++) {
            buffer[i]= Serial.read();
          }
          break;
        }
      }      
    }

    received_speed = (buffer[2] << 8);
    received_speed |= (buffer[1]);

    if (buffer[3] == 0x0C) speed= -1.0 * received_speed;
    else if (buffer[3] == 0xCC) speed= 1.0 * received_speed;
    
    // Serial.write(buffer[3]);

    // //2. Identify msg
    // //2A. If msgid = manual_setpoint, set speed control to cmd_speed
    // if ( (rx_msg.msgid==set_speed_msgid) ) {
    //   mavlink_manual_setpoint_t msg;
    //   mavlink_msg_manual_setpoint_decode(&rx_msg, &msg);
      
    //   speed= msg.roll;
    // } 
    // //2B. If msgid = asking for actual speed, send back actual speed to master
    // else if ( (rx_msg.msgid==actual_speed_query_msgid) ) {
    //   float actual_speed;
    //   mavlink_message_t msg_to_master;
    //   uint8_t system_id= MAV_TYPE_RBMT;
    //   uint8_t component_id= MAV_COMP_ID_ARDUINO_SLAVE1;

    //   uint32_t time_boot_ms= millis(); 
    //   mavlink_msg_manual_setpoint_pack(system_id, component_id, &msg_to_master, time_boot_ms, actual_speed, 0., 0., 0., 0, 0);

    //   uint8_t buf[MAVLINK_MAX_PACKET_LEN];
    //   uint16_t len = mavlink_msg_to_send_buffer(buf, &msg_to_master);

    //   Serial.write(buf, len);
    // }
    
  }

  return 0;
}
