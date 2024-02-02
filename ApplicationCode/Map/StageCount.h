#pragma once


class StageCount
{
public:
	

	void Initi();

	int Now();

	int Up();

	int FloorFour();

	int FloorEight();

	int FloorFourTeen();

	int MoveSavePoint(int set);

	static StageCount* GetIns();

private:
	int stagenum = 0;
};

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
inline int StageCount::FloorFour()
{
	stagenum = 4;
	return stagenum;
}
inline int StageCount::FloorEight()
{
	stagenum = 8;
	return stagenum;
}
inline int StageCount::FloorFourTeen()
{
	stagenum = 14;
	return stagenum;
}
inline int StageCount::MoveSavePoint(int set)
{
	stagenum = set;

	return stagenum;
}
inline StageCount* StageCount::GetIns()
{
	static StageCount instance;
	return &instance;
}
