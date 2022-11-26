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

DWORD WINAPI LogicThread(LPVOID);

Widget* widget = nullptr;
Widget* widget3 = nullptr;

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE, LPSTR lpCmdLine, int nCmdShow) {
	Nk::NkApplication* app = new Nk::NkApplication{};
	widget = new Widget{ nullptr, {0.8, 0.8, 0.8, 1.0} };
	widget->SetWindowGeometry(10, 10, 1000, 500);
	//widget->ShowWindow();
	Widget* widget2 = new Widget(widget, {0.3, 0.4, 0.5, 0.6});
	widget2->SetWindowGeometry(10, 10, 200, 200);
	widget3 = new Widget(widget2, { 0.5, 0.9, 0.5, 0.6 });
	widget3->SetWindowGeometry(-50, -50, 100, 100);
	widget->ShowWindow();
	widget->m_eventHandler->AddEventHandler(widget->GetEventIndex(Widget::Events::ON_MOUSE_MOVE), { OnMouseMove });
	CreateThread(NULL, 0, LogicThread, widget3, 0, NULL);
	app->StartLoop();
	return 0;
}


DWORD WINAPI LogicThread(LPVOID param) {
	Widget* w = (Widget*)param;
	Sleep(1000);
	w->SetWindowGeometry(20, 20, 50, 50);
	w->Repaint();
	Sleep(1000);
	w->SetBackgroundColor({ 0.8, 0.2, 0.3, 1.0 });
	w->SetWindowGeometry(20, 20, 100, 100);
	widget->Repaint();
	return 0;
}


void PROC_CALL OnMouseMove(void* params) {
	MouseStructure* ms = (MouseStructure*)params;
	widget3->SetWindowGeometry(00, 00, ms->xCoord_Px % 200, ms->yCoord_Px % 200);
	//widget3->Repaint();
}