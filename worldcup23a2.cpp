#include "worldcup23a2.h"


world_cup_t::world_cup_t():
players(),
teams_by_id_tree(new Tree<TeamByID>),
teams_by_ability_tree(new Tree<TeamByAbility>)

{}

world_cup_t::~world_cup_t()
{

    int size_of_hash = players.data.size;
    for(int i = 0 ; i < size_of_hash ; i++ )
    {
        shared_ptr<Player> pre_temp = nullptr;
        shared_ptr<Player> temp = players.data[i];
        while (temp != nullptr)
        {
            pre_temp = temp->father;

            if(temp->father == nullptr)
            {
                //temp->node->root_player = nullptr;
                temp->node = nullptr;
            }
            temp->father = nullptr;
            temp = pre_temp;
        }
    }
    for(int i = 0 ; i < size_of_hash ; i++ )
    {
        shared_ptr<Player> pre_temp = nullptr;
        shared_ptr<Player> temp = players.data[i];
        while (temp != nullptr)
        {
            pre_temp = temp->next;
            if(temp->next == nullptr)
            {
                //temp->node->root_player = nullptr;
                temp->node = nullptr;
            }
            temp->next = nullptr;
            temp = pre_temp;
        }
    }
    //if there are leaks we can don another loop for the next to each player
    shared_ptr<TeamByID>* nodes = new shared_ptr<TeamByID>[this->teams_by_id_tree->getSize()];
    int index = 0 ;
    this->teams_by_id_tree->inOrderDataToArray(teams_by_id_tree->getRoot() , nodes , &index);
    for(int iter = 0 ; iter < this->teams_by_id_tree->getSize() ; iter++)
    {
        nodes[iter]->active->team = nullptr;
        nodes[iter]->active->root_player = nullptr;
        nodes[iter]->active = nullptr;
    }
    delete[] nodes;
    this->teams_by_id_tree->destroyTree(this->teams_by_id_tree->getRoot());
    this->teams_by_ability_tree->destroyTree(this->teams_by_ability_tree->getRoot());

}

StatusType world_cup_t::add_team(int teamId)
{
    if(teamId <= 0)
    {
        return StatusType::INVALID_INPUT;
    }
    try{
        shared_ptr<TeamByID> team_to_find(new TeamByID(teamId));
        shared_ptr<TreeNode<TeamByID>> real_team(teams_by_id_tree->find(team_to_find));
        if(real_team != nullptr)
        {
            return StatusType::FAILURE;
        }
        shared_ptr<Node> active(new Node());
        active->team = team_to_find;
        team_to_find->active = active;
        teams_by_id_tree->insert(team_to_find);
        shared_ptr<TeamByAbility> team_to_add(new TeamByAbility(teamId));
        teams_by_ability_tree->insert(team_to_add);
        return StatusType::SUCCESS;

    }
    catch (std::bad_alloc& ba) {
        return StatusType::ALLOCATION_ERROR;
    }
}

StatusType world_cup_t::remove_team(int teamId)
{
    if(teamId <= 0 )
    {
        return StatusType::INVALID_INPUT;
    }
    try{
        shared_ptr<TeamByID> team_to_find(new TeamByID(teamId));
        shared_ptr<TreeNode<TeamByID>> real_team(teams_by_id_tree->find(team_to_find));
        if(real_team == nullptr)
        {
            return StatusType::FAILURE;
        }
        real_team->getData()->active->is_active = false;
        real_team->getData()->active->team = nullptr;
        real_team->getData()->active->root_player = nullptr;
        real_team->getData()->active = nullptr;
        shared_ptr<TeamByAbility> team_to_remove(new TeamByAbility(teamId , real_team->getData()->ability));
        teams_by_id_tree->remove(team_to_find);
        teams_by_ability_tree->remove(team_to_remove);
        return StatusType::SUCCESS;

    }
    catch (std::bad_alloc& ba)
    {
        return StatusType::ALLOCATION_ERROR;
    }
}

StatusType world_cup_t::add_player(int playerId, int teamId,
                                   const permutation_t &spirit, int gamesPlayed,
                                   int ability, int cards, bool goalKeeper)
{
    if(playerId <= 0 || teamId <= 0 || gamesPlayed < 0 || cards < 0 || spirit.isvalid() == false)
    {
        return StatusType::INVALID_INPUT;
    }
    try{
        shared_ptr<TeamByID> team_to_find (new TeamByID(teamId));
        shared_ptr<TreeNode<TeamByID>> real_team(teams_by_id_tree->find(team_to_find));

        if(real_team == nullptr)
        {
            return StatusType::FAILURE;
        }
        if(players.Search(playerId) != nullptr)
        {
            return StatusType::FAILURE;
        }
        shared_ptr<Player> player_to_add (new Player(playerId , teamId  , spirit , gamesPlayed - real_team->getData()->active->games_played , cards , goalKeeper , real_team->getData()->team_sp * spirit));
        shared_ptr<TeamByAbility> old_team (new TeamByAbility(teamId , real_team->getData()->ability));
        teams_by_ability_tree->remove(old_team);
        real_team->getData()->ability += ability;
        real_team->getData()->team_sp = real_team->getData()->team_sp * spirit;
        real_team->getData()->size ++ ;
        if(goalKeeper == true)
        {
            real_team->getData()->num_goalkeeprs++ ;
        }
        shared_ptr<TeamByAbility> updated_team (new TeamByAbility(teamId , real_team->getData()->ability));
        teams_by_ability_tree->insert(updated_team);
        players.Insert(player_to_add);
        shared_ptr<Node> active =  real_team->getData()->active;
        if(active->root_player == nullptr){
            active->root_player = player_to_add;
            player_to_add->node = active;
            //
            player_to_add->gamesPlayed = gamesPlayed;
        }
        else{
            player_to_add->father = active->root_player;
            //
            player_to_add->spirit = active->root_player->spirit.inv() * real_team->getData()->team_sp;
            player_to_add->gamesPlayed = gamesPlayed - active->root_player->gamesPlayed;
        }
        return StatusType::SUCCESS;
    }
    catch (std::bad_alloc& ba)
    {
        return StatusType::ALLOCATION_ERROR;
    }

}

output_t<int> world_cup_t::play_match(int teamId1, int teamId2)
{
    if(teamId1 <= 0 || teamId2 <= 0 || teamId1 == teamId2)
    {
        return StatusType::INVALID_INPUT;
    }
    try{
        shared_ptr<TeamByID> team_to_find1(new TeamByID(teamId1));
        shared_ptr<TreeNode<TeamByID>> real_team1(teams_by_id_tree->find(team_to_find1));
        shared_ptr<TeamByID> team_to_find2(new TeamByID(teamId2));
        shared_ptr<TreeNode<TeamByID>> real_team2(teams_by_id_tree->find(team_to_find2));
        if(real_team1 == nullptr || real_team2 == nullptr){
            return StatusType::FAILURE;
        }
        if(real_team1->getData()->num_goalkeeprs == 0 || real_team2->getData()->num_goalkeeprs == 0){
            return StatusType::FAILURE;
        }
        int total1 = real_team1->getData()->points + real_team1->getData()->ability;
        int total2 = real_team2->getData()->points + real_team2->getData()->ability;
        real_team1->getData()->active->games_played++;
        real_team1->getData()->active->root_player->gamesPlayed++;
        real_team2->getData()->active->games_played++;
        real_team2->getData()->active->root_player->gamesPlayed++;
        if(total1 > total2 )
        {
            real_team1->getData()->points += 3;
            return 1;
        }
        if(total2 > total1)
        {
            real_team2->getData()->points += 3;
            return 3;
        }
        if(total1 == total2)
        {
            if(real_team1->getData()->team_sp.strength() > real_team2->getData()->team_sp.strength())
            {
                real_team1->getData()->points += 3;
                return 2;
            }
            if(real_team1->getData()->team_sp.strength() < real_team2->getData()->team_sp.strength())
            {
                real_team2->getData()->points += 3;
                return 4;
            }
        }
        real_team1->getData()->points ++;
        real_team2->getData()->points ++;
        return 0;

    }
    catch (std::bad_alloc& ba)
    {
        return StatusType::ALLOCATION_ERROR;
    }
}

output_t<int> world_cup_t::num_played_games_for_player(int playerId) {
    if (playerId <= 0) {
        return StatusType::INVALID_INPUT;
    }
    try {
        shared_ptr<Player> player = players.Search(playerId);
        if (player == nullptr) {
            return StatusType::FAILURE;
        }

        if(player->father == nullptr){
            int games_played = player->gamesPlayed;
            return games_played;
        }
        shared_ptr<Player> root_to_find = player;
        int games_played = 0;
        permutation_t all_spirit = permutation_t::neutral();
        while(root_to_find->father != nullptr){
            games_played += root_to_find->gamesPlayed;
            all_spirit = root_to_find->spirit * all_spirit ;
            root_to_find = root_to_find->father;
        }
        all_spirit = root_to_find->spirit * all_spirit;
        games_played += root_to_find->gamesPlayed;
        int temp_gamesPlayed = games_played;
        shared_ptr<Player> temp = player;
        if(temp->father != nullptr)
        {
            while(temp->father != root_to_find){
                int gm = temp->gamesPlayed;
                permutation_t sp = temp->spirit;
                shared_ptr<Player> temp2 = temp->father;
                temp->father = root_to_find;
                temp->spirit =   root_to_find->spirit.inv() * all_spirit;
                temp->gamesPlayed = temp_gamesPlayed - root_to_find->gamesPlayed;
                all_spirit =  all_spirit * sp.inv() ;
                temp_gamesPlayed -= gm;
                temp = temp2;
            }
        }
        return games_played;
    }
    catch (std::bad_alloc& ba)
    {
        return StatusType::ALLOCATION_ERROR;
    }
}

StatusType world_cup_t::add_player_cards(int playerId, int cards)
{
    if(playerId <= 0 || cards < 0 )
    {
        return StatusType::INVALID_INPUT;
    }
    try{
        shared_ptr<Player> player = players.Search(playerId);
        if(player == nullptr)
        {
            return StatusType::FAILURE;
        }
        shared_ptr<Player> root_to_find = player;
        int games_played = 0;
        permutation_t all_spirit = permutation_t::neutral();
        while (root_to_find->father != nullptr)
        {
            games_played += root_to_find->gamesPlayed;
            all_spirit = root_to_find->spirit * all_spirit;
            root_to_find = root_to_find->father;
        }
        all_spirit = root_to_find->spirit * all_spirit;
        games_played += root_to_find->gamesPlayed;
        int temp_gamesPlayed = games_played;
        shared_ptr<Player> temp = player;
        if(temp->father != nullptr)
        {
            while(temp->father != root_to_find){
                int gm = temp->gamesPlayed;
                permutation_t sp = temp->spirit;
                shared_ptr<Player> temp2 = temp->father;
                temp->father = root_to_find;
                temp->spirit = root_to_find->spirit.inv() * all_spirit ;
                temp->gamesPlayed = temp_gamesPlayed - root_to_find->gamesPlayed;
                all_spirit =  all_spirit * sp.inv();
                temp_gamesPlayed -= gm;
                temp = temp2;
            }
        }
        if(root_to_find->node->is_active == false)
        {
            return StatusType::FAILURE;
        }
        player->cards += cards;
        return StatusType::SUCCESS;
    }
    catch (std::bad_alloc& ba)
    {
        return StatusType::ALLOCATION_ERROR;
    }
}

output_t<int> world_cup_t::get_player_cards(int playerId)
{
    if (playerId <= 0 )
    {
        return StatusType::INVALID_INPUT;
    }
    try {
        shared_ptr<Player> player = players.Search(playerId);
        if(player == nullptr)
        {
            return StatusType::FAILURE;
        }
        return player->cards;
    }
    catch (std::bad_alloc& ba)
    {
        return StatusType::ALLOCATION_ERROR;
    }
}

output_t<int> world_cup_t::get_team_points(int teamId)
{
    if(teamId <=  0)
    {
        return StatusType::INVALID_INPUT;
    }
    try {
        shared_ptr<TeamByID> team_to_find (new TeamByID(teamId));
        shared_ptr<TreeNode<TeamByID>> realTeam (teams_by_id_tree->find(team_to_find));
        if(realTeam == nullptr)
        {
            return StatusType::FAILURE;
        }
        return realTeam->getData()->points;
    }
    catch (std::bad_alloc& ba)
    {
        return StatusType::ALLOCATION_ERROR;
    }
}

output_t<int> world_cup_t::get_ith_pointless_ability(int i)
{
    if(i < 0 || teams_by_id_tree->getSize() == 0 || i >= teams_by_id_tree->getSize())
    {
        return StatusType::FAILURE;
    }
    try {
        shared_ptr<TreeNode<TeamByAbility>> i_team = teams_by_ability_tree->Select(i+1);
        return i_team->getData()->id;
    }
    catch (std::bad_alloc& ba)
    {
        return StatusType::ALLOCATION_ERROR;

    }
}

output_t<permutation_t> world_cup_t::get_partial_spirit(int playerId)
{
    if(playerId <= 0){
        return StatusType::INVALID_INPUT;
    }
    try{
        shared_ptr<Player> player = players.Search(playerId);
        if(player == nullptr)
        {
            return StatusType::FAILURE;
        }
        shared_ptr<Player> root_to_find = player;
        int games_played = 0;
        permutation_t all_spirit = permutation_t::neutral();
        while (root_to_find->father != nullptr)
        {
            games_played += root_to_find->gamesPlayed;
            all_spirit =  root_to_find->spirit * all_spirit;
            root_to_find = root_to_find->father;
        }
        games_played += root_to_find->gamesPlayed;
        all_spirit =  root_to_find->spirit * all_spirit;
        permutation_t temp_spirit = all_spirit;
        shared_ptr<Player> temp = player;
        if(temp->father != nullptr)
        {
            while(temp->father != root_to_find){
                shared_ptr<Player> temp2 = temp->father;
                int gm = temp->gamesPlayed;
                permutation_t t_spirit = temp->spirit;
                temp->gamesPlayed = games_played - root_to_find->gamesPlayed;
                games_played -= gm;
                temp->spirit = root_to_find->spirit.inv() * temp_spirit;
                temp_spirit = temp_spirit * t_spirit.inv();
                temp->father = root_to_find;
                temp = temp2;
            }
        }
        if(root_to_find->node->is_active == false)
        {
            return StatusType::FAILURE;
        }
        return all_spirit;
    }
    catch (std::bad_alloc& ba)
    {
        return StatusType::ALLOCATION_ERROR;
    }

}

StatusType world_cup_t::buy_team(int teamId1, int teamId2)
{
    if(teamId1 <= 0 || teamId2 <= 0 || teamId1 == teamId2)
    {
        return StatusType::INVALID_INPUT;
    }
    try{
        shared_ptr<TeamByID> team_to_find1(new TeamByID(teamId1));
        shared_ptr<TreeNode<TeamByID>> buyer_team(teams_by_id_tree->find(team_to_find1));
        shared_ptr<TeamByID> team_to_find2(new TeamByID(teamId2));
        shared_ptr<TreeNode<TeamByID>> bought_team(teams_by_id_tree->find(team_to_find2));
        if(buyer_team == nullptr || bought_team == nullptr){
            return StatusType::FAILURE;
        }
        shared_ptr<Player> root1 = buyer_team->getData()->active->root_player;
        shared_ptr<Player> root2 = bought_team->getData()->active->root_player;
        if(buyer_team->getData()->size >= bought_team->getData()->size)
        {
            if(root2 != nullptr)
            {
                root2->father = root1;
                root2->spirit = root1->spirit.inv() * buyer_team->getData()->team_sp * root2->spirit ;
                root2->gamesPlayed = root2->gamesPlayed - root1->gamesPlayed;
                root2->node->root_player = nullptr;
                root2->node->team = nullptr;
                root2->node = nullptr;
                bought_team->getData()->active = nullptr;
            }
            else{
                bought_team->getData()->active->team = nullptr;
                bought_team->getData()->active = nullptr;
            }
        }
        else{
            if(root1 != nullptr)
            {
                root1->father = root2;
                root1->spirit = root2->spirit.inv() * buyer_team->getData()->team_sp.inv() * root1->spirit ;
                root2->spirit = buyer_team->getData()->team_sp * root2->spirit ;
                root1->gamesPlayed = root1->gamesPlayed - root2->gamesPlayed;
                buyer_team->getData()->active = root1->node;
                root1->node->team = buyer_team->getData();
                root1->node->root_player = root2;
                root2->node->root_player = nullptr;
                root2->node->team = nullptr;
                root2->node = nullptr;
                root2->node = root1->node;
                root1->node = nullptr;
            }
            else{
                if(root2 != nullptr)
                {
                    root2->node->root_player = nullptr;
                    root2->node->team = nullptr;
                    root2->node = nullptr;
                    buyer_team->getData()->active->root_player = root2;
                    root2->spirit = root2->spirit * buyer_team->getData()->team_sp;
                    root2->node = buyer_team->getData()->active;
                }
            }

        }
        buyer_team->getData()->team_sp = buyer_team->getData()->team_sp * bought_team->getData()->team_sp;
        shared_ptr<TeamByAbility> teamToRemove2(new TeamByAbility(buyer_team->getData()->id , buyer_team->getData()->ability));
        buyer_team->getData()->points += bought_team->getData()->points;
        buyer_team->getData()->ability += bought_team->getData()->ability;
        buyer_team->getData()->num_goalkeeprs += bought_team->getData()->num_goalkeeprs;
        buyer_team->getData()->size += bought_team->getData()->size;
        shared_ptr<TeamByAbility> teamtoRemove(new TeamByAbility(bought_team->getData()->id , bought_team->getData()->ability));
        shared_ptr<TeamByAbility> updatedTeam (new TeamByAbility(buyer_team->getData()->id , buyer_team->getData()->ability));
        teams_by_id_tree->remove(bought_team->getData());
        teams_by_ability_tree->remove(teamtoRemove);
        teams_by_ability_tree->remove(teamToRemove2);
        teams_by_ability_tree->insert(updatedTeam);

    }
    catch (std::bad_alloc& ba)
    {
        return StatusType::ALLOCATION_ERROR;
    }
    return StatusType::SUCCESS;
}
