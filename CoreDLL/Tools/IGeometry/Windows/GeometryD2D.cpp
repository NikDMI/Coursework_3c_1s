#include "GeometryD2D.h"
#include "../../../Bean/Exception.h"
#include "../../../Bean/Settings.h"

namespace Nk {

	GeometryD2D::GeometryD2D() {
		if (Settings::GetD2D1Factory()->CreatePathGeometry(m_pathGeometry.GetAddressOf()) != S_OK) {
			throw Exception{"Can't create geometry"};
		}
	}


	GeometryD2D::~GeometryD2D() {

	}


	void GeometryD2D::BeginRecord(Point_t startPoint) {
		if (m_isRecorded) {
			throw Exception{ "This geometry is also recorded" };
		}
		if (Settings::GetD2D1Factory()->CreatePathGeometry(m_pathGeometry.ReleaseAndGetAddressOf()) != S_OK) {
			throw Exception{ "Can't create geometry" };
		}
		HRESULT hr = m_pathGeometry->Open(m_geometrySink.ReleaseAndGetAddressOf());
		if (hr != S_OK) {
			throw Exception{ "Can't start recording" };
		}
		/*
		if (m_geometrySink == nullptr) {
			HRESULT hr = m_pathGeometry->Open(m_geometrySink.ReleaseAndGetAddressOf());
			if (hr != S_OK) {
				throw Exception{ "Can't start recording" };
			}
		}
		*/
		m_geometrySink->BeginFigure({ startPoint.x, startPoint.y }, D2D1_FIGURE_BEGIN_FILLED);
		m_isRecorded = true;
	}


	void GeometryD2D::EndRecord() {
		if (!m_isRecorded) {
			throw Exception{ "This geometry is not recorded" };
		}
		m_geometrySink->EndFigure(D2D1_FIGURE_END_CLOSED);
		m_geometrySink->Close();
		m_isRecorded = false;
	}


	void GeometryD2D::RecordNextPoint(Point_t point) {
		if (!m_isRecorded) {
			throw Exception{ "This geometry is not recorded" };
		}
		m_geometrySink->AddLine({ point.x, point.y });
	}


	void GeometryD2D::SetPolygone(std::initializer_list<Point_t> polygoneList) {
		if (polygoneList.size() < 3) {
			throw Exception{"There is not enought point to create polygone"};
		}
		auto polygoneIterator = polygoneList.begin();
		BeginRecord(*(polygoneIterator++));
		while (polygoneIterator != polygoneList.end()) {
			RecordNextPoint(*(polygoneIterator++));
		}
		EndRecord();
	}


	ComPtr<ID2D1Geometry> GeometryD2D::GetGeometryD2D1() {
		if (m_isRecorded || m_pathGeometry == nullptr) {
			throw Exception{ "Invalid geometry state" };
		}
		return m_pathGeometry;
	}

}