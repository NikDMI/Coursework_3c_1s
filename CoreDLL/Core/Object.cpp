#define DLL_EXPORT_SIGNATURE
#include "Object.h"
#include "../Bean/Exception.h"
#include <vector>

namespace Nk {

	using namespace std;

	//*************************************Addible structures******************************

	struct ClassInfo {
		ClassId classId;
		bool isOutsideTheCore = false;
		FabricMethod fabricMethod = nullptr;
	};

	//***************************************Global variables*******************************

	unordered_map<string, ClassId> Object::m_registeredClasses;
	ClassId Object::m_currentFreeClassId = 0;
	vector<ClassInfo> g_classesInformation;


	//**************************************Methods definitions*****************************

	Object::Object() {
		m_eventHandler = new EventHandler();
	}


	Object::~Object() {
		delete m_eventHandler;
	}

	ClassId Object::RegisterNewClass(std::string className) {
		if (m_registeredClasses.find(className) != m_registeredClasses.end()) {
			string message = "Class " + className + " was registered earlier";
			throw Exception{ message };
		}
		m_registeredClasses.insert({ className, m_currentFreeClassId });
		g_classesInformation.push_back({ m_currentFreeClassId });
		return m_currentFreeClassId++;
	}


	ClassId Object::RegisterNewClass(std::string className, FabricMethod fabricMethod) {
		if (m_registeredClasses.find(className) != m_registeredClasses.end()) {
			string message = "Class " + className + " was registered earlier";
			throw Exception{ message };
		}
		m_registeredClasses.insert({ className, m_currentFreeClassId });
		g_classesInformation.push_back({ m_currentFreeClassId, true, fabricMethod});
		return m_currentFreeClassId++;
	}

}