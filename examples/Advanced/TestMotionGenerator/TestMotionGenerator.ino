/**
 * This code example is in the public domain.
 * http://www.botnroll.com
 *
 * Description:
 * The robot draws shapes by taking in speed values in rpm (rotations per
 * minute) and using the ShapeGenerator class.
 */

// #include <BnrOneAPlus.h>  // Bot'n Roll ONE A+ library
// #include <SPI.h>  // SPI communication library required by BnrOneAPlus.cpp

// Constants definition
#define SSPIN 2                 // Slave Select (SS) pin for SPI communication
#define MINIMUM_BATTERY_V 10.5  // Safety voltage for discharging the battery

// // Slip factor depends on the surface
// // wood: 1.0
// // vinyl: 0.985
// // carpet: 0.985

#include <Arduino.h>  // Include Arduino library for Serial
#include <BnrOneAPlus.h>

#include "utility/ControlUtils.h"
#include "utility/RobotParams.h"

#define STRAIGHT_MOTION 32767
#define TICKS_LEFT_LOW_SPEED 4000
#define MIN_SPEED_MMPS 100

BnrOneAPlus one;  // Object to control the Bot'n Roll ONE A
const float SLIP_FACTOR = 0.94;

class MotionGeneratorTest {
 public:
  MotionGeneratorTest(const BnrOneAPlus& one,
                      const float slip_factor,
                      const RobotParams& robot_params = RobotParams())
      : one_(one),
        slip_factor_(slip_factor),
        axis_length_mm_(robot_params.axis_length_mm),
        cut_(ControlUtils(robot_params)) {}

  void resetEncoders() const { one_.resetEncoders(); }

  PoseSpeeds computePoseSpeeds(const float speed,
                               const float radius_of_curvature_mm,
                               const int direction) const {
    float linear_speed = speed;
    float angular_speed_rad = 0;
    if (radius_of_curvature_mm != 0) {
      if (radius_of_curvature_mm == STRAIGHT_MOTION) {
        angular_speed_rad = 0;
        linear_speed = speed;
      } else {
        angular_speed_rad = direction * (speed / radius_of_curvature_mm);
      }
    } else {
      angular_speed_rad = direction * (speed / (axis_length_mm_ / 2));
      linear_speed = 0;
    }
    return PoseSpeeds(linear_speed, angular_speed_rad);
  }

  PoseSpeeds maybeSlowDown(const PoseSpeeds& pose_speeds,
                           const float speed,
                           const float pulses_remaining,
                           const float slow_down_thresh,
                           const float radius_of_curvature_mm,
                           const int direction) const {
    if (pulses_remaining < TICKS_LEFT_LOW_SPEED &&
        pulses_remaining < slow_down_thresh && pulses_remaining > 0) {
      float ratio = pulses_remaining / TICKS_LEFT_LOW_SPEED;
      float slow_speed = speed * ratio;
      slow_speed = max(MIN_SPEED_MMPS, slow_speed);  // Cap to min speed
      return computePoseSpeeds(slow_speed, radius_of_curvature_mm, direction);
    }
    return pose_speeds;
  }

  void moveAndSlowDown(const long int total_pulses,
                       const float speed,
                       const int direction,
                       const float radius_of_curvature_mm,
                       const float slow_down_thresh) const {
    const auto pose_speeds =
        computePoseSpeeds(speed, radius_of_curvature_mm, direction);

    long int encoder_count = 0;
    one_.moveRpm(200, 200);
    while (encoder_count < total_pulses) {
      int left_encoder = one_.readAndResetLeftEncoder();
      int right_encoder = one_.readAndResetRightEncoder();
      encoder_count += (long int)((abs(left_encoder) + abs(right_encoder)) / 2);
      long int pulses_remaining = round(total_pulses - encoder_count);
      Serial.println(pulses_remaining);
      if (pulses_remaining < 0) break;
      maybeSlowDown(pose_speeds,
                    speed,
                    pulses_remaining,
                    slow_down_thresh,
                    radius_of_curvature_mm,
                    direction);
      const auto wheel_speeds_mmps = cut_.computeWheelSpeeds(
          pose_speeds.getLinearMmps(), pose_speeds.getAngularRad());
      const auto wheel_speeds_rpm = cut_.computeSpeedsRpm(wheel_speeds_mmps);
      one_.moveRpm(wheel_speeds_rpm.getLeft(), wheel_speeds_rpm.getRight());
    }
    one_.brake(100, 100);
  }

  int getSign(const float value) const { return (value >= 0) ? 1 : -1; }

  float applySlip(const float value) const {
    return round(value / slip_factor_);
  }

  void moveStraightAtSpeed(const float distance,
                           const float speed = 200,
                           const float slow_down_distance = 0) const {
    float abs_distance = abs(distance);
    auto total_pulses = cut_.computePulsesFromDistance(abs_distance);
    total_pulses = applySlip(total_pulses);
    Serial.print("total_pulses: ");
    Serial.println(total_pulses);

    float abs_slow_down_distance = abs(slow_down_distance);
    auto slow_down_pulses =
        cut_.computePulsesFromDistance(abs_slow_down_distance);
    slow_down_pulses = applySlip(slow_down_pulses);
    Serial.print("slow_down_pulses: ");
    Serial.println(slow_down_pulses);
    delay(2000);

    resetEncoders();
    moveAndSlowDown(total_pulses, speed, 1, STRAIGHT_MOTION, slow_down_pulses);
  }

  void rotateAngleDegAtSpeed(const float angle_deg,
                             const float speed,
                             const float radius_of_curvature_mm,
                             const float slow_down_thresh_deg) const {
    // BUG: Adjust conversion factor as needed
    float total_pulses = abs(angle_deg) * 10;
    total_pulses = applySlip(total_pulses);
    // BUG: Adjust conversion factor as needed
    float slow_down_pulses_thresh = abs(slow_down_thresh_deg) * 10;
    resetEncoders();
    moveAndSlowDown(total_pulses,
                    abs(speed),
                    getSign(angle_deg),
                    abs(radius_of_curvature_mm),
                    slow_down_pulses_thresh);
  }

 private:
  const BnrOneAPlus& one_;  ///< Reference to BnrOneAPlus object.
  float slip_factor_;       ///< Slip factor for the robot.
  float axis_length_mm_;    ///< Axis length of the robot in millimeters.
  ControlUtils cut_;        ///< Control utils object
};

MotionGeneratorTest one_mg(one, SLIP_FACTOR);

void moveStraight() { one_mg.moveStraightAtSpeed(500); }

void setup() {
  Serial.begin(115200);   // Set baud rate to 115200bps for printing values at
                          // serial monitor.
  one.spiConnect(SSPIN);  // Start SPI communication module
  one.stop();             // Stop motors
  one.setMinBatteryV(MINIMUM_BATTERY_V);  // Battery discharge protection
  one.lcd1("  Draw Shapes   ");
  one.lcd2("www.botnroll.com");
  Serial.println("Get ready");
  delay(3000);
  Serial.println("Go");
  moveStraight();
  one.stop();
  Serial.end();
}

void loop() {
  // Empty loop
}
