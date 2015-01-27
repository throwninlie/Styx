
#include "SFML/Graphics.hpp"

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
        sf::Sprite mPlayer;
        bool mIsMovingUp = false;
        bool mIsMovingDown = false;
        bool mIsMovingRight = false;
        bool mIsMovingLeft = false;

};
Game::Game():mWindow(sf::VideoMode(640,480),"Styx"),mPlayer(){
    //initializes window with cyan circle
    //mPlayer.setRadius(40.f);
    //mPlayer.setPosition(100.f,100.f);
    //mPlayer.setFillColor(sf::Color::Cyan);

    sf::Texture texture;
    if (!texture.loadFromFile("floating_eyebeast.png"))
    {
    // Handle loading error
    }
    sf::Sprite mPlayer(texture);
    mPlayer.setPosition(100.f, 100.f);
}

void Game::run(){
    sf::Clock clock;
    sf::Time timeSinceLastUpdate = sf::Time::Zero;
    sf::Time TimePerFrame = sf::seconds(1.f/60.f);
    while(mWindow.isOpen()){
        processEvents();
        timeSinceLastUpdate += clock.restart();
        while(timeSinceLastUpdate > TimePerFrame){
            timeSinceLastUpdate -= TimePerFrame;
            processEvents();
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
    mPlayer.move(movement*deltaTime.asSeconds());
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
    }
}
void Game::render(){


    //renders game to the screen
    //clears window first
    mWindow.clear();
    //draw all the objects of the current frame by calling draw method
    mWindow.draw(mPlayer);
    //after drawing everything, display it on the screen
    mWindow.display();
}

int main()
{
    Game game;
    game.run();
}
