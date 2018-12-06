#pragma once
#include <SFML/Graphics.hpp>
#include <queue>

class Roboto;
class InanimateObject;

class Projectile{

	float lifetime, elapsed;
	sf::Vector2f vel;
	bool dead = false;

public:
	Projectile(sf::Vector2f position, sf::Vector2f velocity, sf::Texture* t);
	~Projectile();

	bool isDead() { return dead; }
	void update(float dTime);
	void resolveCollision(std::vector<Roboto>& bots, const std::vector<InanimateObject>& tiles);
	void setTexture(sf::Texture* t);
	sf::RectangleShape rs;
};


class ProjectileManager {

	sf::Texture bulletTex;
	float cooldown;
public:
	
	ProjectileManager();
	std::deque <Projectile> bullets;
	float rof;

	void shoot(sf::Vector2f position, sf::Vector2f normalizedDir, float s, float offset = 10.f);
	void update(float dTime, std::vector<Roboto>& bots, const std::vector<InanimateObject>& tiles);
};

