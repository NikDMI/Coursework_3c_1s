#include "IBorder.h"
#include "../../Bean/Exception.h"
#include "../EventStructures/BasicWidgetStructure.h"

namespace Nk {


	void PROC_CALL OnParentResize(void* params);

	IBorder::IBorder(Widget* parent, BorderType borderType, float borderWidth) : Widget{parent},
		m_width{ borderWidth }, m_borderType{ borderType } 
	{
		if (parent == nullptr) {
			throw Exception{"Border can't be exists without parent control"};
		}
		parent->SetBorder(this);
		SetBackgroundColor({ 0, 0, 0, 0 });
		SetBorderSize();
		m_borderBrush = GetPainter()->CreateBrushObject({ 0.5, 0.5, 0.5, 1 });
		SetCustomEvent(Widget::CustomEvents::ON_PARENT_RESIZE, OnParentResize);
		this->SetParentNotification();
	}


	float IBorder::GetBorderWidth() {
		return m_width;
	}

	/*
	void IBorder::SetBorderWidth(float width) {
		if (width < 0) {
			throw Exception{"Invalid width"};
		}
		m_width = width;
		this->Repaint();
	}
	*/


	void IBorder::SetBorderOffset(float offset) {
		m_offset = offset;
	}


	float IBorder::GetBorderOffset() {
		return m_offset;
	}


	IBorder::BorderType IBorder::GetBorderType() {
		return m_borderType;
	}

	IBrush* IBorder::GetBorderBrush() {
		return m_borderBrush;
	}


	void IBorder::SetBorderSize() {
		auto parentRect = GetParentWidget()->GetWidgetRect();
		switch (m_borderType) {
		case BorderType::TOP:
			SetWindowGeometry(0, 0, parentRect.w, m_width);
			break;

		case BorderType::BOTTOM:
			SetWindowGeometry(0, parentRect.h - m_width, parentRect.w, m_width);
			break;

		case BorderType::LEFT:
			SetWindowGeometry(0, 0, m_width, parentRect.h);
			break;

		case BorderType::RIGHT:
			SetWindowGeometry(parentRect.w - m_width, 0, m_width, parentRect.h);
			break;
		}
	}


	void PROC_CALL OnParentResize(void* params) {
		BasicWidgetStructure* bs = (BasicWidgetStructure*)params;
		IBorder* border = (IBorder*)bs->sender;
		border->SetBorderSize();
	}
}