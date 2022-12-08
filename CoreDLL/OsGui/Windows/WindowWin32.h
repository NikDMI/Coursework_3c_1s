#ifndef WINDOW_OS_WIN32_GUI_DLL
#define WINDOW_OS_WIN32_GUI_DLL

#include "../IWindow.h"
#include "../../Painter/IPainter.h"
#include <Windows.h>
#include <string>
#include <map>

namespace Nk {
	
	class WindowWin32 final: public IWindow {
	public:
		WindowWin32(Widget* widget, WindowType windowType, PainterType painterType, IWindow* parent);

		HWND GetHwnd() const;

		IPainter* GetPainter() override;

		void SetWindowGeometry(Coord_t x, Coord_t y, Coord_t w, Coord_t h) override;
		void ShowWindow() override;
		void HideWindow() override;

		/*
		* ON_REPAINT evetn
		* Draw window in special rect(). If we have saved buffer, than draw him
		* If window draw the first time, then Iwindow send ON_DRAW event to widget (fill window buffer)
		*/
		void DrawWindow() override;

		/*
		* Creates new window buffer, wait commands on IPainter
		*/
		void BeginDrawWindowBuffer() override;

		/*
		* Save window buffer, redraw window
		*/
		void EndDrawWindowBuffer() override;

		void RefreshWindow() override;

		Rect_t GetRootClientRect()  override;

		Rect_t GetParentClipRect(Rect_t& rootClipRect) override;

		~WindowWin32() override;

		void SetMouseCapture() override;

		void ReleaseMouseCapture() override;

	private:
		void RegisterWindowClass();

		///////Config data//////////////
		const std::wstring CLASS_NAME = L"Win32WindowClass";
		static bool m_isRegisterWindowClass;

		///////Window data//////////////
		bool m_isVisible = false;
		bool m_isMouseEnter = false;
		Coord_t m_x = 0;
		Coord_t m_y = 0;
		Coord_t m_width = 0;
		Coord_t m_height = 0;

		IPainter* m_windowPainter;
		WindowWin32* m_parentWindow = nullptr;

		HWND m_hWnd;

		/*
		* Balanced tree of hWnd-Widget* correspondence
		*/
		static std::map<HWND, WindowWin32*> m_windowsDictionary;

		friend LRESULT CALLBACK Win32WndProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam);
	};
}

#endif	//#ifndef WINDOW_OS_WIN32_GUI_DLL

//У каждого окна должен быть свой связанный "художник"
