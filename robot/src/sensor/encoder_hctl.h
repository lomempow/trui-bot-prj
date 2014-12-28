#ifndef ENCODER_HCTL_H_
#define ENCODER_HCTL_H_

#include <arduino/Arduino.h>

namespace crim {

class EncoderHctl {
 public:
  EncoderHctl(size_t en_1_pin, size_t en_2_pin, size_t xy_pin, size_t oe_pin, size_t sel_1_pin, size_t sel_2_pin, size_t reset_x_pin, size_t reset_y_pin);
  ~EncoderHctl();

  /**
   * @brief return the number of rising-edge of the a phase pulse
   * 
   */
  int32_t pos();
  
  /**
   * @brief return how much rotarion in radians, can be negative
   * 
   */
  double rot();
  
 private:
  size_t en_1_pin_;
  size_t en_2_pin_;
  size_t xy_pin_;
  size_t oe_pin_;
  size_t sel_1_pin_;
  size_t sel_2_pin_;
  size_t reset_x_pin_;
  size_t reset_y_pin_;

  int8_t hi_old_;
  int8_t hi_new_;
  int8_t second_old_;
  int8_t second_new_;
  int8_t third_old_;
  int8_t third_new_;
  int8_t lo_old_;
  int8_t lo_new_;

  static int8_t result_hi_;
  static int8_t result_2nd_;
  static int8_t result_3rd_;
  static int8_t result_lo_;

  static int64_t result_new_;
  static int64_t result_old_x_;
  static int64_t result_old_y_;
  static int32_t result_;

  void get_hi();
  void get_2nd();
  void get_3rd();
  void get_lo();

};

}// namespace crim

#endif