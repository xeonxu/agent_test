#include "ros/ros.h"
#include "agent_test/task.h"
#include "agent_test/feedback.h"
#include <common.h>

static int robot_state = RB_STS_READY;
static ros::Publisher feedback_pub;

bool task1(agent_test::task::Request  &req,
           agent_test::task::Response &res)
{
    agent_test::feedback msg;
    msg.robot_num.data = 1;
    robot_state = RB_STS_EXEC;
    msg.robot_sts.data = robot_state;

    feedback_pub.publish(msg);

    ROS_INFO("ROBOT1 process task: %s", req.messages.c_str());
    ros::Duration(5).sleep();

    robot_state = RB_STS_READY;
    msg.robot_sts.data = robot_state;
    feedback_pub.publish(msg);

    res.result = true;
    return true;
}

int main(int argc, char **argv)
{
    ros::init(argc, argv, "node_robot_1");
    ros::NodeHandle n;

    ros::ServiceServer service = n.advertiseService("agent_task_1", task1);

    feedback_pub = n.advertise<agent_test::feedback>("agent_feedback", 1000);

    ros::Rate loop_rate(10);

    agent_test::feedback msg;
    msg.robot_num.data = 1;
    msg.robot_sts.data = RB_STS_READY;

    ros::Duration(1).sleep();
    feedback_pub.publish(msg);

    while (ros::ok())
    {
        ros::spinOnce();

        loop_rate.sleep();
    }

    return 0;
}
