#include "ControlElement.h"
#include "../../CoreDLL/Gui/Layout/DefaultLayout.h"


class ControlLayout : public DefaultLayout {
public:

	ControlLayout() : DefaultLayout() {

	}

	void ComputeWidgetsPositions() {
		//Widget* listWidget = m_layoutWidgets.back();
	}

private:
	const float DELTA_HEIGHT = 10;
	const float DELTA_WIDTH = 10;
	const float LABEL_HEIGHT = 25;
	const float SPACE_BETWEEN_CONTROLS = 10;

};

ControlElement::ControlElement(Widget* parentWidget, Widget* innerWidget) :PanelWindow(parentWidget), m_innerWidget{innerWidget} {
	SetResizingSystemMode(true);
}

/*

void PROC_CALL OnMouseMove(void* params) {
	MouseStructure* mouseStructure = (MouseStructure*)params;
	PushButton* btn = (PushButton*)mouseStructure->sender;
	btn->Button_OnMouseMove(params);
}

void PROC_CALL OnMouseLDown(void* params) {
	MouseStructure* mouseStructure = (MouseStructure*)params;
	PushButton* btn = (PushButton*)mouseStructure->sender;
	btn->Button_OnMouseLDown(params);
}


void PROC_CALL OnLMouseLUp(void* params) {
	MouseStructure* mouseStructure = (MouseStructure*)params;
	PushButton* btn = (PushButton*)mouseStructure->sender;
	btn->Button_OnLMouseLUp(params);
}


void PROC_CALL OnKillFocus(void* params) {
	MouseStructure* mouseStructure = (MouseStructure*)params;
	PushButton* btn = (PushButton*)mouseStructure->sender;
	btn->Button_OnLMouseLeave(params);
}
*/