#pragma once


class StageCount
{
public:
	

	void Initi();

	int Now();

	int Up();

	static StageCount* GetIns();

private:
	int stagenum = 0;
};

//int StageCount::stagenum = 0;

inline void StageCount::Initi()
{
	stagenum = 0;
}

inline int StageCount::Now()
{
	return stagenum;
}

inline int StageCount::Up()
{
	return stagenum += 1;

}
inline StageCount* StageCount::GetIns()
{
	static StageCount instance;
	return &instance;
}
