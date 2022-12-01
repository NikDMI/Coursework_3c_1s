#include "Label.h"

namespace Nk {

	void PROC_CALL LabelDrawProc(Widget* widget, IPainter* painter);

	Label::Label(const std::wstring& labelText, Widget* parent) : ILabel{ parent } {
		SetText(labelText);
		m_widgetPainter = GetPainter();
		this->SetWindowDrawProc(LabelDrawProc);
	}


	Label::Label(const std::wstring& labelText) : Label{labelText, nullptr} {

	}

	Label::~Label() {

	}


	void PROC_CALL LabelDrawProc(Widget* widget, IPainter* painter) {
		Widget::BasicDrawProc(widget, painter);
		Label* label = (Label*)widget;
		Rect_t labelRect = label->GetWidgetClientRect();
		labelRect.x = 0; labelRect.y = 0;
		painter->DrawText(labelRect, label->m_elementText);
	}
}