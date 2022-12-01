#include "ICaption.h"

namespace Nk {

	ICaption::ICaption(Widget* parent, const std::wstring& text) :Widget{ parent }, ITextElement{this, text} {
	}

}