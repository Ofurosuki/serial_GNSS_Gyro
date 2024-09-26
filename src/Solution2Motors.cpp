#ifdef _WIN32
#define _WIN32_WINNT 0x0A00
#endif
#define PI 3.141592653589
#define PI180 0.01745329252  // = PI/180
#define eqradius 6378137     // earth equatorial radius in meters
#define polradius 6356752    // earth polar radius in meters

#include <Solution2Motors.hpp>
#include <boost/asio.hpp>
#include <cmath>
#include <iomanip>
#include <iostream>
#include <thread>

Solution2Motors::Solution2Motors(double& latitude1, double& longitude1,
                                 double& altitude1, double& latitude2,
                                 double& longitude2, double& altitude2,
                                 double& head, double& pitch, double& roll) {
  this->latitude1 = latitude1;
  this->longitude1 = longitude1;
  this->altitude1 = altitude1;
  this->latitude2 = latitude2;
  this->longitude2 = longitude2;
  this->altitude2 = altitude2;
  this->heading = head;
  this->pitch = pitch;
  this->roll = roll;
}

void Solution2Motors::caliculate_angle() {
  altdiff = altitude2 - altitude1;
  raddist1 = sqrt(1.0 / (1.0 / eqradius / eqradius +
                         tan(latitude1 * PI180) * tan(latitude1 * PI180) /
                             polradius / polradius));
  latdist1 = raddist1 * tan(latitude1 * PI180);
  raddist2 = sqrt(1.0 / (1.0 / eqradius / eqradius +
                         tan(latitude2 * PI180) * tan(latitude2 * PI180) /
                             polradius / polradius));
  latdist2 = raddist2 * tan(latitude2 * PI180);
  latdiff_distance = sqrt((raddist1 - raddist2) * (raddist1 - raddist2) +
                          (latdist1 - latdist2) * (latdist1 - latdist2));
  if (latitude2 >= latitude1) {
    latdiff_distance = abs(latdiff_distance);
  } else {
    latdiff_distance = -abs(latdiff_distance);
  }
  // std::cout << "altdiff: " << std::setprecision(15) << altdiff << std::endl;
  // std::cout << "raddist1: " << std::setprecision(15) << raddist1 << "
  // latdist1: " << latdist1 << std::endl; std::cout << "raddist2: " <<
  // std::setprecision(15) << raddist2 << " latdist2: " << latdist2 <<
  // std::endl; std::cout << "latdiff_distance: " << std::setprecision(15) <<
  // latdiff_distance << std::endl;
  raddist1x = raddist1 * cos(longitude1 * PI180);
  raddist1y = raddist1 * sin(longitude1 * PI180);
  raddist2x = raddist2 * cos(longitude2 * PI180);
  raddist2y = raddist2 * sin(longitude2 * PI180);
  longdiff_distance = sqrt((raddist1x - raddist2x) * (raddist1x - raddist2x) +
                           (raddist1y - raddist2y) * (raddist1y - raddist2y));
  if (longitude2 >= longitude1) {
    longdiff_distance = abs(longdiff_distance);
  } else {
    longdiff_distance = -abs(longdiff_distance);
  }
  // std::cout << "raddist1x: " << std::setprecision(15) << raddist1x << "
  // raddist1y: " << raddist1y << std::endl; std::cout << "raddist2x: " <<
  // std::setprecision(15) << raddist2x << " raddist2y: " << raddist2y <<
  // std::endl; std::cout << "longdiff_distance: " << std::setprecision(15) <<
  // longdiff_distance << std::endl; std::cout << std::endl;

  cosa = cos(heading * PI180);
  sina = sin(heading * PI180);
  cosb = cos(pitch * PI180);
  sinb = sin(pitch * PI180);
  cosg = cos(roll * PI180);
  sing = sin(roll * PI180);
  m11 = cosa * cosb;
  m12 = cosa * sinb * sing - sina * cosg;
  m13 = cosa * sinb * cosg + sina * sing;
  m21 = sina * cosb;
  m22 = sina * sinb * sing + cosa * cosg;
  m23 = sina * sinb * cosg - cosa * sing;
  m31 = -sinb;
  m32 = cosb * sing;
  m33 = cosb * cosg;
  // std::cout << "m11 m12 m13: " << std::setprecision(15) << m11 << " " << m12
  // << " " << m13 << std::endl; std::cout << "m21 m22 m23: " <<
  // std::setprecision(15) << m21 << " " << m22 << " " << m23 << std::endl;
  // std::cout << "m31 m32 m33: " << std::setprecision(15) << m31 << " " << m32
  // << " " << m33 << std::endl; std::cout << std::endl;

  rotx = m11 * longdiff_distance + m12 * latdiff_distance + m13 * altdiff;
  roty = m21 * longdiff_distance + m22 * latdiff_distance + m23 * altdiff;
  ;
  rotz = m31 * longdiff_distance + m32 * latdiff_distance + m33 * altdiff;
  ;
  // std::cout << "longdiff_distance latdiff_distance altdiff: " <<
  // std::setprecision(15) << longdiff_distance << " " << latdiff_distance << "
  // " << altdiff << std::endl; std::cout << "rotx roty rotz: " <<
  // std::setprecision(15) << rotx << " " << roty << " " << rotz << std::endl;
  // std::cout << std::endl;

  horizontal_rotation = atan(roty / rotx) / PI180;
  vertical_rotation = atan(rotz / sqrt(rotx * rotx + roty * roty)) / PI180;
}
void Solution2Motors::join() {
  th = std::thread([this]() {
    const std::chrono::milliseconds interval(timer_interval);
    while (true) {
      auto start = std::chrono::system_clock::now();
      caliculate_angle();
      auto end = std::chrono::system_clock::now();
      auto waste = end - start;
      if (waste < interval) {
        std::this_thread::sleep_for(interval - waste);
      }
    }
  });
  th.join();
}
double Solution2Motors::get_horizontal_rotation() {
  return horizontal_rotation;
}
double Solution2Motors::get_vertical_rotation() { return vertical_rotation; }