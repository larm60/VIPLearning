#include <ros/ros.h>
#include <geometry_msgs/Twist.h>
#include <kobuki_msgs/BumperEvent.h>
#include <stdlib.h>
#include <std_msgs/String.h>

void callback(const std_msgs::String::ConstPtr&){

}

int main(int argc, char **argv) {
	//Inititalize the ROS system and become a node
	ros :: init(argc, argv, "publish_velocity");
	ros :: NodeHandle nh;

	//Create a publisher object 
	ros:: Publisher pub = nh.advertise<geometry_msgs ::Twist>("/cmd_vel_mux/input/teleop",1000);
	ros:: Subscriber sub = nh.subscribe<kobuki_msgs ::BumperEvent>("/mobile_base/events/bumper",1000,callback);

	//Seed the random number generator
	srand(time(0));

	//Loop at 2Hz until node is shut down.

	ros :: Rate rate(2);
	while(ros::ok()) {
		//Create and fill in the message. The other four fields,
		//which are ignored by turtlesim, default to 0.
		kobuki_msgs:: BumperEvent bumpMsg;
		geometry_msgs:: Twist msg;

		if(bumpMsg.state) {
			msg.linear.x = -1;

		} else {
			msg.linear.x = double(rand())/double(RAND_MAX);
			msg.angular.z = 2*double(rand())/double(RAND_MAX) -1;
		}




		// Publish the message.
		pub.publish(msg);

		//Send a message to rosout with the details.
		ROS_INFO_STREAM("Sending random velocity command:" << "linear=" << msg.linear.x
				<< "angular=" <<msg.angular.z);

		//Wait until its time for another iteration
		rate.sleep();
	}

}

