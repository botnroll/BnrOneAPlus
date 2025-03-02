#pragma once

#include "RobotParams.h"

/**
 * @class Pose
 * @brief Represents the pose of the robot in a 2D coordinate system.
 */
class Pose {
 public:
  /**
   * @brief Constructor for Pose.
   * @param x_mm_in Initial x-coordinate in millimeters.
   * @param y_mm_in Initial y-coordinate in millimeters.
   * @param theta_rad_in Initial orientation in radians.
   */
  Pose(const float x_mm_in = 0,
       const float y_mm_in = 0,
       const float theta_rad_in = 0);

  /**
   * @brief Updates the pose based on the given distance and angle change.
   * @param delta_distance_mm Distance change in millimeters.
   * @param delta_theta_rad Angle change in radians.
   */
  void updatePose(const float delta_distance_mm, const float delta_theta_rad);

  /**
   * @brief Gets the x-coordinate in millimeters.
   * @return x-coordinate in millimeters.
   */
  float getXMm() const;

  /**
   * @brief Gets the y-coordinate in millimeters.
   * @return y-coordinate in millimeters.
   */
  float getYMm() const;

  /**
   * @brief Gets the orientation in radians.
   * @return Orientation in radians.
   */
  float getThetaRad() const;

 private:
  float x_mm;       ///< x-coordinate in millimeters.
  float y_mm;       ///< y-coordinate in millimeters.
  float theta_rad;  ///< Orientation in radians.
};

/**
 * @class PoseSpeeds
 * @brief Represents the linear and angular speeds of the robot.
 */
class PoseSpeeds {
 public:
  /**
   * @brief Constructor for PoseSpeeds.
   * @param linear Linear speed in millimeters per second.
   * @param angular Angular speed in radians per second.
   */
  PoseSpeeds(const float linear = 0, const float angular = 0);

  /**
   * @brief Gets the linear speed in millimeters per second.
   * @return Linear speed in millimeters per second.
   */
  float getLinearMmps() const;

  /**
   * @brief Gets the angular speed in radians per second.
   * @return Angular speed in radians per second.
   */
  float getAngularRad() const;

 private:
  float linear_mmps;  ///< Linear speed in millimeters per second.
  float angular_rad;  ///< Angular speed in radians per second.
};

/**
 * @class WheelSpeeds
 * @brief Represents the speeds of the left and right wheels of the robot.
 */
class WheelSpeeds {
 public:
  /**
   * @brief Constructor for WheelSpeeds.
   * @param left_speed Speed of the left wheel in millimeters per second.
   * @param right_speed Speed of the right wheel in millimeters per second.
   */
  WheelSpeeds(const float left_speed = 0, const float right_speed = 0);

  /**
   * @brief Gets the speed of the left wheel in millimeters per second.
   * @return Speed of the left wheel in millimeters per second.
   */
  float getLeft() const;

  /**
   * @brief Gets the speed of the right wheel in millimeters per second.
   * @return Speed of the right wheel in millimeters per second.
   */
  float getRight() const;

 private:
  float left;   ///< Speed of the left wheel in millimeters per second.
  float right;  ///< Speed of the right wheel in millimeters per second.
};

/**
 * @class ControlUtils
 * @brief Collection of utility methods to compute speeds, distances, and
 * pulses.
 */
class ControlUtils {
 public:
  /**
   * @brief Constructor for ControlUtils.
   * @param params Robot parameters.
   * @param min_speed_mmps Minimum speed in millimeters per second.
   */
  ControlUtils(const RobotParams& params = RobotParams(),
               const float min_speed_mmps = 0);

  /**
   * @brief Gets the axis length in millimeters.
   * @return Axis length in millimeters.
   */
  float getAxisLengthMm() const;

  /**
   * @brief Converts a value from one range to another.
   * @param x_value Value to convert.
   * @param x_min Minimum value of the input range.
   * @param x_max Maximum value of the input range.
   * @param y_min Minimum value of the output range.
   * @param y_max Maximum value of the output range.
   * @return Converted value in the output range.
   */
  float convertRange(const float x_value,
                     const float x_min,
                     const float x_max,
                     const float y_min,
                     const float y_max) const;

  /**
   * @brief Computes the number of revolutions from the number of pulses.
   * @param pulses Number of pulses.
   * @return Number of revolutions.
   */
  float computeRevFromPulses(const int pulses) const;

  /**
   * @brief Computes the distance from the number of revolutions.
   * @param revolutions Number of revolutions.
   * @return Distance in millimeters.
   */
  float computeDistanceFromRev(const float revolutions) const;

  /**
   * @brief Computes the distance from the number of pulses.
   * @param pulses Number of pulses.
   * @return Distance in millimeters.
   */
  float computeDistanceFromPulses(const int pulses) const;

  /**
   * @brief Computes the speed from the distance and time.
   * @param distance_mm Distance in millimeters.
   * @param time_ms Time in milliseconds.
   * @return Speed in millimeters per second.
   */
  float computeSpeedFromDistance(const float distance_mm,
                                 const int time_ms) const;

  /**
   * @brief Computes the speed from the number of pulses and time.
   * @param num_pulses Number of pulses.
   * @param time_ms Time in milliseconds.
   * @return Speed in millimeters per second.
   */
  float computeSpeedFromPulses(const int num_pulses, const int time_ms) const;

  /**
   * @brief Computes the distance from the speed and time.
   * @param speed_mmps Speed in millimeters per second.
   * @param time_ms Time in milliseconds.
   * @return Distance in millimeters.
   */
  float computeDistanceFromSpeed(const float speed_mmps,
                                 const int time_ms) const;

  /**
   * @brief Computes the number of revolutions from the distance.
   * @param distance_mm Distance in millimeters.
   * @return Number of revolutions.
   */
  float computeRevolutionsFromDistance(const float distance_mm) const;

  /**
   * @brief Computes the arc length from the angle and radius of curvature.
   * @param angle_rad Angle in radians.
   * @param radius_of_curvature_mm Radius of curvature in millimeters.
   * @return Arc length in millimeters.
   */
  float computeArcLength(const float angle_rad,
                         const float radius_of_curvature_mm) const;

  /**
   * @brief Computes the number of pulses from the number of revolutions.
   * @param revolutions Number of revolutions.
   * @return Number of pulses.
   */
  int computePulsesFromRev(const float revolutions) const;

  /**
   * @brief Computes the number of pulses from the speed and time.
   * @param speed_mmps Speed in millimeters per second.
   * @param time_ms Time in milliseconds.
   * @return Number of pulses.
   */
  int computePulsesFromSpeed(const float speed_mmps, const int time_ms) const;

  /**
   * @brief Computes the number of pulses from the distance.
   * @param distance Distance in millimeters.
   * @return Number of pulses.
   */
  int computePulsesFromDistance(const float distance) const;

  /**
   * @brief Computes the number of pulses from the angle and radius of
   * curvature.
   * @param angle_rad Angle in radians.
   * @param radius_of_curvature_mm Radius of curvature in millimeters.
   * @return Number of pulses.
   */
  int computePulsesFromAngleAndCurvature(
      const float angle_rad, const float radius_of_curvature_mm = 0) const;

  /**
   * @brief Converts a speed percentage to speed in millimeters per second.
   * @param desired_speed_percentage Desired speed percentage.
   * @return Speed in millimeters per second.
   */
  float convertToMmps(const float desired_speed_percentage) const;

  /**
   * @brief Converts a speed in millimeters per second to speed percentage.
   * @param desired_speed_mmps Desired speed in millimeters per second.
   * @return Speed percentage.
   */
  float convertToPercentage(const float desired_speed_mmps) const;

  /**
   * @brief Computes the pose speeds from the left and right wheel speeds.
   * @param left_speed Speed of the left wheel in millimeters per second.
   * @param right_speed Speed of the right wheel in millimeters per second.
   * @return PoseSpeeds object containing the linear and angular speeds.
   */
  PoseSpeeds computePoseSpeeds(const float left_speed,
                               const float right_speed) const;

  /**
   * @brief Computes the wheel speeds from the linear and angular speeds.
   * @param linear_speed Linear speed in millimeters per second.
   * @param angular_speed_rad Angular speed in radians per second.
   * @return WheelSpeeds object containing the left and right wheel speeds.
   */
  WheelSpeeds computeWheelSpeeds(const float linear_speed,
                                 const float angular_speed_rad) const;

  /**
   * @brief Computes the speeds in RPM from the speeds in millimeters per
   * second.
   * @param wheel_speeds_mmps WheelSpeeds object containing the speeds in
   * millimeters per second.
   * @return WheelSpeeds object containing the speeds in RPM.
   */
  WheelSpeeds computeSpeedsRpm(const WheelSpeeds& wheel_speeds_mmps) const;

 private:
  float axis_length_mm;       ///< Axis length in millimeters.
  float wheel_diameter_mm;    ///< Wheel diameter in millimeters.
  int pulses_per_rev;         ///< Number of pulses per revolution.
  float max_speed_mmps;       ///< Maximum speed in millimeters per second.
  float min_speed_mmps;       ///< Minimum speed in millimeters per second.
  float spot_rotation_delta;  ///< Correction for spot rotations.
};
