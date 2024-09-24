#include <boost/asio.hpp>
class GNSS {
    public:
    GNSS(std::string portname, double& roll, double& pitch, double& yaw);
    void get_data();
    private:
    boost::asio::serial_port* port;
    double* roll;
    double* pitch;
    double* yaw;

    boost::asio::io_service io;

    
};