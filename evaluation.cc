// *******************************************************************
//   (C) Copyright 2013 Leiden Institute of Advanced Computer Science
//   Universiteit Leiden
//   All Rights Reserved
// *******************************************************************
// Kunstmatige Intelligentie --- Jungle
// *******************************************************************
// FILE INFORMATION:
//   File:     evaluation.cc (Depends on: position.h)
//   Author:   Tim van der Meij (1115731) and Simon Klaver (1140760)
//   Revision: 1.0
//   Date:     2013/04/15
// *******************************************************************
// DESCRIPTION:
//   Implementatie van de evaluatiefunctie. Deze functie moet een
//   geheel getal opleveren op basis van de positie op het bord.
//   De evaluatie wordt vanuit het standpunt van de witte speler
//   bekeken: een groter (positief) getal geeft een voordeel voor wit.
//   Gebruikelijk is dat deze functie symmetrisch is rond 0. Dat wil
//   zeggen, indien een positie wordt gewaardeerd voor wit met x,
//   wordt deze positie voor zwart met -x gewaardeerd.
//   Een aantal factoren kunnen in deze evaluatiefunctie worden
//   opgenomen, waarbij direct valt te denken aan de relatieve sterkte
//   van de zich nog op het bord bevindende stukken en hun locaties
//   op het bord. Bijvoorbeeld een stuk dichtbij de `den' van de
//   tegenstander lijkt gunstig.
// *******************************************************************

#include "position.h"
#include <cmath>

// *******************************************************************
// MEMBER evaluate: Evaluates a given board.
// *******************************************************************
int Position::evaluate() const {
    uint32_t piece;
    int sumWhite = 0, sumBlack = 0, distance, minDistanceWhite = 100,
        minDistanceBlack = 100, i, numPiecesWhite = 0, numPiecesBlack = 0, den;

	if(is_won()) {
		return 10000;
	}
    for(i = 0; i < BOARD_SIZE; i++) {
        piece = _board[i];
        if(piece != NONE) {
            // Calculate the total value of the pieces on the board, the total
            // number of pieces for each player and the minimal distance to the
            // den. All these criteria will have an influence on the final result.
            if(is_white(piece)) {
                numPiecesWhite++;
                sumWhite += strength(piece);
                den = (int)LOCATION_BLACK_DEN;
                // Calculation necessary since we do not have (x,y) coordinates,
                // but instead the number of a square on the board.
                distance = (std::abs(i - den) / 7) + ((int)std::abs(i - den) % 7);
                if(distance < minDistanceWhite) {
                    minDistanceWhite = distance;
                }
            } else {
                numPiecesBlack++;
                sumBlack += strength(piece);
                den = (int)LOCATION_WHITE_DEN;
                distance = (std::abs(i - den) / 7) + ((int)std::abs(i - den) % 7);
                if(distance < minDistanceBlack) {
                    minDistanceBlack = distance;
                }
            }
        }
    }
    // The board is 7x9, so a piece in the bottom left corner of the
    // board can traverse at most (4-1) + (9-1) = 11 squares to get
    // to the den at the top center of the board.
	if(is_white_turn()) {
	    return (sumWhite - sumBlack) + ((11 - minDistanceWhite) * numPiecesWhite)
	           - ((11 - minDistanceBlack) * numPiecesBlack);
	}
	return (sumBlack - sumWhite) + ((11 - minDistanceBlack) * numPiecesBlack)
	       - ((11 - minDistanceWhite) * numPiecesWhite);
}
