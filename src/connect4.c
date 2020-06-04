
#include "tm4c123gh6pm.h"
#include "Nokia5110.h"
#include "Random.h"
#include "TExaS.h"
#include "img.h"
#include <stdio.h>
void Delay100ms(unsigned long count);
	
//port f
void PortF_Init(void);


int const cellW = 8; //cell width
int const cellH = 5; //cell height
int const hLineW = 1; //horizontal line width
int const vLineW = 2; // vertical line width
int const numOfCol = 7; 
int const numOfRow = 6;
int const numOfCoinsForEachPlayer = 21; // (numOfCol*numOfRow)/2
int const coinH = 3; //cell height
int const coinW = 6; //cell width
int leftMargin; //empty space on the left of the grid
int topMargin; //empy space on the top of the grid
int fullGridH; //grid height
int fullGridW; //grid width
int turn; //current turn
int lastTurn; //turn in the previous loop
int cellCenX; //half width of the cell
int cellCenY; //half height of the cell
int colCoins[numOfCol]; //number of coins in each column
int colCenter[numOfCol]; //each column center on x axis
int playerPos; //position of the player coin before playing
int winner; // who is the winner
int gameMode; // 0: menu, 1: 2players, 2: 1player vs ai,  3: ai vs ai
int menuCursor;
int i;
int	j;
int isMaster;
int willWePlayFirst;
int ai;
int currPlayer;
int opponentPlayerNum;
int isMenuMode;
int menuNum;
int SW1;
int SW2;
int codingMode;
int cellReq;
//for the communication
const int allowedNumOftrialsToOut = 10;
const int allowedNumOftrialsToInput = 1000000;
int numOfTrialsOut;
int numOfTrialsIn;
unsigned char inputFromTheSecondDevice;
unsigned char outputToTheSecondDevice;
//the protocole
unsigned char handshake = 17;
unsigned char confirmation = 200;
unsigned char masterConf = 20;
unsigned char player1 = 31;
unsigned char player2 = 32;
unsigned char invalid = 24;
unsigned char youWin = 24;
unsigned char iWin = 24;
unsigned char tie = 24;
unsigned char temp;
const unsigned char logo1[] ={
 0x42, 0x4D, 0x4A, 0x04, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x76, 0x00, 0x00, 0x00, 0x28, 0x00, 0x00, 0x00, 0x38, 0x00, 0x00, 0x00, 0x23, 0x00, 0x00, 0x00, 0x01, 0x00, 0x04, 0x00, 0x00, 0x00,
 0x00, 0x00, 0xD4, 0x03, 0x00, 0x00, 0xC4, 0x0E, 0x00, 0x00, 0xC4, 0x0E, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x80, 0x00, 0x00, 0x80,
 0x00, 0x00, 0x00, 0x80, 0x80, 0x00, 0x80, 0x00, 0x00, 0x00, 0x80, 0x00, 0x80, 0x00, 0x80, 0x80, 0x00, 0x00, 0x80, 0x80, 0x80, 0x00, 0xC0, 0xC0, 0xC0, 0x00, 0x00, 0x00, 0xFF, 0x00, 0x00, 0xFF,
 0x00, 0x00, 0x00, 0xFF, 0xFF, 0x00, 0xFF, 0x00, 0x00, 0x00, 0xFF, 0x00, 0xFF, 0x00, 0xFF, 0xFF, 0x00, 0x00, 0xFF, 0xFF, 0xFF, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x88, 0x80, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x88, 0x80, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x88, 0x80, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x88, 0x80, 0x00, 0x00, 0x07, 0x77, 0x70, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
 0x00, 0x00, 0x00, 0x00, 0x00, 0x08, 0x88, 0x80, 0x00, 0x00, 0x08, 0xF8, 0xF7, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
 0x00, 0x08, 0x88, 0x80, 0x00, 0x00, 0x88, 0x88, 0x88, 0x00, 0x03, 0x00, 0x00, 0x08, 0x80, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x08, 0x88, 0x88,
 0x88, 0x00, 0x88, 0x88, 0x8F, 0x00, 0x8F, 0xF7, 0x00, 0x0F, 0x80, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x08, 0x88, 0x88, 0x88, 0x80, 0x78, 0x88,
 0xFF, 0x07, 0xFF, 0xFF, 0x00, 0x08, 0x80, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x08, 0x88, 0x88, 0x88, 0x80, 0x07, 0x8F, 0xF0, 0x07, 0xF8, 0x7F,
 0x00, 0x88, 0x80, 0x00, 0x80, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x08, 0x88, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x08, 0xF7, 0x0F, 0x00, 0x88, 0x30, 0x88,
 0x80, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x08, 0x88, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x08, 0xF7, 0x08, 0x00, 0x88, 0x88, 0x8F, 0x70, 0x00, 0x00, 0x00,
 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x88, 0x88, 0x00, 0x00, 0x00, 0x07, 0x88, 0x70, 0x0F, 0xF0, 0x08, 0x08, 0x80, 0x0F, 0x87, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
 0x00, 0x00, 0x00, 0x00, 0x00, 0x88, 0x88, 0x80, 0x00, 0x00, 0x78, 0x88, 0x88, 0x0F, 0xF0, 0x0F, 0x08, 0x80, 0x07, 0x80, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
 0x00, 0x88, 0x88, 0x88, 0x88, 0x00, 0xFF, 0x88, 0x8F, 0x00, 0x00, 0x08, 0x08, 0x80, 0x07, 0x80, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x08, 0x88, 0x88,
 0x88, 0x00, 0x88, 0x88, 0xF8, 0x00, 0x00, 0x00, 0x08, 0x88, 0x7F, 0x70, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x08, 0x88,
 0xF7, 0x00, 0x00, 0x00, 0x00, 0x88, 0x88, 0x70, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x77, 0x70, 0x00, 0x00, 0x00,
 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x07, 0x28, 0x87, 0x70, 0x00, 0x00, 0x00, 0x00, 0x08, 0x80, 0x07, 0x07, 0x70, 0x08, 0x70,
 0x7F, 0x8F, 0x70, 0x07, 0x78, 0x00, 0x00, 0x06, 0x00, 0x00, 0x00, 0x00, 0x00, 0x78, 0x88, 0x88, 0xF8, 0x70, 0x00, 0x00, 0x00, 0x08, 0x80, 0x08, 0x07, 0x80, 0x88, 0x70, 0x7F, 0x87, 0x70, 0x88,
 0x88, 0x80, 0x00, 0xF7, 0x00, 0x00, 0x00, 0x00, 0x07, 0xFF, 0xFF, 0x77, 0x78, 0x70, 0x00, 0x88, 0x73, 0x0F, 0x80, 0x88, 0x07, 0x70, 0x88, 0x70, 0x78, 0x00, 0x08, 0xF8, 0x77, 0x80, 0x00, 0xF7,
 0x00, 0x00, 0x00, 0x00, 0x07, 0xF8, 0x87, 0x00, 0x07, 0x70, 0x08, 0x88, 0x88, 0x08, 0x88, 0x88, 0x07, 0x78, 0x88, 0x70, 0x7F, 0x88, 0x08, 0xF7, 0x00, 0x00, 0x00, 0xF7, 0x00, 0x00, 0x00, 0x00,
 0x77, 0x88, 0x87, 0x00, 0x00, 0x70, 0x88, 0x88, 0x88, 0x08, 0x88, 0x88, 0x07, 0x88, 0x87, 0x70, 0x7F, 0xF8, 0x08, 0xF7, 0x00, 0x00, 0x00, 0xF7, 0x00, 0x00, 0x00, 0x00, 0x68, 0x88, 0x70, 0x00,
 0x00, 0x00, 0x88, 0x88, 0x88, 0x08, 0x88, 0x88, 0x07, 0x88, 0x00, 0x70, 0x88, 0x70, 0x08, 0xF7, 0x00, 0x20, 0x00, 0xF7, 0x00, 0x00, 0x00, 0x00, 0x68, 0x88, 0x70, 0x00, 0x00, 0x00, 0x08, 0x88,
 0x87, 0x08, 0x88, 0x08, 0x08, 0x87, 0x00, 0x80, 0xF8, 0x77, 0x08, 0xF7, 0x33, 0x00, 0x08, 0xF8, 0x70, 0x00, 0x00, 0x00, 0x68, 0x88, 0x70, 0x00, 0x00, 0x00, 0x00, 0xF8, 0x00, 0x08, 0x80, 0x08,
 0x07, 0xF7, 0x03, 0x80, 0x7F, 0x88, 0x00, 0xFF, 0x78, 0x00, 0x8F, 0xFF, 0xF8, 0x00, 0x00, 0x00, 0x78, 0x88, 0x80, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x08, 0x00, 0x08, 0x00, 0x80, 0x00, 0x00,
 0x8F, 0xFF, 0x70, 0x8F, 0xFF, 0x00, 0xFF, 0x88, 0xF7, 0x00, 0x00, 0x00, 0x08, 0x8F, 0xF7, 0x00, 0x06, 0x70, 0x00, 0x08, 0x70, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x77, 0x00, 0x00,
 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x07, 0xFF, 0xF7, 0x00, 0x07, 0x70, 0x78, 0x8F, 0x87, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
 0x00, 0x00, 0x00, 0x00, 0x07, 0xFF, 0x8E, 0x70, 0x68, 0x00, 0x78, 0x88, 0x88, 0x30, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
 0x00, 0x0F, 0x8E, 0x88, 0xF8, 0x00, 0x78, 0x88, 0x88, 0x30, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x07, 0x78, 0x87,
 0x70, 0x00, 0x7F, 0x88, 0x8F, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x08, 0xFF,
 0x84, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xFF,

};

//this structure describes each individual cell
//(x, y) are the center point of a cell
//player = 0 >> cell is empty
//player = 1 >> cell is taken by player one
//player = 2 >> cell is taken by player two
struct cell{
	int x;
	int y;
	int player;
};

//the full grid cells
struct cell theGrid[numOfRow][numOfCol];


//structure that describes the coins
//(x, y) are the center point of a coin
//image: the image of a coin
//draw(): draws a coin at its position
struct coin {
	int x;
	int y;
	const unsigned char *image;
	
	void (*draw)(struct coin*);
};

void draw(struct coin* c){
	Nokia5110_PrintBMP((*c).x - coinW/2, (*c).y + (coinH/2), (*c).image, 0);
	
}


//conis for each players
//playersCoins[0]: first player
//playersCoins[1]: second player
struct coin playersCoins[2][numOfCoinsForEachPlayer];





void gameInit(){
	
	willWePlayFirst = 1;
	isMenuMode = 1;
	menuNum = 0;
	codingMode = 0;
		
	//the grid dimintions
	fullGridW = (cellW * numOfCol + vLineW*(numOfCol+1));
	fullGridH = (cellH*numOfRow +  hLineW*numOfRow);
	
	
	//get margins
	leftMargin = (SCREENW - fullGridW)/2;
	topMargin = SCREENH - fullGridH;
	
	//individual cell center
	cellCenX = vLineW + (cellW/2);
	cellCenY = hLineW + (cellH/2);
	
	playerPos = 0;
	
	//initialize the grid
	for(i = 0; i < numOfRow; i++){
		for(j = 0; j < numOfCol; j++){
			//calculate center position of each cell
			theGrid[numOfRow - 1 - i][j].x = leftMargin + j*(cellW + vLineW) + cellCenX;
			theGrid[numOfRow - 1 - i][j].y = SCREENH - 1 - (i*(cellH + hLineW) + cellCenY);
			
			//player = 0 mean the cell is empty
			theGrid[numOfRow - 1 - i][j].player = 0;
		}
	}
	
	//initialize the turn
	turn = 0;
	lastTurn = 0;
	
	
	//initialize players coins
	
	for(i = 0; i < numOfCoinsForEachPlayer; i++){
		
		//locate the coins on the top left of the grid and in the center of the 1st column
		//player one
		playersCoins[0][i].x = leftMargin + cellCenX;
		playersCoins[0][i].y = SCREENH - 1 - fullGridH - 3;
		//player two
		playersCoins[1][i].x = leftMargin + cellCenX;
		playersCoins[1][i].y = SCREENH - 1 - fullGridH - 3;
		
		
		//set the img for each player
		//player one
		playersCoins[0][i].image = pl1coin;
		//player two
		playersCoins[1][i].image = pl2coin;
		
		
	}
	
	//each column contains 0 coins at the begining
	for(i = 0; i < numOfCol; i++){
		colCoins[i] = 0;
		
		colCenter[i] = leftMargin + i*(cellW + vLineW) + cellCenX;
	}
	
	gameMode = 0;
}






void DrawGrid(){
	
	//draw vertical lines
	int yPos = SCREENH - 1;
	int xPos = 0;
	int i = 0;
	while( i <= numOfCol){
		xPos = i*(cellW + vLineW);
		Nokia5110_PrintBMP(xPos + leftMargin,  yPos, vvLine, 0);
		i++;
	}
	
	
	//draw vertical lines
	yPos = 0;
	xPos = leftMargin;
	i = 0;
	while( i < numOfRow){
		yPos = SCREENH - ( 1 +  i*(cellH + hLineW));
		Nokia5110_PrintBMP(xPos, yPos, hLine, 0);
		
		i++;
	}
}

void update(){
	Nokia5110_ClearBuffer();
	
	
	//show grid
	DrawGrid();
	
	//show current player
	if(!winner)
		draw(&playersCoins[currPlayer][turn/2]);
	
	
	//show the coins inside the grid
	
	for(i = 0; i < turn; i++){
		draw(&playersCoins[i%2][i/2]);
	}
	
	
	Nokia5110_DisplayBuffer();
	

}




//check if there is a winner
//return 1 if player one is the winner
//return 2 if player two is the winner
int isThereAwinner(){
	int status = 0;
	for(i = 0; i < numOfRow; i++){
		for(j = 0; j < numOfCol; j++){
			//check vertically
			if(i + 3 < numOfRow){
				if(
					theGrid[i][j].player == theGrid[i+1][j].player &&
					theGrid[i+1][j].player == theGrid[i+2][j].player &&
					theGrid[i+2][j].player == theGrid[i+3][j].player &&
					theGrid[i][j].player != 0
					){
						status = theGrid[i][j].player;
						break;
					}
			}
			
			//horizontally
			if(j + 3 < numOfCol){
				if(
					theGrid[i][j].player == theGrid[i][j+1].player &&
					theGrid[i][j+1].player == theGrid[i][j+2].player &&
					theGrid[i][j+2].player == theGrid[i][j+3].player &&
					theGrid[i][j].player != 0
					){
						status = theGrid[i][j].player;
						break;
					}
			}
			
			//diagonally right
			if(i + 3 < numOfRow && j + 3 < numOfCol){
				if(
					theGrid[i][j].player == theGrid[i+1][j+1].player &&
					theGrid[i+1][j+1].player == theGrid[i+2][j+2].player &&
					theGrid[i+2][j+2].player == theGrid[i+3][j+3].player &&
					theGrid[i][j].player != 0
					){
						status = theGrid[i][j].player;
						break;
					}
			}
			
			//diagonally left
			if(i + 3 < numOfRow && j - 3 >= 0){
				if(
					theGrid[i][j].player == theGrid[i+1][j-1].player &&
					theGrid[i+1][j-1].player == theGrid[i+2][j-2].player &&
					theGrid[i+2][j-2].player == theGrid[i+3][j-3].player &&
					theGrid[i][j].player != 0
					){
						status = theGrid[i][j].player;
						break;
					}
			}
		}
		
		if(status != 0)
			break;
	}
	
	
	return status ;
}



int checkTriples(){
		cellReq=-1;
		for(i = 0; i < numOfRow; i++){
			for(j = 0; j < numOfCol; j++){
				//check vertically
				if(i + 2 < numOfRow){
					if(
						theGrid[i][j].player == theGrid[i+1][j].player &&
						theGrid[i+1][j].player == theGrid[i+2][j].player &&
						theGrid[i-1][j].player == 0 &&
						theGrid[i][j].player != 0 
						){
							cellReq = j ;
							if(theGrid[i][j].player == ai)
								break;
						}
				}
				
				//horizontal ends 
				if(		//right side
							j + 2 < numOfCol &&
							theGrid[i][j].player == theGrid[i][j+1].player &&
							theGrid[i][j+1].player == theGrid[i][j+2].player &&
							theGrid[i][j+3].player == 0 &&
							theGrid[i][j].player != 0 &&
							((i == 5 ) || (colCoins[j+3] == numOfRow - i - 1))
						){							
								cellReq = j+3;
								if(theGrid[i][j].player == ai)
									break;
							}
				if(		//left side
							j - 1 >= 0 &&
							theGrid[i][j].player == theGrid[i][j+1].player &&
							theGrid[i][j+1].player == theGrid[i][j+2].player &&
							theGrid[i][j-1].player == 0 &&
							theGrid[i][j].player != 0 &&
							((i == 5 ) || (colCoins[j-1] == numOfRow - i - 1 ))

						){
							cellReq = j-1;
							if(theGrid[i][j].player == ai)
									break;
							
							}
				//horizontal middle left   
				if(
						theGrid[i][j].player == theGrid[i][j+2].player &&
						theGrid[i][j+2].player == theGrid[i][j+3].player &&
						theGrid[i][j+1].player == 0  &&
						theGrid[i][j].player != 0 
						){if( 	//left side
								(i == 5 ) || (colCoins[j+1] == numOfRow - i - 1)
							)
								{
									cellReq = j+1;
									if(theGrid[i][j].player == ai)									
										break;
									
								}
						}
				//horizontal middle right
				if(
						theGrid[i][j].player == theGrid[i][j+1].player &&
						theGrid[i][j+1].player == theGrid[i][j+3].player &&
						theGrid[i][j+2].player == 0  &&
						theGrid[i][j].player != 0 
						){if( 	//right side
								(i == 5 ) || (colCoins[j+2] == numOfRow - i - 1)
							)
								{
									cellReq = j+2;
									if(theGrid[i][j].player == ai)
										break;
								}
						}
				//diagonally right
				if(i + 2 < numOfRow && j + 2 < numOfCol){
					if(	//after diagonal
							theGrid[i][j].player == theGrid[i+1][j+1].player &&
							theGrid[i+1][j+1].player == theGrid[i+2][j+2].player &&
							theGrid[i][j].player != 0 &&
							theGrid[i+3][j+3].player == 0 &&
							colCoins[j+3] == numOfRow - i -4 &&
							j+3 < numOfCol &&
							i+3 < numOfRow 
						)
						{
							cellReq = j+3;
							if(theGrid[i][j].player == ai)
										break;
						}
						if(	//before diagonal
 								j-1 >= 0 &&
								i-1 >= 0 &&
								theGrid[i][j].player == theGrid[i+1][j+1].player &&
								theGrid[i+1][j+1].player == theGrid[i+2][j+2].player &&
								theGrid[i][j].player != 0 &&
								theGrid[i-1][j-1].player == 0 &&
								colCoins[j-1] == numOfRow - i
							)
							{
										cellReq = j-1;
										if(theGrid[i][j].player == ai)
										break;
								
									}
							}
						
			
			
			//diagonally left
			if(i + 2 < numOfRow && j - 2 >= 0){
					if(	//after diagonal
							j-3 >= 0 &&
							i+3 < numOfRow &&
							theGrid[i][j].player == theGrid[i+1][j-1].player &&
							theGrid[i+1][j-1].player == theGrid[i-2][j-2].player &&
							theGrid[i][j].player != 0 &&
							theGrid[i+3][j-3].player == 0 &&
							colCoins[j-3] == numOfRow - i -4 
						)
						{
							cellReq = j-3;
							if(theGrid[i][j].player == ai)
										break;
						}
						if(	//before diagonal
 								j+1 < numOfCol &&
								i-1 >= 0 &&
								theGrid[i][j].player == theGrid[i+1][j-1].player &&
								theGrid[i+1][j-1].player == theGrid[i-2][j-2].player &&
								theGrid[i][j].player != 0 &&
								theGrid[i-1][j+1].player == 0 &&
								colCoins[j+1] == numOfRow - i
							)
							{
										cellReq = j+1;
										if(theGrid[i][j].player == ai)
											break;
									}
							}
							//diagonal right middle left   
				if(
						i + 3 < numOfRow && j + 3 < numOfCol &&
						theGrid[i][j].player == theGrid[i+2][j+2].player &&
						theGrid[i+2][j+2].player == theGrid[i+3][j+3].player &&
						theGrid[i+1][j+1].player == 0  &&
						theGrid[i][j].player != 0 &&
						colCoins[j+1] == numOfRow - i	- 2
						){
									cellReq = j+1;
									if(theGrid[i][j].player == ai)									
										break;
						}
				//diagonal right middle right
				if(
						i + 3 < numOfRow && j + 3 < numOfCol &&
						theGrid[i][j].player == theGrid[i+1][j+1].player &&
						theGrid[i+1][j+1].player == theGrid[i+3][j+3].player &&
						theGrid[i+2][j+2].player == 0  &&
						theGrid[i][j].player != 0 &&
						colCoins[j+2] == numOfRow - i	-3
						)
						{
							cellReq = j+2;
							if(theGrid[i][j].player == ai)
								break;
						}

						//diagonal left middle right
				if(
						(i + 3 < numOfRow )&& (j - 3 >= 0 )&&
						theGrid[i][j].player == theGrid[i+2][j-2].player &&
						theGrid[i+2][j-2].player == theGrid[i+3][j-3].player &&
						theGrid[i+1][j-1].player == 0  &&
						theGrid[i][j].player != 0 &&
						colCoins[j-1] == numOfRow - i	- 2
						){
									cellReq = j-1;
									if(theGrid[i][j].player == ai)									
										break;
						}
				//diagonal left middle left
				if(
						(i + 3 < numOfRow )&& (j - 3 >= 0 )&&
						theGrid[i][j].player == theGrid[i+1][j-1].player &&
						theGrid[i+1][j-1].player == theGrid[i+3][j-3].player &&
						theGrid[i+2][j-2].player == 0  &&
						theGrid[i][j].player != 0 &&
						colCoins[j-2] == numOfRow - i	-3
						)
						{
							cellReq = j-2;
							if(theGrid[i][j].player == ai)
								break;
						}
						
						}
					}

	if (cellReq > -1)
		return cellReq;
	else 
		return -1;
}
int shouldPlayWithSw(){
	if(
		gameMode == 1 || //if p1 vs p2
		(gameMode == 2 && opponentPlayerNum != currPlayer) // if p1 vs ai : p1 only should play with the swiches;
		)
	
		return 1;
	else
		return 0;
}



int playInAcol(){
	if(colCoins[playerPos] < numOfRow){
		theGrid[numOfRow - 1 - colCoins[playerPos]][playerPos].player = currPlayer + 1;
		playersCoins[currPlayer][turn/2].y = theGrid[numOfRow - 1 - colCoins[playerPos]][playerPos].y;
		colCoins[playerPos]++;
		turn++;
		return 1;
	}else
		return 0;
	
}

int triplePos;
int decision;
//should return a valid position
int getAiNextPos(){
	triplePos = checkTriples();
	if(triplePos != -1){
				decision = triplePos;
	}
	else if(colCoins[3] != 6){
					decision = 3;
					triplePos=-1;
	}
	else{
		do
			decision = rand()%7;
		while (decision == 3);
	}
	
	return decision;
}

void outputToTheScreen(int x, int y, char s[], int clear){
	if(clear)
		Nokia5110_Clear();
	Nokia5110_SetCursor(x, y);
	Nokia5110_OutString(s);
}
void theMenu(){
		if(menuNum == 0){
				GPIO_PORTF_DATA_R = 0x06;
				Nokia5110_Clear();
				Nokia5110_SetCursor(4, 0);
				Nokia5110_OutString("MENU");
				Nokia5110_SetCursor(2, 2);
				Nokia5110_OutString("P1 vs P2");
				Nokia5110_SetCursor(2, 3);
				Nokia5110_OutString("P1 vs AI");			
				Nokia5110_SetCursor(2, 4);
				Nokia5110_OutString("AI vs AI");

				
				Nokia5110_SetCursor( 0 , menuCursor + 2);
				Nokia5110_OutString(">>"); 
				//wait for an input
					while(SW1 && SW2){
						SW1 = GPIO_PORTF_DATA_R&0x10;
						SW2 = GPIO_PORTF_DATA_R&0x01;
					};
				//move down in menu	
				if(!SW1){
					while(!SW1){SW1 = GPIO_PORTF_DATA_R&0x10;}
					menuCursor = (menuCursor + 1) % 3;
					GPIO_PORTF_DATA_R = 0x00;
					if(!codingMode)  Delay100ms(5);
				}
				//choose selection
				else if(!SW2){
					while(!SW2){SW2 = GPIO_PORTF_DATA_R&0x01;}
					gameMode = menuCursor + 1 ;
					menuNum = 1 ;
					menuCursor = 0;
					if(!codingMode)  Delay100ms(5);
				}
				Nokia5110_SetCursor( 0 ,menuCursor + 2);
				Nokia5110_OutString(">>"); 
					}
			else if (menuNum == 1){
				isMenuMode = 0;

			}
			

			
		}



const long ColorWheel[8] = {0x02,0x0A,0x08,0x0C,0x04,0x06,0x0E,0x00};
long prevSW1 = 0;        // previous value of SW1
long prevSW2 = 0;        // previous value of SW2
unsigned char inColor;   // color value from other microcontroller
unsigned char color = 0; // this microcontroller's color value

char x;
int main(void){

  TExaS_Init(SSI0_Real_Nokia5110_Scope);  // set system clock to 80 MHz
  Random_Init(1);
  Nokia5110_Init();
  Nokia5110_ClearBuffer();
	Nokia5110_DisplayBuffer();      // draw buffer
	PortF_Init();
	
	gameInit();
	
	//UART1_OutChar((char)50);
	
	gameMode = 0;
	menuCursor = 0;
	
	
	willWePlayFirst = 1;
	ai = willWePlayFirst;
	
	
	if(!codingMode){
		/*Nokia5110_ClearBuffer();
		Nokia5110_DisplayBuffer();
		Nokia5110_SetCursor(2,3);
		Nokia5110_OutString("Connect4");	
		Delay100ms(5);
		Nokia5110_SetCursor(2,3);
		Nokia5110_OutString("           ");
		Delay100ms(5);
		Nokia5110_SetCursor(2,3);
		Nokia5110_OutString("welcome");	
		Delay100ms(5);*/
		int w; 
		Nokia5110_SetCursor(0,0);
		Nokia5110_ClearBuffer();
		Nokia5110_PrintBMP(10, 35, logo1, 2);
		Nokia5110_DisplayBuffer();
		Nokia5110_SetCursor(0,5);
		Nokia5110_OutString("Welcome :D!");	
		Delay100ms(20);
		for(w=0 ; w<7 ; w++)
		{
			Nokia5110_SetCursor(0,w);
			Nokia5110_OutString("           ");
			Delay100ms(1);
		}
	}

	
	
  while(1){
		Nokia5110_ClearBuffer();
		SW1 = GPIO_PORTF_DATA_R&0x10;
		SW2 = GPIO_PORTF_DATA_R&0x01;
		
		if(isMenuMode){
			theMenu();
		}//menu code end
		
		
		else { //start second main if

			currPlayer = turn%2;
			opponentPlayerNum = willWePlayFirst;
		
			if(turn > lastTurn){
				playerPos = 0;
				lastTurn = turn;
			}
			
				winner = isThereAwinner();
			update();
			
			Nokia5110_SetCursor(1, 0);
			if(winner){
				//Nokia5110_Clear();
				if(winner == 1){
					
					Nokia5110_OutString("P1 wins");
				}
				else{
					Nokia5110_OutString("P2 wins");
				}
				break;
			}

			
				
				//when playing with switches
				if(shouldPlayWithSw())
				{
					//wait for an input
					while( SW1 && SW2){
						SW1 = GPIO_PORTF_DATA_R&0x10;
						SW2 = GPIO_PORTF_DATA_R&0x01;
					};
				
				
				
					//SW1 on release, move to the next position
					if(!SW1){
						
						//wait untill SW1 is released
						while(!SW1){SW1 = GPIO_PORTF_DATA_R&0x10;}
						playerPos = (playerPos + 1)%numOfCol;
						
						//turn = 0; turn%2 = 0; first player >> turn/2 = 0; first coin
						//turn = 1; turn%2 = 1; second player >> turn/2 = 0; first coin
						//turn = 2; turn%2 = 0; first player >> turn/2 = 1; second coin
						//turn = 3; turn%2 = 1; second player >> turn/2 = 1; second coin
						playersCoins[currPlayer][turn/2].x = colCenter[playerPos];
					}
					//SW2 on release, place the coin in the column if possible
					if(!SW2){
						//wait untill SW2 is released
						while(!SW2){SW2 = GPIO_PORTF_DATA_R&0x01;}
						
					
						
						
						playInAcol();
					}
					
				}else 
				{//for the ai
					if(
						((gameMode == 2 && opponentPlayerNum == currPlayer) || //p1 vs ai
							(gameMode == 3)) // ai vs ai
						){
						if(!codingMode)
							Delay100ms(1);
						
						playerPos = getAiNextPos();
						playersCoins[currPlayer][turn/2].x = colCenter[playerPos];
						update();
						
						playInAcol();
						if(!codingMode)
							Delay100ms(1);
					}
				}
				
			
			}//end second main if
		
			
		
  }
}


void Delay100ms(unsigned long count){unsigned long volatile time;
  while(count>0){
    time = 333333;  // 0.1sec at 80 MHz
    while(time){
	  	time--;
    }
    count--;
  }
}

void PortF_Init(void){ volatile unsigned long delay;
  SYSCTL_RCGC2_R |= 0x00000020;     // 1) F clock
  delay = SYSCTL_RCGC2_R;           // delay   
  GPIO_PORTF_LOCK_R = 0x4C4F434B;   // 2) unlock PortF PF0  
  GPIO_PORTF_CR_R = 0x1F;           // allow changes to PF4-0       
  GPIO_PORTF_AMSEL_R = 0x00;        // 3) disable analog function
  GPIO_PORTF_PCTL_R = 0x00000000;   // 4) GPIO clear bit PCTL  
  GPIO_PORTF_DIR_R = 0x0E;          // 5) PF4,PF0 input, PF3,PF2,PF1 output   
  GPIO_PORTF_AFSEL_R = 0x00;        // 6) no alternate function
  GPIO_PORTF_PUR_R = 0x11;          // enable pullup resistors on PF4,PF0       
  GPIO_PORTF_DEN_R = 0x1F;          // 7) enable digital pins PF4-PF0        
}