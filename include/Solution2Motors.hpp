#ifndef SOLUTION2MOTORS_HPP
#define SOLUTION2MOTORS_HPP

#include <boost/asio.hpp>
#include <cmath>
#include <thread>
class Solution2Motors {
 public:
  Solution2Motors(double& latitude1, double& longitude1, double& altitude1,
                  double& latitude2, double& longitude2, double& altitude2,
                  double& head, double& pitch, double& roll);
  void caliculate_angle();
  void join();
  double get_horizontal_rotation();
  double get_vertical_rotation();

 private:
  double height;
  double xdiff;
  double ydiff;
  double altdiff;
  double raddist1, latdist1;
  double raddist2, latdist2;
  double latdiff_distance;
  double longdiff_distance;
  double raddist1x, raddist1y;
  double raddist2x, raddist2y;
  double sina, cosa;
  double sinb, cosb;
  double sing, cosg;
  double m11, m12, m13, m21, m22, m23, m31, m32, m33;
  double rotx, roty, rotz;

  double latitude1;
  double longitude1;
  double altitude1;
  double latitude2;
  double longitude2;
  double altitude2;
  double heading;
  double pitch;
  double roll;

  double horizontal_rotation;
  double vertical_rotation;

  std::thread th;
  int timer_interval = 10;  // ms
};

#endif