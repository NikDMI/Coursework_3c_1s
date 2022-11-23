#include <Windows.h>
#undef CreateWindow
#include "../CoreDLL/Application/NkApplication.h"
#include "../CoreDLL/OsGui/WindowFactory.h"
#include "../CoreDLL/OsGui/IWindow.h"

#pragma comment(lib,"CoreDLL.lib")

using namespace Nk;


int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE, LPSTR lpCmdLine, int nCmdShow) {
	Nk::NkApplication* app = new Nk::NkApplication{};
	IWindow* window = WindowFactory::CreateWindow(nullptr, OsType::Windows, WindowType::OVERLAPPED_WINDOW, PainterType::DirectX, nullptr);
	app->StartLoop();
	return 0;
}