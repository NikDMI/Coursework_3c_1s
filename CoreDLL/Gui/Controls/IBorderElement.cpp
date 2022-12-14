#include "IBorderElement.h"
#include "../Border/NormalBorder.h"

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
		if (m_leftBorder) {
			delete m_leftBorder;
		}
		if (m_bottomBorder) {
			delete m_bottomBorder;
		}
		//m_topBorder = new AngleBorder{ m_widget, IBorder::BorderType::TOP, DEFAULT_WIDTH, DEFAULT_ANGLE };
		m_topBorder = new NormalBorder{ m_widget, IBorder::BorderType::TOP, DEFAULT_WIDTH};
		m_topBorder->GetBorderBrush()->SetColor(color);
		//m_rightBorder = new AngleBorder{ m_widget, IBorder::BorderType::RIGHT, DEFAULT_WIDTH, DEFAULT_ANGLE };
		m_rightBorder = new NormalBorder{ m_widget, IBorder::BorderType::RIGHT, DEFAULT_WIDTH};
		m_rightBorder->GetBorderBrush()->SetColor(color);
		color.r -= 0.05; color.g -= 0.05; color.b -= 0.05;
		m_leftBorder = new NormalBorder{ m_widget, IBorder::BorderType::LEFT, DEFAULT_WIDTH };
		m_leftBorder->GetBorderBrush()->SetColor(color);
		//m_rightBorder = new AngleBorder{ m_widget, IBorder::BorderType::RIGHT, DEFAULT_WIDTH, DEFAULT_ANGLE };
		m_bottomBorder = new NormalBorder{ m_widget, IBorder::BorderType::BOTTOM, DEFAULT_WIDTH };
		m_bottomBorder->GetBorderBrush()->SetColor(color);
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