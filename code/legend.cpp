#include "legend.h"
#include "errors.h"
#include <iostream>

template<class T> Legend<T>::Legend(sf::RenderWindow *w, std::string x, T ob): window(w), content(std::move(x)), shape(ob){}
template<>Legend<sf::CircleShape>::Legend(sf::RenderWindow *w, std::string x, float rad): window(w), content(std::move(x)), radius(rad){}
template<>Legend<sf::RectangleShape>::Legend(sf::RenderWindow *w, std::string x, float width, float height): window(w), content(std::move(x)), size(sf::Vector2f(width, height)){}

template<class T> void Legend<T>::init_text(float x, float y) {
    if (!content_font.loadFromFile( "assets/georgia_bold.ttf")){
        throw font_error("georgia bold");
    }
    content_text.setString(content);
    content_text.setFont(content_font);
    content_text.setCharacterSize(16);
    content_text.setOrigin(0, 8);
    content_text.setFillColor(sf::Color(227, 230, 193));
    content_text.setPosition(x, y);
}

template<class T> void Legend<T>::init(float x, float y) {
    std::cout << shape << "\n";
    init_text(x, y);
}

template<> void Legend<sf::CircleShape>::init(float x, float y) {
    shape.setFillColor(sf::Color(72, 23, 179));
    shape.setOrigin(radius, radius);
    shape.setRadius(radius);
    shape.setPosition(x, y);

    init_text(x + 50, y);
}

template<> void Legend<sf::RectangleShape>::init(float x, float y) {
    if(size.x == size.y)
        shape.setFillColor(sf::Color(212, 23, 74));
    else
        shape.setFillColor(sf::Color(27, 209, 164));
    shape.setOrigin(size.x / 2, size.y / 2);
    shape.setSize(size);
    shape.setPosition(x, y);

    init_text(x + 50, y);
}

template<class T> void Legend<T>::show() {
    window->draw(content_text);
}

template<> void Legend<sf::CircleShape>::show() {
    window->draw(shape);
    window->draw(content_text);
}

template<> void Legend<sf::RectangleShape>::show() {
    window->draw(shape);
    window->draw(content_text);
}

template class Legend<double>;
template class Legend<sf::CircleShape>;
template class Legend<sf::RectangleShape>;