#include "Treasure.h"
#include "Game.h"
#include "SpriteComponent.h"
#include "CircleComponent.h"

Treasure::Treasure(Game* game) : Actor(game){
	//スプライトコンポーネント作成、テクスチャ設定
	SpriteComponent* sc = new SpriteComponent(this, 20);
	sc->SetTexture(game->GetTexture("Assets/Treasure.png"));
	
	//CircleComponent作成
	cc = new CircleComponent(this);
	cc->SetRadius(sc->GetTexWidth() / 10.0f);
}
