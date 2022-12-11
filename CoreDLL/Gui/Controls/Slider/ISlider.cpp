#include "ISlider.h"
#include "../../../Bean/Exception.h"
#include "../../EventStructures/ScrollStructure.h"

namespace Nk {

	ISlider::ISlider(Widget* parent, int32_t lowerValue, int32_t upperValue) :Widget{ parent }
	{
		SetRange({ lowerValue, upperValue, lowerValue });
	}


	void ISlider::SetRange(SliderRange sliderRange) {
		if (sliderRange.lowerValue > sliderRange.lowerValue) {
			throw Exception{ "Bad range" };
		}
		m_lowerValue = sliderRange.lowerValue;
		m_upperValue = sliderRange.upperValue;
		SetCurrentValue(sliderRange.currentValue);
	}


	ISlider::SliderRange ISlider::GetRange() {
		return { m_lowerValue , m_upperValue , m_currentValue };
	}


	int32_t ISlider::GetCurrentValue() const noexcept {
		return m_currentValue;
	}


	void ISlider::SetCurrentValue(int32_t value) noexcept {
		if (value < m_lowerValue) {
			m_currentValue = m_lowerValue;
		}
		else if (value > m_upperValue) {
			m_currentValue = m_lowerValue;
		}
		else {
			m_currentValue = value;
		}
		//Send SCROLL EVENT
		SendScrollEvent();
	}


	void ISlider::ProcessScrollEvent() {

	}


	void ISlider::SendScrollEvent() {
		static ScrollStructure scrollStruct;
		//Process system
		ProcessScrollEvent();
		//User callback
		scrollStruct = { m_lowerValue, m_upperValue, m_currentValue, this };
		this->CallUserCallback(this, Widget::CustomEvents::ON_SCROLL, &scrollStruct);
	}

}