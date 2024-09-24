#include <boost/asio.hpp>

#include "GNSS.hpp"

int main() {
  double pitch, roll, yaw = 0;
  GNSS gnss1("/dev/ttyUSB0", roll, pitch, yaw);
  while (true) printf("Pitch: %f, Roll: %f, Yaw: %f\n", pitch, roll, yaw);
}