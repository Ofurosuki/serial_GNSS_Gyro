#include <iostream>
#include <string>
#include <boost/asio.hpp>
#include <thread>

void GNSS_1(boost::asio::serial_port &port_1, double *latitude, double *longitude, double *altitude);
void GNSS_2(boost::asio::serial_port &port_2, double *latitude, double *longitude, double *altitude);
void Gyro(boost::asio::serial_port &port, double &pitch, double &roll, double &yaw);
void loop(double &pitch, double &roll, double &yaw,double *latitude, double *longitude, double *altitude);

int main() {
    double pitch , roll , yaw;
    double longitude[2],latitude[2],altitude[2];

    boost::asio::io_service io;
    boost::asio::serial_port port(io);
    boost::asio::serial_port port_1(io);
    boost::asio::serial_port port_2(io);


    // GNSS_0
    port_1.open("/dev/ttyACM0");
    port_1.set_option(boost::asio::serial_port_base::baud_rate(115200));
    int count_1 = 0;

    // GNSS_1
    port_2.open("/dev/ttyUSB1");
    port_2.set_option(boost::asio::serial_port_base::baud_rate(115200));


    // Gyro
    port.open("/dev/ttyUSB0");
    port.set_option(boost::asio::serial_port_base::baud_rate(115200));




    std::thread th(Gyro, std::ref(port), std::ref(pitch), std::ref(roll), std::ref(yaw));
    std::thread th_1(GNSS_1, std::ref(port_1), latitude, longitude, altitude);
    std::thread th_2(GNSS_2, std::ref(port_2), latitude, longitude, altitude);
    std::thread th_3(loop, std::ref(pitch), std::ref(roll), std::ref(yaw), latitude, longitude, altitude);
    std::this_thread::sleep_for(std::chrono::milliseconds(1000));
    th.join();
    th_1.join();
    th_2.join();
    th_3.join();

    return 0;
}

void loop(double &pitch, double &roll, double &yaw,double *latitude, double *longitude, double *altitude){
    while(true){
        std::cout<<"Pitch: "<<pitch<<" Roll: "<<roll<<" Yaw: "<<yaw<<std::endl;
        std::cout<<"N_0: "<<latitude[0]<<" E_0: "<<longitude[0]<<" geoid_0: "<< altitude[0]<<std::endl;
        std::cout<<"N_1: "<<latitude[1]<<" E_1: "<<longitude[1]<<" geoid_1: "<< altitude[1]<<std::endl;
        std::this_thread::sleep_for(std::chrono::milliseconds(1000));
    }


}

void Gyro(boost::asio::serial_port &port, double &pitch, double &roll, double &yaw){
    while(true){
        boost::asio::streambuf buffer;
        boost::asio::read_until(port, buffer, '\n');

        std::istream is(&buffer);
        std::string line;
        std::getline(is, line);

        // Assuming data is comma-separated
        std::istringstream ss(line);
        std::string token;
        int count = 0;
        std::string parsed_data[7]; // assuming 7 fields
        while (std::getline(ss, token, ',')) {
            parsed_data[count++] = token;
        }

        if (count == 7) {
            try{
            pitch=std::stod(parsed_data[1]);
            roll=std::stod(parsed_data[2]);
            yaw=std::stod(parsed_data[3]);
            }catch (const std:: invalid_argument& e)
        {
            std::cout << "Invalid argument: " << e.what() << '\n';
        } 
            //std::cout << " Pitch: " << pitch << ", Roll: " << roll << ", Heading: " << yaw << std::endl;
        }
    }
}

void GNSS_1(boost::asio::serial_port &port_1, double *latitude, double *longitude, double *altitude){
    while(true){
        int count_1 ;
        boost::asio::streambuf buffer_1;
        boost::asio::read_until(port_1, buffer_1, '\n');

        std::istream is_1(&buffer_1);
        std::string line_1;
        std::getline(is_1, line_1);

        // Assuming data is comma-separated
        std::istringstream ss_1(line_1);
        std::string token_1;
        count_1 = 0;
        std::string parsed_data_1[50]; // adjust the array size according to your data
        while (std::getline(ss_1, token_1, ',')) {
            parsed_data_1[count_1++] = token_1;
        }

        if (count_1 > 0 && parsed_data_1[0] == "$GNGGA") {
            double latitude_deg, longitude_deg=0.0;
            double latitude_min, longitude_min =0.0;
            double latitude_submin, longitude_submin=0.0;

            try{
            latitude_deg = std::stod(parsed_data_1[2].substr(0, 2));
            latitude_min = std::stod(parsed_data_1[2].substr(2, 2));
            latitude_submin = std::stod(parsed_data_1[2].substr(5, 4));

            longitude_deg = std::stod(parsed_data_1[4].substr(0, 3));
            longitude_min = std::stod(parsed_data_1[4].substr(3, 2));
            longitude_submin = std::stod(parsed_data_1[4].substr(6, 4));

            latitude[0]=latitude_deg+latitude_min/60.0+latitude_submin/600000.0;
            longitude[0]=longitude_deg+longitude_min/60.0+longitude_submin/600000.0;

            altitude[0]=std::stod(parsed_data_1[11]);}
            catch (const std:: invalid_argument& e){
                std::cout << "Invalid argument: " << e.what() << '\n';
            }
            //std::cout<<latitude_deg<<" "<<latitude_min<<" "<<latitude_submin<<std::endl;
            //std::cout<<longitude_deg<<" "<<longitude_min<<" "<<longitude_submin<<std::endl;
            //std::cout<<"N_0: "<<latitude[0]<<" E_0: "<<longitude[0]<<" geoid_0: "<< altitude[0]<<std::endl;
            // for (int i = 0; i < count; ++i) {
            //     std::cout << parsed_data[i] << ", ";
            // }
            // std::cout << std::endl;
        }
    }
}

void GNSS_2(boost::asio::serial_port &port_1, double *latitude, double *longitude, double *altitude){
    while(true){
        int count_1 ;
        boost::asio::streambuf buffer_1;
        boost::asio::read_until(port_1, buffer_1, '\n');

        std::istream is_1(&buffer_1);
        std::string line_1;
        std::getline(is_1, line_1);

        // Assuming data is comma-separated
        std::istringstream ss_1(line_1);
        std::string token_1;
        count_1 = 0;
        std::string parsed_data_1[50]; // adjust the array size according to your data
        while (std::getline(ss_1, token_1, ',')) {
            parsed_data_1[count_1++] = token_1;
        }

        if (count_1 > 0 && parsed_data_1[0] == "$GNGGA") {
            double latitude_deg, longitude_deg=0.0;
            double latitude_min, longitude_min =0.0;
            double latitude_submin, longitude_submin=0.0;

            latitude_deg = std::stod(parsed_data_1[2].substr(0, 2));
            latitude_min = std::stod(parsed_data_1[2].substr(2, 2));
            latitude_submin = std::stod(parsed_data_1[2].substr(5, 4));

            longitude_deg = std::stod(parsed_data_1[4].substr(0, 3));
            longitude_min = std::stod(parsed_data_1[4].substr(3, 2));
            longitude_submin = std::stod(parsed_data_1[4].substr(6, 4));

            latitude[1]=latitude_deg+latitude_min/60.0+latitude_submin/600000.0;
            longitude[1]=longitude_deg+longitude_min/60.0+longitude_submin/600000.0;

            altitude[1]=std::stod(parsed_data_1[11]);
            //std::cout<<latitude_deg<<" "<<latitude_min<<" "<<latitude_submin<<std::endl;
            //std::cout<<longitude_deg<<" "<<longitude_min<<" "<<longitude_submin<<std::endl;
            //std::cout<<"N_1: "<<latitude[1]<<" E_1: "<<longitude[1]<<" geoid_1: "<< altitude[1]<<std::endl;
            // for (int i = 0; i < count; ++i) {
            //     std::cout << parsed_data[i] << ", ";
            // }
            // std::cout << std::endl;
        }
    }
}