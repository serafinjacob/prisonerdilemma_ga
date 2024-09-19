#ifndef TOURNAMENT_H
#define TOURNAMENT_H

#include <stdbool.h>

typedef struct {
	int ga_points;
	int u_points;
} TournamentRound;

typedef struct {
	char *strategy;
	char *current_game_view;
	bool isGa;
	TournamentRound *round_results;
} TournamentPlayer;

typedef struct {
	int rounds;
	TournamentPlayer *players;
} Tournament;

Tournament initialize_tournament(int rounds, char *strategy, char *opponent_strategy);
void free_tournament();

TournamentPlayer *create_players(char *strategy, char *opponent_strategy, int rounds);
TournamentPlayer create_player(char *strategy, bool isGa, int rounds);

void play_tournament(Tournament tournament);
void play_round(TournamentPlayer *player, TournamentPlayer *opponent, int i);

char make_move(TournamentPlayer *player, char **lookup_table);
void update_round_results(TournamentPlayer *player, int gaPoints, int uPoints, int i);
void update_game_view(char *game, char player_move, char opponent_move);

char *get_tournament_results(Tournament tournament, int run, int strategy);

#endif