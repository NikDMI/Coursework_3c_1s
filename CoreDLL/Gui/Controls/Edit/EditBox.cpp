#include "EditBox.h"
#include "../../EventStructures/MouseStructure.h"
#include "../../EventStructures/KeyBoard.h"

namespace Nk {

	void PROC_CALL EditBox_OnLMouseLeave(void* params);
	void PROC_CALL EditBox_OnMouseMove(void* params);
	void PROC_CALL EditBox_OnMouseLDown(void* params);
	void PROC_CALL EditBox_OnLMouseLUp(void* params);
	void PROC_CALL EditBox_OnChar(void* params);
	void PROC_CALL EditBox_OnKeyDown(void* params);
	void Core_EditBoxDrawProc(Widget* widget, IPainter* painter);


	EditBox::EditBox(const std::wstring& editText, Widget* parent) :IEdit{ parent } {
		this->SetText(editText);
		this->GetElementFont()->SetMultilineState(true);
		this->SetWindowDrawProc(Core_EditBoxDrawProc);
		//Events
		SetCustomEvent(CustomEvents::ON_MOUSE_MOVE, EditBox_OnMouseMove);
		SetCustomEvent(CustomEvents::ON_MOUSE_LDOWN, EditBox_OnMouseLDown);
		SetCustomEvent(CustomEvents::ON_MOUSE_LUP, EditBox_OnLMouseLUp);
		SetCustomEvent(CustomEvents::ON_MOUSE_LEAVE, EditBox_OnLMouseLeave);
		SetCustomEvent(CustomEvents::ON_CHAR, EditBox_OnChar);
		SetCustomEvent(CustomEvents::ON_KEY_DOWN, EditBox_OnKeyDown);
	}


	EditBox::~EditBox() {

	}


	void PROC_CALL EditBox_OnMouseMove(void* params) {
		MouseStructure* mouseStructure = (MouseStructure*)params;
		EditBox* edit = (EditBox*)mouseStructure->sender;
		edit->Edit_OnMouseMove(params);
	}

	void PROC_CALL EditBox_OnMouseLDown(void* params) {
		MouseStructure* mouseStructure = (MouseStructure*)params;
		EditBox* edit = (EditBox*)mouseStructure->sender;
		edit->Edit_OnMouseLDown(params);
	}


	void PROC_CALL EditBox_OnLMouseLUp(void* params) {
		MouseStructure* mouseStructure = (MouseStructure*)params;
		EditBox* edit = (EditBox*)mouseStructure->sender;
		edit->Edit_OnLMouseLUp(params);
	}


	void PROC_CALL EditBox_OnLMouseLeave(void* params) {
		MouseStructure* mouseStructure = (MouseStructure*)params;
		EditBox* edit = (EditBox*)mouseStructure->sender;
		edit->Edit_OnLMouseLeave(params);
	}


	void PROC_CALL EditBox_OnChar(void* params) {
		KeyboardStructure* keyboardStructure = (KeyboardStructure*)params;
		EditBox* edit = (EditBox*)keyboardStructure->sender;
		if (keyboardStructure->systemKey == KeyboardStructure::SystemKey::NONE_KEY) {
			edit->GetTextReference().push_back(keyboardStructure->charValue);
			edit->Repaint();
		}
	}


	void PROC_CALL EditBox_OnKeyDown(void* params) {
		KeyboardStructure* keyboardStructure = (KeyboardStructure*)params;
		EditBox* edit = (EditBox*)keyboardStructure->sender;
		if (keyboardStructure->systemKey == KeyboardStructure::SystemKey::BACK_KEY) {
			std::wstring& text = edit->GetTextReference();
			if (text.size()) {
				text.pop_back();
			}
			edit->Repaint();
		}
		else if (keyboardStructure->systemKey == KeyboardStructure::SystemKey::ENTER_KEY) {
			edit->GetTextReference().push_back(L'\n');
		}
	}

	void Core_EditBoxDrawProc(Widget* widget, IPainter* painter) {
		EditBox* edit = (EditBox*)widget;
		painter->DrawText(edit->GetWidgetClientRect(), edit->m_elementText);
	}
}