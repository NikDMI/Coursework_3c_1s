#include "Form.h"
#include "../GuiConfigs.h"
#include "../../CoreDLL/Tools/Cursor/ICursor.h"


Form::Form(Widget* widget) : PanelWindow(widget), IBorderElement(this) {
	SetBackgroundColor(m_formColor);
	gl_allWindows.formWindow = this;
}


void Form::AddNewControl(Controls control) {
	ICursor::SetGlobalMouseCoord(this->GetScreenCoordsLeftTop());

}
