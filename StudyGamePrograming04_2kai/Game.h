#pragma once
#include "SDL.h"
#include <unordered_map>
#include <string>
#include <vector>

class Game
{
public:
	Game();
	bool Initialize();
	void RunLoop();
	void Shutdown();

	void AddActor(class Actor* actor);
	void RemoveActor(class Actor* actor);

	void AddSprite(class SpriteComponent* sprite);
	void RemoveSprite(class SpriteComponent* sprite);

	SDL_Texture* GetTexture(const std::string& fileName);

	void SetRunning(bool isrunning) { mIsRunning = isrunning; }

	//��ʃT�C�Y
	const int mWindowWidth = 1280;
	const int mWindowHeight = 1080;

	
	//Game-specific	
	class Maze* GetMaze() { return maze; }
	class Brave* GetBrave() { return brave; }
	class Shadow* GetShadow() { return shadow; }
	class MazeClr* GetMazeClr() { return mazeClr; }
	std::vector<std::vector<Tile*>> GetTiles() { return tiles; }
	void InitMaze();
	
private:
	void ProcessInput();
	void UpdateGame();
	void GenerateOutput();
	void LoadData();
	void UnloadData();

	// �e�N�X�`���̔z��
	std::unordered_map<std::string, SDL_Texture*> mTextures;

	// ���ׂẴA�N�^�[
	std::vector<class Actor*> mActors;
	// ���ׂĂ̑҂��A�N�^�[
	std::vector<class Actor*> mPendingActors;

	// ���ׂĂ̕`�悳���X�v���C�g�R���|�[�l���g
	std::vector<class SpriteComponent*> mSprites;

	SDL_Window* mWindow;
	SDL_Renderer* mRenderer;
	Uint32 mTicksCount;
	bool mIsRunning;
	// �A�N�^�[���X�V����
	bool mIsUpdatingActors;

	
	//Game-specific	
	class Maze* maze;
	class Brave* brave;
	class Shadow* shadow;
	class MazeClr* mazeClr;
	class Tile* tile;
	std::vector<std::vector<Tile*>> tiles;
	
};
