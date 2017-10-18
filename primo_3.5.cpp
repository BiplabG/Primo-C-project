#include<iostream>
#include <windows.h>
#include <vector>
#include <cstdlib>
#include <time.h>
#include <conio.h>
#include <algorithm>
#include <fstream>
using namespace std;
int level[]={7,11,13,17,19,23,29,31,37,43,47,53};
int l=0;

bool LOST;

int LEVEL,CHECK=0;

enum color {
cBLACK=0,     cDARK_BLUE=1,    cDARK_GREEN=2, cDARK_CYAN=3, 
cDARK_RED=4,  cDARK_MAGENTA=5, cBROWN=6,      cLIGHT_GRAY=7,
cDARK_GRAY=8, cBLUE=9,         cGREEN=10,     cCYAN=11, 
cRED=12,      cMAGENTA=13,     cYELLOW=14,    cWHITE=15 };

void SetColor(int ForgC)
{
     WORD wColor;
     ///We will need this handle to get the current background attribute
     HANDLE hStdOut = GetStdHandle(STD_OUTPUT_HANDLE);
     CONSOLE_SCREEN_BUFFER_INFO csbi;

     ///We use csbi for the wAttributes word.
     if(GetConsoleScreenBufferInfo(hStdOut, &csbi))
     {
        ///Mask out all but the background attribute, and add in the forgournd color
          wColor = (csbi.wAttributes & 0xF0) + (ForgC & 0x0F);
          SetConsoleTextAttribute(hStdOut, wColor);
     }
     return;
}

COORD coord = {0,0}; ///set the cordinate to 0, 0 (top-left corner of window);
void gotoxy(int x, int y)
{
    coord.X = x; coord.Y = y; /// X and Y coordinates
    SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), coord);
}

class object
{
	private:
		
		 int posx,posy,color;
		int n;
	
	public:
		object(int x,int y,int c,int z=0):posx(x),posy(y),color(c),n(z)
		{
		}
		object(){
		}
		
		void putvalue(int a)
		{
			n=a;
		}
		void display()
		{
			
			gotoxy(posx,posy);
			cout<<"    ";
			SetColor(color);
			gotoxy(posx,posy);
			cout<<n;
			SetColor(cBLACK);
		}
		void display_blank()
		{
			gotoxy(posx,posy);
			cout<<"    ";
		}
		friend void board(object o[9]);
		int getn()
		{
			return n;
		}
		friend void display_no(object o[9]);
		friend void align(object o[9],int a);
		friend int checklevel(object o[9]);
		
		bool checkcom()
		{
			int check=0;
			for(int i=2;i<=n/2;i++)
			{
				if(n%i==0)
				{
					check++;
					break;
				}
			}
			if(check==0)
				return false;//prime
			else
				return true;//comp
		}
		
};
void drawbox();
void display_no(object o[9]);
void board(object o[9]);
bool checkprime(int sum);
int checklevel(object o[9]);
void align(object o[9],int a)
{
	int i,j;
	int sum;
	
	object p[9]=o[9];
	//gotoxy(30,30);
	
	
	/*for(i=0;i<9;i++)
	{
		cout<<p[i].getn()<<endl;
	}*/
	
	if(a==1)//for up key w
	{
		for(i=6;i>0;i-=3)
		{
			for(j=i;j<9;j++)
			{
				if(o[j].n==1||o[j].n==0||o[j].n==2||o[j-3].n==1||o[j-3].n==2||o[j-3].n==0)
				{
					sum=o[j].n+o[j-3].n;
					o[j-3].putvalue(sum);
					o[j].putvalue(0);
				}
				
				sum=o[j].n+o[j-3].n;
				if(checkprime(sum)==0)//that is if sum is a prime no.
					{
						o[j-3].putvalue(sum);
						o[j].putvalue(0);
					}
				
				if(o[j].checkcom()== 1 && o[j-3].checkcom()== 1)
				{
					sum=o[j].n+o[j-3].n;
					o[j-3].putvalue(sum);
					o[j].putvalue(0);
				}
				
				if(o[j].checkcom()== 0 && o[j-3].checkcom()== 0)
				{
					sum=o[j].n+o[j-3].n;
					if(checkprime(sum)==0)
					{
						o[j-3].putvalue(sum);
						o[j].putvalue(0);
					}
				}
				
			}
		}
	}
	
	
	if(a==3)//for down key or s key
	{
		for(i=0,j=8;i<9||j>=0;i++,j--)//done to follow the same easy algorithm as followed by up key
		{
			p[j]=o[i];
		}
		
		
		for(i=6;i>0;i-=3)
		{
			for(j=i;j<9;j++)
			{
				if(p[j].n==1||p[j].n==0||p[j].n==2||p[j-3].n==1||p[j-3].n==2||p[j-3].n==0)
				{
					sum=p[j].n+p[j-3].n;
					p[j-3].putvalue(sum);
					p[j].putvalue(0);
				}
				
				sum=p[j].n+p[j-3].n;
				if(checkprime(sum)==0)//that is if sum is a prime no.
					{
						p[j-3].putvalue(sum);
						p[j].putvalue(0);
					}
				
				if(p[j].checkcom()== 1 && p[j-3].checkcom()== 1)
				{
					sum=p[j].n+p[j-3].n;
					p[j-3].putvalue(sum);
					p[j].putvalue(0);
				}
				
				if(p[j].checkcom()== 0 && p[j-3].checkcom()== 0)
				{
					sum=p[j].n+p[j-3].n;
					if(checkprime(sum)==0)
					{
						p[j-3].putvalue(sum);
						p[j].putvalue(0);
					}
				}
				
				
				
			}
		}
		
		for(i=0,j=8;i<9||j>=0;i++,j--)//done to follow the same easy algorithm as followed by up key
		{
			o[j]=p[i];
		}
		
	}
	
	if(a==4)//for d key means left side
	{
		for(i=0,j=8;i<9;)
		{
			p[i]=o[j];
			i++;
			j-=3;
			if(i==3)
				j=7;
			if(i==6)
			{
				j=6;
			}
			
		}
		/*gotoxy(0,30);
		for(i=0;i<9;i++)
		{
			cout<<p[i].getn()<<endl;
		}*/
		
		for(i=6;i>0;i-=3)
		{
			for(j=i;j<9;j++)
			{
				if(p[j].n==1||p[j].n==0||p[j].n==2||p[j-3].n==1||p[j-3].n==2||p[j-3].n==0)
				{
					sum=p[j].n+p[j-3].n;
					p[j-3].putvalue(sum);
					p[j].putvalue(0);
				}
				
				sum=p[j].n+p[j-3].n;
				if(checkprime(sum)==0)//that is if sum is a prime no.
					{
						p[j-3].putvalue(sum);
						p[j].putvalue(0);
					}
				
				if(p[j].checkcom()== 1 && p[j-3].checkcom()== 1)
				{
					sum=p[j].n+p[j-3].n;
					p[j-3].putvalue(sum);
					p[j].putvalue(0);
				}
				
				if(p[j].checkcom()== 0 && p[j-3].checkcom()== 0)
				{
					sum=p[j].n+p[j-3].n;
					if(checkprime(sum)==0)
					{
						p[j-3].putvalue(sum);
						p[j].putvalue(0);
					}
				}
				
			}
		}
		
		for(i=0,j=8;i<9;)
		{
			o[i]=p[j];
			i++;
			j-=3;
			if(i==3)
			{
				j=7;
			}
				//j=7;
			if(i==6)
			{
				j=6;
			}
			
		}
		
	}
	
	if(a==2)//for a key which means right shift
	{
		for(i=0,j=0;i<9;)
		{
			p[i]=o[j];
			i++;
			j+=3;
			if(i==3)
				j=1;
			if(i==6)
			{
				j=2;
			}
			
		}
	/*	gotoxy(0,30);
		for(i=0;i<9;i++)
		{
			cout<<p[i].getn()<<endl;
		}
		*/
		for(i=6;i>0;i-=3)
		{
			for(j=i;j<9;j++)
			{
				if(p[j].n==1||p[j].n==0||p[j].n==2||p[j-3].n==1||p[j-3].n==2||p[j-3].n==0)
				{
					sum=p[j].n+p[j-3].n;
					p[j-3].putvalue(sum);
					p[j].putvalue(0);
				}
				
				sum=p[j].n+p[j-3].n;
				if(checkprime(sum)==0)//that is if sum is a prime no.
					{
						p[j-3].putvalue(sum);
						p[j].putvalue(0);
					}
				
				if(p[j].checkcom()== 1 && p[j-3].checkcom()== 1)
				{
					sum=p[j].n+p[j-3].n;
					p[j-3].putvalue(sum);
					p[j].putvalue(0);
				}
				
				if(p[j].checkcom()== 0 && p[j-3].checkcom()== 0)
				{
					sum=p[j].n+p[j-3].n;
					if(checkprime(sum)==0)
					{
						p[j-3].putvalue(sum);
						p[j].putvalue(0);
					}
				}
				
			}
		}
		
		for(i=0,j=0;i<9;)
		{
			o[i]=p[j];
			i++;
			j+=3;
			if(i==3)
				j=1;
			if(i==6)
			{
				j=2;
			}
			
		}
	}
	
	checklevel(o);
	
	if(CHECK==0)
	{
	
		//drawbox();
	
		board(o);
		display_no(o);
		
	}
}

int checklevel(object o[9])
{
	for(int i=0;i<9;i++)
	{
		//for(int j=l;j<6;j++)
	//	{
		if(CHECK==0)
		{
			if(o[i].n==LEVEL)
			{
				gotoxy(25,12);
				cout<<"Congrats,..You have completed the "<<LEVEL<<" level";
				
				
				CHECK++;
				int check_level_exist=0;
				int check_level=0;
				//cout<<"Fiel is going to be formed."<<endl;
				ofstream outfile("select_level.dat",ios::binary|ios::app);
				//cout<<"File is formed"<<endl;
				outfile.close();
				
				ifstream infile("select_level.dat",ios::binary);
				//cout<<"File is opend as read mode.";
				while(!infile.eof())
				{
					if(infile.read(reinterpret_cast<char*>(&check_level),sizeof(check_level)>0))
					{
						//cout<<"already exists";
						if(check_level==LEVEL)
						{
							check_level_exist++;
							//cout<<"already exists";
							break;
						}
					}
				}
				infile.close();
				
				if(check_level_exist==0)
				{
					ofstream outfile("select_level.dat",ios::binary|ios::app);
					outfile.write(reinterpret_cast<char*>(&LEVEL),sizeof(LEVEL));
					//cout<<"file is written";
					outfile.close();
				}
				
				l++;
				LEVEL=level[l];
				getch();
				
				
				
				
				
				//drawbox();
				
			}
			
		}
	}
	
}


bool checkprime(int sum)
{
			int check=0;
			for(int i=2;i<=sum/2;i++)
			{
				if(sum%i==0)
				{
					check++;
					break;
				}
			}
			if(check==0)
				return false;//prime
			else
				return true;//comp
}

void display_no(object o[9])
{
	for(int i=0;i<9;i++)
	{
		if(o[i].getn()!=0)
		{
			o[i].display();
			//cout<<"  ";
		}
		else
		{
			o[i].display_blank();
		}
	}
}

void board(object o[9])	//put the random number(1-5) on board by detecting the void space
{
	vector<int> rndlist;
	int check=0;

    for(int i = 0; i < 9; i++){
        if(o[i].n==0)
		{
            rndlist.push_back(i);
            check++;
        }
    }
  	if(check!=0)
	  {
			int randindex, one_five=0;

			random_shuffle ( rndlist.begin(), rndlist.end() );

			srand (time(NULL));

			randindex = rand() % rndlist.size();

			while(one_five==0)
			{
				one_five=rand()%6;
			}

			o[rndlist[randindex]].putvalue(one_five);
 	  }
 	else
 	{
  			 gotoxy(25,12);
  			 cout<<"You lost the game. THANK YOU";
  			 LOST=1;
  			 for(int i=0;i<9;i++)
    				o[i].putvalue(0);
  			 getch();
	 }
}

void drawbox()
{
	system("cls");
	 int i, j;
    //gotoxy(0,0);
    //cout<<static_cast<char>(201);
    for(i = 33; i < 44; i++){
        gotoxy(i, 0);
        cout<<static_cast<char>(205);
    }
    gotoxy(33,1);
    cout<<"---PRIMO---";
    
    for(i = 33; i < 44; i++){
        gotoxy(i, 2);
        cout<<static_cast<char>(205);
    }
    
 
    for(i = 6; i < 18; i++){
        gotoxy(55, i);
        if(i == 6||i==10||i==14||i==18){
            cout<<static_cast<char>(185);
            for(j = 55; j > 25; j--)
			{
        		gotoxy(j,i);
        		cout<<static_cast<char>(205);
    		}
            
        }else{
            cout<<static_cast<char>(186);
        }
    }

    for(i = 55; i > 25; i--){
        gotoxy(i,18);
        if(i == 35||i==45||i==26){
            cout<<static_cast<char>(202);
            for(j = 6; j < 18; j++)
			{
        		gotoxy(i, j);
            	cout<<static_cast<char>(186);
   		 	}
        }else{
            cout<<static_cast<char>(205);
        }
    }
    
     gotoxy(47,5);
    cout<<"LEVEL: "<<LEVEL;


}

void dis_starting()
{
	system("cls");
	 int i, j;
    //gotoxy(0,0);
    //cout<<static_cast<char>(201);
    for(i = 33; i < 44; i++){
        gotoxy(i, 0);
        cout<<static_cast<char>(205);
    }
    gotoxy(33,1);
    cout<<"---PRIMO---";
    
    for(i = 33; i < 44; i++){
        gotoxy(i, 2);
        cout<<static_cast<char>(205);
    }
	gotoxy(30,5);
	cout<<"1..RESUME";
	gotoxy(30,7);
	cout<<"2..NEW GAME";
	gotoxy(30,9);
	cout<<"3..SELECT LEVEL";
	gotoxy(30,11);
	cout<<"4..INSTRUCTIONS";
	gotoxy(30,13);
	cout<<"5..EXIT";
}

void dis_instructions()
{
	system("cls");
	gotoxy(8,0);
	cout<<"\t**********INSTRUCTIONS*************"<<endl;
	cout<<"This game is a project game named 'PRIMO' which is derived from the game 2048. "<<endl
		<<"But it has different RULES and CONDITIONS"<<endl
		<<"1. Only WASD key are available while playing to shift the no.s as according the key in keyboard."<<endl
		<<"2. Composite numbers (numbers except prime) can add among themselves. "<<endl
		<<"3. Friendly numbers (Term for the game) 1 and 2 add to any number prime or "<<endl
		<<"	  composite. "<<endl
		<<"4. Prime numbers only add to make another prime number. "<<endl
		<<"5. To complete the level any no. in the box must be equal to the LEVEL. "<<endl<<"THANK YOU"<<endl<<endl
		<<"Press Enter to go to the MAIN MENU";
		getch();
}

int dis_arrow()
{
	
	char select;
	char select2;
	select=getch();
	if(select=='1')//resume
	{
		
		gotoxy(25,5);
		cout<<">>>>";
		
		select2=getch();
		if(select2==13)
		{
			return 1;
		}
		
	}
	else if(select=='2')//play
	{
		
		gotoxy(25,7);
		cout<<">>>>";
		
		select2=getch();
    	if(select2==13)
    	{
    		return 2;	
    	}
		
		
	}
	
	else if(select=='3')//select level
	{
		
		gotoxy(25,9);
		cout<<">>>>";
		
		select2=getch();
    	if(select2==13)
    	{
    		return 3;	
    	}
		
		
	}
	
	else if(select=='4')//instructions
	{
		
		gotoxy(25,11);
		cout<<">>>>";
		
		select2=getch();
    	if(select2==13)
    	{
    		return 4;	
    	}
		
	
	}
	
	else if(select=='5')//exit
	{
		gotoxy(25,13);
		cout<<">>>>";
		
		select2=getch();
    	if(select2==13)
    	{
    		return 5;	
    	}
	}
	
	else
		cout<<"\a";	
    	
    	
	
}

void storeobjects(object o[9])
{
	ofstream outfile("Resume.txt");
		for(int i=0;i<9;i++)
		{
			outfile<<o[i].getn()
			<<" ";
		}
		outfile<<l<<" ";
	

	
}


void dis_selectlevels()
{
	int EXIT=0;
while(!EXIT){
	system("cls");
	char ch;
	gotoxy(33,0);
	cout<<"SELECT LEVEL";
	gotoxy(30,1);
	cout<<"------------------";
	gotoxy(29,3);
	cout<<"Unlocked levels:";
	int unlocked_level,search_level;
	int length_in_file=0;
	ofstream outfile("select_level.dat",ios::binary|ios::app);
	outfile.close();
	ifstream infile("select_level.dat",ios::binary);
	gotoxy(20,5);
	while(!infile.eof())
	{
		if(infile.read(reinterpret_cast<char*>(&unlocked_level),sizeof(unlocked_level))>0)
		{
			cout<<unlocked_level<<"\t";
		}
		
	}
	infile.close();
	gotoxy(0,7);
	cout<<"Enter the level:";
	cin>>search_level;
	if(checkprime(search_level)==0)
	{
		ifstream infile("select_level.dat",ios::binary);
		while(!infile.eof())
		{
			if(infile.read(reinterpret_cast<char*>(&unlocked_level),sizeof(unlocked_level))>0)
			{
				if(unlocked_level==search_level)
				{
					LEVEL=unlocked_level;
					EXIT=1;
					break;
				}
				
				length_in_file++;
				l=length_in_file;
				
			}
		}
		infile.close();
		
		if(search_level<7)
		{
			//gotoxy(0,9);
			//cout<<"This level doesnot exists.";
			EXIT=1;
		}
		
		if(EXIT==0)
		{
			gotoxy(0,9);
			cout<<"Sorry, the level is not unlocked."<<endl<<"Please press enter to select the other unlocked level.";
			ch=getch();
			if(ch==27)
				break;
		}
	}
	else
	{
		gotoxy(0,9);
		cout<<"Sorry, the entered level is not valid level for this game since it is a prime level game."<<endl<<"Please press enter n enter any of the level above.";
		ch=getch();
		if(ch==27)
			break;
	}
	
	
}}

int main()
{
	

	SetConsoleTitle("PROJECT IN C++ BY 071 BEX 4 [05,11,12]");
	
	object o[9]={object(30,8,cDARK_BLUE),object(40,8,cDARK_GREEN),object(50,8,cDARK_CYAN),object(30,12,cBROWN),object(40,12,cRED),object(50,12,cDARK_MAGENTA),object(30,16,cDARK_GRAY),object(40,16,cDARK_RED),object(50,16,cBLACK)};

	LEVEL=level[l];
	
	
	
	
	
	bool lost = 0;
	bool EXIT=0;
	int id = 0;
    while(!EXIT)
    {
    	
    	dis_starting();
    	
    	int selectloop;
    	selectloop=dis_arrow();
    	
    	if(selectloop==4)
    		dis_instructions();
    	
    	else if(selectloop==2||selectloop==1)
    	{
    		LOST=0;
    		
    		if(selectloop==2)//NEW GAME
    		{
    			//LEVEL=level[l];
    			srand (time(NULL));
				int randno=0;
				while(randno==0)
				{
					randno=rand()%6;
				}
	
				//srand (time(NULL));
				int randindex=0;
				randindex=rand()%9;
    			
				for(int i=0;i<9;i++)
    				o[i].putvalue(0);
    			o[randindex].putvalue(randno);
    			
				//o[randindex].display();
				
			}
			
			if(selectloop==1)//RESUME
			{
				
				ifstream outfile("Resume.txt");
				int num[9];
				for(int i=0;i<9;i++)
				{
					outfile>>num[i];
				}
				
				
				
				for(int i=0;i<9;i++)
				{
					o[i].putvalue(num[i]);
				}
				outfile>>l;
				LEVEL=level[l];
				
				
				
				
			}

			drawbox();
			
			display_no(o);
    		
    		while(!LOST)
			{
    			storeobjects(o);
    			if(CHECK!=0)
    			{
 			   		for(int i=0;i<9;i++)
    				o[i].putvalue(0);
    			
    				drawbox();
    				board(o);
					display_no(o);
					CHECK=0;
		    	}

        		switch ( getch() ) 
				{

        			case 'w':

            			align(o, 1);


            			break;	
            
        			case 'a':	

            			align(o, 2);

            			break;
            
        			case 's':

            			align(o, 3);

            			break;
            
        			case 'd':

            			align(o, 4);

            			break;
            			
            		case 27:
            			LOST=1;
            			storeobjects(o);
            			break;
            
        			default:

            			//board(o);
            			gotoxy(0,19);
            			cout <<"\a Only WASD keys are recognized" << endl;
            			break;


        		}

    		}
    	
    	}
    	
    	
    	else if(selectloop==5)
    	{
    		
    		storeobjects(o);
    		return 0;
    		gotoxy(0,20);
			exit(0);
    		
    		
    	}
    	
    	else if(selectloop==3)
    	{
    		dis_selectlevels();
    	}
    	
    	
    }
	
	gotoxy(0,20);
	
	
}
