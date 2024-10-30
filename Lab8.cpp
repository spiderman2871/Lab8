

#include <iostream>
#include <SFML/Graphics.hpp>
#include <SFPhysics.h>

using namespace std;
using namespace sf;
using namespace sfp;

void LoadTex(Texture& tex, string filename) {
	if (!tex.loadFromFile(filename)) {
		cout << "Could not load " << filename << endl;
	}
}

int main()
{
	RenderWindow window(VideoMode(800, 600), "Duck Hunter");
	World world(Vector2f(0, 0));
	int score(0);
	int arrows(5);

    PhysicsSprite& crossbow = *new PhysicsSprite(); 
    Texture cbowTex; 
    LoadTex(cbowTex, "images/crossbow.png"); 
    crossbow.setTexture(cbowTex); 
	crossbow.setCenter(Vector2f(400,570));

	PhysicsSprite arrow; 
	Texture arrowTex;
	LoadTex(arrowTex, "images/arrow.png");
	arrow.setTexture(arrowTex);
	bool drawingArrow(false);

	PhysicsRectangle top;
	top.setSize(Vector2f(800,10));
	top.setCenter(Vector2f(400, 5));
	top.setStatic(true);
	world.AddPhysicsBody(top);

	Texture duckTex;
	LoadTex(duckTex, "images/duck.png");
	PhysicsShapeList<PhysicsSprite> ducks;

	top.onCollision = [&drawingArrow, &world, &arrow](PhysicsBodyCollisionResult result) {
		drawingArrow = false; 
		world.RemovePhysicsBody(arrow);
	};

	
	Clock clock;
	Time lastTime(clock.getElapsedTime()); 
	Time currentTime(lastTime);

	long duckMS(0);
	while (arrows>0||drawingArrow) { 
		currentTime = clock.getElapsedTime(); 
		Time deltaTime = currentTime - lastTime;
		long deltaMS = deltaTime.asMilliseconds();
		if (deltaMS > 9) {
			duckMS = duckMS + deltaMS; 
			lastTime = currentTime;
			world.UpdatePhysics(deltaMS);
			if (Keyboard::isKeyPressed(Keyboard::Space) && !drawingArrow) {
				drawingArrow = true;
				arrows = arrows - 1;
				arrow.setCenter(crossbow.getCenter());
				arrow.setVelocity(Vector2f(0, -1));
				world.AddPhysicsBody(arrow);
			}

			window.clear();
			if (drawingArrow) {
				window.draw(arrow);
			}

			window.draw(crossbow);
			world.VisualizeAllBounds(window);
			window.display(); 
		}
		if (duckMS > 2000) {
			duckMS = 0; 
			PhysicsSprite& duck = ducks.Create();
			duck.setTexture(duckTex);
			duck.setCenter();
			duck.setVelocity();

		}
	}
}

