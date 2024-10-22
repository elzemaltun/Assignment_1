# Game Project

## Overview

This project is a game developed for an Arduino-based platform. The game involves a player inputting answers to a randomly generated number, with the goal of achieving a correct answer. The game features multiple states, including an initial state, sleep mode, round start, player input, success state, and game over.

## Directory Structure

The project is organized into the following directories:

- `include`: Contains header files for the project.
- `lib`: Contains libraries used by the project, including `ButtonHandler`, `Debug`, `GameLogic`, and `LedHandler`.
- `src`: Contains the main source code for the project, including `main.cpp`.
- `test`: Contains test files for the project.

## Libraries

The project uses the following libraries:

- `ButtonHandler`: Handles button inputs from the player.
- `Debug`: Provides debugging functionality for the project.
- `GameLogic`: Manages the game logic, including generating random numbers and checking player answers.
- `LedHandler`: Controls the LEDs used in the game.

## Game States

The game has the following states:

- `INITIAL_STATE`: The initial state of the game, where the player can adjust the difficulty level.
- `SLEEP_MODE`: The game enters sleep mode after a period of inactivity.
- `ROUND_START`: The game starts a new round, generating a random number for the player to answer.
- `PLAYER_INPUT`: The player inputs their answer to the random number.
- `SUCCESS_STATE`: The player achieves a correct answer, and the game displays a success message.
- `GAME_OVER`: The game ends, and the player can restart.

## Functions

The project uses the following functions:

- `setup()`: Initializes the game, including setting up the buttons, LEDs, and game logic.
- `loop()`: Runs the game loop, which checks the current game state and performs the corresponding actions.
- `setState()`: Sets the current game state to a new state.
- `handleSleepMode()`: Handles the game's sleep mode.
- `wakeUp()`: Wakes up the game from sleep mode.

## Contributing

Contributions to this project are welcome! If you have any ideas or suggestions for improvements, please open an issue or submit a pull request.

## License

This project is licensed under the [MIT License](LICENSE).