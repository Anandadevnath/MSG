#include <iostream>
#include <fstream>
#include <vector>
#include <cstdlib>
#include <ctime>
#include <map>
#include <algorithm>
#include <cctype>

using namespace std;

// User Account System
struct User {
    string username;
    string password;
    bool isLoggedIn = false;
};

map<string, User> users;

// Save users to file
void saveUsers() {
    ofstream file("users.txt");
    for (const auto& pair : users) {
        file << pair.second.username << " " << pair.second.password << "\n";
    }
}

// Load users from file
void loadUsers() {
    ifstream file("users.txt");
    if (!file) return;

    User user;
    while (file >> user.username >> user.password) {
        users[user.username] = user;
    }
}

// Register a new user
void registerUser() {
    User newUser;
    cout << "Enter username: ";
    cin >> newUser.username;

    if (users.find(newUser.username) != users.end()) {
        cout << "Username already exists!\n";
        return;
    }

    cout << "Enter password: ";
    cin >> newUser.password;

    users[newUser.username] = newUser;
    saveUsers();
    cout << "Registration successful!\n";
}

// Login user
User* loginUser() {
    string username, password;
    cout << "Enter username: ";
    cin >> username;
    cout << "Enter password: ";
    cin >> password;

    auto it = users.find(username);
    if (it != users.end() && it->second.password == password) {
        it->second.isLoggedIn = true;
        cout << "Login successful!\n";
        return &it->second;
    }

    cout << "Invalid username or password!\n";
    return nullptr;
}

// Logout user
void logoutUser(User* user) {
    if (user) {
        user->isLoggedIn = false;
        cout << "Logged out successfully!\n";
    }
}

// ================== POKEMON GAME ==================
struct Player {
    string name;
    int pokeballs = 5;
    int money = 100;
    int level = 1;
    int potions = 2;
    int spells = 1;
    vector<string> caught_pokemon;
};

struct Pokemon {
    string name;
    int level;
    int health;

    Pokemon(string n, int lvl) : name(n), level(lvl), health(lvl * 10) {}
};

vector<string> read_pokemon_names() {
    vector<string> pokemons;
    ifstream file("pokemons.txt");
    string pokemon;
    while (getline(file, pokemon)) {
        pokemons.push_back(pokemon);
    }
    return pokemons;
}

void catch_pokemon(Player &player) {
    vector<string> pokemons = read_pokemon_names();
    string pokemon = pokemons[rand() % pokemons.size()];
    cout << "\nA wild " << pokemon << " appears!\n" << endl;
    cout << "Enter 'C' to catch or 'R' to run: ";
    char action;
    cin >> action;
    if (action == 'C' || action == 'c') {
        if (player.pokeballs > 0) {
            player.pokeballs--;
            if (static_cast<double>(rand()) / RAND_MAX < 0.5) {
                cout << "You caught the " << pokemon << "!\n" << endl;
                player.caught_pokemon.push_back(pokemon);
            } else {
                cout << pokemon << " escaped!\n" << endl;
            }
        } else {
            cout << "You don't have any pokeballs!\n" << endl;
        }
    } else {
        cout << "You ran away!\n" << endl;
    }
}

void visit_shop(Player &player) {
    cout << "\nWelcome to the Shop!\n" << endl;
    cout << "1. Pokeballs: $10 each\n2. Potions: $15 each\n3. Spells: $20 each" << endl;
    int choice, quantity;
    cout << "Enter your choice (1-3): ";
    cin >> choice;
    cout << "Enter quantity: ";
    cin >> quantity;

    int cost = 0;
    if (choice == 1) cost = 10 * quantity;
    else if (choice == 2) cost = 15 * quantity;
    else if (choice == 3) cost = 20 * quantity;
    else {
        cout << "Invalid choice!\n" << endl;
        return;
    }

    if (player.money >= cost) {
        player.money -= cost;
        if (choice == 1) player.pokeballs += quantity;
        else if (choice == 2) player.potions += quantity;
        else if (choice == 3) player.spells += quantity;
        cout << "Purchase successful!\n" << endl;
    } else {
        cout << "Not enough money!\n" << endl;
    }
}

void battle(Player &player) {
    if (player.caught_pokemon.empty()) {
        cout << "You don't have any Pokemon to battle!\n" << endl;
        return;
    }

    vector<string> pokemons = read_pokemon_names();
    if (pokemons.empty()) {
        cout << "No opponent Pokemon available!\n" << endl;
        return;
    }

    string opponent_name = pokemons[rand() % pokemons.size()];
    int opponent_level = rand() % (player.level + 3) + 1;
    Pokemon opponent(opponent_name, opponent_level);
    
    cout << "\nA wild " << opponent.name << " (Level " << opponent.level << ") appears!\n" << endl;
    cout << "Choose a Pokemon to fight with (1-" << player.caught_pokemon.size() << "): " << endl;
    for (size_t i = 0; i < player.caught_pokemon.size(); i++) {
        cout << i + 1 << ". " << player.caught_pokemon[i] << endl;
    }

    int choice;
    cin >> choice;

    if (choice < 1 || choice > player.caught_pokemon.size()) {
        cout << "Invalid choice!\n" << endl;
        return;
    }

    Pokemon myPokemon(player.caught_pokemon[choice - 1], player.level);

    while (myPokemon.health > 0 && opponent.health > 0) {
        cout << "\nYour " << myPokemon.name << " (HP: " << myPokemon.health << ") vs. " << opponent.name << " (HP: " << opponent.health << ")" << endl;
        cout << "Enter 'A' to attack, 'P' to use a potion, 'S' to use a spell: ";
        
        char action;
        cin >> action;

        switch (action) {
            case 'A': case 'a': {
                int damage = (rand() % myPokemon.level) + 2;
                opponent.health -= damage;
                cout << myPokemon.name << " attacks! " << opponent.name << " takes " << damage << " damage!\n" << endl;
                break;
            }
            case 'P': case 'p': {
                if (player.potions > 0) {
                    int heal = (rand() % 5) + 5;
                    myPokemon.health += heal;
                    player.potions--;
                    cout << myPokemon.name << " is healed by " << heal << " HP!\n" << endl;
                } else {
                    cout << "You have no potions left!\n" << endl;
                }
                break;
            }
            case 'S': case 's': {
                if (player.spells > 0) {
                    int spell_damage = (rand() % 10) + 5;
                    opponent.health -= spell_damage;
                    player.spells--;
                    cout << "You cast a spell! " << opponent.name << " takes " << spell_damage << " damage!\n" << endl;
                } else {
                    cout << "You have no spells left!\n" << endl;
                }
                break;
            }
            default: {
                cout << "Invalid action!\n" << endl;
                continue;
            }
        }

        // Opponent attacks
        if (opponent.health > 0) {
            int damage = (rand() % opponent.level) + 2;
            myPokemon.health -= damage;
            cout << opponent.name << " attacks! " << myPokemon.name << " takes " << damage << " damage!\n" << endl;
        }
    }

    if (myPokemon.health > 0) {
        cout << "You won the battle!\n" << endl;
        player.money += opponent.level * 10;
        player.level++;
        myPokemon.level++;
        cout << myPokemon.name << " leveled up to " << myPokemon.level << "!\n" << endl;
    } else {
        cout << "You lost the battle!\n" << endl;
    }
}

void play_pokemon_game(User* user) {
    Player player;
    player.name = user->username;

    while (player.level <= 10) {
        cout << "\n----- Level " << player.level << " -----" << endl;
        cout << "Player: " << player.name << " | Pokeballs: " << player.pokeballs 
             << " | Money: $" << player.money << " | Potions: " << player.potions 
             << " | Spells: " << player.spells << endl;
        cout << "Enter 'C' to catch, 'S' to shop, 'B' to battle, or 'Q' to quit: ";
        char action;
        cin >> action;

        if (action == 'C' || action == 'c') catch_pokemon(player);
        else if (action == 'S' || action == 's') visit_shop(player);
        else if (action == 'B' || action == 'b') battle(player);
        else if (action == 'Q' || action == 'q') {
            cout << "Thanks for playing!\n" << endl;
            break;
        } else {
            cout << "Invalid input. Try again!\n" << endl;
        }
    }
}

// ================== TIC-TAC-TOE GAME ==================
void tictactoe_board(char *space) {
    cout << "     |     |     \n";
    cout << "  " << space[0] << "  |  " << space[1] << "  |  " << space[2] << "  \n";
    cout << "_____|_____|_____\n";
    cout << "     |     |     \n";
    cout << "  " << space[3] << "  |  " << space[4] << "  |  " << space[5] << "  \n";
    cout << "_____|_____|_____\n";
    cout << "     |     |     \n";
    cout << "  " << space[6] << "  |  " << space[7] << "  |  " << space[8] << "  \n";
    cout << "     |     |     \n\n";
}

void tictactoe_player_move(char *space, char player) {
    int num;
    do {
        cout << "Enter a number (1-9): ";
        cin >> num;
        num--;
        if (num >= 0 && num < 9 && space[num] == ' ') {
            space[num] = player;
            break;
        }
        cout << "Invalid move!\n";
    } while (true);
}

void tictactoe_computer_move(char *space, char computer) {
    int num;
    while (true) {
        num = rand() % 9;
        if (space[num] == ' ') {
            space[num] = computer;
            break;
        }
    }
}

bool tictactoe_check_winner(char *space, char player, char computer) {
    // Check rows
    for (int i = 0; i < 9; i += 3) {
        if (space[i] != ' ' && space[i] == space[i+1] && space[i+1] == space[i+2]) {
            space[i] == player ? cout << "You win!\n" : cout << "You lose!\n";
            return true;
        }
    }
    
    // Check columns
    for (int i = 0; i < 3; i++) {
        if (space[i] != ' ' && space[i] == space[i+3] && space[i+3] == space[i+6]) {
            space[i] == player ? cout << "You win!\n" : cout << "You lose!\n";
            return true;
        }
    }
    
    // Check diagonals
    if (space[0] != ' ' && space[0] == space[4] && space[4] == space[8]) {
        space[0] == player ? cout << "You win!\n" : cout << "You lose!\n";
        return true;
    }
    if (space[2] != ' ' && space[2] == space[4] && space[4] == space[6]) {
        space[2] == player ? cout << "You win!\n" : cout << "You lose!\n";
        return true;
    }
    
    return false;
}

bool tictactoe_check_tie(char *space) {
    for (int i = 0; i < 9; i++) {
        if (space[i] == ' ') return false;
    }
    cout << "It's a tie!\n";
    return true;
}

void play_tic_tac_toe(User* user) {
    char space[9] = {' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' '};
    char player = 'X';
    char computer = 'O';
    bool game_over = false;

    cout << "\nWelcome to Tic-Tac-Toe, " << user->username << "!\n";
    tictactoe_board(space);

    while (!game_over) {
        tictactoe_player_move(space, player);
        tictactoe_board(space);
        
        if (tictactoe_check_winner(space, player, computer)) {
            game_over = true;
            break;
        }
        if (tictactoe_check_tie(space)) {
            game_over = true;
            break;
        }

        tictactoe_computer_move(space, computer);
        tictactoe_board(space);
        
        if (tictactoe_check_winner(space, player, computer)) {
            game_over = true;
            break;
        }
        if (tictactoe_check_tie(space)) {
            game_over = true;
            break;
        }
    }
}

// ================== HANGMAN GAME ==================
void play_hangman(User* user) {
    vector<string> words = {"programming", "hangman", "computer", "keyboard", "algorithm", "developer"};
    string word = words[rand() % words.size()];
    string guessed(word.length(), '_');
    int attempts = 6;
    vector<char> incorrect;

    cout << "\nWelcome to Hangman, " << user->username << "!\n";
    
    while (attempts > 0 && guessed != word) {
        cout << "\nWord: " << guessed << endl;
        cout << "Attempts left: " << attempts << endl;
        cout << "Incorrect guesses: ";
        for (char c : incorrect) cout << c << " ";
        cout << "\nGuess a letter: ";
        
        char letter;
        cin >> letter;
        letter = tolower(letter);
        
        bool found = false;
        for (int i = 0; i < word.length(); i++) {
            if (word[i] == letter) {
                guessed[i] = letter;
                found = true;
            }
        }
        
        if (!found) {
            attempts--;
            incorrect.push_back(letter);
            cout << "Incorrect guess!\n";
        }
    }
    
    if (guessed == word) {
        cout << "\nCongratulations! You guessed the word: " << word << endl;
    } else {
        cout << "\nGame over! The word was: " << word << endl;
    }
}

// ================== NUMBER GUESSING GAME ==================
void play_number_guessing(User* user) {
    int number = rand() % 100 + 1;
    int guess;
    int attempts = 0;
    const int max_attempts = 7;

    cout << "\nWelcome to Number Guessing, " << user->username << "!\n";
    cout << "I'm thinking of a number between 1 and 100.\n";
    cout << "You have " << max_attempts << " attempts to guess it.\n";
    
    while (attempts < max_attempts) {
        cout << "\nAttempt " << attempts + 1 << "/" << max_attempts << ": ";
        cin >> guess;
        attempts++;
        
        if (guess < number) {
            cout << "Too low!";
        } else if (guess > number) {
            cout << "Too high!";
        } else {
            cout << "Congratulations! You guessed the number in " << attempts << " attempts!\n";
            return;
        }
        
        // Give hint after 3 attempts
        if (attempts == 3) {
            if (number % 2 == 0) {
                cout << " Hint: The number is even.";
            } else {
                cout << " Hint: The number is odd.";
            }
        }
        cout << endl;
    }
    
    cout << "\nGame over! The number was " << number << endl;
}

// ================== GAME MENU ==================
void game_menu(User* user) {
    while (true) {
        cout << "\nWelcome to the Game Menu, " << user->username << "!\n";
        cout << "1. Play Pokemon Game\n";
        cout << "2. Play Tic-Tac-Toe\n";
        cout << "3. Play Hangman\n";
        cout << "4. Play Number Guessing\n";
        cout << "5. Logout\n";
        cout << "Enter your choice: ";
        
        int choice;
        cin >> choice;

        switch (choice) {
            case 1: play_pokemon_game(user); break;
            case 2: play_tic_tac_toe(user); break;
            case 3: play_hangman(user); break;
            case 4: play_number_guessing(user); break;
            case 5: 
                logoutUser(user);
                return;
            default:
                cout << "Invalid choice. Please try again.\n";
        }
    }
}

// ================== MAIN MENU ==================
void main_menu() {
    loadUsers();
    User* currentUser = nullptr;

    while (true) {
        cout << "\nWelcome to the Game System!\n";
        if (currentUser) {
            cout << "Logged in as: " << currentUser->username << endl;
            cout << "1. Play Games\n";
            cout << "2. Logout\n";
            cout << "3. Exit\n";
            cout << "Enter your choice: ";
            
            int choice;
            cin >> choice;
            
            switch (choice) {
                case 1: game_menu(currentUser); break;
                case 2: 
                    logoutUser(currentUser);
                    currentUser = nullptr;
                    break;
                case 3:
                    cout << "Goodbye!\n";
                    return;
                default:
                    cout << "Invalid choice.\n";
            }
        } else {
            cout << "1. Login\n";
            cout << "2. Register\n";
            cout << "3. Exit\n";
            cout << "Enter your choice: ";
            
            int choice;
            cin >> choice;
            
            switch (choice) {
                case 1: currentUser = loginUser(); break;
                case 2: registerUser(); break;
                case 3:
                    cout << "Goodbye!\n";
                    return;
                default:
                    cout << "Invalid choice.\n";
            }
        }
    }
}

int main() {
    srand(static_cast<unsigned int>(time(nullptr)));
    main_menu();
    return 0;
}