#pragma once
#include "opencv2\imgproc\imgproc.hpp"
#include <opencv2\core\core.hpp>
#include <opencv2\highgui\highgui.hpp>
#include <opencv2\photo\photo.hpp>
#include <opencv2\features2d\features2d.hpp>
#include <opencv2\nonfree\features2d.hpp>
#include <opencv2\objdetect\objdetect.hpp>
#include <opencv2\calib3d\calib3d.hpp>
#include <iostream>
#include <cmath>
#include "CardB.h"
#include "Game.h"

#define MIN_D1 1.0
#define MAX_D1 1.8
#define M_PI 3.14159265358979323846
#define MIN_D2 0.4
#define MAX_D2 0.9
using namespace std;
using namespace cv;

class Card
{
public:
	Point veryOld;
	static int width;
	static int height;
	bool CanBlock(Card &card);
	bool CanAttack();
	bool gaveMana;
	bool checked;
	bool fresh;
	bool newRound;
	vector<pair<int,int>> upkeepAb;
	void Fight(Card &op,Game &game,int &lifePlayer1,int &lifePlayer2);
	void GiveLifeToPlayer(int value,Game &game);
	CardB cardBase;
	bool ChangedPosition(Point a,Point la,Point b,Point lb,Point c,Point lc,Point d,Point ld)
	{
		int moveSize=10;
		return (Game::Distance(a,la)>moveSize && Game::Distance(b,lb)>moveSize && Game::Distance(c,lc)>moveSize && Game::Distance(d,ld)>moveSize);
	}
	static void DrawValidCard(Mat &img)
	{
		int x,y;
		x=y=100;
		int width = 100 *MIN_D1;
		int height = 75 * MIN_D2;
		Point a(x,y);
		Point b(x+width,y);
		Point c(x+width,y+height);
		Point d(x,y+height);
		rectangle(img,a,c,Scalar(255,0,0));
		imshow("Costam",img);
	}
	void SetTarget(int id);
	int GetTarget();
	int additionalAttack;
	int additionalDefense;
	bool hasFirstStrikeEOT;
	bool hasLifelinkEOT;
	bool hasFlyingEOT;
	bool hasHexproofEOT;
	bool hasFirstStrike;
	bool hasLifelink;
	bool hasFlying;
	bool hasHexproof;

	bool hasCantAttack;
	bool hasCantBlock;
	static void Prepare2(vector<Point>&square,Mat &img);

	bool hasCantUntap;

	bool HasLifelink()
	{
		return (cardBase.hasLifelink || hasLifelink || hasLifelinkEOT);
	}

	bool HasFirstStrike()
	{
		return (cardBase.hasFirstStrike ||hasFirstStrike|| hasFirstStrikeEOT);
	}

	bool HasFlying()
	{
		return (cardBase.hasFlying ||hasFlying || hasFlyingEOT);
	}

	bool HasHexproof()
	{
		return (cardBase.hasHexproof ||hasHexproof || hasHexproofEOT);
	}
	bool HasDeadtuch()
	{
		return cardBase.hasDeatchtuch;
	}
	void Add(int attack,int defense);
	void AddEOT(int attack,int defense);
	int attEOT;
	int defEOT;
	int GetAttack();
	int GetDefense();
	static int ID;
	int id;
	int sendTime;
	bool canUntap;
	bool dead;
	Point old;
	int att;
	int def;
	int owner;
	bool taped;
	void prepareToBlock();
	Point a,b,c,d;
	Point la,lb,lc,ld;
	Mat img;
	static const int TTL=20;
	int ttl;
	void die();
	void Clear();
	bool attack;
	Point2f enemy;
	bool deadSended;
	bool block;
	int blocking;
	void Unlock();
	void setCardBase(CardB &card);
	void NewRound(int player);
	float Compare(Mat &img1,Mat &img2,Game &game);
	Card(Point a, Point b, Point c,Point d,Mat &img,vector<CardB>& bkarty,Game &game,bool temp);
	Card(Point a, Point b, Point c,Point d,vector<CardB>& bkarty,Game &game,int owner,int baseId);
	Card(void);
	bool TapUntap();
	float getAngle();
	void Draw(Mat &img1,Game &game);
	void Update(Point a,Point b,Point c,Point d,Mat &img,vector<CardB>& bkarty,Game &game,bool temp);
	void Update(Point a,Point b,Point c,Point d,int att,int def,Game &game);
	void Update(Point a,Point b,Point c,Point d,Game &game);
	Point2f getCenter();
	bool TrySend(Game &game);
	void Damage(int value);
	static void fastImg(string name,float s)
	{
		Mat *img = new Mat(200,200,CV_64F);
		char cad[100];
		sprintf(cad,"%.2f",s);
		putText(*img,cad, Point(10,10),FONT_HERSHEY_SIMPLEX, 0.5,  Scalar(200,200,200),2);
		imshow(name,*img);

	}

	static float getangle( cv::Point pt1, cv::Point pt2, cv::Point pt0 ) {
		float dx1 = pt1.x - pt0.x;
		float dy1 = pt1.y - pt0.y;
		float dx2 = pt2.x - pt0.x;
		float dy2 = pt2.y - pt0.y;
		return (dx1*dx2 + dy1*dy2)/(float)sqrt((dx1*dx1 + dy1*dy1)*(dx2*dx2 + dy2*dy2));
	}

	static Point2f getCenter(Point a,Point b,Point c,Point d);

	void Tap(Game &game);
	void Untap(Game &game);
	static bool Valid(Point a,Point b,Point c,Point d)
	{
		//float w1 = Distance(a,b);
		//float w2 = Distance(b,c);
		float a1=Distance(a,b)/Distance(b,c);
		float a2=Distance(b,c)/Distance(c,d);

		//if(Card::width!=-1 && Card::height!=-1)
		//{
		//	if(!((w1>=Card::width*0.8f && w1<=Card::width*1.2f && w2>=Card::height*0.8f && w2<=Card::height*1.2f) || (w2>=Card::width*0.8f && w2<=Card::width*1.2f && w1>=Card::height*0.8f && w1<=Card::height*1.2f)))
		//	return false;
		//}

		if(a1>=MIN_D1 && a1<=MAX_D1 && a2>=MIN_D2 && a2<=MAX_D2)
		{
			return true;
		}
		else if(a1>=MIN_D2 && a1<=MAX_D2 && a2>=MIN_D1 && a2<=MAX_D1)
		{
			return true;
		}

		//cout<<"Niepoprawne rozmiary karty:\n"<<a1<<" "<<a2<<endl;
		return false;


	}


	
	static float Distance(Point a,Point b)
	{
		return std::sqrtf(((b.x-a.x)*(b.x-a.x)+(b.y-a.y)*(b.y-a.y)));
	}
	~Card(void);
};