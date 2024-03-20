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

	// ���H�̊e�A�N�^�[�̈ʒu�����Z�b�g
	void InitMaze(bool gamestart);
	// �}�b�v���i�āj�\��
	void GenerateMap();
	// �Q�[���N���A
	bool gameClear = false;
	// �Q�[���X�^�[�g
	bool gameStart = false;
	Vector2 GetTilePos(int index[2]);
	
	class Tile* GetStartTile();
	class Tile* GetEndTile();

	std::vector<std::vector<int>> mapIndex;
	int mapWidth;
	int mapHeight;
	
	//�X�^�[�g�ʒu�C���f�b�N�X
	int sindex[2];
	//�S�[���ʒu�C���f�b�N�X
	int gindex[2];	
	// �^�C���T�C�Y
	float mTileSize;
	
	// �o�H�T���p
	void MakeGraphNodes();
	bool FindPath(class Tile* start, class Tile* goal);
	void UpdatePathTiles(class Tile* start);
	std::unordered_map <Tile*, Tile* > outMap;	// �m�[�h����e�ւ̃}�b�v

};

