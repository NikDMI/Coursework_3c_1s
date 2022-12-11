#ifndef SCROLL_BAR_GUI_DLL
#define SCROLL_BAR_GUI_DLL

#include "ISlider.h"
#include "../Button/ImageButton.h"

namespace Nk {

	CLASS_PARAMS class ScrollBar : public ISlider {
	public:

		CLASS_METHOD ScrollBar(Widget* parent = nullptr, int32_t lowerValue = 1, int32_t upperValue = 100);

		CLASS_METHOD ImageButton* GetScrollElement() const noexcept;

		~ScrollBar() override;

	protected:

		void ProcessScrollEvent() override;

	private:
		static const Color_t DEFAULT_COLOR;

		ImageButton* m_scrollElement;
		bool m_isScrolled = false;
		Point_t m_fixCursorPoint;
		Rect_t m_startScrollElementPosition;

		void MoveScrollElement();

		friend void PROC_CALL OnMouseDownScrollElement(void* params);
		friend void PROC_CALL OnMouseUpScrollElement(void* params);
		friend void PROC_CALL OnMouseMoveScrollElement(void* params);
		friend void PROC_CALL OnParentResizeScrollElement(void* params);


	};

}

#endif	//#ifndef SCROLL_BAR_GUI_DLL