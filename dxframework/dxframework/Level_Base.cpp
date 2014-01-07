#include "Level_Base.h"


Level_Base::Level_Base(void)
{
}


Level_Base::~Level_Base(void)
{
	for ( int i = 0; i < enemies.size(); i++)
		delete enemies[i];

}

void Level_Base::Update( float dt, Object_Player* player, gunType bulletColor, D3DXVECTOR3 eyePos, D3DXVECTOR3 lookAt)
{
	// Update all the ghosts that are created
	for ( int i = 0; i < enemies.size(); i++)
	{
		// if the enemy is not dead then update and check for bullet collision
		if ( enemies[i]->GetIsDead() == false)
		{
			enemies[i]->Update( dt, player);
			enemies[i]->BulletCollision( dt, player, bulletColor);

			// check for wall collisions based on current room
			if( currentRoom == FOYER )
			{
				enemies[i]->RoomWallCollision( dt, Foyer);
			}
			else if ( currentRoom == BALLROOM )
			{
				enemies[i]->RoomWallCollision( dt, Ballroom );
			}
			else if ( currentRoom == DININGROOM )
			{
				enemies[i]->RoomWallCollision( dt, Diningroom );
			}
			else if ( currentRoom == KITCHEN )
			{
				enemies[i]->RoomWallCollision( dt, Kitchen );
			}
			else
			{
				// TODO:  bounding box for the house
			}
		}
		// if enemy is dead then erase from vector list
		else
			enemies.erase( enemies.begin() + i );
	}

	// if there are no enemies in the vector list then set to dead
	if ( enemies.empty() == true )
		enemiesDead = true;

	// if enemies are dead and puzzle is solved then the level is cleared
	if ( enemiesDead == true && puzzleSolved == true )
		levelCleared = true;

	// switch to update the puzzles
	switch( level )
	{
	case 0:
		{
			// Torch Puzzle Update
			Puzzle_FT->Update( dt, eyePos, lookAt );
			break;
		}
	case 1:
		{
			// Chandelier Puzzle Update
			break;
		}
	case 2:
		{
			// Cup puzzle Update
			break;
		}
	case 3:
		{
			// Piano Puzzle update
			break;
		}
	default:
		{
			// don't update any puzzles
			break;
		}
	}
}

void Level_Base::Render(HWND hwnd, D3DXMATRIX veiwMat, D3DXMATRIX projMat, D3DXVECTOR3 eyePos)
{
	// Render all of the ghosts if they are not dead
	for ( int i = 0; i < enemies.size(); i++)
	{
		if ( enemies[i]->GetIsDead() == false)
		{
			enemies[i]->Render( hwnd, veiwMat, projMat);
		}
	}

	// switch to render the puzzles
	switch( level )
	{
	case 0:
		{
			// Torch Puzzle render
			Puzzle_FT->Render( hwnd, eyePos, veiwMat, projMat);
			break;
		}
	case 1:
		{
			// Chandelier Puzzle render
			break;
		}
	case 2:
		{
			// Cup puzzle render
			break;
		}
	case 3:
		{
			// Piano Puzzle render
			break;
		}
	default:
		{
			// Don't render any puzzles
			break;
		}
	}
}

// Random Binomial that takes in two variables in which the output can be between
float Level_Base::RandomBinomial( float min, float max)
{
	float random = ((float) rand()) / (float) RAND_MAX;

	float range = max - min;  
	return (random*range) + min;
}