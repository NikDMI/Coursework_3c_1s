#include <Windows.h>

#undef CreateWindow

#include "../CoreDLL/Application/NkApplication.h"
#include "../CoreDLL/OsGui/WindowFactory.h"
#include "../CoreDLL/OsGui/IWindow.h"
#include "../CoreDLL/Gui/EventStructures/MouseStructure.h"
#include "../CoreDLL/OsGui/Windows/WindowWin32.h"
#include "../CoreDLL/Gui/Controls/Label/Label.h"
#include "../CoreDLL/Tools/Bitmap/IBitmap.h"
#include "../CoreDLL/Tools/Cursor/ICursor.h"
#include "../CoreDLL/Gui/Controls/Caption/MainCaption.h"
#include "../CoreDLL/Painter/Brush/IBrush.h"
#include "../CoreDLL/Tools/IGeometry/IGeometry.h"
#include "../CoreDLL/Gui/Border/IBorder.h"
#include "../CoreDLL/Gui/Border/AngleBorder.h"
#include "../CoreDLL/Gui/Controls/Window/MainWindow.h"
#include "../CoreDLL/Gui/ResizeManager/RectangleResizer.h"
#include "../CoreDLL/Gui/Controls/Button/PushButton.h"
#include "../CoreDLL/Gui/Controls/Button/ImageButton.h"
#include "../CoreDLL/Gui/Controls/Edit/EditBox.h"
#include "../CoreDLL/Gui/Controls/Window/PanelWindow.h"
#include "../CoreDLL/Gui/Controls/Slider/ScrollBar.h"
#include "../CoreDLL/Gui/Controls/List/IList.h"
#include "../CoreDLL/Gui/EventStructures/ScrollStructure.h"
#include "../CoreDLL/Gui/Layout/StackLayout.h"

#include "ICustomLayout.h"
#include "MainControlPanel.h"
#include "Controls/Form.h"
#include "GuiConfigs.h"

#include <string>

#pragma comment(lib,"CoreDLL.lib")

using namespace Nk;
using namespace std;

void CreateGuiWindows();

ProgramWindows gl_allWindows;

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE, LPSTR lpCmdLine, int nCmdShow) {
	Nk::NkApplication* app = new Nk::NkApplication{};
	CreateGuiWindows();
	app->StartLoop();
	return 0;
}


const Color_t MAIN_WINDOW_COLOR = { 0.4, 0.4 , 0.4, 1.0 };
const Color_t MAIN_MENU_COLOR = { 0.5, 0.5 , 0.5, 1.0 };
const Coord_t MAIN_MENU_PANEL_H = 40;
const Coord_t MAIN_PANEL_BOTTOM_OFFSET = 30;
const wstring DEFAULT_FONT_FAMILY = L"Times New Roman";


/*
* Creates all windows in this app
*/
void CreateGuiWindows() {
	auto mainWindow = new MainWindow(L"Nk Forms", 100, 100, 1300, 600);
	mainWindow->SetBackgroundColor(MAIN_WINDOW_COLOR);
	mainWindow->AddNewLayout(new CustomLayout());
	//Menu panel
	auto mainMenuPanel = new PanelWindow(mainWindow, 0, 0, 0, MAIN_MENU_PANEL_H);
	mainMenuPanel->SetBackgroundColor(MAIN_MENU_COLOR);
	//Content panel
	auto mainContentPanel = new MainControlPanel(mainWindow);
	mainWindow->ShowWindow();
}
