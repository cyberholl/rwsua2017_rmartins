#include <iostream>
#include <vector>
#include <rwsua2017_libs/player.h>
#include <rwsua2017_msgs/MakeAPlay.h>
using namespace std;

namespace rwsua2017
{
class MyPlayer: public Player
{
public:
MyPlayer(string argin_name, string argin_team_name): Player(argin_name, argin_team_name)

{  
cout<<"Initialized MyPlayer" << endl;
};

vector<string> teammates;

};



}



int main()
{
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

    //cout << "Team Name =  " << player.team_name << endl;
    return 1;                                                      
}
