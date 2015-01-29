#define _USE_MATH_DEFINES
#include <math.h>
#include "SFML/Graphics.hpp"
#include "unit.hpp"
#include <vector>
class Game{
    public:
            Game();
        void run();

    private:
        void processEvents();
        void handlePlayerInput(sf::Keyboard::Key key, bool isPressed);
        void update(sf::Time deltaTime, sf::Time now);
        void render();
    private:
        sf::RenderWindow mWindow;
        player* uPlayer;

        //Background* mBackground;
        //Global Clock
        sf::Clock* globalClock;
        //Global collider registry
        std::map<int,obstacle*> colliderMap;
        //world view
        sf::View mWorldView;
        //scroll speed of view
        int mScrollSpeed = -1.0;


        int spawnNum =0;

        bool mIsMovingUp = false;
        bool mIsMovingDown = false;
        bool mIsMovingRight = false;
        bool mIsMovingLeft = false;
        bool mSpawnSprite = false;
        bool mRotateLeft = false;
        bool mRotateRight = false;
        bool mWantJump = false;

};
int windowX = 640;
int windowY = 480;
Game::Game():mWindow(sf::VideoMode(windowX,windowY),"Styx"),uPlayer(),globalClock(),mWorldView(),colliderMap(){

    //started background stuff


    //in the game class

    globalClock = new sf::Clock;
    mWorldView.reset(sf::FloatRect(0, 0, windowX, windowY));
    //Global positions, velocities, and accelerations
    sf::Vector2f defaultPos(windowX*0.5, windowY);
    sf::Vector2f defaultVel(0.f, 0.f);
    sf::Vector2f passiveAccel(0.f,100.f);
    uPlayer = new player;
    uPlayer->player_init("..\\Assets\\hexagonTiles\\Tiles\\alienYellow.png",defaultPos,defaultVel,passiveAccel,windowX,windowY,globalClock);
    uPlayer->sprite.scale(1.5,1.5);
}

void Game::run(){
    //Spawning clock
    sf::Clock clock;
    sf::Time timeSinceLastUpdate = sf::Time::Zero;
    sf::Time TimePerFrame = sf::seconds(1.f/60.f);

    //spawn sprite stuff
    sf::Time timeSinceLastSpawn = sf::Time::Zero;
    sf::Time timePerSpawn = sf::seconds(1.0f);


    while(mWindow.isOpen()){
        processEvents();
        timeSinceLastUpdate += clock.restart();
        sf::Time now = globalClock->getElapsedTime();
        while(timeSinceLastUpdate > TimePerFrame){
            timeSinceLastUpdate -= TimePerFrame;
            processEvents();
            update(TimePerFrame,now);
        }
        render();
    }
}

bool intersects = false;
sf::FloatRect boolRect;
void Game::update(sf::Time deltaTime, sf::Time now){
    //updates game logic

    if(mIsMovingUp){
        uPlayer->accelUp();
    }if(mIsMovingDown){
        uPlayer->accelDown();
    }if(mIsMovingLeft){
        uPlayer->accelLeft();
    }if(mIsMovingRight){
        uPlayer->accelRight();
    }if(mRotateLeft){uPlayer->rot('L');
    }if(mRotateRight){uPlayer->rot('R');
    }if(mWantJump){uPlayer->jump();
    }
    //you can start scrolling the game
    sf::Time scrollStart = sf::seconds(5.0f);
    if(now.asSeconds() > scrollStart.asSeconds()){
        mWorldView.move(0.f, (now.asSeconds() - scrollStart.asSeconds()) * mScrollSpeed * deltaTime.asSeconds());
    }

    if(mSpawnSprite){
        obstacle* mSpawn;
        mSpawn = new obstacle;
        std::string s = "..\\Assets\\floating_eyebeast.png";
        sf::Vector2f initialVelocity = sf::Vector2f(100.0,100.0);
        sf::Vector2f initialPosition = sf::Vector2f(300.0,-100.0);
        float rotationRate,pathEnd;
        rotationRate = 0.0;
        pathEnd = 300.0;
        bool isMonster = true;

        mSpawn->obstacle_init(s, initialPosition,initialVelocity, rotationRate, pathEnd, isMonster, uPlayer);
        //sf::Vector2f randVec(rand() %windowX,rand() %windowY);

        //mSpawn->sprite_init("..\\Assets\\floating_eyebeast.png",randVec,false);
        mSpawn->sprite.scale(0.2,0.2);
        mSpawn->setID(spawnNum);
        colliderMap[spawnNum] = mSpawn;
        //spawns.push_back(mSpawn);
        spawnNum +=1;
        mSpawnSprite = false;
    }
    //Check player for collisions with obstacles/monsters
            intersects = false;
    if(spawnNum != 0){
            for(int i = 0; i < spawnNum; i++){
                //Use SFML rectangle intersection library for rectangle collision detection
                intersects = uPlayer->sprite.getGlobalBounds().intersects(colliderMap.at(i)->sprite.getGlobalBounds(),boolRect);
                if(intersects){
                    uPlayer->collision(); //Reset collision timer
                    break;
                }
            }
    }
    //New player update sequence
    uPlayer->update(deltaTime);
    int colliderMapSize = colliderMap.size();
    if(spawnNum > 0){
        for(std::map<int,obstacle*>::iterator it=colliderMap.begin(); it!=colliderMap.end(); ++it){
            obstacle *obs = it->second;
            obs->update(deltaTime);
        }
    }



}

void Game::processEvents(){
    //handles user input
    sf::Event event;
    while(mWindow.pollEvent(event)){
        switch(event.type){
            case sf::Event::KeyPressed:
                handlePlayerInput(event.key.code,true);
                break;
            case sf::Event::KeyReleased:
                handlePlayerInput(event.key.code,false);
                break;
            case sf::Event::Closed:
                mWindow.close();
                break;
        }
    }
}

void Game::handlePlayerInput(sf::Keyboard::Key key, bool isPressed){
    if (key == sf::Keyboard::W){
        mIsMovingUp = isPressed;
    }else if (key == sf::Keyboard::S){
        mIsMovingDown = isPressed;
    }else if(key == sf::Keyboard::A){
        mIsMovingLeft = isPressed;
    }else if(key == sf::Keyboard::D){
        mIsMovingRight = isPressed;
    }else if(key == sf::Keyboard::E){
        mRotateRight = isPressed;
    }else if(key == sf::Keyboard::Q){
        mRotateLeft = isPressed;
    }else if(key == sf::Keyboard::Space){
        mWantJump = isPressed;
    }else if(key == sf::Keyboard::Return){
        mSpawnSprite = isPressed;
    }
}
void Game::render(){


    //renders game to the screen
    //clears window first
    mWindow.clear();
    mWindow.setView(mWorldView);
    //draw all the objects of the current frame by calling draw method
    mWindow.draw(uPlayer->sprite);
    //draw spawns
    for(int i = 0; i < spawnNum; i++){
        mWindow.draw(colliderMap.at(i)->sprite);
    }

    //after drawing everything, display it on the screen
    mWindow.display();
}

int main()
{
    Game game;
    game.run();
}
