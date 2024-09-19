#include "prisoner_dilemma.h"

char **create_encoding_lookup_table() {
	char **lookup_table = (char **) malloc(STRATEGY_LENGTH * sizeof(char *));

	for (int i = 0; i < STRATEGY_LENGTH; i++) {
		lookup_table[i] = (char *) malloc(7 * sizeof(char));
	}

	for (int i = 0; i < STRATEGY_LENGTH; i++) {
		char game[6];
		for (int j = 0; j < 3; j++) {
			game[j * 2] = (i & (1 << j)) ? 'D' : 'C';
			game[j * 2 + 1] = (i & (1 << (j + 3))) ? 'D' : 'C';
		}
		strcpy(lookup_table[i], game);
	}

	return lookup_table;
}

void free_encoding_lookup_table(char **lookup_table) {
	for (int i = 0; i < STRATEGY_LENGTH; i++) {
		free(lookup_table[i]);
	}
	free(lookup_table);
}

int get_index_of_encoding(char **lookup_table, char *game) {
	for (int i = 0; i < STRATEGY_LENGTH; i++) {
		if (strcmp(lookup_table[i], game) == 0) {
			return i;
		}
	}
	return -1;
}

char *create_random_pd_strategy() {
	char *strategy = (char *)malloc(STRATEGY_LENGTH + STRATEGY_PREVIOUS_HYPOTHETICAL_LENGTH + 1 * sizeof(char));

	for (int i = 0; i < STRATEGY_LENGTH + STRATEGY_PREVIOUS_HYPOTHETICAL_LENGTH; i++) {
		strategy[i] = (rand() % 2 == 0) ? 'C' : 'D';
	}

	strategy[STRATEGY_LENGTH + STRATEGY_PREVIOUS_HYPOTHETICAL_LENGTH] = '\0';

	return strategy;
}

char mutate_pd_strategy(char locus) {
	return (locus == 'C') ? 'D' : 'C';
}

int evaluate_pd_strategy(char *strategy) {
	int fitness = 0;
	char **lookup_table = create_encoding_lookup_table();

	// with the selected strategy, play a game for each of the 64 possible 3 turn encodings
	for (int i = 0; i < STRATEGY_LENGTH; i++) {
		char game[7];
		strcpy(game, lookup_table[i]);

		// play the hypothetical game attached to the strategy
		for (int j = 0; j < STRATEGY_PREVIOUS_HYPOTHETICAL_LENGTH; j++) {
			char round[3] = "";
			round[0] = strategy[STRATEGY_LENGTH + j];
			round[1] = strategy[STRATEGY_LENGTH + j + 1];
			round[2] = '\0';

			fitness += get_value(round, true) - get_value(round, false);
		}

		// lookup what the strategy says the next move should be
		char ga_move = get_move(game, strategy, lookup_table);
		
		// the opponent wants to choose a move that will give the ga the least points - assume they are playing optimally
		// if the ga chooses C then the opponent should also C since the ga will get 3 points instead of 5
		// if the ga chooses D then the opponent should C since the ga will get 0 point instead of 1 (since the ga will get 1 point if the opponent D)
		char opponent_move = (ga_move == 'C') ? 'D' : 'C';

		char round[3] = "";
		round[0] = ga_move;
		round[1] = opponent_move;
		round[2] = '\0';

		fitness += get_value(round, true);

	}
	free_encoding_lookup_table(lookup_table);

	// if the fitness is negative, set it to 0
	if (fitness < 0) {
		fitness = 0;
	}

	return fitness;
}

int get_value(char *round, bool ga) {
	if (strcmp(round, "CC") == 0) {
		return 3;
	} else if (strcmp(round, "CD") == 0) {
		if (ga) {
			return 5;
		} 
		return 0;
	} else if (strcmp(round, "DC") == 0) {
		if (ga) {
			return 0;
		}
		return 5;
	} else if (strcmp(round, "DD") == 0) {
		return 1;
	}

	return -1;
}

char get_move(char *game, char *strategy, char **lookup_table) {
	int index = get_index_of_encoding(lookup_table, game);
	return strategy[index];
}