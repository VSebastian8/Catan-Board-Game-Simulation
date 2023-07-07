#ifndef OOP_STRUCTURES_H
#define OOP_STRUCTURES_H

#include <SFML/Graphics.hpp>
#include <iostream>
#include "player.h"

class Structure{
    static int total_structures;
protected:
    Player* owner;
public:
    Structure();
    virtual ~Structure();
    virtual void purchase(Player&) = 0;
    static void show_total();
    virtual void show(sf::Color, sf::RenderWindow*) = 0;
    [[nodiscard]] virtual bool at(const int&, const int&) const = 0;
    [[nodiscard]] virtual bool is_town() const;
};

class Settlement: public Structure{
protected:
    std::pair<int, int> place;
public:
    Settlement(int x, int y): Structure(), place(std::pair<int, int>(x, y)){}
    virtual void check() = 0;
    void produce(int, int, const std::string&);
    virtual void give_resource(const int&) = 0;
    [[nodiscard]] bool at(const int&, const int&) const override;
    [[nodiscard]] bool is_near(const int&, const int&) const;
};

class Town: public Settlement{
    sf::CircleShape town_mark;
public:
    Town(int x, int y);
    void check() final {std::cout << "town\n"; }
    void purchase(Player&) final;
    void show(sf::Color, sf::RenderWindow*) final;
    void give_resource(const int&) final;
    [[nodiscard]] bool is_town() const override;
};

class City: public Settlement{
    sf::RectangleShape city_mark;
public:
    City(int x, int y);
    void check() final {std::cout << "city\n";}
    void purchase(Player&) final;
    void show(sf::Color, sf::RenderWindow*) final;
    void give_resource(const int&) final;
};

class Road: public Structure{
    std::pair<int, int> start, finish;
    sf::RectangleShape road_mark;
public:
    Road(int x1, int y1, int x2, int y2);
    void calculate_shape();
    void purchase(Player&) final;
    void show(sf::Color, sf::RenderWindow*) final;
    [[nodiscard]] bool at(const int&, const int&) const override;
};

#endif