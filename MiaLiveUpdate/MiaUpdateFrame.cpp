
#include "MiaUpdateFrame.h"
#include <windowsx.h>
#include <wingdi.h>
#include"resource.h"

#define PROCESS_TEST_TIMER 10000

CMiaUpdateFrame::CMiaUpdateFrame()
	: m_bMouseTracking(FALSE)
	, m_eBtnState(ENomal)
	, m_strTitle(L"米亚公开课 v1.1.1.1")
	, m_nPos(0)
{

}

CMiaUpdateFrame::~CMiaUpdateFrame()
{

}

LRESULT CMiaUpdateFrame::HandleMessage(UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	BOOL bHandled = FALSE;

	switch (uMsg)
	{
	case WM_CREATE:
		OnCreate(uMsg, wParam, lParam);
		break;
	case WM_LBUTTONDOWN:
		OnLButtonDown(uMsg, wParam, lParam);
		break;
	case WM_LBUTTONUP:
		OnLButtonUp(uMsg, wParam, lParam);
		break;
	case WM_MOUSEMOVE:
		OnMouseMove(uMsg, wParam, lParam);
		break;
	case WM_MOUSELEAVE:
		OnMouseLeave(uMsg, wParam, lParam);
		break;
	case WM_SYSCOMMAND:
		OnSysCommand(uMsg, wParam, lParam, bHandled);
		break;
	case WM_TIMER:
	{
		if (PROCESS_TEST_TIMER == wParam)
		{
			SetProcess(++m_nPos);
			if (100 == m_nPos)
			{
				KillTimer(m_hWnd, PROCESS_TEST_TIMER);
				//打开安装包
			}
		}
		
	}
	break;
	default:
		break;
	}
	if (!bHandled) return ::DefWindowProc(m_hWnd, uMsg, wParam, lParam);
	return 0;
}

LRESULT CMiaUpdateFrame::OnCreate(UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	::SetWindowText(m_hWnd, m_strTitle);

	LONG styleValue = ::GetWindowLong(m_hWnd, GWL_STYLE);
	styleValue &= ~WS_CAPTION;
	::SetWindowLong(m_hWnd, GWL_STYLE, styleValue | WS_CLIPSIBLINGS | WS_CLIPCHILDREN);

	HINSTANCE hInst = ::GetModuleHandle(NULL);
	m_ImgBK.LoadFromResource(hInst, IDB_MIA_BK_PNG, _T("PNG"));
	m_ImgProgressBar.LoadFromResource(hInst, IDB_PNG_PROGRESS, _T("PNG"));
	m_ImgCloseNormal.LoadFromResource(hInst, IDB_PNG_CLOSE_NORMAL, _T("PNG"));
	m_ImgCloseHot.LoadFromResource(hInst, IDB_PNG_CLOSE_HOT, _T("PNG"));

	HFONT m_hTitileFont = ::CreateFont(12, 0, 0, 0, FW_NORMAL, FALSE, FALSE, 0, GB2312_CHARSET, OUT_DEFAULT_PRECIS, CLIP_DEFAULT_PRECIS, DEFAULT_QUALITY, DEFAULT_PITCH | FF_SWISS, _T("微软雅黑"));
	
	::MoveWindow(m_hWnd, 200,200, 598, 448, TRUE);
	DrawFrame();

	::SetTimer(m_hWnd, PROCESS_TEST_TIMER, 50, NULL);
	return 0;
}

LRESULT CMiaUpdateFrame::OnLButtonDown(UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	POINT point = { GET_X_LPARAM(lParam), GET_Y_LPARAM(lParam) };

	BTN_STATE eState = AdustButtonState(point);
	if (eState != ENomal) return 0;

	RECT rcCaption = { 0, 0, 598, 38 };
	if (::PtInRect(&rcCaption, point))
	{
		::PostMessage(m_hWnd, WM_NCLBUTTONDOWN, HTCAPTION, MAKELPARAM(point.x, point.y));
		return 0;
	}
	return 0;
}

LRESULT CMiaUpdateFrame::OnLButtonUp(UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	POINT point = { GET_X_LPARAM(lParam), GET_Y_LPARAM(lParam) };
	BTN_STATE eState = AdustButtonState(point);

	if (eState == ECloseHot)
	{
		OnCancel();
		return 0;
	}

	return 0;
}

LRESULT CMiaUpdateFrame::OnMouseMove(UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	UINT nFlags = (UINT)wParam;
	POINT point = { GET_X_LPARAM(lParam), GET_Y_LPARAM(lParam) };

	//鼠标离开和进入消息注册
	if (!m_bMouseTracking){
		TRACKMOUSEEVENT tme = { 0 };
		tme.cbSize = sizeof(TRACKMOUSEEVENT);
		tme.dwFlags = TME_HOVER | TME_LEAVE;
		tme.hwndTrack = m_hWnd;
		tme.dwHoverTime = 400L;
		TrackMouseEvent(&tme);
		m_bMouseTracking = TRUE;
	}

	//调整按钮状态
	m_eBtnState = AdustButtonState(point);

	//重绘界面
	DrawFrame();
	return 0;
}

LRESULT CMiaUpdateFrame::OnMouseLeave(UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	UINT nFlags = (UINT)wParam;
	POINT point = { GET_X_LPARAM(lParam), GET_Y_LPARAM(lParam) };

	m_bMouseTracking = FALSE;
	//重绘界面
	DrawFrame();
	return 0;
}

LRESULT CMiaUpdateFrame::OnSysCommand(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL &bHandled)
{
	if (wParam == SC_CLOSE)
	{
		OnCancel();
		bHandled = TRUE;
		return 0;
	}
	return 0;
}

void CMiaUpdateFrame::OnFinalMessage(HWND hWnd)
{
	__super::OnFinalMessage(hWnd);
	delete this;
}

//////////////////////////////////////////////////////////////////////////
void CMiaUpdateFrame::SetProcess(INT32 i32Process)
{
	if (i32Process >= 0 && i32Process <= 100)
	{
		m_nPos = i32Process;
	
		DrawFrame();
		
	}
}


void CMiaUpdateFrame::CenterWindow()
{
	//ASSERT(::IsWindow(m_hWnd));
	//ASSERT((GetWindowStyle(m_hWnd)&WS_CHILD) == 0);
	RECT rcDlg = { 0 };
	::GetWindowRect(m_hWnd, &rcDlg);
	RECT rcArea = { 0 };
	RECT rcCenter = { 0 };
	HWND hWnd = m_hWnd;
	HWND hWndParent = ::GetParent(m_hWnd);
	HWND hWndCenter = ::GetWindowOwner(m_hWnd);
	if (hWndCenter != NULL)
		hWnd = hWndCenter;

	// 处理多显示器模式下屏幕居中
	MONITORINFO oMonitor = {};
	oMonitor.cbSize = sizeof(oMonitor);
	::GetMonitorInfo(::MonitorFromWindow(hWnd, MONITOR_DEFAULTTONEAREST), &oMonitor);
	rcArea = oMonitor.rcWork;

	if (hWndCenter == NULL)
		rcCenter = rcArea;
	else
		::GetWindowRect(hWndCenter, &rcCenter);

	int DlgWidth = rcDlg.right - rcDlg.left;
	int DlgHeight = rcDlg.bottom - rcDlg.top;

	// Find dialog's upper left based on rcCenter
	int xLeft = (rcCenter.left + rcCenter.right) / 2 - DlgWidth / 2;
	int yTop = (rcCenter.top + rcCenter.bottom) / 2 - DlgHeight / 2;

	// The dialog is outside the screen, move it inside
	if (xLeft < rcArea.left) xLeft = rcArea.left;
	else if (xLeft + DlgWidth > rcArea.right) xLeft = rcArea.right - DlgWidth;
	if (yTop < rcArea.top) yTop = rcArea.top;
	else if (yTop + DlgHeight > rcArea.bottom) yTop = rcArea.bottom - DlgHeight;
	::SetWindowPos(m_hWnd, NULL, xLeft, yTop, -1, -1, SWP_NOSIZE | SWP_NOZORDER | SWP_NOACTIVATE);
}
//////////////////////////////////////////////////////////////////////////
//绘制框架
void CMiaUpdateFrame::DrawFrame()
{
	RECT rcClient = { 0 };
	HDC hDC = NULL, hMemDC = NULL;
	HBITMAP hBmp = NULL, hOldBmp = NULL;
	int nWidth = 0, nHeight = 0;

	::GetClientRect(m_hWnd, &rcClient);
	nWidth = rcClient.right - rcClient.left;
	nHeight = rcClient.bottom - rcClient.top;

	//nHeight = 448;
	//nWidth = 598;
	hDC = ::GetDC(m_hWnd);
	hMemDC = ::CreateCompatibleDC(hDC);
	hBmp = ::CreateCompatibleBitmap(hDC, nWidth, nHeight);
	hOldBmp = (HBITMAP)::SelectObject(hMemDC, hBmp);

	PaintBkimage(hMemDC);
	PaintCloseButton(hMemDC);
	PaintTitle(hMemDC);
	PaintProgress(hMemDC);

	//把dc中的数据画到窗口上
	PaintLayeredWindow(hDC, hMemDC, nWidth, nHeight);

	//是否dc
	::SelectObject(hMemDC, hOldBmp);
	::DeleteObject(hBmp);
	::ReleaseDC(m_hWnd, hDC);
	::DeleteDC(hMemDC);
}

//绘制透明图片
void CMiaUpdateFrame::PaintLayeredWindow(HDC hDC, HDC hMemDC, const int nWidth, const int nHeight)
{
	typedef BOOL(WINAPI * PUNDATELAYERWINDOW)(HWND hWnd, HDC hdcDst, POINT *pptDst, SIZE *psize, HDC hdcSrc, POINT *pptSrc, COLORREF crKey, BLENDFUNCTION *pblend, DWORD dwFlags);
	HMODULE hUser32 = ::GetModuleHandle(_T("User32.dll"));
	if (hUser32 == NULL) return;
	PUNDATELAYERWINDOW pUpdateLayeredWindow = (PUNDATELAYERWINDOW)GetProcAddress(hUser32, "UpdateLayeredWindow");
	if (pUpdateLayeredWindow == NULL) return;

	//绘制
	SIZE szDst = { nWidth, nHeight };
	BLENDFUNCTION bl;
	POINT ptDst = { 0, 0 }, ptSrc = { 0, 0 };
	bl.BlendOp = AC_SRC_OVER;
	bl.BlendFlags = 0;
	bl.AlphaFormat = AC_SRC_ALPHA;
	bl.SourceConstantAlpha = 255;
	pUpdateLayeredWindow(m_hWnd, hDC, NULL, &szDst, hMemDC, &ptSrc, 0, &bl, ULW_ALPHA);
}

//绘制背景
void CMiaUpdateFrame::PaintBkimage(HDC hDC)
{
	RECT rcClient;
	::GetClientRect(m_hWnd, &rcClient);
	RECT rcSrc = { 0, 0, m_ImgBK.GetWidth(), m_ImgBK.GetHeight() };
	m_ImgBK.Draw(hDC, rcClient, rcSrc);
}

//绘制关闭按钮
void CMiaUpdateFrame::PaintCloseButton(HDC hDC)
{
	RECT rcDest = GetCloseRect();

	if (m_eBtnState == ECloseHot)
	{
		RECT rcSrc = { 0, 0, m_ImgCloseHot.GetWidth(), m_ImgCloseHot.GetHeight() };
		m_ImgCloseHot.Draw(hDC, rcDest, rcSrc);
	}
	else
	{
		RECT rcSrc = { 0, 0, m_ImgCloseNormal.GetWidth(), m_ImgCloseNormal.GetHeight() };
		m_ImgCloseNormal.Draw(hDC, rcDest, rcSrc);
	}
}

//绘制标题
void CMiaUpdateFrame::PaintTitle(HDC hDC)
{
	RECT rc = { 15, 14, 400, 39 };
	PaintText(hDC, 16, DT_LEFT | DT_SINGLELINE | DT_TOP, RGB(76, 76, 76), rc, m_strTitle);
}



//绘制文件进度
void CMiaUpdateFrame::PaintProgress(HDC hDC)
{
	RECT rcDest = { 25, 350, 573, 355 };
	RECT rcSrc = { 0, 0, m_ImgProgressBar.GetWidth(), (int)(m_ImgProgressBar.GetHeight() * 0.5f) };
	m_ImgProgressBar.Draw(hDC, rcDest, rcSrc);

	//进度图片
	RECT rcDest2 = rcDest;
	RECT rcSrc2 = rcSrc;
	rcDest2.right = rcDest2.left + (m_nPos * (rcDest2.right - rcDest2.left)) / 100;
	rcSrc2.top = rcSrc2.bottom;
	rcSrc2.bottom = rcSrc2.top * 2;
	if (rcDest2.right - rcDest2.left > 0)
		m_ImgProgressBar.Draw(hDC, rcDest2, rcSrc2);

	//绘制进度文字
	CString strValue;
	strValue.Format(L"%d%%", m_nPos);
	RECT rcText = rcDest;
	rcText.bottom = rcText.top - 2;
	rcText.top = rcText.bottom - 22;
	rcText.left = rcText.right - 50;
	PaintText(hDC, 14, DT_RIGHT | DT_TOP | DT_SINGLELINE, RGB(0, 0, 0), rcText, strValue);

	CString  strText;
	strText.Format(_T("升级中,请稍后..."));
	RECT rcText2 = rcDest;
	rcText2.bottom = rcText2.top - 2;
	rcText2.top = rcText2.bottom - 22;
	rcText2.right = rcText2.left + 150;
	PaintText(hDC, 14, DT_LEFT | DT_SINGLELINE | DT_END_ELLIPSIS, RGB(0, 0, 0), rcText2, strText);
}


//绘制文字
void CMiaUpdateFrame::PaintText(HDC hDC, int nFontSize, UINT uStyle, DWORD dwColor, RECT rc, LPCWSTR szText)
{
	Gdiplus::FontFamily fontfamily(L"微软雅黑");
	Gdiplus::Font font(&fontfamily, (Gdiplus::REAL)nFontSize, Gdiplus::FontStyleRegular, Gdiplus::UnitPixel);

	Gdiplus::Graphics graphics(hDC);
	Gdiplus::RectF rectF((Gdiplus::REAL)rc.left, (Gdiplus::REAL)rc.top, (Gdiplus::REAL)(rc.right - rc.left), (Gdiplus::REAL)(rc.bottom - rc.top));
	Gdiplus::SolidBrush brush(Gdiplus::Color(254, GetBValue(dwColor), GetGValue(dwColor), GetRValue(dwColor)));

	Gdiplus::StringFormat stringFormat = Gdiplus::StringFormat::GenericTypographic();

	if ((uStyle & DT_END_ELLIPSIS) != 0) {
		stringFormat.SetTrimming(Gdiplus::StringTrimmingEllipsisCharacter);
	}

	int formatFlags = 0;
	if ((uStyle & DT_NOCLIP) != 0) {
		formatFlags |= Gdiplus::StringFormatFlagsNoClip;
	}
	if ((uStyle & DT_SINGLELINE) != 0) {
		formatFlags |= Gdiplus::StringFormatFlagsNoWrap;
	}

	stringFormat.SetFormatFlags(formatFlags);

	if ((uStyle & DT_LEFT) != 0) {
		stringFormat.SetAlignment(Gdiplus::StringAlignmentNear);
	}
	else if ((uStyle & DT_CENTER) != 0) {
		stringFormat.SetAlignment(Gdiplus::StringAlignmentCenter);
	}
	else if ((uStyle & DT_RIGHT) != 0) {
		stringFormat.SetAlignment(Gdiplus::StringAlignmentFar);
	}
	else {
		stringFormat.SetAlignment(Gdiplus::StringAlignmentNear);
	}
	stringFormat.GenericTypographic();
	if ((uStyle & DT_TOP) != 0) {
		stringFormat.SetLineAlignment(Gdiplus::StringAlignmentNear);
	}
	else if ((uStyle & DT_VCENTER) != 0) {
		stringFormat.SetLineAlignment(Gdiplus::StringAlignmentCenter);
	}
	else if ((uStyle & DT_BOTTOM) != 0) {
		stringFormat.SetLineAlignment(Gdiplus::StringAlignmentFar);
	}
	else {
		stringFormat.SetLineAlignment(Gdiplus::StringAlignmentNear);
	}

	graphics.DrawString(szText, -1, &font, rectF, &stringFormat, &brush);
}

//绘制按钮和文字
void CMiaUpdateFrame::PaintButtonText(HDC hDC, LPCTSTR szText, int nFontSize, DWORD dwColor, CImageEx & img, RECT rcDest, RECT rcSrc)
{
	img.Draw(hDC, rcDest, rcSrc);
	if (szText)
	{
		rcDest.bottom -= 2;
		PaintText(hDC, nFontSize, DT_SINGLELINE | DT_VCENTER | DT_CENTER, dwColor, rcDest, szText);
	}
}

//根据鼠标位置调整按钮状态
BTN_STATE CMiaUpdateFrame::AdustButtonState(POINT pt)
{
	RECT rcClose = { 0 };
	rcClose = GetCloseRect();

	if (::PtInRect(&rcClose, pt))
		return ECloseHot;
	return ENomal;
}

//取消下载
void CMiaUpdateFrame::OnCancel()
{
	::DestroyWindow(m_hWnd);
}

/////////////////////////////////////////////////////////////////////////
//获取关闭按钮位置
RECT CMiaUpdateFrame::GetCloseRect()
{
	RECT rcClient;
	::GetClientRect(m_hWnd, &rcClient);
	rcClient.right -= 9;
	rcClient.left = rcClient.right - 38;
	rcClient.top += 5;
	rcClient.bottom = rcClient.top + 38;
	return rcClient;
}
