#include "RectangleResizer.h"
#include "../../Bean/Exception.h"

namespace Nk {

	RectangleResizer::RectangleResizer(Coord_t dLeft, Coord_t dRight, Coord_t dUp, Coord_t dDown) : m_dDown{ dDown }, m_dLeft{dLeft},
		m_dRight{ dRight }, m_dUp{dUp} 
	{
		if (dLeft < 0 || dRight < 0 || dUp < 0 || dDown < 0) {
			throw Exception{"Bad resize boundaries"};
		}
	}


	IResizeManager::ResizeType RectangleResizer::GetResizeType(Rect_t clientRect, Point_t cursorPosition) {
		bool isUp = false, isDown = false, isLeft = false, isRight = false;
		//Find dimensions
		if (cursorPosition.x < m_dLeft) {
			isLeft = true;
		}
		if (clientRect.w - cursorPosition.x < m_dRight) {
			isRight = true;
		}
		if (cursorPosition.y < m_dUp) {
			isUp = true;
		}
		if (clientRect.h - cursorPosition.y < m_dDown) {
			isDown = true;
		}
		if (isLeft) {
			if (isUp) return IResizeManager::ResizeType::UP_LEFT;
			if (isDown) return IResizeManager::ResizeType::DOWN_LEFT;
			return IResizeManager::ResizeType::LEFT;
		}
		if (isRight) {
			if (isUp) return IResizeManager::ResizeType::UP_RIGHT;
			if (isDown) return IResizeManager::ResizeType::DOWN_RIGHT;
			return IResizeManager::ResizeType::RIGHT;
		}
		if (isUp) return IResizeManager::ResizeType::UP;
		if (isDown) return IResizeManager::ResizeType::DOWN;
		return IResizeManager::ResizeType::NONE;
	}

}