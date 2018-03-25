#pragma once

#include "BaseWnd.h"
#include <atlstr.h>
#include "ImageEx.h"

// ��ť״̬
typedef enum BTN_STATE
{
	ENomal = 0,			//û�н���
	ECloseHot,			//�رհ�ť���������״̬
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
	//���ƿ��
	void DrawFrame();
	//����͸��ͼƬ
	void PaintLayeredWindow(HDC hDC, HDC hMemDC,const int nWidth, const int nHeight);
	//���ƹرհ�ť
	void PaintCloseButton(HDC hDC);
	//���Ʊ���
	void PaintBkimage(HDC hDC);
	//���Ʊ���
	void PaintTitle(HDC hDC);
	//�����ļ�����
	void PaintProgress(HDC hDC);
	void OnCancel();

	//��������
	void PaintText(HDC hDC, int nFontSize, UINT uStyle, DWORD dwColor, RECT rc, LPCWSTR szText);
	//���ư�ť������
	void PaintButtonText(HDC hDC, LPCTSTR szText, int nFontSize, DWORD dwColor, CImageEx & img, RECT rcDest, RECT rcSrc);
	//�������λ�õ�����ť״̬
	BTN_STATE AdustButtonState(POINT pt);

protected:
	//��ȡ�رհ�ťλ��
	RECT GetCloseRect();
protected:

protected:
	int					m_nPos;
	ATL::CStringW		m_strTitle;
	BTN_STATE			m_eBtnState;						//��ť״̬
	BOOL				m_bMouseTracking;					//��������
	CImageEx			m_ImgBK;
	CImageEx			m_ImgCloseNormal;
	CImageEx			m_ImgCloseHot;
	CImageEx			m_ImgProgressBar;

};


