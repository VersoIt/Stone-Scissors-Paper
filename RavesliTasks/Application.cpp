#include <iostream>
#include <vector>
#include <algorithm>
#include <ctime>
 
// I did not distribute each individual class into header files, because this game is simple and I'm too lazy)

class GameItem
{
public:

	enum class Types
	{
		DEFAULT,
		STONE,
		SCISSORS,
		PAPER,
		DIFFICULT
	};

	GameItem(Types type = Types::DEFAULT) : m_type{ type }
	{
	}

	/// <summary>
	/// Returns current type of object.
	/// </summary>
	/// <returns></returns>
	Types getType()
	{
		return m_type;
	}

	/// <summary>
	/// Returns true, if current object wins another.
	/// </summary>
	/// <param name="gameItem"></param>
	/// <returns></returns>
	virtual bool fight(const GameItem& gameItem) const
	{
		for (size_t i = 0; i < gameItem.getBeatableObjects().size(); ++i)
		{
			if (gameItem.getBeatableObjects()[i] == m_type) return false;
		}

		return true;
	}

	/// <summary>
	/// Returns types of objects, which current object can win.
	/// </summary>
	/// <returns></returns>
	const std::vector<Types>& getBeatableObjects() const
	{
		return m_beatable;
	}

protected:

	std::vector<Types> m_beatable;

	Types m_type;
};

/// <summary>
/// It implies a game object that can influence the outcome of the game.
/// </summary>
class GameEntity : public GameItem
{
public:
	GameEntity(Types type, const std::string& name) : GameItem(type), m_name{ name }
	{
	}

	const std::string& getName() const
	{
		return m_name;
	}

protected:
	std::string m_name;
};

class Stone : public GameEntity
{
public:
	Stone(const std::string& name) : GameEntity(Types::STONE, name)
	{
		m_beatable.push_back(Types::SCISSORS);
	}

};

class Scissors : public GameEntity
{
public:
	Scissors(const std::string& name) : GameEntity(Types::SCISSORS, name)
	{
		m_beatable.push_back(Types::PAPER);
	}

};

class Random
{
public:

	template <typename T>
	static T getRandomItemFromVector(const std::vector<T>& items)
	{
		return items[abs(static_cast<int>(rand() % items.size()))];
	}
};

class Paper : public GameEntity
{
public:
	Paper(const std::string& name) : GameEntity(Types::PAPER, name)
	{
		m_beatable.push_back(Types::STONE);
	}

};

class LanguageLocale
{
public:
	static void SetRussian()
	{
		setlocale(LC_ALL, "Russian");
	}
};

int main(int argc, char* argv[])
{
	srand(time(0));

	Stone* stone = new Stone("Stone");
	Paper* paper = new Paper("Paper");
	Scissors* scissors = new Scissors("Scissors");

	// Интерфейс для пользователя
	std::cout << "What do you want to select?" << std::endl;
	std::cout << "1 - Stone" << std::endl;
	std::cout << "2 - Scissors" << std::endl;
	std::cout << "3 - Paper" << std::endl;

	size_t selectedItemType{ 0 };

	GameEntity* player = nullptr;
	std::vector<GameEntity*> gameEntites{ stone, scissors, paper };

	bool isSelected{ false };
	while (!isSelected)
	{
		std::cout << "Enter: ";
		std::cin >> selectedItemType;

		// Checking if the selected value of the element type is in the entire range of types.
		if ((selectedItemType > static_cast<size_t>(GameItem::Types::DEFAULT)) 
			&& (selectedItemType < static_cast<size_t>(GameItem::Types::DIFFICULT)))
		{
			isSelected = true;
			player = gameEntites[selectedItemType - 1];

			std::cout << "You selected: " << player->getName() << std::endl;
		}
		else std::cout << "Entered incorrect value!" << std::endl;
	}

	auto secondEntity = Random::getRandomItemFromVector<GameEntity*>(gameEntites);

	std::cout << "------------------------------ " << player->getName() << " <-> " << secondEntity->getName() << " ------------------------------" << std::endl;
	std::cout << "Result of the game: ";
	std::cout << ((player->getType() == secondEntity->getType()) ? "Draw" : player->fight(*secondEntity) ? "The player who chose the " + player->getName() + " wins!" : "The bot who chose the " + secondEntity->getName() + " wins!") << std::endl;

	std::for_each(gameEntites.begin(), gameEntites.end(), [](auto e) {delete e; });
}