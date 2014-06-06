#include "Server.h"

///Czasami usuwa karte z karty, popraw
//popraw czas wysy³ania wiadomoœci

void Server::SendWinner(int owner)
{
	char data[100];
	int n = sprintf_s(data,"| WINNER %d |",owner);	

	if(soc.send(data, n, client, port) != sf::Socket::Done)
	{
		cout<<"Blad podczas wysylanie danych o nowej karcie"<<endl;
	}
	else if(showLog)
	{
		cout<< data <<endl;Write(data);
	}
}

void Server::Agree(int owner)
{
	char data[100];
	int n = sprintf_s(data,"| AGREE %d |",owner);	

	if(soc.send(data, n, client, port) != sf::Socket::Done)
	{
		cout<<"Blad podczas wysylanie danych o nowej karcie"<<endl;
	}
	else if(showLog)
	{
		cout<< data <<endl;Write(data);
	}
}
void Server::Damage(int id1,int value1)
{
	char data[100];
	int n = sprintf_s(data,"| DAMAGE %d %d |",id1,value1);	

	if(soc.send(data, n, client, port) != sf::Socket::Done)
	{
		cout<<"Blad podczas wysylanie danych o nowej karcie"<<endl;
	}
	else if(showLog)
	{
		cout<< data <<endl;Write(data);
	}
}
void Server::VisualEffect(string id,int player,int creature)
{
	if(id.compare("NONE")==0) return;
	char data[100];
	int n = sprintf_s(data,"| EFFECT %s %d %d |",id.c_str(),player,creature);	

	if(soc.send(data, n, client, port) != sf::Socket::Done)
	{
		cout<<"Blad podczas wysylanie danych o nowej karcie"<<endl;
	}
	else if(showLog)
	{
		cout<< data <<endl;Write(data);
	}
}
void Server::Write(const char tab[100])
{
	file << tab << endl;
}
void Server::SetInterval(int value)
{
	this->interval=value;
}

void Server::Markers(int w,int h,bool isMultiplayer, int idInMultiplayer)
{	
	char data[100];
	char s = isMultiplayer ? 'M' : 'S';

	int n = sprintf_s(data,"| MARKERS %d %d %c %d |",w,h,s,idInMultiplayer);	

	if(soc.send(data, n, client, port) != sf::Socket::Done)
	{
		cout<<"Blad podczas wysylanie danych o nowej karcie"<<endl;
	}
	else
	{
		cout<< data <<endl;Write(data);
	}
}
void Server::Played(int id)
{	
	char data[100];
	int n = sprintf_s(data,"| PLAYED %d |",id);	

	if(soc.send(data, n, client, port) != sf::Socket::Done)
	{
		cout<<"Blad podczas wysylanie danych o nowej karcie"<<endl;
	}
	else if(showLog)
	{
		cout<< data <<endl;Write(data);
	}
}

void Server::Init(string ip,int port,int interval,bool showLog)
{
	client = ip;
	this->port = port;
	this->interval = interval;
	this->showLog=showLog;
	if(soc.bind(port) != Socket::Done)
	{
		cout<<"Blad podczas tworzenia socketa"<<endl;

	}
	else
	{
		Write("Utworzono serwer");
		cout<<"Utworzono serwer UDP na porcie "<<port<<" z interval = "<<this->interval<<endl;
	}
}

Server::Server()
{
	file.open( "log.txt", std::ios::in | std::ios::out | std::ios::app);
}
void Server::Start()
{
	char data[100];
	int n = sprintf(data,"| START |");	
	if (soc.send(data, n, client, port) != sf::Socket::Done)
	{
		cout<<"Blad podczas wysylanie aktualnych danych o karcie"<<endl;
	}
	else if(showLog)
	{
		cout<< data <<endl;Write(data);
	}
}
void Server::Send(string s)
{
	char data[100];
	int n = sprintf(data,"%s",s.c_str());	
	if (soc.send(data, n, client, port) != sf::Socket::Done)
	{
		cout<<"Blad podczas wysylanie aktualnych danych o karcie"<<endl;
	}
	else if(showLog)
	{
		cout<< data <<endl;Write(data);
	}
}
void Server::CardOnStack(int owner,int id,int state)
{
	char data[100];
	int n = sprintf(data,"| CARDONSTACK %d %d %d |",id,owner,state);	
	if (soc.send(data, n, client, port) != sf::Socket::Done)
	{
		cout<<"Blad podczas wysylanie aktualnych danych o karcie"<<endl;
	}
	else if(showLog)
	{
		cout<< data <<endl;Write(data);
	}
}

void Server::DrawCard(int id,int value)
{
	char data[100];
	int n = sprintf(data,"| DRAW %d %d |",id,value);	
	if (soc.send(data, n, client, port) != sf::Socket::Done)
	{
		cout<<"Blad podczas wysylanie aktualnych danych o karcie"<<endl;
	}
	else if(showLog)
	{
		cout<< data <<endl;Write(data);
	}
}
void Server::Attack(int id,int idb,int gracz,Point a, Point b,Point c,Point d,bool taped,int att,int def)
{
	char data[100];

	int n = sprintf(data,"| ATTACK %d %d %d %d %d %d %d %d %d %d %d %d %d %d |",id,idb,gracz,taped,a.x,a.y,b.x,b.y,c.x,c.y,d.x,d.y,att,def);	
	if (soc.send(data, n, client, port) != sf::Socket::Done)
	{
		cout<<"Blad podczas wysylanie aktualnych danych o karcie"<<endl;
	}
	else if(showLog)
	{
		cout<< data <<endl;Write(data);
	}

}
void Server::Scry(int id,int value)
{
	char data[100];
	int n = sprintf_s(data,"| SCRY %d %d |",id,value);	

	if(soc.send(data, n, client, port) != sf::Socket::Done)
	{
		cout<<"Blad podczas wysylanie danych o nowej karcie"<<endl;
	}
	else if(showLog)
	{
		cout<< data <<endl;Write(data);
	}
}

void Server::PutPermanentOnLibrary(int id)
{
	char data[100];
	int n = sprintf_s(data,"| PUTONLIB %d |",id);	

	if(soc.send(data, n, client, port) != sf::Socket::Done)
	{
		cout<<"Blad podczas wysylanie danych o nowej karcie"<<endl;
	}
	else if(showLog)
	{
		cout<< data <<endl;Write(data);
	}
}

void Server::AddMana(int id,Color color,int count)
{
	char data[100];
	string a = "X";
	switch(color)
	{
	case WHITE:
		a="W";
		break;
	case GREEN:
		a="G";
		break;
	case RED:
		a="R";
		break;
	case BLUE:
		a="U";
		break;
	case BLACK:
		a="B";
	}
	int n = sprintf_s(data,"| ADDMANA %d %s %d |",id,a.c_str(),count);	

	if(soc.send(data, n, client, port) != sf::Socket::Done)
	{
		cout<<"Blad podczas wysylanie danych o nowej karcie"<<endl;
	}
	else if(showLog)
	{
		cout<< data <<endl;Write(data);
	}
}

void Server::SubMana(int id,Color color,int count)
{
	char data[100];
	string a;
	switch(color)
	{
	case WHITE:
		a="W";
		break;
	case GREEN:
		a="G";
		break;
	case RED:
		a="R";
		break;
	case BLUE:
		a="U";
		break;
	case BLACK:
		a="B";
	}
	int n = sprintf_s(data,"| SUBMANA %d %s %d |",id,a.c_str(),count);	

	if(soc.send(data, n, client, port) != sf::Socket::Done)
	{
		cout<<"Blad podczas wysylanie danych o nowej karcie"<<endl;
	}
	else if(showLog)
	{
		cout<< data <<endl;Write(data);
	}
}

void Server::SubMana(int id,int white,int blue,int black,int red,int green)
{
	char data[100];
	int n = sprintf_s(data,"| SUBMANA2 %d %d %d %d %d %d |",id,white,blue,black,red,green);	

	if(soc.send(data, n, client, port) != sf::Socket::Done)
	{
		cout<<"Blad podczas wysylanie danych o nowej karcie"<<endl;
	}
	else if(showLog)
	{
		cout<< data <<endl;Write(data);
	}
}

void Server::SubLife(int id,int count)
{
	char data[100];
	int n = sprintf_s(data,"| SUBLIFE %d %d |",id,count);	

	if(soc.send(data, n, client, port) != sf::Socket::Done)
	{
		cout<<"Blad podczas wysylanie danych o nowej karcie"<<endl;
	}
	else if(showLog)
	{
		cout<< data <<endl;Write(data);
	}
}

void Server::AddLife(int id,int count)
{
	char data[100];
	int n = sprintf_s(data,"| ADDLIFE %d %d |",id,count);	

	if(soc.send(data, n, client, port) != sf::Socket::Done)
	{
		cout<<"Blad podczas wysylanie danych o nowej karcie"<<endl;
	}
	else if(showLog)
	{
		cout<< data <<endl;Write(data);
	}
}

void Server::StackColor(int id,State state)
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
	char data[100];
	int n = sprintf_s(data,"| STACK %d %s |",id,st.c_str());	

	if(soc.send(data, n, client, port) != sf::Socket::Done)
	{
		cout<<"Blad podczas wysylanie danych o nowej karcie"<<endl;
	}
	else if(showLog)
	{
		cout<< data <<endl;Write(data);
	}

}
void Server::Cost(int id,int white,int blue,int black,int red,int green)
{
	char data[100];
	int n = sprintf_s(data,"| COST %d %d %d %d %d %d |",id,white,blue,black,red,green);	

	if(soc.send(data, n, client, port) != sf::Socket::Done)
	{
		cout<<"Blad podczas wysylanie danych o nowej karcie"<<endl;
	}
	else if(showLog)
	{
		cout<< data <<endl;Write(data);
	}
}
void Server::NextPhase()
{
	char data[100];
	int n = sprintf_s(data,"| NEXTPHASE |");	

	if(soc.send(data, n, client, port) != sf::Socket::Done)
	{
		cout<<"Blad podczas wysylanie danych o nowej karcie"<<endl;
	}
	else if(showLog)
	{
		cout<< data <<endl;Write(data);
	}
}

void Server::Dead(int id)
{
	char data[100];
	int n = sprintf_s(data,"| DEAD %d |",id);	

	if(soc.send(data, n, client, port) != sf::Socket::Done)
	{
		cout<<"Blad podczas wysylanie danych o nowej karcie"<<endl;
	}
	else if(showLog)
	{
		cout<< data <<endl;Write(data);
	}

}
void Server::ActivePlayer(int id)
{
	char data[100];
	int n = sprintf_s(data,"| ACTIVEPLAYER %d |",id);	

	if(soc.send(data, n, client, port) != sf::Socket::Done)
	{
		cout<<"Blad podczas wysylanie danych o nowej karcie"<<endl;
	}
	else if(showLog)
	{
		cout<< data <<endl;Write(data);
	}

}
void Server::Block(int id,int idb,int gracz,Point a, Point b,Point c,Point d,bool taped,int id2,int att,int def)
{
	char data[100];

	int n = sprintf(data,"| BLOCK %d %d %d %d %d %d %d %d %d %d %d %d %d %d %d |",id,idb,gracz,taped,a.x,a.y,b.x,b.y,c.x,c.y,d.x,d.y,id2,att,def);	
	if (soc.send(data, n, client, port) != sf::Socket::Done)
	{
		cout<<"Blad podczas wysylanie aktualnych danych o karcie"<<endl;
	}
	else if(showLog)
	{
		cout<< data <<endl;Write(data);
	}

}


void Server::SendNewCard(int id,int idb,int gracz,Point a, Point b,Point c,Point d,bool taped)
{
	char data[100];
	int n = sprintf_s(data,"| ADD %d %d %d %d %d %d %d %d %d %d %d %d |",id,idb,gracz,taped,a.x,a.y,b.x,b.y,c.x,c.y,d.x,d.y);	
	if(soc.send(data, n, client, port) != sf::Socket::Done)
	{
		cout<<"Blad podczas wysylanie danych o nowej karcie"<<endl;
	}
	else if(showLog)
	{
		cout<< data <<endl;Write(data);
	}

}

void Server::UpdateCard(int id,int idb,int gracz,Point a, Point b,Point c,Point d,bool taped,int att,int def)
{
	char data[100];

	int n = sprintf(data,"| UPDATE %d %d %d %d %d %d %d %d %d %d %d %d %d %d |",id,idb,gracz,taped,a.x,a.y,b.x,b.y,c.x,c.y,d.x,d.y,att,def);	
	if (soc.send(data, n, client, port) != sf::Socket::Done)
	{
		cout<<"Blad podczas wysylanie aktualnych danych o karcie"<<endl;
	}
	else if(showLog)
	{
		cout<< data <<endl;Write(data);
	}

}
int Server::GetInterval()
{
	return interval;
}

void Server::AddPlayer(int id,const char name[])
{	
	char data[100];
	int n = sprintf(data,"| PLAYER %d %s |",id,name);	
	if (soc.send(data, n, client, port) != sf::Socket::Done)
	{
		cout<<"Nie wyslano"<<endl;
	}
	else if(showLog)
	{
		cout<<data<<endl;Write(data);
	}
}

Server::~Server()
{
	soc.unbind();

	Write("Zamknieto serwer");
	file.close();
}