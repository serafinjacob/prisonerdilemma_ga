#include "genetic.h"

// one run of the genetic algorithm
void execute(Strategy *best_strategy, Strategy *initial_population, Strategy **generations) {
	// check if the functions are set
	if (s_functions.create_random == NULL || s_functions.mutate == NULL || s_functions.evaluate == NULL) {
		return;
	}

	// copy the initial population to the population
	Strategy *population = (Strategy *)malloc(POPULATION_SIZE * sizeof(Strategy));
	for (int i = 0; i < POPULATION_SIZE; i++) {
		population[i].strategy = (char *)malloc(strlen(initial_population[i].strategy) + 1 * sizeof(char));
		strcpy(population[i].strategy, initial_population[i].strategy);
		population[i].fitness = initial_population[i].fitness;
	}

	srand(time(NULL));
	
	for (int i = 0; i < GENERATIONS; i++) {
		store_generation_of_population(population, generations, i);
		
		// each generations goal is to replace the current population with a new population
		Strategy *population_offsprings = create_offspring_population(population);
		update_population(population, population_offsprings);
	}	
		
	get_best_strategy(population, best_strategy);
	free_population(population);
}

void store_generation_of_population(Strategy *population, Strategy **generations, int generation) {
	Strategy *generation_population = (Strategy *)malloc(POPULATION_SIZE * sizeof(Strategy));
	
	for (int i = 0; i < POPULATION_SIZE; i++) {
		generation_population[i].strategy = (char *)malloc(strlen(population[i].strategy) + 1 * sizeof(char));
		strcpy(generation_population[i].strategy, population[i].strategy);
		generation_population[i].fitness = population[i].fitness;
	}
	
	generations[generation] = generation_population;
}

void selection(Strategy *population, Strategy *offspring1, Strategy *offspring2) {
	int total_fitness = get_population_fitness(population);
	
	double *selection_probabilities = (double *)malloc(POPULATION_SIZE * sizeof(double));
	for (int i = 0; i < POPULATION_SIZE; i++) {
		selection_probabilities[i] = (double)population[i].fitness / total_fitness;
	}
	
	*offspring1 = select_from_population(population, selection_probabilities);
	*offspring2 = select_from_population(population, selection_probabilities);

	free(selection_probabilities);
}

void crossover(Strategy *offspring1, Strategy *offspring2) {
	
	// makes huge assumption that both strategies are the same length
	// and that the strategy is a string
	int strategy_length = strlen(offspring1->strategy); 
	
	
	if (rand() % 100 < CROSSOVER_RATE * 100) {
		int crossover_point = rand() % strategy_length;
		for (int i = crossover_point; i < strategy_length; i++) {
			char temp = offspring1->strategy[i];
			offspring1->strategy[i] = offspring2->strategy[i];
			offspring2->strategy[i] = temp;
		}
	}

	offspring1->fitness = fitness(offspring1);
	offspring2->fitness = fitness(offspring2);
}

void mutate(Strategy *offspring) {
	// makes assumption that the strategy is a string
	int strategy_length = strlen(offspring->strategy);
	
	for (int i = 0; i < strategy_length; i++) {
		if (rand() % 100 < MUTATION_RATE * 100) {
			offspring->strategy[i] = s_functions.mutate(offspring->strategy[i]);
		}
	}
	
	offspring->fitness = fitness(offspring);
}

Strategy *init_population() {
	Strategy *population = (Strategy *)malloc(POPULATION_SIZE * sizeof(Strategy));

	for (int i = 0; i < POPULATION_SIZE; i++) {
		population[i].strategy = s_functions.create_random();
		population[i].fitness = fitness(&population[i]);
	}
	return population;
}

void free_population(Strategy *population) {
	for (int i = 0; i < POPULATION_SIZE; i++) {
		free(population[i].strategy);
	}
	free(population);
}

int fitness(Strategy *strategy) { 
	return s_functions.evaluate(strategy->strategy);
}

int get_population_fitness(Strategy *population) {
	int total_fitness = 0;
	for (int i = 0; i < POPULATION_SIZE; i++) {
		total_fitness += population[i].fitness;
	}
	return total_fitness;
}

Strategy select_from_population(Strategy *population, double *selection_probabilities) {
	int selected_strategy = -1;
	int timeOut = 0;
	
	while (selected_strategy == -1) {
		double r = (double)rand() / RAND_MAX;
		
		for (int i = 0; i < POPULATION_SIZE; i++) {
			r -= selection_probabilities[i];
			if (r <= 0) {
				selected_strategy = i;
				break;
			}

			
		}
		if (timeOut++ > 1000) {
			int random = rand() % POPULATION_SIZE;
			selected_strategy = random;
			break;
		}
	}



	return population[selected_strategy];
}

Strategy *create_offspring_population(Strategy *population) {
	Strategy *offspring_population = (Strategy *)malloc(POPULATION_SIZE * sizeof(Strategy));
	
	int offspring_created = 0;

	while (offspring_created != POPULATION_SIZE) {
		Strategy offspring1, offspring2;
		
		selection(population, &offspring1, &offspring2);
		
		crossover(&offspring1, &offspring2);
		
		mutate(&offspring1);
		mutate(&offspring2);

		offspring_population[offspring_created++] = offspring1;
		offspring_population[offspring_created++] = offspring2;
	}

	return offspring_population;
}

void update_population(Strategy *population, Strategy *offspring_population) {
	// sets the new strategies as the current strategies
	for (int i = 0; i < POPULATION_SIZE; i++) {
		if (population[i].strategy != offspring_population[i].strategy) {
			strcpy(population[i].strategy, offspring_population[i].strategy);
			population[i].fitness = offspring_population[i].fitness;
		}
	}

	free(offspring_population);
}


void get_best_strategy(Strategy *population, Strategy *best_strategy) {
	strcpy(best_strategy->strategy, population[0].strategy);
	best_strategy->fitness = population[0].fitness;

	for (int i = 1; i < POPULATION_SIZE; i++) {
		if (population[i].fitness > best_strategy->fitness) {
			strcpy(best_strategy->strategy, population[i].strategy);
			best_strategy->fitness = population[i].fitness;
		}
	}
}