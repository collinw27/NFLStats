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
			player = new Player { playerID, playerName, std::vector<GameStats*>{}, playerHeight, playerWeight };
			players[ playerID ] = player;
		}
		else
			player = players[ playerID ];

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
		games.push_back( game );
		player->games.push_back( game );
        if ( i++ % 10000 == 9999 )
        	std::cout << "Loaded " << i << " games" << std::endl;
    }

    std::cout << "Done! (" << i << " games total)" << std::endl;
}

int StatsDatabase::getlineInt( std::ifstream& file, char delimiter )
{
	std::string buffer;
	std::getline( file, buffer, ',' );
	return std::stoi( buffer );
}