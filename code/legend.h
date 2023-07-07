#ifndef OOP_LEGEND_H
#define OOP_LEGEND_H

#include <SFML/Graphics.hpp>
#include <string>

template<class T> class Legend {
    sf::RenderWindow* window;
    std::string content;
    T shape;
    float radius = {};
    sf::Vector2f size;
    sf::Text content_text;
    sf::Font content_font;
public:
    Legend(sf::RenderWindow* w, std::string x, T ob);
    Legend(sf::RenderWindow* w, std::string x, float rad);
    Legend(sf::RenderWindow* w, std::string x, float width, float height);
    void init(float x, float y);
    void init_text(float, float);
    void show();
};

#endif