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
	widget->SetWindowGeometry(10, 10, 1000, 500);
	//widget->ShowWindow();
	Widget* widget2 = new Widget(widget, {0.3, 0.4, 0.5, 0.6});
	widget2->SetWindowGeometry(10, 10, 200, 200);
	Widget* widget3 = new Widget(widget2, { 0.5, 0.9, 0.5, 0.6 });
	widget3->SetWindowGeometry(10, 10, 100, 100);
	widget->ShowWindow();
	widget2->m_eventHandler->AddEventHandler(widget2->GetEventIndex(Widget::Events::ON_MOUSE_MOVE), { OnMouseMove });
	app->StartLoop();
	return 0;
}


void PROC_CALL OnMouseMove(void* params) {
	MouseStructure* ms = (MouseStructure*)params;
	std::wstring message = L"Mouse: x = " + std::to_wstring(ms->xCoord_Px) + L" y = " + std::to_wstring(ms->yCoord_Px);
	//MessageBox(NULL, message.c_str(), L"f", MB_OK);
}