#pragma once

#include <iostream>
#include <vector>
#include <fstream>
#include <unordered_set>
#include <unordered_map>

struct GameStats;

// Stores data for each player that's consistent across seasons
// Also stores an aggregate of their game stats
struct Player
{
	int id;
	std::string name;
	std::vector<GameStats*> games;
	int height;
	int weight;

	// Store an aggregate of stats to compute an average
	int passAttempts;
	int completedPasses;
	int incompletePasses;
	int passingYards;
	int passingAirYards;
	int passTD;
	int interceptions;
	int targets;
	int receptions;
	int receivingYards;
	int receivingAirYards;
	int yardsAfterCatch;
	int receptionTD;
	int rushAttempts;
	int rushingYards;
	int rushingTD;
	int touches;
	int totalTD;
	int totalYards;
	int numDataPoints;
};

// An aggregate of stats for each player for each game
struct GameStats
{
	Player* player;
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
};

class StatsDatabase
{
	// Each player is stored using their ID for easy access
	// Games are stored in a single list
	std::unordered_map<int, Player*> players;
	std::vector<GameStats*> games;

	// Store the last heap that was created/used
	// This way, more games can be extracted without needing to rebuild the heap
	std::pair<GameStats*, int>* gameHeap = nullptr;
	int heapSize = 0;

	// Store a sorted vector containing all players
	// Can be sorted by different stats
	std::vector<Player*> playerList;

public:

	StatsDatabase( const std::string& filename );

	// Games: Searching & heap building
	void buildHeap( std::vector<int>& weightMatrix );
	std::vector<GameStats*> extractGames( int count );
	int getGameScore( GameStats* game, std::vector<int>& weightMatrix );

	// Players: List sorting & searching
	std::vector<Player*> extractPlayers();

	// Shorthand to use std::getline and convert to integer
	static int getlineInt( std::ifstream& file, char delimiter = ',' );
};