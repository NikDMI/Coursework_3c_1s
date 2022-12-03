#define DLL_EXPORT_SIGNATURE

#include "WindowWin32.h"
#include "../../Bean/Exception.h"
#include "../../Painter/Windows/PainterD2D.h"
#include "../../Core/EventManager/IEventManager.h"
#include "../../Application/NkApplication.h"
#include "../../Gui/Widget.h"
#include "../../Gui/EventStructures/MouseStructure.h"

namespace Nk {

	using namespace std;

	map<HWND, Widget*> WindowWin32::m_windowsDictionary;
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
		m_windowsDictionary.insert({ m_hWnd, widget });
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


	void WindowWin32::SetWindowGeometry(Coord_t x, Coord_t y, Coord_t w, Coord_t h) {
		if (w < 0 || h < 0) {
			throw Exception{"Bad window geometry"};
		}
		m_x = x; m_y = y; m_width = w; m_height = h;
		MoveWindow(m_hWnd, x, y, w, h, FALSE);
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
			auto bmpRect = this->GetParentClipRect(clientRect);
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
		//auto parentOffset = this->GetRootOffset();
		//m_windowPainter->BeginDraw(clientRect, bmpRect);
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


	void WindowWin32::RegisterWindowClass() {
		if (!m_isRegisterWindowClass) {
			static WNDCLASSEX wc;
			wc.cbSize = sizeof(wc);
			wc.lpfnWndProc = Win32WndProc;
			wc.hInstance = GetModuleHandle(NULL);
			wc.hCursor = LoadCursor(NULL, IDC_ARROW);
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


	/*
	* Main window proc, that dispatch events to corresponding objects
	*/
	LRESULT CALLBACK Win32WndProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam) {
		static HWND lastHwnd = 0;
		static Widget* lastWidget = nullptr;
		static IEventManager* eventManager = NkApplication::GetEventManager();
		if (lastHwnd != hWnd || lastWidget == nullptr) {
			lastHwnd = hWnd;
			auto nkWidget = WindowWin32::m_windowsDictionary.find(hWnd);
			if (nkWidget != WindowWin32::m_windowsDictionary.end()) {
				lastWidget = (*nkWidget).second;
			}
		}

		//Local vars
		static MouseStructure mouseStructure;

		switch (uMsg) {

		case WM_MOUSEMOVE:
			mouseStructure = { LOWORD(lParam), HIWORD(lParam), lastWidget };
			eventManager->PushEvent(lastWidget, lastWidget->GetEventIndex(Widget::Events::ON_MOUSE_MOVE), &mouseStructure);
			//Get address of the static var (it's save, because my gui loop garantee, that after pushing event it wiil be processed)
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
			eventManager->PushEvent(lastWidget, lastWidget->GetEventIndex(Widget::Events::ON_REPAINT), lastWidget);
			ValidateRect(hWnd, NULL);
			break;

		case WM_ERASEBKGND:
			break;

		default:
			return DefWindowProc(hWnd, uMsg, wParam, lParam);
		}
		return 0;
	}
}