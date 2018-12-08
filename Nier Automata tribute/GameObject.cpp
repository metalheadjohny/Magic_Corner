#include "GameObject.h"
#include "Roboto.h"

GameObject::GameObject()
{
}


GameObject::~GameObject()
{
}


void Player::OnNotify2b(const InputManager& iMan, const Event2B& event) {
	s2b.current = event;
}



void Player::OnNotify9s(const InputManager& iMan, const Event9S& cmd) {
	s9s.current = cmd;
}



void Player::Update(float dTime, std::vector<Roboto>& bots, const std::vector<InanimateObject>& tiles, MessageType pt)
{

	if (pt == MessageType::T_2B) {
		mouseDir = mousePos - posMin - sf::Vector2f(48.f, 48.f);
		mouseDir = mouseDir / sqrt(mouseDir.x * mouseDir.x + mouseDir.y * mouseDir.y);
		//float length = sqrt(mouseDir.x * mouseDir.x + mouseDir.y * mouseDir.y);	mouseDir.x /= length; mouseDir.y /= length;
	}
	

	//2b updates
	switch (s2b.current) {
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
	}

	posMin += velocity * dTime * s2b.speed;
	
	s2b.sprite.setPosition(posMin);
	s2b.desAndTroy.update(dTime, bots, tiles);

	switch (s9s.current) {

	case Event9S::CHILL9S:
		break;

	case Event9S::ROTATE_POS:
		s9s.rot -= s9s.ANGULAR_SPEED * dTime;
		break;

	case Event9S::ROTATE_NEG:
		s9s.rot += s9s.ANGULAR_SPEED * dTime;
		break;

	case Event9S::HACK:
		if(s9s.sinceHack >= s9s.HACK_CD)
			hack(bots);
		else
			std::cout << "CANT HACK YET" << std::endl;
		break;

	case Event9S::PUSH:
		if (s9s.sincePush >= s9s.PUSH_CD)
			push(bots);
		else
			std::cout << "CANT PUSH YET" << std::endl;
		break;

	case Event9S::PHASE:
		phase();
		break;
	}

	if (s2b.current != s2b.old) {
		if (s2b.current == Event2B::CHILL) {
			s2b.ssa = s2b.animap.at("idle");
			s2b.sprite.setScale(sf::Vector2f(1.0f, 1.0f));
		}
	}

	s2b.ssa.play(dTime);
	s2b.sprite.setScale(sf::Vector2f(86.0f / (float)s2b.ssa.cellSize.x, 86.0f / (float)s2b.ssa.cellSize.y));
	stateChanged2b = s2b.current == s2b.old ? false : true;
	s2b.old = s2b.current;


	s9s.offset9s =
		sf::Vector2f(
			cos(s9s.rot) * s9s.refDir.x - sin(s9s.rot) * s9s.refDir.y,
			sin(s9s.rot) * s9s.refDir.x + cos(s9s.rot) * s9s.refDir.y
		) * s9s.dist;

	s9s.sprite.setPosition(
		s2b.sprite.getPosition() +
		sf::Vector2f(s2b.sprite.getLocalBounds().width, s2b.sprite.getLocalBounds().height)  * 0.5f +
		s9s.offset9s);
	s9s.ssa = s9s.animap.at("9s_idle");
	s9s.sprite.setScale(sf::Vector2f(48.0f / (float)s9s.ssa.cellSize.x, 48.0f / (float)s9s.ssa.cellSize.y));
	s9s.ssa.play(dTime);
	stateChanged9s = s9s.current == s9s.old ? false : true;
	s9s.old = s9s.current;

	s9s.sinceHack += dTime;
	s9s.sincePush += dTime;
}



int Player::hack(std::vector<Roboto>& bots) {
	
	sf::Vector2f hdNormalized = s9s.offset9s / s9s.dist;	//I think this is normalized now
	
	std::vector<std::pair<int, float>> indicesToHack;
	for (int i = 0; i < bots.size(); i++) {

		sf::Vector2f playerToBot = bots[i].rs.getPosition() - s9s.sprite.getPosition();
		float ptbDistSquared = playerToBot.x * playerToBot.x + playerToBot.y * playerToBot.y;

		if (ptbDistSquared > s9s.HACK_RANGE * s9s.HACK_RANGE)
			continue;
		
		float ptbDist = sqrt(ptbDistSquared);
		playerToBot = playerToBot / ptbDist;
		if (playerToBot.x * hdNormalized.x + playerToBot.y * hdNormalized.y < s9s.HACK_DOT_LMIT)
			continue;

		indicesToHack.push_back(std::make_pair(i, ptbDist));
	}

	if (indicesToHack.empty())
		return -1;

	float minIndex = indicesToHack[0].first;
	float minDist = indicesToHack[0].second;

	for (int i = 1; i < indicesToHack.size(); i++) {
		if (indicesToHack[i].second < minDist) {
			minDist = indicesToHack[i].second;
			minIndex = indicesToHack[i].first;
		}
	}

	bots[minIndex].hacked = true;
	s9s.sinceHack = 0.f;

	return minIndex;
}



std::vector<int> Player::push(std::vector<Roboto>& bots)
{
	sf::Vector2f hdNormalized = s9s.offset9s / s9s.dist;

	std::vector<std::pair<int, sf::Vector2f>> indicesToPush;
	for (int i = 0; i < bots.size(); i++) {

		sf::Vector2f playerToBot = bots[i].rs.getPosition() - s9s.sprite.getPosition();
		float ptbDistSquared = playerToBot.x * playerToBot.x + playerToBot.y * playerToBot.y;

		if (ptbDistSquared > s9s.PUSH_RANGE * s9s.PUSH_RANGE)
			continue;
		
		playerToBot = playerToBot / sqrt(ptbDistSquared);
		if (playerToBot.x * hdNormalized.x + playerToBot.y * hdNormalized.y < s9s.PUSH_DOT_LIMIT)
			continue;

		indicesToPush.push_back(std::make_pair(i, playerToBot));
	}

	for (auto p : indicesToPush)
		bots[p.first].yeet(p.second);

	s9s.sincePush = 0.f;

	std::vector<int> result;
	for (auto itp : indicesToPush)
		result.push_back(itp.first);

	return result;
}



void Player::phase() {


}