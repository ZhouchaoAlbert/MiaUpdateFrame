#pragma once

#include <Windows.h>
#include <tchar.h>

/*
�������ܣ����ڻ��࣬��ʵ��windows������Ϣ��ת
*/

class CBaseWnd
{
public:
	CBaseWnd(void) : m_hWnd(NULL), m_OldWndProc(::DefWindowProc) { }
	virtual ~CBaseWnd(void) { }
public:
	// ��������
	HWND Create(HINSTANCE hInst,
		HWND hwndParent,
		LPCTSTR pstrName,
		DWORD dwStyle,
		DWORD dwExStyle,
		int x = CW_USEDEFAULT,
		int y = CW_USEDEFAULT,
		int cx = CW_USEDEFAULT,
		int cy = CW_USEDEFAULT,
		HMENU hMenu = NULL) {
		RegisterWindowClass(hInst);
		m_hWnd = ::CreateWindowEx(dwExStyle, GetWindowClassName(), pstrName, dwStyle, x, y, cx, cy, hwndParent, hMenu, hInst, this);
		return m_hWnd;
	}

	// ��ȡ���ھ��
	HWND GetSafeHwnd() {
		return m_hWnd;
	}
protected:
	//
	// ������д�����ǵ��ò���Ҫ��ע
	//
	// ��������
	virtual LPCTSTR GetWindowClassName() const{
		return _T("BaseWndClass");
	}

	// ������Ϣ�ص�����Ҫ����������Ϣ�������
	virtual LRESULT HandleMessage(UINT uMsg, WPARAM wParam, LPARAM lParam){
		return ::DefWindowProc(m_hWnd, uMsg, wParam, lParam);
	}

	// �����������յ��ú���
	virtual void OnFinalMessage(HWND hWnd) {}
protected:
	//
	// �ڲ��̶�����
	//
	// ע�ᴰ����
	bool RegisterWindowClass(HINSTANCE hInst) {
		WNDCLASSEX wc = { 0 };
		wc.cbSize = sizeof(WNDCLASSEX);
		wc.style = 0;
		wc.cbClsExtra = 0;
		wc.cbWndExtra = 0;
		wc.hIcon = NULL;
		wc.lpfnWndProc = CBaseWnd::__WndProc;
		wc.hInstance = hInst;
		wc.hCursor = ::LoadCursor(NULL, IDC_ARROW);
		wc.hbrBackground = NULL;
		wc.lpszMenuName = NULL;
		wc.lpszClassName = GetWindowClassName();
		ATOM ret = ::RegisterClassEx(&wc);
		return ret != NULL || ::GetLastError() == ERROR_CLASS_ALREADY_EXISTS;
	}

	// ��Ϣ�ص�
	static LRESULT CALLBACK __WndProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam){
		CBaseWnd* pThis = NULL;
		if (uMsg == WM_NCCREATE) {
			LPCREATESTRUCT lpcs = reinterpret_cast<LPCREATESTRUCT>(lParam);
			pThis = static_cast<CBaseWnd*>(lpcs->lpCreateParams);
			pThis->m_hWnd = hWnd;
			::SetWindowLongPtr(hWnd, GWLP_USERDATA, reinterpret_cast<LPARAM>(pThis));
		}
		else {
			pThis = reinterpret_cast<CBaseWnd*>(::GetWindowLongPtr(hWnd, GWLP_USERDATA));
			if (uMsg == WM_NCDESTROY && pThis != NULL) {
				LRESULT lRes = ::CallWindowProc(pThis->m_OldWndProc, hWnd, uMsg, wParam, lParam);
				::SetWindowLongPtr(pThis->m_hWnd, GWLP_USERDATA, 0L);
				pThis->m_hWnd = NULL;
				pThis->OnFinalMessage(hWnd);
				return lRes;
			}
		}
		if (pThis != NULL) {
			return pThis->HandleMessage(uMsg, wParam, lParam);
		}
		else {
			return ::DefWindowProc(hWnd, uMsg, wParam, lParam);
		}
	}
protected:
	HWND			m_hWnd;
	WNDPROC			m_OldWndProc;
};
