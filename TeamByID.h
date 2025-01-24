//
// Created by ameer on 1/4/2023.
//

#ifndef UNTITLED2_TEAMBYID_H
#define UNTITLED2_TEAMBYID_H




#include "wet2util.h"
#include "Player.h"
#include <memory>
class Node;
using std::shared_ptr;

class TeamByID {
public:
    int id;
    int ability;
    int points;
    int size;
    int num_goalkeeprs;
    shared_ptr<Node> active;
    permutation_t team_sp;

    TeamByID(int id):id(id)
            ,ability(0)
            ,points(0)
            ,size(0)
            ,num_goalkeeprs(0)
            ,active(nullptr)
            ,team_sp(permutation_t::neutral())
    {};
    bool operator==(const TeamByID& Team_a){
        return (this->id == Team_a.id) ;
    };
    bool operator!=(const TeamByID& Team_a){
        return (this->id != Team_a.id) ;
    };
    bool operator>(const TeamByID& Team_a ){
        return (this->id > Team_a.id) ;
    };
    bool operator<(const TeamByID& Team_a ){
        return (this->id < Team_a.id) ;
    };
    bool operator>=(const TeamByID& Team_a){
        return (this->id >= Team_a.id) ;
    };
    bool operator<=(const TeamByID& Team_a){
        return (this->id <= Team_a.id) ;
    };




};

class Node {
public:
    shared_ptr<Player> root_player;
    shared_ptr<TeamByID> team;
    bool is_active;
    int games_played;

    Node()
    {
        games_played = 0 ;
        root_player = nullptr;
        team = nullptr;
        is_active = true;
    };
    Node (const Node& tree_node) = default;
    ~Node() = default;

};



#endif //UNTITLED2_TEAMBYID_H
