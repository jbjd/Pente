
// ChildView.cpp : implementation of the CChildView class
//

#include "pch.h"
#include "framework.h"
#include "Pente.h"
#include "ChildView.h"
#include "DoubleBufferDC.h"
#include <memory>
#include <string>

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// CChildView

CChildView::CChildView()
{
}

CChildView::~CChildView()
{
}


BEGIN_MESSAGE_MAP(CChildView, CWnd)
	ON_WM_PAINT()
	ON_WM_ERASEBKGND()
//	ON_WM_LBUTTONDBLCLK()
	ON_WM_LBUTTONDOWN()
	ON_WM_MOUSEMOVE()
END_MESSAGE_MAP()



// CChildView message handlers

BOOL CChildView::PreCreateWindow(CREATESTRUCT& cs) 
{
	if (!CWnd::PreCreateWindow(cs))
		return FALSE;

	cs.dwExStyle |= WS_EX_CLIENTEDGE;
	cs.style &= ~WS_BORDER;
	cs.lpszClass = AfxRegisterWndClass(CS_HREDRAW|CS_VREDRAW|CS_DBLCLKS, 
		::LoadCursor(nullptr, IDC_ARROW), reinterpret_cast<HBRUSH>(COLOR_WINDOW+1), nullptr);

	return TRUE;
}

void CChildView::OnPaint() 
{
	CPaintDC paintDC(this);     // device context for painting
	CDoubleBufferDC dc(&paintDC); // device context for painting
	Gdiplus::Graphics graphics(dc.m_hDC);

	CRect rect;
	GetClientRect(&rect);
	
	//Gdiplus::SolidBrush brush(Gdiplus::Color::White);
	//graphics.FillRectangle(&brush, 0, 0, rect.Width(), rect.Height());

	mGame.Draw(&graphics, rect.Width(), rect.Height());
}



BOOL CChildView::OnEraseBkgnd(CDC* pDC)
{
	// TODO: Add your message handler code here and/or call default

	return FALSE;
}


//void CChildView::OnLButtonDblClk(UINT nFlags, CPoint point)
//{
//	mGame.mouseInput();
//
//	CWnd::OnLButtonDblClk(nFlags, point);
//}


void CChildView::OnLButtonDown(UINT nFlags, CPoint point)
{
	mGame.mouseInput();
	Invalidate();
	CWnd::OnLButtonDown(nFlags, point);
}


void CChildView::OnMouseMove(UINT nFlags, CPoint point)
{
	mGame.mouseMove(point.x, point.y);
	Invalidate();
	CWnd::OnMouseMove(nFlags, point);
}
