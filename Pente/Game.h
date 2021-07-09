#pragma once

#include <vector>
//#include <memory>

class CGame
{
public:
	CGame();
	CGame(const CGame&) = delete;
	void operator=(const CGame&) = delete;
	~CGame();

	void Draw(Gdiplus::Graphics* graphics, int width, int height);
	void mouseInput();
	void mouseMove(int x, int y);
	bool checkWin(bool greenTurn);//bool if checking for green win
	void checkCapture(int x, int y);//bool if green capture
private:
	bool mStartScreen = true;
	Gdiplus::RectF* mRectangle; ///< Rectangle to format centered text in
	Gdiplus::StringFormat mFormat; ///< formatting for text centering
	float mScale=0; ///< scale of resized game menu
	float mYOffset=0; ///< y offset of screen
	float mXOffset=0; ///< x offset of screen
	Gdiplus::SolidBrush* mBlueBrush;
	Gdiplus::SolidBrush* mGreenBrush;
	Gdiplus::Pen* mBlueLine;
	Gdiplus::Pen* mGreenLine;
	Gdiplus::Pen* mLinePen;
	int mX = 0;
	int mY = 0;
	bool mGreenTurn = true;
	std::vector<std::vector<int>> mBoard; //vector of game board  0= no piece    1 = green    2 = blue
	Gdiplus::SolidBrush* curBrush; //points to which brush to use for minor efficiency, don't delete
	Gdiplus::Pen* curPen;
	int mTurn = 1;
	bool mWin = false;
};

