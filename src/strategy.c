#include "strategy.h"

void set_strategy_functions(char* (*create)(), char (*mutate)(char locus), int (*evaluate)(char *strategy)) {
	s_functions.create_random = create;
	s_functions.mutate = mutate;
	s_functions.evaluate = evaluate;
}