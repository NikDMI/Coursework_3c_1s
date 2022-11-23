#ifndef NK_APPLICATION_DLL
#define NK_APPLICATION_DLL

#include <Windows.h>
#include "../SystemManager/ISystemManager.h"
#include "../Core/EventManager/IEventManager.h"
#include "../Bean/Config.h"


namespace Nk {

	CLASS_PARAMS class NkApplication {
	public:
		/*
		* Initialize application resources and GUI thread
		*/
		CLASS_METHOD NkApplication();
		~NkApplication();

		static IEventManager* GetEventManager();

		static ISystemManager* GetSystemManager();

		static NkApplication* GetNkApplication();

		CLASS_METHOD void StartLoop();
	private:

		static NkApplication* m_nkApplication;	//Singelton
		HANDLE m_guiThread;	//Gui thread (for message queue)
		IEventManager* m_eventManager;
		ISystemManager* m_systemManager;

		static volatile LONG m_isActiveThread;

		friend DWORD WINAPI GuiThread(LPVOID lpParam);
	};
}

#endif	//#ifndef NK_APPLICATION_DLL

