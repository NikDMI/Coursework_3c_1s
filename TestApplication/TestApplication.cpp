#include <Windows.h>

#undef CreateWindow

#include "../CoreDLL/Application/NkApplication.h"
#include "../CoreDLL/OsGui/WindowFactory.h"
#include "../CoreDLL/OsGui/IWindow.h"
#include "../CoreDLL/Gui/EventStructures/MouseStructure.h"
#include "../CoreDLL/OsGui/Windows/WindowWin32.h"

#include <string>

#pragma comment(lib,"CoreDLL.lib")

using namespace Nk;

void PROC_CALL OnMouseMove(void* params);

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE, LPSTR lpCmdLine, int nCmdShow) {
	Nk::NkApplication* app = new Nk::NkApplication{};
	Widget* widget = new Widget();
	widget->SetWindowGeometry(10, 10, 300, 300);
	widget->ShowWindow();
	Widget* widget2 = new Widget(widget);
	widget2->SetWindowGeometry(10, 10, 100, 100);
	widget2->ShowWindow();
	widget->m_eventHandler->AddEventHandler(widget->GetEventIndex(Widget::Events::ON_MOUSE_MOVE), { OnMouseMove });
	app->StartLoop();
	return 0;
}


void PROC_CALL OnMouseMove(void* params) {
	MouseStructure* ms = (MouseStructure*)params;
	std::wstring message = L"Mouse: x = " + std::to_wstring(ms->xCoord_Px) + L" y = " + std::to_wstring(ms->yCoord_Px);
	//MessageBox(NULL, message.c_str(), L"f", MB_OK);
}