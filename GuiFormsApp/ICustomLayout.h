#include "../CoreDLL/Gui/Layout/StackLayout.h"

class CustomLayout : public Nk::StackLayout {
public:

	CustomLayout();

	void ComputeWidgetsPositions() override;

private:
	static const int BOTTOM_OFFSET = 15;

};
