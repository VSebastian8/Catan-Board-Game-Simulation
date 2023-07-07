#ifndef OOP_PLAYER_H
#define OOP_PLAYER_H

#include <SFML/Graphics.hpp>
#include <iostream>
#include <vector>
#include <string>
#include <memory>

class Structure;
class Player{
    static int player_count;
    static std::vector<sf::Color> player_colors;
    std::string name;
    int player_number, score;
    sf::Color color;
    bool turn;
    std::vector<int> resources; //[brick, sheep, hay, wood, rock]
    sf::Text info;
    sf::Font info_font;
    std::vector<std::shared_ptr<Structure>> structures;
public:
    explicit Player(std::string nm = "?", const std::vector<int>& res = {3, 4, 2, 7, 3});
    ~Player() = default;
    friend std::ostream& operator <<(std::ostream&, const Player&);
    sf::Text show();
    void set_turn(bool);
    std::vector<int> get_resources() const;
    int get_score() const;
    void add_structure(const std::shared_ptr<Structure>&);
    void increase_res(int, int);
    void decrease_res(int, int);
    void show_structures(sf::RenderWindow*);
    bool town_at(int, int);
    bool connects_to(int, int, int, int);
};

#endif