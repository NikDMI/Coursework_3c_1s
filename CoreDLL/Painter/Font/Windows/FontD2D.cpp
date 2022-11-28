#include "FontD2D.h"

#pragma comment(lib, "dwrite.lib")
#include "../../../Bean/Settings.h"
#include "../../../Bean/Exception.h"

namespace Nk {


	FontD2D::FontD2D(const std::wstring& fontFamily, FLOAT fontSize) :FontD2D{ fontFamily, NULL, DWRITE_FONT_WEIGHT_NORMAL,
	DWRITE_FONT_STYLE_NORMAL, DWRITE_FONT_STRETCH_NORMAL, fontSize, L"" } {

	}

	FontD2D::FontD2D(): FontD2D{ L"Times New Roman", 14} {

	}

	FontD2D::FontD2D(const std::wstring& fontFamily, IDWriteFontCollection* fontCollection, DWRITE_FONT_WEIGHT weight,
		DWRITE_FONT_STYLE fontStyle, DWRITE_FONT_STRETCH fontStretch, FLOAT fontSize, const std::wstring& localName) :
		m_fontFamily{ fontFamily }, m_fontCollection{ fontCollection }, m_fontWeight{ weight },
		m_fontStyle{ fontStyle }, m_fontStretch{ fontStretch }, m_fontSize{ fontSize }, m_fontLocalName{ localName }
	{

		m_pWriteFactory = Settings::GetDWriteFactory();
		CreateNewTextFormat();
	}

	void FontD2D::CreateNewTextFormat() {
		HRESULT hRes = m_pWriteFactory->CreateTextFormat(m_fontFamily.c_str(), m_fontCollection, m_fontWeight, m_fontStyle,
			m_fontStretch, m_fontSize, m_fontLocalName.c_str(), m_textFormat.GetAddressOf());
		if (hRes != S_OK) {
			DWORD errCode = GetLastError();
			throw Exception{"Can't crete text format"};
		}
		m_textFormat->SetWordWrapping(DWRITE_WORD_WRAPPING_EMERGENCY_BREAK); //Breaks big words
		m_textFormat->SetTextAlignment(m_textAlignment);
		m_textFormat->SetParagraphAlignment(m_textVerticalAlignment);
	}


	void FontD2D::SetSizeInPt(int sizeInPt) {
		//m_textFormat->
	}


	void FontD2D::SetSizeInPixels(int sizeInPixels) {
		if (m_fontSize != sizeInPixels) {
			m_fontSize = sizeInPixels;
			m_changedFontStates |= StateFontFlags::fontSize;
		}
	}


	void FontD2D::SetFamily(std::wstring familyName) {
		if (familyName != m_fontFamily) {
			m_fontFamily = familyName;
			m_changedFontStates |= StateFontFlags::fontFamily;
		}
	}


	void FontD2D::SetHorizontalAlignment(HorizontalAlignment alignment) {
		DWRITE_TEXT_ALIGNMENT userAlignment;
		switch (alignment) {
		case HorizontalAlignment::TRAILING:
			userAlignment = DWRITE_TEXT_ALIGNMENT_TRAILING;
			break;

		case HorizontalAlignment::LEADING:
			userAlignment = DWRITE_TEXT_ALIGNMENT_LEADING;
			break;

		case HorizontalAlignment::CENTER:
			userAlignment = DWRITE_TEXT_ALIGNMENT_CENTER;
			break;
		}
		if (m_textAlignment != userAlignment) {
			m_textAlignment = userAlignment;
			m_textFormat->SetTextAlignment(userAlignment);
		}
	}


	void FontD2D::SetVerticalAlignment(VerticalAlignment alignment) {
		DWRITE_PARAGRAPH_ALIGNMENT userVerticalAlignment;
		switch (alignment) {
		case VerticalAlignment::TOP:
			userVerticalAlignment = DWRITE_PARAGRAPH_ALIGNMENT_NEAR;
			break;

		case VerticalAlignment::BOTTOM:
			userVerticalAlignment = DWRITE_PARAGRAPH_ALIGNMENT_FAR;
			break;

		case VerticalAlignment::CENTER:
			userVerticalAlignment = DWRITE_PARAGRAPH_ALIGNMENT_CENTER;
			break;
		}
		if (userVerticalAlignment != m_textVerticalAlignment) {
			m_textVerticalAlignment = userVerticalAlignment;
			m_textFormat->SetParagraphAlignment(userVerticalAlignment);
		}
	}


	void FontD2D::ChangeFontState() {
		if (m_changedFontStates) {
			bool isCriticalChanges = false; //This flag shows, do we need to create new textFormat object
			if (m_changedFontStates & StateFontFlags::fontSize || m_changedFontStates & StateFontFlags::fontFamily) {
				isCriticalChanges = true;
			}

			if (isCriticalChanges)
				CreateNewTextFormat();
			m_changedFontStates = 0;
		}
	}



	ComPtr<IDWriteTextLayout> FontD2D::GetFormattedTextLayout(const std::wstring text, D2D_RECT_F textRect) {
		ChangeFontState(); //Create new textFormat object if user make some changes
		ComPtr<IDWriteTextLayout> textLayout;
		FLOAT maxWidth = textRect.right - textRect.left;
		FLOAT maxHeight = textRect.bottom - textRect.top;
		HRESULT hRes = m_pWriteFactory->CreateTextLayout(text.c_str(), text.size(), m_textFormat.Get(),
			maxWidth, maxHeight, textLayout.GetAddressOf());
		if (hRes != S_OK)
			throw Exception{};
		return textLayout;
	}


	/*
	FLOAT FontD2D::GetTextMaxHeight(const std::wstring& text, FLOAT maxWidth) {
		ChangeFontState(); //Create new textFormat object if user make some changes
		ComPtr<IDWriteTextLayout> textLayout;
		HRESULT hRes = m_pWriteFactory->CreateTextLayout(text.c_str(), text.size(), m_textFormat.Get(),
			maxWidth, 0, textLayout.GetAddressOf());
		if (hRes != S_OK)
			throw CreateFontException{};
		DWRITE_TEXT_METRICS textMetrix;
		textLayout->GetMetrics(&textMetrix);
		return textMetrix.height;
	}
	*/

}