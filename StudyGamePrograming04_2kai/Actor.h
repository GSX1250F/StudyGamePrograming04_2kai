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
	const Vector2& GetVelocity() const { return mVelocity; }		//���x�Q�b�^�[��ǉ�
	void SetPosition(const Vector2& pos) { mPosition = pos; }
	void SetVelocity(const Vector2& vel) { mVelocity = vel; }		//���x�Z�b�^�[��ǉ�
	float GetScale() const { return mScale; }
	void SetScale(float scale) { mScale = scale; }
	float GetRotation() const { return mRotation; }
	void SetRotation(float rotation) { mRotation = rotation; }

	Vector2 GetForward() const { return Vector2(Math::Cos(mSpin), -Math::Sin(mSpin)); }		//�P�ʃx�N�g���������擾

	float GetSpin() const { return mSpin; }				//�X�s���Q�b�^�[��ǉ�
	void SetSpin(float spin) { mSpin = spin; }			//�X�s���Z�b�^�[��ǉ�


	State GetState() const { return mState; }
	void SetState(State state) { mState = state; }

	class Game* GetGame() { return mGame; }


	// Add/remove components
	void AddComponent(class Component* component);
	void RemoveComponent(class Component* component);

private:
	// �A�N�^�[�̏��
	State mState;

	// �ړ�
	Vector2 mPosition;		//�ʒux,y
	Vector2 mVelocity;		//���xx,y
	float mScale;			//�傫��
	float mRotation;		//�^������ (rad)

	// ��]�̂�
	float mSpin;			//��]���� (rad)

	std::vector<class Component*> mComponents;
	class Game* mGame;
};
