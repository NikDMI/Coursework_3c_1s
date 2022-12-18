#pragma once
#include "../CoreDLL/Painter/Font/IFont.h"
#include "../CoreDLL/Gui/Controls/Slider/ScrollBar.h"
#include "../CoreDLL/Gui/Controls/Button/ImageButton.h"
#include <string>

using namespace std;
using namespace Nk;


const wstring defaultFontFamily = L"Times New Roman";
const int fontSizeInPx = 25;
const Color_t SCROLL_BAR_COLOR = { 0.57, 0.58, 0.59, 1.0 };
const Color_t SCROLL_BAR_ELEMENT_COLOR = { 0.49, 0.5, 0.52, 1.0 };
const Color_t SCROLL_BAR_ELEMENT_MOVE_COLOR = { 0.54, 0.54, 0.55, 1.0 };
const Color_t SCROLL_BAR_ELEMENT_PUSH_COLOR = { 0.45, 0.46, 0.47, 1.0 };
const Color_t SCROLL_BAR_ELEMENT_BORDER_COLOR = SCROLL_BAR_COLOR;

inline void SetDefaultFont(IFont* fontObject) {
	fontObject->SetFamily(defaultFontFamily);
	fontObject->SetSizeInPixels(fontSizeInPx);
}

inline void SetDefaultCenterFont(IFont* fontObject) {
	SetDefaultFont(fontObject);
	fontObject->SetHorizontalAlignment(IFont::HorizontalAlignment::CENTER);
	fontObject->SetVerticalAlignment(IFont::VerticalAlignment::CENTER);
}


inline void ConfigScrollBarColor(ScrollBar* scrollBar) {
	scrollBar->SetBackgroundColor(SCROLL_BAR_COLOR);
	auto imageButton = scrollBar->GetScrollElement();
	imageButton->SetButtonColor(IColorElement::ElementState::STATIC, SCROLL_BAR_ELEMENT_COLOR);
	imageButton->SetButtonColor(IColorElement::ElementState::HOVER, SCROLL_BAR_ELEMENT_MOVE_COLOR);
	imageButton->SetButtonColor(IColorElement::ElementState::PUSH, SCROLL_BAR_ELEMENT_PUSH_COLOR);
	imageButton->SetBackgroundColor(SCROLL_BAR_ELEMENT_COLOR);
	imageButton->SetNormalBorder(SCROLL_BAR_ELEMENT_BORDER_COLOR);
}

