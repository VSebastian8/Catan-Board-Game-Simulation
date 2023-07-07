#ifndef OOP_TILES_H
#define OOP_TILES_H

#include <SFML/Graphics.hpp>
#include <iostream>
#include <string>
#include <vector>
#include <memory>

class Tile{
protected:
    std::string name, resource;
    const int x, y;
    const float width, disk_width;
    const sf::Vector2f dim;
    const sf::Vector2f disk_dim;
    int dice_roll;
    float x_offset, y_offset;
    std::vector<std::pair<int, int>> adjacent_points;
    sf::RectangleShape square;
    sf::CircleShape disk;
    sf::Vector2f poz;
    sf::Color color;
    sf::Color disk_color;
    sf::Text text;
    sf::Font font;
    bool outlined;
    virtual void print() const;
public:
    explicit Tile(int x = -1, int y = -1, float w = 100, int d = 0);
    Tile(std::string name, std::string res);
    virtual ~Tile() = default;
    void initialize_text();
    friend std::ostream& operator <<(std::ostream& out, const Tile& tile);
    virtual std::shared_ptr<Tile> clone();
    void calculate_offset(sf::Vector2f place);
    sf::RectangleShape show();
    virtual sf::CircleShape show_disk();
    int get_dice_value() const;
    void set_dice_value(int);
    virtual sf::Text show_dice_value();
    void outline_disk(int);
    virtual void calculate_points(int);
    virtual void parse_points( std::vector<std::pair<std::pair<int, int>, std::string>>&, int&);
};

class Useless: virtual public Tile{
public:
    explicit Useless() = default;
    void calculate_points(int) override{} //nu calculam vectorul de puncte pentru desert si ocean
    sf::CircleShape show_disk() override{ return sf::CircleShape(); }
    sf::Text show_dice_value() override{ return {}; }
};

class Ocean: public Useless{
public:
    explicit Ocean(int x = -1, int y = -1, float w = 100);
    std::shared_ptr<Tile> clone() override;
};
class Desert: public Useless{
public:
    explicit Desert(int x = -1, int y = -1, float w = 100, int d = 0);
    std::shared_ptr<Tile> clone() override;
};
class Hill: public Tile{
public:
    explicit Hill(int x = -1, int y = -1, float w = 100, int d = 0);
    std::shared_ptr<Tile> clone() override;
};
class Pasture: public Tile{
public:
    explicit Pasture(int x = -1, int y = -1, float w = 100, int d = 0);
    std::shared_ptr<Tile> clone() override;
};
class Field: public Tile{
public:
    explicit Field(int x = -1, int y = -1, float w = 100, int d = 0);
    std::shared_ptr<Tile> clone() override;
};
class Forest: virtual public Tile{
public:
    explicit Forest(int x = -1, int y = -1, float w = 100, int d = 0);
    std::shared_ptr<Tile> clone() override;
};
class Mountain: virtual public Tile{
public:
    explicit Mountain(int x = -1, int y = -1, float w = 100, int d = 0);
    std::shared_ptr<Tile> clone() override;
};
class Rocky_Jungle: public Forest, public Mountain{
    std::string resource2;
public:
    explicit Rocky_Jungle(int x = -1, int y = -1, float w = 100, int d = 0);
    void print() const override;
    void parse_points( std::vector<std::pair<std::pair<int, int>, std::string>>& data, int& dice) override;
    std::shared_ptr<Tile> clone() override;
};

class tile_factory{
public:
        static Tile forest() { return {"Forest", "wood"}; }
        static Tile hill() { return {"Hill", "brick"}; }
        static Tile pasture() { return {"Pasture", "sheep"}; }
        static Tile field() { return {"Field", "hay"}; }
        static Tile mountain() { return {"Mountain", "rock"}; }
        static Tile rocky_jungle() { return {"Rocky Jungle", "wood + rock"}; }
};

void info_console();

#endif