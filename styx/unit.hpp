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
        sf::Vector2f pos, vel, velMax, pAcc, maxAcc, localAcc, lastAcc, jumpMagnitude;
        sf::Clock* globalClock;
        bool pAccel_toggle_on, left, right, up, down;
        int xMax, yMax;
        float lastJump, jumpCooldown, now, orientation;
    public:
        void unit_init(std::string s,sf::Vector2f initialPosition, sf::Vector2f initialVelocity, sf::Vector2f passiveAccel, int gameWindowX, int gameWindowY,sf::Clock* clock){
            pos = initialPosition;
            vel = initialVelocity;
            sprite_init(s,pos);
            pAcc = passiveAccel;
            xMax = gameWindowX - 1.2*tex.getSize().x;
            yMax = gameWindowY - 1.2*tex.getSize().y;
            velMax.x = 200;
            velMax.y = 200;
            maxAcc.x = 200;
            maxAcc.y = passiveAccel.y;
            pAccel_toggle_on = true; //Gravity "on" by default
            globalClock = clock;
            lastJump = globalClock->getElapsedTime().asSeconds();
            jumpCooldown = 1.0;
            jumpMagnitude.x = 0.f;
            jumpMagnitude.y = 400.f;
            orientation = 0.0;
        }
        void addLocalAccX(float accX){
            localAcc.x += accX;
        }
        void addLocalAccY(float accY){
            localAcc.y -= accY;
        }
        void setLocalAccX(float accX){
            localAcc.x = accX;
        }
        void setLocalAccY(float accY){
            localAcc.y = accY;
        }
        void accelRight(){
            localAcc.x = maxAcc.x;
        }
        void accelLeft(){
            localAcc.x = -maxAcc.x;
        }
        void accelUp(){
            localAcc.y = -maxAcc.y;
        }
        void accelDown(){
            localAcc.y = maxAcc.y;
        }
        void setMaxAccX(float accX){
            maxAcc.x = accX;
        }
        void setMaxAccY(float accY){
            maxAcc.y = accY;
        }
        void setJump(float newJump){
            jumpMagnitude.y = newJump;
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
        void rot(char dir){
            switch(dir){
                case 'L':
                    orientation += 10;
                case 'R':
                    orientation -= 10;
            }
        }
        void update(sf::Time deltaTime){
            //Apply rotation
                sprite.setRotation(orientation);
            //Apply changes to unit's velocity
            if(pAccel_toggle_on){
                deltaV(deltaTime.asSeconds() * (pAcc+localAcc));
            }
            else{
                deltaV(deltaTime.asSeconds() * (localAcc));
            }
            //Limit velocity to unit's maximum velocity
            if(vel.x > velMax.x){
                vel.x = velMax.x;
            }
            else if(vel.x < -velMax.x){
               vel.x = -velMax.x;
            }
            if(vel.y > velMax.y){
                vel.y = velMax.y;
            }
            else if(vel.y < -velMax.y){
               vel.y = -velMax.y;
            }
            //Apply changes to unit's position (does not move sprite)
            deltaP(deltaTime.asSeconds() * vel);
            //Check for boundary collisions
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
                vel.y = -0.2 * vel.y;
            }
            //Update time
            now = globalClock->getElapsedTime().asSeconds();
            //Jump if appropriate
            if(now - lastJump > jumpCooldown && lastAcc.y == 0 && localAcc.y < 0){
                //jumpMagnitude.x = 400 * sin(2*M_PI*orientation/360);
                //jumpMagnitude.y = 400 * cos(2*M_PI*orientation/360);
                deltaV(jumpMagnitude);
                lastJump = now;
            }
            //Update acceleration history
            lastAcc = localAcc;
            //Change sprite position and reset local acceleration to zero to avoid auto-run
            sprite.setPosition(pos.x,pos.y);
            setLocalAccX(0.0);
            setLocalAccY(0.0);
        }
};

#endif // PLAYER_HPP_INCLUDED
