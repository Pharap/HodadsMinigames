// Angle bracket includes are for global libraries
#include <Pokitto.h>
#include <cstdlib>

// Quote includes are for local files
#include "GameState.h"

Pokitto::Core mygame;

// Pre-declare functions so the rest of the code can see them
void updateMainMenu(void);
void renderMainMenu(void);
void updateSortingGame(void);
void renderSortingGame(void);
void updateGame2(void);
void renderGame2(void);

// Global variables are accessible by all functions
// because they exist outside of functions
GameState gameState = GameState::MainMenu;

int main ()
{
	// This allows us to skip having to write Pokitto:: in front of Pokitto lib stuff
	using namespace Pokitto;

	// Initialise the Pokitto
	// This runs the volume screen etc
	Core::begin();

	// An almost infinite loop
	while (Core::isRunning())
	{
		// If the Pokitto is ready to draw the next frame
		if (Core::update())
		{
			// Decide what to do depending on the value of gameState
			switch(gameState)
			{
				case GameState::MainMenu:
					updateMainMenu();
					renderMainMenu();
					break;
				case GameState::SortingGame:
					updateSortingGame();
					renderSortingGame();
					break;
				case GameState::Game2:
					updateGame2();
					renderGame2();
					break;
			}
		}
	}
}

constexpr int menuSelectionMin = 0;
constexpr int menuSelectionMax = 1;

int menuSelection = 0;

void updateMainMenu(void)
{	
	using namespace Pokitto;

	// If up is pressed
	if(Buttons::pressed(BTN_UP))
	{
		// Decrease menuSelection only if
		// menuSelection is larger than
		// our minimum allowed value
		if(menuSelection > menuSelectionMin)
		{
			--menuSelection;
		}
	}

	// If down is pressed
	if(Buttons::pressed(BTN_DOWN))
	{
		// Increase menuSelection only if
		// menuSelection is less than
		// our maximum allowed value
		if(menuSelection < menuSelectionMax)
		{
			++menuSelection;
		}
	}

	// If A is pressed
	if(Buttons::pressed(BTN_A))
	{
		// Decide which state to change to
		// based on the value of menuSelection
		switch(menuSelection)
		{
			case 0:
				gameState = GameState::SortingGame;
				break;
			case 1:
				gameState = GameState::Game2;
				break;
		}
	}
}

const char * menuOptions[]
{
	"Sorting Game",
	"Game 2",
};

void renderMainMenu(void)
{
	using namespace Pokitto;

	for(int i = menuSelectionMin; i <= menuSelectionMax; ++i)
	{
		if(i == menuSelection)
		{
			Display::print("> ");
		}
		else
		{
			Display::print("- ");
		}
		
		Display::println(menuOptions[i]);
	}
}

enum class Symbol
{
	Tingle,
	Navi,
	Cuccoo,
};

Symbol currentSymbol;
int score = 0;
bool gameOver = false;

Symbol generateRandomSymbol(void)
{
	return static_cast<Symbol>(std::rand() % 3);
}

void handleHitAction(void)
{
	switch(currentSymbol)
	{
		case Symbol::Tingle:
			--score;
			break;
		case Symbol::Navi:
			++score;
			break;
		case Symbol::Cuccoo:
			gameOver = true;
			break;
	}
}

void handlePassAction(void)
{
	switch(currentSymbol)
	{
		case Symbol::Tingle:
			++score;
			break;
		case Symbol::Navi:
			--score;
			break;
		case Symbol::Cuccoo:
			score += 10;
			break;
	}
}

void updateSortingGame(void)
{
	using namespace Pokitto;

	if(!gameOver)
	{			
		if(Buttons::pressed(BTN_A))
		{
			handlePassAction();
			currentSymbol = generateRandomSymbol();
		}

		if(Buttons::pressed(BTN_B))
		{
			handleHitAction();
			currentSymbol = generateRandomSymbol();
		}
	}
	else
	{
		if(Buttons::pressed(BTN_A))
		{
			gameOver = false;
			score = 0;
		}

		if(Buttons::pressed(BTN_B))
		{
			gameState = GameState::MainMenu;
			gameOver = false;
			score = 0;
		}
	}
}

void renderSortingGame(void)
{
	using namespace Pokitto;

	Display::print("Score: ");
	Display::print(score);

	Display::setCursor(0, Display::getHeight() / 2);
	if(!gameOver)
	{
		switch(currentSymbol)
		{
			case Symbol::Tingle:
				Display::print("Pretend this is Tingle");
				break;
			case Symbol::Navi:
				Display::print("Pretend this is Navi");
				break;
			case Symbol::Cuccoo:
				Display::print("Pretend this is a Cuccoo");
				break;
		}
	}
	else
	{
		Display::print("GAME OVER");
	}
}

void updateGame2(void)
{
	if(Pokitto::Buttons::pressed(BTN_B))
		gameState = GameState::MainMenu;
}

void renderGame2(void)
{
	Pokitto::Display::println("Under construction");
	Pokitto::Display::println("Press B to go back");
}