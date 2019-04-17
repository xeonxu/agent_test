#include "ros/ros.h"
#include "agent_test/feedback.h"
#include "agent_test/task.h"
#include <common.h>
#include <sstream>
#include <cstdlib>
#include <thread>

static const char* tasks[5] = {
    "task1",
    "task2",
    "task3",
    "task4",
    "task5"
};

static int task_idx = 0;

static int robot_states[2] = {1};

static ros::NodeHandle *np;
// ros::ServiceClient client;

int do_task(const int robot_num)
{
    agent_test::task srv;

    ROS_INFO("do task");
    robot_states[robot_num-1] = RB_STS_EXEC;
    srv.request.messages = tasks[task_idx++];

    std::stringstream sc;
    sc << "agent_task_" << robot_num;

    ros::ServiceClient client = np->serviceClient<agent_test::task>(sc.str());

    if (client.call(srv))
    {
        ROS_INFO("Result: %d", (long int)srv.response.result);
    }
    else
    {
        ROS_ERROR("Failed to call service agent_task_1");
        return 1;
    }
  
}

void agent_feedback_cb(const agent_test::feedback::ConstPtr& msg)
{
    ROS_INFO("ROBOT%d: [%s]", (int)msg->robot_num.data, (int)msg->robot_sts.data?"executing":"ready");
    robot_states[msg->robot_num.data-1] = msg->robot_sts.data;
}

int main(int argc, char **argv)
{
    ros::init(argc, argv, "node_mini_factory_server");
 
    ros::NodeHandle nh;

    ros::Subscriber sub = nh.subscribe("/agent_feedback", 1000, agent_feedback_cb);
    ros::Rate loop_rate(10);

    np = &nh;

    ros::Duration(1).sleep();

    while (ros::ok())
    {
        if (task_idx < 5) {
            for (int i = 0; i < sizeof(robot_states)/sizeof(robot_states[0]); ++i) {
                if (RB_STS_READY == robot_states[i]) {
                    ROS_DEBUG("robot %d status %d", i, robot_states[i]);
                    std::thread *t = new std::thread(do_task, i+1);
                    t->detach();
                    delete t;
                }
            }
        }
        ros::spinOnce();
        loop_rate.sleep();
    }
}
