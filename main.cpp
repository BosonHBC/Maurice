//
//	Subject:		SM2603 - 2D Game Programming
//	Assignment:		PP3
//	Student Name:	HUANG Bo Cheng
//	Student Number: 54014163
//	Date:	2016/10/27
//
//  Filename: main.cpp
//
//  Note: This example Prototype of final project
//
//	Program Description:
//	
//		This C++/SDL code simulate a basic function of my final project, like character moving, trap triggering and so on
//
//

// These 4 lines link in the required SDL components for our project. 
// Alternatively, we could have linked them in our project settings.    
#pragma comment(lib, "SDL.lib")
#pragma comment(lib, "SDLmain.lib")
#pragma comment(lib, "SDL_image.lib")
#pragma comment(lib, "SDL_mixer.lib")
#pragma comment(lib, "SDL_ttf.lib")

//
// include SDL header
//
#include <stdio.h>
#include <stdlib.h>
#include <iostream>
#include <time.h> 
#include "SDL/SDL.h"
#include "SDL/SDL_image.h"
#include "SDL/SDL_mixer.h"
#include "SDL/SDL_ttf.h"
using namespace std;

//
//	Define game constant
//	

const int SCREEN_WIDTH	= 1280;			// screen dimension
const int SCREEN_HEIGHT	= 960;

const int MAU_IMAGE_SIZE = 32;		// 32 x 32 pixels
const int MAU_MOVE_DIST = 4;		// the Maurice moving at 4 pixel at a time
const int MAU_JUMP_FORCE = 12;		// the force when Maurice Jump
const int GRAVITY = 6;				// the force pointing down
const int NUM_MAU_STAY_FRAME = 2;	// the no. of animating frame for Maurice stay
const int NUM_MAU_MOVE_FRAME = 12;	// the no. of animating frame for Maurice stay

// the initial chicken position
const int INIT_MAU_POS_X = 30;//((SCREEN_WIDTH - MAU_IMAGE_SIZE) / 2);
const int INIT_MAU_POS_Y = 700-MAU_IMAGE_SIZE;//(SCREEN_HEIGHT - MAU_IMAGE_SIZE);


const int UP	= 0;				// up key
const int DOWN	= 1;				// down key
const int RIGHT	= 2;				// right key
const int LEFT	= 3;				// left key



int	g_bLoopDone = false;			// true if the quit the game loop
int g_bResume = true;

//
//	Define local function prototypes
//
SDL_Surface*	Load_Image(char *szFname);
Mix_Chunk*		Load_Sound(char *szFname);
void			Draw_Image(SDL_Surface *draw_Img, int iPosi_X, int iPosi_Y, SDL_Rect dest, SDL_Surface *pScreen);

//
//	main() - main program.
//
int 
	main(int argc, char *argv[])
{
	SDL_Surface *pScreen;	// Game primary screen

	// Pointers to chicken image
	SDL_Surface *apMau_Stay_Left[2];	// Image Stay Left
	SDL_Surface *apMau_Stay_Right[2];	// Image Stay Right
	SDL_Surface *pMau_Image_Hurt;		// Image Hurt
	SDL_Surface *apMau_Move_Left[12];	// Image Movw Left
	SDL_Surface *apMau_Move_Right[12];	// Image Move Right

	// Background
	SDL_Surface *pGround;

	// Chicken variables
	int iMauPosX = INIT_MAU_POS_X;	// Maurice position in x
	int iMauPosY = INIT_MAU_POS_Y;	// Maurice position in y
	int iMauFacing = RIGHT;				// Maurice facing direction
	int iMovingFrame = 0;				// Maurice moving frame
	int iStayingFrame = 0;			// Maurice staying frame
	bool bGround = false;
	bool bJumping = false;
	bool bMovingL = false;
	bool bMovingR = false;
	bool bStaying = true;

	// Timer variables
	Uint32 dwStartTime;		// start time
	Uint32 dwEndTime;		// end time

	Uint32 dwJumpStartTime;	// jump start time
	Uint32 dwJumpEndTime;	// jump end time


	int bKeyPressed[4];		// record the arrow key pressed
	SDL_Event	event;		// SDL event
	SDL_Rect	dest;		// for drawing use

	//
	// Initialize SDL Video and Audio
	//
	if (SDL_Init(SDL_INIT_VIDEO | SDL_INIT_AUDIO) < 0)
	{
		fprintf(stderr, "Error init'ing SDL: %s\n", SDL_GetError());
		exit(1);
	}

	//
	// Setup the primary display: 640 x 480, double buffer
	//
	pScreen = SDL_SetVideoMode(SCREEN_WIDTH, SCREEN_HEIGHT, 0, SDL_DOUBLEBUF);

	if (pScreen == NULL)
	{
		fprintf(stderr, "Error: Can't open window! %s\n", SDL_GetError());
		exit(1);
	}


	//
	// Load in chicken graphics
	//
	apMau_Stay_Left[0]		= Load_Image("mauriceL1.png");
	apMau_Stay_Left[1]		= Load_Image("mauriceL2.png");
	apMau_Stay_Right[0]		= Load_Image("mauriceR1.png");
	apMau_Stay_Right[1]		= Load_Image("mauriceR2.png");

	apMau_Move_Right[0]		= Load_Image("MoveR/mauriceMoveR1.png");
	apMau_Move_Right[1]		= Load_Image("MoveR/mauriceMoveR2.png");
	apMau_Move_Right[2]		= Load_Image("MoveR/mauriceMoveR3.png");
	apMau_Move_Right[3]		= Load_Image("MoveR/mauriceMoveR4.png");
	apMau_Move_Right[4]		= Load_Image("MoveR/mauriceMoveR5.png");
	apMau_Move_Right[5]		= Load_Image("MoveR/mauriceMoveR6.png");
	apMau_Move_Right[6]		= Load_Image("MoveR/mauriceMoveR7.png");
	apMau_Move_Right[7]		= Load_Image("MoveR/mauriceMoveR8.png");
	apMau_Move_Right[8]		= Load_Image("MoveR/mauriceMoveR9.png");
	apMau_Move_Right[9]		= Load_Image("MoveR/mauriceMoveR10.png");
	apMau_Move_Right[10]	= Load_Image("MoveR/mauriceMoveR11.png");
	apMau_Move_Right[11]	= Load_Image("MoveR/mauriceMoveR12.png");

	apMau_Move_Left[0]		= Load_Image("MoveL/mauriceMoveL1.png");
	apMau_Move_Left[1]		= Load_Image("MoveL/mauriceMoveL2.png");
	apMau_Move_Left[2]		= Load_Image("MoveL/mauriceMoveL3.png");
	apMau_Move_Left[3]		= Load_Image("MoveL/mauriceMoveL4.png");
	apMau_Move_Left[4]		= Load_Image("MoveL/mauriceMoveL5.png");
	apMau_Move_Left[5]		= Load_Image("MoveL/mauriceMoveL6.png");
	apMau_Move_Left[6]		= Load_Image("MoveL/mauriceMoveL7.png");
	apMau_Move_Left[7]		= Load_Image("MoveL/mauriceMoveL8.png");
	apMau_Move_Left[8]		= Load_Image("MoveL/mauriceMoveL9.png");
	apMau_Move_Left[9]		= Load_Image("MoveL/mauriceMoveL10.png");
	apMau_Move_Left[10]		= Load_Image("MoveL/mauriceMoveL11.png");
	apMau_Move_Left[11]		= Load_Image("MoveL/mauriceMoveL12.png");


	// load back ground
	pGround					= Load_Image("background/ground.png");


	do
	{

		// Record the current time
		dwStartTime = SDL_GetTicks();

		//
		// Handle they keyboard events here
		//
		while (SDL_PollEvent(&event) > 0)
		{
			if (event.type == SDL_QUIT)
			{
				// Quit event! (Window close, kill signal, etc.)
				g_bLoopDone = true;
			}
			// Check for key pressed
			else if (event.type == SDL_KEYDOWN)
			{
				// Check individual key
				SDLKey key = event.key.keysym.sym;

				if (key == SDLK_ESCAPE && g_bResume)
				{
					// Pause on escape key

				}
				else if(key == SDLK_r)
				{
					// Restart new round

				}
				else if (key == SDLK_UP || key == SDLK_DOWN ||
					key == SDLK_RIGHT || key == SDLK_LEFT)
				{					
					// record the key stroke
					bKeyPressed[(int)(key - SDLK_UP)] = true;

					// change the chicken facing base on the left/right key
					if (key == SDLK_LEFT && !bMovingR )
					{
						iMauFacing = LEFT;
						bMovingL = true;
						bStaying = false;
					}
					else if (key == SDLK_RIGHT && !bMovingL )
					{
						iMauFacing = RIGHT;
						bMovingR = true;						
						bStaying = false;
					}

					//bMovingL = ()?true:false;

					// First JUMP
					
				}

				if(key == SDLK_SPACE)
					{
						if(!bJumping)
						{
							bJumping = true;
							dwJumpStartTime = SDL_GetTicks();
						}
						dwJumpEndTime = SDL_GetTicks();

						if(dwJumpEndTime - dwJumpStartTime <= 10000)
						{
						iMauPosY -= MAU_JUMP_FORCE;
						}

					}
			}


			// Check for key released
			else if (event.type == SDL_KEYUP)
			{
				SDLKey key = event.key.keysym.sym;

				if (key == SDLK_UP || key == SDLK_DOWN ||
					key == SDLK_RIGHT || key == SDLK_LEFT)
				{

					// record the key stroke
					bKeyPressed[(int)(key - SDLK_UP)] = false;
				}

				if(key == SDLK_LEFT)
				{
					bMovingL = false;
				}

				if(key == SDLK_RIGHT)
				{
					bMovingR = false;
				}

				if(key == SDLK_SPACE)
				{
					bJumping = false;

				}
			}




		}

		if (g_bResume)
		{


			//
			// Try move the Maurice
			//

			// if maurice is moving(key down)
			if(bMovingL || bMovingR){	

				if (bKeyPressed[RIGHT] && bMovingR) // move Maurice
				{
					iMauPosX += MAU_MOVE_DIST;

					// avoid leaving the screen
					if (iMauPosX > (SCREEN_WIDTH - MAU_IMAGE_SIZE))
						iMauPosX = SCREEN_WIDTH - MAU_IMAGE_SIZE;

				}
				else if (bKeyPressed[LEFT] && bMovingL)
				{
					iMauPosX -= MAU_MOVE_DIST;

					// avoid leaving the screen
					if (iMauPosX < 0)
						iMauPosX = 0;

				}
				// When moving, increase the frame of moving animation
				iMovingFrame = (iMovingFrame + 1) % NUM_MAU_MOVE_FRAME;
			}
			// if Maurice is on the ground, not moving, staying on the ground
			else if(!bMovingL && !bMovingR && bStaying)
			{
				iStayingFrame = (iStayingFrame + 1)%NUM_MAU_STAY_FRAME;
			}

			// if Maurice is in the air, add gravity
			if(!bGround)
			{
				//iMauPosY += GRAVITY;
			}
			else			// else no gravity
			{

			}

		}

		//
		// Redraw the whole screen with the gray color
		//
		SDL_FillRect(pScreen, NULL, SDL_MapRGB(pScreen->format, 128, 128, 128));


		//
		// Draw the chicken here
		//
		dest.x = iMauPosX;
		dest.y = iMauPosY;

		if (iMauFacing == LEFT && !bMovingL && !bMovingR)
			SDL_BlitSurface(apMau_Stay_Left[iStayingFrame], NULL, pScreen, &dest);

		else if (iMauFacing == RIGHT && !bMovingL && !bMovingR)
			SDL_BlitSurface(apMau_Stay_Right[iStayingFrame], NULL, pScreen, &dest);	

		else if (iMauFacing == LEFT && bMovingL)
			SDL_BlitSurface(apMau_Move_Left[iMovingFrame], NULL, pScreen, &dest);

		else if (iMauFacing == RIGHT && bMovingR)
			SDL_BlitSurface(apMau_Move_Right[iMovingFrame], NULL, pScreen, &dest);	

		// Draw background
		Draw_Image(pGround,0,800,dest,pScreen);

		// Update the display
		SDL_Flip(pScreen);

		//
		// Pause at the end if necessary to control the frame rate
		//
		dwEndTime = SDL_GetTicks();
		if (dwEndTime < dwStartTime + (1000 / 60))
		{
			SDL_Delay(dwStartTime + (1000 / 60) - dwEndTime);
		}

	} while (!g_bLoopDone);


	// Close up SDL!
	SDL_Quit();

	return(0);
}



//
//	Extra function
//
void Draw_Image(SDL_Surface *draw_Img, int iPosi_X, int iPosi_Y, SDL_Rect dest, SDL_Surface *pScreen)
{
	dest.x = iPosi_X;
	dest.y = iPosi_Y;
	SDL_BlitSurface(draw_Img, NULL, pScreen, &dest);
}

//
//	Load_Image() - load an image, abort if error.
//
SDL_Surface* 
	Load_Image(char *szFname)
{
	SDL_Surface *pSurface = NULL;

	pSurface = IMG_Load(szFname);

	if (pSurface == NULL)
	{
		fprintf(stderr, "Error loading image %s: %s\n", szFname, IMG_GetError());
		exit(1);
	}

	return pSurface;
}



//
//	Load_Sound() - Load a wav sound file, abort if fail.
//
Mix_Chunk* 
	Load_Sound(char *szFname)
{
	Mix_Chunk *pChunk = NULL;

	pChunk = Mix_LoadWAV(szFname);

	if (pChunk == NULL)
	{
		fprintf(stderr, "Error loading sound %s: %s\n", szFname, Mix_GetError());
		exit(1);
	}

	return pChunk;
}