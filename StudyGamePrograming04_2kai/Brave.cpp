#include "Brave.h"
#include "Game.h"
#include "Maze.h"
#include "Tile.h"
#include "Treasure.h"
#include "Maze.h"
#include "MazeClr.h"
#include "AnimSpriteComponent.h"
#include "CircleComponent.h"
#include "MoveComponent.h"


Brave::Brave(Game* game) : Actor(game){
	// アニメーションのスプライトコンポーネントを作成
	asc = new AnimSpriteComponent(this, 40);
	std::vector<SDL_Texture*> anims = {
		game->GetTexture("Assets/Brave01.png"),
		game->GetTexture("Assets/Brave02.png"),
		game->GetTexture("Assets/Brave03.png"),
		game->GetTexture("Assets/Brave04.png"),
		game->GetTexture("Assets/Brave05.png"),
		game->GetTexture("Assets/Brave06.png"),
		game->GetTexture("Assets/Brave07.png"),
		game->GetTexture("Assets/Brave08.png"),
		game->GetTexture("Assets/Brave09.png"),
		game->GetTexture("Assets/Brave10.png"),
		game->GetTexture("Assets/Brave11.png"),
		game->GetTexture("Assets/Brave12.png"),
		game->GetTexture("Assets/Brave13.png"),
		game->GetTexture("Assets/Brave14.png"),
		game->GetTexture("Assets/Brave15.png"),
		game->GetTexture("Assets/Brave16.png")
	};
	asc->SetAnimTextures(anims, 1, 4, true);

	//CircleComponent作成
	cc = new CircleComponent(this);
	cc->SetRadius(asc->GetTexWidth() / 2.0f);

	//MoveComponent作成
	mc = new MoveComponent(this);
}

void Brave::ActorInput(const uint8_t* keyState){
	if (GetGame()->maze->gameStart == true) {
		//入力に応じて、アニメーションの設定と移動
		SetVelocity(Vector2::Zero);		//速度リセット
		int bg = asc->GetAnimNumBeg();
		int ed = asc->GetAnimNumLast();
		if (keyState[SDL_SCANCODE_DOWN]){
			if (bg != 1 || ed != 4) { asc->SetAnimNum(1, 4, true); }
			SetVelocity(Vector2(0.0f, 150.0f));
		}
		else if (keyState[SDL_SCANCODE_UP]){
			if (bg != 5 || ed != 8) { asc->SetAnimNum(5, 8, true); }
			SetVelocity(Vector2(0.0f, -150.0f));
		}
		else if (keyState[SDL_SCANCODE_RIGHT]){
			if (bg != 9 || ed != 12) { asc->SetAnimNum(9, 12, true); }
			SetVelocity(Vector2(150.0f, 0.0f));
		}
		else if (keyState[SDL_SCANCODE_LEFT]){
			if (bg != 13 || ed != 16) { asc->SetAnimNum(13, 16, true); }
			SetVelocity(Vector2(-150.0f, 0.0f));
		}
	}
	
}

void Brave::UpdateActor(float deltaTime){
	if (GetGame()->maze->gameStart == true) {
		std::vector<std::vector<Tile*>> tiles = GetGame()->tiles;
		for (int i = 0; i < tiles.size(); i++){
			for (int j = 0; j < tiles[i].size(); j++){
				if (tiles[i][j]->mTileState == Tile::EWall){
					//壁衝突判定
					if (Intersect(*cc, *(tiles[i][j]->cc))){

						while (Intersect(*cc, *(tiles[i][j]->cc))){
							//位置を調整。差ベクトルのx,y成分の大きいほうを1とした分だけ離す
							Vector2 n = this->GetPosition() - tiles[i][j]->GetPosition();
							if (Math::Abs(n.x) >= Math::Abs(n.y)) { n = Vector2(n.x / Math::Abs(n.x), 0.0f); }
							else { n = Vector2(0.0f, n.y / Math::Abs(n.y)); }
							this->SetPosition(this->GetPosition() + n);
						}
					}
				}
			}
		}

		//ゴール判定
		if (Intersect(*cc, *(GetGame()->treasure->cc))) {
			GetGame()->maze->gameClear = true;
		}
	}
	
}