#ifndef MONSTER_H_INCLUDED
#define MONSTER_H_INCLUDED
class Monster::public obstacle{
    public:
        enum Type{
            Eyeball

        };

    public:
        explicit    Monster(Type type);
    private:
        Type        mType;

    private:
        sf::Vector2f pos, vel;
        sf::Clock* globalClock;

        sf::Vector2f find_player(player *player, sf::Vector2f ourPos){
            sf::Vector2f direction;
            direction = player->pos - ourPos;
            return direction;
        }
        magnitude = pow(pow(direction.x,2) + pow(direction.y,2),0.5);
        finalDir = direction / magnitude;
        orientation = atan(finalDir.x/finalDir.y) * 180.0/M_PI;

        }

    public:
        void monster_init(player *player, std::string s,sf::Vector2f initialPosition, sf::Vector2f initialVelocity,float rotationRate,float pathEnd,std::map<int,sf::Sprite*>* colliderMap){
            monster = true;
            obstacle_init(s,initialPosition,initialVelocity,rotationRate,pathEnd,colliderMap,monster);
        }


};

    Monster::Monster(Type type):mType(type){


}
#endif // MONSTER_H_INCLUDED
