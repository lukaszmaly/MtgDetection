#pragma once
#include <string>
#include "opencv2\imgproc\imgproc.hpp"
#include "opencv2/core/core.hpp"
#include "opencv2/features2d/features2d.hpp"
#include "opencv2/highgui/highgui.hpp"
#include "opencv2/nonfree/nonfree.hpp"
#include <iostream>
#include "settings.h"
#include <vector>
using namespace std;
using namespace cv;


class Mana
{
	public:
	int white;
	int blue;
	int black;
	int red;
	int green;
	int all;

	void CopyMana(int &white,int &blue,int &black,int &red, int &green)
	{
		white=this->white;
		blue=this->blue;
		black=this->black;
		red=this->red;
		green=this->green;
	}
	void Add(Color color)
	{
		switch(color)
		{
		case WHITE:
			Add(1,0,0,0,0);
			break;
		case GREEN:
			Add(0,0,0,0,1);
			break;
		case RED:
			Add(0,0,0,1,0);
			break;
		case BLUE:
			Add(0,1,0,0,0);
			break;
		case BLACK:
			Add(0,0,1,0,0);
		}
	}
	void AddMana(Color color,int value=1)
	{
		switch(color)
		{
		case WHITE:
			this->white+=value;
			this->all+=value;
			break;
		case GREEN:
			this->green+=value;
			this->all+=value;
			break;
		case RED:
			this->red+=value;
			this->all+=value;
			break;
		case BLUE:
			this->blue+=value;
			this->all+=value;
			break;
		case BLACK:
			this->black+=value;
			this->all+=value;
		}
	}
	void SubMana(Color color,int value=1)
	{
		switch(color)
		{
		case WHITE:
			this->white-=value;
			this->all-=value;
			break;
		case GREEN:
			this->green-=value;
			this->all-=value;
			break;
		case RED:
			this->red-=value;
			this->all-=value;
			break;
		case BLUE:
			this->blue-=value;
			this->all-=value;
			break;
		case BLACK:
			this->black-=value;
			this->all-=value;
		}
	}
	bool IsMana(Color color)
	{
		switch(color)
		{
		case WHITE:
			return this->white>0;
			break;
		case GREEN:
			return this->green>0;
			break;
		case RED:
			return this->red>0;
			break;
		case BLUE:
			return this->blue>0;
			break;
		case BLACK:
			return this->black>0;
		}
		return false;
	}
	void Add(int white,int blue,int black,int red,int green)
	{
	
		this->white+=white;
		this->black+=black;
		this->blue+=blue;
		this->green+=green;
		this->red+=red;
		this->all=this->white+this->blue+this->black+this->red+this->green;
	}
	void Print()
	{
	
		cout<<"White: "<<this->white<<endl;
		cout<<"Blue: "<<this->blue<<endl;
		cout<<"Black: "<<this->black<<endl;
		cout<<"Red: "<<this->red<<endl;
		cout<<"GReen: "<<this->green<<endl;
	}
	Mana()
	{
		white=blue=black=red=green=all=0;
	}
	void Sub(int &colorless,int &color)
	{
		if(color>=colorless)
		{
			color-=colorless;
			colorless=0;
		}
		else
		{
			colorless-=color;
			color=0;
		}
	
	}
	bool CanPay(int white, int blue,int black,int red,int green,int colorless)
	{
			if( this->white >= white && this->blue >= blue && this->black >= black && this->red >= red && this->green >= green
			&& all >= (white+blue+black+red+green+colorless) ) return true;
			return false;
	}
	bool Pay(int white, int blue,int black,int red,int green,int colorless)
	{
		if( this->white >= white && this->blue >= blue && this->black >= black && this->red >= red && this->green >= green
			&& all >= (white+blue+black+red+green+colorless) )
		{
			this->white-=white;
			this->black-=black;
			this->blue-=blue;
			this->green-=green;
			this->red-=red;
			if(colorless<=0) return true;
			Sub(colorless,this->white);
			if(colorless<=0) return true;
			Sub(colorless,this->black);
			if(colorless<=0) return true;
			Sub(colorless,this->blue);
			if(colorless<=0) return true;
			Sub(colorless,this->green);
			if(colorless<=0) return true;
			Sub(colorless,this->red);
			return true;
		}
		return false;
	}
};

class Player
{
public:
	int currentMarkerId;
	int secondMarkerId;
	bool recognized;
	bool ChangedMarkersId(int id);
	Point stackB;
	Point stackE;
	Player(void);
	void Init(string name,int markerId,int secondMarkerID);
	Player(string name,int markerId);
	~Player(void);
	int hp;
	int angle;
	bool agree;
	int angleDiff;
	Mat img;
	int oldangle;
	Mana mana;
	string name;
	int markerId;
	bool active;
	void Draw(Mat &frame);

	int operator==(const Player &p) {return  markerId==p.markerId;}
};

