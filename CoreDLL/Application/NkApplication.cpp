#define DLL_EXPORT_SIGNATURE

#include "NkApplication.h"
#include "../Bean/Exception.h"
#include "../Core/EventManager/EventManagerFactory.h"
#include "../SystemManager/Windows/WindowsSystemManager.h"

namespace Nk {

	//bool NkApplication::m_isCreated = false;
	NkApplication* NkApplication::m_nkApplication = nullptr;
	LONG volatile NkApplication::m_isActiveThread = 0;



	DWORD WINAPI GuiThread(LPVOID lpParam);


	NkApplication::NkApplication() {
		if (!m_nkApplication) {
			m_nkApplication = this;
			//Create managers
			m_eventManager = EventManagerFactory::GetEventManager(EventManagerFactory::ManagerType::QUEUE);
			m_systemManager = new WindowsSystemManager{};
			//Create gui thread
			m_isActiveThread = 1;
			//m_guiThread = CreateThread(NULL, 0, GuiThread, NULL, 0, NULL);
			//if (m_guiThread == INVALID_HANDLE_VALUE) {
			//	throw Exception{ "Can't craete gui thread" };
			//}
			
		}
		else {
			throw Exception{"Init NkApplication twice"};
		}
	}


	void NkApplication::StartLoop() {
		NkApplication* currentApplication = NkApplication::GetNkApplication();
		ISystemManager* systemManager = currentApplication->GetSystemManager();
		IEventManager* eventManager = currentApplication->GetEventManager();
		while (NkApplication::m_isActiveThread) {
			systemManager->DispatchSystemMessage(true);
			eventManager->DispatchEvent();
		}
	}


	IEventManager* NkApplication::GetEventManager() {
		if (m_nkApplication) {
			return m_nkApplication->m_eventManager;
		}
		throw Exception{"NkApplication didn't initialized"};
	}


	ISystemManager* NkApplication::GetSystemManager() {
		if (m_nkApplication) {
			return m_nkApplication->m_systemManager;
		}
		throw Exception{ "NkApplication didn't initialized" };
	}


	NkApplication* NkApplication::GetNkApplication() {
		return m_nkApplication;
	}


	NkApplication::~NkApplication() {
		//TerminateThread(m_guiThread, 0);
		//NkApplication::m_isActiveThread = 0;	//Thread safety(because gui thread only read this data)
		//WaitForSingleObject(m_guiThread, INFINITE);
		//CloseHandle(m_guiThread);
		delete m_eventManager;
		delete m_systemManager;
	}


	DWORD WINAPI GuiThread(LPVOID lpParam) {
		NkApplication* currentApplication = NkApplication::GetNkApplication();
		ISystemManager* systemManager = currentApplication->GetSystemManager();
		IEventManager* eventManager = currentApplication->GetEventManager();
		while (NkApplication::m_isActiveThread) {
			systemManager->DispatchSystemMessage(true);
			eventManager->DispatchEvent();
		}
		return 0;
	}
}