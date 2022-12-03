#include "IGeometry.h"
#include "../../Application/NkApplication.h"
#include "Windows/GeometryD2D.h"

namespace Nk {

	IGeometry* IGeometry::CreateGeometry() {
		switch (NkApplication::GetNkApplication()->GetPainterType()) {
		case PainterType::DirectX:
			return new GeometryD2D();
			break;
		}
		return nullptr;
	}

}