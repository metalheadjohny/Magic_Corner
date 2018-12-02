#include "GameObject.h"

GameObject::GameObject()
{
}


GameObject::~GameObject()
{
}


void Player::OnNotify2b(const InputManager& iMan, const Event2B& event) {

	s2b.current = event;

	if (s2b.current != s2b.old)
		stateChanged2b = true;

	switch (event) {
		case Event2B::CHILL:
			velocity = sf::Vector2f(0.f, 0.f);
			break;
		case Event2B::DODGE:
			velocity = sf::Vector2f(0.f, 0.f);
			break;
		case Event2B::MOVE_LEFT:
			velocity = sf::Vector2f(-1.f, 0.f);
			s2b.ssa = s2b.animap.at("walk_left");
			s2b.rot = -1.f;
			break;
		case Event2B::MOVE_UP:
			velocity = sf::Vector2f(0.f, -1.f);
			break;
		case Event2B::MOVE_DOWN:
			velocity = sf::Vector2f(0.f, 1.f);
			break;
		case Event2B::MOVE_RIGHT:
			velocity = sf::Vector2f(1.f, 0.f);
			s2b.ssa = s2b.animap.at("walk_right");
			s2b.rot = 1.f;
			break;
		case Event2B::MELEE_ATTACK:
			//play anim, do shit
			break;
		case Event2B::RANGED_ATTACK:
			velocity = sf::Vector2f(0.0f, 0.0f);
			s2b.ssa = s2b.animap.at("ranged_attack");
			s2b.desAndTroy.shoot(posMin + sf::Vector2f(s2b.sprite.getLocalBounds().width  * 0.5f, s2b.sprite.getLocalBounds().height  * 0.5f),
				sf::Vector2f(mouseDir.x, mouseDir.y), 5, 50);
			break;
		default:
			std::cout << "Unknown command received!" << std::endl;
			break;
	}
}



void Player::OnNotify9s(const InputManager& iMan, const Event9s& cmd) {

	s9s.current = cmd;
	switch (cmd) {
	default:
		std::cout << "Unknown command received!" << std::endl;
		break;
	}
}



void Player::Update(float dTime){

	//general updates
	posMin += velocity * dTime * s2b.speed;
	mouseDir = mousePos - posMin;
	mouseDir = mouseDir / sqrt(mouseDir.x * mouseDir.x + mouseDir.y * mouseDir.y);
	//float length = sqrt(mouseDir.x * mouseDir.x + mouseDir.y * mouseDir.y);	mouseDir.x /= length; mouseDir.y /= length;

	//2b updates
	s2b.sprite.setPosition(posMin);
	s2b.desAndTroy.update(dTime);

	if (s2b.current != s2b.old) {
		if (s2b.current == Event2B::CHILL) {
			s2b.ssa = s2b.animap.at("idle");
			s2b.sprite.setScale(sf::Vector2f(1.0f, 1.0f));
		}
	}
	s2b.old = s2b.current;
	s2b.ssa.play(dTime);

	s2b.sprite.setScale(sf::Vector2f(86.0f / (float)s2b.ssa.cellSize.x, 86.0f / (float)s2b.ssa.cellSize.y));



	//9s updates
	if (s9s.current == Event9s::ROTATE_NEG)
		s9s.rot += s9s.ANGULAR_SPEED * dTime;
	
	if (s9s.current == Event9s::ROTATE_POS)
		s9s.rot -= s9s.ANGULAR_SPEED * dTime;

	sf::Vector2f offset9s = 
		sf::Vector2f(
			cos(s9s.rot) * s9s.refDir.x - sin(s9s.rot) * s9s.refDir.y,
			sin(s9s.rot) * s9s.refDir.x + cos(s9s.rot) * s9s.refDir.y
		) * s9s.dist;

	s9s.sprite.setPosition(
		s2b.sprite.getPosition() +
		sf::Vector2f(s2b.sprite.getLocalBounds().width, s2b.sprite.getLocalBounds().height)  * 0.5f +
		offset9s);
	s9s.ssa = s9s.animap.at("9s_idle");
	s9s.sprite.setScale(sf::Vector2f(48.0f / (float)s9s.ssa.cellSize.x, 48.0f / (float)s9s.ssa.cellSize.y));
	s9s.ssa.play(dTime);
	s9s.old = s9s.current;
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