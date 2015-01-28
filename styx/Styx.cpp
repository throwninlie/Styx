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
        //Global Clock
        sf::Clock* globalClock;
        //Global collider registry
        std::map<int,sf::obstacle*>* colliderMap;
        //world view
        sf::View mWorldView;
        //scroll speed of view
        int mScrollSpeed = -1.0;

        std::vector<pSprite*> spawns;
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
Game::Game():mWindow(sf::VideoMode(windowX,windowY),"Styx"),uPlayer(),globalClock(),mWorldView(){
    //in the game class
    colliderMap = new std::map<int,sf::obstacle*>;
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
        pSprite* mSpawn;
        mSpawn = new pSprite;
        sf::Vector2f randVec(rand() %windowX,rand() %windowY);
        mSpawn->sprite_init("..\\Assets\\floating_eyebeast.png",randVec);
        mSpawn->sprite.scale(0.2,0.2);
        spawns.push_back(mSpawn);
        spawnNum +=1;
        mSpawnSprite = false;
    }
    //New player update sequence
    uPlayer->update(deltaTime);
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
        mWindow.draw(spawns.at(i)->sprite);
    }

    //after drawing everything, display it on the screen
    mWindow.display();
}

int main()
{
    Game game;
    game.run();
}
