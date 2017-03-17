//math includes
#include <math.h>

//ATD includes
#include <iostream>
#include <vector>

//ROS INCLUDES
#include <ros/ros.h>
#include <tf/transform_broadcaster.h>
#include <tf/transform_listener.h>
#include <visualization_msgs/Marker.h>

//Local includes
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
      ros::Publisher vis_pub;
      NodeHandle node;
      TransformListener listener;



MyPlayer(string argin_name, string argin_team_name): Player(argin_name, argin_team_name)

{  






cout<<"Initialized MyPlayer" << endl;
      Transform t1;

// Subscribe to the make_a_play_message
sub = n.subscribe("/make_a_play/turtle", 1000, &MyPlayer::makeAPlayCallback, this);
vis_pub = n.advertise<visualization_msgs::Marker>( "/bocas", 0 );



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

        tf::StampedTransform getpose(float time_to_wait=0.1){
	tf::StampedTransform trans;
	ros::Time now=Time(0); 
try{
listener.waitForTransform("map",name,now,Duration(time_to_wait));
	listener.lookupTransform("map", name,now, trans); // variavel trans
	}

	catch (TransformException &ex) {
         ROS_ERROR("%s",ex.what());
         Duration(1.0).sleep();
        }

	return trans;
        

};


float getAngleTo(string player_name,float time_to_wait=0.1){
	tf::StampedTransform trans;
	ros::Time now=Time(0); 

	try{
	listener.waitForTransform(name,player_name,now,Duration(time_to_wait));
	listener.lookupTransform(name, player_name,now, trans); // variavel trans
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

float getDistanceTo(string player_name,float time_to_wait=0.1){
tf::StampedTransform trans;
ros::Time now=Time(0);

	try{
	listener.waitForTransform(name,player_name,now,Duration(time_to_wait));
	listener.lookupTransform(name, player_name,now, trans); // variavel trans
	}

	catch (TransformException &ex) {
         ROS_ERROR("%s",ex.what());
         Duration(1.0).sleep();
        }

	float x=trans.getOrigin().x();
        float y=trans.getOrigin().y();
	return sqrt(y*y+x*x);
}


void move(float displacement,float turn_angle,float max_displacement,float max_turn_angle){

//Saturate turn angle
        double max_t=max_turn_angle;
	if(turn_angle > max_t) turn_angle=max_t;
	else if (turn_angle < -max_t) turn_angle = -max_t;



// Saturation displacement
if(displacement>max_displacement)
{
displacement=max_displacement;
}


        //Compute the new reference frame
        tf::Transform t_mov;
        Quaternion q;
        q.setRPY(0, 0, turn_angle);
        t_mov.setRotation(q);
        t_mov.setOrigin( Vector3(displacement , 0.0, 0.0) );

        tf::Transform t = getpose()  * t_mov;
        //Send the new transform to ROS
        br.sendTransform(StampedTransform(t, ros::Time::now(), "/map", name));

}


// IA para o jogo
 void makeAPlayCallback(const rwsua2017_msgs::MakeAPlay::ConstPtr& msg)
      {
        cout << "received a make a play msg with max_displacement = " << msg->max_displacement << endl;
	// Máximos
	float displacement = msg->max_displacement;
	
	// Move my player
//	

	double max_t=(M_PI/30);
        // Definicao dos angulos de rotação e valores de translação 
	// Localizar os outros jogadores
	float dist_dcorreia = getDistanceTo("dcorreia");
        float dist_vsilva = getDistanceTo("vsilva");
        float dist_jsousa= getDistanceTo("jsousa");

	float turn_angle_dcorreia=getAngleTo("dcorreia");
        float turn_angle_vsilva=getAngleTo("vsilva");
        float turn_angle_jsousa=getAngleTo("jsousa");

	// Equipa 5LB
 	float dist_moliveira = getDistanceTo("moliveira");
        float dist_brocha = getDistanceTo("brocha");
        float dist_bvieira = getDistanceTo("bvieira");
	
	float turn_angle_moliveira=getAngleTo("moliveira");
        float turn_angle_brocha=getAngleTo("brocha");
        float turn_angle_bvieira=getAngleTo("bvieira");
	

	float dist_campo = getDistanceTo("map");
        float turn_campo=getAngleTo("map");

	if( dist_campo>5.5)
	move(displacement,turn_campo*1.1,msg->max_displacement,M_PI/30);
	else if(dist_moliveira<=2)
        move(displacement,-turn_angle_moliveira,msg->max_displacement,M_PI/30);
        else if(dist_brocha<=2)
        move(displacement,-turn_angle_brocha,msg->max_displacement,M_PI/30);
 	else if(dist_brocha<=2)
        move(displacement,-turn_angle_bvieira,msg->max_displacement,M_PI/30);
	else {
	if(dist_dcorreia<dist_vsilva && dist_jsousa>dist_dcorreia)
	move(displacement,turn_angle_dcorreia,msg->max_displacement,M_PI/30);
	if(dist_dcorreia>dist_vsilva && dist_jsousa>dist_vsilva)
	move(displacement,turn_angle_vsilva,msg->max_displacement,M_PI/30);
	if(dist_dcorreia>dist_jsousa && dist_jsousa<dist_vsilva)
	move(displacement,turn_angle_jsousa,msg->max_displacement,M_PI/30);
	}



        //
        visualization_msgs::Marker marker;
        marker.header.frame_id = name; //sistema de referencias
        marker.header.stamp = ros::Time();
        marker.ns = name;
        marker.id = 0;
        marker.type = visualization_msgs::Marker::TEXT_VIEW_FACING;
        marker.action = visualization_msgs::Marker::ADD;
        marker.pose.position.x = 0;
        marker.pose.position.y = 0.1;
        marker.pose.position.z = 0;
        marker.pose.orientation.x = 0.0;
        marker.pose.orientation.y = 0.6;
        marker.pose.orientation.z = 0.0;
        marker.pose.orientation.w = 1.0;
        marker.scale.z = 0.4; //when is text, only scale z is used
        marker.color.a = 1.0; // Don't forget to set the alpha!
        marker.color.r = 0.3;
        marker.color.g = 0.3;
        marker.color.b = 0.3;
        marker.frame_locked = 1;
        marker.lifetime = ros::Duration(1);
        marker.text = "sup?!";
        vis_pub.publish( marker ); //publicar o marcador


  	




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
