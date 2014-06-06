#include "Game.h"

bool Game::IsMana(int id,Color color)
{
	if(id==1)
	{
		return this->player1.mana.IsMana(color);
	}
	else
	{
		return this->player2.mana.IsMana(color);
	}
	return false;
}
void Game::AddMana(int id,Color color)
{
	if(id==1)
	{
		this->player1.mana.AddMana(color);
	}
	else
	{
		this->player2.mana.AddMana(color);
	}
	this->server.AddMana(id,color);
}
void Game::Clear()
{
	this->lastId=-1;
	playedLand=false;
}
void Game::SubMana(int id,Color color)
{
	if(id==1)
	{
		this->player1.mana.SubMana(color);
	}
	else
	{
		this->player2.mana.SubMana(color);
	}
	this->server.SubMana(id,color);
}
void Game::AddLife(int id,int value)
{

	if(id==1)
	{
		this->player1.hp+=value;
	}
	else
	{
		this->player2.hp+=value;
	}
	this->server.AddLife(id,value);
	GHAddLife(id,value);
}
void Game::SubLife(int id,int value)
{
	if(id==1)
	{
		this->player1.hp-=value;
	}
	else
	{
		this->player2.hp-=value;
	}
	this->server.SubLife(id,value);
	GHSubLife(id,value);
}
bool Game::CanPay(int id,int white, int blue,int black,int red,int green,int colorless)
{
	if(id==1)
	{
		return this->player1.mana.CanPay(white,blue,black,red,green,colorless);
	}
	else if(id==2)
	{
		return this->player2.mana.CanPay(white,blue,black,red,green,colorless);
	}
	return false;
}
void Game::Pay(int id,int white, int blue,int black,int red,int green,int colorless)
{
	int w,u,b,r,g;
	int aw,au,ab,ar,ag;
	if(id==1)
	{
		this->player1.mana.CopyMana(w,u,b,r,g);
		if(this->player1.mana.CanPay(white,blue,black,red,green,colorless)==true)
		{
			this->player1.mana.Pay(white,blue,black,red,green,colorless);
			this->player1.mana.CopyMana(aw,au,ab,ar,ag);
			this->server.SubMana(id,w-aw,u-au,b-ab,r-ar,g-ag);
			this->ChangeStackState(1,NEUTRAL);
		}
	}
	else
	{
		this->player2.mana.CopyMana(w,u,b,r,g);
		if(this->player2.mana.CanPay(white,blue,black,red,green,colorless)==true)
		{
			this->player2.mana.Pay(white,blue,black,red,green,colorless);
			this->player2.mana.CopyMana(aw,au,ab,ar,ag);
			this->server.SubMana(id,w-aw,u-au,b-ab,r-ar,g-ag);
			this->ChangeStackState(2,NEUTRAL);
		}
	}
}


bool Game::CheckCardsProp()
{
	return checkCardsProp;
}
Player & Game::GetPlayer(int id)
{
	if(id==1)
	{
		return this->player1;
	}
	return this->player2;
}
void Game::setFaza(int i)
{

	switch(i)
	{
	case 0:
		oneAttack=true;
		phase=PIERWSZY;

		break;
	case 1:
		oneAttack=false;
		phase=ATAK;
		break;
	case 2:
		oneAttack=false;
		phase=OBRONA;
		break;
	case 3:
		oneAttack=false;
		phase=WYMIANA;
		break;
	case 4:
		oneAttack=true;
		phase=DRUGI;
		break;
	case 5:
		phase=UPKEEP;
		break;
	}
}
bool Game::IsBgrMode()
{
	return bgrMode;
}

bool Game::IsTargetMode()
{
	return targetMode;
}
void Game::SetTargetMode(bool value)
{
	targetMode=value;
}



string Game::getCurrentPhase()
{
	switch(phase)
	{
	case PIERWSZY:
		return "Faza pierwsza";
		break;
	case ATAK:
		return "Faza ataku";
		break;
	case OBRONA:

		return "Faza obrony";
		break;
	case WYMIANA:
		return "Faza wymiany obrazen";
		break;
	case DRUGI:
		return "Faza druga";
		break;
	case UPKEEP:
		return "Faza upkeepu";
		break;
	}
	return "INCORRECT";
}

void Game::nextPhase()
{

	zmiana=true;
	switch(phase)
	{
	case PIERWSZY:
		phase=ATAK;
		server.NextPhase();
		this->oneAttack=false;
		break;
	case ATAK:
		server.NextPhase();
		this->oneAttack=false;
		phase=OBRONA;
		break;
	case OBRONA:
		server.NextPhase();
		this->oneAttack=false;
		phase=WYMIANA;
		break;
	case WYMIANA:
		server.NextPhase();
		phase=DRUGI;
		this->oneAttack=false;
		break;
	case DRUGI:

		phase=UPKEEP;
		break;
	case UPKEEP:
		server.NextPhase();
		phase = PIERWSZY;
		break;
	}
}

void Game::Draw(Mat &frame)
{
	player1.Draw(frame);
	player2.Draw(frame);
	//imshow("DiffCard",diff);

}

void Game::setPlayer(int i)
{
	aPlayer = player2.markerId;
	if(i==0) aPlayer = player1.markerId;

}

int Game::GetGameHeight()
{
	return gameHeight;
}

int Game::GetGameWidth()
{
	return gameWidth;
}

void Game::Update()
{
	if(player1.agree==true && player2.agree==true)
	{
		player1.agree=false; 
		player2.agree=false;
		nextPhase();
	}

	if(beAbleMarker==false)
	{
		if(abs(targetAngle-targetOldAngle)>45)
		{
			beAbleMarker=true;
			targetOldAngle = targetAngle;
		}
	}
	if(player1.hp<=0)
	{
		gameEnded=true;
		server.SendWinner(2);
	}
		if(player2.hp<=0)
	{
		gameEnded=true;
		server.SendWinner(1);
	}


}

Game::~Game()
{
}

Player& Game::getCurrentPlayer()
{
	if(aPlayer == player1.markerId) return player1;
	return player2;
}

int Game::GetCurrentPlayer()
{
	if(aPlayer == player1.markerId) return 1;
	return 2;
}



Phase Game::GetPhase()
{
	return phase;
}
void Game::SetPlayerPrepared()
{
	if(this->playerIdInMultiplayerMode==1)
	{
		this->player1Done=true;
	}
	else
	{
		this->player2Done=true;
	}
}
bool Game::ArePlayersReady() const
{
	return (player1Done && player2Done);
}
void Game::ChangeStackState(int id,State state)
{
	if(id==1 && state==this->stack1) return;
	else if(id==2 && state==this->stack2) return;
	if(id==1) this->stack1 = state;
	if(id==2) this->stack2 = state;
	this->server.StackColor(id,state);
}
bool Game::IsMultiplayer() const
{
	return this->multiplayerMode;
}

Game::Game(string player1s,int player1Id,int player1SecondId,string player2s,int player2Id,int player2SecondId,string ip,int port,int w,int h,int interval,bool showLog)
{
	useBlur=true;
	gameEnded=false;
	bgrMode=false;
	gameStarted=false;
	player1Done = false;
	player2Done= false;
	player1StackClean=true;
	player2StackClean=true;

	oneLandEachTurn = false;
	playedLand = false;
	stackState=NEUTRAL;
	SetTargetMode(false);
	lastId=-1;
	stack1=NEUTRAL;
	stack2=NEUTRAL;
	firsCardPoint = Point(550,250);
	firstCardHeight = 350;
	firstCardWidth = 250;

	firstCardChecked=false;
	checkCardsProp=true;
	CanResolve = true;
	beAbleMarker=true;
	gameWidth = w;
	oneAttack=false;
	gameHeight = h;
	zmiana = false;
	phase = PIERWSZY;
	t=false;
	server.Init(ip,port,interval,showLog);
	player1.Init(player1s,player1Id,player1SecondId);
	player2.Init(player2s,player2Id,player2SecondId);
	aPlayer = player1.markerId;
	server.ActivePlayer(1);
	
}

int Game::distance(Point a,Point b)
{
	return std::sqrtf(((b.x-a.x)*(b.x-a.x)+(b.y-a.y)*(b.y-a.y)));
}
