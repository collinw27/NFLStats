#include "StatsDatabase.h"

StatsDatabase::StatsDatabase( const std::string& filename )
{
    std::cout << "Loading database..." << std::endl;

	// Test by reading the names from the first 10 lines of the CSV file
    std::ifstream csvFile { filename };

    // Discard the first line
    std::string buffer;
    std::getline( csvFile, buffer );

    // Continue reading the data for each line
    // When large chunks of data go unused, they are repeatedly extracted to the buffer
    int i = 0;
    while ( true )
    {
    	int playerID;
    	std::string playerName;
    	int playerHeight;
    	int playerWeight;

    	std::string team;
    	std::string position;
		int season;
		int week;
		std::string gameType;
		int passAttempts;
		int completedPasses;
		int incompletePasses;
		int passingYards;
		int passingAirYards;
		int passTD;
		int interceptions;
		// skipped passFumbleLost
		int targets;
		int receptions;
		int receivingYards;
		int receivingAirYards;
		int yardsAfterCatch;
		int receptionTD;
		// skipped receptionFumbleLost
		int rushAttempts;
		int rushingYards;
		int rushingTD;
		// skipped runFumbleLost
		// skipped
		int touches;
		int totalTD;
		// skipped
		int totalYards;
		// skipped
		std::string opponent;

    	std::getline( csvFile, buffer, ',' );
    	if ( csvFile.eof() )
    		break;

    	team = buffer;
    	std::getline( csvFile, buffer, ',' );
    	playerID = std::stoi( buffer.substr( 3 ) );
    	std::getline( csvFile, playerName, ',' );
    	std::getline( csvFile, position, ',' );
    	season = getlineInt( csvFile );
    	week = getlineInt( csvFile );
    	std::getline( csvFile, gameType, ',' );
    	passAttempts = getlineInt( csvFile );
    	completedPasses = getlineInt( csvFile );
    	incompletePasses = getlineInt( csvFile );
    	passingYards = getlineInt( csvFile );
    	passingAirYards = getlineInt( csvFile );
    	passTD = getlineInt( csvFile );
    	interceptions = getlineInt( csvFile );

    	std::getline( csvFile, buffer, ',' );

    	targets = getlineInt( csvFile );
    	receptions = getlineInt( csvFile );
		receivingYards = getlineInt( csvFile );
		receivingAirYards = getlineInt( csvFile );
		yardsAfterCatch = getlineInt( csvFile );
		receptionTD = getlineInt( csvFile );

    	std::getline( csvFile, buffer, ',' );

		rushAttempts = getlineInt( csvFile );
		rushingYards = getlineInt( csvFile );
		rushingTD = getlineInt( csvFile );

		for ( int j = 0; j < 13; ++j ) std::getline( csvFile, buffer, ',' );

		touches = getlineInt( csvFile );
		totalTD = getlineInt( csvFile );

    	std::getline( csvFile, buffer, ',' );

		totalYards = getlineInt( csvFile );

		for ( int j = 0; j < 2; ++j ) std::getline( csvFile, buffer, ',' );

		std::getline( csvFile, opponent, ',' );

		for ( int j = 0; j < 9; ++j ) std::getline( csvFile, buffer, ',' );

		playerHeight = getlineInt( csvFile );
		playerWeight = getlineInt( csvFile );

		std::getline( csvFile, buffer );

		// If necessary, create a new player
		Player* player = nullptr;
		if ( players.find( playerID ) == players.end() )
		{
			player = new Player { playerID, playerName, std::vector<GameStats*>{}, playerHeight, playerWeight,
			0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 };
			players[ playerID ] = player;
			playerList.push_back( player );
		}
		else
			player = players[ playerID ];

		// Create a new GameStats object for this line of data
		GameStats* game = new GameStats {
			player,
			team,
			position,
			season,
			week,
			gameType,
			passAttempts,
			completedPasses,
			incompletePasses,
			passingYards,
			passingAirYards,
			passTD,
			interceptions,
			targets,
			receptions,
			receivingYards,
			receivingAirYards,
			yardsAfterCatch,
			receptionTD,
			rushAttempts,
			rushingYards,
			rushingTD,
			touches,
			totalTD,
			totalYards,
			opponent
		};

		// Add to this player's average
		player->passAttempts += game->passAttempts;
		player->completedPasses += game->completedPasses;
		player->incompletePasses += game->incompletePasses;
		player->passingYards += game->passingYards;
		player->passingAirYards += game->passingAirYards;
		player->passTD += game->passTD;
		player->interceptions += game->interceptions;
		player->targets += game->targets;
		player->receptions += game->receptions;
		player->receivingYards += game->receivingYards;
		player->receivingAirYards += game->receivingAirYards;
		player->yardsAfterCatch += game->yardsAfterCatch;
		player->receptionTD += game->receptionTD;
		player->rushAttempts += game->rushAttempts;
		player->rushingYards += game->rushingYards;
		player->rushingTD += game->rushingTD;
		player->touches += game->touches;
		player->totalTD += game->totalTD;
		player->totalYards += game->totalYards;
		++player->numDataPoints;

		// Store the game object
		games.push_back( game );
		player->games.push_back( game );

		// For debugging
        if ( i++ % 10000 == 9999 )
        	std::cout << "Loaded " << i << " games" << std::endl;
    }

    std::cout << "Done! (" << i << " games total)" << std::endl;
}

// Creates a new heap using the weights passed in
void StatsDatabase::buildHeap( std::vector<int>& weightMatrix )
{
	// Clear the old heap, if one existed
	delete[] gameHeap;
	heapSize = 0;
	gameHeap = new std::pair<GameStats*, int>[ games.size() ];

	// Add every game to the heap
	for ( GameStats* gamePtr : games )
	{
		// Start by appending it to the last slot in the heap
		std::pair<GameStats*, int> game { gamePtr, getGameScore( gamePtr, weightMatrix ) };
		gameHeap[ heapSize ] = game;

		// Continue moving the element upward while its score is greater than its parent's
		int child = heapSize++;
		int parent = ( child - 1 ) / 2;
		while ( parent >= 0 && gameHeap[ parent ].second < gameHeap[ child ].second )
		{
			std::pair<GameStats*, int> temp = gameHeap[ parent ];
			gameHeap[ parent ] = gameHeap[ child ];
			gameHeap[ child ] = temp;
			child = parent;
			parent = ( child - 1 ) / 2;
		}
	}
}

// Totals the game's stats multiplied by the user-defined weights
int StatsDatabase::getGameScore( GameStats* game, std::vector<int>& weightMatrix )
{
	return (
		game->passAttempts * weightMatrix[0] +
		game->completedPasses * weightMatrix[1] +
		game->incompletePasses * weightMatrix[2] +
		game->passingYards * weightMatrix[3] +
		game->passingAirYards * weightMatrix[4] +
		game->passTD * weightMatrix[5] +
		game->interceptions * weightMatrix[6] +
		game->targets * weightMatrix[7] +
		game->receptions * weightMatrix[8] +
		game->receivingYards * weightMatrix[9] +
		game->receivingAirYards * weightMatrix[10] +
		game->yardsAfterCatch * weightMatrix[11] +
		game->receptionTD * weightMatrix[12] +
		game->rushAttempts * weightMatrix[13] +
		game->rushingYards * weightMatrix[14] +
		game->rushingTD * weightMatrix[15] +
		game->touches * weightMatrix[16] +
		game->totalTD * weightMatrix[17] +
		game->totalYards * weightMatrix[18]
	);
}

// Extracts the highest scoring n games remaining in the heap
std::vector<GameStats*> StatsDatabase::extractGames( int count )
{
	std::vector<GameStats*> output {};

	// Run for the number of iterations, or until there are no games left
	for ( int i = 0; ( i < count && heapSize > 0 ); ++i )
	{
		output.push_back( gameHeap[0].first );
		gameHeap[0] = gameHeap[ --heapSize ];
		int parent = 0;
		int child1 = 1;
		int child2 = 2;
		while ( ( child1 < heapSize && gameHeap[ child1 ].second > gameHeap[ parent ].second )
			|| ( child2 < heapSize && gameHeap[ child2 ].second > gameHeap[ parent ].second ) )
		{
			// Find the greater child (choosing the greater child if applicable)
			int minChild;
			if ( child1 < heapSize && child2 < heapSize )
				minChild = ( gameHeap[ child1 ].second > gameHeap[ child2 ].second ) ? child1 : child2;
			else
				minChild = ( child1 < heapSize ) ? child1 : child2;

			// Swap the values
			// Then, prepare the pointers for the next iteration
            std::pair<GameStats*, int> temp = gameHeap[ parent ];
			gameHeap[ parent ] = gameHeap[ minChild ];
			gameHeap[ minChild ] = temp;
			parent = minChild;
			child1 = parent * 2 + 1;
			child2 = parent * 2 + 2;
		}
	}

	return output;
}

// Like extractGames(), but extracts every single player
// Assumes the list is already sorted
std::vector<Player*> StatsDatabase::extractPlayers()
{
	return playerList;
}

// Shorthand to use std::getline and convert to integer
int StatsDatabase::getlineInt( std::ifstream& file, char delimiter )
{
	std::string buffer;
	std::getline( file, buffer, ',' );
	return std::stoi( buffer );
}