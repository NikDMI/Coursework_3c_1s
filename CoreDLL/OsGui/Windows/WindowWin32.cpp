#define DLL_EXPORT_SIGNATURE

#include "WindowWin32.h"
#include "../../Bean/Exception.h"
#include "../../Painter/Windows/PainterD2D.h"
#include "../../Core/EventManager/IEventManager.h"
#include "../../Application/NkApplication.h"
#include "../../Gui/Widget.h"
#include "../../Gui/EventStructures/MouseStructure.h"
#include "../../Gui/EventStructures/KeyBoard.h"
#include "../../Gui/EventStructures/ResizeWindowStructure.h"
#include <windowsx.h>
#include <Dwmapi.h>

#pragma comment(lib, "Dwmapi.lib")

namespace Nk {

	using namespace std;

	map<HWND, WindowWin32*> WindowWin32::m_windowsDictionary;
	bool WindowWin32::m_isRegisterWindowClass = false;


	LRESULT CALLBACK Win32WndProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam);


	WindowWin32::WindowWin32(Widget* widget, WindowType windowType, PainterType painterType, IWindow* parent):
		IWindow{ widget, windowType, painterType, parent }
	{
		if (!NkApplication::IsGuiThread()) {
			throw Exception{"Try to create window in not gui thread"};
		}
		if (widget == nullptr) {
			//throw Exception{"Can't create window without widghet object"};
		}
		if (parent != nullptr && !dynamic_cast<WindowWin32*>(parent)) {
			throw Exception{ "Invalid parent class" };
		}
		m_parentWindow = (WindowWin32*)(parent);
		//Create phisical window
		RegisterWindowClass();
		DWORD windowStyle = WS_OVERLAPPED;
		if (parent == nullptr) {
			windowStyle = WS_POPUP;
		}
		else {
			windowStyle |= WS_CHILD;
		}
		m_hWnd = CreateWindow(CLASS_NAME.c_str(), L"Window", windowStyle, m_x, m_y, m_width, m_height,
			(parent == nullptr) ? NULL : m_parentWindow->GetHwnd(), NULL, GetModuleHandle(NULL), 0);
		if (m_hWnd == NULL) {
			throw Exception{ "Can't create window" };
		}
		::ShowWindow(m_hWnd, SW_HIDE);
		//Add window to dictionaty
		m_windowsDictionary.insert({ m_hWnd, this });
		//Choose painter
		PainterD2D* parentPainter;
		switch (painterType) {
		case PainterType::DirectX:
			parentPainter = (m_parentWindow == nullptr) ? nullptr : dynamic_cast<PainterD2D*>(m_parentWindow->m_windowPainter);
			m_windowPainter = new PainterD2D{m_hWnd, parentPainter};
			break;
		default:
			throw Exception{"Invalid window painter"};
		}
	}


	WindowWin32::~WindowWin32() {
		delete m_windowPainter;
	}


	HWND WindowWin32::GetHwnd() const {
		return m_hWnd;
	}


	IPainter* WindowWin32::GetPainter() {
		return m_windowPainter;
	}


	Rect_t WindowWin32::GetRootClientRect() {
		if (m_parentWindow == nullptr) {
			return { 0, 0, m_width, m_height };
		}
		Rect_t parentRect = m_parentWindow->GetRootClientRect();
		parentRect.x += m_x;
		parentRect.y += m_y;
		parentRect.w = parentRect.x + m_width;
		parentRect.h = parentRect.y + m_height;
		return parentRect;
	}


	Rect_t WindowWin32::GetParentClipRect(Rect_t& rootClipRect) {
		if (m_parentWindow == nullptr) return rootClipRect;
		if (m_parentWindow->m_width < m_x || m_parentWindow->m_height < m_y) {
			return { 0, 0, 0, 0 };
		}
		Rect_t clipRect = {0, 0, m_width, m_height};
		if (m_x < 0) {
			clipRect.x -= m_x;
			clipRect.w += m_x;
			rootClipRect.x -= m_x;
		}
		if (m_y < 0) {
			clipRect.y -= m_y;
			clipRect.h += m_y;
			rootClipRect.y -= m_y;
		}
		if (m_parentWindow->m_width < m_width) {
			clipRect.w = m_parentWindow->m_width;
		}
		if (m_parentWindow->m_height < m_height) {
			clipRect.h = m_parentWindow->m_height;
		}
		rootClipRect.w = clipRect.w;
		rootClipRect.h = clipRect.h;
		return clipRect;
	}


	void WindowWin32::SetWindowGeometry(Coord_t x, Coord_t y, Coord_t w, Coord_t h, Point_t originPoint) {
		if (w < 0 || h < 0) {
			throw Exception{"Bad window geometry"};
		}
		m_x = x; m_y = y; m_width = w; m_height = h;
		MoveWindow(m_hWnd, originPoint.x + x, originPoint.y + y, w, h, FALSE);
		m_windowPainter->Resize(w, h);
	}


	void WindowWin32::ShowWindow() {
		if (!m_isVisible) {
			::ShowWindow(m_hWnd, SW_SHOW);
			m_isVisible = true;
		}
	}


	void WindowWin32::HideWindow() {
		if (m_isVisible) {
			::ShowWindow(m_hWnd, SW_HIDE);
			m_isVisible = false;
		}
	}


	void WindowWin32::RefreshWindow() {
		SendMessage(m_hWnd, WM_PAINT, 0, 0);
		//InvalidateRect(m_hWnd, NULL, FALSE);
	}


	void WindowWin32::DrawWindow() {
		if (m_windowPainter->IsValidBackBuffer()) {
			auto clientRect = this->GetRootClientRect();
			//auto bmpRect = this->GetParentClipRect(clientRect);
			//m_windowPainter->DrawBufferBitmap(clientRect, bmpRect);
			m_windowPainter->DrawBufferBitmap({ m_x, m_y, m_width, m_height });
		}
		else {
			m_windowPainter->CreateBuffer();
			//NkApplication::GetEventManager()->PushEvent(m_correspondingWidget, 
			//	m_correspondingWidget->GetEventIndex(Widget::Events::ON_DRAW), m_correspondingWidget);
			m_correspondingWidget->SendRepaintEvent();
		}
	}


	void WindowWin32::BeginDrawWindowBuffer() {
		auto clientRect = this->GetRootClientRect();
		auto bmpRect = this->GetParentClipRect(clientRect);
		m_windowPainter->BeginDraw({m_x, m_y, m_width, m_height});
	}

	
	void WindowWin32::EndDrawWindowBuffer() {
		m_windowPainter->EndDraw();
	}


	void WindowWin32::SetMouseCapture() {
		SetCapture(m_hWnd);
	}


	void WindowWin32::ReleaseMouseCapture() {
		ReleaseCapture();
	}

	void WindowWin32::SetFocus() {
		::SetFocus(m_hWnd);
	}


	void WindowWin32::RegisterWindowClass() {
		if (!m_isRegisterWindowClass) {
			static WNDCLASSEX wc;
			wc.cbSize = sizeof(wc);
			wc.lpfnWndProc = Win32WndProc;
			wc.hInstance = GetModuleHandle(NULL);
			wc.hCursor = NULL;
			wc.lpszClassName = CLASS_NAME.c_str();
			wc.hIcon = LoadIcon(NULL, IDI_APPLICATION);
			wc.hIconSm = LoadIcon(NULL, IDI_APPLICATION);
			wc.lpszMenuName = NULL;
			wc.hbrBackground = (HBRUSH)GetStockObject(WHITE_BRUSH);
			wc.style = CS_HREDRAW | CS_VREDRAW;
			RegisterClassEx(&wc);
			m_isRegisterWindowClass = true;

			
		}
	}

	void TranslateVirtualKey(KeyboardStructure& keyboardstructure, WPARAM virtualKey);
	LRESULT GetWindowBorderCode(DWORD xPos, DWORD yPos, HWND hWnd);

	/*
	* Main window proc, that dispatch events to corresponding objects
	*/
	LRESULT CALLBACK Win32WndProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam) {
		static HWND lastHwnd = 0;
		static Widget* lastWidget = nullptr;
		static WindowWin32* lastWindow = nullptr;
		static IEventManager* eventManager = NkApplication::GetEventManager();
		//static bool isWindowCreated = false;
		if (lastHwnd != hWnd || lastWidget == nullptr) {
			lastHwnd = hWnd;
			auto nkWidget = WindowWin32::m_windowsDictionary.find(hWnd);
			if (nkWidget != WindowWin32::m_windowsDictionary.end()) {
				lastWindow = (*nkWidget).second;
				lastWidget = lastWindow->m_correspondingWidget;
			}
		}

		//Local vars
		static MouseStructure mouseStructure;
		static KeyboardStructure keyboardStructure;
		static BasicWidgetStructure basicStructure;
		static ResizeWindowStructure resizeWindowStructure;
		static bool isResizing = false;
		RECT windowRect;
		static RECT border_thickness;


		switch (uMsg) {

			
		case WM_CREATE:
		{
			/*
			//find border thickness
			SetRectEmpty(&border_thickness);
			if (GetWindowLongPtr(hWnd, GWL_STYLE) & WS_THICKFRAME)
			{
				AdjustWindowRectEx(&border_thickness, GetWindowLongPtr(hWnd, GWL_STYLE) & ~WS_CAPTION, FALSE, NULL);
				border_thickness.left *= -1;
				border_thickness.top *= -1;
			}
			else if (GetWindowLongPtr(hWnd, GWL_STYLE) & WS_BORDER)
			{
				SetRect(&border_thickness, 1, 1, 1, 1);
			}

			MARGINS margins = { 10, 10, 10, 10 };
			DwmExtendFrameIntoClientArea(hWnd, &margins);
			SetWindowPos(hWnd, NULL, 0, 0, 0, 0, SWP_NOMOVE | SWP_NOSIZE | SWP_FRAMECHANGED);
			*/
		}
			break;

		case WM_ENTERSIZEMOVE:
			isResizing = true;
			break;

		case WM_EXITSIZEMOVE:
			isResizing = false;
			GetWindowRect(hWnd, &windowRect);
			resizeWindowStructure = { windowRect.left, windowRect.top, windowRect.right - windowRect.left, windowRect.bottom - windowRect.top, lastWidget };
			eventManager->PushEvent(lastWidget, lastWidget->GetEventIndex(Widget::Events::ON_WINDOW_RESIZE), &resizeWindowStructure);
			break;

		case WM_WINDOWPOSCHANGED:
			break;
			
			
			/*
		case WM_NCACTIVATE:
			return 0;

		case WM_NCCALCSIZE:
			if (wParam)
			{
				NCCALCSIZE_PARAMS* sz = (NCCALCSIZE_PARAMS*)lParam;
				sz->rgrc[0].left += 10;
				sz->rgrc[0].top += 10;
				if (sz->rgrc[0].left <= sz->rgrc[0].right - 10) sz->rgrc[0].right -= 10;
				if (sz->rgrc[0].top <= sz->rgrc[0].bottom - 10) sz->rgrc[0].bottom -= 10;
				return 0;
			}
			break;
			*/


		case WM_MOUSEMOVE:
			if (!lastWindow->m_isMouseEnter) {
				static MouseStructure enterMouseStructure;
				enterMouseStructure = { LOWORD(lParam), HIWORD(lParam), lastWidget };
				eventManager->PushEvent(lastWidget, lastWidget->GetEventIndex(Widget::Events::ON_MOUSE_ENTER), &enterMouseStructure);
				//Set track mouse event
				TRACKMOUSEEVENT trackMouseEvent{sizeof(TRACKMOUSEEVENT)};
				trackMouseEvent.dwFlags = TME_LEAVE;
				trackMouseEvent.hwndTrack = hWnd;
				TrackMouseEvent(&trackMouseEvent);
				lastWindow->m_isMouseEnter = true;
			}
			mouseStructure = { LOWORD(lParam), HIWORD(lParam), lastWidget};
			eventManager->PushEvent(lastWidget, lastWidget->GetEventIndex(Widget::Events::ON_MOUSE_MOVE), &mouseStructure);
			//Get address of the static var (it's save, because my gui loop garantee, that after pushing event it wiil be processed)
			break;


		case WM_MOUSELEAVE:
			if (lastWindow->m_isMouseEnter) {
				static MouseStructure leaveMouseStructure;
				lastWindow->m_isMouseEnter = false;
				leaveMouseStructure = { LOWORD(lParam), HIWORD(lParam), lastWidget };
				eventManager->PushEvent(lastWidget, lastWidget->GetEventIndex(Widget::Events::ON_MOUSE_LEAVE), &leaveMouseStructure);
			}
			break;

		case WM_LBUTTONDOWN:
			mouseStructure = { LOWORD(lParam), HIWORD(lParam), lastWidget };
			eventManager->PushEvent(lastWidget, lastWidget->GetEventIndex(Widget::Events::ON_MOUSE_LDOWN), &mouseStructure);
			break;

		case WM_LBUTTONUP:
			mouseStructure = { LOWORD(lParam), HIWORD(lParam), lastWidget };
			eventManager->PushEvent(lastWidget, lastWidget->GetEventIndex(Widget::Events::ON_MOUSE_LUP), &mouseStructure);
			break;

		case WM_PAINT:
			if (!isResizing) {
				eventManager->PushEvent(lastWidget, lastWidget->GetEventIndex(Widget::Events::ON_REPAINT), lastWidget);
			}
			else {
				GetWindowRect(hWnd, &windowRect);
				lastWidget->SetWindowGeometry(windowRect.left, windowRect.top, windowRect.right - windowRect.left, windowRect.bottom - windowRect.top);
				lastWidget->Repaint();
			}
			ValidateRect(hWnd, NULL);
			break;

		case WM_SETFOCUS:
			if (lastWidget != nullptr) {
				basicStructure.sender = lastWidget;
				eventManager->PushEvent(lastWidget, lastWidget->GetEventIndex(Widget::Events::ON_GET_FOCUS), &basicStructure);
			}
			break;

		case WM_KILLFOCUS:
			basicStructure.sender = lastWidget;
			eventManager->PushEvent(lastWidget, lastWidget->GetEventIndex(Widget::Events::ON_LEAVE_FOCUS), &basicStructure);
			break;

		case WM_CHAR:
			keyboardStructure = { (wchar_t)wParam, KeyboardStructure::SystemKey::NONE_KEY, lastWidget };
			if (wParam < 32) {//All system keys in ASCII
				keyboardStructure.systemKey = KeyboardStructure::SystemKey::SYSTEM_KEY;
			}
			eventManager->PushEvent(lastWidget, lastWidget->GetEventIndex(Widget::Events::ON_CHAR), &keyboardStructure);
			break;

		case WM_KEYDOWN:
			keyboardStructure = { (wchar_t)wParam, KeyboardStructure::SystemKey::NONE_KEY, lastWidget };
			TranslateVirtualKey(keyboardStructure, wParam);
			eventManager->PushEvent(lastWidget, lastWidget->GetEventIndex(Widget::Events::ON_KEY_DOWN), &keyboardStructure);
			break;

		case WM_KEYUP:
			keyboardStructure = { (wchar_t)wParam, KeyboardStructure::SystemKey::NONE_KEY, lastWidget };
			TranslateVirtualKey(keyboardStructure, wParam);
			eventManager->PushEvent(lastWidget, lastWidget->GetEventIndex(Widget::Events::ON_KEY_UP), &keyboardStructure);
			break;

		case WM_ERASEBKGND:
			break;


		case WM_NCHITTEST://Set virtual border
			
			if (lastWidget->GetResizingSystemMode()) {	// can be resized by system
				DWORD xPos = GET_X_LPARAM(lParam);
				DWORD yPos = GET_Y_LPARAM(lParam);
				return GetWindowBorderCode(xPos, yPos, hWnd);
			}
			else {
				return DefWindowProc(hWnd, uMsg, wParam, lParam);
			}

		default:
			return DefWindowProc(hWnd, uMsg, wParam, lParam);
		}
		return 0;
	}


	void TranslateVirtualKey(KeyboardStructure& keyboardstructure, WPARAM virtualKey) {
		switch (virtualKey) {
		case VK_BACK:
			keyboardstructure.systemKey = KeyboardStructure::SystemKey::BACK_KEY;
			break;
		case VK_RETURN:
			keyboardstructure.systemKey = KeyboardStructure::SystemKey::ENTER_KEY;
			break;
		default:
			keyboardstructure.systemKey = KeyboardStructure::SystemKey::NONE_KEY;
		}
	}


	LRESULT GetWindowBorderCode(DWORD xPos, DWORD yPos, HWND hWnd) {
		static const int BORDER_HALF_WIDTH = 5;
		RECT windowRect;
		POINT cursorPoint = { xPos, yPos };
		GetWindowRect(hWnd, &windowRect);
		RECT currentRect = windowRect;
		DWORD rectW = windowRect.right - windowRect.left;
		DWORD rectH = windowRect.bottom - windowRect.top;
		//up rect
		currentRect.left += BORDER_HALF_WIDTH;
		currentRect.right -= BORDER_HALF_WIDTH;
		currentRect.top -= BORDER_HALF_WIDTH;
		currentRect.bottom = currentRect.top + 2 * BORDER_HALF_WIDTH;
		if (PtInRect(&currentRect, cursorPoint)) {
			return HTTOP;
		}
		//Bottom
		OffsetRect(&currentRect, 0, rectH);
		if (PtInRect(&currentRect, cursorPoint)) {
			return HTBOTTOM;
		}
		//left
		currentRect = windowRect;
		currentRect.left -= BORDER_HALF_WIDTH;
		currentRect.right = currentRect.left + 2 * BORDER_HALF_WIDTH;
		currentRect.top += BORDER_HALF_WIDTH;
		currentRect.bottom -= BORDER_HALF_WIDTH;
		if (PtInRect(&currentRect, cursorPoint)) {
			return HTLEFT;
		}
		//Right
		OffsetRect(&currentRect, rectW, 0);
		if (PtInRect(&currentRect, cursorPoint)) {
			return HTRIGHT;
		}
		//left - top
		currentRect = windowRect;
		currentRect.left -= BORDER_HALF_WIDTH;
		currentRect.right = currentRect.left + 2 * BORDER_HALF_WIDTH;
		currentRect.top -= BORDER_HALF_WIDTH;
		currentRect.bottom -= currentRect.top + 2 * BORDER_HALF_WIDTH;
		if (PtInRect(&currentRect, cursorPoint)) {
			return HTTOPLEFT;
		}
		//right-top
		OffsetRect(&currentRect, rectW, 0);
		if (PtInRect(&currentRect, cursorPoint)) {
			return HTTOPRIGHT;
		}
		//right-btn
		OffsetRect(&currentRect, 0, rectH);
		if (PtInRect(&currentRect, cursorPoint)) {
			return HTBOTTOMRIGHT;
		}
		//ltft-btn
		OffsetRect(&currentRect, 0 - rectW, 0);
		if (PtInRect(&currentRect, cursorPoint)) {
			return HTBOTTOMLEFT;
		}
		if (PtInRect(&windowRect, cursorPoint)) {
			return HTCLIENT;
		}
		return HTNOWHERE;
	}
}