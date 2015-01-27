#ifndef PSPRITE_HPP_INCLUDED
#define PSPRITE_HPP_INCLUDED

class pSprite{
    public:
        sf::Texture tex;
        sf::Sprite sprite;
    private:
        void sprite_init(std::string s,float x, float y){
        tex.loadFromFile(s);
        sprite.setTexture(tex);
        sprite.setTextureRect(sf::IntRect(10, 10, 100, 100));
        sprite.setColor(sf::Color(255, 255, 255, 200));
        sprite.setPosition(100, 25);
        }
};

#endif // PSPRITE_H_INCLUDED
