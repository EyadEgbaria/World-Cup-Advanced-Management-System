//
// Created by ameer on 1/4/2023.
//

#include "Player_Hash.h"

void Player_Hash::Insert_in_array(ArrayHash<shared_ptr<Player>>& data, shared_ptr<Player> player)
{
    if (player == nullptr)
    {
        return ;
    }
    int index=(player->id)%data.size;
    player->next = data[index];
    if(data[index] != nullptr)
    {
        data[index] = player;
    }
    data[index]=player;
    return ;
}
//////change return///////////////////////////////////////////
void Player_Hash::Insert(shared_ptr<Player> player)
{
    if (player == nullptr)
    {
        return ;
    }
    if(Search(player->id) != nullptr){
        return ;
    }
    Insert_in_array(data,player);
    num_of_elements++;
    if (double(num_of_elements/data.size) > LOAD_FACTOR )
    {
        int old_size=data.size;
        int new_size=old_size*2;
        ArrayHash<shared_ptr<Player>> new_data(new_size);
        for (int i = 0; i < new_size; i++)
        {
            new_data[i]= nullptr;
        }
        for (int i = 0; i < old_size; i++)
        {
            shared_ptr<Player> it=data[i];
            while (it != nullptr)
            {
                shared_ptr<Player> temp_next=it->next;
                if(temp_next == nullptr){
                    Insert_in_array(new_data, it);
                    data[i]= nullptr;
                    break;
                }
                data[i]=temp_next;
                it->next = nullptr;
                Insert_in_array(new_data, it);
                it=temp_next;
            }

        }
        this->data = new_data;
    }
    return;
}

shared_ptr<Player> Player_Hash::Search(int player_id)
{
    int index= player_id% data.size;
    if(data[index] == nullptr){
        return nullptr;
    }
    shared_ptr<Player> it=data[index];
    while (it != nullptr)
    {
        if(it->id == player_id){
            return it;
        }
        it=it->next;
    }
    return nullptr;
}
