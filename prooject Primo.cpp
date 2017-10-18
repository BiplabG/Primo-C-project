#include <iostream>
#include <SDL.h>
#include <SDL_image.h>
#include <SDL_ttf.h>
#include <cstring>
#include <vector>
#include <algorithm>
#include <ctime>
#include <sstream>

using namespace std;

enum
{
    Box0=0,Box1,Box2,Box3,Box4,Box5,Box6,Box7,Box8,Box9,Box10,Box11
};

//Screen Dimensions Constants
const int SCREEN_WIDTH = 360;
const int SCREEN_HEIGHT = 480;

SDL_Window* PrimoWindow =NULL ;
SDL_Renderer* PrimoRend =NULL ;
TTF_Font* Pfont =NULL;
bool quit=false;

SDL_Color ColorPrime={0xFF,0x25,0x00};
SDL_Color ColorComposite={0xFF,0xFF,0xFF};
SDL_Color ColorFriendly={0x25,0xCD,0x00};
SDL_Color ColorTitle={0xFF,0x00,0x00};

bool Initialiser();//It initialises SDL window, SDLimage, SDLttf and other required elements.
void Closer();//It shuts down SDL subsystems on closing of program.
bool LoadFont();//It loads the true type font file.
bool CheckPrime(int);//Checks the number is prime or not
void CheckLevel();
void NewLevel();
//SDL_Texture* Texture_Loader( string );

int levels[]={11,13,17,19,23,29,31,37,41,43,47,53,59,61,67,71,73,79,83,87,89,91,97,101};
int LEVEL,LevelCounter=0;

class MainTexture
{
    public:
		//Initializes variables
		MainTexture();

		//Deallocates memory
		~MainTexture();

		//Deallocates texture
		void free();

		//Renders texture at given point
		void FinalRender(int );

		 //Set positions of the boxes
		 void SetBoxPos();

		 // It draws the main playing board
		 void DrawBoard();

		 void LoadFromRenderedText(std::string,SDL_Color,int , TTF_Font*);

		 void EasyLoader(int,int);

		 //friend class NumberObjects;

	private:
		//The actual hardware texture
		SDL_Texture* pTexture[11];

		//Image dimensions
		int pWidth;
		int pHeight;
		SDL_Rect Boxes[11];
		//for the dimensions of image from text
        int mWidth[11];
		int mHeight[11];
};
MainTexture :: MainTexture()
{
    for(int i=0;i<11;i++)
    pTexture[i]=NULL;
    pWidth=SCREEN_WIDTH;
    pHeight=SCREEN_HEIGHT;
    SetBoxPos();
}

MainTexture :: ~MainTexture()
{
    free();
}

void MainTexture :: free()
{
    //Free texture if it exists
    for(int i=0;i<11;i++)
    {
        if( pTexture[i] != NULL )
        {
            SDL_DestroyTexture( pTexture[i] );
            pTexture[i] = NULL;
        }
        pWidth = 0;
        pHeight = 0;
    }
}

void MainTexture :: FinalRender(int Boxnum )
{
    //Set rendering space and render to screen
	SDL_Rect tempRect;
	tempRect.h=Boxes[Boxnum].h-80;
	tempRect.w=Boxes[Boxnum].w-80;
	tempRect.x=Boxes[Boxnum].x+30;
	tempRect.y=Boxes[Boxnum].y+30;
	//Render to screen
	SDL_Texture* TempTexture = pTexture[Boxnum];
	SDL_RenderCopy( PrimoRend,TempTexture, NULL, &tempRect );
	SDL_DestroyTexture(TempTexture);
}

void MainTexture :: SetBoxPos()
{
    Boxes[0] = {0,0,240,pHeight/4};
    Boxes[1] = {240,0,pWidth/3,pHeight/4};
    Boxes[2] = {0,pHeight/4,pWidth/3,pHeight/4};
    Boxes[3] = {pWidth/3,pHeight/4,pWidth/3,pHeight/4};
    Boxes[4] = {240,pHeight/4,pWidth/3,pHeight/4};
    Boxes[5] = {0,pHeight/2,pWidth/3,pHeight/4};
    Boxes[6] = {pWidth/3,pHeight/2,pWidth/3,pHeight/4};
    Boxes[7] = {240,pHeight/2,pWidth/3,pHeight/4};
    Boxes[8] = {0,360,pWidth/3,pHeight/4};
    Boxes[9] = {pWidth/3,360,pWidth/3,pHeight/4};
    Boxes[10] = {240,360,pWidth/3,pHeight/4};
}

void MainTexture :: DrawBoard()
{
    SDL_SetRenderDrawColor( PrimoRend, 0xE0, 0xE0, 0xE0, 0xFF );
    for(int i=2;i<11;i++)
        SDL_RenderDrawRect(PrimoRend,&Boxes[i]);

        SDL_RenderDrawLine(PrimoRend,0,119,360,119);//Upper line
        SDL_RenderDrawLine(PrimoRend,1,120,1,480);//Leftmost line
        SDL_RenderDrawLine(PrimoRend,0,478,360,478);//Bottom line
        SDL_RenderDrawLine(PrimoRend,358,120,358,480);//Rightmost line
}
//Function to load elements of playing board easily
void MainTexture ::EasyLoader(int ObjectValue,int Boxnumber)
{
    stringstream ss;
    ss << ObjectValue;
    string ValueString = ss.str(); //To convert Integer to string

    SDL_Color SendingColor;
    if (ObjectValue==0){ValueString=" ";}
    else if(ObjectValue==1||ObjectValue==2)
    {
        SendingColor = ColorFriendly;
    }
   else if(CheckPrime(ObjectValue))
    {
       SendingColor = ColorPrime;
    }
    else SendingColor = ColorComposite;

    LoadFromRenderedText(ValueString,SendingColor,Boxnumber,Pfont);
}
void MainTexture::LoadFromRenderedText(string Boxstring, SDL_Color NumColor, int Boxnum, TTF_Font* Tempfont)
{
    //Free all textures.
    free();
    //Render Text Surface
    SDL_Surface* TextSurface = TTF_RenderText_Solid(Tempfont, Boxstring.c_str(), NumColor);
    if(TextSurface==NULL)
    {
        cout<<"Unable to render text surface! SDL_ttf Error:"<<TTF_GetError()<<endl;
    }
    else
        //Create Texture from the loaded surface
    {
        pTexture[Boxnum]= SDL_CreateTextureFromSurface(PrimoRend,TextSurface);
        if(pTexture[Boxnum]==NULL)
        {
            cout<<"Unable to create texture from rendered text! SDL Error:"<<SDL_GetError()<<endl;
        }
        else
		{
			//Get image dimensions
			mWidth[Boxnum] = TextSurface->w;
			mHeight[Boxnum] = TextSurface->h;
		}

		//Get rid of old surface
		SDL_FreeSurface( TextSurface );
    }
}

class NumberObjects
{
private:
    int Num;
public:
    void putvalue(int);
    int Givevalue()
    {
        return Num;
    }

    bool operator +(NumberObjects *);
    friend void CheckLevel();
    friend void NewLevel();
    friend int RandomGenerator();

};

void NumberObjects:: putvalue(int a)
{
    Num=a;
}
//Defines How the objects are added as per the game rule.
bool NumberObjects::operator+(NumberObjects* Temp)
{
    bool addable = true;
    int tempSum;
    if(Num==0||Num==1||Num==2||Temp->Num==0||Temp->Num==1||Temp->Num==2)
    {
        tempSum=Num+Temp->Num;
        Temp->putvalue(tempSum);
        Num=0;
    }
    else if(!(CheckPrime(Num)||CheckPrime(Temp->Num)))
    {
        tempSum = Num+Temp->Num;
        Temp->putvalue(tempSum);
        Num=0;
    }
    else if(CheckPrime(Num+Temp->Num))
    {
        tempSum = Num+Temp->Num;
        Temp->putvalue(tempSum);
        Num=0;
    }
    else addable = false;
    return addable;
}

class LoadingImages
{
private:
    SDL_Texture* newtexture;
public:
    LoadingImages()
    {
        newtexture = NULL;
    }
    ~LoadingImages()
    {
        SDL_DestroyTexture(newtexture);
    }

    void Texture_Loader( string path)
    {
    SDL_Surface* newsurface=IMG_Load(path.c_str());
    if(newsurface==NULL)
    {
        cout<<"Surface could not be created.";
    }
    else newtexture=SDL_CreateTextureFromSurface(PrimoRend,newsurface);
    if (newtexture==NULL)
        cout<<"Texture could not be loaded";
    SDL_FreeSurface(newsurface);
    }

    SDL_Texture* giveTexture()
    {
        return newtexture;
    }

};


NumberObjects Objects[3][3]; //Numbers of each boxes
MainTexture main1;
LoadingImages Welcome, LevelCompletion, GameOver, Instructions;


void DownAdder()
{
    for(int j=0;j<3;j++)
    {
        int k=0;
        for(int i=0;i<2;i++)
        {
            Objects[i][j] + &Objects[i+1][j];
        }
        Objects[k][j] + &Objects[k+1][j];
    }
}

void UpAdder()
{
    for(int j=0;j<3;j++)
    {
        int k=2;
        for(int i=2;i>0;i--)
        {
            Objects[i][j] + &Objects[i-1][j];
        }
        Objects[k][j]+&Objects[k-1][j];
    }
}

void RightAdder()
{
    for(int i=0;i<3;i++)
    {
        int k=0;
        for(int j=0;j<2;j++)
        {
            Objects[i][j] + &Objects[i][j+1];
        }
        Objects[i][k]+ &Objects[i][k+1];
    }
}

void LeftAdder()
{
    for(int i=0;i<3;i++)
    {
        int k=2;
        for(int j=2;j>0;j--)
        {
            Objects[i][j] + &Objects[i][j-1];
        }
        Objects[i][k] + &Objects[i][k-1];
    }
}


bool CheckPrime(int num)
{
    int checker;
    bool Prime= true;
    for(int i=2;i<num;i++)
    {
        checker=num%i;
        if(checker==0)
        {
            Prime = false;
            break;
        }
    }
    return Prime;
}

void CheckLevel()
{
    for(int i=0;i<3;i++)
    {
        for(int j=0;j<3;j++)
        {
           if (Objects[i][j].Num==LEVEL)
           {
               SDL_Delay(2000);
               LevelCounter++;
               LEVEL=levels[LevelCounter];
               NewLevel();
               LevelCompletion.Texture_Loader("LevelCompleted.png");
               SDL_Event Q;

               bool KeepDisplaying=true;
               while(KeepDisplaying)
               {
                   //cout<<"Level Completed 1";
                   SDL_RenderClear(PrimoRend);
                   SDL_SetRenderDrawColor(PrimoRend,0xFF,0xFF,0xFF,0xFF);
                   SDL_RenderCopy(PrimoRend,LevelCompletion.giveTexture(),NULL,NULL);
                   SDL_RenderPresent(PrimoRend);
                   while(SDL_PollEvent(&Q)!=0)
                   {
                       if(Q.type==SDL_QUIT)
                       {
                           KeepDisplaying=false;
                           quit = true;
                           break;
                       }
                       else if(Q.type==SDL_KEYDOWN)
                       {
                           switch (Q.key.keysym.sym)
                           {
                           case SDLK_ESCAPE:
                            {
                                quit = true;
                                KeepDisplaying=false;
                                break;
                            }
                            case SDLK_SPACE:
                                {
                                KeepDisplaying=false;
                                break;
                                }
                           }
                       }
                   }
               }


           }

        }
    }
}
//Initialise Objects value at new level to zero.
void NewLevel()
{
    for(int i=0;i<3;i++)
    {
        for(int j=0;j<3;j++)
        {
            Objects[i][j].Num=0;
        }
    }
    Objects[1][1].Num=2;
}

struct PosDeterminer
{
    int Row,Col;
};

int RandomGenerator()
{
  //put the random number(1-5) on board by detecting the void space
	vector <PosDeterminer> RndList;
	PosDeterminer P[9];
	int check=0;
	int k=0;

    for(int i=0; i<3; i++)
    {
        for(int j = 0;j<3;j++)
        {
            P[k].Row=i;
            P[k].Col=j;
            if(Objects[i][j].Num==0)
            {
                RndList.push_back(P[k]);
                check++;
            }
            k++;
        }
    }
  	if(check!=0)
	  {
			int RandIndex, One_Five=0;

			random_shuffle ( RndList.begin(), RndList.end() );

			srand (time(NULL));

			RandIndex = rand() % RndList.size();

			while(One_Five==0)
			{
				One_Five=rand()%6;
			}

			PosDeterminer Ptemp = RndList[RandIndex];
			Objects[Ptemp.Row][Ptemp.Col].putvalue(One_Five);
 	  }
 	else
 	 {
 	     SDL_Delay(1000);
 	     GameOver.Texture_Loader("Gameover.png");
 	     bool KeepDisplay=true;
 	     SDL_Event Q;
               while(KeepDisplay)
               {
                   SDL_RenderClear(PrimoRend);
                   SDL_SetRenderDrawColor(PrimoRend,0xFF,0xFF,0xFF,0xFF);
                   SDL_RenderCopy(PrimoRend,GameOver.giveTexture(),NULL,NULL);
                   SDL_RenderPresent(PrimoRend);
                   while(SDL_PollEvent(&Q)!=0)
                   {
                       if(Q.type==SDL_QUIT)
                       {
                           quit = true;
                           KeepDisplay=false;
                           break;
                       }
                      else if(Q.type==SDL_KEYDOWN)
                       {
                           switch (Q.key.keysym.sym)
                           {
                           case SDLK_ESCAPE:
                            {
                                quit = true;
                                KeepDisplay=false;
                                break;
                            }
                            case SDLK_SPACE:
                                {
                                    NewLevel();
                                    KeepDisplay=false;
                                    break;
                                }
                           }
                       }
                   }
               }


	 }
}

//Function Definitions
bool Initialiser()
{
    // Initialization Flag
    bool success = true;
    //Initialise SDL subsystem
    if( SDL_Init( SDL_INIT_VIDEO ) < 0 )
	{
		cout<< "SDL could not initialize! SDL Error: %s"<<SDL_GetError()<<endl;
		success = false;
	}
	else
    {
        if( !SDL_SetHint( SDL_HINT_RENDER_SCALE_QUALITY, "1" ) )
		{
			cout<< "Warning: Linear texture filtering not enabled!"<<endl ;
		}
		//Create SDL window

		PrimoWindow = SDL_CreateWindow("Primo" , SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, SCREEN_WIDTH, SCREEN_HEIGHT, SDL_WINDOW_SHOWN);

		if(PrimoWindow == NULL )
        {
            cout<<"Window could not be initialized."<<endl<<SDL_GetError()<<endl;
            success=false;
        }
        else
        {
            //Create the renderer
            PrimoRend = SDL_CreateRenderer(PrimoWindow,-1,SDL_RENDERER_PRESENTVSYNC);
            if( PrimoRend == NULL )
			{
				cout<<"Renderer could not be created! SDL Error:\n"<< SDL_GetError()<<endl;
				success = false;
			}
			else
            {
                SDL_SetRenderDrawColor(PrimoRend,0xE0,0xFF,0xFF,0xFF);
                int imgFlags= IMG_INIT_PNG;
                if(!(IMG_Init(imgFlags) &imgFlags ))
                {
                    cout<<"SDL image could not initialize."<<IMG_GetError();
                    success=false;
                }
                //Initialize SDL_ttf
				if( TTF_Init() == -1 )
				{
					cout<<"SDL_ttf could not initialize! SDL_ttf Error:"<<TTF_GetError()<<endl;
					success = false;
				}
				LoadFont();
            }
        }
        return success;
    }
}

void Closer()
{
	//Destroy window
	SDL_DestroyRenderer( PrimoRend );
	SDL_DestroyWindow( PrimoWindow );
	PrimoWindow = NULL;
	PrimoRend = NULL;

	//Quit SDL subsystems
	IMG_Quit();
	SDL_Quit();
	TTF_Quit();
}

bool LoadFont()
{
    bool success = true;
    Pfont = TTF_OpenFont("Calibri.ttf",1000);
    if(Pfont==NULL)
    {
        cout<<"Failed to load the font file."<<TTF_GetError()<<endl;
        success=false;
    }
    return success;
}



int main(int argc , char* argv[])
{
    LEVEL=levels[LevelCounter];
    NewLevel();

    if(!Initialiser())
    {
        cout<<"Failed to initialize.";
    }
    else
    {

        SDL_Event e;
        Welcome.Texture_Loader("Welcome.png");
        Instructions.Texture_Loader("Instructions.png");
        bool Displaying=true;
        while(Displaying)
        {
            SDL_RenderClear(PrimoRend);
            SDL_SetRenderDrawColor(PrimoRend,0xFF,0xFF,0xFF,0xFF);
            SDL_RenderCopy(PrimoRend,Welcome.giveTexture(),NULL,NULL);
            SDL_RenderPresent(PrimoRend);
            bool Displayinginst=true;
            while(SDL_PollEvent(&e)!=0)
            {
                if(e.type==SDL_QUIT)
                {
                    quit= true;
                    Displaying=false;
                    break;
                }
                else if( e.type==SDL_KEYDOWN)
                {
                    switch(e.key.keysym.sym)
                    {
                    case SDLK_SPACE:
                    {
                        Displaying=false;
                        break;
                    }
                    case SDLK_ESCAPE:
                        {
                            Displaying=false;
                            quit=true;
                            break;
                        }
                    case SDLK_i:
                        {
                            while(Displayinginst)
                            {
                                SDL_RenderClear(PrimoRend);
                                SDL_SetRenderDrawColor(PrimoRend,0xFF,0xFF,0xFF,0xFF);
                                SDL_RenderCopy(PrimoRend,Instructions.giveTexture(),NULL,NULL);
                                SDL_RenderPresent(PrimoRend);
                                while(SDL_PollEvent(&e)!=0)
                                    {
                                        if(e.type==SDL_QUIT)
                                        {
                                            quit= true;
                                            Displayinginst=false;
                                            break;
                                        }
                                        else if (e.type==SDL_KEYDOWN)
                                        {
                                            switch(e.key.keysym.sym)
                                            {
                                            case SDLK_SPACE:
                                                {
                                                    Displayinginst=false;
                                                    Displaying=false;
                                                    break;
                                                }
                                            case SDLK_ESCAPE:
                                                {
                                                    Displayinginst=false;
                                                    Displaying=false;
                                                    quit=true;
                                                    break;
                                                }
                                            case SDLK_BACKSPACE:
                                                {
                                                    Displayinginst=false;
                                                    break;
                                                }
                                            }
                                        }
                                }
                            }
                        }

                    }
                }
            }
        }

//Game loop starts
        do
        {
            //Handle events on queue
				while( SDL_PollEvent( &e ) != 0 )
				{
					//User requests quit
					if( e.type == SDL_QUIT )
					{
						quit = true;
					}

					 if( e.type == SDL_KEYDOWN )
					{
						//Select functions based on key press
						switch( e.key.keysym.sym )
						{
						    case SDLK_UP:
						    {
						        UpAdder();
						        RandomGenerator();
						        break;
						    }
						    case SDLK_DOWN:
						    {
						        DownAdder();
						        RandomGenerator();
						        break;
						    }
						    case SDLK_RIGHT:
						    {
						        RightAdder();
						        RandomGenerator();
						        break;
						    }
						    case SDLK_LEFT:
						    {
						        LeftAdder();
						        RandomGenerator();
						        break;
						    }
						    case SDLK_ESCAPE:
						    {
						        quit=true;
						        break;
						    }
						}
					}
				}
				SDL_SetRenderDrawColor(PrimoRend,0x00,0x80,0xFF, 0xFF );
				SDL_RenderClear( PrimoRend );
				main1.DrawBoard();

				for(int k_Load=2,i_Load=0;i_Load<3;i_Load++)
                {
                    for(int j_Load=0;j_Load<3;j_Load++)
                    {
                        main1.EasyLoader(Objects[i_Load][j_Load].Givevalue(),k_Load);
                         main1.FinalRender(k_Load);
                        k_Load++;
                    }
				}

                char Nepal[]="Primo";
				main1.LoadFromRenderedText(Nepal,ColorTitle,Box0,Pfont);
				main1.FinalRender(Box0);
				main1.EasyLoader(LEVEL,Box1);
				main1.FinalRender(Box1);
				SDL_RenderPresent(PrimoRend); //To update the screen

                CheckLevel();
				//SDL_RenderPresent( PrimoRend );
				SDL_Delay(250);

        }while(!quit);
    }

    Closer();
    return 0;
}





