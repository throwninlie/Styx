#ifndef PSPRITE_HPP_INCLUDED
#define PSPRITE_HPP_INCLUDED

class pSprite{
    public:
        sf::Texture tex;
        sf::Sprite sprite;
    public:
        void sprite_init(std::string s,sf::Vector2f pos){
            if (!tex.loadFromFile(s))
            {
                // Handle loading error
            };
            sprite.setTexture(tex);
            //tex.setSmooth(true);
            //sprite.setTextureRect(sf::IntRect(10, 10, 100, 100));
            //sprite.setColor(sf::Color(255, 255, 255, 200));
            sprite.setPosition(pos.x, pos.y);
        }
};

#endif // PSPRITE_H_INCLUDED
