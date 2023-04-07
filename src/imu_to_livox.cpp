#include <ros/ros.h>
#include <sensor_msgs/Imu.h>

class ImuToLivox{
    private:
        ros::NodeHandle _nh;
        ros::NodeHandle _nhPrivate;
        ros::Subscriber _sub_pc;
        ros::Publisher _pub_pc;
        std::string _target_frame;

    public:
        ImuToLivox();
        void callbackPC(const sensor_msgs::Imu msgs);
        
};

ImuToLivox::ImuToLivox()
    : _nhPrivate("~")
{
    _nhPrivate.param("target_frame", _target_frame, std::string("base_link"));
    _sub_pc = _nh.subscribe("/imu/data_raw", 1, &ImuToLivox::callbackPC, this);
    _pub_pc = _nh.advertise<sensor_msgs::Imu>("/imu/data_livox", 1);
}

void ImuToLivox::callbackPC(const sensor_msgs::Imu msgs)
{
    sensor_msgs::Imu imu_livox = msgs;

    // reverse linear_acceleration
    imu_livox.linear_acceleration.x *= -1;
    imu_livox.linear_acceleration.y *= -1;
    imu_livox.linear_acceleration.z *= -1;

    // based on gravitational acceleration
    imu_livox.linear_acceleration.x /= 9.80665;
    imu_livox.linear_acceleration.y /= 9.80665;
    imu_livox.linear_acceleration.z /= 9.80665;

    _pub_pc.publish(imu_livox);
}

int main(int argc, char **argv){

    ros::init(argc, argv, "imu_to_livox_node");

    ImuToLivox imu_to_livox;

    ros::spin();

    return 0;

}

