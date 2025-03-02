#include "ControlUtils.h"

#include <cmath>

// Pose class implementation
Pose::Pose(const float x_mm_in, const float y_mm_in, const float theta_rad_in)
    : x_mm(x_mm_in), y_mm(y_mm_in), theta_rad(theta_rad_in) {}

void Pose::updatePose(const float delta_distance_mm,
                      const float delta_theta_rad) {
  x_mm += delta_distance_mm * std::cos(theta_rad + delta_theta_rad / 2.0);
  y_mm += delta_distance_mm * std::sin(theta_rad + delta_theta_rad / 2.0);
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
    : axis_length_mm(params.axis_length_mm),
      wheel_diameter_mm(params.wheel_diameter_mm),
      pulses_per_rev(params.pulses_per_rev),
      max_speed_mmps(params.max_speed_rpm * M_PI * wheel_diameter_mm / 60),
      min_speed_mmps(min_speed_mmps),
      spot_rotation_delta(0) {}

float ControlUtils::getAxisLengthMm() const { return axis_length_mm; }

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
  return static_cast<float>(pulses) / pulses_per_rev;
}

float ControlUtils::computeDistanceFromRev(const float revolutions) const {
  return M_PI * wheel_diameter_mm * revolutions;
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
  const float perimeter_of_circle = M_PI * wheel_diameter_mm;
  return distance_mm / perimeter_of_circle;
}

float ControlUtils::computeArcLength(const float angle_rad,
                                     const float radius_of_curvature_mm) const {
  if (std::abs(radius_of_curvature_mm) > 0.1) {
    return angle_rad * radius_of_curvature_mm;
  } else {
    return 0.0;
  }
}

int ControlUtils::computePulsesFromRev(const float revolutions) const {
  return static_cast<int>(std::round(pulses_per_rev * revolutions));
}

int ControlUtils::computePulsesFromSpeed(const float speed_mmps,
                                         const int time_ms) const {
  const float distance_mm = computeDistanceFromSpeed(speed_mmps, time_ms);
  const float revolutions = computeRevolutionsFromDistance(distance_mm);
  return computePulsesFromRev(revolutions);
}

int ControlUtils::computePulsesFromDistance(const float distance) const {
  const float revolutions = computeRevolutionsFromDistance(distance);
  return computePulsesFromRev(revolutions);
}

int ControlUtils::computePulsesFromAngleAndCurvature(
    const float angle_rad, const float radius_of_curvature_mm) const {
  const float arc_length_mm =
      computeArcLength(angle_rad, radius_of_curvature_mm);
  return computePulsesFromDistance(arc_length_mm);
}

float ControlUtils::convertToMmps(const float desired_speed_percentage) const {
  return (desired_speed_percentage / 100.0) * max_speed_mmps;
}

float ControlUtils::convertToPercentage(const float desired_speed_mmps) const {
  return (desired_speed_mmps / max_speed_mmps) * 100.0;
}

PoseSpeeds ControlUtils::computePoseSpeeds(const float left_speed,
                                           const float right_speed) const {
  const float linear_speed = (left_speed + right_speed) / 2.0;
  const float angular_speed = (right_speed - left_speed) / axis_length_mm;
  return PoseSpeeds(linear_speed, angular_speed);
}

WheelSpeeds ControlUtils::computeWheelSpeeds(
    const float linear_speed, const float angular_speed_rad) const {
  const float left_speed =
      linear_speed - (angular_speed_rad * axis_length_mm / 2.0);
  const float right_speed =
      linear_speed + (angular_speed_rad * axis_length_mm / 2.0);
  return WheelSpeeds(left_speed, right_speed);
}

float ControlUtils::computeSpeedsRpm(
    const WheelSpeeds& wheel_speeds_mmps) const {
  return (wheel_speeds_mmps.getLeft() + wheel_speeds_mmps.getRight()) /
         (2.0 * M_PI * wheel_diameter_mm) * 60.0;
}
