#define _USE_MATH_DEFINES
#include <math.h>
#include "SFML/Graphics.hpp"
#include "unit.hpp"
#include <vector>
#include <string>
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
        sf::Vector2f lastPlayerPos;
        //Background* mBackground;
        //Global Clock
        sf::Clock* globalClock;
        //Global collider registry
        std::map<int,obstacle*> colliderMap;
        //world view
        sf::View mWorldView;
        sf::Vector2f mWorldViewCenter;
        //scroll speed of view
        int mScrollSpeed = -12.0;
        int score;
        float height;
        float scrollY = 0;
        //time since last spawn
        sf::Time lastSpawn = sf::seconds(0.0);
        sf::Time lastHorizontalSpawn = sf::seconds(0.0);
        int spawnNum =0;
        sf::Text startSequenceText;
        sf::Font startSequenceFont;
        sf::Text heightText;
        bool mIsMovingUp = false;
        bool mIsMovingDown = false;
        bool mIsMovingRight = false;
        bool mIsMovingLeft = false;
        bool mSpawnSprite = false;
        bool mRotateLeft = false;
        bool mRotateRight = false;
        bool mWantJump = false;
        bool mLeftPlatform = false;
        bool mRightPlatform = false;
        bool mHorizontalPlatform = false;
        bool mStarSprite = false;

};
int windowX = 640;
int windowY = 480;
Game::Game():mWindow(sf::VideoMode(windowX,windowY),"Styx"),uPlayer(),globalClock(),mWorldView(),colliderMap(),lastPlayerPos(),scrollY(),
startSequenceFont(),startSequenceText(){

    //started background stuff

    //text
    startSequenceFont.loadFromFile("..\\Assets\\font\\moonhouse.ttf");
    //sf::Text startSequenceText(" SDFASGWE",startSequenceFont);
    heightText.setFont(startSequenceFont);
    heightText.setCharacterSize(30);
    heightText.setColor(sf::Color::Green);
    score = 0;
    //
    startSequenceText.setFont(startSequenceFont);
    startSequenceText.setCharacterSize(100);
    startSequenceText.setStyle(sf::Text::Bold);
    startSequenceText.setColor(sf::Color::Green);
    startSequenceText.setOrigin(windowX *-0.45, windowY * -0.2);

    //in the game class

    globalClock = new sf::Clock;
    globalClock->restart();
    mWorldView.reset(sf::FloatRect(0, 0, windowX, windowY));
    mWorldViewCenter = mWorldView.getCenter();
    scrollY = windowY;
    //Global positions, velocities, and accelerations
    sf::Vector2f defaultPos(windowX*0.5, windowY-250.0);
    sf::Vector2f defaultVel(0.f, 0.f);
    sf::Vector2f passiveAccel(0.f,100.f);
    uPlayer = new player;
    uPlayer->player_init("..\\Assets\\hexagonTiles\\Tiles\\alienYellow.png",defaultPos,defaultVel,passiveAccel,windowX,windowY,globalClock);
    uPlayer->sprite.scale(1.5,1.5);
    uPlayer->miniMe->sprite.scale(0.8,0.8);
    lastPlayerPos = uPlayer->sprite.getPosition();

    //draw horizontal platform to window first thing
    obstacle* mSpawn;
    mSpawn = new obstacle;
    std::string s;
    sf::Vector2f initialPosition,initialVelocity,relativeCenter;
    relativeCenter = sf::Vector2f(0.0,mWorldViewCenter.y - windowY *0.5);
    float rotationRate,pathEnd;
    bool isMonster, spriteBool,platforms = false;
    isMonster = false;
    initialPosition = sf::Vector2f(0.5*windowX,windowY-25.0);
    initialVelocity = sf::Vector2f(60.0,0.0);
    s = "..\\Assets\\inkscape crap\\platform.png";
    rotationRate = 0.0;
    pathEnd = 500.0;
    mHorizontalPlatform = false;
    mSpawn->obstacle_init(s, initialPosition,initialVelocity, rotationRate, pathEnd, isMonster, uPlayer);
    mSpawn->sprite.scale(0.5,0.25);
    mSpawn->setID(spawnNum);
    mSpawn->setOrientation(90.0);
    colliderMap[spawnNum] = mSpawn;
    //spawns.push_back(mSpawn);
    spawnNum +=1;

}

void Game::run(){
    //Spawning clock
    sf::Clock clock;
    sf::Time timeSinceLastUpdate = sf::Time::Zero;
    sf::Time TimePerFrame = sf::seconds(1.f/60.f);

    //spawn sprite stuff
    sf::Time timeSinceLastSpawn = sf::Time::Zero;



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
    sf::Time scrollStart = sf::seconds(10.0f);
    float nowSeconds = now.asSeconds();
    if (nowSeconds >= 1.5 && nowSeconds < 2.5){
        startSequenceText.setString("1");

    }else if (nowSeconds >= 2.5 && nowSeconds < 3.5){
        startSequenceText.setString("2");

    }else if (nowSeconds >= 3.5 && nowSeconds < 4.5){
        startSequenceText.setString("3");

    }else if (nowSeconds >= 4.5 && nowSeconds < 5.5){
        startSequenceText.setString("GO!");

    }else{
        startSequenceText.setString("");
    }

    sf::Vector2f playerPos;
    playerPos = uPlayer->sprite.getPosition();


    sf::String heightTextString;

    if(playerPos.y < 0){
        if((abs(playerPos.y) - abs(lastPlayerPos.y)) > 0){
            height+= (abs(playerPos.y) - abs(lastPlayerPos.y)) - height;
        }else{
            //keep score the same
            height = 0;
        }


    }else{
        height = 0;
        //score = windowY - playerPos.y;
        //do nothing
    }
    score = height*2.5  + score;
    char buff[100];
    if (score < 0){
        score = 0;
    }
    sprintf(buff, "Score:%d", score);
    std::string stringy = buff;
    //std::wstring heightChar =std::to_wstring(height);
    heightTextString = sf::String(stringy);
    heightText.setString(heightTextString);

    heightText.setPosition(10,mWorldViewCenter.y-windowY*0.5-10);
    //sets rotation sprite
    uPlayer->miniMe->sprite.setPosition(60.0, (mWorldViewCenter.y-windowY*0.5)+60.0);
    //uPlayer->miniMe.(uPlayer->getOrientation());


    if(nowSeconds > scrollStart.asSeconds()){
        //instead of scrolling up, set the view to the player
        //mWorldView.move(0.f, mScrollSpeed * deltaTime.asSeconds());

        scrollY += mScrollSpeed*deltaTime.asSeconds();
        mScrollSpeed = pow(nowSeconds - scrollStart.asSeconds(),0.5) * -25.0;
        if(mScrollSpeed < -400){
            //maxScrollSpeed
            mScrollSpeed = -400;
        }

        mWorldView.move(0,mScrollSpeed*deltaTime.asSeconds());
        //mWorldView.move(0.f,(playerPos.y - lastPlayerPos.y));

        mWorldViewCenter = mWorldView.getCenter();
        lastPlayerPos = playerPos;
    }
    sf::Time timePerSpawn = sf::seconds(1.2f);
    sf::Time horizontalSpawn = sf::seconds(15.0f);
    if((now- timePerSpawn) > lastSpawn){
        //spawn stuff
        if((now - horizontalSpawn) > lastHorizontalSpawn){
            mHorizontalPlatform = true;
            lastHorizontalSpawn = now;
        }
        lastSpawn = now;
        //pick 1 or 0.0
        int num, spritesNum;
        num = rand()% 2;
        spritesNum = rand() %500;
        if(num == 0){
            //spawn left
            mLeftPlatform = true;
        }else if (num == 1){
            //spawn right
            mRightPlatform = true;
        }
        if(spritesNum < 365 && spritesNum > 355){
            //spawn a star
            mStarSprite = true;
        }else if(spritesNum > 490 and spritesNum < 500){
            //spawn a ghost
            mSpawnSprite = true;
        }
    }

    if(mSpawnSprite || mLeftPlatform || mRightPlatform || mHorizontalPlatform || mStarSprite){
        obstacle* mSpawn;
        mSpawn = new obstacle;
        std::string s;
        sf::Vector2f initialPosition,initialVelocity,relativeCenter;
        relativeCenter = sf::Vector2f(0.0,mWorldViewCenter.y - windowY *0.5);
        float rotationRate,pathEnd;
        bool isMonster, spriteBool,platforms = false;

        if(mLeftPlatform || mRightPlatform || mHorizontalPlatform){
            platforms = true;
        }
        if(mSpawnSprite || mStarSprite){
            spriteBool = true;
        }
        if(mSpawnSprite){
            isMonster= true;
            initialPosition = sf::Vector2f(rand() %(windowX- 400) + 200.0,-100.0);
            initialVelocity = sf::Vector2f(70.0,70.0);
            s = "..\\Assets\\floating_eyebeast.png";
            rotationRate = 0.0;
            //doesn't matter
            pathEnd = 300.0;
            mSpawnSprite = false;
            initialPosition = initialPosition + relativeCenter;
            mSpawn->obstacle_init(s, initialPosition,initialVelocity, rotationRate, pathEnd, isMonster, uPlayer);

        }
        else if(mLeftPlatform){
            isMonster = false;
            initialPosition = sf::Vector2f(0,-250.0);
            initialVelocity = sf::Vector2f(0.0,50.0);
            s = "..\\Assets\\inkscape crap\\platform.png";
            rotationRate = 0.0;
            pathEnd = 800.0;

            mLeftPlatform = false;
            initialPosition = initialPosition + relativeCenter;
            mSpawn->obstacle_init(s, initialPosition,initialVelocity, rotationRate, pathEnd, isMonster, uPlayer);

        }else if(mRightPlatform){
            isMonster = false;
            initialPosition = sf::Vector2f(windowX,-250.0);
            initialVelocity = sf::Vector2f(0.0,50.0);
            s = "..\\Assets\\inkscape crap\\platformmirror.png";
            rotationRate = 0.0;
            pathEnd = 800.0;
            mRightPlatform = false;
            initialPosition = initialPosition + relativeCenter;
            mSpawn->obstacle_init(s, initialPosition,initialVelocity, rotationRate, pathEnd, isMonster, uPlayer);

        }else if(mHorizontalPlatform){
            isMonster = false;
            initialPosition = sf::Vector2f(rand() % (windowX- 400) + 200.0,-200.0);
            initialVelocity = sf::Vector2f(50.0,0.0);
            s = "..\\Assets\\inkscape crap\\platform.png";
            rotationRate = 0.0;
            pathEnd = 600.0;
            mHorizontalPlatform = false;
            initialPosition = initialPosition + relativeCenter;
            mSpawn->obstacle_init(s, initialPosition,initialVelocity, rotationRate, pathEnd, isMonster, uPlayer);
            mSpawn->setOrientation(90.0);
        }else if(mStarSprite){
            isMonster = true;
            initialPosition = sf::Vector2f(rand() % (windowX- 400) + 200.0,-100.0);
            initialVelocity = sf::Vector2f(rand() %60,rand() %60 + 10.0);
            s = "..\\Assets\\inkscape crap\\star.png";
            rotationRate = 5.0;
            pathEnd = (float)windowY;
            mStarSprite = false;
            initialPosition = initialPosition + relativeCenter;
            mSpawn->obstacle_init(s, initialPosition,initialVelocity, rotationRate, pathEnd, isMonster, uPlayer);
        }

        //sf::Vector2f randVec(rand() %windowX,rand() %windowY);
        if(spriteBool){
            mSpawn->sprite.scale(0.2,0.2);
        }else if(platforms && !spriteBool) {
            mSpawn->sprite.scale(0.5,0.25);
        }
        mSpawn->setID(spawnNum);
        //rotate if horizontal

        colliderMap[spawnNum] = mSpawn;
        //spawns.push_back(mSpawn);
        spawnNum +=1;

    }
    //Check player for collisions with obstacles/monsters
    intersects = false;
    if(spawnNum != 0){
            for(int i = 0; i < spawnNum; i++){
                //Use SFML rectangle intersection library for rectangle collision detection
                intersects = uPlayer->sprite.getGlobalBounds().intersects(colliderMap.at(i)->getHitBox(),boolRect);
                if(intersects){
                    uPlayer->collision(); //Reset collision timer
                    if(colliderMap.at(i)->monster){
                        //React to touching monster
                        //subtracts from score
                        score -= (float)pow(nowSeconds,0.5);
                    }else if(colliderMap.at(i)->getPlatformType() == true){
                        //React to touching horizontal platform
                        uPlayer->bounceY();
                    }else if(colliderMap.at(i)->getPlatformType()== false){
                        //React to touching vertical platform
                        uPlayer->bounceX();
                    }
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
        score -= 1;
    }else if(key == sf::Keyboard::Equal){
        mRightPlatform = isPressed;
        score -= 10;
    }else if(key == sf::Keyboard::Dash){
        mLeftPlatform = isPressed;
        score -= 10;
    }else if(key == sf::Keyboard::BackSpace){
        mHorizontalPlatform = isPressed;
        score -= 10;
    }else if(key == sf::Keyboard::Delete){
        mStarSprite = isPressed;
        score -= 1;
    }

}
void Game::render(){


    //renders game to the screen
    //clears window first
    mWindow.clear();
    mWindow.setView(mWorldView);
    //draw spawns and start text

    mWindow.draw(startSequenceText);

    for(int i = 0; i < spawnNum; i++){
        mWindow.draw(colliderMap.at(i)->sprite);
    }
    //draw all the objects of the current frame by calling draw method


    mWindow.draw(heightText);
    mWindow.draw(uPlayer->miniMe->sprite);
    mWindow.draw(uPlayer->sprite);



    //after drawing everything, display it on the screen
    mWindow.display();
}

int main()
{
    Game game;
    game.run();
}
