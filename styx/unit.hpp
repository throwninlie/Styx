#ifndef PLAYER_HPP_INCLUDED
#define PLAYER_HPP_INCLUDED

class unit : public pSprite{
    private:
        sf::Vector2f pos, vel, pAcc, acc, localAcc;
        bool pAccel_toggle_on;
        int xMax, yMax;
    public:
        void unit_init(std::string s,sf::Vector2f initialPosition, sf::Vector2f initialVelocity, sf::Vector2f passiveAccel, int gameWindowX, int gameWindowY){
            pos = initialPosition;
            vel = initialVelocity;
            sprite_init(s,pos);
            pAcc = passiveAccel;
            xMax = gameWindowX - tex.getSize().x;
            yMax = gameWindowY - tex.getSize().y;
            pAccel_toggle_on = true; //Gravity "on" by default
        }
        void setLocalAccX(float accX){
            localAcc.x = accX;
        }
        void setLocalAccY(float accY){
            localAcc.y = accY;
        }
        void setGravity(bool toggle){
            pAccel_toggle_on = toggle;
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
        void update(sf::Time deltaTime){
            //Check for contact with wall before applying changes to position and velocity
            if(pAccel_toggle_on){
                deltaV(deltaTime.asSeconds() * (pAcc+localAcc));
            }
            else{
                deltaV(deltaTime.asSeconds() * (localAcc));
            }

            deltaP(deltaTime.asSeconds() * vel);

            if(pos.x > xMax){
                pos.x = xMax;
                vel.x = -0.5 * vel.x;
            }
            else if(pos.x < 0){
                pos.x = 0;
                vel.x = -0.5 * vel.x;
            }

            if(pos.y > yMax){
                pos.y = yMax;
                vel.y = -0.5 * vel.y;
            }
            else if(pos.y < 0){
                pos.y = 0;
                vel.y = -0.5 * vel.y;
            }

            sprite.setPosition(pos.x,pos.y);
            setLocalAccX(0.0);
            setLocalAccY(0.0);

        }
};

#endif // PLAYER_HPP_INCLUDED
