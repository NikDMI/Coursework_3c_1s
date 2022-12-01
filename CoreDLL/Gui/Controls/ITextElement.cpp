#include "ITextElement.h"

namespace Nk {

	ITextElement::ITextElement(Widget* widget, std::wstring text) : m_elementText{text} {
		m_elementFont = widget->GetPainter()->CreateFontObject();
		widget->GetPainter()->SetFont(m_elementFont);
	}

	void ITextElement::SetText(const std::wstring& text) noexcept {
		m_elementText = text;
	}

	IFont* ITextElement::GetElementFont() {
		return m_elementFont;
	}
}