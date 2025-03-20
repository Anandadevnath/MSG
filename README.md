# Pokemon C++ Game

This is a simple C++ game that allows you to play two different games: a Pokemon catching and battling game, and a Tic-Tac-Toe game.

## Features

1. **Pokemon Game**:
   - Catch wild Pokemon.
   - Visit the shop to buy items like Pokeballs, Potions, and Spells.
   - Battle with wild Pokemon.
   - Level up your player and Pokemon.

2. **Tic-Tac-Toe Game**:
   - Play a classic Tic-Tac-Toe game against the computer.

## How to Play

### Pokemon Game

1. Run the game.
2. Enter your player name.
3. Choose an action:
   - `C` to catch a wild Pokemon.
   - `S` to visit the shop and buy items.
   - `B` to battle with a wild Pokemon.
   - `Q` to quit the game.
4. Continue playing until your player reaches level 10 or you decide to quit.

### Tic-Tac-Toe Game

1. Run the game.
2. Choose the Tic-Tac-Toe option from the main menu.
3. Play against the computer by entering a number between 1 to 9 to place your mark on the board.
4. The game continues until there is a winner or a tie.

## File Structure

- `main.cpp`: Contains the main logic for both the Pokemon game and the Tic-Tac-Toe game.
- `pokemons.txt`: A list of Pokemon names used in the Pokemon game.
- `README.md`: This file.

## How to Compile and Run

1. Make sure you have a C++ compiler installed.
2. Compile the `main.cpp` file:
   ```sh
   g++ main.cpp -o main
