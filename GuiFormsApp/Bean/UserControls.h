#pragma once
#include "../../CoreDLL/Tools/Bitmap/IBitmap.h"
#include "../../CoreDLL/Gui/Controls/Button/LabelImageButton.h"
#include <string>
#include <vector>

using namespace Nk;
using namespace std;

#undef LoadBitmap

class UserControl;

extern vector<UserControl*> userControls;

struct UserControl {
	wstring controlName;
	IBitmap* controlImage;
	LabelImageButton* labelImageButton;

	UserControl(wstring imagePath, const wstring& controlName, Widget* parent) {
		controlImage = IBitmap::LoadBitmap(imagePath);
		this->controlName = controlName;
		labelImageButton = new LabelImageButton(controlImage, controlName, parent);
		labelImageButton->SetWindowGeometry(0, 0, 0, 30);
		userControls.push_back(this);
	}
};


