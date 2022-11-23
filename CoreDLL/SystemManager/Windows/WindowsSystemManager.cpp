#include "WindowsSystemManager.h"
#include <Windows.h>

namespace Nk {

	WindowsSystemManager::WindowsSystemManager() {

	}


	WindowsSystemManager::~WindowsSystemManager() {

	}


	void WindowsSystemManager::DispatchSystemMessage() {
		MSG msg;
		if (PeekMessage(&msg, NULL, 0, 0, PM_REMOVE)) {
			TranslateMessage(&msg);
			DispatchMessage(&msg);
		}
	}
}