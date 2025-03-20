#include <iostream>
#include <fstream>
#include <vector>
#include <cstdlib>
#include <ctime>

struct Player
{
    std::string name;
    int pokeballs = 5;
    int money = 100;
    int level = 1;
    int potions = 2;
    int spells = 1;
    std::vector<std::string> caught_pokemon;
};

struct Pokemon {
    std::string name;
    int level;
    int health;

    Pokemon(std::string n, int lvl) : name(n), level(lvl), health(lvl * 10) {}
};

std::vector<std::string> read_pokemon_names()
{
    std::vector<std::string> pokemons;
    std::ifstream file("pokemons.txt");
    std::string pokemon;
    while (std::getline(file, pokemon))
    {
        pokemons.push_back(pokemon);
    }
    return pokemons;
}

void catch_pokemon(Player &player)
{
    std::vector<std::string> pokemons = read_pokemon_names();
    std::string pokemon = pokemons[rand() % pokemons.size()];
    std::cout << "A wild " << pokemon << " appears!" << std::endl;
    std::cout << "Enter 'C' to catch or 'R' to run: ";
    char action;
    std::cin >> action;
    if (action == 'C' || action == 'c')
    {
        if (player.pokeballs > 0)
        {
            player.pokeballs--;
            if (static_cast<double>(rand()) / RAND_MAX < 0.5)
            {
                std::cout << "You caught the " << pokemon << "!" << std::endl;
                player.caught_pokemon.push_back(pokemon);
            }
            else
            {
                std::cout << pokemon << " escaped!" << std::endl;
            }
        }
        else
        {
            std::cout << "You don't have any pokeballs!" << std::endl;
        }
    }
    else
    {
        std::cout << "You ran away!" << std::endl;
    }
}

void visit_shop(Player &player)
{
    std::cout << "Welcome to the Shop!" << std::endl;
    std::cout << "1. Pokeballs: $10 each\n2. Potions: $15 each\n3. Spells: $20 each" << std::endl;
    int choice, quantity;
    std::cout << "Enter your choice (1-3): ";
    std::cin >> choice;
    std::cout << "Enter quantity: ";
    std::cin >> quantity;

    int cost = 0;
    if (choice == 1)
        cost = 10 * quantity;
    else if (choice == 2)
        cost = 15 * quantity;
    else if (choice == 3)
        cost = 20 * quantity;
    else
    {
        std::cout << "Invalid choice!" << std::endl;
        return;
    }

    if (player.money >= cost)
    {
        player.money -= cost;
        if (choice == 1)
            player.pokeballs += quantity;
        else if (choice == 2)
            player.potions += quantity;
        else if (choice == 3)
            player.spells += quantity;
        std::cout << "Purchase successful!" << std::endl;
    }
    else
    {
        std::cout << "Not enough money!" << std::endl;
    }
}

void battle(Player &player)
{
    if (player.caught_pokemon.empty())
    {
        std::cout << "You don't have any Pokemon to battle!" << std::endl;
        return;
    }

    // Generate opponent Pokemon
    std::vector<std::string> pokemons = read_pokemon_names();
    if (pokemons.empty())
    {
        std::cout << "No opponent Pokemon available!" << std::endl;
        return;
    }

    std::string opponent_name = pokemons[rand() % pokemons.size()];
    int opponent_level = rand() % (player.level + 3) + 1;
    Pokemon opponent(opponent_name, opponent_level);
    
    std::cout << "A wild " << opponent.name << " (Level " << opponent.level << ") appears!" << std::endl;
    
    // Choose player's Pokemon
    std::cout << "Choose a Pokemon to fight with (1-" << player.caught_pokemon.size() << "): " << std::endl;
    for (size_t i = 0; i < player.caught_pokemon.size(); i++)
    {
        std::cout << i + 1 << ". " << player.caught_pokemon[i] << std::endl;
    }

    int choice;
    std::cin >> choice;

    if (choice < 1 || choice > player.caught_pokemon.size())
    {
        std::cout << "Invalid choice!" << std::endl;
        return;
    }

    Pokemon myPokemon(player.caught_pokemon[choice - 1], player.level);

    while (myPokemon.health > 0 && opponent.health > 0)
    {
        std::cout << "\nYour " << myPokemon.name << " (HP: " << myPokemon.health << ") vs. " << opponent.name << " (HP: " << opponent.health << ")" << std::endl;
        std::cout << "Enter 'A' to attack, 'P' to use a potion, 'S' to use a spell: ";
        
        char action;
        std::cin >> action;

        switch (action)
        {
            case 'A':
            case 'a':
            {
                int damage = (rand() % myPokemon.level) + 2;
                opponent.health -= damage;
                std::cout << myPokemon.name << " attacks! " << opponent.name << " takes " << damage << " damage!" << std::endl;
                break;
            }
            case 'P':
            case 'p':
            {
                if (player.potions > 0)
                {
                    int heal = (rand() % 5) + 5;
                    myPokemon.health += heal;
                    player.potions--;
                    std::cout << myPokemon.name << " is healed by " << heal << " HP!" << std::endl;
                }
                else
                {
                    std::cout << "You have no potions left!" << std::endl;
                }
                break;
            }
            case 'S':
            case 's':
            {
                if (player.spells > 0)
                {
                    int spell_damage = (rand() % 10) + 5;
                    opponent.health -= spell_damage;
                    player.spells--;
                    std::cout << "You cast a spell! " << opponent.name << " takes " << spell_damage << " damage!" << std::endl;
                }
                else
                {
                    std::cout << "You have no spells left!" << std::endl;
                }
                break;
            }
            default:
            {
                std::cout << "Invalid action!" << std::endl;
                continue;
            }
        }

        // Opponent attacks
        if (opponent.health > 0)
        {
            int damage = (rand() % opponent.level) + 2;
            myPokemon.health -= damage;
            std::cout << opponent.name << " attacks! " << myPokemon.name << " takes " << damage << " damage!" << std::endl;
        }
    }

    if (myPokemon.health > 0)
    {
        std::cout << "You won the battle!" << std::endl;
        player.money += opponent.level * 10;
        player.level++;

        // Level up Pokémon
        myPokemon.level++;
        std::cout << myPokemon.name << " leveled up to " << myPokemon.level << "!" << std::endl;
    }
    else
    {
        std::cout << "You lost the battle!" << std::endl;
    }
}

void play_game()
{
    Player player;
    std::cout << "Enter your player name: ";
    std::cin >> player.name;

    while (player.level <= 10)
    {
        std::cout << "\n----- Level " << player.level << " -----" << std::endl;
        std::cout << "Player: " << player.name << " | Pokeballs: " << player.pokeballs << " | Money: $" << player.money << " | Potions: " << player.potions << " | Spells: " << player.spells << std::endl;
        std::cout << "Enter 'C' to catch, 'S' to shop, 'B' to battle, or 'Q' to quit: ";
        char action;
        std::cin >> action;

        if (action == 'C' || action == 'c')
            catch_pokemon(player);
        else if (action == 'S' || action == 's')
            visit_shop(player);
        else if (action == 'B' || action == 'b')
            battle(player);
        else if (action == 'Q' || action == 'q')
        {
            std::cout << "Thanks for playing!" << std::endl;
            break;
        }
        else
        {
            std::cout << "Invalid input. Try again!" << std::endl;
        }
    }
}

int main()
{
    std::srand(static_cast<unsigned int>(std::time(nullptr)));
    std::cout << "Welcome to the Pokemon Game!" << std::endl;
    play_game();
    return 0;
}