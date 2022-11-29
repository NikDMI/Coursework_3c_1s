#include "DefaultLayout.h"
#include "../../Bean/Exception.h"

namespace Nk {

	void DefaultLayout::AddWidget(Widget* widget) {
		if (widget == nullptr) {
			throw Exception{"Try to add nullptr widget to layout"};
		}
		m_layoutWidgets.push_back(widget);
	}


	void DefaultLayout::AddWidgets(const std::list<Widget*>& widgets) {
		for (Widget* widget : widgets) {
			if (widget == nullptr) throw Exception{ "Try to add nullptr widget to layout" };
			m_layoutWidgets.push_back(widget);
		}
	}


	void DefaultLayout::RemoveWidget(Widget* widget) {
		auto iterWidget = std::find(m_layoutWidgets.begin(), m_layoutWidgets.end(), widget);
		if (iterWidget == m_layoutWidgets.end()) {
			throw Exception{"Layout doesn't contain this widget"};
		}
		m_layoutWidgets.erase(iterWidget);
	}


	void DefaultLayout::ClearLayout() {
		m_layoutWidgets.clear();
	}


	void DefaultLayout::ComputeWidgetsPositions() {

	}



	DefaultLayout::~DefaultLayout() {

	}

}