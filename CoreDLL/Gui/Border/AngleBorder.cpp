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
			m_borderGeometry->SetPolygone({ {m_offset, m_width}, {m_offset + dx, 0}, {borderRect.w - m_width + dx, 0}, {borderRect.w - m_width, m_width} });
			break;

		case IBorder::BorderType::BOTTOM:
			dx = m_width / tanf(m_andleOxRadians);
			m_borderGeometry->SetPolygone({ {m_offset, 0}, {m_offset + dx, m_width}, {borderRect.w - m_width + dx, m_width}, {borderRect.w - m_width, 0} });
			break;

		case IBorder::BorderType::RIGHT:
			dy = m_width * tanf(m_andleOxRadians);
			m_borderGeometry->SetPolygone({ {0, m_width}, {m_width, m_width - dy}, {m_width, borderRect.h - m_offset - dy}, {0, borderRect.h - m_offset} });
			break;

		case IBorder::BorderType::LEFT:
			dy = m_width * -tanf(m_andleOxRadians);
			m_borderGeometry->SetPolygone({ {0, m_width - dy}, {m_width, m_width}, {m_width, borderRect.h - m_width}, {0, borderRect.h - m_width - dy} });
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