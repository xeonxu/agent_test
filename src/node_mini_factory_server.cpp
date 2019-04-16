#include "ros/ros.h"
#include "agent_test/feedback.h"
#include "agent_test/task.h"
#include <cstdlib>

// struct task_msg = {
    
// };

ros::ServiceClient client;
int do_task(void)
{
    agent_test::task srv;

    srv.request.messages = "test1";

    if (client.call(srv))
    {
        ROS_INFO("Result: %d", (long int)srv.response.result);
    }
    else
    {
        ROS_ERROR("Failed to call service agnet_task_1");
        return 1;
    }
  
}

void agent_feedback(const agent_test::feedback::ConstPtr& msg)
{
    ROS_INFO("robot %d: [%s]", (int)msg->robot_num.data, msg->robot_sts.data.c_str());
    do_task();       
}

int main(int argc, char **argv)
{
    ros::init(argc, argv, "node_mini_factory_server");
 
    ros::NodeHandle n;

    ros::Subscriber sub = n.subscribe("/agent_feedback", 1000, agent_feedback);
    client = n.serviceClient<agent_test::task>("agent_task_1");
    
    ros::spin();
}
