#pragma once
#include <vector>
#include "Actor.h"
#include <unordered_map>

class Maze : public Actor
{
public:
	Maze(class Game* game);
	void ActorInput(const uint8_t* keyState) override;
	void UpdateActor(float deltaTime) override;


	int GetMapWidth() { return mapWidth; }
	int GetMapHeight() { return mapHeight; }
	void SetWidth(int width) { mapWidth = width; }
	void SetHeigth(int height) { mapHeight = height; }
	std::vector<std::vector<int>> GetMapIndex() { return mapIndex; }
	void SetMapIndex(int i, int j, int data) { mapIndex[i][j] = data; }

	void GenerateMap();
	bool startOk = false;
	void SetMazeClear(bool data) { mazeClear = data; };

	
	
private:
	std::vector<std::vector<int>> mapIndex;
	int mapWidth;
	int mapHeight;
	
	//�X�^�[�g�ʒu�C���f�b�N�X
	int sxindex;
	int syindex;
	//�S�[���ʒu�C���f�b�N�X
	int gxindex;	
	int gyindex;
	
	// ���H�N���A
	bool mazeClear = false;

	// �o�H�T���p
	void MakeGraphNodes();
	bool FindPath(class Tile* start, class Tile* goal);
	void UpdatePathTiles(class Tile* start);
	std::unordered_map <Tile*, Tile* > outMap;	// �m�[�h����e�ւ̃}�b�v

};

