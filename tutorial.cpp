#include "engine/ECS.hpp"
#include <iostream>

struct RectangleComponent : public Component
{
    float w, h;
    RectangleComponent(float width, float height)
    {
        w = width;
        h = height;
    }
};

struct RenderComponent : public Component {};

struct RenderSystem : public System
{
    RenderSystem() { AddComponents<RectangleComponent, RenderComponent>(); }
    void Update(double deltatime, std::vector<Entity*> entities)
    {
        for (Entity* e : entities) {        
            auto rect = e->GetComponent<RectangleComponent>();
            for (int row = 0; row < rect->h; row++) {
                for (int col = 0; col < rect->w; col++) {
                    std::cout << "X";
                }
                std::cout << std::endl;
            }
            std::cout << std::endl;
        }
    }
};

int main()
{
	AutoManager manager(new EntityManager());
	manager->AddSystem<RenderSystem>();
	auto& box = manager->AddEntity();
	box.AddComponent<RenderComponent>();
	box.AddComponent<RectangleComponent>(3, 5);
	manager->Update(1);
	
	return 0;
}
