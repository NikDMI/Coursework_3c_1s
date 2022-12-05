#include "AngleBorder.h"
#include <math.h>
#include "../../Tools/IGeometry/IGeometry.h"

constexpr float M_PI = 3.1415;

namespace Nk {

	void PROC_CALL AngleDrawProc(Widget* widget, IPainter* painter);

	AngleBorder::AngleBorder(Widget* parent, BorderType borderType, float borderWidth, float angleOxInDeegre) : IBorder{ parent, borderType, borderWidth }
	{
		m_andleOxRadians = (M_PI / 180.0) * angleOxInDeegre;
		m_borderGeometry = IGeometry::CreateGeometry();
		this->SetWindowDrawProc(AngleDrawProc);
	}


	void AngleBorder::InitBorderGeometry() {
		IBorder::BorderType borderType = GetBorderType();
		auto borderRect = GetWidgetRect();
		float dx; float dy;
		switch (borderType) {
		case IBorder::BorderType::TOP:
			dx = m_width / tanf(m_andleOxRadians);
			//dy = m_width * sinf(m_andleOxRadians);
			m_borderGeometry->SetPolygone({ {m_width, m_width}, {m_width + dx, 0}, {borderRect.w - m_width + dx, 0}, {borderRect.w - m_width, m_width} });
			break;

		case IBorder::BorderType::RIGHT:
			//dx = m_width * cosf(m_andleOxRadians);
			dy = m_width * tanf(m_andleOxRadians);
			m_borderGeometry->SetPolygone({ {0, m_width}, {m_width, m_width - dy}, {m_width, borderRect.h - m_width - dy}, {0, borderRect.h - m_width} });
			break;
		}
	}


	void PROC_CALL AngleDrawProc(Widget* widget, IPainter* painter) {
		AngleBorder* border = (AngleBorder*)widget;
		border->InitBorderGeometry();
		painter->SetBackgroundBrush(border->GetBorderBrush());
		painter->DrawGeometry(border->m_borderGeometry);
	}
}