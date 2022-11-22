#ifndef EXCEPTION_CORE_DLL
#define EXCEPTION_CORE_DLL

#include <string>

namespace Nk {
	
	class Exception {
	public:
		Exception();
		Exception(std::string message);
		Exception(std::string message, Exception& innerException);
		Exception(const Exception& copy) = default;
	private:
		std::string m_message;
		Exception* innerException = nullptr;
	};
}

#endif	//#ifndef EXCEPTION_CORE_DLL
