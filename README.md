# serial_GNSS_Gyro
serial communication for GNSS and Gyro.

## install 
if you have not installed boost library yet, visit https://boostjp.github.io/howtobuild.html and follow the instrunctions.

## build & run
execute
```
g++ -I /path/to/your/boost/dir/boost_1_84_0/include/ main.cpp -o Main
```
and then
```
sudo ./Main
```
Make sure that you run the program as admin.

## usage
### set port 
in the main function, change port name for your environment.
```cpp
    // GNSS_0
    port_1.open("/dev/ttyACM0");//here
    port_1.set_option(boost::asio::serial_port_base::baud_rate(115200));
    int count_1 = 0;

    // GNSS_1
    port_2.open("/dev/ttyUSB1");//here 
    port_2.set_option(boost::asio::serial_port_base::baud_rate(115200));


    // Gyro
    port.open("/dev/ttyUSB0");//here
    port.set_option(boost::asio::serial_port_base::baud_rate(115200));
```
### sample
In the loop function you can see all variables(altitude,latitude,longitude,roll,pitch,yaw) can be used.
