#include <iostream>
#include <vector>

using namespace std;

namespace rwsua2017
{
class Player
{
    public:

    Player(std::string argin_name, std::string argin_team_name="red")
    {
       
	std::cout << "player name " << argin_name << std::endl;    
        this->name = argin_name;
	//this->team_name = argin_team_name;
	set_team_name(argin_team_name);
    }


    std::string name;


	// accessor (SET)
  
void set_team_name(string argin_team_name)
{
	if (argin_team_name =="red" || argin_team_name =="green" || argin_team_name =="blue" )
{
	this->team_name=argin_team_name;
}
	else
{
	cout<<"Error incorrect team name" << endl;
}
}


void set_team_name(void)
{
set_team_name("blue");
}
	// accessor (GET)
	string get_team_name(void)
	{
	return this->team_name;
	};



    	private:
  	string team_name;

};


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
