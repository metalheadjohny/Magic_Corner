#include "TileMapper.h"


TileMapper::TileMapper(){

	sf::Texture t;
	t.loadFromFile(TILEPATH + "4.png");
	texMap.insert(std::make_pair('1', t));

	//t.loadFromFile(TILEPATH + "");
	texMap.insert(std::make_pair('2', t));

	t.loadFromFile(TILEPATH + "12.png");
	texMap.insert(std::make_pair('3', t));

	t.loadFromFile(TILEPATH + "13.png");
	texMap.insert(std::make_pair('4', t));

	t.loadFromFile(TILEPATH + "16.png");
	texMap.insert(std::make_pair('5', t));

}



TileMapper::~TileMapper()
{
}
