#ifndef GEOMETRYD2D_TOOLS_DLL
#define GEOMETRYD2D_TOOLS_DLL


#include "../IGeometry.h"
#include <d2d1.h>
#include <wrl.h>


namespace Nk {

	using Microsoft::WRL::ComPtr;

	/*
	* This class represent information about user geometry
	*/
	CLASS_PARAMS class GeometryD2D : public IGeometry {
	public:
		GeometryD2D();

		~GeometryD2D() override;

		CLASS_METHOD void BeginRecord(Point_t startPoint) override;

		CLASS_METHOD void EndRecord() override;

		CLASS_METHOD void RecordNextPoint(Point_t point) override;

		CLASS_METHOD void SetPolygone(std::initializer_list<Point_t> polygoneList) override;

		ComPtr<ID2D1Geometry> GetGeometryD2D1();

	private:
		bool m_isRecorded = false;
		ComPtr<ID2D1PathGeometry> m_pathGeometry;
		ComPtr<ID2D1GeometrySink> m_geometrySink;
	};

}

#endif	//#ifndef GEOMETRYD2D_TOOLS_DLL