#include "ILabel.h"

namespace Nk {

	ILabel::ILabel(Widget* parent) :Widget{parent} {

	}

	void ILabel::SetText(const std::wstring& userText) noexcept {
		m_labelText = userText;
	}

}