#include "diablo.h"

unsigned char SkelKingTrans1[8] =
{
	19, 47, 26, 55,
	26, 49, 30, 53
};

unsigned char SkelKingTrans2[8] =
{
	33, 19, 47, 29,
	37, 29, 43, 39
};

unsigned char SkelKingTrans3[20] =
{
	27, 53, 35, 61,
	27, 35, 34, 42,
	45, 35, 53, 43,
	45, 53, 53, 61,
	31, 39, 49, 57
};

unsigned char SkelKingTrans4[28] =
{
	49, 45, 58, 51,
	57, 31, 62, 37,
	63, 31, 69, 40,
	59, 41, 73, 55,
	63, 55, 69, 65,
	73, 45, 78, 51,
	79, 43, 89, 53
};

unsigned char SkelChamTrans1[20] =
{
	43, 19, 50, 26,
	51, 19, 59, 26,
	35, 27, 42, 34,
	43, 27, 49, 34,
	50, 27, 59, 34
};

unsigned char SkelChamTrans2[8] =
{
	19, 31, 34, 47,
	34, 35, 42, 42
};

unsigned char SkelChamTrans3[36] =
{
	43, 35, 50, 42,
	51, 35, 62, 42,
	63, 31, 66, 46,
	67, 31, 78, 34,
	67, 35, 78, 42,
	67, 43, 78, 46,
	35, 43, 42, 51,
	43, 43, 49, 51,
	50, 43, 59, 51
};

char *quest_level_names[] =
{
  "",
  "Skeleton King's Lair",
  "Bone Chamber",
  "Maze",
  "Poisoned Water Supply",
  "Archbishop Lazarus' Lair"
};

int ObjIndex(int x, int y)
{
	int i, oi;

	for(i = 0; i < nobjects; i++) {
		oi = objectactive[i];
		if(object[oi]._ox == x && object[oi]._oy == y) {
			return oi;
		}
	}

	app_fatal("ObjIndex: Active object not found at (%d,%d)", x, y);
	return -1;
}

void AddSKingObjs()
{
	SetObjMapRange(ObjIndex(64, 34), 20, 7, 23, 10, 1);
	SetObjMapRange(ObjIndex(64, 59), 20, 14, 21, 16, 2);
	SetObjMapRange(ObjIndex(27, 37), 8, 1, 15, 11, 3);
	SetObjMapRange(ObjIndex(46, 35), 8, 1, 15, 11, 3);
	SetObjMapRange(ObjIndex(49, 53), 8, 1, 15, 11, 3);
	SetObjMapRange(ObjIndex(27, 53), 8, 1, 15, 11, 3);
}

void AddSChamObjs()
{
	SetObjMapRange(ObjIndex(37, 30), 17, 0, 21, 5, 1);
	SetObjMapRange(ObjIndex(37, 46), 13, 0, 16, 5, 2);
}

void AddVileObjs()
{
	SetObjMapRange(ObjIndex(26, 45), 1, 1, 9, 10, 1);
	SetObjMapRange(ObjIndex(45, 46), 11, 1, 20, 10, 2);
	SetObjMapRange(ObjIndex(35, 36), 7, 11, 13, 18, 3);
}

void DRLG_SetMapTrans(char *sFileName)
{
	int i, j, rw, rh;
	long mapoff;
	BYTE *pLevelMap, *lm;

	pLevelMap = DiabLoad(sFileName, NULL, 'LMPt');
	lm = pLevelMap;
	rw = *lm;
	lm += 2;
	rh = *lm;
	mapoff = 2 * rw * rh + 2;
	rw <<= 1;
	rh <<= 1;
	mapoff += 6 * rw * rh;
	lm += mapoff;

	for(j = 0; j < rh; j++) {
		for(i = 0; i < rw; i++) {
			dTransVal[i + 16][j + 16] = *lm;
			lm += 2;
		}
	}

	MemFreeDbg(pLevelMap);
}

void LoadSetMap()
{
	switch(setlvlnum) {
	case SL_SKELKING:
		if(quests[Q_SKELKING]._qactive == 1) {
			quests[Q_SKELKING]._qactive = 2;
			quests[Q_SKELKING]._qvar1 = 1;
		}
		LoadPreL1Dungeon("Levels\\L1Data\\SklKng1.DUN", 83, 45);
		LoadL1Dungeon("Levels\\L1Data\\SklKng2.DUN", 83, 45);
		LoadPalette("Levels\\L1Data\\L1_2.pal");
		DRLG_AreaTrans(2, SkelKingTrans1);
		DRLG_ListTrans(2, SkelKingTrans2);
		DRLG_AreaTrans(5, SkelKingTrans3);
		DRLG_ListTrans(7, SkelKingTrans4);
		AddL1Objs(0, 0, 112, 112);
		AddSKingObjs();
		InitSKingTriggers();
		break;
	case SL_BONECHAMB:
		LoadPreL2Dungeon("Levels\\L2Data\\Bonecha2.DUN", 69, 39);
		LoadL2Dungeon("Levels\\L2Data\\Bonecha1.DUN", 69, 39);
		LoadPalette("Levels\\L2Data\\L2_2.pal");
		DRLG_ListTrans(5, SkelChamTrans1);
		DRLG_AreaTrans(2, SkelChamTrans2);
		DRLG_ListTrans(9, SkelChamTrans3);
		AddL2Objs(0, 0, 112, 112);
		AddSChamObjs();
		InitSChambTriggers();
		break;
	case SL_MAZE:
		LoadPreL1Dungeon("Levels\\L1Data\\Lv1MazeA.DUN", 20, 50);
		LoadL1Dungeon("Levels\\L1Data\\Lv1MazeB.DUN", 20, 50);
		LoadPalette("Levels\\L1Data\\L1_5.pal");
		AddL1Objs(0, 0, 112, 112);
		DRLG_SetMapTrans("Levels\\L1Data\\Lv1MazeA.DUN");
		break;
	case SL_POISONWATER:
		if(quests[Q_PWATER]._qactive == 1) {
			quests[Q_PWATER]._qactive = 2;
		}
		LoadPreL3Dungeon("Levels\\L3Data\\Foulwatr.DUN", 19, 50);
		LoadL3Dungeon("Levels\\L3Data\\Foulwatr.DUN", 20, 50);
		LoadPalette("Levels\\L3Data\\L3pfoul.pal");
		InitPWaterTriggers();
		break;
	case SL_VILEBETRAYER:
		if(quests[Q_BETRAYER]._qactive == 3) {
			quests[Q_BETRAYER]._qvar2 = 4;
		} else if(quests[Q_BETRAYER]._qactive == 2) {
			quests[Q_BETRAYER]._qvar2 = 3;
		}
		LoadPreL1Dungeon("Levels\\L1Data\\Vile1.DUN", 35, 36);
		LoadL1Dungeon("Levels\\L1Data\\Vile2.DUN", 35, 36);
		LoadPalette("Levels\\L1Data\\L1_2.pal");
		AddL1Objs(0, 0, 112, 112);
		AddVileObjs();
		DRLG_SetMapTrans("Levels\\L1Data\\Vile1.DUN");
		InitNoTriggers();
		break;
	}
}
