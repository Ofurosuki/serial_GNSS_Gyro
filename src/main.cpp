#include <Gyro.hpp>
#include <boost/asio.hpp>
#include <chrono>
#include <iostream>
// #include <main.hpp>
#include <GNSS.hpp>
#include <Solution2Motors.hpp>
#include <thread>
// std::mutex Gyro::mtx;

int main() {
  double pitch, roll, yaw = 0;
  double lattitude[2], longitude[2], altitude[2] = {0};

  Gyro gyro("/dev/ttyUSB0", roll, pitch, yaw);  // register variables
  GNSS gnss("/dev/ttyACM0", lattitude[0], longitude[0], altitude[0]);
  GNSS gnss2("/dev/ttyACM1", lattitude[1], longitude[1], altitude[1]);
  Solution2Motors solution(lattitude[0], longitude[0], altitude[0],
                           lattitude[1], longitude[1], altitude[1], yaw, pitch,
                           roll);

  // std::thread th2([&lattitude, &longitude, &altitude]() {
  //   while (true) {
  //     std::cout << "[Main]" << "Lattitude: " << lattitude[0]
  //               << ", Longitude: " << longitude[0]
  //               << ", Altitude: " << altitude[0] << std::endl;
  //   }
  // });
  // th2.join();

  // std::thread th([&pitch, &roll, &yaw]() {
  //   while (true) {
  //     std::lock_guard<std::mutex> lock(Gyro::mtx);
  //     std::cout << "[Main]" << "Pitch: " << pitch << ", Roll: " << roll
  //               << ", Yaw: " << yaw << std::endl;
  //   }
  // });
  // th.join();

  // std::thread th3([&solution]() {
  //   while (true) {
  //     solution.caliculate_angle();
  //     std::cout << "[Main]"
  //               << "Horizontal Rotation: " <<
  //               solution.get_horizontal_rotation()
  //               << ", Vertical Rotation: " <<
  //               solution.get_vertical_rotation()
  //               << std::endl;
  //   }
  // });
  // th3.join();

  gyro.join();
  gnss.join();
  solution.join();

  return 0;
}