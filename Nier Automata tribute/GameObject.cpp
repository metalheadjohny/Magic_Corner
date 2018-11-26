#include "GameObject.h"

GameObject::GameObject()
{
}


GameObject::~GameObject()
{
}


void Player::OnNotify(const InputManager& iMan, const Event& event) {

	current = event;
	switch (event) {
		case Event::CHILL:
			velocity = sf::Vector2f(0.f, 0.f);
			break;
		case Event::DODGE:
			velocity = sf::Vector2f(0.f, 0.f);
			//play dodge anim aka fade out or some shit
			break;
		case Event::MOVE_LEFT:
			velocity = sf::Vector2f(-1.f, 0.f);
			rot = -1.f;
			break;
		case Event::MOVE_UP:
			velocity = sf::Vector2f(0.f, -1.f);
			break;
		case Event::MOVE_DOWN:
			velocity = sf::Vector2f(0.f, 1.f);
			break;
		case Event::MOVE_RIGHT:
			velocity = sf::Vector2f(1.f, 0.f);
			rot = 1.f;
			break;
		case Event::MELEE_ATTACK:
			//play anim, do shit
			break;
		case Event::RANGED_ATTACK:
			velocity = sf::Vector2f(0.0f, 0.0f);
			desAndTroy.shoot(posMin + sf::Vector2f(sprite.getLocalBounds().width  * 0.5f, sprite.getLocalBounds().height  * 0.5f),
				sf::Vector2f(mouseDir.x, mouseDir.y), 5, 50);
			break;
		case Event::SELF_DESTRUCT:

			break;
		case Event::ULTIMATE:

			break;

		default:
			std::cout << "Unknown command received!" << std::endl;
			break;
	}
}



void Player::Update(float dTime){
		
	posMin += velocity * dTime * speed;
	sprite.setPosition(posMin);

	mouseDir = mousePos - posMin;
	float length = sqrt(mouseDir.x * mouseDir.x + mouseDir.y * mouseDir.y);
	mouseDir.x /= length;
	mouseDir.y /= length;
	desAndTroy.update(dTime);

	if (current == Event::MOVE_LEFT) {
		ssa = animap.at("walk_left");
	}

	if (current == Event::MOVE_RIGHT) {
		ssa = animap.at("walk_right");
	}

	if (current == Event::RANGED_ATTACK) {
		ssa = animap.at("ranged_attack");
	}

	if (current != old) {
		if (current == Event::CHILL) {
			ssa = animap.at("idle");
			sprite.setScale(sf::Vector2f(1.0f, 1.0f));
		}
	}
	old = current;
	ssa.play(dTime);

	sprite.setScale(sf::Vector2f(86.0f / (float)ssa.cellSize.x, 86.0f / (float)ssa.cellSize.y));
}


/*
void Player::Jump(float dTime) {

	isJumping = true;
	elapsed += dTime;

	posMin += velocity * 15.f * gravity * (1 - elapsed) * dTime;
	sprite.setPosition(posMin);

	if (elapsed >= jumpDuration) {
		isJumping = false;
		elapsed = 0.0f;
	}

}
*/