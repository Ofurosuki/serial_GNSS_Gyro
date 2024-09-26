#ifndef GYRO_HPP
#define GYRO_HPP
#include <boost/asio.hpp>
class Gyro {
 public:
  Gyro(std::string portname, double& roll, double& pitch, double& yaw);
  void read_data();
  void set_baud_rate(int baud_rate);
  void set_timer_interval(int timer_interval);
  int get_timer_interval();
  void join();
  void detach();
  bool joinable();

  // static std::mutex mtx;

 private:
  double* roll;
  double* pitch;
  double* yaw;
  std::thread th;
  boost::asio::io_service io;
  boost::asio::serial_port port;
  int baud_rate = 115200;
  int timer_interval = 10;  // ms
};
#endif