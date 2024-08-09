#pragma once
#include "Game/GameObjects/EModelObject.h"

class EPteraObject : public EModelObject {
public:
	EPteraObject();
	~EPteraObject() = default;

protected:
	// Run when the object spawns in
	virtual void OnStart() override;

	// Run every frame, passes in deltaTime
	virtual void OnTick(float deltaTime) override;

private:
	// Model scale
	float m_modelScale;
	float m_minModelScale;
	float m_maxModelScale;
	
	// Speed to move
	float m_moveSpeed;
	float m_maxMoveSpeed;

	// Flag to change size
	bool m_changeSize;

	// Move multi for the change size flag to oscillate size
	float m_moveMulti;
};