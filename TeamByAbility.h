//
// Created by ameer on 1/4/2023.
//

#ifndef UNTITLED2_TEAMBYABILITY_H
#define UNTITLED2_TEAMBYABILITY_H

class TeamByAbility
{
public:
    int id;
    int ability;

    TeamByAbility(int id): id(id) , ability(0)
    {};
    TeamByAbility(int id , int ability):id(id) , ability(ability)
    {};
    bool operator==(const TeamByAbility& Team_a){
        return (this->id == Team_a.id) ;
    }
    bool operator>(const TeamByAbility& Team_a ){
        if (this->ability > Team_a.ability)
            return true;
        if(this->ability == Team_a.ability){
            if(this->id > Team_a.id){
                return true;
            }
        }
        return false;
    }
    bool operator!=(const TeamByAbility& Team_a){
        return (this->id != Team_a.id) ;
    }
    bool operator<(const TeamByAbility& Team_a){
        if (this->ability < Team_a.ability)
            return true;
        if(this->ability == Team_a.ability){
            if(this->id < Team_a.id){
                return true;
            }
        }
        return false;
    }
    bool operator>=(const TeamByAbility& Team_a){
        if (this->ability >= Team_a.ability)
            return true;
        if(this->ability == Team_a.ability){
            if(this->id >= Team_a.id){
                return true;
            }
        }
        return false;
    }
    bool operator<=(const TeamByAbility& Team_a){
        if (this->ability <= Team_a.ability)
            return true;
        if(this->ability == Team_a.ability){
            if(this->id <= Team_a.id){
                return true;
            }
        }
        return false;
    }

};


#endif //UNTITLED2_TEAMBYABILITY_H
