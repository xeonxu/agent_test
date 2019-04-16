#include "ros/ros.h"
#include "agent_test/task.h"
 
bool task1(agent_test::task::Request  &req,
         agent_test::task::Response &res)
{
    ROS_INFO("Hello: %s", req.messages.c_str());
    res.result = true;
    return true;
}

int main(int argc, char **argv)
{
    ros::init(argc, argv, "node_robot_1");
    ros::NodeHandle n;
 
    ros::ServiceServer service = n.advertiseService("agent_task_1", task1);
    ROS_INFO("ROBOT1:Ready to receive task.");
    ros::spin();
 
    return 0;
}
