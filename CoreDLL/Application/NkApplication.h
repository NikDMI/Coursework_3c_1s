#ifndef NK_APPLICATION_DLL
#define NK_APPLICATION_DLL

#include <Windows.h>
#include "../SystemManager/ISystemManager.h"
#include "../Core/EventManager/IEventManager.h"
#include "../Bean/Config.h"


namespace Nk {

	/*
	* Represent configurations of the application
	*/
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

		/*
		* Checks if current thread id is owned by gui
		*/
		static bool IsGuiThread() noexcept;

		/*
		* Starts main loop of message process (user must create thread for gui loop by himself)
		*/
		CLASS_METHOD void StartLoop();

		/*
		* Send stop message to the loop
		*/
		CLASS_METHOD void StopLoop();
	private:

		static NkApplication* m_nkApplication;	//Singelton
		HANDLE m_guiThread;	//Gui thread (for message queue)
		IEventManager* m_eventManager;
		ISystemManager* m_systemManager;

		static volatile LONG m_isActiveThread;
		static DWORD m_guiThreadId;

		friend DWORD WINAPI GuiThread(LPVOID lpParam);
	};
}

#endif	//#ifndef NK_APPLICATION_DLL

