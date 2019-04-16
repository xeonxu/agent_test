#include "ros/ros.h"
#include "agent_test/feedback.h"
#include <cstdlib>
 
int task(void)
{
  
}

void agent_feedback(const agent_test::feedback::ConstPtr& msg)
{
    ROS_INFO(": [%s]", msg->info.data.c_str());
}

int main(int argc, char **argv)
{
    ros::init(argc, argv, "node_mini_factory_server");
 
    ros::NodeHandle n;

    ros::Subscriber sub = n.subscribe("/agent_feedback", 1000, agent_feedback);

    ros::spin();
}
