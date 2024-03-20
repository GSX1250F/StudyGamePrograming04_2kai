#include "MazeClr.h"
#include "SpriteComponent.h"
#include "Game.h"

MazeClr::MazeClr(Game* game) : Actor(game)
{
	//スプライトコンポーネント作成、テクスチャ設定
	SpriteComponent* sc = new SpriteComponent(this, 100);
	sc->SetTexture(game->GetTexture("Assets/MazeClr.png"));
}
