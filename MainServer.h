#pragma once
#include "opencv2\imgproc\imgproc.hpp"
#include <opencv2\core\core.hpp>
#include <opencv2\highgui\highgui.hpp>
#include <opencv2\photo\photo.hpp>
#include <opencv2\features2d\features2d.hpp>
#include <opencv2\nonfree\features2d.hpp>
#include <opencv2\objdetect\objdetect.hpp>
#include <opencv2\calib3d\calib3d.hpp>
#include <SFML\Network.hpp>
#include <vector>
#include <iostream>
#include <cmath>
#include "Game.h"
#include "Card.h"
#include "settings.h"
using namespace std;
using namespace cv;




class PlayerStatus
{
public:
	int id;
	State stackState;
	int cardOnStackId;
	PlayerStatus()
	{
	}


};
class MainServer
{
private:
	int port;
	IpAddress ip;
	TcpListener listener;
	TcpSocket socket;
public:
	void HostGame();
	void ConnectGame();
	void Init(string adres);
	void Send(string tresc);
	void Recv(vector<string> &Msg);


	PlayerStatus player1,player2;
	vector<string> Msg;
	void Recv();
	bool IsStackClean(int player,int cardId) const
	{
		if(player==1) return (this->player1.cardOnStackId==-1 || this->player1.cardOnStackId!=cardId);
		return (this->player2.cardOnStackId==-1 || this->player2.cardOnStackId!=cardId);
	}

	void ChangeStackColor(int id,State state)
	{
		if(id==1 && state==this->player1.stackState) return;
		else if(id==2 && state==this->player2.stackState) return;
		if(id==1) {this->player1.stackState = state; if(state==NEUTRAL) this->player1.cardOnStackId=-1;}
		if(id==2) {this->player2.stackState = state; if(state==NEUTRAL) this->player2.cardOnStackId=-1; }
		SendStackColor(id,state);
	}

	void SendStackColor(int id,State state)
	{
		string st = "X";
		switch(state)
		{
		case OK:
			st="G";
			break;
		case DENY:
			st = "R";
			break;
		case NEUTRAL:
			st = "B";
			break;
		}

		ostringstream os;
		os<<"| STACKCOLOR "<<id<<" "<<st<<" |";
		string buffer(os.str());
		Send(buffer);
	}


	MainServer()
	{
	}

	vector<string> &split(string &s, char delim, vector<string> &elems) {
		stringstream ss(s);
		string item;
		while (std::getline(ss, item, delim)) {
			elems.push_back(item);
		}
		return elems;
	}


	vector<string> split(string &s, char delim) {
		vector<string> elems;
		split(s, delim, elems);
		return elems;
	}




	void Read(vector<Card>&cards,vector<CardB>&bcards,Game &game)
	{
		while(Msg.size()!=0)
		{
		vector<string> msg;

		if(Msg.size()!=0)
		{
			msg=split(Msg[0],' ');
		}
		if(msg.size()==0) return;
		cout<<"------"<<endl;
		cout<<"Odebrano "<<Msg[0]<<endl;
		cout<<"------"<<endl;

		if(msg[1].compare("ADD")==0)
		{
			//os<<"| ADD "<<id<<" "<<idb<<" "<<owner<<" "<<a.x<<" "<<a.y<<" "<<b.x<<" "<<b.y<<" "<<c.x<<" "<<c.y<<" "<<d.x<<" "<<d.y<<" "<<taped<<" |";
	
			AddCard(atoi(msg[2].c_str()),atoi(msg[3].c_str()),Point(atoi(msg[5].c_str()),atoi(msg[6].c_str())),Point(atoi(msg[7].c_str()),atoi(msg[8].c_str())),Point(atoi(msg[9].c_str()),atoi(msg[10].c_str())),Point(atoi(msg[11].c_str()),atoi(msg[12].c_str())),cards,bcards,game);
		}
		else if(msg[1].compare("UPDATE")==0)
		{
			//os<<"| UPDATE "<<id<<" "<<idb<<" "<<owner<<" "<<a.x<<" "<<a.y<<" "<<b.x<<" "<<b.y<<" "<<c.x<<" "<<c.y<<" "<<d.x<<" "<<d.y<<" "<<taped<<" "<<att<<" "<<def<<" |";
			UpdateCard(atoi(msg[2].c_str()),Point(atoi(msg[5].c_str()),atoi(msg[6].c_str())),Point(atoi(msg[7].c_str()),atoi(msg[8].c_str())),Point(atoi(msg[9].c_str()),atoi(msg[10].c_str())),Point(atoi(msg[11].c_str()),atoi(msg[12].c_str())),cards,game,atoi(msg[14].c_str()),atoi(msg[15].c_str()));
		}
		else if(msg[1].compare("ATTACK")==0)
		{
			//	os<<"| ATTACK "<<id<<" "<<idb<<" "<<owner<<" "<<a.x<<" "<<a.y<<" "<<b.x<<" "<<b.y<<" "<<c.x<<" "<<c.y<<" "<<d.x<<" "<<d.y<<" "<<taped<<" "<<att<<" "<<def<<" |";
	
			//UpdateCard(atoi(msg[2].c_str()),Point(atoi(msg[6].c_str()),atoi(msg[7].c_str())),Point(atoi(msg[8].c_str()),atoi(msg[9].c_str())),Point(atoi(msg[10].c_str()),atoi(msg[11].c_str())),Point(atoi(msg[12].c_str()),atoi(msg[13].c_str())),cards,game,atoi(msg[14].c_str()),atoi(msg[15].c_str()));
			Attack(atoi(msg[2].c_str()),Point(atoi(msg[5].c_str()),atoi(msg[6].c_str())),Point(atoi(msg[7].c_str()),atoi(msg[8].c_str())),Point(atoi(msg[9].c_str()),atoi(msg[10].c_str())),Point(atoi(msg[11].c_str()),atoi(msg[12].c_str())),cards,game,atoi(msg[14].c_str()),atoi(msg[15].c_str()));
		}
		else if(msg[1].compare("BLOCK")==0)
		{
				//os<<"| BLOCK "<<id<<" "<<idb<<" "<<owner<<" "<<a.x<<" "<<a.y<<" "<<b.x<<" "<<b.y<<" "<<c.x<<" "<<c.y<<" "<<d.x<<" "<<d.y<<" "<<taped<<" "<<att<<" "<<def<<" "<<defId<<" |";
	
			//UpdateCard(atoi(msg[2].c_str()),Point(atoi(msg[6].c_str()),atoi(msg[7].c_str())),Point(atoi(msg[8].c_str()),atoi(msg[9].c_str())),Point(atoi(msg[10].c_str()),atoi(msg[11].c_str())),Point(atoi(msg[12].c_str()),atoi(msg[13].c_str())),cards,game,atoi(msg[14].c_str()),atoi(msg[15].c_str()));
			Block(atoi(msg[2].c_str()),Point(atoi(msg[5].c_str()),atoi(msg[6].c_str())),Point(atoi(msg[7].c_str()),atoi(msg[8].c_str())),Point(atoi(msg[9].c_str()),atoi(msg[10].c_str())),Point(atoi(msg[11].c_str()),atoi(msg[12].c_str())),cards,game,atoi(msg[14].c_str()),atoi(msg[15].c_str()),atoi(msg[16].c_str()));
		}
		else if(msg[1].compare("STACKCOLOR")==0)
		{
			game.server.Send(Msg[0]);
		}
		else if(msg[1].compare("CARDONSTACK")==0)
		{
			game.server.Send(Msg[0]);
		}
		else if(msg[1].compare("NEXTPHASE")==0)
		{
			NextPhase(game);
		}
		else if(msg[1].compare("READY")==0)
		{
			cout<<"Przygotowano"<<endl;
			Ready(game);
		}
		else if(msg[1].compare("DEAD")==0)
		{
			Dead(atoi(msg[2].c_str()),cards,game);
		}
		else if(msg[1].compare("SUBMANA2")==0)
		{
			Pay(atoi(msg[2].c_str()),atoi(msg[3].c_str()),atoi(msg[4].c_str()),atoi(msg[5].c_str()),atoi(msg[6].c_str()),atoi(msg[7].c_str()),atoi(msg[8].c_str()),game);
		}
		else if(msg[1].compare("ADDLIFE")==0)
		{
			AddLife(atoi(msg[2].c_str()),atoi(msg[3].c_str()),game);
		}
		else if(msg[1].compare("SUBLIFE")==0)
		{
			SubLife(atoi(msg[2].c_str()),atoi(msg[3].c_str()),game);
		}
		else if(msg[1].compare("PLAYED")==0)
		{
			PlayedCard(atoi(msg[2].c_str()),atoi(msg[3].c_str()),msg[4],atoi(msg[5].c_str()),atoi(msg[6].c_str()),game);
		}
		else if(msg[1].compare("PLAYER")==0)
		{
			Player(atoi(msg[2].c_str()),msg[3],game);
			game.server.AddPlayer(atoi(msg[2].c_str()),msg[3].c_str());
		}
		else if(msg[1].compare("EFFECT")==0)
		{
			game.server.Send(Msg[0]);
		}
		Msg.erase(Msg.begin()+0);
		}
	
	
	
	}
	~MainServer(void);

	void SendSubLife(int owner,int value)
	{
		ostringstream os;
		os<<"| SUBLIFE " <<owner <<" "<<value<<" |";
		string buffer(os.str());
		Send(buffer);
	}

	void AddLife(int owner,int value,Game &game)
	{
		game.AddLife(owner,value);
	}
	void SubLife(int owner,int value,Game &game)
	{
		game.SubLife(owner,value);
	}
	void SendAddLife(int owner,int value)
	{
		ostringstream os;
		os<<"| SUBLIFE " <<owner <<" "<<value<<" |";
		string buffer(os.str());
		Send(buffer);
	}
	//[done]
	void SendNextPhase();
	void NextPhase(Game &game);
	void SendReady();
	void Ready(Game &game);
	void SendDead(int id);
	void Dead(int id,vector<Card>&cards,Game &game);
	void SendEffect(string effect,int player,int creature);
	void Effect(string effect,int player,int creature,Game &game);
	void SendNewCard(int id,int idb,int owner,Point a, Point b,Point c,Point d,bool taped);
	void AddCard(int id,int baseId,Point a,Point b,Point c,Point d,vector<Card>&cards,vector<CardB>&bcards,Game &game);
	void SendUpdatedCard(int id,int idb,int owner,Point a, Point b,Point c,Point d,bool taped,int att,int def);
	void UpdateCard(int id,Point a,Point b,Point c,Point d,vector<Card>&cards,Game &game,int att,int def);
	//[end of done]

	void PlayedCard(int owner,int id,string name,int targetCreature,int targetPlayer,Game &game)
	{
		game.GHPlay(owner,id,name,targetCreature,targetPlayer);
	}
	void SendPlayedCard(int owner,int id,string name,int targetCreature,int targetPlayer)
	{
		ostringstream os;
		os<<"| PLAYED " <<owner <<" "<<id<<" "<<name<<" "<<targetCreature<<" "<<targetPlayer<<" |";
		string buffer(os.str());
		Send(buffer);
	}
	
	void Player(int owner,string name,Game &game)
	{
		if(owner==1) {game.player1.name=name; game.player1.recognized=true;}
		else if(owner==2) {game.player2.name=name; game.player2.recognized=true;}
	}
	void SendPlayer(int owner,string name)
	{
		ostringstream os;
		os<<"| PLAYER " <<owner <<" "<<name<<" |";
		string buffer(os.str());
		Send(buffer);
	}

	void SendCardOnStack(int owner,int id)
	{
		if(owner==1)
		{
			this->player1.cardOnStackId=id;
		}
		else
		{
			this->player2.cardOnStackId=id;
		}
		ostringstream os;
		os<<"| CARDONSTACK " <<owner <<" "<<id<<" |";
		string buffer(os.str());
		Send(buffer);
	}


	void Pay(int id,int white, int blue,int black,int red,int green,int colorless,Game &game)
	{
		game.Pay(id,white,blue,black,red,green,colorless);
	}

	void SendPaid(int id,int white, int blue,int black,int red,int green,int colorless)
	{
		ostringstream os;
		os<<"| SUBMANA2 "<<id<<" "<<white<<" "<<blue<<" "<<black<<" "<<red<<" "<<green<<" "<<colorless<<" |";
		string buffer(os.str());
		Send(buffer);
	}



	//popraw, bo nie pamietam co to robi :P
	void Stack(int id,int baseId,State state,Game &game,vector<Card>&stack,vector<CardB>&bcards)
	{
		int owner = 1;
		if(game.playerIdInMultiplayerMode==1)	owner=2;
		if(baseId!=-1)
		{
			for(unsigned int i=0;i<stack.size();i++)//pisane na szybko :/
			{
				if(stack[i].owner==owner)
				{
				}
				else if(i==stack.size()-1)
				{
					Card t=Card(Point(0,0),Point(0,0),Point(0,0),Point(0,0),bcards,game,owner,baseId);
					stack.push_back(t);
				}
			}

		}
		else
		{
			for(unsigned int i=0;i<stack.size();i++)
			{
				if(stack[i].owner==owner)
				{
					stack.erase(stack.begin()+i);
					break;
				}
			}
		}
		game.server.Stack(owner,baseId,state);

	}



	void SendAttackingCard(int id,int idb,int owner,Point a, Point b,Point c,Point d,bool taped,int att,int def)
	{
			ostringstream os;
		os<<"| ATTACK "<<id<<" "<<idb<<" "<<owner<<" "<<a.x<<" "<<a.y<<" "<<b.x<<" "<<b.y<<" "<<c.x<<" "<<c.y<<" "<<d.x<<" "<<d.y<<" "<<taped<<" "<<att<<" "<<def<<" |";
		string buffer(os.str());
		Send(buffer);
	}

	void Attack(int id,Point a,Point b,Point c,Point d,vector<Card>&cards,Game &game,int att,int def)
	{
		for(unsigned int i=0;i<cards.size();i++)
		{
			if(cards[i].id==id)
			{
				cards[i].Update(a,b,c,d,att,def,game);
				cards[i].attack=true;
				//game.server.Attack(cards[i].id,cards[i].cardBase.id,cards[i].owner,a,b,c,d,cards[i].taped,att,def);
			}
		}
	}




	void SendBlockingCard(int id,int idb,int owner,Point a, Point b,Point c,Point d,bool taped,int att,int def,int defId)
	{
			ostringstream os;
		os<<"| BLOCK "<<id<<" "<<idb<<" "<<owner<<" "<<a.x<<" "<<a.y<<" "<<b.x<<" "<<b.y<<" "<<c.x<<" "<<c.y<<" "<<d.x<<" "<<d.y<<" "<<taped<<" "<<att<<" "<<def<<" "<<defId<<" |";
		string buffer(os.str());
		Send(buffer);
	}


	void Block(int id,Point a,Point b,Point c,Point d,vector<Card>&cards,Game &game,int att,int def,int blockId)
	{
		for(unsigned int i=0;i<cards.size();i++)
		{
			if(cards[i].id==id)
			{
				cards[i].Update(a,b,c,d,att,def,game);
			}
		}
	}


};

