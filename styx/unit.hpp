#ifndef PLAYER_HPP_INCLUDED
#define PLAYER_HPP_INCLUDED

class unit : public pSprite{
    private:
        //pSprite* pSprite;
        sf::Vector2f pos, vel, acc;
    public:
        void unit_init(std::string s,sf::Vector2f initialPosition, sf::Vector2f initialVelocity){
            //pSprite = new pSprite;
            //pSprite->init(s, x, y);
            sprite_init(s,initialPosition.x,initialPosition.y);
            pos = initialPosition;
            vel = initialVelocity;
            acc = (0.0f,0.0f);
        }
        void setAcceleration(sf::Vector2f enforcedAccel){
            acc = enforcedAccel;
        }
        void setVelocity(sf::Vector2f enforcedVelocity){
            vel = enforcedVelocity;
        }
        void deltaV(sf::Vector2f deltaVelocity){
            vel += deltaVelocity;
        }
        void setPosition(sf::Vector2f enforcedPosition){
            pos = enforcedPosition;
        }
        void deltaP(sf::Vector2f deltaPosition){
            pos += deltaPosition;
        }
        void update(sf::Time deltaTime, sf::Vector2f environmentAccel){
            deltaV(deltaTime * (acc+environmentAccel));
            deltaP(deltaTime * vel);
            sprite.setPosition(pos.x,pos.y);
        }
};

#endif // PLAYER_HPP_INCLUDED
