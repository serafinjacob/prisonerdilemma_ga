#ifndef STRATEGIES_H
#define STRATEGIES_H

typedef struct {
	char* (*create_random)();
	char (*mutate)(char locus);
	int (*evaluate)(char *strategy);
} Strategy_Functions;

Strategy_Functions s_functions;

/*
	* Strategy_Functions is a struct that contains the functions that are used to create, mutate and evaluate a strategy
	* create_random returns a pointer to a random strategy
	* mutate takes a locus and returns the mutated locus
	* evaluate takes a strategy and returns the fitness of the strategy

	* implementing these functions outside of genetic.c allows for the use of:
		* different strategy encodings
		* locus mutation methods
		* fitness functions
	* without changing the genetic algorithm itself
*/
void set_strategy_functions(char* (*create)(), char (*mutate)(char locus), int (*evaluate)(char *strategy));

#endif
