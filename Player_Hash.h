//
// Created by ameer on 1/4/2023.
//

#ifndef UNTITLED2_PLAYER_HASH_H
#define UNTITLED2_PLAYER_HASH_H
#define LOAD_FACTOR 2
#define INITIAL_ELEMENTS 0

#include "ArrayHash.h"
#include "Player.h"

using std::shared_ptr;

class Player_Hash {
public:
    ArrayHash<shared_ptr<Player>> data;
    int num_of_elements;

    static void Insert_in_array(ArrayHash<shared_ptr<Player>>& data, shared_ptr<Player> player);


    Player_Hash():num_of_elements(INITIAL_ELEMENTS){
        for (int i = 0; i < data.size; i++)
        {
            data[i]=nullptr;
        }
    };
    void Insert(shared_ptr<Player> player);
    shared_ptr<Player> Search(int player_id);



};


#endif //UNTITLED2_PLAYER_HASH_H
