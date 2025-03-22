#include "ControlUtils.h"

#include <Arduino.h>  // Include Arduino library for math functions

// Pose class implementation
Pose::Pose(const float x_mm_in, const float y_mm_in, const float theta_rad_in)
    : x_mm(x_mm_in), y_mm(y_mm_in), theta_rad(theta_rad_in) {}

void Pose::updatePose(const float delta_distance_mm,
                      const float delta_theta_rad) {
  x_mm += delta_distance_mm * cos(theta_rad + delta_theta_rad / 2.0);
  y_mm += delta_distance_mm * sin(theta_rad + delta_theta_rad / 2.0);
  theta_rad += delta_theta_rad;
}

float Pose::getXMm() const { return x_mm; }
float Pose::getYMm() const { return y_mm; }
float Pose::getThetaRad() const { return theta_rad; }

// PoseSpeeds class implementation
PoseSpeeds::PoseSpeeds(const float linear, const float angular)
    : linear_mmps(linear), angular_rad(angular) {}

float PoseSpeeds::getLinearMmps() const { return linear_mmps; }
float PoseSpeeds::getAngularRad() const { return angular_rad; }

// WheelSpeeds class implementation
WheelSpeeds::WheelSpeeds(const float left_speed, const float right_speed)
    : left(left_speed), right(right_speed) {}

float WheelSpeeds::getLeft() const { return left; }
float WheelSpeeds::getRight() const { return right; }

// ControlUtils class implementation
ControlUtils::ControlUtils(const RobotParams& params,
                           const float min_speed_mmps)
    : axis_length_mm_(params.axis_length_mm),
      wheel_diameter_mm_(params.wheel_diameter_mm),
      pulses_per_rev_(params.pulses_per_rev),
      max_speed_mmps_(params.max_speed_rpm * PI * wheel_diameter_mm_ / 60),
      min_speed_mmps_(min_speed_mmps),
      spot_rotation_delta(0) {}

float ControlUtils::getAxisLengthMm() const { return axis_length_mm_; }

float ControlUtils::convertRange(const float x_value,
                                 const float x_min,
                                 const float x_max,
                                 const float y_min,
                                 const float y_max) const {
  const float x_range = x_max - x_min;
  const float y_range = y_max - y_min;

  // Avoid division by zero
  if (x_range == 0) {
    return y_min;
  }

  // Calculate the converted value
  const float y = ((x_value - x_min) / x_range) * y_range + y_min;
  return y;
}

float ControlUtils::computeRevFromPulses(const int pulses) const {
  return static_cast<float>(pulses) / pulses_per_rev_;
}

float ControlUtils::computeDistanceFromRev(const float revolutions) const {
  return PI * wheel_diameter_mm_ * revolutions;
}

float ControlUtils::computeDistanceFromPulses(const int pulses) const {
  const float rev = computeRevFromPulses(pulses);
  return computeDistanceFromRev(rev);
}

float ControlUtils::computeSpeedFromDistance(const float distance_mm,
                                             const int time_ms) const {
  return (distance_mm * 1000) / time_ms;
}

float ControlUtils::computeSpeedFromPulses(const int num_pulses,
                                           const int time_ms) const {
  const float revolutions = computeRevFromPulses(num_pulses);
  const float distance_mm = computeDistanceFromRev(revolutions);
  return computeSpeedFromDistance(distance_mm, time_ms);
}

float ControlUtils::computeDistanceFromSpeed(const float speed_mmps,
                                             const int time_ms) const {
  return (speed_mmps * time_ms) / 1000.0;
}

float ControlUtils::computeRevolutionsFromDistance(
    const float distance_mm) const {
  const float perimeter_of_circle = PI * wheel_diameter_mm_;
  return distance_mm / perimeter_of_circle;
}

float ControlUtils::computeArcLength(const float angle_rad,
                                     const float radius_of_curvature_mm) const {
  float arc_length_mm = 0.0;
  if (abs(radius_of_curvature_mm) > 0.1) {
    arc_length_mm = angle_rad * radius_of_curvature_mm;
  } else {
    arc_length_mm = (angle_rad * (float)axis_length_mm_) / 2.0;
  }
  return arc_length_mm;
}

long int ControlUtils::computePulsesFromRev(const float revolutions) const {
  return static_cast<long int>(round(pulses_per_rev_ * revolutions));
}

long int ControlUtils::computePulsesFromSpeed(const float speed_mmps,
                                              const int time_ms) const {
  const float distance_mm = computeDistanceFromSpeed(speed_mmps, time_ms);
  const float revolutions = computeRevolutionsFromDistance(distance_mm);
  return computePulsesFromRev(revolutions);
}

long int ControlUtils::computePulsesFromDistance(const float distance) const {
  const float revolutions = computeRevolutionsFromDistance(distance);
  return computePulsesFromRev(revolutions);
}

long int ControlUtils::computePulsesFromAngleAndCurvature(
    const float angle_rad, const float radius_of_curvature_mm) const {
  const float arc_length_mm =
      computeArcLength(angle_rad, radius_of_curvature_mm);
  return computePulsesFromDistance(arc_length_mm);
}

float ControlUtils::convertToMmps(const float desired_speed_percentage) const {
  return (desired_speed_percentage / 100.0) * max_speed_mmps_;
}

float ControlUtils::convertToPercentage(const float desired_speed_mmps) const {
  return (desired_speed_mmps / max_speed_mmps_) * 100.0;
}

PoseSpeeds ControlUtils::computePoseSpeeds(const float left_speed,
                                           const float right_speed) const {
  const float linear_speed = (left_speed + right_speed) / 2.0;
  const float angular_speed = (right_speed - left_speed) / axis_length_mm_;
  return PoseSpeeds(linear_speed, angular_speed);
}

WheelSpeeds ControlUtils::computeWheelSpeeds(
    const float linear_speed, const float angular_speed_rad) const {
  const float left_speed =
      linear_speed - (angular_speed_rad * axis_length_mm_ / 2.0);
  const float right_speed =
      linear_speed + (angular_speed_rad * axis_length_mm_ / 2.0);
  return WheelSpeeds(left_speed, right_speed);
}

float ControlUtils::mmpsToRpm(const float mmps) const {
  return (mmps * 60) / (wheel_diameter_mm_ * PI);
}

WheelSpeeds ControlUtils::computeSpeedsRpm(
    const WheelSpeeds& wheel_speeds_mmps) const {
  const auto left_rpm = mmpsToRpm(wheel_speeds_mmps.getLeft());
  const auto right_rpm = mmpsToRpm(wheel_speeds_mmps.getRight());
  return WheelSpeeds(left_rpm, right_rpm);
}

float ControlUtils::rpmToMmps(const float speed_rpm) const {
  return (speed_rpm / 60.0) * (wheel_diameter_mm_ * PI);
}

WheelSpeeds ControlUtils::computeSpeedsMmps(
    const WheelSpeeds& wheel_speeds_rpm) const {
  float left_mmps = rpmToMmps(wheel_speeds_rpm.getLeft());
  float right_mmps = rpmToMmps(wheel_speeds_rpm.getRight());
  return WheelSpeeds(left_mmps, right_mmps);
}