#include "IEdit.h"
#include "../../EventStructures/MouseStructure.h"

namespace Nk {

	const Color_t IEdit::DEFAULT_COLOR = { 0.5, 0.6, 0.3, 1.0 };
	const Color_t IEdit::DEFAULT_HOVER_COLOR = { 0.6, 0.7, 0.3, 1.0 };
	const Color_t IEdit::DEFAULT_PUSH_COLOR = { 0.3, 0.4, 0.6, 1.0 };

	IEdit::IEdit(Widget* parent) :Widget{ parent }, ITextElement{ this }, IBorderElement{ this }, IColorElement{ this }{
		SetBackgroundColor(DEFAULT_COLOR);
		//Configs
		SetButtonColor(IColorElement::ElementState::HOVER, DEFAULT_HOVER_COLOR);
		SetButtonColor(IColorElement::ElementState::PUSH, DEFAULT_PUSH_COLOR);
		SetButtonColor(IColorElement::ElementState::STATIC, DEFAULT_COLOR);
	}


	void PROC_CALL IEdit::Edit_OnMouseMove(void* params) {
		MouseStructure* mouseStructure = (MouseStructure*)params;
		IEdit* edit = (IEdit*)mouseStructure->sender;
		edit->ColorElement_OnMouseMove(params);
	}

	void PROC_CALL IEdit::Edit_OnMouseLDown(void* params) {
		MouseStructure* mouseStructure = (MouseStructure*)params;
		IEdit* edit = (IEdit*)mouseStructure->sender;
		edit->ColorElement_OnMouseLDown(params);
	}


	void PROC_CALL IEdit::Edit_OnLMouseLUp(void* params) {
		MouseStructure* mouseStructure = (MouseStructure*)params;
		IEdit* edit = (IEdit*)mouseStructure->sender;
		edit->ColorElement_OnLMouseLUp(params);
		edit->SetFocus();
	}


	void PROC_CALL IEdit::Edit_OnLMouseLeave(void* params) {
		MouseStructure* mouseStructure = (MouseStructure*)params;
		IEdit* edit = (IEdit*)mouseStructure->sender;
		edit->ColorElement_OnLMouseLeave(params);
	}
}