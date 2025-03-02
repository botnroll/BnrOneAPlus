#include "MotionGenerator.h"

#include <Arduino.h>  // Include Arduino library for Serial
#include <BnrOneAPlus.h>

#include "ControlUtils.h"

#define STRAIGHT_MOTION 32767
#define TICKS_LEFT_LOW_SPEED 4000
#define MIN_SPEED_MMPS 100

MotionGenerator::MotionGenerator(
    const BnrOneAPlus& one,
    const float slip_factor,
    const RobotParams& robot_params = RobotParams())
    : one_(one),
      slip_factor_(slip_factor),
      axis_length_mm_(robot_params.axis_length_mm),
      cut_(ControlUtils(robot_params)) {}

void MotionGenerator::resetEncoders() const { one_.resetEncoders(); }

PoseSpeeds MotionGenerator::computePoseSpeeds(
    const float speed,
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

PoseSpeeds MotionGenerator::maybeSlowDown(const PoseSpeeds& pose_speeds,
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

void MotionGenerator::moveAndSlowDown(const long int total_pulses,
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

int MotionGenerator::getSign(const float value) const {
  return (value >= 0) ? 1 : -1;
}

float MotionGenerator::applySlip(const float value) const {
  return round(value / slip_factor_);
}

void MotionGenerator::moveStraightAtSpeed(
    const float distance,
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

void MotionGenerator::rotateAngleDegAtSpeed(
    const float angle_deg,
    const float speed,
    const float radius_of_curvature_mm,
    const float slow_down_thresh_deg) const {
  auto total_pulses = cut_.computePulsesFromAngleAndCurvature(
      radians(angle_deg), radius_of_curvature_mm);
  total_pulses = applySlip(total_pulses);
  auto slow_down_pulses_thresh = cut_.computePulsesFromAngleAndCurvature(
      radians(slow_down_thresh_deg), radius_of_curvature_mm);
  slow_down_pulses_thresh = applySlip(slow_down_pulses_thresh);
  resetEncoders();
  moveAndSlowDown(total_pulses,
                  abs(speed),
                  getSign(angle_deg),
                  abs(radius_of_curvature_mm),
                  slow_down_pulses_thresh);
}
