#define DLL_EXPORT_SIGNATURE

#include "WindowsSystemManager.h"
#include <Windows.h>

namespace Nk {

	WindowsSystemManager::WindowsSystemManager() {

	}


	WindowsSystemManager::~WindowsSystemManager() {

	}


	void WindowsSystemManager::DispatchSystemMessage(bool canBlock) {
		MSG msg;
		if (!canBlock) {
			if (PeekMessage(&msg, NULL, 0, 0, PM_REMOVE)) {
				TranslateMessage(&msg);
				DispatchMessage(&msg);
			}
		}
		else {
			GetMessage(&msg, NULL, 0, 0);
		}
	}
}