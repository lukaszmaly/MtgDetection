#pragma once
#include "Player.h"
#include "Server.h"
#include <string>
#include "aruco.h"
#include "opencv2/imgproc/imgproc.hpp"
#include "opencv2/core/core.hpp"
#include "cvdrawingutils.h"
#include "settings.h"
#include <vector>
#include <time.h>
#include <fstream>
using namespace std;
using namespace cv;
using namespace aruco;
#define TARGETMARKER 760
#define M_PI 3.14159265358979323846


class Spell
{
public:
	int cardId;
	int baseId;
	int owner;
	int targetCreature;
	int targetPlayer;
	int value;

	string effect;
	Spell()
	{
	}
	Spell(int cardId,int baseId,int owner,int targetCreature,int targetPlayer,int value,string effect)
	{
		this->cardId=cardId;
		this->effect=effect;
		this->baseId=baseId;
		this->owner=owner;
		this->targetCreature=targetCreature;
		this->targetPlayer=targetPlayer;
		this->value=value;

	}
};


class Action
{
public:
	Actions a;
	int owner;
	int id;
	int targetPlayer;
	string name1;
	string name2;
	int value;
	int targetCreature1;
	int targetCreature2;

	Action(Actions a,int owner=-1,int id=-1,int targetPlayer=-1,int targetCreature1=-1,int targetCreature2=-1,int value=-1,string name1="X",string name2="X")
	{
		this->a=a;
		this->owner=owner;
		this->targetCreature1=targetCreature1;
		this->targetCreature2=targetCreature2;
		this->targetPlayer=targetPlayer;
		this->value=value;
		this->name1=name1;
		this->id=id;
		this->name2=name2;
	}

};

class GameRound
{
public:
	int id;
	int player;
	vector<Action> actions;
	GameRound()
	{
	}
	GameRound(int id,int player)
	{
		this->id=id;
		this->player=player;
	}

	void AddAction(Actions a,int owner=-1,int id=-1,int targetPlayer=-1,int targetCreature1=-1,int targetCreature2=-1,int value=-1,string name1="X",string name2="X")
	{
		actions.push_back(Action(a,owner,id,targetPlayer,targetCreature1,targetCreature2,value,name1,name2));
	}

	void Prepare()
	{
		for(unsigned int i=0;i<actions.size();i++)
		{
			if(actions[i].a!=DEFEND) continue;
			for(unsigned int j=i+1;j<actions.size();j++)
			{
				if(actions[j].a==ATTACK)
				{
					swap(actions[j],actions[i]);
					break;
				}
			}
		}
	}
};

class GameHistory
{

public:
	int round;
	vector<GameRound> rounds;

	GameHistory()
	{
		round=0;
		Init();
	}

	void Init()
	{
		rounds.push_back(GameRound(round,1));
		round++;
	}

	void NewRound(int player)
	{
		rounds[rounds.size()-1].Prepare();
		rounds.push_back(GameRound(round,player));
		round++;
	}

	void AddAction(Actions a,int owner=-1,int id=-1,int targetPlayer=-1,int targetCreature1=-1,int targetCreature2=-1,int value=-1,string name1="X",string name2="X")
	{
		rounds[rounds.size()-1].AddAction(a,owner,id,targetPlayer,targetCreature1,targetCreature2,value,name1,name2);
	}

	void GenerateLog(string player1,string player2)
	{
		fstream plik;
		ostringstream os1;
		ostringstream os;
		os<<time(NULL)<<".txt";
		string buffer(os.str());
		plik.open(buffer, std::ios::in | std::ios::out | std::ios::app);
		os1<<"log=";
		os1<<"NEWGAME "<<player1<<" "<<player2<<endl;
		plik<<"NEWGAME "<<player1<<" "<<player2<<endl;
		for(unsigned int i=0;i<rounds.size();i++)
		{
			if(i!=0) 
			{
				plik<<"NEXTTURN"<<endl;
				os1<<"NEXTTURN"<<endl;
			}

			vector<Action> ac = rounds[i].actions;
			for(unsigned int k=0;k<ac.size();k++)
			{
				if(ac[k].a==PLAYED)
				{
					plik<<"PLAY "<<ac[k].owner<<" "<<ac[k].id<<" "<<ac[k].name1<<" "<<ac[k].targetCreature1<<" "<<ac[k].targetPlayer<<endl;
			os1<<"PLAY "<<ac[k].owner<<" "<<ac[k].id<<" "<<ac[k].name1<<" "<<ac[k].targetCreature1<<" "<<ac[k].targetPlayer<<endl;
				
				}
				else if(ac[k].a==SUBLIFE)
				{
					plik<<"SUBLIFE "<<ac[k].owner<<" "<<ac[k].value<<endl;
				os1<<"SUBLIFE "<<ac[k].owner<<" "<<ac[k].value<<endl;
				}
				else if(ac[k].a==ADDLIFE)
				{
					plik<<"ADDLIFE "<<ac[k].owner<<" "<<ac[k].value<<endl;
					os1<<"ADDLIFE "<<ac[k].owner<<" "<<ac[k].value<<endl;
				
				}
				else if(ac[k].a==DEAD)
				{
					plik<<"DEAD "<<ac[k].id<<endl;
						os1<<"DEAD "<<ac[k].id<<endl;
				
				}
				else if(ac[k].a==ATTACK)
				{
					plik<<"ATTACK "<<ac[k].id<<endl;
					os1<<"ATTACK "<<ac[k].id<<endl;
				
				}
				else if(ac[k].a==DEFEND)
				{
					plik<<"DEFENCE "<<ac[k].targetCreature1<<" "<<ac[k].targetCreature2<<endl;
						os1<<"DEFENCE "<<ac[k].targetCreature1<<" "<<ac[k].targetCreature2<<endl;
				
				}
				else if(ac[k].a==STATS)
				{
					plik<<"STATS "<<ac[k].id<<" "<<ac[k].targetCreature1<<" "<<ac[k].targetCreature2<<" "<<ac[k].value<<endl;
					os1<<"STATS "<<ac[k].id<<" "<<ac[k].targetCreature1<<" "<<ac[k].targetCreature2<<" "<<ac[k].value<<endl;
				
				}
		/*		else if(ac[k].a==NEWGAME)
				{
					plik<<"NEWGAME "<<ac[k].name1<<" "<<ac[k].name2<<endl;
				os1<<"NEWGAME "<<ac[k].name1<<" "<<ac[k].name2<<endl;
				}*/
					else if(ac[k].a==ADDDAMAGE)
				{
					plik<<"ADDDAMAGE "<<ac[k].id<<" "<<ac[k].value<<endl;
				os1<<"ADDDAMAGE "<<ac[k].id<<" "<<ac[k].value<<endl;
				}
			}
		}
		plik.close();
		
		
					Http::Request request("/log/",Http::Request::Post);


	request.setBody(os1.str());

	Http http;
	http.setHost("158.75.2.10",8000);


	Http::Response response = http.sendRequest(request);

	if (response.getStatus() == sf::Http::Response::Ok)
	{
		std::cout << response.getBody() << std::endl;
	}
	else
	{
		std::cout << "request failed" << std::endl;
	}
	


		
	}
};


class Game
{
private:
	
	int aPlayer;
	Phase phase;
	State stackState;
	int gameWidth;
	int gameHeight;
	bool zmiana;
	State stack1;
	State stack2;
	bool checkCardsProp;
	bool bgrMode;
	bool targetMode;
public:
	bool useBlur;
	bool gameEnded;
	GameHistory gh;
	bool gameStarted;
	bool multiplayerMode;
	int playerIdInMultiplayerMode;
	bool ArePlayersReady() const;
	bool player1Done;
	bool player2Done;
	bool player1StackClean;
	bool player2StackClean;
	bool IsStackClean(int player) const;

	void SetPlayerPrepared();

	bool IsMultiplayer() const;
	bool CanResolve;
	vector<Spell> stack;
	void ChangeStackState(int id,State state);
	int lastId;
	static float Distance(Point a,Point b)
	{
		return std::sqrtf(((b.x-a.x)*(b.x-a.x)+(b.y-a.y)*(b.y-a.y)));
	}

	bool IsTargetMode();
	void SetTargetMode(bool value);
	Point firsCardPoint;
	int firstCardWidth;
	int firstCardHeight;
	bool firstCardChecked;

	bool IsBgrMode();
	bool CheckCardsProp();
	Point target;
	bool beAbleMarker;
	float targetAngle;
	float targetOldAngle;
	void StopMarker();

	static float getangle( cv::Point pt1, cv::Point pt2, cv::Point pt0 ) {
		return atan2f( (pt1.y - pt2.y ),( pt1.x - pt2.x ) ) * 180 / M_PI + 180;
	}
	bool oneAttack;

	Point a,b,c,d;
	bool t;
	Player & GetPlayer(int id);
	Player player1;
	Player player2;
	Phase GetPhase();
	Server server;
	vector<Marker> markers;
	int GetGameWidth();
	int GetGameHeight();
	int distance(Point a,Point b);
	void setFaza(int i);
	void setPlayer(int i);
	string getCurrentPhase();
	void nextPhase();
	void Update();
	void Draw(Mat &frame);
	int GetCurrentPlayer();
	Player &getCurrentPlayer();
	Game(string player1s,int player1Id,int player1secondId,string player2s,int player2Id,int player2secondId,string ip,int port,int w,int h,int interval,bool showLog);
	~Game();


	bool oneLandEachTurn;
	bool playedLand;
	void Clear();
	void AddMana(int id,Color color);
	void SubMana(int id,Color color);
	void Pay(int id,int white, int blue,int black,int red,int green,int colorless);
	bool CanPay(int id,int white, int blue,int black,int red,int green,int colorless);
	void SubLife(int id,int value);
	void AddLife(int id,int value);
	bool IsMana(int id,Color color);

	string lastString;

	void GHDefense(int attacker,int defencer)
	{
		cout<<"OBRRONAA"<<endl;
		gh.AddAction(DEFEND,-1,-1,-1,attacker,defencer);
	}

	void GHPlay(int owner,int id,string name,int targetCreature,int targetPlayer)
	{
		gh.AddAction(PLAYED,owner,id,targetPlayer,targetCreature,-1,-1,name);
	}

	void GHAtack(int id)
	{
		cout<<"AATAAAK"<<endl;
		gh.AddAction(ATTACK,-1,id);
	}

	void GHTap(int id)
	{
		string log = "TAP "+to_string((_Longlong)id);
	}
	void GHUntap(int id)
	{
		string log = "UNTAP "+to_string((_Longlong)id);
	}

	void GHSubLife(int owner,int value)
	{
		gh.AddAction(SUBLIFE,owner,-1,-1,-1,-1,value);
	}

	void GHAddLife(int owner,int value)
	{
		gh.AddAction(ADDLIFE,owner,-1,-1,-1,-1,value);
	}
		void GHAddStats(int id,int att,int def,int perm)
	{
		gh.AddAction(STATS,-1,id,-1,att,def,perm);
	}
	void GHNewGame(string name1,string name2)
	{
		gh.AddAction(NEWGAME,-1,-1,-1,-1,-1,-1,name1,name2);
	}

	void GHDie(int id)
	{
		gh.AddAction(DEAD,-1,id);
	}
		void GHAddDamage(int id,int value)
	{
		gh.AddAction(ADDDAMAGE,-1,id,-1,-1,-1,value);
	}
	static float GetAngle(Point p1,Point p2,Point p0)
	{
		float p01 = Distance(p0,p1);
		float p02 = Distance(p0,p2);
		float p12 = Distance(p1,p2);
		float an= acos((p01*p01+p02*p02-p12*p12)/(float)(2*p01*p02)) * 180/M_PI;
		Point w1 = Point(p1.x-p0.x,p1.y-p0.y);
		Point w2 = Point(p2.x-p0.x,p2.y-p0.y);
		if(w1.x*w2.y - w1.y*w2.x < 0)
			an = -an;
		return an;
	}

	void StackSettings(int w,int h,int x1,int y1,int x2,int y2)
	{
		player1.stackB=Point(x1,y1);
		player2.stackB=Point(x2,y2);
		player1.stackE=Point(x1+w,y1+h);
		player2.stackE=Point(x2+w,y2+h);
	}
};