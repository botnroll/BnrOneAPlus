#pragma once

#include <BnrOneAPlus.h>

#include "MotionGenerator.h"
#include "RobotParams.h"

/**
 * @class ShapeGenerator
 * @brief Class that wraps the MotionGenerator module and offers predefined
 * shapes.
 */
class ShapeGenerator {
 public:
  /**
   * @brief Constructor for ShapeGenerator.
   * @param one Reference to BnrOneAPlus object.
   * @param slip_factor Slip factor for the robot.
   */
  ShapeGenerator(BnrOneAPlus& one,
                 const float slip_factor = 1.0,
                 const RobotParams robot_params = RobotParams());

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

  /**
   * @brief Moves the robot for the given distance at the given speed.
   * @param distance Distance to move.
   * @param speed Speed to move at.
   * @param slow_down_distance Distance at which to start slowing down.
   */
  void moveStraightAtSpeed(const float distance,
                           const float speed = 200,
                           const float slow_down_distance = 0) const;

  /**
   * @brief Generates a spot rotation of 90 degrees counterclockwise.
   * @param speed Speed to rotate at.
   * @param slow_down_thresh_deg Angle at which to start slowing down.
   */
  void rotate90DegCcw(const float speed,
                      const float slow_down_thresh_deg = 30) const;

  /**
   * @brief Describes a polygon shaped motion given the side length and the
   * number of sides.
   * @param side_mm Length of each side in millimeters.
   * @param num_sides Number of sides of the polygon.
   * @param speed Speed to move at.
   */
  void polygon(const float side_mm,
               const int num_sides,
               const float speed = 200) const;

  /**
   * @brief Describes a rounded polygon shaped motion given the side length and
   * the number of sides.
   * @param side_mm Length of each side in millimeters.
   * @param num_sides Number of sides of the polygon.
   * @param speed Speed to move at.
   */
  void roundedPolygon(const float side_mm,
                      const int num_sides,
                      const float speed = 200) const;

  /**
   * @brief Moves by describing a triangular motion with the given side length.
   * @param side_mm Length of each side in millimeters.
   * @param speed Speed to move at.
   */
  void triangle(const float side_mm, const float speed = 200) const;

  /**
   * @brief Describes a square motion with the given side length.
   * @param side_mm Length of each side in millimeters.
   * @param speed Speed to move at.
   */
  void square(const float side_mm, const float speed = 150) const;

  /**
   * @brief Describes a circular motion with the given radius.
   * @param radius_mm Radius of the circle in millimeters.
   * @param speed Speed to move at.
   */
  void circle(const float radius_mm, const float speed = 200) const;

  /**
   * @brief Describes a semi-circular motion with the given radius.
   * @param radius_mm Radius of the semi-circle in millimeters.
   * @param speed Speed to move at.
   */
  void semiCircle(const float radius_mm, const float speed = 200) const;

  /**
   * @brief Describes a Fibonacci spiral motion.
   * @param seed_radius Initial radius of the spiral.
   * @param num_segments Number of segments of 90 degrees of the spiral.
   * @param speed Speed to move at.
   */
  void fibonacciSpiral(const float seed_radius,
                       const int num_segments,
                       const float speed = 200) const;

  /**
   * @brief Describes an Archimedean spiral motion.
   * @param spiral_factor Constant factor for the spiral.
   * @param total_angle_deg Total angle of the spiral in degrees.
   * @param speed Speed to move at.
   */
  void archimedeanSpiral(const float spiral_factor,
                         const float total_angle_deg,
                         const float speed = 200) const;

  /**
   * @brief Describes an ondulatory motion (like a snake).
   * @param length_mm Length of the motion in millimeters.
   * @param num_elements Number of ondulatory elements.
   * @param speed Speed to move at.
   * @param snaking_angle_deg Snaking angle in degrees.
   * @param turning_rate_deg Turning rate in degrees.
   */
  void snake(const float length_mm = 700,
             const int num_elements = 7,
             const float speed = 50,
             const float snaking_angle_deg = 60,
             const float turning_rate_deg = 0) const;

  /**
   * @brief Example on how to set a motion with the shape of a heart.
   */
  void heart() const;

 private:
  /**
   * @brief Computes a Fibonacci sequence with a predetermined number of
   * elements.
   * @param number_of_elements Number of elements in the Fibonacci sequence.
   * @param fibonacci_sequence Array to store the Fibonacci sequence.
   */
  void computeFibonacciSequence(const int number_of_elements,
                                int* fibonacci_sequence) const;

  MotionGenerator mg_;  ///< MotionGenerator object.
};
