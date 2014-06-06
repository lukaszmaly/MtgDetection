#include "CardB.h"


CardB::CardB(void)
{
}


CardB::~CardB(void)
{
}

string CardB::GetTypeToString()
{
	switch(this->type)
	{
	case CREATURE:
		return "Creature";
		break;
	case ENCHANTMENT:
		return "Enchantment";
		break;
	case SORCERY:
		return "Sorcery";
		break;
	case INSTANT:
		return "Instant";
		break;
	case ARTIFACT:
		return "Artifact";
		break;
			case LAND:
		return "Land";
		break;
		
	}
	return "NONE";
}

void CardB::PrintStats()
{
	cout<<"----"<<endl;
	cout<<"Id: "<< id<<endl;
	cout<<"Nazwa: "<<name<<endl;
	cout<<"Typ: "<<GetTypeToString()<<endl;
	if(blueCost>0) cout<<"Blue: "<<blueCost<<endl;
	if(greenCost>0) cout<<"Green: "<<greenCost<<endl;
	if(blackCost>0) cout<<"Black: "<<blackCost<<endl;
	if(whiteCost>0) cout<<"White: "<<whiteCost<<endl;
	if(redCost>0) cout<<"Red: "<<redCost<<endl;
	if(lessCost>0) cout<<"Colorless: "<<lessCost<<endl;
	cout<<"Static abilities: ";
	if(hasHexproof) cout<<"Hexproof";
	if(hasDeatchtuch) cout<<"Deatchtuch";
	if(hasLifelink) cout<<"Lifelink";
	if(hasDefender) cout<<"Defender";
	if(hasFlying) cout<<"Flying";
	if(hasReach) cout<<"Reach";
	if(hasHaste) cout<<"Haste";
	if(hasFirstStrike) cout<<"FirstStrike";
	cout<<endl;
	cout<<"Enter battlefield abb: "<<enterAbilities.size()<<endl;
	for(int i=0;i<enterAbilities.size();i++)
	{
		cout<<i<<"-ty "<< enterAbilities[i].first<<" "<<enterAbilities[i].second<<endl;
	}
	cout<<"Upkeep  abb: "<<upkeepAbilities.size()<<endl;
	cout<<"Passive abb: "<<passiveAbilities.size()<<endl;
	if(effect.compare("NONE")!=0)
	cout<<"Effect: "<<effect<<endl;
}


void CardB::Init(int id,string name,int type,int att,int def,int redCost,int whiteCost,int greenCost,int blackCost,int blueCost,int lessCost,bool hasDefender,bool hasLifelink,bool hasDeatchtuch,bool hasHaste,bool hasFlying,bool hasReach,bool hasFirstStrike,bool hasHexproof,vector<pair<int,int>> enterList,vector<pair<int,int>> upkeepList,vector<pair<int,int>> passiveList,string effect)
{

	this->id=id-1;
	this->name=name;

	switch(type)
	{
	case 0:
		this->type=CREATURE;
		break;
	case 1:
		this->type=ENCHANTMENT;
		break;
	case 2:
		this->type=LAND;
		break;
	case 3:
		this->type=INSTANT;
		break;
	case 4:
		this->type=SORCERY;
		break;
	case 5:
		this->type= ARTIFACT;
		break;
	}

	this->att=att;
	this->def=def;
	this->redCost=redCost;
	this->whiteCost=whiteCost;
	this->greenCost=greenCost;
	this->blueCost=blueCost;
	this->blackCost=blackCost;
	this->lessCost=lessCost;
	landColor=NONE;
	if(redCost==-1) landColor = RED;
	if(blueCost==-1) landColor = BLUE;
	if(blackCost==-1) landColor = BLACK;
	if(whiteCost==-1) landColor = WHITE;
	if(greenCost==-1) landColor = GREEN;


	this->hasHexproof=hasHexproof;
	this->hasHaste= hasHaste;
	this->hasDefender=hasDefender;
	this->hasDeatchtuch=hasDeatchtuch;
	this->hasFlying=hasFlying;
	this->hasReach=hasReach;
	this->hasFirstStrike=hasFirstStrike;
	this->hasLifelink=hasLifelink;
	this->enterAbilities=enterList;
	this->upkeepAbilities=upkeepList;
	this->passiveAbilities=passiveList;
	this->effect=effect;
}

CardB::CardB(Mat &img)
{
	this->img = img;
	if(!this->img.data)
	{
		cout<<"File does not exist!"<<endl;
	}
}

