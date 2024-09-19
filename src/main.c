#include "genetic.h"
#include "strategy.h"
#include "prisoner_dilemma.h"
#include "tournament.h"
#include "log.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <sys/stat.h>

int main ( int argc, char *argv[] ) {
	set_strategy_functions(create_random_pd_strategy, mutate_pd_strategy, evaluate_pd_strategy);

	char filename[100];
	char directory[100];
	char message[256];

	// time and date for logging
	time_t t = time(NULL);
	struct tm tm = *localtime(&t);

	sprintf(directory, "output/%d-%d-%d-%d-%d-%d", tm.tm_year + 1900, tm.tm_mon + 1, tm.tm_mday, tm.tm_hour, tm.tm_min, tm.tm_sec);
	create_directories(directory);

	for (int n = 0; n < RUNS; n++) {
		Strategy best_strategy;
		best_strategy.strategy = (char *)malloc(STRATEGY_LENGTH + STRATEGY_PREVIOUS_HYPOTHETICAL_LENGTH + 1 * sizeof(char));

		Strategy *initial_strategies = init_population();
		Strategy **generations = (Strategy **)malloc(GENERATIONS * sizeof(Strategy *));
		
		execute(&best_strategy, initial_strategies, generations);

		// print the average fitness of the all the generations
		for (int i = 0; i < GENERATIONS; i++) {
			int total_fitness = get_population_fitness(generations[i]);
			
			sprintf(directory, "output/%d-%d-%d-%d-%d-%d/run-%d", tm.tm_year + 1900, tm.tm_mon + 1, tm.tm_mday, tm.tm_hour, tm.tm_min, tm.tm_sec, n+1);
			create_directories(directory);
			sprintf(filename, "generation-%d.csv", i);
			for (int j = 0; j < POPULATION_SIZE; j++) {
				sprintf(message, "%d, %d, %s", j, generations[i][j].fitness, generations[i][j].strategy);
				write_to_log_file(directory, filename, message);
			}
			
			// change filename to store the average fitness of each generation
			sprintf(directory, "output/%d-%d-%d-%d-%d-%d", tm.tm_year + 1900, tm.tm_mon + 1, tm.tm_mday, tm.tm_hour, tm.tm_min, tm.tm_sec);
			create_directories(directory);
			sprintf(filename, "run-%d-fitness.csv", n+1);
			sprintf(message, "%d, %d", i, total_fitness / POPULATION_SIZE);
			write_to_log_file(directory, filename, message);
		}


		sprintf(filename, "results.csv");
		sprintf(directory, "output/%d-%d-%d-%d-%d-%d/", tm.tm_year + 1900, tm.tm_mon + 1, tm.tm_mday, tm.tm_hour, tm.tm_min, tm.tm_sec);

		for (int i = 0; i < POPULATION_SIZE; i++) {
			int rounds = 5;
			
			char *gaStrategy = best_strategy.strategy;
			char *ustrategy = initial_strategies[i].strategy;
			
			Tournament tournament = initialize_tournament(rounds, gaStrategy, ustrategy);
			play_tournament(tournament);

			char *results = get_tournament_results(tournament, n, i);
			strcpy(message, results);
			write_to_log_file(directory, filename, message);
			
			free(results);
			free_tournament(tournament);
		}

		
		free(best_strategy.strategy);
		free_population(initial_strategies);

		for (int i = 0; i < GENERATIONS; i++) {
			free_population(generations[i]);
		}
		free(generations);
	}

	return 0;
}