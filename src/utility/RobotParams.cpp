#include "RobotParams.h"

/**
 * @brief Constructor for RobotParams.
 * @param max_speed_rpm_in Maximum speed in RPM.
 * @param axis_length_mm_in Axis length in millimeters.
 * @param wheel_diameter_mm_in Wheel diameter in millimeters.
 * @param pulses_per_rev_in Number of pulses per revolution.
 */
RobotParams::RobotParams(const int max_speed_rpm_in,
                         const float axis_length_mm_in,
                         const float wheel_diameter_mm_in,
                         const int pulses_per_rev_in)
    : max_speed_rpm(max_speed_rpm_in),
      axis_length_mm(axis_length_mm_in),
      wheel_diameter_mm(wheel_diameter_mm_in),
      pulses_per_rev(pulses_per_rev_in) {}
