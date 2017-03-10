//STD Inludes
#include <iostream>
#include <vector>

//ROS includes
#include <ros/ros.h>

//
#include <rwsua2017_libs/player.h>
#include <rwsua2017_msgs/MakeAPlay.h>

using namespace std;

namespace rwsua2017
{
class MyPlayer: public Player
{
public:
// Variaveis têm que ser declaradas antes das funcões
  ros::NodeHandle n;
  ros::Subscriber sub;

MyPlayer(string argin_name, string argin_team_name): Player(argin_name, argin_team_name)

{  
sub = n.subscribe("/make_a_play", 1000, &MyPlayer::makeAPlayCallback, this);

cout<<"Initialized MyPlayer" << endl;
};


void makeAPlayCallback(const rwsua2017_msgs::MakeAPlay::ConstPtr& msg)
{
cout << "received a make a play msg with max displacemente = " <<msg->max_displacement <<endl;   

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
    rwsua2017::MyPlayer myplayer("rmartins","red");
   

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
