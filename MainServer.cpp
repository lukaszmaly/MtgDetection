#include "MainServer.h"



MainServer::~MainServer(void)
{

}

void MainServer::Init(string adres)
{
	port = 54545;
	ip = adres;
}

void MainServer::HostGame()
{
	//socket.setBlocking(false);
	if (listener.listen(port) != Socket::Done)
		printf("HostGame: nieudana inicjalizacja nas³uchu");
	if (listener.accept(socket) != Socket::Done)
		printf("HostGame: nieudana akcpetacja po³¹czenia");
}

void MainServer::ConnectGame()
{
	if (socket.connect(ip, port) != sf::Socket::Done)
	{
		printf("ConnectGame: nieudane po³aczenie z serwerem");
	}
}

void MainServer::Send(string tresc)
{
	Packet packet; 
	packet << tresc;
	socket.send(packet);
	packet.clear(); 
}
void MainServer::Recv()
{
	string cos;
	Packet packet;
	socket.setBlocking(false);

	while (socket.receive(packet) != Socket::NotReady)
	{
		if (packet >> cos)
		{
			Msg.push_back(cos);
		} 
	} 
}
void MainServer::Recv(vector<string> &Msg)
{
	string cos;
	Packet packet;
	socket.setBlocking(false);

	while (socket.receive(packet) != Socket::NotReady)
	{
		if (packet >> cos)
		{
			Msg.push_back(cos);
		} 
	} 
}


void MainServer::NextPhase(Game &game)
{
	if(game.playerIdInMultiplayerMode==1)
	{
		game.player2.agree=true;
	}
	else if(game.playerIdInMultiplayerMode==2)
	{
		game.player1.agree=true;
	}
}

void MainServer::SendNextPhase()
{
	Send("| NEXTPHASE |");
}

void MainServer::AddCard(int id,int baseId,Point a,Point b,Point c,Point d,vector<Card>&cards,vector<CardB>&bcards,Game &game)
{
	int owner = 1;
	if(game.playerIdInMultiplayerMode==1)	owner=2;
	Card t=Card(a,b,c,d,bcards,game,owner,baseId);
	t.id=id;
	cards.push_back(t);
	game.server.SendNewCard(t.id,t.cardBase.id,t.owner,t.a,t.b,t.c,t.d,t.taped);
}

void MainServer::SendNewCard(int id,int idb,int owner,Point a, Point b,Point c,Point d,bool taped)
{
	ostringstream os;
	os<<"| ADD "<<id<<" "<<idb<<" "<<owner<<" "<<a.x<<" "<<a.y<<" "<<b.x<<" "<<b.y<<" "<<c.x<<" "<<c.y<<" "<<d.x<<" "<<d.y<<" "<<taped<<" |";
	string buffer(os.str());
	Send(buffer);
}

void MainServer::SendReady()
{
	Send("| READY |");
}

void MainServer::Ready(Game &game)
{
	if(game.playerIdInMultiplayerMode==1)
	{
		game.player2Done=true;
	}
	else if(game.playerIdInMultiplayerMode==2)
	{
		game.player1Done=true;
	}
}

void MainServer::SendDead(int id)
{
	ostringstream os;
	os<<"| DEAD "<<id<<" |";
	string buffer(os.str());
	Send(buffer);
}

void MainServer::Dead(int id,vector<Card>&cards,Game &game)
{
	for(unsigned int i=0;i<cards.size();i++)
	{
		if(cards[i].id==id && !cards[i].dead)
		{
			cards[i].die();
			game.server.Dead(id);
		}
	}
}

void MainServer::SendUpdatedCard(int id,int idb,int owner,Point a, Point b,Point c,Point d,bool taped,int att,int def)
{
	ostringstream os;
	os<<"| UPDATE "<<id<<" "<<idb<<" "<<owner<<" "<<a.x<<" "<<a.y<<" "<<b.x<<" "<<b.y<<" "<<c.x<<" "<<c.y<<" "<<d.x<<" "<<d.y<<" "<<taped<<" "<<att<<" "<<def<<" |";
	string buffer(os.str());
	Send(buffer);
}
void MainServer::UpdateCard(int id,Point a,Point b,Point c,Point d,vector<Card>&cards,Game &game,int att,int def)
{
	for(unsigned int i=0;i<cards.size();i++)
	{
		if(cards[i].id==id)
		{
			cards[i].attack=false;
			cards[i].block=false;
			cards[i].Update(a,b,c,d,att,def,game);
		
			//game.server.UpdateCard(cards[i].id,cards[i].cardBase.id,cards[i].owner,a,b,c,d,cards[i].taped,att,def);
		}
	}
}

void MainServer::SendEffect(string effect,int player,int creature)
{
	ostringstream os;
	os<<"| EFFECT "<<effect<<" "<<player<<" "<<creature<<" |";
	string buffer(os.str());
	Send(buffer);
}
void MainServer::Effect(string effect,int player,int creature,Game &game)
{
	game.server.VisualEffect(effect,player,creature);		
}