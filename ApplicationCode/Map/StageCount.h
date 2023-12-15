#pragma once


class StageCount
{
public:
	StageCount();
	~StageCount();

	void Initi();

	int Now();

	int Up();

	static StageCount* GetIns();

private:
	static int stagenum;
};

int StageCount::stagenum = 0;

StageCount::StageCount()
{
	
}

StageCount::~StageCount()
{
}

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
