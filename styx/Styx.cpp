
#include "SFML/Graphics.hpp"
#include <string>
#include <cstring>


//A sprite class for sprites of arbitrary size/position/texture


class sprite{
    public:
        sf::Texture tex;
    public:
        sf::Sprite psprite;
    public:
        void init(std::string s, float xpos, float ypos){
        tex.loadFromFile(s);
        psprite.setTexture(tex);
        psprite.setTextureRect(sf::IntRect(10,10,200,200));
        psprite.setColor(sf::Color(200,100,210,150));
        psprite.setPosition(xpos,ypos);
        }

};

//The primary Game class, including graphics, event manager, game logic, etc
class Game{
    public:
            Game();
        void run();

    private:
        void processEvents();
        void update();
        void render();
    private:
        sf::RenderWindow mWindow;
        sf::CircleShape mPlayer;
        sprite* sPlayer;
};


Game::Game():mWindow(sf::VideoMode(640,480),"Styx"),mPlayer(){
    //initializes window with cyan circle
    mPlayer.setRadius(40.f);
    mPlayer.setPosition(100.f,100.f);
    mPlayer.setFillColor(sf::Color::Cyan);
}

void Game::run(){
    //Run once
    float xpos = 0;
    float ypos = 0;
    sprite* sPlayer = new sprite;
    sPlayer->init("C:\\Users\\-\\Desktop\\Classes\\6.179\\Project\\Styx\\Styx\\Assets\\floating_eyebeast.png",xpos,ypos);


    //Run continuously
    while(mWindow.isOpen()){
        processEvents();
        update();
        render();
    }
}

void Game::processEvents(){sf::Texture texture;
    //handles user input
    sf::Event event;
    while(mWindow.pollEvent(event)){
        if(event.type == sf::Event::Closed){
            mWindow.close();
        }
    }
}

void Game::update(){
    //empty for now, this updates game logic
}

void Game::render(){
    //renders game to the screen
    //clears window first
    mWindow.clear();
    //draw all the objects of the current frame by calling draw method
    //mWindow.draw(mPlayer);
    mWindow.draw(sPlayer->psprite);
    //after drawing everything, display it on the screen
    mWindow.display();

}

int main()
{


    Game game;
    game.run();
}
