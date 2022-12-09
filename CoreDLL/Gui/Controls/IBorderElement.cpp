#include "IBorderElement.h"

namespace Nk {

	IBorderElement::IBorderElement(Widget* thisWidget) : m_widget{thisWidget} {

	}

	void IBorderElement::SetNormalBorder(Color_t color) {
		if (m_topBorder) {
			delete m_topBorder;
		}
		if (m_rightBorder) {
			delete m_rightBorder;
		}
		m_topBorder = new AngleBorder{ m_widget, IBorder::BorderType::TOP, DEFAULT_WIDTH, DEFAULT_ANGLE };
		m_topBorder->GetBorderBrush()->SetColor(color);
		m_rightBorder = new AngleBorder{ m_widget, IBorder::BorderType::RIGHT, DEFAULT_WIDTH, DEFAULT_ANGLE };
		color.r -= 0.05; color.g -= 0.05; color.b -= 0.05;
		m_rightBorder->GetBorderBrush()->SetColor(color);
	}

	/*
	void IBorderElement::DeleteBorders() {
		if (m_topBorder) {
			delete m_topBorder;
		}
		if (m_rightBorder) {
			delete m_rightBorder;
		}
	}
	*/
}