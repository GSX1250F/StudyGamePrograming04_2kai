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
	// ‚Q‚Â‚ÌCircleComponent‚Ì’†SŠÔ‹——£‚ğŒvZ
	Vector2 diff = a.GetCenter() - b.GetCenter();
	float dist = diff.Length();

	// ‚Q‚Â‚ÌCircleComponent‚Ì”¼Œa‚Ì˜a‚ğŒvZ 
	float sumRadius = a.GetRadius() + b.GetRadius();

	// ’†SŠÔ‹——£ <= ”¼Œa‚Ì˜a ‚Ì‚Æ‚«AÕ“Ë‚µ‚½‚Æ”»’è
	if (dist <= sumRadius)
	{
		return true;
	}
	else
	{
		return false;
	}
}
