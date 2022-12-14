#include "NormalBorder.h"
#include <math.h>
#include "../../Tools/IGeometry/IGeometry.h"

constexpr float M_PI = 3.1415;

namespace Nk {


	NormalBorder::NormalBorder(Widget* parent, BorderType borderType, float borderWidth) : IBorder{ parent, borderType, borderWidth }
	{
		ShowWindow();
	}


	void NormalBorder::InitBorderGeometry() {
		IBorder::BorderType borderType = GetBorderType();
		auto borderRect = GetWidgetRect();
		switch (borderType) {
		case IBorder::BorderType::TOP:
			m_borderGeometry->SetPolygone({ {m_offset, m_width}, {0, 0}, {borderRect.w, 0}, {borderRect.w - m_width, m_width} });
			break;

		case IBorder::BorderType::BOTTOM:
			m_borderGeometry->SetPolygone({ {m_offset, 0}, {0, m_width}, {borderRect.w, m_width}, {borderRect.w - m_width, 0} });
			break;

		case IBorder::BorderType::LEFT:

			m_borderGeometry->SetPolygone({ {0, 0}, {m_width, m_width}, {m_width, borderRect.h - m_offset}, {0, borderRect.h} });
			break;

		case IBorder::BorderType::RIGHT:
			m_borderGeometry->SetPolygone({ {0, m_width}, {m_width, 0}, {m_width, borderRect.h}, {0, borderRect.h - m_width} });
			break;
		}
	}


	/*
	void PROC_CALL AngleDrawProc(Widget* widget, IPainter* painter) {
		AngleBorder* border = (AngleBorder*)widget;
		border->InitBorderGeometry();
		painter->SetBackgroundBrush(border->GetBorderBrush());
		painter->DrawGeometry(border->m_borderGeometry);
	}
	*/
}