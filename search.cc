// *******************************************************************
//   (C) Copyright 2013 Leiden Institute of Advanced Computer Science
//   Universiteit Leiden
//   All Rights Reserved
// *******************************************************************
// Kunstmatige Intelligentie --- Jungle
// *******************************************************************
// FILE INFORMATION:
//   File:     search.cc (Depends on: search.h)
//   Author:   Tim van der Meij (1115731) and Simon Klaver (1140760)
//   Revision: 1.0
//   Date:     2013/04/15
// *******************************************************************
// DESCRIPTION:
//   Implementatie van de zoekalgoritmen.
//   2013/01/28 ADDED: variabelen voor het bijhouden van het aantal
//                     bezochte knopen.
// *******************************************************************

#include "search.h"
#include <climits>

uint64_t node_count = 0;
uint64_t leaf_count = 0;

// *******************************************************************
// MEMBER negamax: Implementation of the negamax algorithm.
// Used to determine the best move for the current player.
// *******************************************************************
int negamax(Position &position, int const depth, Move &move) {
    int maxValue = -INT_MAX, i, value;
    Move_list moveList;
    Move bestMove, doMove, undoMove;

    if(depth == 0 || position.is_won() || position.last_turn() ||
       position.no_more_moves() || position.is_threefold_repetition()) {
        return position.evaluate();
    }
    position.generate_moves(moveList);
    for(i = 0; i < moveList.size(); i++) {
        doMove = moveList.move(i);
        position.do_move(doMove);
        // We need the next line to undo the move from above,
        // because the recursive 'negamax' call can change the
        // move argument.
        undoMove = doMove;
        value = -negamax(position, depth-1, doMove);
        if(value > maxValue) {
            maxValue = value;
            bestMove = undoMove;
        }
        position.undo_move(undoMove);
    }
    move = bestMove;
    return maxValue;
}

// *******************************************************************
// MEMBER alphabeta: Implementation of the alpha-beta pruning algorithm.
// Used to determine the best move for the current player faster than
// negamax by pruning search tree branches if possible.
// *******************************************************************
int alphabeta(Position &position, int depth, int alpha,
              int beta, Move &move) {
    int maxValue = -INT_MAX, i, value;
    Move_list moveList;
    Move bestMove, doMove, undoMove;

    if(depth == 0 || position.is_won() || position.last_turn() ||
       position.no_more_moves() || position.is_threefold_repetition()) {
        return position.evaluate();
    }
    position.generate_moves(moveList);
    for(i = 0; i < moveList.size(); i++) {
        doMove = moveList.move(i);
        position.do_move(doMove);
        undoMove = doMove;
        value = -alphabeta(position, depth-1, -beta, -alpha, doMove);
        if(value > maxValue) {
            maxValue = value;
            move = bestMove = undoMove;
        }
        position.undo_move(undoMove);
        if(value >= beta) {
            return value;
        } else if(value > alpha) {
            alpha = value;
        }
    }
    return maxValue;
}
