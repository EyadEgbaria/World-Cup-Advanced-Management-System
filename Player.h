//
// Created by ameer on 1/4/2023.
//

#ifndef UNTITLED2_PLAYER_H
#define UNTITLED2_PLAYER_H

#include "wet2util.h"
#include <memory>

using std::shared_ptr;
class Node;

class Player {
public:
        int id;
        int teamId;
        permutation_t spirit;
        int gamesPlayed;
        int cards;
        bool goalKeeper;
        permutation_t partial_spirit;
        shared_ptr<Player> next;
        shared_ptr<Player> father;
        shared_ptr<Node> node;

    Player(int id ,int teamId ,permutation_t spirit,int gamesPlayed,int cards,bool goalKeeper,permutation_t partial_spirit):id(id),teamId(teamId),spirit(spirit),gamesPlayed(gamesPlayed),cards(cards),goalKeeper(goalKeeper),partial_spirit(partial_spirit),next(
            nullptr),father(nullptr),node(nullptr) {};

};

#endif //UNTITLED2_PLAYER_H
