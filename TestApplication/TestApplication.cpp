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


#include <string>

#pragma comment(lib,"CoreDLL.lib")

void PROC_CALL OnMouseMove2(void* params);

using namespace Nk;

void PROC_CALL OnMouseMove(void* params);
void PROC_CALL OnMouseClick(void* params);
void PROC_CALL OnScroll(void* params);

DWORD WINAPI LogicThread(LPVOID);

Widget* widget = nullptr;
Widget* widget3 = nullptr;
Label* lbl;
IBitmap* bmp;
MainCaption* caption;
IBorder* topBorder;
PushButton* btn;
IList* list;

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE, LPSTR lpCmdLine, int nCmdShow) {
	Nk::NkApplication* app = new Nk::NkApplication{};
	
	widget = new Widget{ nullptr, {0.8, 0.8, 0.8, 0.1} };
	widget->SetWindowGeometry(300, 300, 1000, 500);
	//widget->ShowWindow();
	Widget* widget2 = new Widget(widget, {0.3, 0.4, 0.5, 0.1});
	widget2->SetWindowGeometry(10, 10, 200, 200);
	widget3 = new Widget(widget2, { 0.5, 0.9, 0.5, 1.0 });
	widget3->SetWindowGeometry(-50, -50, 100, 100);
	widget->SetCustomEvent(Widget::CustomEvents::ON_MOUSE_MOVE, OnMouseMove);
	lbl = new Label(L"Test text", widget);
	lbl->GetElementFont()->SetSizeInPixels(30)->SetHorizontalAlignment(IFont::HorizontalAlignment::CENTER)->
		SetVerticalAlignment(IFont::VerticalAlignment::CENTER);
	lbl->SetBackgroundColor({ 0.7, 0.4, 0.6, 0.5 });
	lbl->SetWindowGeometry(200, 200, 300, 300);
	bmp = IBitmap::LoadBitmap(L"Picture.png");
	lbl->ShowWindow();
	lbl->SetCustomEvent(Widget::CustomEvents::ON_MOUSE_MOVE, OnMouseMove2);
	caption = new MainCaption{widget, L"Hello"};
	caption->SetBackgroundColor({ 0.5, 0.3, 0.6, 1.0 });
	caption->GetElementFont()->SetSizeInPixels(20);
	caption->ShowWindow();
	widget->SetHeaderWidget(caption);
	topBorder = new AngleBorder(widget, IBorder::BorderType::TOP, 5, 40);
	IBorder* rightBorder = new AngleBorder(widget, IBorder::BorderType::RIGHT, 5, 40);
	//IBorder* btmBorder = new AngleBorder(widget, IBorder::BorderType::BOTTOM, 5, 110);
	//IBorder* leftBorder = new AngleBorder(widget, IBorder::BorderType::LEFT, 5, 110);
	widget->ShowWindow();
	
	MainWindow* mainWindow = new MainWindow(L"Hello, window", 100, 100, 600, 800);
	mainWindow->ShowWindow();

	ICursor* cursorCross = ICursor::CreateCursor();
	cursorCross->SetSystemCursor(ICursor::SystemCursor::CROSS);
	mainWindow->SetCursor(cursorCross);
	mainWindow->SetResizeManager(new RectangleResizer{15,15,15,15});
	
	btn = new PushButton{ L"Hello", mainWindow};
	btn->SetWindowGeometry(30, 30, 100, 50);
	btn->SetNormalBorder({ 0.5, 0.5, 0.6, 1.0 });
	btn->ShowWindow();
	mainWindow->SetCustomEvent(Widget::CustomEvents::ON_MOUSE_LUP, OnMouseClick);
	ImageButton* imgBtn = new ImageButton{IBitmap::GetSystemBitmap(IBitmap::SystemBitmaps::CLOSE_IMAGE), mainWindow};
	imgBtn->SetWindowGeometry(100, 100, 100, 50);
	imgBtn->SetNormalBorder({ 0.2, 0.5, 0.8, 1.0 });
	imgBtn->ShowWindow();
	
	EditBox* editBox = new EditBox { L"Hello", mainWindow };
	editBox->SetWindowGeometry(100, 200, 200, 300);
	editBox->SetNormalBorder({ 0.8, 0.3, 0.4, 1.0 });
	editBox->ShowWindow();
	PanelWindow* panel = new PanelWindow{ editBox, 40,40,100,80 };
	panel->ShowWindow();
	ScrollBar* sc = new ScrollBar{ panel };
	sc->SetWindowGeometry(15, 0, 15, 80);
	sc->ShowWindow();
	sc->SetCustomEvent(Widget::CustomEvents::ON_SCROLL, OnScroll);
	list = new IList{ mainWindow };
	list->SetWindowGeometry(300, 100, 150, 300);
	for (int i = 0; i < 50; ++i) {
		Label* listItem = new Label{ L"Hello" + std::to_wstring(i), list }; listItem->SetWindowGeometry(0, 0, 0, 20+i);
		listItem->SetBackgroundColor({ (float)(0.4 + i*0.02), 0.3, 0.4, 1.0 });
		list->AddListItem(listItem);
	}
	list->RecomputeListLayout();
	list->ShowWindow();
	mainWindow->AddNewLayout(new StackLayout{ StackLayout::StackType::VERTICAL, StackLayout::StackAlignment::ALL_SPACE });
	mainWindow->Repaint();
	CreateThread(NULL, 0, LogicThread, widget3, 0, NULL);
	app->StartLoop();
	return 0;
}

void PROC_CALL UserWindowProc(Widget* widget, IPainter* painter);

void PROC_CALL OnScroll(void* params) {
	ScrollStructure* sc = (ScrollStructure*)params;
	btn->SetText(std::to_wstring(sc->currentValue));
	btn->Repaint();
	list->SetScrolling((float)sc->currentValue / (float)(sc->upperValue - sc->lowerValue));
	list->Repaint();
}

DWORD WINAPI LogicThread(LPVOID param) {
	Widget* w = (Widget*)param;
	Sleep(1000);
	w->SetWindowGeometry(20, 20, 50, 50);
	w->Repaint();
	Sleep(1000);
	w->SetBackgroundColor({ 0.8, 0.2, 0.3, 1.0 });
	w->SetWindowGeometry(20, 20, 100, 100);
	widget->Repaint();
	Sleep(1000);
	w->SetWindowDrawProc(UserWindowProc);
	w->Repaint();
	return 0;
}

int x = 0;
int y = 0;

void PROC_CALL UserWindowProc(Widget* widget, IPainter* painter) {
	static IBrush* bkBrush = painter->CreateBrushObject({ 0.5,0,0,1 });
	static IBrush* contureBrush = painter->CreateBrushObject({ 0.3,0.8,0.9,1 });
	painter->DrawText({ 0,0,100,100 }, L"x = " + std::to_wstring(x)+L" y = "+std::to_wstring(y));
	painter->DrawBitmap(bmp, { 20,20,50,50 });
	painter->SetBackgroundBrush(bkBrush);
	bkBrush->SetColor({ 0.5,0,0,1 });
	painter->FillRectangle({ 60,60,30,30 });
	bkBrush->SetColor({ 0,0.5,0,1 });
	painter->SetContureBrush(contureBrush);
	painter->SetPenWidth(3);
	painter->DrawRectangle({ 80,80,30,30 });
	static IGeometry* geometry = IGeometry::CreateGeometry();
	bkBrush->SetColor({ 0,0.5,0.8,1 });
	geometry->SetPolygone({ {30,30}, {40,40}, {40, 10}, {70,50} });
	painter->DrawGeometry(geometry);
}

void PROC_CALL OnMouseMove(void* params) {
	MouseStructure* ms = (MouseStructure*)params;
	x = ms->xCoord_Px;
	y = ms->yCoord_Px;
	//widget3->SetWindowGeometry(00, 00, ms->xCoord_Px % 200, ms->yCoord_Px % 200);
	widget->SetWindowGeometry(300, 300, ms->xCoord_Px + 100, ms->yCoord_Px + 100);
	lbl->SetText(L"Coords: x = " + std::to_wstring(ms->xCoord_Px) + L" y = " + std::to_wstring(ms->yCoord_Px));
	lbl->Repaint();
	widget3->Repaint();
}


void PROC_CALL OnMouseClick(void* params) {
	MouseStructure* ms = (MouseStructure*)params;
	x = ms->xCoord_Px;
	y = ms->yCoord_Px;
	btn->SetWindowGeometry(x, y, 100, 50);
	btn->Repaint();
}

static Point_t lastGlobalCoord;
bool isCapture = false;

void PROC_CALL OnMouseMove2(void* params) {
	/*
	MouseStructure* ms = (MouseStructure*)params;
	if (!isCapture) {
		isCapture = true;
		lbl->SetMouseCapture();
		lastGlobalCoord = ICursor::GetGlobalMouseCoord();
	}
	else {
		Point_t p = ICursor::GetGlobalMouseCoord();
		Coord_t dx = p.x - lastGlobalCoord.x;
		Coord_t dy = p.y - lastGlobalCoord.y;
		lastGlobalCoord = p;
	}
	*/
}