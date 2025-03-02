#pragma once

#include <BnrOneAPlus.h>

#include "ControlUtils.h"
#include "RobotParams.h"

/**
 * @class MotionGenerator
 * @brief Class that enables moving in curved or straight lines by specifying
 * the distance and/or angle and speed of the desired motion.
 */
class MotionGenerator {
 public:
  /**
   * @brief Constructor for MotionGenerator.
   * @param one Reference to BnrOneAPlus object.
   * @param slip_factor Slip factor for the robot.
   */
  MotionGenerator(BnrOneAPlus& one,
                  const float slip_factor = 1.0,
                  const RobotParams robot_params = RobotParams());

  /**
   * @brief Moves the robot for the given distance at the given speed.
   * @param distance Distance to move.
   * @param speed Speed to move at.
   * @param slow_down_distance Distance at which to start slowing down.
   */
  void moveStraightAtSpeed(const float distance,
                           const float speed = 50,
                           const float slow_down_distance = 0) const;

  /**
   * @brief Rotates the robot the specified angle at the given speed.
   * @param angle_deg Angle to rotate in degrees.
   * @param speed Speed to rotate at.
   * @param radius_of_curvature_mm Radius of curvature for the rotation.
   * @param slow_down_thresh_deg Angle at which to start slowing down.
   */
  void rotateAngleDegAtSpeed(const float angle_deg,
                             const float speed = 50,
                             const float radius_of_curvature_mm = 0,
                             const float slow_down_thresh_deg = 0) const;

 private:
  /**
   * @brief Resets both encoders.
   */
  void resetEncoders() const;

  /**
   * @brief Computes the angular speed given the linear speed, radius of
   * curvature, and direction.
   * @param speed Linear speed.
   * @param radius_of_curvature_mm Radius of curvature.
   * @param direction Direction of motion.
   * @return Angular speed.
   */
  float computeAngularSpeed(const float speed,
                            const float radius_of_curvature_mm,
                            const int direction = 1) const;

  /**
   * @brief Adjusts the speed if the robot needs to slow down.
   * @param linear_speed Reference to linear speed.
   * @param angular_speed_rad Reference to angular speed.
   * @param speed Current speed.
   * @param pulses_remaining Pulses remaining to complete the motion.
   * @param slow_down_thresh Threshold for slowing down.
   * @param radius_of_curvature_mm Radius of curvature.
   * @param direction Direction of motion.
   */
  void maybeSlowDown(float& linear_speed,
                     float& angular_speed_rad,
                     const float speed,
                     const float pulses_remaining,
                     const float slow_down_thresh,
                     const float radius_of_curvature_mm,
                     const int direction) const;

  /**
   * @brief Moves the robot and slows down when pulses remaining are less than
   * the threshold.
   * @param total_pulses Total pulses required for the motion.
   * @param speed Speed to move at.
   * @param direction Direction of motion.
   * @param radius_of_curvature_mm Radius of curvature.
   * @param slow_down_thresh Threshold for slowing down.
   */
  void moveAndSlowDown(const float total_pulses,
                       const float speed = 50,
                       const int direction = 1,
                       const float radius_of_curvature_mm = 0,
                       const float slow_down_thresh = 4000) const;

  /**
   * @brief Gets the sign of a value.
   * @param value Value to check.
   * @return 1 if value is positive or zero, -1 if value is negative.
   */
  int getSign(const float value) const;

  /**
   * @brief Checks if the wheel speed is within limits.
   * @param speed Speed to check.
   */
  void checkWheelSpeedLimit(const float speed) const;

  /**
   * @brief Checks if the speed limits are within bounds.
   * @param linear_speed Linear speed.
   * @param angular_speed_rad Angular speed.
   */
  void checkSpeedLimits(const float linear_speed,
                        const float angular_speed_rad) const;

  /**
   * @brief Applies the slip factor to a value.
   * @param value Value to apply slip to.
   * @return Value after applying slip.
   */
  float applySlip(const float value) const;

  float slip_factor_;     ///< Slip factor for the robot.
  float axis_length_mm_;  ///< Axis length of the robot in millimeters.
  ControlUtils cut_;      ///< Control utils object
  BnrOneAPlus& one_;      ///< Reference to BnrOneAPlus object.
};
