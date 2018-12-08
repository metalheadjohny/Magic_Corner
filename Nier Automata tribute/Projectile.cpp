#include "Projectile.h"
#include "Roboto.h"
#include "TileMapper.h"
#include <algorithm>

Projectile::Projectile(sf::Vector2f position, sf::Vector2f velocity, sf::Texture* t) : elapsed(0.f), vel(velocity){

	rs.setPosition(position);
	rs.setTexture(t);
	rs.setSize(sf::Vector2f(33.f, 33.f));
	rs.setOrigin(rs.getLocalBounds().width * 0.5f, rs.getLocalBounds().height * 0.5f);
	lifetime = 3.0f;
}


Projectile::~Projectile()
{
}


//abomination.... absolute abomination... but I have no time to implement partitioning
void Projectile::resolveCollision(std::vector<Roboto>& bots, const std::vector<InanimateObject>& tiles) {

	for (Roboto& r : bots) 
	{
		if (r.isDead()) continue;

		if (rs.getGlobalBounds().intersects(r.rs.getGlobalBounds())) {
			r.exterminate();
			dead = true;
			return;
		}
	}

	for (const InanimateObject& t : tiles) {
		if (t.rs.getGlobalBounds().intersects(rs.getGlobalBounds())) {
			dead = true;
			return;
		}
	}
}


void Projectile::update(float dTime) {

	elapsed += dTime;
	if (elapsed >= lifetime) {
		dead = true;
		return;
	}
	
	rs.move(vel);
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


void ProjectileManager::update(float dTime, std::vector<Roboto>& bots, const std::vector<InanimateObject>& tiles) {

	cooldown += dTime;

	for (int i = 0; i < bullets.size(); i++) {
		bullets.at(i).update(dTime);
		bullets.at(i).resolveCollision(bots, tiles);
	}
		
	
	bullets.erase(std::remove_if(bullets.begin(), bullets.end(), [](Projectile& b) { return b.isDead(); }), bullets.end());

}