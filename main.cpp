#define SDL_MAIN_HANDLED


#include<iostream>
#include"sdl.h"
//#include<Windows.h>
#include<SDL_keyboard.h>
#include<SDL_keycode.h>
#include<SDL_scancode.h>
//int SDL_main(int argc, char *argv[]){
//int main(){
//int main(int argc, char *argv[]){
//	system("PAUSE");
//return 1;}



//#include<DarkGDK.h>
#include<iostream>
#include<string>
#include"main.h"
#include<time.h>

int EXIST;

using namespace std;

int main(){
_INIT();
//=====================================================================================================================
//                            MAIN GAME LOOP                                                                                
//                            DATE:  11/22/17                                                                                
//                            DEPENDANCIES: SDL.lib
//                                          DDraw.lib                                                   
//                                          DirectX 9.0(MAYBE 7.0 Not sure)                                                  
//=====================================================================================================================

	cout << "entered game loop" << endl;
//SDL_SetRenderTarget(RENDER_HND, MAP);
   while (1) {
       if (SDL_PollEvent(&EVENT) && EVENT.type == SDL_QUIT) break;

	   if( EVENT.type == SDL_KEYDOWN)
            {
                switch( EVENT.key.keysym.sym )
                {
                    case SDLK_UP: SHOW_VISION = FALSE; break;
                    case SDLK_DOWN: SHOW_VISION = TRUE; break;
                    case SDLK_LEFT: SHOW_COLLISION = FALSE; break;
                    case SDLK_RIGHT: SHOW_COLLISION = TRUE; break;
                }
            }

	MAIN_LOOP_SPEED = SDL_GetTicks();
	SDL_LockSurface(WINDOW_SURFACE);
   _MOVE_CREATURES();
	SDL_UnlockSurface(WINDOW_SURFACE);
   _PRINT_STATS();
	EXIST = 0;
	SDL_RenderPresent( RENDER_HND );
	//SDL_UpdateWindowSurface( WINDOW_HWND );
	//cout << "Move Creatures" << endl;
	SDL_SetRenderDrawColor(RENDER_HND, 0, 0, 0, 100);
	SDL_RenderClear(RENDER_HND);
   }

   SDL_DestroyRenderer(RENDER_HND);
   SDL_DestroyWindow(WINDOW_HWND);
   SDL_Quit();
   return EXIT_SUCCESS;
}

void _MOVE_CREATURES(){

//dbLockPixels();

//for(int Y = 0;Y<SCREENY;Y++){for(int X = 0;X<SCREENX;X++){if(WORLD[(int)X][(int)Y] != 0)dbDot(X,Y,dbRGB(0,0,250));}}
    AVG_BRAIN1=0;AVG_BRAIN2=0;AVG_BRAIN3=0;AVG_BRAIN4=0;AVG_HEALTH=0;


	for(int COUNT = 0;COUNT < MAX_CREATURES;COUNT++){
        NUM_OF_COLLISIONS = 0;


		if(CREATURES[COUNT].EXIST == TRUE){
			EXIST++;
			   AVG_BRAIN4+=CREATURES[COUNT].BRAIN[2][0];
			   AVG_BRAIN1+=CREATURES[COUNT].BRAIN[2][1];
	           AVG_BRAIN2+=CREATURES[COUNT].BRAIN[2][2];
	           AVG_BRAIN3+=CREATURES[COUNT].BRAIN[2][3];
			   AVG_HEALTH+=CREATURES[COUNT].HEALTH;

			CREATURES[COUNT].AGE++;
			CREATURES[COUNT].LAST_BREED++;
			CREATURES[COUNT].LAST_MEAL++;
			//CREATURES[COUNT].COLOR = dbRGB(255 / CREATURES[COUNT].AGE,255,255);
			float OLDX = CREATURES[COUNT].X;
			float OLDY = CREATURES[COUNT].Y;

			WORLD[int(OLDX)][(int)(OLDY)] = 0;

			int SEES = 0;
			SEES = CREATURES[COUNT].SIGHT();
		

//=================RUN SIGHT METHOD =====================================i NEED TO ADD FOOD WHICH IS -2 ON THE WORLD GRID =======================================================

			if(SEES != 0){
				if(SEES == -1){
					CREATURES[COUNT].SIGHT_HANDLER(WALL); //SEES A WALL 
				}
			    else if(CREATURES[COUNT].SEX == CREATURES[SEES].SEX){   // IF IT SEES SAME SEX CREATURE
					CREATURES[COUNT].SIGHT_HANDLER(ENEMY);
				}

				if(CREATURES[SEES].SEX != CREATURES[COUNT].SEX){  // IF IT SEES OPPOSITE SEX CREATURES
					CREATURES[COUNT].SIGHT_HANDLER(MATE);
				}

			}
			else{
				CREATURES[COUNT].SIGHT_HANDLER(0); // SEES NOTHING 
			}

//====================================COLLISION DETECTION AND REACTION ==================================================================
		
			int HIT = CREATURES[COUNT].HIT();

			if(HIT != 0){
				if(HIT == -1){
					CREATURES[COUNT].HEALTH -=50;
				}else if(HIT == -2){
				    CREATURES[COUNT].HEALTH +=50;
				}else if(CREATURES[HIT].SEX != CREATURES[COUNT].SEX){  if( CREATURES[COUNT].LAST_BREED >= 18){ 
    			    CREATURES[COUNT].LAST_BREED = 0;
				    CREATURES[COUNT].BREED(CREATURES[HIT]);					
				}}
				else{

					CREATURES[COUNT].HEALTH -= 50;
				}
 		        CREATURES[COUNT].X = OLDX;
				CREATURES[COUNT].Y = OLDY;
	    	    WORLD[int(OLDX)][int(OLDY)] = COUNT;
	    	    //if NUM_OF_COLLISIONS > 7 THEN _KILL_CREATURE(COUNT)				
			}
			else{
				CREATURES[COUNT].X = CREATURES[COUNT].POTENTIAL_X;
				CREATURES[COUNT].Y = CREATURES[COUNT].POTENTIAL_Y;
               WORLD[(int)CREATURES[COUNT].X][(int)CREATURES[COUNT].Y] = COUNT;
			}

		    if(CREATURES[COUNT].AGE >= MAX_AGE)
			{
				CREATURES[COUNT].HEALTH-=1;
			}

            if(CREATURES[COUNT].HEALTH <= 0)
			{
				CREATURES[COUNT].KILL();
			}
		 
			if(CREATURES[COUNT].EXIST == TRUE)CREATURES[COUNT].DRAW();
			//CREATURES[COUNT].SET_POSITION();

			if(CREATURES[COUNT].LAST_MEAL > 100)CREATURES[COUNT].KILL();

   } //<--- creature exist
}/// < ----- Cycling through every possible creature


////	for(int Y = 0;Y<SCREENY;Y++){for(int X = 0;X<SCREENX;X++){if(WORLD[(int)X][(int)Y] != 0)dbDot(X,Y,dbRGB(255,0,0));}}
//for(int Y = 0;Y<SCREENY;Y++){
//	for(int X = 0;X<SCREENX;X++){
//	     if(WORLD[(int)X][(int)Y] == -1){
//			 SDL_RenderDrawPoint(RENDER_HND, X, Y);
//		 }
//		  if(WORLD[(int)X][(int)Y] == -2){
//			   SDL_SetRenderDrawColor(RENDER_HND, 0, 255, 0, 255);
//			   SDL_RenderDrawPoint(RENDER_HND, X, Y);
//		  }
	//
	//	}
	//}

	

//dbUnlockPixels();
return;
}


int _CHECK_SPACE(int XX,int YY){
    int A = WORLD[(int)CREATURE::WRAPX(XX)][(int)(CREATURE::WRAPY(YY))];
	return A;
}

//=================FOR DEBUGGING, IT ALLOWS YOU TO CONTROL CREATURE 1 IF NEEDED. =======================================================================================

void _CONTROL_CREATURE()
{
	int COUNT;
/*	
	int OX = CREATURES[1].X,OY = CREATURES[1].Y;
	WORLD[OX][OY] = 0;
	if(dbUpKey())CREATURES[1].MOVE(1);
	if(dbDownKey())CREATURES[1].MOVE(-1);
	if(dbRightKey())CREATURES[1].ROTATE(1);
	if(dbLeftKey())CREATURES[1].ROTATE(-1);
	
	CREATURES[1].SIGHT();
	CREATURES[1].HIT();
	if(CREATURES[1].HIT()){
		CREATURES[1].X = OX;
		CREATURES[1].Y = OY;
	
	}
	CREATURES[1].DRAW();
	CREATURES[1].SET_POSITION();*/
	return;
}


void _PRINT_STATS()
{
			//float speed =  GetTickCount() - MAIN_LOOP_SPEED; dbPrint(speed);
cout << string( 100, '\n' );
        
       SDL_GetMouseState(&MOUSE_POSITION.x, &MOUSE_POSITION.y);
    	cout <<  SDL_GetTicks() - MAIN_LOOP_SPEED << endl;
    	/*cout << (NUM_CREATURES) << "  DEATHS: " << DEATHS << "   Number of Births" << BIRTHS << endl;
    	cout << CREATURES[1].BRAIN[0][1] << " " <<  CREATURES[1].BRAIN[0][2] << CREATURES[1].BRAIN[0][3] << " " <<  CREATURES[1].BRAIN[0][4] << "\n";
    	cout << CREATURES[1].BRAIN[1][1] << " " <<  CREATURES[1].BRAIN[1][2] << CREATURES[1].BRAIN[1][3] << " " <<  CREATURES[1].BRAIN[1][4] << "\n";
    	cout << CREATURES[1].BRAIN[2][1] << " " <<  CREATURES[1].BRAIN[2][2] << CREATURES[1].BRAIN[2][3] << " " <<  CREATURES[1].BRAIN[2][4] << "\n";
    	cout << " ______________________________________" << endl;
    	cout << "X:" << MOUSE_POSITION.x << " Y:" << MOUSE_POSITION.y << endl;

		//	cout << speed << endl; */
		
		return;
}


int  _INIT(){


	if (SDL_Init(0) != 0) {
        printf("Error initializing SDL:  %s\n", SDL_GetError());
        return 1;
    }







    if(SDL_Init(SDL_INIT_VIDEO) !=0) cout << "Init Video Error" << endl;

	if(SDL_CreateWindowAndRenderer(SCREENX, SCREENY, 0, &WINDOW_HWND, &RENDER_HND) !=0)cout << "Init Render and Window Error" << endl;
    if(SDL_SetRenderDrawColor(RENDER_HND, 0, 0, 0, 100) != 0)cout << "Set Color Error" << endl;
    SDL_RenderPresent(RENDER_HND);


	SDL_SetRenderDrawColor(RENDER_HND, 0,0, 0, 0);
    SDL_RenderDrawPoint(RENDER_HND, 100, 100);
	SDL_GL_SwapWindow(WINDOW_HWND);

	//SDL_HINT_FRAMEBUFFER_ACCELERATION
	
	SDL_Texture *MAP = SDL_CreateTexture(RENDER_HND,SDL_PIXELFORMAT_ARGB8888, SDL_TEXTUREACCESS_STATIC, SCREENX, SCREENY);
	
	SDL_SetRenderTarget(RENDER_HND,MAP);
	WINDOW_SURFACE = SDL_GetWindowSurface(WINDOW_HWND);

	Uint32 *pixels = (Uint32 *)WINDOW_SURFACE->pixels;
	
//SDL_GLContext SDL_GL_CreateContext(SDL_Window* window)

	SDL_SetWindowResizable(WINDOW_HWND,SDL_TRUE );

	for(int count = 0;count < (MAX_CREATURES);count++){ //Initalize Starting creatures
		RGBA_COLOR col;col.R = rand()%256;col.G = rand()%256;col.B = rand()%256;

        CREATURES[count] = CREATURE(0, rand() % WORLD_WIDTH,  rand() % WORLD_HEIGHT, rand() % 6, rand() % 360,rand() % 2, col, rand() % 30,rand()%15) ;   
 	}


	cout << "Made Creatures" << endl;
	for(int count = 0;count < AMOUNT_FOOD;count++){
		int x= rand()%SCREENX;
	    int y= rand()%SCREENY;
		WORLD[x][y] = -2;
	}

	cout << "Drew Food" << endl;
	for(int COUNT = 0;COUNT < 1000;COUNT++){ // MAKE THE INITIAL MAZE
		int length = rand() % 200;
		int X = CREATURE::WRAPX(rand()%SCREENX);
		int Y = CREATURE::WRAPX(rand()%SCREENY);
		for (int c = 0;c < length;c++){
		 X += rand()%3 - 1;
		 X = CREATURE::WRAPX(X);
		 Y += rand()%3 - 1;
		 Y = CREATURE::WRAPY(Y);
			//WORLD[X][Y] = -1;
		}
	}
	cout << "Made maze" << endl;


}



//int _INIT(){

//WndClass WCLASS; 
//	 &WINDOW_HWND, 
//  WINDOW_HWND = CreateWindow();   
//
//HDC RENDER_HND = GetDC(WINDOW_HWND); //&RENDER_HND;
// 
//// Release DC
//ReleaseDC(hwnd, hdc);
//return 1;}
//
//
//
//HDC hdc = GetDC(hwnd);
// 
//
// 
//ReleaseDC(hwnd, hdc);
//
//
//




/*-----------------------------Print Various Debug Information --------------------------------------------------------------
    	dbSetCursor(0,0);
    	cout <<  GetTickCount() - MAIN_LOOP_SPEED << endl;
    	cout << (NUM_CREATURES) << "  DEATHS: " << DEATHS << "   Number of Births" << BIRTHS << endl;
    	cout << BRAINS[1][0][1] << " " <<  BRAINS[1][0][2] << BRAINS[1][0][3] << " " <<  BRAINS[1][0][4] << endl;
    	cout << BRAINS[1][1][1] << " " <<  BRAINS[1][1][2] << BRAINS[1][1][3] << " " <<  BRAINS[1][1][4] << endl;
    	cout << BRAINS[1][2][1] << " " <<  BRAINS[1][2][2] << BRAINS[1][2][3] << " " <<  BRAINS[1][2][4] << endl;
    	cout << " ______________________________________" << endl;
    	cout << "X:" << dbMouseX() << " Y:" << dbMouseY() << endl;
//----------------------------------------------------------------------------------------------------------------------------*/

/*
	bool BCOLLISION;
	float RESPONSEX,RESPONSEY;


for(A = ANG;A  < ANG + 180;A +=1){
	 NX = X + DIST * cos(A * 3.14159/180 );
     NY = Y + DIST * sin(A * 3.14159/180 );
	RESPONSEX += RESPONSEX - NX;
	RESPONSEY += RESPONSEY - NY;
	COLLISION = WORLD[(int)WRAPX(NX)][(int)WRAPY(NY)];
	if((COLLISION != 0) &&  (COLLISION != ID)){
		



		//WORLD[(int)X][(int)Y]= 0;
//for(A = ANG;A  < ANG + 180;A +=1){ // 10 IS THE THETA FIX THIS
//	 NX = X + DIST * cos(A * 3.14159/180 );
//	 NY = Y + DIST * sin(A * 3.14159/180 );
//#ifdef COL
//	 dbDot(NX,NY,dbRGB(255,0,0));
//#endif
//	COLLISION = WORLD[(int)WRAPX(NX)][(int)WRAPY(NY)];
//	if((COLLISION != 0) &&  (COLLISION != ID)){ANGLE = -(ANGLE); return COLLISION;}
//}
//







	}*/