#include "Form.h"


Form::Form(Widget* widget) : PanelWindow(widget), IBorderElement(this) {
	SetBackgroundColor(m_formColor);
}
