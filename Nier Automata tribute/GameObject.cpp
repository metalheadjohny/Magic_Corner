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
			if(!isJumping)
				velocity = sf::Vector2f(0.f, 0.f);
			break;
		case Event::DODGE:
			velocity = sf::Vector2f(0.f, 0.f);
			//play dodge anim aka fade out or some shit
			break;
		case Event::MOVE_LEFT:
			velocity = sf::Vector2f(-1.f, 0.f);
			break;
		case Event::MOVE_RIGHT:
			velocity = sf::Vector2f(1.f, 0.f);
			break;
		case Event::JUMP:
			velocity = sf::Vector2f(0.0f, -1.0f);
			//make this not look like shit... idk...
			break;
		case Event::MELEE_ATTACK:
			//play anim, do shit
			break;
		case Event::RANGED_ATTACK:

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
		
	posMin += velocity * dTime * speed + sf::Vector2f(0.0f, 1.0f) * gravity * dTime * 10.f; //
	sprite.setPosition(posMin);

	if (current == Event::JUMP || isJumping) {
		Jump(dTime);
	}

	if (current == Event::MOVE_LEFT) {
		ssa = animap.at("walk_left");
		if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Space))
			Jump(dTime);
	}

	if (current == Event::MOVE_RIGHT) {
		ssa = animap.at("walk_right");
		if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Space))
			Jump(dTime);
	}

	if (current != old) {
		if (current == Event::CHILL) {
			ssa = animap.at("idle");
		}
	}
	old = current;
	ssa.play(dTime);



	//animation.sprite.setPosition(posMin);
	//animation.play(cat, dTime);
}



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
