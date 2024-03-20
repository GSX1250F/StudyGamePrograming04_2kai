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

	//画面サイズ
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

	// テクスチャの配列
	std::unordered_map<std::string, SDL_Texture*> mTextures;

	// すべてのアクター
	std::vector<class Actor*> mActors;
	// すべての待ちアクター
	std::vector<class Actor*> mPendingActors;

	// すべての描画されるスプライトコンポーネント
	std::vector<class SpriteComponent*> mSprites;

	SDL_Window* mWindow;
	SDL_Renderer* mRenderer;
	Uint32 mTicksCount;
	bool mIsRunning;
	// アクターが更新中か
	bool mIsUpdatingActors;

	
	//Game-specific	
	class Maze* maze;
	class Brave* brave;
	class Shadow* shadow;
	class MazeClr* mazeClr;
	class Tile* tile;
	std::vector<std::vector<Tile*>> tiles;
	
};
