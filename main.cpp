#include "GNSS.hpp"
#include <boost/asio.hpp>

int main(){
    double pitch, roll, yaw=0;
    GNSS gnss1("/dev/ttyUSB0",pitch,roll,yaw);
}