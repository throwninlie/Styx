
#include "SFML/Graphics.hpp"
#include "pSprite.hpp"
#include "unit.hpp"
#include <vector>
class Game{
    public:
            Game();
        void run();

    private:
        void processEvents();
        void handlePlayerInput(sf::Keyboard::Key key, bool isPressed);
        void update(sf::Time deltaTime);
        void render();
    private:
        sf::RenderWindow mWindow;
        //sf::CircleShape mPlayer;
        //sf::Sprite mPlayer;
        //sf::Texture mTexture;
        //pSprite* mPlayer;
        unit* uPlayer;
        std::vector<pSprite*> spawns;
        int spawnNum =0;
        bool mIsMovingUp = false;
        bool mIsMovingDown = false;
        bool mIsMovingRight = false;
        bool mIsMovingLeft = false;
        bool mSpawnSprite = false;

};
Game::Game():mWindow(sf::VideoMode(640,480),"Styx"),uPlayer(){
    //mPlayer = new pSprite;
    //mPlayer->sprite_init("..\\Assets\\hexagonTiles\\Tiles\\alienYellow.png",100.f,100.f);
    //mPlayer->sprite.scale(1.5,1.5);
    sf::Vector2f defaultPos(0.f, 0.f);
    sf::Vector2f defaultVel(0.f, 0.f);
    uPlayer = new unit;
    uPlayer->unit_init("..\\Assets\\hexagonTiles\\Tiles\\alienYellow.png",defaultPos,defaultVel);
    uPlayer->sprite.scale(1.5,1.5);
}

void Game::run(){
    sf::Clock clock;
    sf::Time timeSinceLastUpdate = sf::Time::Zero;
    sf::Time TimePerFrame = sf::seconds(1.f/60.f);

    //spawn sprite stuff
    sf::Time timeSinceLastSpawn = sf::Time::Zero;
    sf::Time timePerSpawn = sf::seconds(1.0f);
    while(mWindow.isOpen()){
        processEvents();
        timeSinceLastUpdate += clock.restart();
        while(timeSinceLastUpdate > TimePerFrame){
            timeSinceLastUpdate -= TimePerFrame;
            processEvents();
            if(mSpawnSprite){
                pSprite* mSpawn;
                mSpawn = new pSprite;
                int xRand = rand() %500;
                int yRand = rand() %500;
                mSpawn->sprite_init("..\\Assets\\floating_eyebeast.png",xRand,yRand);
                mSpawn->sprite.scale(0.2,0.2);
                spawns.push_back(mSpawn);
                spawnNum +=1;
                mSpawnSprite = false;
            }
            update(TimePerFrame);
        }

        render();
    }
}


void Game::update(sf::Time deltaTime){
    //updates game logic
    float playerSpeed = 100.f;
    sf::Vector2f movement(0.f,0.f);
    if(mIsMovingUp){
        movement.y -= playerSpeed;
    }if(mIsMovingDown){
        movement.y += playerSpeed;
    }if(mIsMovingLeft){
        movement.x -= playerSpeed;
    }if(mIsMovingRight){
        movement.x += playerSpeed;
    }
    //mPlayer->sprite.move(movement*deltaTime.asSeconds());
    uPlayer->sprite.move(movement*deltaTime.asSeconds());
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
    }else if(key == sf::Keyboard::Q){
        mSpawnSprite = isPressed;
    }
}
void Game::render(){


    //renders game to the screen
    //clears window first
    mWindow.clear();
    //draw all the objects of the current frame by calling draw method
    //mWindow.draw(mPlayer->sprite);
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
