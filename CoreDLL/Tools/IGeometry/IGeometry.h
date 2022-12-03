#ifndef IGEOMETRY_TOOLS_DLL
#define IGEOMETRY_TOOLS_DLL


#include "../../Bean/Config.h"
#include <initializer_list>


namespace Nk {

	/*
	* This class represent information about user geometry (in DI pixels)
	*/
	CLASS_PARAMS class IGeometry{
	public:
		virtual ~IGeometry() {};

		/*
		* Fabric method that create geometry according to the target platform
		*/
		CLASS_METHOD static IGeometry* CreateGeometry();

		CLASS_METHOD virtual void BeginRecord(Point_t startPoint) = 0;

		CLASS_METHOD virtual void EndRecord() = 0;

		CLASS_METHOD virtual void RecordNextPoint(Point_t point) = 0;

		/*
		* Create a specialized polygone
		*/
		CLASS_METHOD virtual void SetPolygone(std::initializer_list<Point_t> polygoneList) = 0;


	protected:
		IGeometry() {}

	};

}

#endif	//#ifndef IGEOMETRY_TOOLS_DLL