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
			player = new Player { playerID, playerName,
				std::unordered_set<std::string>{}, std::unordered_set<std::string>{}, playerHeight, playerWeight,
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
			opponent,
            0
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
		player->teams.insert( game->team );
		player->positions.insert( game->position );

		// Store the game object
		games.push_back( game );

		// For debugging
        if ( i++ % 10000 == 9999 )
        	std::cout << "Loaded " << i << " games" << std::endl;
    }

    std::cout << "Done! (" << i << " games total, " << players.size() << " players total)" << std::endl;
}

// Deallocate dynamic memory from the heap
StatsDatabase::~StatsDatabase()
{
	delete[] gameHeap;
	for ( auto player : players )
		delete player.second;
	for ( auto game : games )
		delete game;
}

// Creates a new heap using the weights passed in
void StatsDatabase::buildGameHeap( std::vector<int>& weightMatrix )
{
	// Clear the old heap, if one existed
	delete[] gameHeap;
	heapSize = 0;
	gameHeap = new GameStats*[ games.size() ];

	// Add every game to the heap
	for ( GameStats* game : games )
	{
		// Start by appending it to the last slot in the heap
		game->score = getGameScore( game, weightMatrix );
		gameHeap[ heapSize ] = game;

		// Continue moving the element upward while its score is greater than its parent's
		int child = heapSize++;
		int parent = ( child - 1 ) / 2;
		while ( parent >= 0 && gameHeap[ parent ]->score < gameHeap[ child ]->score )
		{
			GameStats* temp = gameHeap[ parent ];
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
		output.push_back( gameHeap[0] );
		gameHeap[0] = gameHeap[ --heapSize ];
		int parent = 0;
		int child1 = 1;
		int child2 = 2;
		while ( ( child1 < heapSize && gameHeap[ child1 ]->score > gameHeap[ parent ]->score )
			|| ( child2 < heapSize && gameHeap[ child2 ]->score > gameHeap[ parent ]->score ) )
		{
			// Find the greater child (choosing the greater child if applicable)
			int minChild;
			if ( child1 < heapSize && child2 < heapSize )
				minChild = ( gameHeap[ child1 ]->score > gameHeap[ child2 ]->score ) ? child1 : child2;
			else
				minChild = ( child1 < heapSize ) ? child1 : child2;

			// Swap the values
			// Then, prepare the pointers for the next iteration
            GameStats* temp = gameHeap[ parent ];
			gameHeap[ parent ] = gameHeap[ minChild ];
			gameHeap[ minChild ] = temp;
			parent = minChild;
			child1 = parent * 2 + 1;
			child2 = parent * 2 + 2;
		}
	}

	return output;
}

void StatsDatabase::sortPlayers( std::vector<int>& weightMatrix )
{
	// First, assign a score to each player
	for ( auto player : players )
		player.second->score = getPlayerScore( player.second, weightMatrix );

	// Then, recursively run merge sort
    mergeSortPlayers( 0, players.size() - 1 );
}

// Recursively calls merge sort on the players vector
void StatsDatabase::mergeSortPlayers( int left, int right )
{
	// Once left and right reach each other, stop splitting (only one element)
	if ( left < right )
	{
		int middle = left + ( right - left ) / 2;
		mergeSortPlayers( left, middle );
		mergeSortPlayers( middle + 1, right );

		// Create 2 sub arrays (adapted from lecture slides)
		int n1 = middle - left + 1;
		int n2 = right - middle;
		Player** sub1 = new Player*[n1];
		Player** sub2 = new Player*[n2];
		for ( int i = 0; i < n1; ++i )
			sub1[i] = playerList[ left + i ];
		for ( int i = 0; i < n2; ++i )
			sub2[i] = playerList[ middle + 1 + i ];

		// Continue merging the sub arrays
		int i = 0;
		int j = 0;
		int k = left;
		while ( i < n1 && j < n2 )
		{
			if ( sub1[i]->score >= sub2[j]->score )
				playerList[k] = sub1[i++];
			else
				playerList[k] = sub2[j++];
			++k;
		}

		// Append the remaining elements
		while ( i < n1 )
			playerList[k++] = sub1[i++];
		while ( j < n2 )
			playerList[k++] = sub2[j++];

		// Deallocate arrays
		delete[] sub1;
		delete[] sub2;
	}
}

// Like extractGames(), but extracts every single player
// Assumes the list is already sorted
std::vector<Player*> StatsDatabase::extractPlayers()
{
	return playerList;
}

// Totals the player's stats multiplied by the user-defined weights
// The stats are averaged across all the games they played in
float StatsDatabase::getPlayerScore( Player* player, std::vector<int>& weightMatrix )
{
	return (
		player->passAttempts * ( weightMatrix[0] / player->numDataPoints ) +
		player->completedPasses * ( weightMatrix[1] / player->numDataPoints ) +
		player->incompletePasses * ( weightMatrix[2] / player->numDataPoints ) +
		player->passingYards * ( weightMatrix[3] / player->numDataPoints ) +
		player->passingAirYards * ( weightMatrix[4] / player->numDataPoints ) +
		player->passTD * ( weightMatrix[5] / player->numDataPoints ) +
		player->interceptions * ( weightMatrix[6] / player->numDataPoints ) +
		player->targets * ( weightMatrix[7] / player->numDataPoints ) +
		player->receptions * ( weightMatrix[8] / player->numDataPoints ) +
		player->receivingYards * ( weightMatrix[9] / player->numDataPoints ) +
		player->receivingAirYards * ( weightMatrix[10] / player->numDataPoints ) +
		player->yardsAfterCatch * ( weightMatrix[11] / player->numDataPoints ) +
		player->receptionTD * ( weightMatrix[12] / player->numDataPoints ) +
		player->rushAttempts * ( weightMatrix[13] / player->numDataPoints ) +
		player->rushingYards * ( weightMatrix[14] / player->numDataPoints ) +
		player->rushingTD * ( weightMatrix[15] / player->numDataPoints ) +
		player->touches * ( weightMatrix[16] / player->numDataPoints ) +
		player->totalTD * ( weightMatrix[17] / player->numDataPoints ) +
		player->totalYards * ( weightMatrix[18] / player->numDataPoints )
	);
}

// Shorthand to use std::getline and convert to integer
int StatsDatabase::getlineInt( std::ifstream& file, char delimiter )
{
	std::string buffer;
	std::getline( file, buffer, ',' );
	return std::stoi( buffer );
}