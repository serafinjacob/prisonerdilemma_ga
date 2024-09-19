#include "tournament.h"
#include "prisoner_dilemma.h"
#include <stdlib.h>

Tournament initialize_tournament(int rounds, char *strategy, char *opponent_strategy) {
	Tournament tournament;
	tournament.rounds = rounds;
	tournament.players = create_players(strategy, opponent_strategy, rounds);

	return tournament;
}

TournamentPlayer *create_players(char *strategy, char *opponent_strategy, int rounds) {
	TournamentPlayer *players = (TournamentPlayer *)malloc(2 * sizeof(TournamentPlayer));
	
	players[0] = create_player(strategy, true, rounds);
	players[1] = create_player(opponent_strategy, false, rounds);

	return players;
}

TournamentPlayer create_player(char *strategy, bool isGa, int rounds) {
	TournamentPlayer player;
	
	player.strategy = strategy;
	player.current_game_view = strategy + STRATEGY_LENGTH;
	player.isGa = isGa;
	player.round_results = (TournamentRound *)malloc(rounds * sizeof(TournamentRound));

	return player;
} 

void free_tournament(Tournament tournament) {
	free(tournament.players[0].round_results);
	free(tournament.players[1].round_results);
	free(tournament.players);
}

void play_tournament(Tournament tournament) {
	for (int i = 0; i < tournament.rounds; i++) {
		play_round(&tournament.players[0], &tournament.players[1], i);
	}
}

void play_round(TournamentPlayer *player, TournamentPlayer *opponent, int i) {
	char **lookup_table = create_encoding_lookup_table();
	
	// make move shouldn't rely on having to import prisoner_dilemma.h
	char player_move = make_move(player, lookup_table);
	char opponent_move = make_move(opponent, lookup_table);

	update_game_view(player->current_game_view, player_move, opponent_move);
	update_game_view(opponent->current_game_view, opponent_move, player_move);

	char round[3] = "";
	round[0] = player_move;
	round[1] = opponent_move;
	round[2] = '\0';

	// value shouldn't rely on having to import prisoner_dilemma.h
	int gaPoints = get_value(round, true);
	int uPoints = get_value(round, false);

	update_round_results(player, gaPoints, uPoints, i);
	update_round_results(opponent, uPoints, gaPoints, i);
	
	free_encoding_lookup_table(lookup_table);
}

char make_move(TournamentPlayer *player, char **lookup_table) {
	char *game = player->current_game_view;
	char *strategy = player->strategy;

	int index = get_index_of_encoding(lookup_table, game);

	return strategy[index];
}

void update_round_results(TournamentPlayer *player, int gaPoints, int uPoints, int i) {
	player->round_results[i].ga_points = gaPoints;
	player->round_results[i].u_points = uPoints;
}

void update_game_view(char *game, char player_move, char opponent_move) {
	// shift the game view to the left by 2
	for (int i = 0; i < STRATEGY_PREVIOUS_HYPOTHETICAL_LENGTH - 2; i++) {
		game[i] = game[i + 2];
	}

	// add the new moves to the end of the game view
	memcpy(game + STRATEGY_PREVIOUS_HYPOTHETICAL_LENGTH - 2, &player_move, 1);
	memcpy(game + STRATEGY_PREVIOUS_HYPOTHETICAL_LENGTH - 1, &opponent_move, 1);
}

char *get_tournament_results(Tournament tournament, int run, int strategy) {
	int gaPoints = 0;
	int uPoints = 0;
	
	int gaWins = 0;
	int uWins = 0;

	for (int i = 0; i < tournament.rounds; i++) {
		gaPoints += tournament.players[0].round_results[i].ga_points;
		uPoints += tournament.players[0].round_results[i].u_points;

		if (tournament.players[0].round_results[i].ga_points > tournament.players[0].round_results[i].u_points) {
			gaWins++;
		} else if (tournament.players[0].round_results[i].ga_points < tournament.players[0].round_results[i].u_points) {
			uWins++;
		}
	}

	char *results = (char *)malloc(100 * sizeof(char));
	sprintf(results, "%d, %d, %d, %d, %d, %d", run, strategy, gaPoints, uPoints, gaWins, uWins);

	return results;
}