#pragma once

#include "BaseWnd.h"
#include <atlstr.h>
#include "ImageEx.h"

// 按钮状态
typedef enum BTN_STATE
{
	ENomal = 0,			//没有焦点
	ECloseHot,			//关闭按钮鼠标在上面状态
}BTN_STATE;

class CMiaUpdateFrame : public CBaseWnd
{
public:
	CMiaUpdateFrame();
	~CMiaUpdateFrame();
public:
	LRESULT HandleMessage(UINT uMsg, WPARAM wParam, LPARAM lParam);
	LRESULT OnCreate(UINT uMsg, WPARAM wParam, LPARAM lParam);
	LRESULT OnLButtonDown(UINT uMsg, WPARAM wParam, LPARAM lParam);
	LRESULT OnLButtonUp(UINT uMsg, WPARAM wParam, LPARAM lParam);
	LRESULT OnMouseMove(UINT uMsg, WPARAM wParam, LPARAM lParam);
	LRESULT OnMouseLeave(UINT uMsg, WPARAM wParam, LPARAM lParam);
	LRESULT OnSysCommand(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL &bHandled);
	void OnFinalMessage(HWND hWnd);
public:
	void SetProcess(INT32 i32Process);
	void CenterWindow();
protected:
	//绘制框架
	void DrawFrame();
	//绘制透明图片
	void PaintLayeredWindow(HDC hDC, HDC hMemDC,const int nWidth, const int nHeight);
	//绘制关闭按钮
	void PaintCloseButton(HDC hDC);
	//绘制背景
	void PaintBkimage(HDC hDC);
	//绘制标题
	void PaintTitle(HDC hDC);
	//绘制文件进度
	void PaintProgress(HDC hDC);
	void OnCancel();

	//绘制文字
	void PaintText(HDC hDC, int nFontSize, UINT uStyle, DWORD dwColor, RECT rc, LPCWSTR szText);
	//绘制按钮和文字
	void PaintButtonText(HDC hDC, LPCTSTR szText, int nFontSize, DWORD dwColor, CImageEx & img, RECT rcDest, RECT rcSrc);
	//根据鼠标位置调整按钮状态
	BTN_STATE AdustButtonState(POINT pt);

protected:
	//获取关闭按钮位置
	RECT GetCloseRect();
protected:

protected:
	int					m_nPos;
	ATL::CStringW		m_strTitle;
	BTN_STATE			m_eBtnState;						//按钮状态
	BOOL				m_bMouseTracking;					//鼠标进入标记
	CImageEx			m_ImgBK;
	CImageEx			m_ImgCloseNormal;
	CImageEx			m_ImgCloseHot;
	CImageEx			m_ImgProgressBar;

};


