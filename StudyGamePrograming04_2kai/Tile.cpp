#include "Tile.h"
#include "AnimSpriteComponent.h"
#include "Game.h"
#include "CircleComponent.h"

Tile::Tile(class Game* game)
	:Actor(game)
	, mTileState(EDefault)
	, mParent(nullptr)
{
	// アニメーションのスプライトコンポーネントを作成
	asc = new AnimSpriteComponent(this, 10);
	std::vector<SDL_Texture*> anims = {
		game->GetTexture("Assets/Default.png"),
		game->GetTexture("Assets/Wall.png"),
		game->GetTexture("Assets/Path.png")
	};
	asc->SetAnimTextures(anims, 1, 3, false);
	mTexSize = asc->GetTexWidth();
	
	//CircleComponent作成
	cc = new CircleComponent(this);
	cc->SetRadius(asc->GetTexWidth() / 2.0f);
	
}

void Tile::UpdateActor(float deltaTime)
{
	UpdateTexture();
}

void Tile::SetTileState(TileState state)
{
	mTileState = state;
	UpdateTexture();
}

void Tile::UpdateTexture()
{
	switch (mTileState)
	{
		case EPath:
			asc->SetAnimNum(3, 3, false);
			break;
		case EWall:
			asc->SetAnimNum(2, 2, false);
			break;
		default:
			asc->SetAnimNum(1, 1, false);
	}
}