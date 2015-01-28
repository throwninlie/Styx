#ifndef PLAYER_HPP_INCLUDED
#define PLAYER_HPP_INCLUDED

class pSprite{
    public:
        sf::Texture tex;
        sf::Sprite sprite;
    public:
        void sprite_init(std::string s,sf::Vector2f pos){
            if (!tex.loadFromFile(s))
            {
                // Handle loading error
            };
            sprite.setTexture(tex);
            //tex.setSmooth(true);
            //sprite.setTextureRect(sf::IntRect(10, 10, 100, 100));
            //sprite.setColor(sf::Color(255, 255, 255, 200));
            sprite.setPosition(pos.x, pos.y);
        }
};

class unit : public pSprite{
    private:
        sf::Vector2f pos, vel, velMax, pAcc, acc, localAcc;
        bool pAccel_toggle_on;
        int xMax, yMax;
        float jumpMagnitude;
    public:
        void unit_init(std::string s,sf::Vector2f initialPosition, sf::Vector2f initialVelocity, sf::Vector2f passiveAccel, int gameWindowX, int gameWindowY){
            pos = initialPosition;
            vel = initialVelocity;
            sprite_init(s,pos);
            pAcc = passiveAccel;
            xMax = gameWindowX - tex.getSize().x;
            yMax = gameWindowY - tex.getSize().y;
            velMax.x = 200;
            velMax.y = 200;
            pAccel_toggle_on = true; //Gravity "on" by default
        }
        void setLocalAccX(float accX){
            localAcc.x = accX;
        }
        void setLocalAccY(float accY){
            localAcc.y = accY;
        }
        void setJumpMagnitude(float newJump){
            jumpMagnitude = newJump;
        }
        void setSpeedX(float newSpeed){
            velMax.x = newSpeed;
        }
        void setSpeedY(float newSpeed){
            velMax.y = newSpeed;
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
            //Apply changes to position and acceleration; do not move sprite
            if(pAccel_toggle_on){
                deltaV(deltaTime.asSeconds() * (pAcc+localAcc));
            }
            else{
                deltaV(deltaTime.asSeconds() * (localAcc));
            }

            deltaP(deltaTime.asSeconds() * vel);
            //Check for boundary collision
            if(pos.x > xMax){
                pos.x = xMax;
                vel.x = -0.7 * vel.x;
            }
            else if(pos.x < 0){
                pos.x = 0;
                vel.x = -0.7 * vel.x;
            }

            if(pos.y > yMax){
                pos.y = yMax;
                vel.y = -0.7 * vel.y;
            }
            else if(pos.y < 0){
                pos.y = 0;
                vel.y = -0.7 * vel.y;
            }
            //Change sprite position and reset local acceleration to zero to avoid auto-run
            sprite.setPosition(pos.x,pos.y);
            setLocalAccX(0.0);
            setLocalAccY(0.0);
        }
};

#endif // PLAYER_HPP_INCLUDED
