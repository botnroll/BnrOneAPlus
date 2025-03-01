#include "ShapeGenerator.h"

#include <Arduino.h>  // Include Arduino library for Serial

ShapeGenerator::ShapeGenerator(BnrOneAPlus& one, const float slip_factor)
    : mg_(one, slip_factor) {}

void ShapeGenerator::rotateAngleDegAtSpeed(
    const float angle_deg,
    const float speed,
    const float radius_of_curvature_mm,
    const float slow_down_thresh_deg) const {
  mg_.rotateAngleDegAtSpeed(
      angle_deg, speed, radius_of_curvature_mm, slow_down_thresh_deg);
}

void ShapeGenerator::moveStraightAtSpeed(const float distance,
                                         const float speed,
                                         const float slow_down_distance) const {
  mg_.moveStraightAtSpeed(distance, speed, slow_down_distance);
}

void ShapeGenerator::rotate90DegCcw(const float speed,
                                    const float slow_down_thresh_deg) const {
  mg_.rotateAngleDegAtSpeed(90, speed, 0, slow_down_thresh_deg);
}

void ShapeGenerator::polygon(const float side_mm,
                             const int num_sides,
                             const float speed) const {
  float angle_deg = 180 - ((num_sides - 2) * 180.0) / num_sides;
  Serial.print("angle_deg: ");
  Serial.println(angle_deg);
  for (int i = 0; i < num_sides; ++i) {
    mg_.moveStraightAtSpeed(side_mm, speed);
    mg_.rotateAngleDegAtSpeed(angle_deg, speed);
  }
}

void ShapeGenerator::roundedPolygon(const float side_mm,
                                    const int num_sides,
                                    const float speed) const {
  float angle_deg = 180 - ((num_sides - 2) * 180.0) / num_sides;
  Serial.print("angle_deg: ");
  Serial.println(angle_deg);
  for (int i = 0; i < num_sides; ++i) {
    mg_.moveStraightAtSpeed(side_mm, speed);
    mg_.rotateAngleDegAtSpeed(90, speed, 80, 0);
  }
}

void ShapeGenerator::triangle(const float side_mm, const float speed) const {
  polygon(side_mm, 3, speed);
}

void ShapeGenerator::square(const float side_mm, const float speed) const {
  polygon(side_mm, 4, speed);
}

void ShapeGenerator::circle(const float radius_mm, const float speed) const {
  mg_.rotateAngleDegAtSpeed(360, speed, radius_mm);
}

void ShapeGenerator::semiCircle(const float radius_mm,
                                const float speed) const {
  mg_.rotateAngleDegAtSpeed(180, speed, radius_mm);
}

void ShapeGenerator::fibonacciSpiral(const float seed_radius,
                                     const int num_segments,
                                     const float speed) const {
  int fibonacci_sequence[num_segments];
  computeFibonacciSequence(num_segments, fibonacci_sequence);
  for (int i = 0; i < num_segments; ++i) {
    float radius_of_curvature = fibonacci_sequence[i] * seed_radius;
    mg_.rotateAngleDegAtSpeed(90, speed, radius_of_curvature);
  }
}

void ShapeGenerator::archimedeanSpiral(const float spiral_factor,
                                       const float total_angle_deg,
                                       const float speed) const {
  float angle_step = 5;
  float current_angle = 0;
  for (int i = 0; i < total_angle_deg; i += angle_step) {
    float radius_of_curvature_mm = spiral_factor * current_angle;
    mg_.rotateAngleDegAtSpeed(angle_step, speed, radius_of_curvature_mm);
    current_angle += angle_step;
  }
}

void ShapeGenerator::snake(const float length_mm,
                           const int num_elements,
                           const float speed,
                           const float snaking_angle_deg,
                           const float turning_rate_deg) const {
  float secant_length = length_mm / num_elements;
  float theta_rad = radians(snaking_angle_deg);
  float radius_of_curvature_mm = secant_length / (2 * sin(theta_rad / 2));
  mg_.rotateAngleDegAtSpeed(-snaking_angle_deg / 2, speed);
  for (int i = 0; i < num_elements; ++i) {
    mg_.rotateAngleDegAtSpeed(
        snaking_angle_deg + turning_rate_deg, speed, radius_of_curvature_mm);
    snaking_angle_deg = -snaking_angle_deg;
  }
}

void ShapeGenerator::heart() const {
  float speed = 200;
  mg_.rotateAngleDegAtSpeed(45, speed);
  mg_.moveStraightAtSpeed(200, speed);
  mg_.rotateAngleDegAtSpeed(230, speed, 100);
  mg_.rotateAngleDegAtSpeed(-180, speed);
  mg_.rotateAngleDegAtSpeed(230, speed, 100);
  mg_.moveStraightAtSpeed(230, speed);
}

void ShapeGenerator::computeFibonacciSequence(const int number_of_elements,
                                              int* fibonacci_sequence) const {
  fibonacci_sequence[0] = 1;
  fibonacci_sequence[1] = 1;
  for (int i = 2; i < number_of_elements; ++i) {
    fibonacci_sequence[i] =
        fibonacci_sequence[i - 1] + fibonacci_sequence[i - 2];
  }
}
