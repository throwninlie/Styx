#ifndef BACKGROUND_HPP_INCLUDED
#define BACKGROUND_HPP_INCLUDED

class Background::public pSprite{

    bool isMonster = false;

    //multiple sprites for multiple textures
    void sprite_init(std::string s,sf::Vector2f pos, bool isMonster);
};

#endif // BACKGROUND_HPP_INCLUDED
