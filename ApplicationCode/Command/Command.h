#pragma once
class Command
{
public:
	/// <summary>
	/// デストラクタ
	/// </summary>
	virtual ~Command() {}

public: //メンバ関数
	/// <summary>
	/// コマンド実行
	/// </summary>
	virtual void Excute() = 0;
};

