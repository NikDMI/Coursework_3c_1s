#include "FormPanel.h"
#include "../../CoreDLL/Gui/ResizeManager/RectangleResizer.h"

const Color_t INNER_PANEL_COLOR = { 0.6, 0.6 , 0.6, 1.0 };
const Color_t INNER_PANEL_BORDER_COLOR = { 0.5, 0.5 , 0.5, 1.0 };

FormPanel::FormPanel(Widget* widget) : PanelWindow(widget), IBorderElement(this) {
	RectangleResizer* rectangleResizer = new RectangleResizer();
	SetWindowGeometry(0, 0, 300, 300);
	SetBackgroundColor(INNER_PANEL_COLOR);
	SetResizeManager(rectangleResizer);
	SetNormalBorder(INNER_PANEL_BORDER_COLOR);
}