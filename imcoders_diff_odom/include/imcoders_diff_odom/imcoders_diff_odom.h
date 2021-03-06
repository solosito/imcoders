#ifndef IMCODERS_DIFF_ODOM
#define IMCODERS_DIFF_ODOM

#include <ros/ros.h>
#include <tf/transform_broadcaster.h>
#include <nav_msgs/Odometry.h>
#include <sensor_msgs/Imu.h>
#include <message_filters/subscriber.h>
#include <message_filters/sync_policies/approximate_time.h>
#include <message_filters/synchronizer.h>


typedef message_filters::sync_policies::ApproximateTime<sensor_msgs::Imu, sensor_msgs::Imu> ImcodersSyncPolicy;

namespace imcoders_diff_odom
{

class imcodersDiffOdom
{
  public:
    imcodersDiffOdom(ros::NodeHandle& nh, const ros::NodeHandle& private_nh);

    ~imcodersDiffOdom() = default;

    ///
    /// @brief      Inits the ROS variables (publishers, subscribes and params)
    ///
    /// @param      nh    Node for pub/sub
    ///
    /// @return     True if right. False otherwise
    ///
    bool init(ros::NodeHandle& nh);

    ///
    /// @brief      Gets the parameters.
    ///
    /// @param[in]  private_nh  Private node for ROS parameters
    ///
    /// @return     The parameters.
    ///
    bool getParams(const ros::NodeHandle& private_nh);

    ///
    /// @brief      Subscribes to the left and right imcoders sensors in order to compute an odometry
    ///
    /// @param[in]  imcoder_left   The left imcoder sensor
    /// @param[in]  imcoder_right  The right imcoder sensor
    ///
    void imcodersCallback(const sensor_msgs::ImuConstPtr& imcoder_left,
                          const sensor_msgs::ImuConstPtr& imcoder_right);

    ///
    /// @brief      Runs the node
    ///
    void run();

  private:

    ros::NodeHandle& nh_;  // NodeHandle for class, defined outside

    bool debug_mode_;
    bool publish_tf_;
    std::string odom_frame_id_;
    std::string odom_child_frame_id_;
    std::string odom_topic_name_;
    std::string imcoder_left_topic_name_;
    std::string imcoder_right_topic_name_;
    double wheel_radius_;
    double wheel_separation_;
    double dtheta_threshold_;

    ros::Time last_time_;
    double last_x_, last_y_, last_theta_;
    tf::Quaternion last_q_l_, last_q_r_;
    tf::Vector3 last_orientation_imu_;

    double cum_pitch_l_;

    sensor_msgs::Imu imcoder_left_msg_;
    sensor_msgs::Imu imcoder_right_msg_;
    nav_msgs::Odometry odom_msg_;

    ros::Publisher odom_pub_;

    tf::TransformBroadcaster odom_broadcaster_;

    message_filters::Subscriber<sensor_msgs::Imu> *imcoder_left_sub_;
    message_filters::Subscriber<sensor_msgs::Imu> *imcoder_right_sub_;

    message_filters::Synchronizer<ImcodersSyncPolicy>* imcoders_sync_;

};
}  // namespace imcoder_reader
#endif  // IMCODERS_DIFF_ODOM
