#ifndef PRISONER_DILEMMA_H
#define PRISONER_DILEMMA_H

#define STRATEGY_LENGTH 64
#define STRATEGY_PREVIOUS_HYPOTHETICAL_LENGTH 6 

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <stdbool.h>

char **create_encoding_lookup_table();
void free_encoding_lookup_table(char **lookup_table);
int get_index_of_encoding(char **lookup_table, char *game);

char *create_random_pd_strategy();
char mutate_pd_strategy(char locus); // may want to consider passing the strategy and the locus index instead - would allow for more flexibility such as using other loci to determine the mutation
int evaluate_pd_strategy(char *strategy);

char get_move(char *game, char *strategy, char **lookup_table);
int get_value(char *round, bool ga);

#endif