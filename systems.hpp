#pragma once

#include "engine/ECS.hpp"
#include "Keyboard.hpp"

/**
 * Renders entities.
 */
struct RenderSystem : public System
{
	RenderSystem() { AddComponents<RenderComponent, SquareObjectComponent>(); }
	void Update(double deltatime, std::vector<Entity*> entities)
	{
		for (Entity* e : entities) {
			auto obj = e->GetComponent<SquareObjectComponent>();
			auto col = e->GetComponent<RenderComponent>();

			// Loop through the width and height and fill in the box
			for (int i = round(obj->xpos); i < round(obj->xpos+obj->width); i++) {
				for (int j = round(obj->ypos); j < round(obj->ypos+obj->height); j++) {
					fromRGB(col->red, col->green, col->blue);
					mvaddch(i, j, ' ');
				}
			}
		}
	}
};

/**
 * Moves entities based on their velocity.
 */
struct MovementSystem : public System
{
	MovementSystem() { AddComponents<SquareObjectComponent, VelocityComponent>(); }
	void Update(double deltatime, std::vector<Entity*> entities)
	{
		for (Entity* e : entities) {
			auto square = e->GetComponent<SquareObjectComponent>();
			auto vel = e->GetComponent<VelocityComponent>();
			square->xpos += vel->xvel * deltatime;
			square->ypos += vel->yvel * deltatime;
		}
	}
};

/**
 * Moves entities towards being aligned with the ball.
 */
struct AISystem : public System
{
	AISystem() { AddComponents<AIComponent, SquareObjectComponent, VelocityComponent>(); }
	void Update(double deltatime, std::vector<Entity*> entities)
	{
		for (Entity* e : entities) {
			auto square = e->GetComponent<SquareObjectComponent>();
			auto ai = e->GetComponent<AIComponent>();
			auto vel = e->GetComponent<VelocityComponent>();
			auto ballsquare = ai->ball->GetComponent<SquareObjectComponent>();
			if (ballsquare->xpos > square->xpos+3)
				vel->xvel = 2;
			else
				vel->xvel = -2;
		}
	}
};

/**
 * Moves entities based on the keyboard state.
 */
struct ControlSystem : public System
{
	ControlSystem() { AddComponents<KeyboardComponent, SquareObjectComponent, VelocityComponent>(); }
	void Update(double deltatime, std::vector<Entity*> entities)
	{
		for (Entity* e : entities) {
			auto vel = e->GetComponent<VelocityComponent>();
			if (Keyboard.up) vel->xvel = -2;
			else if (Keyboard.dn) vel->xvel = 2;
			else vel->xvel = 0;			
		}
	}
};

/**
 * Standard AABB collision detection.
 */
bool AABB(float x1, float y1, float w1, float h1, float x2, float y2, float w2, float h2)
{
    return (x1 < x2 + w2 &&
        x2 < x1 + w1 &&
        y1 < y2 + h2 &&
        y2 < y1 + h1);
}

/**
 * Acts accordingly when objects collide. Decides what to do based on their names.
 */
struct CollisionSystem : public System
{
	CollisionSystem() { AddComponents<SquareObjectComponent>(); }
	void Update(double deltatime, std::vector<Entity*> entities)
	{
		// Find the ball		
		Entity* ball;
		for (Entity* e : entities) {
			if (e->name == "ball") {
			    ball = e;
			    break;
			}
		}

		// If ball is found, check if it collides with other entities and act accordingly.
		if (ball != nullptr) {
		    auto ballsq = ball->GetComponent<SquareObjectComponent>();
		    for (Entity* e : entities) {
		        if (e == ball) continue;
                auto sq = e->GetComponent<SquareObjectComponent>();
                if (AABB(sq->xpos, sq->ypos, sq->width, sq->height, ballsq->xpos, ballsq->ypos, ballsq->width, ballsq->height)) {
					if (e->name == "player" || e->name == "enemy")
						ball->GetComponent<VelocityComponent>()->yvel *= -1;
					else if (e->name == "wall") 
						ball->GetComponent<VelocityComponent>()->xvel *= -1;
					else if (e->name == "goal") 
						ball->GetComponent<VelocityComponent>()->yvel *= -1;
                }		        
		    }
		}
	}
};
