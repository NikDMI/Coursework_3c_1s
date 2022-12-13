#include "ILayout.h"
#include "../../Bean/Exception.h"

namespace Nk {

	ILayout::ILayout() {

	}

	void ILayout::AddWidget(Widget* widget) {
		if (widget == nullptr) {
			throw Exception{ "Try to add nullptr widget to layout" };
		}
		m_layoutWidgets.push_back(widget);
	}


	void ILayout::AddWidgets(const std::list<Widget*>& widgets) {
		for (Widget* widget : widgets) {
			if (widget == nullptr) throw Exception{ "Try to add nullptr widget to layout" };
			m_layoutWidgets.push_back(widget);
		}
	}


	void ILayout::RemoveWidget(Widget* widget) {
		auto iterWidget = std::find(m_layoutWidgets.begin(), m_layoutWidgets.end(), widget);
		if (iterWidget == m_layoutWidgets.end()) {
			throw Exception{ "Layout doesn't contain this widget" };
		}
		m_layoutWidgets.erase(iterWidget);
	}


	void ILayout::ClearLayout() {
		m_layoutWidgets.clear();
	}
}