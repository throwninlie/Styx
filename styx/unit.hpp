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
            sprite.setOrigin(tex.getSize().x / 2.0,tex.getSize().y / 2.0);
            sprite.setPosition(pos.x, pos.y);
        }
};

class unit : public pSprite{
    private:
        sf::Vector2f pos, vel, velMax, pAcc, maxAcc, localAcc, lastAcc, jumpMagnitude;
        sf::Clock* globalClock;
        bool pAccel_toggle_on, left, right, up, down, collided, wantJump;
        int xMax, yMax, xMin, yMin;
        float lastJump, jumpCooldown, now, orientation, lastCollision, jumpWindow;
    public:
        void unit_init(std::string s,sf::Vector2f initialPosition, sf::Vector2f initialVelocity, sf::Vector2f passiveAccel, int gameWindowX, int gameWindowY,sf::Clock* clock){
            pos = initialPosition;
            vel = initialVelocity;
            sprite_init(s,pos);
            pAcc = passiveAccel;
            xMin = 0.5 * tex.getSize().x;
            yMin = 0.5 * tex.getSize().y;
            xMax = gameWindowX - xMin;
            yMax = gameWindowY - yMin;
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
            jumpWindow = 0.1;
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
        sf::Vector2f getVelocity(){
            return vel;
        }
        sf::Vector2f getPosition(){
            return pos;
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
                    orientation -= 10.0;
                    break;
                case 'R':
                    orientation += 10.0;
                    break;
            }
        }
        void collision(){
            collided = true;
            lastCollision = globalClock->getElapsedTime().asSeconds();
        }
        void jumped(){
            collided = false;
            wantJump = false;
        }
        void jump(){
            if(now - lastCollision < jumpWindow){
                wantJump = true;
            }
        }
        void update(sf::Time deltaTime){
            //Update time
            now = globalClock->getElapsedTime().asSeconds();
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
                collision();
            }
            else if(pos.x < xMin){
                pos.x = xMin;
                vel.x = -0.7 * vel.x;
                collision();
            }
            if(pos.y > yMax){
                pos.y = yMax;
                vel.y = -0.7 * vel.y;
                collision();
            }
            else if(pos.y < yMin){
                pos.y = yMin;
                vel.y = -0.2 * vel.y;
                collision();
            }
            //Jump if appropriate
            if(wantJump == true && collided == true){
                jumpMagnitude.x = 600 * sin(2*M_PI*orientation/360);
                jumpMagnitude.y = 600 * -cos(2*M_PI*orientation/360);
                deltaV(jumpMagnitude);
                lastJump = now;
                jumped();
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
