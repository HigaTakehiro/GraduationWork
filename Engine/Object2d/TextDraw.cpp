#include "TextDraw.h"

const std::wstring TextDraw::defaultFont = L"メイリオ";

void TextDraw::Initialize() {
	DirectXSetting::GetIns()->registerSolidColorBrush("white", D2D1::ColorF::White);
	DirectXSetting::GetIns()->registerSolidColorBrush("orange", D2D1::ColorF::Orange);
	DirectXSetting::GetIns()->registerSolidColorBrush("black", D2D1::ColorF::Black);
	DirectXSetting::GetIns()->registerSolidColorBrush("red", D2D1::ColorF::Red);
	DirectXSetting::GetIns()->registerSolidColorBrush("green", D2D1::ColorF::Green);
	DirectXSetting::GetIns()->registerSolidColorBrush("blue", D2D1::ColorF::Blue);
	DirectXSetting::GetIns()->registerSolidColorBrush("cyan", D2D1::ColorF::Cyan);
	DirectXSetting::GetIns()->registerSolidColorBrush("magenta", D2D1::ColorF::Magenta);
	DirectXSetting::GetIns()->registerSolidColorBrush("yellow", D2D1::ColorF::Yellow);
	DirectXSetting::GetIns()->registerSolidColorBrush("purple", D2D1::ColorF::Purple);
	DirectXSetting::GetIns()->registerSolidColorBrush("gray", D2D1::ColorF::Gray);
	DirectXSetting::GetIns()->registerSolidColorBrush("darkGray", D2D1::ColorF::DarkGray);
	DirectXSetting::GetIns()->registerSolidColorBrush("lightGray", D2D1::ColorF::LightGray);
	DirectXSetting::GetIns()->registerSolidColorBrush("dimGray", D2D1::ColorF::DimGray);
	DirectXSetting::GetIns()->registerTextFormat("meiryo", defaultFont, defaultFontSize);
	DirectXSetting::GetIns()->registerTextFormat("meiryo_16", defaultFont, 16);
	DirectXSetting::GetIns()->registerFontFormat("bestTen", L"ベストテン-CRT", L"Engine/Resources/fonts/BestTen-CRT.otf", defaultFontSize);
	DirectXSetting::GetIns()->registerFontFormat("bestTen_16", L"ベストテン-CRT", L"Engine/Resources/fonts/BestTen-CRT.otf", 16);
	DirectXSetting::GetIns()->registerFontFormat("bestTen_12", L"ベストテン-CRT", L"Engine/Resources/fonts/BestTen-CRT.otr", 12);
}

void TextDraw::Draw(const std::string& textFormatKey, const std::string& solidColorBrushKey, const std::wstring& text, const D2D1_RECT_F& rect) {
	const auto textFormat = DirectXSetting::GetIns()->GetTextFormats().at(textFormatKey);
	const auto solidColorBrush = DirectXSetting::GetIns()->GetColorBrushes().at(solidColorBrushKey);

	DirectXSetting::GetIns()->GetD2DDeviceContext()->DrawTextW(text.c_str(),
		static_cast<UINT32>(text.length()), textFormat.Get(), &rect, solidColorBrush.Get());

}