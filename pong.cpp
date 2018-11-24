#include "imports.hpp"
#include "colorpicker.hpp"
#include "engine/ECS.hpp"
#include "components.hpp"
#include "systems.hpp"
#include "Keyboard.hpp"

#define BACKGROUNDCOLOR 60, 40, 40
#define WALLCOLOR 0, 0, 0
#define BALLCOLOR 220, 220, 220
#define PLAYERCOLOR 20, 220, 20
#define ENEMYCOLOR 220, 20, 20

#define GAMESPEED 1
#define FPS 200

/**
 * Loops until q is pressed. Checks input and updates the game every frame.
 */
void mainloop(AutoManager manager)
{
	while (1) {
		// Check input
		char ch = getch();
		if (ch == 'q') {
			break;
		} else if (ch == 'w') {
			Keyboard.up = true;
			Keyboard.dn = false;
		} else if (ch == 's') {
			Keyboard.dn = true;
			Keyboard.up = false;
		}

		// Update and limit framerate to 200 FPS
		usleep(1000000 / FPS);
		manager->Update((float)GAMESPEED / 10);
	}
}

int main()
{	
	// Setup terminal
	initscr();
	refresh();
	cbreak();
	nodelay(stdscr, TRUE);
	noecho();
	start_color();
	keypad(stdscr, TRUE);
	curs_set(0);
	srand(time(NULL));

	int maxx, maxy;
	getmaxyx(stdscr, maxy, maxx);
	
	// Setup logging
	freopen("log.txt", "w", stderr);

	// Initialize game
	AutoManager manager(new EntityManager());
	manager->AddSystem<RenderSystem>();
	manager->AddSystem<MovementSystem>();
	manager->AddSystem<ControlSystem>();
	manager->AddSystem<AISystem>();
	manager->AddSystem<CollisionSystem>();

	// Initialize entities and components
	auto& bg = manager->AddEntity();
	bg.AddComponent<SquareObjectComponent>(0, 0, maxy, maxx);
	bg.AddComponent<RenderComponent>(BACKGROUNDCOLOR);

	auto& ball = manager->AddEntity();
	ball.name = "ball";
	ball.AddComponent<SquareObjectComponent>(maxy/2-1, maxx/2-1, 2, 3);
	ball.AddComponent<RenderComponent>(BALLCOLOR);
	ball.AddComponent<VelocityComponent>(rand()%5-2, -5);
	ball.AddComponent<CollisionComponent>();

	auto& player = manager->AddEntity();
	player.name = "player";
	player.AddComponent<SquareObjectComponent>(maxy/2-3, maxx-4, 7, 2);
	player.AddComponent<RenderComponent>(PLAYERCOLOR);
	player.AddComponent<VelocityComponent>();
	player.AddComponent<KeyboardComponent>();
	player.AddComponent<CollisionComponent>();
	
	auto& enemy = manager->AddEntity();
	enemy.name = "enemy";
	enemy.AddComponent<SquareObjectComponent>(maxy/2-3, 4, 7, 2);
	enemy.AddComponent<RenderComponent>(ENEMYCOLOR);
	enemy.AddComponent<VelocityComponent>();
	enemy.AddComponent<AIComponent>(&ball);
	enemy.AddComponent<CollisionComponent>();

	auto& walltop = manager->AddEntity();
	walltop.name = "wall";
	walltop.AddComponent<SquareObjectComponent>(0, 0, 2, maxx);
	walltop.AddComponent<CollisionComponent>();
	walltop.AddComponent<RenderComponent>(WALLCOLOR);

	auto& wallbottom = manager->AddEntity();
	wallbottom.name = "wall";
	wallbottom.AddComponent<SquareObjectComponent>(maxy-2, 0, 2, maxx);
	wallbottom.AddComponent<CollisionComponent>();
	wallbottom.AddComponent<RenderComponent>(WALLCOLOR);

	auto& goalleft = manager->AddEntity();
	goalleft.name = "goal";
	goalleft.AddComponent<SquareObjectComponent>(0, 0, maxy, 2);
	goalleft.AddComponent<CollisionComponent>();
	goalleft.AddComponent<RenderComponent>(WALLCOLOR);

	auto& goalright = manager->AddEntity();
	goalright.name = "goal";
	goalright.AddComponent<SquareObjectComponent>(0, maxx-2, maxy, 2);
	goalright.AddComponent<CollisionComponent>();
	goalright.AddComponent<RenderComponent>(WALLCOLOR);

	// Game loop
    mainloop(std::move(manager));
	
	// Exit
	endwin();
}

