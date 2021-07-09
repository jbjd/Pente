#include "pch.h"
#include "Game.h"
#include <string>
#include <algorithm>

/// Game area in virtual pixels
const static int Width = 1664;

/// Game area height in virtual pixels
const static int Height = 2048;

const static int xStart = 64;
const static int yStart = 164;
const static int xEnd = 1600;
const static int yEnd = 1700;
const static int size = 128;

const static int greenMark = 1;
const static int blueMark = 2;

CGame::CGame() {
	mRectangle = new Gdiplus::RectF(836, 60, 0, 0);
	mFormat.SetAlignment(Gdiplus::StringAlignmentCenter);
	mBlueBrush = new Gdiplus::SolidBrush(Gdiplus::Color(37, 52, 228));
	mGreenBrush = new Gdiplus::SolidBrush(Gdiplus::Color(37, 228, 52));
	mBlueLine = new Gdiplus::Pen(Gdiplus::Color(37, 52, 228));
	mGreenLine = new Gdiplus::Pen(Gdiplus::Color(37, 228, 52));
	mLinePen = new Gdiplus::Pen(Gdiplus::Color(0, 0, 0), 4);
	for (int i = 0; i < 13; i++) {
		mBoard.push_back(std::vector<int>(13, 0));
	}
	curBrush = mBlueBrush;
	curPen = mBlueLine;
}

CGame::~CGame() {
	delete mRectangle;
	delete mBlueBrush;
	delete mGreenBrush;
	delete mLinePen;
	delete mBlueLine;
	delete mGreenLine;
}

void CGame::Draw(Gdiplus::Graphics* graphics, int width, int height) {
	float scaleX = float(width) / float(Width);
	float scaleY = float(height) / float(Height);
	mScale = min(scaleX, scaleY);
	int helper = (int)(64 * mScale);
	double newScl = float(helper) / 64.0;
	mScale = (float)newScl;
	// Ensure it is centered horizontally
	mXOffset = (float)(int)((width - Width * mScale) / 2);

	// Ensure it is centered vertically
	mYOffset = (float)(int)((height - Height * mScale) / 2);

	graphics->TranslateTransform(mXOffset, mYOffset);
	graphics->ScaleTransform(mScale, mScale);

	if (mStartScreen) {
		Gdiplus::SolidBrush brush(Gdiplus::Color(0, 0, 0));
		Gdiplus::FontFamily fontFamily(L"Arial");
		Gdiplus::Font font(&fontFamily, 45);


		graphics->DrawString(L"Please Select Who Will Go First:", -1, &font, *mRectangle, &mFormat, &brush);
		graphics->FillEllipse(mBlueBrush, 0, 500, 512, 512);
		graphics->FillEllipse(mGreenBrush, 1152, 500, 512, 512);
	}
	else {
		for (int j = 0; j < 13; j++) {
			int yLoc = yStart + (j * 128);
			int xLoc = xStart + (j * 128);
			graphics->DrawLine(mLinePen, xStart, yLoc, xEnd, yLoc);
			graphics->DrawLine(mLinePen, xLoc, yStart, xLoc, yEnd);
		}
		//for (int j = 100; j < 1700; j += 128) { //for loop for positioning board lines (testing only)
		//	graphics->FillEllipse(mBluePen, j-100, j, 128, 128);
		//}
		for (int i = 0; i < 13; i++) {
			std::vector<int> curRow = mBoard[i];
			for (int j = 0; j < 13; j++) {
				int val = curRow[j];
				int xLoc = i * size;
				int yLoc = (j * size) + 100;
				if (val) {
					if (val == greenMark) {
						curBrush = mGreenBrush;
					}
					else {
						curBrush = mBlueBrush;
					}
					graphics->FillEllipse(curBrush, xLoc, yLoc, size, size);
				}
				else if (mX > xLoc && mX < (xLoc + size) && mY > yLoc && mY < (yLoc+size)) { //this else will see if user is hovering spot with no piece
					if (mGreenTurn) {
						curPen = mGreenLine;
					}else{
						curPen = mBlueLine;
					}
					graphics->DrawEllipse(curPen, xLoc, (j * size) + 100, size, size);
				}
			}
		}
	}

}

void CGame::mouseInput() {
	if (mStartScreen) {
		if(mX <= 512 && mX >= 0) {
			mGreenTurn = true;
			mBoard[6][6] = 2;
			mStartScreen = false;
		}
		else if (mX <= 1664 && mX >= 1152) {
			mGreenTurn = false;
			mBoard[6][6] = 1;
			mStartScreen = false;
		}
	}
	else {
		int xLoc = (int)(mX / 128);
		int yLoc = (int)((mY-100) / 128);
		if (xLoc >=0 && yLoc >= 0 && xLoc < 13 && yLoc < 13 && !mBoard[xLoc][yLoc] && !mWin) {
			if (mGreenTurn) {
				mBoard[xLoc][yLoc] = 1;
			}
			else {
				mBoard[xLoc][yLoc] = 2;
			}
			this->checkCapture(xLoc,yLoc);
			if (mTurn > 7) {
				mWin = this->checkWin(mGreenTurn);
			}
			mGreenTurn = !mGreenTurn;
			mTurn += 1;
		}
	}
}

void CGame::mouseMove(int x, int y) {
	mX = (int)((x - mXOffset)/mScale);
	mY = (int)((y - mYOffset)/mScale);
	mBlueBrush->SetColor(Gdiplus::Color(37, 52, 228));
	mGreenBrush->SetColor(Gdiplus::Color(37, 228, 52));
	if (mStartScreen) {
		if (mX <= 512 && mX >= 0) {
			mBlueBrush->SetColor(Gdiplus::Color(37, 72, 248));
		}
		else if (mX <= Width && mX >= 1152) {
			mGreenBrush->SetColor(Gdiplus::Color(37, 248, 72));
		}
	}

}

bool CGame::checkWin(bool greenTurn) {
	int checkVal = 1;
	if (!greenTurn) {
		checkVal += 1;
	}
	//three checks: vertical, horizantal, diagonal
	//check one: vertical
	for (int i = 0; i < 13; i++) {
		std::vector<int> curCol = mBoard[i];
		int count = std::count(curCol.begin(), curCol.end(), checkVal);
		if (count >= 5) {
			int consec = 0;
			for (int j = 0; j < 13; j++) {
				if (curCol[j] == checkVal) {
					consec += 1;
					if (consec > 4) {
						return true;
					}
				}
				else {
					consec = 0;
				}
			}
		}
	}
	//check two: horizantal
	for (int i = 0; i < 13; i++) {
		int consec = 0;
		for (int j = 0; j < 13; j++) {
			if (mBoard[j][i] == checkVal) {
				consec += 1;
				if (consec > 4) {
					return true;
				}
			}
			else {
				consec = 0;
			}
		}
	}
	//check three: diagonal topleft to bottomright
	for (int j = 8; j >= -8; j--) {
		int consec = 0;
		for (int i = 12; (i-j >= 0); i--) {
			if ( ((i - j) < 13) && (i>=0) ) {
				if (mBoard[i][i - j] == checkVal) {
					consec += 1;
					if (consec > 4) {
						return true;
					}
				}
				else {
					consec = 0;
				}
			}
		}
	}
	//check three part 2: diagonal bottomleft to topright
	for (int j = 4; j  < 21; j++) {
		int consec = 0;
		for (int i = 0; j-i >= 0; i++) {
			if ( ((j - i) < 13)&&(i<13) ) {
				if (mBoard[i][j-i] == checkVal) {
					consec += 1;
					if (consec > 4) {
						return true;
					}
				}
				else {
					consec = 0;
				}
			}
		}
	}
	return false;
}

void CGame::checkCapture(int x, int y) {
	int placedColor = mBoard[x][y];
	int enemy;
	if (placedColor == 1) {
		enemy = 2;
	}
	else {
		enemy = 1;
	}
	//vertical check
	if (y >= 3 && mBoard[x][y - 1] == mBoard[x][y - 2] && mBoard[x][y - 1] == enemy && mBoard[x][y - 3] == placedColor) {
		mBoard[x][y - 1] = 0;
		mBoard[x][y - 2] = 0;
	}
	if (y < 10 && mBoard[x][y + 1] == mBoard[x][y + 2] && mBoard[x][y + 1] == enemy && mBoard[x][y + 3] == placedColor) {
		mBoard[x][y + 1] = 0;
		mBoard[x][y + 2] = 0;
	}
	//horizantal check
	if (x >= 3 && mBoard[x - 1][y] == mBoard[x - 2][y] && mBoard[x - 1][y] == enemy && mBoard[x-3][y] == placedColor) {
		mBoard[x-1][y] = 0;
		mBoard[x-2][y] = 0;
	}
	if (x < 10 && mBoard[x + 1][y] == mBoard[x + 2][y] && mBoard[x + 1][y] == enemy && mBoard[x + 3][y] == placedColor) {
		mBoard[x + 1][y] = 0;
		mBoard[x+2][y] = 0;
	}
	//bottomright to topleft check
	if (x >= 3 && y >= 3 && mBoard[x - 1][y-1] == mBoard[x - 2][y-2] && mBoard[x - 1][y-1] == enemy && mBoard[x - 3][y-3] == placedColor) {
		mBoard[x - 1][y-1] = 0;
		mBoard[x - 2][y-2] = 0;
	}
	//topleft to bottomright check
	if (x < 10 && y <10 && mBoard[x + 1][y + 1] == mBoard[x + 2][y + 2] && mBoard[x + 1][y + 1] == enemy && mBoard[x + 3][y + 3] == placedColor) {
		mBoard[x + 1][y + 1] = 0;
		mBoard[x + 2][y + 2] = 0;
	}
	//bottomleft to topright check
	if (x < 10 && y >= 3 && mBoard[x + 1][y - 1] == mBoard[x + 2][y - 2] && mBoard[x + 1][y - 1] == enemy && mBoard[x + 3][y - 3] == placedColor) {
		mBoard[x + 1][y - 1] = 0;
		mBoard[x + 2][y - 2] = 0;
	}
	//topright to bottomleft check
	if (x >= 3 && y < 10 && mBoard[x - 1][y + 1] == mBoard[x - 2][y + 2] && mBoard[x - 1][y + 1] == enemy && mBoard[x - 3][y + 3] == placedColor) {
		mBoard[x - 1][y + 1] = 0;
		mBoard[x - 2][y + 2] = 0;
	}
}