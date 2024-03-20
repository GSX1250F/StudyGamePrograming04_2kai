#include "CircleComponent.h"
#include "Actor.h"

CircleComponent::CircleComponent(Actor* owner) : Component(owner),mRadius(0.0f)
{}

float CircleComponent::GetRadius() const
{
	return mOwner->GetScale() * mRadius;
}

const Vector2& CircleComponent::GetCenter() const
{
	return  mOwner->GetPosition();
}

bool Intersect(const CircleComponent& a, const CircleComponent& b)
{
	// �Q��CircleComponent�̒��S�ԋ������v�Z
	Vector2 diff = a.GetCenter() - b.GetCenter();
	float dist = diff.Length();

	// �Q��CircleComponent�̔��a�̘a���v�Z 
	float sumRadius = a.GetRadius() + b.GetRadius();

	// ���S�ԋ��� <= ���a�̘a �̂Ƃ��A�Փ˂����Ɣ���
	if (dist <= sumRadius)
	{
		return true;
	}
	else
	{
		return false;
	}
}
