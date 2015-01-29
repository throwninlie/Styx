#ifndef UNIT_HPP_INCLUDED
#define UNIT_HPP_INCLUDED

class pSprite{
    public:
        sf::Texture tex;
        sf::Sprite sprite;
        sf::Sprite* spritePointer;
        bool monster;
    public:
        void sprite_init(std::string s,sf::Vector2f pos, bool isMonster){
            if (!tex.loadFromFile(s))
            {
                // Handle loading error
            };
            sprite.setTexture(tex);
            tex.setSmooth(true);
            sprite.setOrigin(tex.getSize().x / 2.0,tex.getSize().y / 2.0);
            sprite.setPosition(pos.x, pos.y);
            spritePointer = &sprite;
            monster = isMonster;
        }
};

class player : public pSprite{
    private:
        sf::Vector2f pos, vel, velMax, pAcc, maxAcc, localAcc, lastAcc, jumpMagnitude;
        sf::Clock* globalClock;
        bool pAccel_toggle_on, left, right, up, down, collided, wantJump;
        int xMax, yMax, xMin, yMin;
        float lastJump, jumpCooldown, now, orientation, lastCollision, jumpWindow;
        float bounceCooldown = 0.10;

    public:
        void player_init(std::string s,sf::Vector2f initialPosition, sf::Vector2f initialVelocity, sf::Vector2f passiveAccel, int gameWindowX, int gameWindowY,sf::Clock* clock){
            pos = initialPosition;   //Set initial conditions
            vel = initialVelocity;
            sprite_init(s,pos,false);     //Initialize sprite object
            pAcc = passiveAccel;    //Set gravity
            xMin = 0.5 * tex.getSize().x; //Solve for window boundary locations based on texture size
            yMin = 0.5 * tex.getSize().y;
            xMax = gameWindowX - xMin;
            yMax = gameWindowY - yMin;
            velMax.x = 200;    //Set maximum allowable velocities and accelerations
            velMax.y = 200;
            maxAcc.x = 200;
            maxAcc.y = passiveAccel.y;
            pAccel_toggle_on = true; //Gravity "on" by default
            globalClock = clock;    //Store pointer to game clock
            lastJump = globalClock->getElapsedTime().asSeconds();   //Set first jump timer
            jumpCooldown = 1.0;         //Set variables relevant to jump and rotate functionality
            jumpMagnitude.x = 0.f;
            jumpMagnitude.y = 400.f;
            orientation = 0.0;
            jumpWindow = 0.1; //Window of time after collision during which jumping is allowed

        }
        void addLocalAccX(float accX){ //Functions to read and change various properties
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
        void collision(){       //Collision event handling
            collided = true;
            lastCollision = globalClock->getElapsedTime().asSeconds();
        }
        void jumped(){          //Jump event handling
            collided = false;
            wantJump = false;
        }
        void jump(){            //Limit jumping ability to time shortly after collision
            if(now - lastCollision < jumpWindow){
                wantJump = true;
            }
        }
        void bounceX(){
            if(now - lastCollision > bounceCooldown){
                vel.x = -vel.x;
            }
        }
        void bounceY(){
            if(now - lastCollision > bounceCooldown){
                vel.y = -vel.y;
            }
        }
        void update(sf::Time deltaTime){
            //Update time
            now = globalClock->getElapsedTime().asSeconds();
            //Apply rotation
            sprite.setRotation(orientation);
            //Apply changes to player's velocity
            if(pAccel_toggle_on){
                deltaV(deltaTime.asSeconds() * (pAcc+localAcc));
            }
            else{
                deltaV(deltaTime.asSeconds() * (localAcc));
            }
            //Limit velocity to player's maximum velocity
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
            //Apply changes to player's position (does not move sprite)
            deltaP(deltaTime.asSeconds() * vel);
            //Check for boundary collisions
            if(pos.x > xMax){
                //pos.x = xMax;
                //vel.x = -0.7 * vel.x;
                //collision();
            }
            else if(pos.x < xMin){
                //pos.x = xMin;
                //vel.x = -0.7 * vel.x;
                //collision();
            }
            if(pos.y > yMax){
                pos.y = yMax;
                vel.y = -0.7 * vel.y;
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

class obstacle : public pSprite{
    private:
        sf::Vector2f pos, vel, direction, finalDir;
        sf::FloatRect hitbox;
        float rotRate, pBegin, pEnd, orientation,dirMagnitude;
        float velMag;
        int id;
        bool xTraveler;
        player* playerPointer;
    public:
        void obstacle_init(std::string s,sf::Vector2f initialPosition, sf::Vector2f initialVelocity, float rotationRate,float pathEnd, bool isMonster, player* playerP){
           //Initialize sprite object
           sprite_init(s,pos,isMonster);
           //Assign identification number
           //id = colliderMap.size() + 1;
           //Set initial conditions
           pos = initialPosition;
           vel = initialVelocity;
           orientation = 0;
           hitbox = sprite.getGlobalBounds();
           playerPointer = playerP;
           velMag = (float)(pow(pow((double)vel.x,2)+pow((double)vel.y,2),0.5));
           if(!monster){ //If this obstacle is not a monster, set variables relevant to a platform
                hitbox.width = hitbox.width / 4.0;
                rotRate = rotationRate;
                //If initialized with x-velocity, this is an x-traveling obstacle; false => y-traveling
                if(vel.x != 0.0){
                pBegin = pos.x;
                xTraveler = true;
                }else{
                pBegin = pos.y;
                xTraveler = false;
                }
                pEnd = pathEnd;
           }
        }
        //Functions to read and modify various properties
        sf::Vector2f getPosition(){
            return pos;
        }
        sf::Vector2f getVelocity(){
            return vel;
        }
        bool getPlatformType(){
            return xTraveler;
        }
        sf::FloatRect getHitBox(){
            return hitbox;
        }
        float getRotRate(){
            return rotRate;
        }
        void setPosition(sf::Vector2f newPosition){
            pos = newPosition;
        }
        void setVelocity(sf::Vector2f newVelocity){
            vel = newVelocity;
        }
        void setRotRate(float newRotRate){
            rotRate = newRotRate;
        }
        void deltaP(sf::Vector2f deltaPosition){
            pos += deltaPosition;
        }
        void setOrientation(float newOrientation){
            orientation = newOrientation;
        }
        int getID(){
            return id;
        }
        void setID(int newId){
            id = newId;
        }
        void find_player(player *playerPointer, sf::Vector2f pos){
            direction = playerPointer->getPosition() - pos;
        }
        void update(sf::Time deltaTime){
            if(!monster){ //If not monster, move like an obstacle
            //Calculate new orientation
            orientation += 2.0 * M_PI * rotRate * deltaTime.asSeconds()/360.0;
            //Apply rotation
            sprite.setRotation(orientation);
            //Apply change in position
            deltaP(deltaTime.asSeconds() * vel);
            //Check for path collision and change velocity accordingly
                if(xTraveler){
                    if(pos.x < pBegin){
                        //pos.x = pBegin;
                        vel.x = -vel.x;
                        vel.y = -vel.y;
                    }
                    if(pos.x > pEnd){
                        //pos.x = pBegin;
                        vel.x = -vel.x;
                        vel.y = -vel.y;
                    }
                }else{
                    if(pos.y < pBegin){
                        //pos.y = pBegin;
                        vel.x = -vel.x;
                        vel.y = -vel.y;
                    }
                    if(pos.y > pEnd){
                        //pos.y = pEnd;
                        vel.x = -vel.x;
                        vel.y = -vel.y;
                    }
                }
                //Update sprite's position and rotation
                sprite.setPosition(pos.x, pos.y);
                hitbox = sprite.getGlobalBounds();
                hitbox.width /= 2.0;
            }else{ //If monster, move like a monster
                //Turn toward player
                find_player(playerPointer,pos);
                dirMagnitude = (float)pow(pow((double)direction.x,2) + pow((double)direction.y,2),0.5);
                finalDir = direction / dirMagnitude;
                orientation = 180+atan2f(finalDir.y,finalDir.x) * 180.0/M_PI ;

                vel = velMag * finalDir;

                //vel = sf::Vector2f(400.0,400.0);

                deltaP(deltaTime.asSeconds() * vel);
                sprite.setPosition(pos.x, pos.y);
                sprite.setRotation(orientation);
                hitbox = sprite.getGlobalBounds();
            }
            hitbox = sprite.getGlobalBounds();
        }
};

#endif // UNIT_HPP_INCLUDED
