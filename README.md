# ECS-Pong
This game is a very quick and unfinished game to demonstrate the utility of game development with an Entity Component System.

This game uses a fully qualified ECS, meaning the data and logic are 100% separated.
All code was written by myself.

# Principles
Here is how it works:
### Component
A component is literally just data and a pointer to the Entity which owns it. Each component should have enough data to provide a certain functionality, and nothing more. For example, a position component would have two ints for the x and y positions, in 2D.

### Entity
An entity is a container of components and a name, in my system. Naming is not required, but I use it because it gives me an easy way to classify entities into groups.

### System
A system is a set of logic which is called every update and may change the components it is accessing. It is passed a list of entities which have components that this system cares about each update, and acts however defined.

### Entity Manager
This is what ties everything together.
The entity manager is a big sorting machine. It has a list of entities which can be thought of as the input, and a list of systems which can be thought of as the output. Its job is to figure out which entities to pass into which system on each update. It also deletes entities flagged for removal.

# Real example
Let's say we want to be able to render a square to the screen. Here's how I
would do that:

### Component
This would generally require two components. An object needs to have a rectangle and it *should* be renderable. This is just a design choice, though. You can implement it many ways. Regardless, here is how I would do it.
First, include the ECS header and iostream.
```
#include "ECS.hpp"
#include <iostream>
```
Second, make the component. It must extend Component to work. If you implement
a constructor, you can easily pass arguments, as you will see later.
```
struct RectangleComponent : public Component
{
	float w, h;
	RectangleComponent(float width, float height)
	{
		w = width;
		h = height;
	}
};
```
And that's it! The component is just a container of data.
Now, I will make a component which will be a flag for rendering.
```
struct RenderComponent : public Component {};
```
Why is there nothing in the struct? This component serves only as a flag. This is useful because I might not want to render everything that has a width and height.

### System
The system is given entities with the appropriate components and you get to write the update function. There is a lot of boilerplate but bear with me.
```
struct RenderSystem : public System
{
	// In the constructor, use the AddComponents method to tell the manager
	// which components an entity must have to be passed to this Update()
	RenderSystem()
	{
		AddComponents<RectangleComponent, RenderComponent>();
	}

	// The update function is where you add the logic. The header is boilerplate
	// but everything inside the function is fair game.
	void Update(double deltatime, std::vector<Entity*> entities)
	{
		// I'm going to loop through the entities and draw them on the screen
		for (Entity* e : entities)
		{
			// I get a pointer to the rectangle component for easy access
			auto rect = e->GetComponent<RectangleComponent>();
			for (int row = 0; row < rect->h; row++)
			{
				for (int col = 0; col < rect->w; col++)
				{
					std::cout << "X";
				}
				std::cout << std::endl;
			}

			// I print a newline at the end to separate multiple rectangles
			std::cout << std::endl;
		}
	}
};
```
And that is the system done. It's the hardest part but nothing is very hard once you get a grasp of this architecture. The whole system can be condensed into only a few lines while still maintaining readability.

## Putting it together
There MUST be an EntityManager in order for anything to happen. The typedef `AutoManager` masks a `unique_ptr` to an EntityManager.
```
AutoManager manager(new EntityManager());
```
Next, add the render system to the manager.
```
manager->AddSystem<RenderSystem>();
```
Thats it! Now we have to create an entity that we can add the components to.
```
auto& box = manager->AddEntity();
```
This is so siple because *Entities are just containers*.
Give it a name if you want:
```
box.name = "mybox";
```
Finally, add the components to it.
```
box.AddComponent<RenderComponent>();
box.AddComponent<RectangleComponent>(3, 5);
```
The parameters in `AddComponent()` are forwarded to the constructor of the component passed into the template. This allows us to *basically* call the constructor from the parameters.
You can get a reference to the component if you want, but there is almost always no point in a properly designed system. If you want, do it like so:
```
auto& rect = box.AddComponent<RenderComponent>();
```
Finally, the system is set up. The update function in the render system will be called every time you update the manager. You can move this into a game loop for games, or manually call it or testing. Call update with the time
that has elapsed since the last update. This time does not need to be the actual time. For testing, I will just use 1 second.
```
manager->Update(1);
```
If you leave the application like this, it will set up the ECS, print the box, and exit.

# Running the tutorial and example application
The tutorial above can be run using `make tutorial`
The pong game demo can be run using `make pong`

The pong game requires the ncurses library. You probably have it already.
To play the pong game, use W and S to move up and down, respectively.
Press Q an any time to exit the application.