#pragma once
#include "opencv2\imgproc\imgproc.hpp"
#include "opencv2/core/core.hpp"
#include "opencv2/features2d/features2d.hpp"
#include "opencv2/highgui/highgui.hpp"
#include "opencv2/nonfree/nonfree.hpp"
#include <iostream>
#include <vector>
#include <string>
#include "settings.h"
using namespace std;
using namespace cv;

enum Type{
	CREATURE = 0,
	LAND = 1,
	INSTANT = 2,
	ENCHANTMENT = 3,
	SORCERY = 4,
	ARTIFACT = 5
};

class CardB
{
public:
	int id;
	Color landColor;

	string name;
	Color color;
	Mat img;
	int att,def;
	Type type;

	bool targetsCreature;
	bool targetsPlayer;

	bool hasHexproof;
	bool hasDeatchtuch;
	bool hasLifelink;
	bool hasDefender;
	bool hasFlying;
	bool hasReach;
	bool hasHaste;
	bool hasFirstStrike;
	string effect;
	vector<pair<int,int>> enterAbilities;
	vector<pair<int,int>> upkeepAbilities;
	vector<pair<int,int>> passiveAbilities;
	void Init(int id,string name,int type,int att,int def,int redCost,int whiteCost,int greenCost,int blackCost,int blueCost,int lessCost,bool hasDefender,bool hasLifelink,bool hasDeatchtuch,bool hasHaste,bool hasFlying,bool hasReach,bool hasFirstStrike,bool hasHexproof,vector<pair<int,int>> enterList,vector<pair<int,int>> upkeepList,vector<pair<int,int>> passiveList,string effect);
	int redCost;
	int blueCost;
	int greenCost;
	int whiteCost;
	int blackCost;
	int lessCost;
	string GetTypeToString();
	CardB(Mat &img1);
	void PrintStats();
	CardB(void);
	~CardB(void);
};

