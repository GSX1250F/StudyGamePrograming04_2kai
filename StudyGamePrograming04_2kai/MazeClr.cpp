#include "MazeClr.h"
#include "SpriteComponent.h"
#include "Game.h"

MazeClr::MazeClr(Game* game) : Actor(game)
{
	SetPosition(Vector2(GetGame()->mWindowWidth / 2.0f , 2*GetGame()->mWindowHeight));	//�����ʒu�͉�ʊO
	//�X�v���C�g�R���|�[�l���g�쐬�A�e�N�X�`���ݒ�
	SpriteComponent* sc = new SpriteComponent(this, 100);
	sc->SetTexture(game->GetTexture("Assets/MazeClr.png"));
}
