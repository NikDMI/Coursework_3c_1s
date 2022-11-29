#ifndef LABLE_GUI_DLL
#define LABLE_GUI_DLL

#include "ILabel.h"
#include "../../../Painter/Font/IFont.h"
#include "../../../Painter/IPainter.h"

namespace Nk {

	CLASS_PARAMS class Label : public ILabel {
	public:

		CLASS_METHOD Label(const std::wstring& labelText, Widget* parent);

		CLASS_METHOD Label(const std::wstring& labelText = {});


		IFont* GetElementFont() override;

		~Label() override;

	private:
		IPainter* m_widgetPainter;
		IFont* m_labelFont;

		friend void LabelDrawProc(Widget* widget, IPainter* painter);
	};

}

#endif	//#ifndef ILAYOUT_GUI_DLL