#pragma once
#include "Game.h"
#include "Card.h"
#include "CardB.h"
#include <vector>
#include "aruco.h"
#include "opencv2/core/core.hpp"
#include "marker.h"
#include "cvdrawingutils.h"
#include "MainServer.h"
#include <cmath>
using namespace aruco;
using namespace std;
class ScriptsManager
{
public:
	int lastCardId;
	Point target;
	bool canUseMarker;
	int targetId;
	int targetPlayerId;
	bool targetsPlayer;
	MarkerDetector MDetector;
	bool first;
	ScriptsManager(void)
	{
		canUseMarker=true;
		target=Point(-1,-1);
		targetId=-1;
		targetPlayerId=-1;
	}

	void AddEOT(int id,vector<Card> &cards,int attack,int defense)
	{
		for(unsigned int i=0;i<cards.size();i++)
		{
			if(cards[i].id==id)
			{
				cards[i].AddEOT(attack,defense);
				cout<<"KARTA: "<<attack<<" "<<defense<<endl;
				return;
			}
		}
	}
	void DestroyCreature(int id,vector<Card> &cards)
	{
		for(unsigned int i=0;i<cards.size();i++)
		{
			if(cards[i].id==id)
			{
				cards[i].die();
				return;
			}
		}
	}
	void AddDamage(int id,vector<Card> &cards,int value)
	{
		for(unsigned int i=0;i<cards.size();i++)
		{
			if(cards[i].id==id)
			{
				cards[i].Damage(value);
				return;
			}
		}
	}
	void SubLife(int id,int n,Game &game)
	{
		game.SubLife(id,n);
	}
	void AddLife(int id,int n,Game &game)
	{
		game.AddLife(id,n);
	}
	void DrawCard(int id,int n,Game &game)
	{
		game.server.DrawCard(id,n);
	}
	void ScryCard(int id,int n,Game &game)
	{
		game.server.Scry(id,n);
	}

	int GetCardId(Point p,vector<Card> &cards,Game &game)
	{
		targetsPlayer=true;
		int index =-1;
		int distance = 10000000;
		targetPlayerId=1;
		int d;
		distance = game.distance(p,Point(game.player1.stackB.x,0));
		d = game.distance(p,Point(game.player2.stackB.x,game.GetGameHeight()));
		if(d<distance)
		{
			targetPlayerId=2;
			distance = d;
		}
		int distance1=100000;
		for(unsigned int i=0;i<cards.size();i++)
		{
			d = game.distance(cards[i].getCenter(),p);
			if(d<distance1 && !cards[i].cardBase.hasHexproof && cards[i].cardBase.type==CREATURE)
			{
					distance1 = d;
				if(distance1<distance)
				{
				targetsPlayer=false;
				}
			
				index = cards[i].id;
				
			}

		}
		return index;
	}

	void Upkeep(Game &game,vector<Card> &cards,int player)
	{

		for(unsigned int i=0;i<cards.size();i++)
		{
			vector<pair<int,int>> tt=cards[i].cardBase.upkeepAbilities;
			for(unsigned int z =0; z<tt.size();z++)
			{
				if(player==cards[i].owner) game.stack.push_back(Spell(cards[i].cardBase.id,tt[z].first,cards[i].owner,cards[i].id,cards[i].owner,tt[z].second,cards[i].cardBase.effect));
			}
			 tt=cards[i].upkeepAb;
			for(unsigned int z =0; z<tt.size();z++)
			{
				if(player==cards[i].owner) game.stack.push_back(Spell(cards[i].cardBase.id,tt[z].first,cards[i].owner,cards[i].id,cards[i].owner,tt[z].second,cards[i].cardBase.effect));
			}

		}
	}

	void Update(Mat &img,Game &game,vector<Card> &cards,vector<Card> &stos,MainServer &mainServer)
	{
		//char cad6[100];
		lastCardId=-1;
		MDetector.setMinMaxSize(0.01f);
		vector<Marker> markers;
		MDetector.detect(img,markers);
		for(unsigned int i=0;i<markers.size();i++)
		{
			if(markers[i].id==TARGETMARKER)
			{
				if(canUseMarker==false) {break;}
				target=markers[i].getCenter();
				targetId=GetCardId(target,cards,game);
				if(targetId!=-1 || targetPlayerId!=-1)
				{
					canUseMarker=false;
				}
				break;
			}
		}
		if(stos.size()==0)
		{
			targetId=-1;
			targetPlayerId=-1;
			canUseMarker=true;
		}
		/*sprintf(cad6,"Aktualna cel:%d %d (%d,%d)",targetId,targetPlayerId,target.x,target.y);
		putText(img,cad6, Point(10,100),FONT_HERSHEY_SIMPLEX, 0.5,  Scalar(0,0,255),2);
*/
		for(unsigned int i=0;i<stos.size();i++)
		{
			if((targetId!=-1 || targetPlayerId!=-1) && (stos[i].cardBase.type==INSTANT || stos[i].cardBase.type==ENCHANTMENT || stos[i].cardBase.type==SORCERY) && stos[i].cardBase.id!=game.lastId 
				&& game.CanPay(stos[i].owner,stos[i].cardBase.whiteCost,stos[i].cardBase.blueCost,stos[i].cardBase.blackCost,stos[i].cardBase.redCost,stos[i].cardBase.greenCost,stos[i].cardBase.lessCost))
			{

				game.Pay(stos[i].owner,stos[i].cardBase.whiteCost,stos[i].cardBase.blueCost,stos[i].cardBase.blackCost,stos[i].cardBase.redCost,stos[i].cardBase.greenCost,stos[i].cardBase.lessCost);
			
				
				
					int tc=targetId;
				int tp=targetPlayerId;
				if(targetsPlayer==true) tc=-1;
				else
					tp=-1;
				game.GHPlay(stos[i].owner,stos[i].id,stos[i].cardBase.name,tc,tp);
						if(game.IsMultiplayer()) 
						{
							mainServer.ChangeStackColor(stos[i].owner,NEUTRAL);
							mainServer.SendPlayedCard(stos[i].owner,stos[i].id,stos[i].cardBase.name,tc,tp);
						}
						game.server.StackColor(stos[i].owner,NEUTRAL);
				vector<pair<int,int>> t =stos[i].cardBase.enterAbilities;
				for(int j=0;j<t.size();j++)
				{
					game.stack.push_back(Spell(stos[i].cardBase.id,t[j].first,stos[i].owner,targetId,targetPlayerId,t[j].second,stos[i].cardBase.effect));
				}
						 t =stos[i].cardBase.upkeepAbilities;
						for(int j=0;j<cards.size();j++)
				{
					if(targetId==cards[j].id)
					{
						for(unsigned int k=0;k<t.size();k++)
						{
							cards[j].upkeepAb.push_back(t[k]);
						}
					}
				//	game.stack.push_back(Spell(stos[i].cardBase.id,t[j].first,stos[i].owner,targetId,targetPlayerId,t[j].second,stos[i].cardBase.effect));
				}

				game.lastId=stos[i].cardBase.id;
				canUseMarker=true;
				targetId=-1;
				targetPlayerId=-1;
				stos.erase(stos.begin()+i);
			}

		

		}

		for(unsigned int i=0;i<game.stack.size();i++)
		{
			Resolve(game.stack[i],cards,game,mainServer);
			game.stack.erase(game.stack.begin()+i);
			i=-1;
		}
	}


	void VisualEffect(Game &game,MainServer &mainServer,string d,string s,int targetPlayer,int targetCreature)
	{
		if(d.compare("NONE")==0)
		{
			if(first==true)
			{
				game.server.VisualEffect(s,targetPlayer,targetCreature);
				mainServer.SendEffect(s,targetPlayer,targetCreature);
			}

		}
		else
		{
			game.server.VisualEffect(d,targetPlayer,targetCreature);
			mainServer.SendEffect(d,targetPlayer,targetCreature);
		}
	}
	void Resolve(Spell s,vector<Card> &cards,Game &game,MainServer &mainServer)
	{
		first=true;
		if(s.cardId==lastCardId) first=false;
		lastCardId=s.cardId;
		cout<<s.baseId<< " " <<s.targetCreature << s.targetPlayer<<endl;
		switch(s.baseId)
		{
		case 0:
			AddLife(s.targetPlayer,s.value,game);
			VisualEffect(game,mainServer,s.effect,"ADDLIFE",s.targetPlayer,-1);
			if(game.IsMultiplayer()) mainServer.SendAddLife(s.targetPlayer,s.value);
			break;
		case 1:
			SubLife(s.targetPlayer,s.value,game);
			VisualEffect(game,mainServer,s.effect,"SUBLIFE",s.targetPlayer,-1);
			if(game.IsMultiplayer()) mainServer.SendSubLife(s.targetPlayer,s.value);
			break;
		case 2:
			AddDamage(s.targetCreature,cards,s.value);
			game.GHAddDamage(s.targetCreature,s.value);
			VisualEffect(game,mainServer,s.effect,"BOLT",-1,s.targetCreature);
			break;
		case 3:
			SubLife(s.targetPlayer,s.value,game);
				if(game.IsMultiplayer()) mainServer.SendSubLife(s.targetPlayer,s.value);
			VisualEffect(game,mainServer,s.effect,"SPEAR",s.targetPlayer,-1);
			break;
		case 4:
			AddEOT(s.targetCreature,cards,s.value,s.value);
			game.GHAddStats(s.targetCreature,s.value,s.value,1);
			VisualEffect(game,mainServer,s.effect,"BOOST",-1,s.targetCreature);
			break;

		case 5:
			AddEOT(s.targetCreature,cards,(-1)*s.value,(-1)*s.value);
			game.GHAddStats(s.targetCreature,(-1)*s.value,(-1)*s.value,1);
			VisualEffect(game,mainServer,s.effect,"REDUCTION",-1,s.targetCreature);
			break;
		case 6:
			DestroyCreature(s.targetCreature,cards);
			VisualEffect(game,mainServer,s.effect,"DESTROY",-1,s.targetCreature);
			break;
		case 7:
			AddEOT(s.targetCreature,cards,s.value,0);
				game.GHAddStats(s.targetCreature,s.value,0,1);
			VisualEffect(game,mainServer,s.effect,"BOOST",-1,s.targetCreature);
			break;
		case 8:
			AddEOT(s.targetCreature,cards,0,s.value);
				game.GHAddStats(s.targetCreature,0,s.value,1);
			VisualEffect(game,mainServer,s.effect,"ADDDEF",-1,s.targetCreature);
			break;
		case 9:
			AddLifelink(s.targetCreature,cards,true);
			VisualEffect(game,mainServer,s.effect,"LIFELINKP",-1,s.targetCreature);
			break;
		case 10:
			AddLifelink(s.targetCreature,cards,false);
			VisualEffect(game,mainServer,s.effect,"LIFELINK",-1,s.targetCreature);
			break;
		case 11:
			AddFirstStrike(s.targetCreature,cards,true);
			VisualEffect(game,mainServer,s.effect,"FIRSTSTRIKEP",-1,s.targetCreature);
			break;
		case 12:
			AddFirstStrike(s.targetCreature,cards,false);
			VisualEffect(game,mainServer,s.effect,"FIRSTSTRIKE",-1,s.targetCreature);
			break;
		case 13:
			AddFlying(s.targetCreature,cards,true);
			break;
		case 14:
			AddFlying(s.targetCreature,cards,false);
			break;
		case 19:
			PutCounter(s.targetCreature,cards,s.value,s.value);
				game.GHAddStats(s.targetCreature,s.value,s.value,2);
			VisualEffect(game,mainServer,s.effect,"BOOST",-1,s.targetCreature);
			break;
		case 20:
			PutCounter(s.targetCreature,cards,(-1)*s.value,(-1)*s.value);
				game.GHAddStats(s.targetCreature,(-1)*s.value,(-1)*s.value,2);
			VisualEffect(game,mainServer,s.effect,"REDUCTION",-1,s.targetCreature);
			break;
		case 21:
			DrawCard(s.owner,s.value,game);
			VisualEffect(game,mainServer,s.effect,"DRAW",s.targetPlayer,-1);
			break;
		case 22:
			CantAttack(s.targetCreature,cards);
			break;
		case 23:
			CantBlock(s.targetCreature,cards);
			break;
		case 24:
			AddLife(s.owner,s.value,game);
					if(game.IsMultiplayer()) mainServer.SendAddLife(s.owner,s.value);
			break;
		case 25:
			SubLife(s.owner,s.value,game);
					if(game.IsMultiplayer()) mainServer.SendSubLife(s.owner,s.value);
			break;
		case 30:
			ScryCard(s.owner,s.value,game);
			VisualEffect(game,mainServer,s.effect,"SCRY",s.owner,-1);
			break;
		case 31:
			if(targetsPlayer==true && s.targetPlayer!=-1)
			{
				SubLife(s.targetPlayer,s.value,game);
					if(game.IsMultiplayer()) mainServer.SendSubLife(s.targetPlayer,s.value);
					VisualEffect(game,mainServer,s.effect,"SPEAR",s.targetPlayer,-1);
		
			}
			else if(targetsPlayer==false && s.targetCreature!=-1)
			{
				AddDamage(s.targetCreature,cards,s.value);
				game.GHAddDamage(s.targetCreature,s.value);
				VisualEffect(game,mainServer,s.effect,"SPEAR",-1,s.targetCreature);
				
			}
			break;
		}
		if(s.targetCreature!=-1)
		{
			for(unsigned int i=0;i<cards.size();i++)
			{
				if(s.targetCreature==cards[i].id)
				{
					cards[i].la=cards[i].lb=cards[i].lc=cards[i].ld=Point(-1,-1);
				}
			}
		}
	}


	
	void CantBlock(int id,vector<Card>&cards)
	{
		for(unsigned int i=0;i<cards.size();i++)
		{
			if(cards[i].id==id)
			{
				cards[i].hasCantBlock=true;
			}
		}
	}
	void CantAttack(int id,vector<Card>&cards)
	{
		for(unsigned int i=0;i<cards.size();i++)
		{
			if(cards[i].id==id)
			{
				cards[i].hasCantAttack=true;
			}
		}
	}
	void PutCounter(int id,vector<Card>&cards,int att,int def)
	{
		for(unsigned int i=0;i<cards.size();i++)
		{
			if(cards[i].id==id)
			{
				cards[i].Add(att,def);
			}
		}
	}
	void AddHexproof(int id,vector<Card>&cards,bool permanent)
	{
		for(unsigned int i=0;i<cards.size();i++)
		{
			if(cards[i].id==id)
			{
				if(permanent) cards[i].hasHexproof=true;
				else	cards[i].hasHexproofEOT=true;
			}
		}
	}
	void AddFlying(int id,vector<Card>&cards,bool permanent)
	{
		for(unsigned int i=0;i<cards.size();i++)
		{
			if(cards[i].id==id)
			{
				if(permanent) cards[i].hasFlying=true;
				else cards[i].hasFlyingEOT=true;
			}
		}
	}
	void AddLifelink(int id,vector<Card>&cards,bool permanent)
	{
		for(unsigned int i=0;i<cards.size();i++)
		{
			if(cards[i].id==id)
			{
				if(permanent) cards[i].hasLifelink=true;
				else	cards[i].hasLifelinkEOT=true;
			}
		}
	}
	void AddFirstStrike(int id,vector<Card>&cards,bool permanent)
	{
		for(unsigned int i=0;i<cards.size();i++)
		{
			if(cards[i].id==id)
			{
				if(permanent) cards[i].hasFirstStrike=true;
				else	cards[i].hasFirstStrikeEOT=true;
			}
		}
	}
	~ScriptsManager(void);
};

