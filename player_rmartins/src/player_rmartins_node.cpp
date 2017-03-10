//STD Inludes
#include <iostream>
#include <vector>
 

//ROS includes
#include <ros/ros.h>
#include <tf/transform_broadcaster.h>
#include <tf/transform_listener.h>

// my lybraries includes
#include <rwsua2017_libs/player.h>
#include <rwsua2017_msgs/MakeAPlay.h>

using namespace std;
using namespace ros;
using namespace tf;
using namespace boost;

namespace rwsua2017
{
class MyPlayer: public Player
{
public:

      Subscriber sub;
      TransformBroadcaster br;
      Transform t1;
      NodeHandle node;
      TransformListener listener;

MyPlayer(string argin_name, string argin_team_name): Player(argin_name, argin_team_name)

{  
cout<<"Initialized MyPlayer" << endl;


// Subscribe to the make_a_play_message
sub = n.subscribe("/make_a_play/turtle", 1000, &MyPlayer::makeAPlayCallback, this);

        t1.setOrigin( tf::Vector3(randNumber(),randNumber(), 0.0) );
        Quaternion q;
        q.setRPY(0, 0, 0);
        t1.setRotation(q);
        br.sendTransform(tf::StampedTransform(t1, ros::Time::now(), "map", name));

      cout << "Initialized MyPlayer" << endl;

};

      double randNumber(){
        struct timeval t1;
        gettimeofday(&t1,NULL);
        srand(t1.tv_usec);
        double x =((((double)rand()/(double)RAND_MAX)*2 -1)*5);

        return x;
      }


float getAngleTo(string player_name){
tf::StampedTransform trans;

	try{
	listener.lookupTransform(name, player_name,Time(0), trans); // variavel trans
	}

	catch (TransformException &ex) {
         ROS_ERROR("%s",ex.what());
         Duration(1.0).sleep();
        }

	float x=trans.getOrigin().x();
        float y=trans.getOrigin().y();

        cout << "x= " << x << "y =" << y << endl;
	return atan2(y,x);

}

 void makeAPlayCallback(const rwsua2017_msgs::MakeAPlay::ConstPtr& msg)
      {
        cout << "received a make a play msg with max_displacement = " << msg->max_displacement << endl;


        //Definicao dos angulos de rotação e valores de translação 
        //DEVERIA SER CALCULADO PELA AI DO SISTEMA

       // float turn_angle = getAngleTo(preys_team->at(0));
	float turn_angle = getAngleTo("vsilva");
        float displacement = msg->max_displacement;


	double max_t=(M_PI/30);
	if(turn_angle > max_t) turn_angle=max_t;
	else if (turn_angle < -max_t) turn_angle = -max_t;


        //Compute the new reference frame
        tf::Transform t_mov;
        Quaternion q;
        q.setRPY(0, 0, turn_angle);
        t_mov.setRotation(q);
        t_mov.setOrigin( Vector3(displacement , 0.0, 0.0) );

        tf::Transform t = t1  * t_mov;
        //Send the new transform to ROS
        br.sendTransform(StampedTransform(t, ros::Time::now(), "/map", name));
	t1=t;
      }

vector<string> teammates;

};
}



int main(int argc, char **argv)
{
 ros::init(argc, argv, "player_rmartins");


    cout << "Hello world" << endl;   
   
    //rwsua2017::Player player("rmartins","blue"); 
    //layer.set_team_name();
    rwsua2017::MyPlayer myplayer("rmartins","green");
   

        cout << "name =  " << myplayer.name << endl;
	cout << "team =  " << myplayer.get_team_name()<< endl;
	myplayer.teammates.push_back("fsilva");
	myplayer.teammates.push_back("vsilva");

cout << "Team"<<endl;
//for(unsigned long int i=0; i<myplayer.teammates.size(); ++i)
for(size_t i=0; i<myplayer.teammates.size(); ++i)
{
cout << myplayer.teammates[i]<<endl;
}
ros::spin();// fica em constante funcionamento

    //cout << "Team Name =  " << player.team_name << endl;
    return 1;                                                      
}
