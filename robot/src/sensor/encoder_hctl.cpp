#include "encoder_hctl.h"

using namespace crim;

int64_t result_new_ = 0;
int64_t result_old_x_ = 0;
int64_t result_old_y_ = 0;
int32_t result_ = 0;

EncoderHctl::EncoderHctl(size_t en_1_pin, size_t en_2_pin, size_t xy_pin, size_t oe_pin, size_t sel_1_pin, size_t sel_2_pin, size_t reset_x_pin, size_t reset_y_pin)
    : en_1_pin_(en_1_pin), en_2_pin_(en_2_pin), xy_pin_(xy_pin), oe_pin_(oe_pin), sel_1_pin_(sel_1_pin), sel_2_pin_(sel_2_pin), reset_x_pin_(reset_x_pin), reset_y_pin_(reset_y_pin) {

  pinMode(en_1_pin_, OUTPUT);
  pinMode(en_2_pin_, OUTPUT);
  pinMode(xy_pin_, OUTPUT);
  pinMode(oe_pin_, OUTPUT);
  pinMode(sel_1_pin_, OUTPUT);
  pinMode(sel_2_pin_, OUTPUT);
  DDRB = B00000000;

  digitalWrite(en_1_pin_, HIGH); //EN1 = 1
  digitalWrite(en_2_pin_, LOW); //EN2 = 0
  digitalWrite(xy_pin_, LOW); //Select X-axis

}

EncoderHctl::~EncoderHctl() {

}

int32_t EncoderHctl::pos() {
  digitalWrite(oe_pin_, HIGH); //Disable OE
  delay(25);

  digitalWrite(sel_1_pin_, LOW); //SEL1 = 0 (MSB)
  digitalWrite(sel_2_pin_, HIGH); //SEL2 = 1 (MSB)
  digitalWrite(oe_pin_, LOW); //Enable OE
  EncoderHctl::get_hi(); //Get MSB

  digitalWrite(sel_1_pin_, HIGH); //SEL1 = 1 (2nd Byte) 
  digitalWrite(sel_2_pin_, HIGH); //SEL2 = 1 (2nd Byte) 
  EncoderHctl::get_2nd(); //Get 2nd Byte

  digitalWrite(sel_1_pin_, LOW); //SEL1 = 0 (3rd Byte) 
  digitalWrite(sel_2_pin_, LOW); //SEL2 = 0 (3rd Byte) 
  EncoderHctl::get_3rd(); //Get 3rd Byte

  digitalWrite(sel_1_pin_, HIGH); //SEL1 = 1 (LSB) 
  digitalWrite(sel_2_pin_, LOW); //SEL2 = 0 (LSB) 
  EncoderHctl::get_lo(); //Get LSB

  digitalWrite(oe_pin_, HIGH); //Disable OE
  delay(25);

  result_ = (result_hi_ << 24); //Assign MSB
  result_ |= (result_2nd_ << 16); //Assign 2nd Byte
  result_ |= (result_3rd_ << 8); //Assign 3rd Byte
  result_ |= result_lo_; //Assign LSB

  return result_;
}

void EncoderHctl::get_hi() {
  hi_old_ = PINB;
  hi_new_ = PINB;
  if (hi_new_ == hi_old_){
    result_hi_ = hi_new_;
  } else {
    EncoderHctl::get_hi();
  }
}

void EncoderHctl::get_2nd() {
  second_old_ = PINB;
  second_new_ = PINB;
  if (second_new_ == second_old_){
    result_2nd_ = second_new_;
  } else {
    EncoderHctl::get_2nd();
  }
}

void EncoderHctl::get_3rd() {
  third_old_ = PINB;
  third_new_ = PINB;
  if (third_new_ == third_old_){
    result_3rd_ = third_new_;
  } else {
    EncoderHctl::get_3rd();
  }
}

void EncoderHctl::get_lo() {
  lo_old_ = PINB;
  lo_new_ = PINB;
  if (lo_new_ == lo_old_){
    result_lo_ = lo_new_;
  } else {
    EncoderHctl::get_lo();
  }
}