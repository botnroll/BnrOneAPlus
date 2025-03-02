#pragma once

/**
 * @class RobotParams
 * @brief Set of robot parameters.
 */
class RobotParams {
 public:
  /**
   * @brief Constructor for RobotParams.
   * @param max_speed_rpm_in Maximum speed in RPM.
   * @param axis_length_mm_in Axis length in millimeters.
   * @param wheel_diameter_mm_in Wheel diameter in millimeters.
   * @param pulses_per_rev_in Number of pulses per revolution.
   */
  RobotParams(const int max_speed_rpm_in = 300,
              const float axis_length_mm_in = 165,
              const float wheel_diameter_mm_in = 63,
              const int pulses_per_rev_in = 2251);

  int max_speed_rpm;        ///< Maximum speed in RPM.
  float axis_length_mm;     ///< Axis length in millimeters.
  float wheel_diameter_mm;  ///< Wheel diameter in millimeters.
  int pulses_per_rev;       ///< Number of pulses per revolution.
};
