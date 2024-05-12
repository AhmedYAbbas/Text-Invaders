#include "AlienSwarm.h"
#include "Game.h"
#include "Shield.h"
#include "Player.h"
#include "Utility/CursesUtils.h"

static const int NUM_ALIEN_ROWS = 5;
static const int NUM_ALIEN_COLUMNS = 11;
static const int MAX_NUMBER_OF_ALIEN_BOMBS = 3;
static const int ALIENS_X_PADDING = 1;
static const int ALIENS_Y_PADDING = 1;
static const int SHIELD_SPRITE_WIDTH = 7;
static const int SHIELD_SPRITE_HEIGHT = 3;
static const int NUM_LEVELS = 10;
static const int WAIT_TIME = 10;

const int NUM_30_POINT_ALIEN_ROWS = 1;
const int NUM_20_POINT_ALIEN_ROWS = 2;
const int NUM_10_POINT_ALIEN_ROWS = 2;

const static std::vector<std::string> ALIEN30_SPRITE = {"/oo\\", "<  >", "/oo\\", "/\"\"\\"};
const static std::vector<std::string> ALIEN20_SPRITE = {" >< ", "|\\/|", "|><|", "/  \\"};
const static std::vector<std::string> ALIEN10_SPRITE = {"/--\\", "/  \\", "/--\\", "<  >"};
const static std::vector<std::string> ALIEN_EXPLOSION = {"\\||/", "/||\\"};

AlienSwarm::AlienSwarm()
{
	Reset();
}

void AlienSwarm::Render()
{
	for (int col = 0; col < NUM_ALIEN_COLUMNS; ++col)
	{
		int xPos = m_Position.X + col * (m_SpriteSize.Width + ALIENS_X_PADDING);
		int yPos = m_Position.Y;

		if (m_Aliens[0][col] == AlienState::Alive)
			CursesUtils::DrawSprite(xPos, yPos, ALIEN30_SPRITE, m_SpriteSize.Height, m_Animation * m_SpriteSize.Height);
		else if (m_Aliens[0][col] == AlienState::Exploding)
			CursesUtils::DrawSprite(xPos, yPos, ALIEN_EXPLOSION, m_SpriteSize.Height);
	}

	for (int row = 0; row < NUM_20_POINT_ALIEN_ROWS; ++row)
	{
		for (int col = 0; col < NUM_ALIEN_COLUMNS; ++col)
		{
			int xPos = m_Position.X + col * (m_SpriteSize.Width + ALIENS_X_PADDING);
			int yPos = m_Position.Y + row * (m_SpriteSize.Height + ALIENS_Y_PADDING) + NUM_30_POINT_ALIEN_ROWS * (m_SpriteSize.Height + ALIENS_Y_PADDING);

			if (m_Aliens[NUM_30_POINT_ALIEN_ROWS + row][col] == AlienState::Alive)
				CursesUtils::DrawSprite(xPos, yPos, ALIEN20_SPRITE, m_SpriteSize.Height, m_Animation * m_SpriteSize.Height);
			else if (m_Aliens[NUM_30_POINT_ALIEN_ROWS + row][col] == AlienState::Exploding)
				CursesUtils::DrawSprite(xPos, yPos, ALIEN_EXPLOSION, m_SpriteSize.Height);
		}
	}

	for (int row = 0; row < NUM_10_POINT_ALIEN_ROWS; ++row)
	{
		for (int col = 0; col < NUM_ALIEN_COLUMNS; ++col)
		{
			int xPos = m_Position.X + col * (m_SpriteSize.Width + ALIENS_X_PADDING);
			int yPos = m_Position.Y + row * (m_SpriteSize.Height + ALIENS_Y_PADDING) + NUM_30_POINT_ALIEN_ROWS * (m_SpriteSize.Height + ALIENS_Y_PADDING) + NUM_20_POINT_ALIEN_ROWS * (m_SpriteSize.Height + ALIENS_Y_PADDING);

			if (m_Aliens[NUM_30_POINT_ALIEN_ROWS + NUM_20_POINT_ALIEN_ROWS + row][col] == AlienState::Alive)
				CursesUtils::DrawSprite(xPos, yPos, ALIEN10_SPRITE, m_SpriteSize.Height, m_Animation * m_SpriteSize.Height);
			else if (m_Aliens[NUM_30_POINT_ALIEN_ROWS + NUM_20_POINT_ALIEN_ROWS + row][col] == AlienState::Exploding)
				CursesUtils::DrawSprite(xPos, yPos, ALIEN_EXPLOSION, m_SpriteSize.Height);
		}
	}

	if (m_NumberOfBombsInPlay > 0)
	{
		for (int i = 0; i < MAX_NUMBER_OF_ALIEN_BOMBS; ++i)
		{
			if (m_Bombs[i].GetPosition().X != -1 && m_Bombs[i].GetPosition().Y != -1)
				CursesUtils::DrawChar(m_Bombs[i].GetPosition().X, m_Bombs[i].GetPosition().Y, m_Bombs[0].GetSprite()[m_Bombs[i].GetAnimation()]);
		}
	}

}

void AlienSwarm::Reset()
{
	for (int r = 0; r < NUM_ALIEN_ROWS; r++)
	{
		for (int c = 0; c < NUM_ALIEN_COLUMNS; ++c)
			m_Aliens[r][c] = AlienState::Alive;
	}

	ResetMovementTimer();

	m_Direction = 1;
	m_NumOfAliensLeft = NUM_ALIEN_ROWS * NUM_ALIEN_COLUMNS;
	m_Animation = 0;
	m_SpriteSize.Width = 4;
	m_SpriteSize.Height = 2;
	m_NumberOfBombsInPlay = 0;
	m_Position.X = (Game::GetWindowSize().Width - NUM_ALIEN_COLUMNS * (m_SpriteSize.Width + ALIENS_X_PADDING)) / 2;
	m_Position.Y = Game::GetWindowSize().Height - NUM_ALIEN_COLUMNS - NUM_ALIEN_ROWS * m_SpriteSize.Height - ALIENS_Y_PADDING * (NUM_ALIEN_ROWS - 1) - 3 + Game::GetLevel();
	m_Line = NUM_ALIEN_COLUMNS - (Game::GetLevel() - 1);
	m_ExplosionTimer = -1;

	for (int i = 0; i < MAX_NUMBER_OF_ALIEN_BOMBS; i++)
	{
		m_Bombs[i].SetAnimation(0);;
		m_Bombs[i].SetXPosition(-1);
		m_Bombs[i].SetYPosition(-1);
	}
}

void AlienSwarm::ResetMovementTimer()
{
	m_MovementTimer = m_Line * 2 + (4 * (static_cast<float>(m_NumOfAliensLeft) / static_cast<float>(NUM_ALIEN_COLUMNS * NUM_ALIEN_ROWS)));
}

void AlienSwarm::Update(std::clock_t dt, Game& game, const Player& player)
{
	if (game.GetState() == GameState::Play)
	{
		if (UpdateBombs(game.GetShields(), player))
			game.SetGameState(GameState::PlayerDead);

		if (m_ExplosionTimer >= 0)
			--m_ExplosionTimer;

		for (int row = 0; row < NUM_ALIEN_ROWS; ++row)
		{
			for (int col = 0; col < NUM_ALIEN_COLUMNS; ++col)
			{
				if (m_ExplosionTimer == -1 && m_Aliens[row][col] == AlienState::Exploding)
					m_Aliens[row][col] = AlienState::Dead;
			}
		}

		--m_MovementTimer;

		bool moveHorizontal = 0 >= m_MovementTimer;
		int emptyColsLeft = 0;
		int emptyColsRight = 0;
		int emptyRowsBottom = 0;

		FindEmptyRowsAndColumns(emptyColsLeft, emptyColsRight, emptyRowsBottom);

		int numberOfColumns = NUM_ALIEN_COLUMNS - emptyColsLeft - emptyColsRight;
		int leftAlienPosition = m_Position.X + emptyColsLeft * (m_SpriteSize.Width + ALIENS_X_PADDING);
		int rightAlienPosition = leftAlienPosition + numberOfColumns * m_SpriteSize.Width + (numberOfColumns - 1) * ALIENS_X_PADDING;

		if (((rightAlienPosition >= Game::GetWindowSize().Width && m_Direction > 0) || (leftAlienPosition <= 0 && m_Direction < 0)) && moveHorizontal && m_Line > 0)
		{
			moveHorizontal = false;
			++m_Position.Y;
			--m_Line;
			m_Direction *= -1;
			ResetMovementTimer();
			DestroyShields(game.GetShields());

			if (m_Line == 0)
				game.SetGameState(GameState::GameOver);
		}

		if (moveHorizontal)
		{
			m_Position.X += m_Direction;
			ResetMovementTimer();
			m_Animation = m_Animation == 0 ? 1 : 0;
			DestroyShields(game.GetShields());
		}

		if (!moveHorizontal)
		{
			int activeColumns[NUM_ALIEN_COLUMNS];
			int numActiveCols = 0;
			for (int c = emptyColsLeft; c < numberOfColumns; ++c)
			{
				for (int r = 0; r < NUM_ALIEN_ROWS; r++)
				{
					if (m_Aliens[r][c] == AlienState::Alive)
					{
						activeColumns[numActiveCols] = c;
						++numActiveCols;
						break;
					}
				}
			}

			if (ShouldShootBomb())
			{
				if (numActiveCols > 0)
				{
					int numberOfShots = ((std::rand() % 3) + 1) - m_NumberOfBombsInPlay;
					for (int i = 0; i < numberOfShots; ++i)
					{
						int columnToShoot = std::rand() % numActiveCols;
						ShootBomb(activeColumns[columnToShoot]);
					}
				}
			}
		}

		if (m_NumOfAliensLeft == 0)
		{
			game.SetLevel((game.GetLevel() + 1) % NUM_LEVELS);
			if (game.GetLevel() == 0)
				game.SetLevel(1);

			game.SetGameState(GameState::Wait);
			game.SetWaitTimer(WAIT_TIME);
			game.ResetGame();
		}
	}
}

bool AlienSwarm::UpdateBombs(std::vector<Shield>& shields, const Player& player)
{
	int numBombSprites = m_Bombs[0].GetSprite().size();

	for (int i = 0; i < MAX_NUMBER_OF_ALIEN_BOMBS; ++i)
	{
		if (m_Bombs[i].GetPosition().X != -1 && m_Bombs[i].GetPosition().Y != -1)
		{
			m_Bombs[i].Move(1);
			m_Bombs[i].SetAnimation((m_Bombs[i].GetAnimation() + 1) % numBombSprites);

			Position collisionPoint;
			int shieldIndex = m_Bombs[i].Collides(shields, collisionPoint);

			if (shieldIndex != -1)
			{
				m_Bombs[i].SetXPosition(-1);
				m_Bombs[i].SetYPosition(-1);
				m_Bombs[i].SetAnimation(0);
				--m_NumberOfBombsInPlay;
				m_Bombs[i].ResolveShieldCollision(shieldIndex, shields, collisionPoint);
			}
			else if (m_Bombs[i].Collides(m_Bombs[i].GetPosition(), player.GetPosition(), player.GetSpriteSize()))
			{
				m_Bombs[i].SetXPosition(-1);
				m_Bombs[i].SetYPosition(-1);
				m_Bombs[i].SetAnimation(0);
				--m_NumberOfBombsInPlay;
				return true;
			}
			else if (m_Bombs[i].GetPosition().Y >= Game::GetWindowSize().Height)
			{
				m_Bombs[i].SetXPosition(-1);
				m_Bombs[i].SetYPosition(-1);
				m_Bombs[i].SetAnimation(0);
				--m_NumberOfBombsInPlay;
			}
		}
	}
	return false;
}

void AlienSwarm::FindEmptyRowsAndColumns(int& emptyColsLeft, int& emptyColsRight, int& emptyRowsBottom)
{
	bool found = false;
	for (int col = 0; col < NUM_ALIEN_COLUMNS && !found; ++col)
	{
		for (int row = 0; row < NUM_ALIEN_ROWS && !found; ++row)
		{
			if (m_Aliens[row][col] == AlienState::Dead)
			{
				if (row == NUM_ALIEN_ROWS - 1)
					++emptyColsLeft;
			}
			else
				found = true;
		}
	}

	found = false;
	for (int col = NUM_ALIEN_COLUMNS - 1; col >= 0 && !found; --col)
	{
		for (int row = 0; row < NUM_ALIEN_ROWS && !found; ++row)
		{
			if (m_Aliens[row][col] == AlienState::Dead)
			{
				if (row == NUM_ALIEN_ROWS - 1)
					++emptyColsRight;
			}
			else
				found = true;
		}
	}

	found = false;
	for (int row = NUM_ALIEN_ROWS - 1; row >= 0 && !found; --row)
	{
		for (int col = 0; col < NUM_ALIEN_COLUMNS && !found; ++col)
		{
			if (m_Aliens[row][col] == AlienState::Dead)
			{
				if (col == NUM_ALIEN_COLUMNS - 1)
					++emptyRowsBottom;
			}
			else
				found = true;
		}
	}
}

void AlienSwarm::DestroyShields(std::vector<Shield>& shields)
{
	for (int row = 0; row < NUM_ALIEN_ROWS; ++row)
	{
		for (int col = 0; col < NUM_ALIEN_COLUMNS; ++col)
		{
			if (m_Aliens[row][col] == AlienState::Alive)
			{
				int xPos = m_Position.X + col * (m_SpriteSize.Width + ALIENS_X_PADDING);
				int yPos = m_Position.Y + row * (m_SpriteSize.Height + ALIENS_Y_PADDING);
				CollideShieldsWithAlien(shields, xPos, yPos, m_SpriteSize);
			}
		}
	}
}

void AlienSwarm::CollideShieldsWithAlien(std::vector<Shield>& shields, int alienPositionX, int alienPositionY, const Size& spriteSize)
{
	for (int i = 0; i < shields.size(); ++i)
	{
		if (alienPositionX < shields[i].GetPosition().X + SHIELD_SPRITE_WIDTH && alienPositionX + spriteSize.Width >= shields[i].GetPosition().X &&
			alienPositionY < shields[i].GetPosition().Y + SHIELD_SPRITE_HEIGHT && alienPositionY + spriteSize.Height >= shields[i].GetPosition().Y)
		{
			int dy = alienPositionY - shields[i].GetPosition().Y;
			int dx = alienPositionX - shields[i].GetPosition().X;
			for (int h = 0; h < spriteSize.Height; ++h)
			{
				int shieldY = dy + h;
				if (shieldY >= 0 && shieldY < SHIELD_SPRITE_HEIGHT)
				{
					for (int w = 0; w < spriteSize.Width; ++w)
					{
						int shieldX = dx + w;
						if (shieldX >= 0 && shieldX < SHIELD_SPRITE_WIDTH)
							shields[i].SetSprite(shieldY, shieldX, ' ');
					}
				}
			}
			break;
		}
	}
}

bool AlienSwarm::ShouldShootBomb()
{
	return static_cast<int>(std::rand() % (70 - static_cast<int>(static_cast<float>(NUM_ALIEN_ROWS * NUM_ALIEN_COLUMNS) / static_cast<float>(m_NumOfAliensLeft + 1)))) == 1;
}

void AlienSwarm::ShootBomb(int columnToShoot)
{
	int bombId = -1;
	for (int i = 0; i < MAX_NUMBER_OF_ALIEN_BOMBS; ++i)
	{
		if (m_Bombs[i].GetPosition().X == -1 || m_Bombs[i].GetPosition().Y == -1)
		{
			bombId = i;
			break;
		}
	}

	for (int r = NUM_ALIEN_ROWS - 1; r >= 0; --r)
	{
		if (m_Aliens[r][columnToShoot] == AlienState::Alive && bombId != -1)
		{
			int xPos = m_Position.X + columnToShoot * (m_SpriteSize.Width + ALIENS_X_PADDING) + 1;
			int yPos = m_Position.Y + r * (m_SpriteSize.Height + ALIENS_Y_PADDING) + m_SpriteSize.Height;

			m_Bombs[bombId].SetAnimation(0);
			m_Bombs[bombId].SetXPosition(xPos);
			m_Bombs[bombId].SetYPosition(yPos);
			++m_NumberOfBombsInPlay;
			break;
		}
	}
}
