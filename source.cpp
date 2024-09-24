#define _WIN32_WINNT 0x0A00
#define PI 3.141592653589
#define PI180 0.01745329252 // = PI/180
#define eqradius 6378137 // earth equatorial radius in meters
#define polradius 6356752 // earth polar radius in meters

#include <iostream>
#include <string>
#include <boost/asio.hpp>
#include <thread>
#include <iomanip>
#include <cmath>

bool is_GNSS_1_valid = false;
bool is_GNSS_2_valid = false;
double horizontal_rotation, vertical_rotation = 0.0;

void GNSS_1(boost::asio::serial_port& port_1, double& latitude, double& longitude, double& altitude);
void GNSS_2(boost::asio::serial_port& port_2, double& latitude, double& longitude, double& altitude);
void Gyro(boost::asio::serial_port& port, double& roll, double& pitch, double& yaw);
void loop(double& pitch, double& roll, double& yaw, double& latitude1, double& longitude1, double& altitude1, double& latitude2, double& longitude2, double& altitude2);
void solution_angle(double& pitch, double& roll, double& heading, double& latitude1, double& longitude1, double& altitude1, double& latitude2, double& longitude2, double& altitude2, double& horizontal_rotation, double& vertical_rotation);
int main() {
    double pitch, roll, yaw;
    double longitude1, latitude1, altitude1;
    double longitude2, latitude2, altitude2;
   

    boost::asio::io_service io;
    boost::asio::serial_port port(io);
    boost::asio::serial_port port_1(io);
    boost::asio::serial_port port_2(io);

    // GNSS_1
    port_1.open("COM4");
    port_1.set_option(boost::asio::serial_port_base::baud_rate(115200));

    // GNSS_2
    port_2.open("COM5");
    port_2.set_option(boost::asio::serial_port_base::baud_rate(115200));

    // Gyro
    port.open("COM3");
    port.set_option(boost::asio::serial_port_base::baud_rate(115200));    

    std::thread th(Gyro, std::ref(port), std::ref(roll), std::ref(pitch), std::ref(yaw));
    std::thread th_1(GNSS_1, std::ref(port_1), std::ref(latitude1), std::ref(longitude1), std::ref(altitude1));
    std::thread th_2(GNSS_2, std::ref(port_2), std::ref(latitude2), std::ref(longitude2), std::ref(altitude2));
    std::thread th_3(loop, std::ref(pitch), std::ref(roll), std::ref(yaw), std::ref(latitude1), std::ref(longitude1), std::ref(altitude1), std::ref(latitude2), std::ref(longitude2), std::ref(altitude2));
    std::thread th_4(solution_angle, std::ref(pitch), std::ref(roll), std::ref(yaw), std::ref(latitude1), std::ref(longitude1), std::ref(altitude1), std::ref(latitude2), std::ref(longitude2), std::ref(altitude2),std::ref(horizontal_rotation), std::ref(vertical_rotation));
    std::this_thread::sleep_for(std::chrono::milliseconds(1000));   

    th.join();
    th_1.join();
    th_2.join();
    th_3.join();

    return 0;
}
void solution_angle(double& pitch, double& roll, double& heading, double& latitude1, double& longitude1, double& altitude1, double& latitude2, double& longitude2, double& altitude2,double& horizontal_rotation,double& vertical_rotation) {
    double height;
    double xdiff;
    double ydiff;
    double altdiff;
    double raddist1, latdist1;
    double raddist2, latdist2;
    double latdiff_distance;
    double longdiff_distance;
    double raddist1x, raddist1y;
    double raddist2x, raddist2y;
    double sina, cosa;
    double sinb, cosb;
    double sing, cosg;
    double m11, m12, m13, m21, m22, m23, m31, m32, m33;
    double rotx, roty, rotz;


    while (true) {
        //std::cout << "Pitch: " << pitch << " Roll: " << roll << " Heading: " << heading << std::endl;
        //std::cout << "N_1: " << std::setprecision(15) << latitude1 << " E_1: " << longitude1 << " geoid_1: " << altitude1 << std::endl;
        //std::cout << "N_2: " << latitude2 << " E_2: " << longitude2 << " geoid_2: " << altitude2 << std::endl;

        altdiff = altitude2 - altitude1;
        raddist1 = sqrt(1.0 / (1.0 / eqradius / eqradius + tan(latitude1 * PI180) * tan(latitude1 * PI180) / polradius / polradius));
        latdist1 = raddist1 * tan(latitude1 * PI180);
        raddist2 = sqrt(1.0 / (1.0 / eqradius / eqradius + tan(latitude2 * PI180) * tan(latitude2 * PI180) / polradius / polradius));
        latdist2 = raddist2 * tan(latitude2 * PI180);
        latdiff_distance = sqrt((raddist1 - raddist2) * (raddist1 - raddist2) + (latdist1 - latdist2) * (latdist1 - latdist2));
        if (latitude2 >= latitude1) {
            latdiff_distance = abs(latdiff_distance);
        }
        else {
            latdiff_distance = -abs(latdiff_distance);
        }
        //std::cout << "altdiff: " << std::setprecision(15) << altdiff << std::endl;
        //std::cout << "raddist1: " << std::setprecision(15) << raddist1 << " latdist1: " << latdist1 << std::endl;
        //std::cout << "raddist2: " << std::setprecision(15) << raddist2 << " latdist2: " << latdist2 << std::endl;
        //std::cout << "latdiff_distance: " << std::setprecision(15) << latdiff_distance << std::endl;
        raddist1x = raddist1 * cos(longitude1 * PI180);
        raddist1y = raddist1 * sin(longitude1 * PI180);
        raddist2x = raddist2 * cos(longitude2 * PI180);
        raddist2y = raddist2 * sin(longitude2 * PI180);
        longdiff_distance = sqrt((raddist1x - raddist2x) * (raddist1x - raddist2x) + (raddist1y - raddist2y) * (raddist1y - raddist2y));
        if (longitude2 >= longitude1) {
            longdiff_distance = abs(longdiff_distance);
        }
        else {
            longdiff_distance = -abs(longdiff_distance);
        }
        //std::cout << "raddist1x: " << std::setprecision(15) << raddist1x << " raddist1y: " << raddist1y << std::endl;
        //std::cout << "raddist2x: " << std::setprecision(15) << raddist2x << " raddist2y: " << raddist2y << std::endl;
        //std::cout << "longdiff_distance: " << std::setprecision(15) << longdiff_distance << std::endl;
        //std::cout << std::endl;

        cosa = cos(heading * PI180);
        sina = sin(heading * PI180);
        cosb = cos(pitch * PI180);
        sinb = sin(pitch * PI180);
        cosg = cos(roll * PI180);
        sing = sin(roll * PI180);
        m11 = cosa * cosb;
        m12 = cosa * sinb * sing - sina * cosg;
        m13 = cosa * sinb * cosg + sina * sing;
        m21 = sina * cosb;
        m22 = sina * sinb * sing + cosa * cosg;
        m23 = sina * sinb * cosg - cosa * sing;
        m31 = -sinb;
        m32 = cosb * sing;
        m33 = cosb * cosg;
        //std::cout << "m11 m12 m13: " << std::setprecision(15) << m11 << " " << m12 << " " << m13 << std::endl;
        //std::cout << "m21 m22 m23: " << std::setprecision(15) << m21 << " " << m22 << " " << m23 << std::endl;
        //std::cout << "m31 m32 m33: " << std::setprecision(15) << m31 << " " << m32 << " " << m33 << std::endl;
        //std::cout << std::endl;

        rotx = m11 * longdiff_distance + m12 * latdiff_distance + m13 * altdiff;
        roty = m21 * longdiff_distance + m22 * latdiff_distance + m23 * altdiff;;
        rotz = m31 * longdiff_distance + m32 * latdiff_distance + m33 * altdiff;;
        //std::cout << "longdiff_distance latdiff_distance altdiff: " << std::setprecision(15) << longdiff_distance << " " << latdiff_distance << " " << altdiff << std::endl;
        //std::cout << "rotx roty rotz: " << std::setprecision(15) << rotx << " " << roty << " " << rotz << std::endl;
        //std::cout << std::endl;

        horizontal_rotation = atan(roty / rotx) / PI180;
        vertical_rotation = atan(rotz / sqrt(rotx * rotx + roty * roty)) / PI180;
        // std::cout << "solution_angle: horizontal_rotation: " << std::setprecision(15) << horizontal_rotation << std::endl;
        // std::cout << "solution_angle: vertical_rotation: " << std::setprecision(15) << vertical_rotation << std::endl;
        std::this_thread::sleep_for(std::chrono::milliseconds(10));
    }

}
void loop(double& pitch, double& roll, double& heading, double& latitude1, double& longitude1, double& altitude1, double& latitude2, double& longitude2, double& altitude2) {
    double height;
    double xdiff;
    double ydiff;
    double altdiff;
    double raddist1, latdist1;
    double raddist2, latdist2;
    double latdiff_distance;
    double longdiff_distance;
    double raddist1x, raddist1y;
    double raddist2x, raddist2y;
    double sina, cosa;
    double sinb, cosb;
    double sing, cosg;
    double m11, m12, m13, m21, m22, m23, m31, m32, m33;
    double rotx, roty, rotz;

    while (true) {
        //std::cout << "Pitch: " << pitch << " Roll: " << roll << " Heading: " << heading << std::endl;
       
        std::cout << "=====================================================" << std::endl;
        std::cout << "loop: horizontal_rotation: " << std::setprecision(15) << horizontal_rotation << std::endl;
        std::cout << "loop: vertical_rotation: " << std::setprecision(15) << vertical_rotation << std::endl;
        std::cout << "=====================================================" << std::endl;

        //if (is_GNSS_1_valid) {
        //    std::cout << "[GNSS1] N: " << latitude1 << " E: " << longitude1 << " geoid: " << altitude1 << std::endl;
        //}
        //else {
        //    std::cout << "[GNSS1] unavailable" << std::endl;
        //}

        //if (is_GNSS_2_valid) {
        //    std::cout << "[GNSS2] N: " << latitude2 << " E: " << longitude2 << " geoid: " << altitude2 << std::endl;
        //}
        //else {
        //    std::cout << "[GNSS2] unavailable" << std::endl;
        //}
        
        std::this_thread::sleep_for(std::chrono::milliseconds(1000));
    }
}

void Gyro(boost::asio::serial_port& port, double& roll, double& pitch, double& yaw) {
    while (true) {
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
            try {
                roll = std::stod(parsed_data[1]);
                pitch = std::stod(parsed_data[2]);
                yaw = std::stod(parsed_data[3]);
            }
            catch (const std::invalid_argument& e)
            {
                std::cout << "Invalid argument: " << e.what() << '\n';
            }
            //std::cout << " Pitch: " << pitch << ", Roll: " << roll << ", Heading: " << yaw << std::endl;
        }
    }
}

void GNSS_1(boost::asio::serial_port& port_1, double& latitude, double& longitude, double& altitude) {
    latitude = 0;
    longitude = 0;
    altitude = 0;
    while (true) {
        int count_1;
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
            double latitude_deg, longitude_deg = 0.0;
            double latitude_min, longitude_min = 0.0;
            double latitude_submin, longitude_submin = 0.0;

            try {
                latitude_deg = std::stod(parsed_data_1[2].substr(0, 2));
                latitude_min = std::stod(parsed_data_1[2].substr(2, 2));
                latitude_submin = std::stod(parsed_data_1[2].substr(5, 4));

                longitude_deg = std::stod(parsed_data_1[4].substr(0, 3));
                longitude_min = std::stod(parsed_data_1[4].substr(3, 2));
                longitude_submin = std::stod(parsed_data_1[4].substr(6, 4));

                latitude = latitude_deg + latitude_min / 60.0 + latitude_submin / 600000.0;
                longitude = longitude_deg + longitude_min / 60.0 + longitude_submin / 600000.0;

                altitude = std::stod(parsed_data_1[11]);

               //std::cout << "N_1: " << latitude << " E_1: " << longitude << " geoid_1: " << altitude << std::endl;
               is_GNSS_1_valid= true;
            }
            catch (const std::invalid_argument& e) {
                std::cout << "[GNSS_1] Invalid argument or not have received yet: " << e.what() << '\n';
               is_GNSS_1_valid=false;
                
            }
        }
        else {
            std::cout << "[GNSS_1] Invalid argument or not have received yet. " << '\n';
            is_GNSS_1_valid = false;
        }
    }
}

void GNSS_2(boost::asio::serial_port& port_1, double& latitude, double& longitude, double& altitude) {
    latitude = 0;
    longitude = 0;
    altitude = 0;
    while (true) {
        int count_1;
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
            double latitude_deg, longitude_deg = 0.0;
            double latitude_min, longitude_min = 0.0;
            double latitude_submin, longitude_submin = 0.0;

            try {
                latitude_deg = std::stod(parsed_data_1[2].substr(0, 2));
                latitude_min = std::stod(parsed_data_1[2].substr(2, 2));
                latitude_submin = std::stod(parsed_data_1[2].substr(5, 4));

                longitude_deg = std::stod(parsed_data_1[4].substr(0, 3));
                longitude_min = std::stod(parsed_data_1[4].substr(3, 2));
                longitude_submin = std::stod(parsed_data_1[4].substr(6, 4));

                latitude = latitude_deg + latitude_min / 60.0 + latitude_submin / 600000.0;
                longitude = longitude_deg + longitude_min / 60.0 + longitude_submin / 600000.0;

                altitude = std::stod(parsed_data_1[11]);

              // std::cout << "N_2: " << latitude << " E_2: " << longitude << " geoid_2: " << altitude << std::endl;
               is_GNSS_2_valid = true;
            }
            catch (const std::invalid_argument& e) {
                std::cout << "Invalid argument: " << e.what() << '\n';
                is_GNSS_2_valid = false;
            }
        }
        else {
            // std::cout << "[GNSS_2] Invalid argument or not have received yet. " << '\n';
            is_GNSS_2_valid = false;
        }
    }
}