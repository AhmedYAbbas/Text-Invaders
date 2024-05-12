#include <fstream>
#include <iostream>
#include <algorithm>

#include "Game.h"
#include "Shield.h"
#include "Utility/CursesUtils.h"

const int MAX_NUMBER_OF_CHARACTERS_IN_NAME = 3;
const int MAX_HIGH_SCORES = 10;

Size Game::m_WindowSize = {80, 32};
int Game::m_Level = 1;
GameState Game::m_CurrentState = GameState::Intro;
std::clock_t Game::m_GameTimer = 0;

Game::Game()
	: m_WaitTimer(0), m_Player(), m_Shields(4)
{
	ResetGameOverCursorPos();
	ResetShields();
	m_AlienSwarm.Reset();
	m_UFO.Reset();
	LoadHighScores();
}

void Game::Update()
{
	bool quit = false;
	int input;
	clock_t lastTime = clock();
	while (!quit)
	{
		input = HandleInput();
		if (input != 'q')
		{
			clock_t currentTime = clock();
			clock_t dt = currentTime - lastTime;
			if (dt > CLOCKS_PER_SEC / 20)
			{
				lastTime = currentTime;
				m_GameTimer += dt;

				m_Player.Update(dt, *this);
				m_AlienSwarm.Update(dt, *this, m_Player);
				m_UFO.Update(dt, m_Player);

				if (m_CurrentState == GameState::Wait)
				{
					--m_WaitTimer;
					if (m_WaitTimer == 0)
						m_CurrentState = GameState::Play;
				}

				CursesUtils::Clear();
				Render();
				CursesUtils::Refresh();
			}
		}
		else
			quit = true;
	}
}

void Game::ResetShields()
{
	const int NUM_OF_SHIELDS = 4;
	const int SHIELD_SPRITE_WIDTH = 7;
	const int SHIELD_SPRITE_HEIGHT = 3;
	const int PLAYER_SPRITE_HEIGHT = 2;

	int firstPadding = ceil(static_cast<float>(Game::GetWindowSize().Width - NUM_OF_SHIELDS * SHIELD_SPRITE_WIDTH) / (NUM_OF_SHIELDS + 1.f));
	int xPadding = floor(static_cast<float>(Game::GetWindowSize().Width - NUM_OF_SHIELDS * SHIELD_SPRITE_WIDTH) / (NUM_OF_SHIELDS + 1.f));

	for (int i = 0; i < NUM_OF_SHIELDS; i++)
	{
		m_Shields[i].SetPosition(firstPadding + i * (SHIELD_SPRITE_WIDTH + xPadding), Game::GetWindowSize().Height - PLAYER_SPRITE_HEIGHT - 1 - SHIELD_SPRITE_HEIGHT - 2);
		m_Shields[i].SetSprite({"/IIIII\\", "IIIIIII", "I/   \\I"});
	}
}

void Game::ResetGame()
{
	m_GameTimer = 0;
	ResetShields();
	m_Player.Reset();
	m_AlienSwarm.Reset();
}

void Game::AddHighScore(int score, const std::string& name)
{
	Score highScore;
	highScore.Value = score;
	highScore.Name = name;

	m_HighScoreTable.Scores.push_back(highScore);
	std::sort(m_HighScoreTable.Scores.begin(), m_HighScoreTable.Scores.end(), [](const Score& score1, const Score& score2)
	{
		return score1.Value > score2.Value;
	});
	SaveHighScore();
}

void Game::LoadHighScores()
{
	std::ifstream inFile;
	inFile.open("HighScores.txt");

	std::string name;
	int scoreVal;
	Score score;
	if (inFile.is_open())
	{
		while (!inFile.eof())
		{
			inFile >> std::ws;
			if (inFile.eof())
				break;

			inFile >> name >> scoreVal;
			score.Name = name;
			score.Value = scoreVal;
			m_HighScoreTable.Scores.push_back(score);
		}
		inFile.close();
	}
}

void Game::SaveHighScore()
{
	const int MAX_HIGH_SCORES = 10;

	std::ofstream outFile;
	outFile.open("HighScores.txt");
	if (outFile.is_open())
	{
		for (int i = 0; i < m_HighScoreTable.Scores.size() && i < MAX_HIGH_SCORES; ++i)
			outFile << m_HighScoreTable.Scores[i].Name << " " << m_HighScoreTable.Scores[i].Value << std::endl;
		outFile.close();
	}
}

void Game::DrawGameOverScreen()
{
	std::string gameOverString = "Game Over!";
	std::string pressSpaceString = "Press Space Bar to continue";
	std::string namePromptString = "Please Enter you name:";

	const int yPos = m_WindowSize.Height / 3;
	const int gameOverXPos = m_WindowSize.Width / 2 - gameOverString.size() / 2;
	const int pressSpaceXPos = m_WindowSize.Width / 2 - pressSpaceString.size() / 2;
	const int namePromptXPos = m_WindowSize.Width / 2 - namePromptString.size() / 2;

	CursesUtils::DrawString(gameOverXPos, yPos, gameOverString);
	CursesUtils::DrawString(pressSpaceXPos, yPos + 1, pressSpaceString);
	CursesUtils::DrawString(namePromptXPos, yPos + 3, namePromptString);

	for (int i = 0; i < MAX_NUMBER_OF_CHARACTERS_IN_NAME; ++i)
	{
		if (m_GameOverHPosCursor == i)
			attron(A_UNDERLINE);

		CursesUtils::DrawChar(m_WindowSize.Width / 2 - MAX_NUMBER_OF_CHARACTERS_IN_NAME / 2 + i, yPos + 5, m_Player.GetName()[i]);

		if (m_GameOverHPosCursor == i)
			attroff(A_UNDERLINE);
	}
}

void Game::DrawIntroScreen()
{
	std::string startString = "Welcome to Text Invaders";
	std::string pressSpaceString = "Press Space Bar to start";
	std::string pressSString = "Press (s) to go to the high scores";

	const int yPos = m_WindowSize.Height / 2 - 2;
	const int startStringXPos = m_WindowSize.Width / 2 - startString.size() / 2;
	const int pressSpaceXPos = m_WindowSize.Width / 2 - pressSpaceString.size() / 2;
	const int pressSXPos = m_WindowSize.Width / 2 - pressSString.size() / 2;

	CursesUtils::DrawString(startStringXPos, yPos, startString);
	CursesUtils::DrawString(pressSpaceXPos, yPos + 1, pressSpaceString);
	CursesUtils::DrawString(pressSXPos, yPos + 2, pressSString);
}

void Game::DrawHighScoreTable()
{
	std::string title = "High Scores";
	int titleXPos = m_WindowSize.Width / 2 - title.size() / 2;
	int yPos = 5;
	int yPadding = 2;

	attron(A_UNDERLINE);
	CursesUtils::DrawString(titleXPos, yPos, title);
	attroff(A_UNDERLINE);

	for (int i = 0; i < m_HighScoreTable.Scores.size() && i < MAX_HIGH_SCORES; ++i)
	{
		Score score;
		score.Name = m_HighScoreTable.Scores[i].Name;
		score.Value = m_HighScoreTable.Scores[i].Value;
		mvprintw(yPos + (i + 1) * yPadding, titleXPos - MAX_NUMBER_OF_CHARACTERS_IN_NAME, "%s\t\t%i", score.Name.c_str(), score.Value);
	}
}

int Game::HandleInput()
{
	int input = CursesUtils::GetChar();
	switch (input)
	{
		case 's':
			if (m_CurrentState == GameState::Intro)
				m_CurrentState = GameState::HighScore;
			break;
		case 'q':
			return input;
			break;
		case ArrowKeys::Left:
			if (m_CurrentState == GameState::Play)
				m_Player.Move(-2);
			else if (m_CurrentState == GameState::GameOver)
			{
				--m_GameOverHPosCursor;
				if (m_GameOverHPosCursor < 0)
					m_GameOverHPosCursor = 2;
			}
			break;
		case ArrowKeys::Right:
			if (m_CurrentState == GameState::Play)
				m_Player.Move(2);
			else if (m_CurrentState == GameState::GameOver)
				m_GameOverHPosCursor = (m_GameOverHPosCursor + 1) % 3;
			break;
		case ArrowKeys::Up:
			if (m_CurrentState == GameState::GameOver)
			{
				--m_GameOverVPosCursor[m_GameOverHPosCursor];
				if (m_GameOverVPosCursor[m_GameOverHPosCursor] < 0)
					m_GameOverVPosCursor[m_GameOverHPosCursor] = 25;
				m_Player.SetPlayerName(m_GameOverHPosCursor, 'A' + m_GameOverVPosCursor[m_GameOverHPosCursor]);
			}
			break;
		case ArrowKeys::Down:
			if (m_CurrentState == GameState::GameOver)
			{
				m_GameOverVPosCursor[m_GameOverHPosCursor] = (m_GameOverVPosCursor[m_GameOverHPosCursor] + 1) % 26;
				m_Player.SetPlayerName(m_GameOverHPosCursor, 'A' + m_GameOverVPosCursor[m_GameOverHPosCursor]);
			}
			break;
		case ' ':
			if (m_CurrentState == GameState::Play)
				m_Player.Shoot();
			else if (m_CurrentState == GameState::PlayerDead)
			{
				m_Player.DecreaseLives();
				m_Player.SetAnimation(0);
				if (m_Player.GetLives() == 0)
				{
					m_CurrentState = GameState::GameOver;
					ResetGameOverCursorPos();
				}
				else
				{
					m_CurrentState = GameState::Wait;
					m_WaitTimer = 10;
				}
			}
			else if (m_CurrentState == GameState::GameOver)
			{
				m_Player.SetPlayerName(3, '\0');
				AddHighScore(m_Player.GetScore(), m_Player.GetName());
				m_CurrentState = GameState::HighScore;
			}
			else if (m_CurrentState == GameState::HighScore)
			{
				m_CurrentState = GameState::Intro;
				ResetGame();
			}
			else if (m_CurrentState == GameState::Intro)
				m_CurrentState = GameState::Play;
		default:
			break;
	}
}

void Game::Render()
{
	if (m_CurrentState == GameState::Play || m_CurrentState == GameState::PlayerDead || m_CurrentState == GameState::Wait)
	{
		if (m_CurrentState == GameState::Play || m_CurrentState == GameState::Wait)
			m_Player.Render({" =A= ", "====="});
		else
			m_Player.Render({",~^.'", "=====", "'+-`.", "====="});

		for (auto& shield : m_Shields)
			shield.Render();

		m_AlienSwarm.Render();
		m_UFO.Render();
	}
	else if (m_CurrentState == GameState::GameOver)
		DrawGameOverScreen();
	else if (m_CurrentState == GameState::Intro)
		DrawIntroScreen();
	else if (m_CurrentState == GameState::HighScore)
		DrawHighScoreTable();
}

void Game::ResetGameOverCursorPos()
{
	m_GameOverHPosCursor = 0;
	for (int i = 0; i < 3; i++)
	{
		m_Player.SetPlayerName(i, 'A');
		m_GameOverVPosCursor[i] = 0;
	}
}
