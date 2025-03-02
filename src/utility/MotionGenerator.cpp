#include "MotionGenerator.h"

#include <Arduino.h>  // Include Arduino library for Serial
#include <BnrOneAPlus.h>

#define STRAIGHT_MOTION = 32767
#define TICKS_LEFT_LOW_SPEED = 4000
#define MIN_SPEED_MMPS = 100

MotionGenerator::MotionGenerator(BnrOneAPlus& one,
                                 const float slip_factor,
                                 const RobotParams& robot_params)
    : one_(one),
      slip_factor_(slip_factor),
      axis_length_mm_(robot_params.axis_length_mm),
      cut_(ControlUtils(robot_params)) {}

void MotionGenerator::resetEncoders() const { one_.resetEncoders(); }

float MotionGenerator::computeAngularSpeed(const float speed,
                                           const float radius_of_curvature_mm,
                                           const int direction) const {
  if (radius_of_curvature_mm != 0) {
    if (radius_of_curvature_mm == STRAIGHT_MOTION) {  // Straight motion
      return 0;
    } else {
      return direction * (speed / radius_of_curvature_mm);
    }
  } else {
    return direction * (speed / (axis_length_mm_ / 2));
  }
}

void MotionGenerator::maybeSlowDown(float& linear_speed,
                                    float& angular_speed_rad,
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
    linear_speed = slow_speed;
    angular_speed_rad =
        computeAngularSpeed(slow_speed, radius_of_curvature_mm, direction);
  }
}

void MotionGenerator::moveAndSlowDown(const float total_pulses,
                                      const float speed,
                                      const int direction,
                                      const float radius_of_curvature_mm,
                                      const float slow_down_thresh) const {
  float linear_speed = speed;
  float angular_speed_rad =
      computeAngularSpeed(speed, radius_of_curvature_mm, direction);

  float encoder_count = 0;
  while (encoder_count < total_pulses) {
    int left_encoder = abs(one_.readAndResetLeftEncoder());
    int right_encoder = abs(one_.readAndResetRightEncoder());
    encoder_count += (left_encoder + right_encoder) / 2.0;
    float pulses_remaining = round(total_pulses - encoder_count);
    if (pulses_remaining < 0) break;
    maybeSlowDown(linear_speed,
                  angular_speed_rad,
                  speed,
                  pulses_remaining,
                  slow_down_thresh,
                  radius_of_curvature_mm,
                  direction);
    one_.moveRpm(linear_speed, angular_speed_rad);
  }
  one_.brake(100, 100);
}

int MotionGenerator::getSign(const float value) const {
  return (value >= 0) ? 1 : -1;
}

float MotionGenerator::applySlip(const float value) const {
  return round(value / slip_factor_);
}

void MotionGenerator::moveStraightAtSpeed(
    const float distance,
    const float speed,
    const float slow_down_distance) const {
  float abs_distance = abs(distance);
  float abs_slow_down_distance = abs(slow_down_distance);
  // BUG: Adjust conversion factor as needed
  float total_pulses = abs_distance * 10;
  total_pulses = applySlip(total_pulses);
  // BUG: Adjust conversion factor as needed
  float slow_down_pulses = abs_slow_down_distance * 10;
  resetEncoders();
  moveAndSlowDown(total_pulses, speed, 1, 32767, slow_down_pulses);
}

void MotionGenerator::rotateAngleDegAtSpeed(
    const float angle_deg,
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
