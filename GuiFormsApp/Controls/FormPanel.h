#pragma once
#include "../../CoreDLL/Gui/Controls/Window/PanelWindow.h"
#include "../../CoreDLL/Gui/Controls/IBorderElement.h"
#include "../../CoreDLL/Gui/Controls/Label/Label.h"
#include "Form.h"

using namespace Nk;

class FormPanel : public PanelWindow, public IBorderElement {
public:
	FormPanel(Widget* widget);

private:
	Label* m_captionLabel;
	Form* m_form;

	friend class FormPanelLayout;
};
