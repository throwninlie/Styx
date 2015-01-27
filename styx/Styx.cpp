
#include "SFML/Graphics.hpp"

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

};
Game::Game():mWindow(sf::VideoMode(640,480),"Styx"),mPlayer(){
    //initializes window with cyan circle
    mPlayer.setRadius(40.f);
    mPlayer.setPosition(100.f,100.f);
    mPlayer.setFillColor(sf::Color::Cyan);
}

void Game::run(){

    while(mWindow.isOpen()){
        processEvents();
        update();
        render();
    }
}

void Game::processEvents(){
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
    mWindow.draw(mPlayer);
    //after drawing everything, display it on the screen
    mWindow.display();
}

int main()
{
    Game game;
    game.run();
}
