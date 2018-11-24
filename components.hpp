struct AIComponent : public Component
{
	Entity* ball;
	AIComponent(Entity* b) : ball(b) {};
};

struct CollisionComponent : public Component
{	
};

struct KeyboardComponent : public Component
{
};

struct RenderComponent : public Component
{
	int red, green, blue;
	RenderComponent(int r, int g, int b) : red(r), green(g), blue(b) {};
};

struct SquareObjectComponent : public Component
{
	float xpos, ypos, width, height;
	SquareObjectComponent(float x, float y, float w, float h) : xpos(x), ypos(y), width(w), height(h) {};
};

struct VelocityComponent : public Component
{
	float xvel = 0;
	float yvel = 0;
	VelocityComponent() {}
	VelocityComponent(float x, float y) : xvel(x), yvel(y) {};
};
