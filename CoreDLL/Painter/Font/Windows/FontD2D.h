#ifndef FONTD2D_PAINTER_DLL
#define FONTD2D_PAINTER_DLL

#include <string>
#include <d2d1.h>
#include <wrl.h>
#include <dwrite.h>
#include "../IFont.h"

namespace Nk {

	using Microsoft::WRL::ComPtr;

	class FontD2D : public IFont {
	public:
		FontD2D(const std::wstring& fontFamily, IDWriteFontCollection* fontCollection, DWRITE_FONT_WEIGHT weight, DWRITE_FONT_STYLE fontStyle, DWRITE_FONT_STRETCH fontStretch,
			FLOAT fontSize, const std::wstring& localName);
		FontD2D(const std::wstring& fontFamily, FLOAT fontSize);
		FontD2D();

		IFont* SetSizeInPt(int sizeInPt) override;
		IFont* SetSizeInPixels(int sizeInPixels) override;
		IFont* SetFamily(std::wstring familyName) override;
		IFont* SetHorizontalAlignment(HorizontalAlignment alignment) override;
		IFont* SetVerticalAlignment(VerticalAlignment alignment) override;

		//FLOAT GetTextMaxHeight(const std::wstring& text, FLOAT maxWidth) override;

		//This function is called every time, when user want to draw text layout
		//Used by PainterD2D to draw formated text in specified layout
		ComPtr<IDWriteTextLayout> GetFormattedTextLayout(const std::wstring text, D2D_RECT_F textRect);

	private:
		ComPtr<IDWriteFactory> m_pWriteFactory;

		ComPtr<IDWriteTextFormat> m_textFormat; //Represent default text format

		//Describes font stated, that can be changed
		enum StateFontFlags : int64_t { fontSize = 0x1, fontFamily = 0x2 };
		int64_t m_changedFontStates = ~(int64_t)0; //Show, that state of the text layout was chanded (add new config params)

		void ChangeFontState();
		void CreateNewTextFormat();

		std::wstring m_fontFamily;
		std::wstring m_fontLocalName;
		IDWriteFontCollection* m_fontCollection;
		DWRITE_FONT_WEIGHT m_fontWeight;
		DWRITE_FONT_STYLE m_fontStyle;
		DWRITE_FONT_STRETCH m_fontStretch;
		FLOAT m_fontSize; //In pixels (1/96 inch)

		//Text characteristics
		DWRITE_TEXT_ALIGNMENT m_textAlignment = DWRITE_TEXT_ALIGNMENT_LEADING;
		DWRITE_PARAGRAPH_ALIGNMENT m_textVerticalAlignment = DWRITE_PARAGRAPH_ALIGNMENT_NEAR;
	};
}

#endif	//#ifndef FONTD2D_PAINTER_DLL

//У каждого окна должен быть свой связанный "художник"
