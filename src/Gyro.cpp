#include <Gyro.hpp>
#include <boost/asio.hpp>
#include <chrono>
#include <iostream>
// #include <main.hpp>
#include <mutex>
#include <string>
#include <thread>

Gyro::Gyro(std::string portname, double& roll, double& pitch, double& yaw)
    : port(io) {
  this->roll = &roll;
  this->pitch = &pitch;
  this->yaw = &yaw;

  port.open(portname);
  port.set_option(boost::asio::serial_port_base::baud_rate(baud_rate));
}

void Gyro::read_data() {
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
      std::cout << "Invalid argument: " << e.what() << std::endl;
    }
    // std::lock_guard<std::mutex> lock(mtx);
    // // std::cout << "executed" << std::endl;
    // std::cout << "[Gyro]" << " Pitch: " << *pitch << ", Roll: " << *roll
    //           << ", Heading: " << *yaw << std::endl;
  }
}

void Gyro::join() {
  th = std::thread([this]() {
    const std::chrono::milliseconds interval(timer_interval);
    while (true) {
      auto start = std::chrono::system_clock::now();
      read_data();
      auto end = std::chrono::system_clock::now();
      auto waste = end - start;
      if (waste < interval) {
        std::this_thread::sleep_for(interval - waste);
      }
    }
  });
  th.join();
}
void Gyro::detach() { th.detach(); }

void Gyro::set_baud_rate(int baud_rate) { this->baud_rate = baud_rate; }

void Gyro::set_timer_interval(int timer_interval) {
  this->timer_interval = timer_interval;
}
int Gyro::get_timer_interval() { return timer_interval; }
bool Gyro::joinable() { return th.joinable(); }