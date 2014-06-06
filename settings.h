#pragma once
#define CARDWIDTH 115
#define CARDHEIGHT 155
	//tmp.cols=251;
	//	tmp.rows=356;
enum Color{
	WHITE = 0,
	BLUE = 1,
	BLACK = 2,
	RED = 3,
	GREEN = 4,
	NONE = 5
};

enum State
{
	DENY = 0,
	OK = 1,
	NEUTRAL =2

};
enum Actions
{
	PLAYED,
	ATTACK,
	DEFEND,
	SUBLIFE,
	ADDLIFE,
	DEAD,
	NEWGAME,
	STATS,
	ADDDAMAGE
};
enum Phase {
	PIERWSZY = 0,
	ATAK = 1,
	OBRONA = 2,
	WYMIANA = 3,
	DRUGI = 4,
	UPKEEP = 5
};

