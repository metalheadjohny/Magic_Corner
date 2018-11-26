#include "Projectile.h"
#include <algorithm>


Projectile::Projectile(sf::Vector2f position, sf::Vector2f velocity, sf::Texture* t) : elapsed(0.f), vel(velocity){

	rs.setPosition(position);
	rs.setTexture(t);
	rs.setSize(sf::Vector2f(33.f, 33.f));
	lifetime = 3.0f;
}


Projectile::~Projectile()
{
}


void Projectile::update(float dTime) {
	rs.move(vel);
	elapsed += dTime;
	if (elapsed >= lifetime)
		dead = true;
}

void Projectile::setTexture(sf::Texture* t){
	rs.setTexture(t);
}


ProjectileManager::ProjectileManager() {
	bulletTex.loadFromFile("../Assets/projectile.png");
	cooldown = 0.f;
	rof = 0.1f;
}


void ProjectileManager::shoot(sf::Vector2f position, sf::Vector2f normalizedDir, float s, float offset) {
	
	if (cooldown >= rof) {
		cooldown = 0.f;
		Projectile p(position + normalizedDir * offset, normalizedDir * s, &bulletTex);
		bullets.push_back(p);
	}
}


void ProjectileManager::update(float dTime) {

	cooldown += dTime;

	for (int i = 0; i < bullets.size(); i++)
		bullets.at(i).update(dTime);
	
	bullets.erase(std::remove_if(bullets.begin(), bullets.end(), [](Projectile& b) { return b.isDead(); }), bullets.end());

}