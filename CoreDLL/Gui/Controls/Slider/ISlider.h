#ifndef ISLIDER_WIDGET_GUI_DLL
#define ISLIDER_WIDGET_GUI_DLL

#include "../../Widget.h"
#include "../../../Bean/Config.h"
#include <numeric>

namespace Nk {

	/*
	* This class represent control, that can capture mouse events
	*/
	CLASS_PARAMS class ISlider : public Widget {
	public:
		struct SliderRange {
			int32_t lowerValue = 0;
			int32_t upperValue = 0;
			int32_t currentValue = 0;
		};


		ISlider(Widget* parent, int32_t lowerValue = 1, int32_t upperValue = 100);

		virtual ~ISlider() {};

		/*
		* Sets max min current value to the slider widget and send scroll event
		*/
		CLASS_METHOD void SetRange(SliderRange sliderRange);
		CLASS_METHOD SliderRange GetRange();


		CLASS_METHOD int32_t GetCurrentValue() const noexcept;

		/*
		* Send scroll event to this widget
		*/
		CLASS_METHOD void SetCurrentValue(int32_t value) noexcept;

		/*
		* This method is system. Repaint sliders if needed
		* Send before user callback will be called
		*/
		virtual void ProcessScrollEvent();


	protected:
		int32_t m_lowerValue;
		int32_t m_upperValue;
		int32_t m_currentValue;

		void SendScrollEvent();


	private:
	};

}

#endif	//#ifndef ISLIDER_WIDGET_GUI_DLL