#include<Windows.h>
#include<atlstr.h>
using namespace ATL;
#include"MiaUpdateFrame.h"

#define UI_WNDSTYLE_DIALOG     (WS_VISIBLE | WS_POPUPWINDOW | WS_CAPTION | WS_DLGFRAME | WS_CLIPSIBLINGS | WS_CLIPCHILDREN)

extern "C" int WINAPI _tWinMain(HINSTANCE hInstance, HINSTANCE /*hPrevInstance*/,
	LPTSTR /*lpCmdLine*/, int nShowCmd)
{

	CMiaUpdateFrame * pMiaUpdateFrame = new CMiaUpdateFrame();
	if (pMiaUpdateFrame == NULL)
		return NULL;
	pMiaUpdateFrame->Create(hInstance, NULL, _T("米亚公开课升级"), UI_WNDSTYLE_DIALOG, WS_EX_LAYERED);
	if (!::IsWindow(pMiaUpdateFrame->GetSafeHwnd()))
	{
		delete pMiaUpdateFrame;
		return NULL;
	}
	pMiaUpdateFrame->CenterWindow();
	
 
   MSG msg = { 0 };
   while (GetMessage(&msg, NULL, 0, 0) > 0)
   {
	   TranslateMessage(&msg);
	   DispatchMessage(&msg);
   }
	return 0;
}