#include "TextDraw.h"

const std::wstring TextDraw::defaultFont = L"メイリオ";

void TextDraw::Initialize() {
	DirectXSetting::GetIns()->registerSolidColorBrush("white", D2D1::ColorF::White);
	DirectXSetting::GetIns()->registerSolidColorBrush("orange", D2D1::ColorF::Orange);
	DirectXSetting::GetIns()->registerTextFormat("meiryo", defaultFont, defaultFontSize);
	DirectXSetting::GetIns()->registerTextFormat("meiryo_16", defaultFont, 16);
	DirectXSetting::GetIns()->registerFontFormat("bestTen", L"ベストテン-CRT", L"Engine/Resources/fonts/BestTen-CRT.otf", defaultFontSize);
}

void TextDraw::Draw(const std::string& textFormatKey, const std::string& solidColorBrushKey, const std::wstring& text, const D2D1_RECT_F& rect) {
	const auto textFormat = DirectXSetting::GetIns()->GetTextFormats().at(textFormatKey);
	const auto solidColorBrush = DirectXSetting::GetIns()->GetColorBrushes().at(solidColorBrushKey);

	DirectXSetting::GetIns()->GetD2DDeviceContext()->DrawTextW(text.c_str(),
		static_cast<UINT32>(text.length()), textFormat.Get(), &rect, solidColorBrush.Get());

}