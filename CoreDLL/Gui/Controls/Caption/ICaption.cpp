#include "ICaption.h"

namespace Nk {

	ICaption::ICaption(Widget* parent, const std::wstring& text) :Widget{parent} {
		//Widget* parent = 
		this->SetText(text);
	}


	void ICaption::SetText(const std::wstring& caption) noexcept {
		this->m_captionText = caption;
	}

}