#include "Card.h"

Card::Card(void)
{

}

Card::~Card(void)
{

}
void Card::Damage(int value)
{
	cout<<"Damage"<<endl;
	this->defEOT-=value;
	if(GetDefense()<=0) this->die();
}



void Card::Prepare2(vector<Point>&square,Mat &img)
{
	Point a=square[0],b=square[1],c=square[2],d=square[3];
	Point center = Card::getCenter(a,b,c,d);
	if(Game::GetAngle(a,b,center)<0)
	{
		Point t = b;
		b = d;
		d = t;
	}
	// w poprawnym kierunku
	if(Game::GetAngle(a,b,center) >Game::GetAngle(b,c,center))
	{
		Point t = a;
		a = b;
		b = c;
		c = d;
		d = t;
	}
	//Poprawne krawedzie 

	Mat tmp;
	Mat t;
	img.copyTo(t);
	tmp.cols=200;
	tmp.rows=400;
	Point2f c1[4] = {a,b,c,d};
	Point2f c2[4] = {Point2f(0,0), Point2f(251,0), Point2f(251,356),Point2f(0,356)};
	Mat mmat(3,3,CV_32FC1);
	mmat=getAffineTransform(c1,c2);
	cv::warpAffine(t,tmp,mmat,Size(251,356));

	int tab1[3]={0,0,0};
	int tab2[3]={0,0,0};
	int tab3[3]={0,0,0};
	int width=tmp.cols;
	int channels=tmp.channels();
	int height=tmp.rows;

	for(int y=10;y<height/2-10;y++)
	{
		for(int x=0;x<width;x++)
		{
			tab1[0]+=tmp.data[channels*(width*y + x)];
			tab1[1]+=tmp.data[channels*(width*y + x) +1];
			tab1[2]+=tmp.data[channels*(width*y + x) +2];
		}
	}

	for(int y=height/2+10;y<height-10;y++)
	{
		for(int x=0;x<width;x++)
		{
			tab2[0]+=tmp.data[channels*(width*y + x)];
			tab2[1]+=tmp.data[channels*(width*y + x) +1];
			tab2[2]+=tmp.data[channels*(width*y + x) +2];
		}
	}
	tab3[0]=tab2[0]-tab1[0];
	tab3[1]=tab2[1]-tab1[1];
	tab3[2]=tab2[2]-tab1[2];

	//jeœli r jest mniejsze od 2 to góra kart jest dobrze okreslona
	int r=0;
	if(tab3[0]<0) r++;
	if(tab3[1]<0) r++;
	if(tab3[2]<0) r++;
	if(r>=2)
	{
		swap(a,c);
		swap(d,b);
	}
	square[0] = a;
	square[1] = b;
	square[2] = c;
	square[3] = d;
}



float Card::getAngle()
{
	return atan2f(( a.y - getCenter().y ),( a.x - getCenter().x ) ) * 180 / M_PI + 180;
}

float Card::Compare(Mat &img1,Mat &img2,Game &game)
{
	if(img1.data && img2.data)
	{
	
		int width=img1.cols;
		int height=img1.rows;
		int n=width*height;
		int channels=img1.channels();
		long int red=0,green=0,blue=0;
		int red2=0,green2=0,blue2=0;
		unsigned int wsk=0;

	
		for(int y=0;y<height;y++)
		{
			for(int x=0;x<width;x++)
			{
				wsk=channels*(width*y + x);
				blue=img1.data[wsk]-img2.data[wsk];
				blue2+=blue*blue;

				green=img1.data[wsk+1]-img2.data[wsk+1];
				
				green2+=green*green;
				red=img1.data[wsk+2]-img2.data[wsk +2];
				
				red2+=red*red;
			}
		}
		
		return (sqrtf(0.2*red2)+sqrtf(0.4*green2)+sqrtf(0.4*blue2))/(float)n;
	}
	return 9999999;
}


void Card::Unlock()
{
	ID++;
	if(owner==1) id=ID;
	else if(owner==2) id=1000+ID;
}

int Card::GetAttack()
{
	if(this->cardBase.type==LAND) return -1;
	return max(0,(att+attEOT+additionalAttack));
}
int Card::GetDefense()
{
	if(this->cardBase.type==LAND) return -1;
	return max(0,(def+defEOT+additionalDefense));
}

void Card::AddEOT(int attack,int defense)
{
	attEOT+=attack;
	defEOT+=defense;
	if(GetDefense()<=0) die();
}
void Card::Add(int attack,int defense)
{
	additionalAttack+=attack;
	additionalDefense+=defense;
	if(GetDefense()<=0) die();
}

Card::Card(Point a, Point b, Point c,Point d,vector<CardB>& bkarty,Game &game,int owner,int baseId)
{

	fresh = true;
	newRound=true;
	attEOT=defEOT=0;
	additionalAttack=additionalDefense=0;

	deadSended=false;
	canUntap=true;
		hasLifelink=hasLifelinkEOT=hasFlying=hasFlyingEOT=hasFirstStrike=hasFirstStrikeEOT=false;

	sendTime=0;
	id=-1;
	blocking=-1;
	this->owner=owner;	
	taped=false;
	attack=block=false;
	this->a=a;
	old=Point(-1,-1);
	this->b=b;
	this->c=c;
	this->d=d;
	ttl=TTL;
	veryOld=getCenter();
	hasCantAttack=hasCantBlock=false;
	att=-1;
	def=-1;
	dead=false;
	
	for(int i=0;i<bkarty.size();i++)
	{
		if(bkarty[i].id==baseId)
		{
			setCardBase(bkarty[i]);
			break;
		}
	}

	cardBase.type==LAND ? gaveMana=false : gaveMana=true;
	old=getCenter();
}

Card::Card(Point a, Point b, Point c,Point d,Mat &img,vector<CardB>& bkarty,Game &game,bool temp=false)
{
	fresh = true;
	checked=false;
	newRound=true;
	attEOT=defEOT=0;
	additionalAttack=additionalDefense=0;

	deadSended=false;
	canUntap=true;
	hasLifelink=hasLifelinkEOT=hasFlying=hasFlyingEOT=hasFirstStrike=hasFirstStrikeEOT=false;
	sendTime=0;
	id=-1;
	blocking=-1;
	owner=game.GetCurrentPlayer();
	taped=false;
	attack=block=false;
	
	old=Point(-1,-1);
	this->a=a;
	this->b=b;
	this->c=c;
	this->d=d;
	this->la=a;
	this->lb=b;
	this->lc=c;
	this->ld=d;
	ttl=TTL;
	hasCantAttack=hasCantBlock=false;
	att=-1;
	def=-1;
	dead=false;
	//Update(a,b,c,d,img,bkarty,game,temp);
	cardBase.type==LAND ? gaveMana=false : gaveMana=true;
	old=getCenter();
}
bool Card::TrySend(Game &game)
{
	sendTime++;
	if(sendTime>=game.server.GetInterval())
	{
		sendTime=0;
		return true;
	}
	return false;
}

Point2f Card::getCenter()
{
	Point2f t(0,0);
	t.x=t.x+a.x+b.x+c.x+d.x;
	t.y=t.y+a.y+b.y+c.y+d.y;
	t.x/=(float)4;
	t.y/=(float)4;
	return t;
}

Point2f Card::getCenter(Point a,Point b,Point c,Point d)
{
	Point2f t(0,0);
	t.x=t.x+a.x+b.x+c.x+d.x;
	t.y=t.y+a.y+b.y+c.y+d.y;
	t.x/=(float)4;
	t.y/=(float)4;
	return t;
}

bool Card::TapUntap()
{
	bool taptemp = true;
	float t = getAngle();
	if((t > 180 && t < 270) || (t > 0 && t < 90))	taptemp = false;
	if(taptemp != taped) return true;
	return false;
}

void Card::Update(Point a,Point b,Point c,Point d,Mat &img,vector<CardB>& bkarty,Game &game,bool temp=false)
{
	if(dead==false)ttl=TTL;
	if(temp == false)
	{
		this->a=a;
		this->b=b;
		this->c=c;
		this->d=d;
		if(TapUntap()==true)
		{
			if(taped==true)
			{
				Untap(game);
			}
			else
			{
				Tap(game);
			}
		}

		if(game.GetPhase()==PIERWSZY || game.GetPhase()==DRUGI) {old=getCenter(); enemy=Point(-1,-1);}
	}

if(temp==true)
{
	Mat tmp;
	Mat t;
	img.copyTo(t);
	tmp.cols=251;
	tmp.rows=356;
	Point2f c1[4] = {a,b,c,d};
	Point2f c2[4] = {Point2f(0,0), Point2f(251,0), Point2f(251,356),Point2f(0,356)};
	Mat mmat(3,3,CV_32FC1);
	mmat=getAffineTransform(c1,c2);
	warpAffine(t,tmp,mmat,Size(251,356));
	float min=256*256;
	int cardId=-1;
		if(game.useBlur) GaussianBlur(tmp,tmp,Size(15,15),0);
		if(game.IsBgrMode()==false)
			cvtColor(tmp,tmp,COLOR_BGR2HSV);
	
	for(unsigned int i=0;i<bkarty.size();i++)
	{
		
		float t1=Compare(tmp,bkarty[i].img,game);
		if(t1<min) { cardId=i;min=t1;}
	}
	if(cardId!=-1)
		setCardBase(bkarty[cardId]);
}
}

void Card::Update(Point a,Point b,Point c,Point d,int att,int def,Game &game)
{
	veryOld=getCenter();
	this->a=a;
	this->b=b;
	this->c=c;
	this->d=d;
	this->additionalAttack=this->attEOT=0;
	this->att=att;
	this->additionalDefense=this->defEOT=0;
	this->def=def;
	if(dead==false)ttl=TTL;

	if(TapUntap()==true)
	{
		if(taped==true)
		{
			Untap(game);
		}
		else
		{
			Tap(game);
		}
	}

	if(game.GetPhase()==PIERWSZY || game.GetPhase()==DRUGI) {old=getCenter(); enemy=Point(-1,-1);}
}
void Card::Update(Point a,Point b,Point c,Point d,Game &game)
{
	this->a=a;
	this->b=b;
	this->c=c;
	this->d=d;
	if(dead==false)ttl=TTL;

	if(TapUntap()==true)
	{
		if(taped==true)
		{
			Untap(game);
		}
		else
		{
			Tap(game);
		}
	}

	if(game.GetPhase()==PIERWSZY || game.GetPhase()==DRUGI) {old=getCenter(); enemy=Point(-1,-1);}
}
void Card::setCardBase(CardB &card)
{
	this->cardBase=card;
	att=card.att;
	def=card.def;
}

void Card::Tap(Game &game)
{
	cout<<"Tapnieto"<<endl;
	taped=true;
	if(this->cardBase.type==LAND && canUntap==true)
	{
		game.AddMana(this->owner,this->cardBase.landColor);
	}
	newRound=false;
	game.GHTap(this->id);
}

void Card::Untap(Game &game)
{
	cout<<"Odtapowano"<<endl;
	taped=false;
	if(this->cardBase.type==LAND)
	{
		if(game.IsMana(this->owner,this->cardBase.landColor))
		{
			game.SubMana(this->owner,this->cardBase.landColor);
		}
		else
		{
			canUntap=false;
		}
		if(newRound==true)
			canUntap=true;
		game.GHTap(this->id);
	}


}

void Card::die()
{
	dead=true;
	ttl=2*TTL;
}

void Card::GiveLifeToPlayer(int value,Game &game)
{
	game.AddLife(owner,value);
}

void Card::Fight(Card &op,Game &game,int &lp1,int &lp2)
{
	lp1=lp2=0;
	//this - karta atakuj¹ca
	this->canUntap=false;

	int tempDef=op.GetDefense();
	if((this->HasFirstStrike() && op.HasFirstStrike()) || (!this->HasFirstStrike() && !op.HasFirstStrike()))
	{

		Damage(op.GetAttack());
		op.Damage(GetAttack());
		game.GHAddDamage(op.id,GetAttack());
		game.GHAddDamage(id,op.GetAttack());
		game.server.Damage(op.id,GetAttack());
		game.server.Damage(id,op.GetAttack());
		if(this->HasLifelink())	{this->GiveLifeToPlayer(this->GetAttack(),game); lp1+=this->GetAttack();}
		if(op.HasLifelink())	{op.GiveLifeToPlayer(op.GetAttack(),game); lp2+=op.GetAttack();}

		if(GetDefense()<=0 || op.HasDeadtuch()) die();
		if(op.GetDefense()<=0 || this->HasDeadtuch()) 
		{
			op.die();
		}
	}
	else if(this->cardBase.hasFirstStrike)
	{
		op.Damage(GetAttack());
		game.GHAddDamage(op.id,GetAttack());
	
		if(this->cardBase.hasLifelink)	{this->GiveLifeToPlayer(this->GetAttack(),game);lp1+=this->GetAttack();}
		if(op.GetDefense()<=0 || this->cardBase.hasDeatchtuch) 
		{
			game.server.Damage(op.id,GetAttack());

			op.die(); 
		}
		else
		{
			Damage(op.GetAttack());
				game.GHAddDamage(id,op.GetAttack());
			game.server.Damage(op.id,GetAttack());
		game.server.Damage(id,op.GetAttack());
		if(op.cardBase.hasLifelink)	{op.GiveLifeToPlayer(op.GetAttack(),game); lp2+=op.GetAttack();}
			if(GetDefense()<=0 || op.cardBase.hasDeatchtuch) die();
		}
	}
	else if(op.cardBase.hasFirstStrike)
	{
		Damage(op.GetAttack());
			game.GHAddDamage(id,op.GetAttack());
		if(op.cardBase.hasLifelink)	{op.GiveLifeToPlayer(op.GetAttack(),game); lp2+=op.GetAttack();}
		if(GetDefense()<=0 || op.cardBase.hasDeatchtuch) 
		{
			
			game.server.Damage(id,op.GetAttack());
			die(); 
		}
		else
		{
			op.Damage(GetAttack());
				game.GHAddDamage(op.id,GetAttack());
			game.server.Damage(op.id,GetAttack());
			game.server.Damage(id,op.GetAttack());
			if(this->cardBase.hasLifelink)	{this->GiveLifeToPlayer(this->GetAttack(),game); lp1+=this->GetAttack();}
			if(op.GetDefense()<=0 || this->cardBase.hasDeatchtuch) die();
		}
	}

	Clear();
	op.Clear();
}

void Card::Draw(Mat &img1,Game &game)
{
	if(--ttl>=0)
	{
		char cad[100];
		char cad1[100];
		char cad2[100];
		line(img1,a,b,Scalar(0,0,255),2);
		line(img1,b,c,Scalar(0,0,255),2);
		line(img1,c,d,Scalar(0,0,255),2);
		line(img1,d,a,Scalar(0,0,255),2);
		putText(img1,"a", Point(a.x,a.y),FONT_HERSHEY_SIMPLEX, 0.5,  Scalar(0,0,255),2);
		putText(img1,"b", Point(b.x,b.y),FONT_HERSHEY_SIMPLEX, 0.5,  Scalar(0,0,255),2);
		putText(img1,"c", Point(c.x,c.y),FONT_HERSHEY_SIMPLEX, 0.5,  Scalar(0,0,255),2);
		putText(img1,"d", Point(d.x,d.y),FONT_HERSHEY_SIMPLEX, 0.5,  Scalar(0,0,255),2);

		sprintf(cad2,"ID:%d",id);
		putText(img1,cad2, Point(d.x,d.y)+Point(0,20),FONT_HERSHEY_SIMPLEX, 0.5,  Scalar(0,0,255),2);




		sprintf(cad,"%s",cardBase.name.c_str());


		if(taped==true)
			sprintf(cad1,"Taped(%s)",game.GetPlayer(this->owner).name.c_str());
		else
			sprintf(cad1,"Untaped(%s)",game.GetPlayer(this->owner).name.c_str());



		putText(img1,cad, a+Point(10,15),FONT_HERSHEY_SIMPLEX, 0.5,  Scalar(0,0,255),2);		
		putText(img1,cad1, a+Point(10,30),FONT_HERSHEY_SIMPLEX, 0.5,Scalar(0,0,255),2);		
		if(att!=-1)
		{
			char cad3[100];
			sprintf(cad3,"Att: %d",GetAttack());
			putText(img1,cad3, d-Point(0,20),FONT_HERSHEY_SIMPLEX, 0.5,  Scalar(0,0,255),2);	
		}

		if(def!=-1)
		{
			char cad4[100];
			sprintf(cad4,"CanUntap: %d ",this->canUntap);
			putText(img1,cad4, c-Point(0,20),FONT_HERSHEY_SIMPLEX, 0.5,  Scalar(0,0,255),2);	
		}


		if(game.GetPhase()==ATAK || (game.GetPhase()==OBRONA && attack==true))
		{
			if(Distance(getCenter(),old)>50 && attack==false)
			{
				putText(img1,"Tapnij zeby zaatakowac",getCenter()-Point2f(0,50),FONT_HERSHEY_SIMPLEX, 0.5,  Scalar(0,200,200),4);
				line(img1,old,getCenter(),Scalar(255,0,0),3);
			}

			if(attack==true) {
				line(img1,old,getCenter(),Scalar(255,0,0),3); putText(img1,"Atakuje",getCenter()+Point2f(0,50),FONT_HERSHEY_SIMPLEX, 0.5,  Scalar(0,0,255),2);	



			}
		}

		if(game.GetPhase()==OBRONA)
		{
			if(block==true)
			{
				line(img1,old,getCenter(),Scalar(255,0,0),3);
				putText(img1,"Bronie",getCenter()+Point2f(0,50),FONT_HERSHEY_SIMPLEX, 0.5,  Scalar(0,0,255),2);	
				if(enemy.x!=-1) {
					line(img1,enemy,getCenter(),Scalar(255,0,200),3);
				}
			}
		}
		if(dead==true)
		{
			line(img1,a,c,Scalar(255,0,0),3);
			line(img1,b,d,Scalar(255,0,0),3);
		}
	}
}

void Card::NewRound(int player)
{
	if(this->owner==player)
	{
		this->canUntap=true;
		this->newRound=true;
	}
	fresh = true;
	this->attack=false;
	this->block=false;
	def=cardBase.def;
	att=cardBase.att;
	defEOT=0;
	attEOT=0;
	hasFlyingEOT=hasLifelinkEOT=hasFirstStrikeEOT=hasHexproofEOT=false;
	la=lb=lc=ld=Point(-1,-1);
}

void Card::Clear()
{
	attack=false;
	block=false;
	old=getCenter();
	enemy=Point(-1,-1);
	blocking=-1;
	la=lb=lc=ld=Point(-1,-1);
}

bool Card::CanBlock(Card &card)
{
	if(this->canUntap==false || this->cardBase.type!=CREATURE || this->hasCantBlock==true) return false;
	
	if(card.HasFlying()==true && (this->cardBase.hasFlying==false && this->cardBase.hasReach==false))
	{
		return false;
	}
	cout<<"TUTAJ"<<endl;
	return true;
}
bool Card::CanAttack()
{
	if(this->cardBase.hasDefender==true || this->hasCantAttack==true || this->cardBase.type!=CREATURE) return false;
	return true;
}