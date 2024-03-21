#pragma once
#include <vector>
#include "Math.h"
#include <cstdint>

class Actor
{
public:
	enum State
	{
		EActive,		//�ғ���
		EPaused,		//�X�V��~��
		EDead			//�폜�Ώ�
	};

	Actor(class Game* game);
	virtual ~Actor();

	// �Q�[������Ăяo�����X�V�֐�(�I�[�o�[���C�h�s��)
	void Update(float deltaTime);
	// �A�N�^�[�����S�R���|�[�l���g���X�V(�I�[�o�[���C�h�s��)
	void UpdateComponents(float deltaTime);
	// �A�N�^�[�Ǝ��̍X�V����(�I�[�o�[���C�h�\)
	virtual void UpdateActor(float deltaTime);

	// �Q�[������Ăяo�����Process Input(�I�[�o�[���C�h�\)
	virtual void ProcessInput(const uint8_t* keyState);
	// �A�N�^�[�Ǝ��̓��͏���(�I�[�o�[���C�h�\)
	virtual void ActorInput(const uint8_t* keyState);

	// Getters/setters
	const Vector2& GetPosition() const { return mPosition; }
	void SetPosition(const Vector2& pos) { mPosition = pos; }
	float GetScale() const { return mScale; }
	void SetScale(float scale) { mScale = scale; }
	float GetRotation() const { return mRotation; }
	void SetRotation(float rotation) { mRotation = rotation; }

	Vector2 GetForward() const { return Vector2(Math::Cos(mRotation + mDriftAngle), -Math::Sin(mRotation + mDriftAngle)); }

	State GetState() const { return mState; }
	void SetState(State state) { mState = state; }

	class Game* GetGame() { return mGame; }


	// Add/remove components
	void AddComponent(class Component* component);
	void RemoveComponent(class Component* component);

	//�I���W�i��
	//Vector2 GetDriftForward() const { return Vector2(Math::Cos(mDriftAngle), -Math::Sin(mDriftAngle)); }
	float GetDriftAngle() const { return mDriftAngle; }
	void SetDriftAngle(float angle) { mDriftAngle = angle; }
private:
	// �A�N�^�[�̏��
	State mState;

	// �ړ�
	Vector2 mPosition;		//�ʒux,y
	float mScale;			//�傫��
	float mRotation;		//�X�v���C�g���� (rad)
	
	std::vector<class Component*> mComponents;
	class Game* mGame;

	//�I���W�i��
	float mDriftAngle;		//�X�v���C�g������ς��Ȃ��A�N�^�[�����irad�j

};
