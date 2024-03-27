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
In the function loop, you can see all variables(altitude,latitude,longitude,roll,pitch,yaw) can be used. It prints all variables to terminal every one second.
```cpp
void loop(double &pitch, double &roll, double &yaw,double *latitude, double *longitude, double *altitude){
    while(true){
        std::cout<<"Pitch: "<<pitch<<" Roll: "<<roll<<" Yaw: "<<yaw<<std::endl;
        std::cout<<"N_0: "<<latitude[0]<<" E_0: "<<longitude[0]<<" geoid_0: "<< altitude[0]<<std::endl;
        std::cout<<"N_1: "<<latitude[1]<<" E_1: "<<longitude[1]<<" geoid_1: "<< altitude[1]<<std::endl;
        std::this_thread::sleep_for(std::chrono::milliseconds(1000));
    }


}
```


