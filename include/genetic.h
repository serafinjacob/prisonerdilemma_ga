#ifndef GENETIC_H
#define GENETIC_H

#include "strategy.h" // defines s_functions - REQUIRED
#include <time.h>
#include <stdlib.h>
#include <string.h>
#include <stdio.h>

#define POPULATION_SIZE 20
#define GENERATIONS 50
#define RUNS 40

#define CROSSOVER_RATE 0.7
#define MUTATION_RATE 0.001

typedef struct {
	char *strategy;
	int fitness;
} Strategy;

void execute(Strategy *best_strategy, Strategy *initial_population, Strategy **generations);

void selection(Strategy *population, Strategy *offspring1, Strategy *offspring2);
void crossover(Strategy *offspring1, Strategy *offspring2);
void mutate(Strategy *offspring);

int fitness(Strategy *strategy);

Strategy *init_population();
void free_population(Strategy *population);
Strategy *create_offspring_population(Strategy *population);
Strategy select_from_population(Strategy *population, double *selection_probabilities);
void store_generation_of_population(Strategy *population, Strategy **generations, int generation);
void update_population(Strategy *population, Strategy *offspring_population);
int get_population_fitness(Strategy *population);

void get_best_strategy(Strategy *population, Strategy *best_strategy);

#endif