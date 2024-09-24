#include <GNSS.hpp>
#include <boost/asio.hpp>
#include <iostream>
#include <string>
#include <thread>

GNSS::GNSS(std::string portname, double& roll, double& pitch, double& yaw) {
  this->roll = &roll;
  this->pitch = &pitch;
  this->yaw = &yaw;

  boost::asio::serial_port port(io);
  port.open(portname);
  port.set_option(boost::asio::serial_port_base::baud_rate(baud_rate));

  auto alive = std::atomic_bool(true);
  auto th = std::thread([&alive] {
    const std::chrono::milliseconds interval(timer_interval);
    while (alive.load()) {
      auto start = std::chrono::system_clock::now();
      this.read_data();
      auto end = std::chrono::system_clock::now();
      auto waste = end - start;
      if (waste < interval) {
        std::this_thread::sleep_for(interval - waste);
      }
    }
  });
  th.join();

  // alive.store(false); to stop the thread
}

void GNSS::read_data() {
  boost::asio::streambuf buffer;
  boost::asio::read_until(port, buffer, '\n');

  std::istream is(&buffer);
  std::string line;
  std::getline(is, line);

  // data is comma-separated
  std::istringstream ss(line);
  std::string token;
  int count = 0;
  std::string parsed_data[7];
  while (std::getline(ss, token, ',')) {
    parsed_data[count++] = token;
  }

  if (count == 7) {
    try {
      *roll = std::stod(parsed_data[1]);
      *pitch = std::stod(parsed_data[2]);
      *yaw = std::stod(parsed_data[3]);
    } catch (const std::invalid_argument& e) {
      std::cout << "Invalid argument: " << e.what() << '\n';
    }
    // std::cout << " Pitch: " << pitch << ", Roll: " << roll << ", Heading: "
    // << yaw << std::endl;
  }
}

void GNSS::set_baud_rate(int baud_rate) { this->baud_rate = baud_rate; }

void GNSS::set_timer_interval(int timer_interval) {
  this->timer_interval = timer_interval;
}