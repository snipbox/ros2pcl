/*
 * @Author: xiaohui.zhou xhzhou1991@gmail.com
 * @Date: 2022-06-24 08:45:50
 * @LastEditors: xiaohui.zhou xhzhou1991@gmail.com
 * @LastEditTime: 2022-06-25 00:00:13
 * @FilePath: /ros2pcl/src/ros2-pcl/src/main.cpp
 * @Description:
 *
 * Copyright (c) 2022 by xiaohui.zhou xhzhou1991@gmail.com, All Rights Reserved.
 */
#define BOOST_BIND_NO_PLACEHOLDERS
#include <iostream>
#include <string>
// ros2
#include <rclcpp/rclcpp.hpp>
#include <sensor_msgs/msg/point_cloud2.hpp>
// pcl
#include <pcl_conversions/pcl_conversions.h>
#include <pcl/point_cloud.h>
#include <pcl/point_types.h>
#include <pcl/io/ply_io.h>
#include <Eigen/Core>

using std::placeholders::_1;
using std::placeholders::_2;
class pclHandler : public rclcpp::Node {
public:
    // 构造函数,有一个参数为节点名称
    pclHandler(std::string name) :
        Node(name) {
        RCLCPP_INFO(this->get_logger(), "大家好，我是%s.", name.c_str());

        auto default_qos = rclcpp::QoS(rclcpp::SystemDefaultsQoS());
        sub_novel        = this->create_subscription<sensor_msgs::msg::PointCloud2>("/zvision_lidar_points", default_qos, std::bind(&pclHandler::topic_callback, this, _1));
    }

private:
    // 声明一个订阅者（成员变量）,用于订阅小说
    rclcpp::Subscription<sensor_msgs::msg::PointCloud2>::SharedPtr sub_novel;
    // 收到话题数据的回调函数
    void topic_callback(const sensor_msgs::msg::PointCloud2::SharedPtr msg) {
        // extract the point cloud
        pcl::PointCloud<pcl::PointXYZ>::Ptr pcl_point_cloud(new pcl::PointCloud<pcl::PointXYZ>);
        pcl::fromROSMsg(*msg, *pcl_point_cloud);
        pcl::io::savePLYFile("/home/hui/ros2pcl/test.ply", *pcl_point_cloud);
    };
};
int main(int argc, char const *argv[]) {
    // ros2 init
    rclcpp::init(argc, argv);
    auto node = std::make_shared<pclHandler>("ros2pcl");
    /* 运行节点，并检测退出信号*/
    rclcpp::spin(node);
    rclcpp::shutdown();

    /* code */
    return 0;
}
