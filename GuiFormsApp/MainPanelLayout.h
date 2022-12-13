#pragma once
#include "../CoreDLL/Gui/Layout/StackLayout.h"

class MainPanelLayout : public Nk::StackLayout {
public:

	MainPanelLayout();

	void ComputeWidgetsPositions() override;

private:
	static const float CHILDS_HEIGHT_COEFF;

};
