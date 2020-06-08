#pragma once
class Components
{
public:
	Components();
	virtual ~Components();

	virtual void initialize() {};
	virtual void update(const float& deltaTime) {};
	virtual void render(const float& deltaTime) {};

};

