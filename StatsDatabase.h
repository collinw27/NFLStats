#pragma once

#include <iostream>
#include <vector>
#include <fstream>
#include <unordered_map>

class StatsDatabase
{
	struct GameStats;

	struct Player
	{
		int id;
		std::string name;
		std::vector<GameStats*> games;
		int height;
		int weight;
	};

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

		void print()
		{
			std::cout << "Player: " << player->name << std::endl;
			std::cout << "Team: " << team << std::endl;
			std::cout << "Position: " << position << std::endl;
			std::cout << "Season: " << season << std::endl;
			std::cout << "Week: " << week << std::endl;
			std::cout << "Game type: " << gameType << std::endl;
			std::cout << "Pass attempts: " << passAttempts << std::endl;
			std::cout << "Complete passes: " << completedPasses << std::endl;
			std::cout << "Incomplete passes: " << incompletePasses << std::endl;
			std::cout << "Passing yards: " << passingYards << std::endl;
			std::cout << "Passing air yards: " << passingAirYards << std::endl;
			std::cout << "Passing TDs: " << passTD << std::endl;
			std::cout << "Interceptions: " << interceptions << std::endl;
			std::cout << "Targets: " << targets << std::endl;
			std::cout << "Receptions: " << receptions << std::endl;
			std::cout << "Receiving yards: " << receivingYards << std::endl;
			std::cout << "Receiving air yards: " << receivingAirYards << std::endl;
			std::cout << "Yards after catch: " << yardsAfterCatch << std::endl;
			std::cout << "Reception TDs: " << receptionTD << std::endl;
			std::cout << "Rush attempts: " << rushAttempts << std::endl;
			std::cout << "Rushing yards: " << rushingYards << std::endl;
			std::cout << "Rushing TDs: " << rushingTD << std::endl;
			std::cout << "Touches: " << touches << std::endl;
			std::cout << "Total TDs: " << totalTD << std::endl;
			std::cout << "Total yards: " << totalYards << std::endl;
			std::cout << "Opponent: " << opponent << std::endl;
		}
	};

	std::unordered_map<int, Player*> players;
	std::vector<GameStats*> games;

public:

	StatsDatabase( const std::string& filename );

	static int getlineInt( std::ifstream& file, char delimiter = ',' );
};