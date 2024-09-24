#include <boost/asio.hpp>
class GNSS {
 public:
  GNSS(std::string portname, double& roll, double& pitch, double& yaw);
  void read_data();
  void set_baud_rate(int baud_rate);
  void set_timer_interval(int timer_interval);

 private:
  double* roll;
  double* pitch;
  double* yaw;

  boost::asio::io_service io;
  int baud_rate = 115200;
  int timer_interval = 1000;  // ms
};