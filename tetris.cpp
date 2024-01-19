#include <iostream>
#include <vector>
using namespace std;

#include<windows.h> // Holds the Declaration of all the Functions in the Windows API 

//wstring is just only the string class for wide characters
wstring tetromino[7];
//Field measures 
int nFieldWidth=12;
int nFieldHeight=18;
unsigned char *pField=nullptr;  //Allocated Dynamically and also diffrent information is stored in one array

int nScreenWidth=80;    //Console Screen Size x (Columns)
int nScreenHeight=30;   //Consoole Screen Size y (Rows)

//Rotating logic of an 2D matrix into an 1D matrix 
int Rotate(int px,int py, int r){
    switch(r%4)
    {
        case 0:return py * 4 + px;              //0 degress
        case 1:return 12 + py - (px*4);         //90 degress
        case 2:return 15 - (py * 4 ) - px;      //180 degreees
        case 3:return 3 - py + (px*4);          //270 degreess
    }
}

bool DoesPieceFit(int nTetomino , int nRotation , int nPosX , int nPosY){
    for (int px=0 ; px < 4 ; px++)
        for(int py = 0 ; py < 4 ; py++){
            //Get index into Piece
            int pi = Rotate(px ,py , nRotation);

            //Get index into field 
            int fi= (nPosY + py) * nFieldWidth + (nPosX + px);

            if(nPosX + px >= 0 && nPosX + px <nFieldWidth){
                if(nPosY + py >= 0 && nPosY + py <nFieldHeight){
                    if(tetromino[nTetomino][pi] != L'.' && pField[fi] != 0)
                    return false ;     //Fail on Firs Hit 
                }
            }
        }
    return true;
    }


int main(){

    wchar_t *screen = new wchar_t[nScreenWidth*nScreenHeight];
    // CHAR_INFO *screen = new CHAR_INFO[nScreenWidth*nScreenHeight];
    // wchar_t screen[nScreenWidth * nScreenHeight];
	for (int i = 0; i < nScreenWidth*nScreenHeight; i++) screen[i] = L' ';
	HANDLE hConsole = CreateConsoleScreenBuffer(GENERIC_READ | GENERIC_WRITE, 0, NULL, CONSOLE_TEXTMODE_BUFFER, NULL);
	SetConsoleActiveScreenBuffer(hConsole);
	DWORD dwBytesWritten = 0;

    //creating assets
	tetromino[0].append(L"..X...X...X...X."); // Tetronimos 4x4
	tetromino[1].append(L"..X..XX...X.....");
	tetromino[2].append(L".....XX..XX.....");
	tetromino[3].append(L"..X..XX..X......");
	tetromino[4].append(L".X...XX...X.....");
	tetromino[5].append(L".X...X...XX.....");
	tetromino[6].append(L"..X...X..XX.....");

    pField= new unsigned char[nFieldWidth*nFieldHeight]; //Creating the main playground
    for(int x=0 ; x < nFieldWidth ; x++){               //Board Boundry
        for (int y=0 ; y < nFieldHeight ; y++)
            pField[y * nFieldWidth + x] = (x == 0 || x == nFieldWidth -1 || y == nFieldHeight - 1) ? 9 : 0 ;
    }



    //Game Logic
    bool bKey[4];
	int nCurrentPiece = 0;
	int nCurrentRotation = 0;
	int nCurrentX = nFieldWidth / 2;
	int nCurrentY = 0;
	int nSpeed = 20;  // Difficulty of the game 
	int nSpeedCount = 0;
	bool bForceDown = false;
	bool bRotateHold = true;
	int nPieceCount = 0;
	int nScore = 0;
	vector<int> vLines; // for disparing the last line of == 
    bool bGameover = false;


    while(!bGameover)
    {
        //Gamer timing ==============================
        Sleep(50);
        nSpeedCount++;
        bForceDown = (nSpeedCount == nSpeed);

        //Input ==================================
        for (int k = 0; k < 4; k++)								// R   L   D Z
			bKey[k] = (0x8000 & GetAsyncKeyState((unsigned char)("\x27\x25\x28Z"[k]))) != 0;
            // x27 is for right key , x25 is for left key and hence the notations are given there

        //Game Logicc =================================
        if(bKey[0]){
            if(DoesPieceFit(nCurrentPiece , nCurrentRotation , nCurrentX + 1 , nCurrentY)){
                nCurrentX = nCurrentX + 1;
            }
        }
        if(bKey[1]){
            if(DoesPieceFit(nCurrentPiece , nCurrentRotation , nCurrentX - 1 , nCurrentY)){
                nCurrentX = nCurrentX - 1;
            }
        }
        if(bKey[2]){
            if(DoesPieceFit(nCurrentPiece , nCurrentRotation , nCurrentX  , nCurrentY + 1)){
                nCurrentY = nCurrentY + 1;
            }
        }
        if(bKey[3]){
            nCurrentRotation += (bRotateHold && DoesPieceFit(nCurrentPiece, nCurrentRotation + 1, nCurrentX, nCurrentY)) ? 1 : 0;
			bRotateHold = false;
        }    
        else{
            bRotateHold = true;
        }

        if (bForceDown)
		{
			// Update difficulty every 10 pieces
			nSpeedCount = 0;
			nPieceCount++;
			if (nPieceCount % 10 == 0)
				if (nSpeed >= 10) nSpeed--;
			
			// Test if piece can be moved down
			if (DoesPieceFit(nCurrentPiece, nCurrentRotation, nCurrentX, nCurrentY + 1))
				nCurrentY++; // It can, so do it!
			else{
				// It can't! Lock the piece in place
				for (int px = 0; px < 4; px++)
					for (int py = 0; py < 4; py++)
						if (tetromino[nCurrentPiece][Rotate(px, py, nCurrentRotation)] != L'.')
							pField[(nCurrentY + py) * nFieldWidth + (nCurrentX + px)] = nCurrentPiece + 1;

				// Check for lines
				for (int py = 0; py < 4; py++)
					if(nCurrentY + py < nFieldHeight - 1)
					{
						bool bLine = true;
						for (int px = 1; px < nFieldWidth - 1; px++)
							bLine &= (pField[(nCurrentY + py) * nFieldWidth + px]) != 0;

						if (bLine)
						{
							// Remove Line, set to =
							for (int px = 1; px < nFieldWidth - 1; px++)
								pField[(nCurrentY + py) * nFieldWidth + px] = 8;
							vLines.push_back(nCurrentY + py);
						}						
					}

				nScore += 25;
				if(!vLines.empty())	nScore += (1 << vLines.size()) * 100; //Exponential increasing of score 
                //rewarding more let them to take bigger risk

				// Pick New Piece
				nCurrentX = nFieldWidth / 2;
				nCurrentY = 0;
				nCurrentRotation = 0;
				nCurrentPiece = rand() % 7;

				// If piece does not fit straight away, game over!
				if(DoesPieceFit(nCurrentPiece, nCurrentRotation, nCurrentX, nCurrentY) == false){
                    bGameover = true;
                }
			}
		}

        //Render Output ==================================


        // Draw a Field 
        for (int x = 0 ; x <nFieldWidth ; x++){
        for (int y=0 ; y < nFieldHeight ; y++){
            screen[(y+2)*nScreenWidth + (x+2)] = L" ABCDEFG=#"[pField[y*nFieldWidth + x]];
        }
        }

        //Draw Current Piece 
        for(int px= 0 ; px < 4 ; px++)
        {
            for(int py=0 ; py< 4 ; py ++)
            {
                if(tetromino[nCurrentPiece][Rotate(px , py , nCurrentRotation)] == L'X')
            screen[(nCurrentY + py + 2)* nScreenWidth + (nCurrentX + px + 2)] = nCurrentPiece + 65; // Taking tetro pieces and make them in
            // making them ins Ascii and changing them into A, B , C .... so on . 
            }
        }

        //Display Frame
        WriteConsoleOutputCharacterW(hConsole , screen , nScreenWidth * nScreenHeight , {0,0} , &dwBytesWritten);
    }
    //Game over message and its content
    CloseHandle(hConsole);
	cout<< "Game Over!! Score:" << nScore << endl;
	system("pause");
	return 0;
}
