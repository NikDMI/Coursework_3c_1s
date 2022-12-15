#pragma once
#include "../CoreDLL/Painter/Font/IFont.h"
#include <string>

using namespace std;
using namespace Nk;


wstring defaultFontFamily = L"Times New Roman";
int fontSizeInPx = 25;

void SetDefaultFont(IFont* fontObject) {
	fontObject->SetFamily(defaultFontFamily);
	fontObject->SetSizeInPixels(fontSizeInPx);
}

void SetDefaultCenterFont(IFont* fontObject) {
	SetDefaultFont(fontObject);
	fontObject->SetHorizontalAlignment(IFont::HorizontalAlignment::CENTER);
	fontObject->SetVerticalAlignment(IFont::VerticalAlignment::CENTER);
}

