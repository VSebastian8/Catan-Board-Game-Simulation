#pragma once

class Tile{
protected:
    std::string name, resource;
    const float x, y;
    std::vector<std::pair<int, int>> adjacent_points;
    static const int width;
    static const sf::Vector2f dim;
    sf::RectangleShape square;
    sf::Vector2f poz;
    sf::Color color;
public:
    explicit Tile(float x = -1, float y = -1): x(x), y(y), adjacent_points(std::vector<std::pair<int, int>>()),
             poz(sf::Vector2f((y + 1) * width,(x + 1) * width)), color(sf::Color(0,0,0)){
        square.setOrigin(0, 0);
        square.setSize(dim);
    } 
    virtual ~Tile() = default;
    friend std::ostream& operator <<(std::ostream& out, const Tile& tile){
        tile.print();
        return out;
    }
    virtual void print() const {
        std::cout << name << " gives you: " << resource << "\n";
    }
    sf::RectangleShape show() {
        square.setPosition(poz);
        square.setFillColor(color);
        return square;
    }
    virtual void calculate_points(){
        //try catch sa nu fie sub 0 sau peste board_dim?
        adjacent_points = {std::pair<int, int>(x - 1, y - 1), std::pair<int, int>(x, y - 1),
                            std::pair<int, int>(x, y - 1), std::pair<int, int>(x, y)};
    }
    void parse_points(){
        for(int i = 0; i < (int)adjacent_points.size(); i++){
            std::cout << adjacent_points[i].first << " " << adjacent_points[i].second << "\n";
        }
    }
};
const int Tile::width = 100;
const sf::Vector2f Tile::dim = sf::Vector2f(100, 100);

class Useless: virtual public Tile{
public:
    explicit Useless() = default;
    void calculate_points() override{} //nu calculam vectorul de puncte pentru desert si ocean
};
class Ocean: public Useless{
public:
    explicit Ocean(float x = -1, float y = -1): Tile(x, y)
        {name = "Ocean"; resource = "none"; color = sf::Color(120, 230, 223 );}
};
class Desert: public Useless{
public:
    explicit Desert(float x = -1, float y = -1): Tile(x, y)
        {name = "Desert"; resource = "none"; color = sf::Color(	250, 213, 165);}
};
class Hill: public Tile{
public:
    explicit Hill(float x = -1, float y = -1): Tile(x, y)
        {name = "Hill"; resource = "brick"; color = sf::Color(236, 148, 101);}
};
class Pasture: public Tile{
public:
    explicit Pasture(float x = -1, float y = -1): Tile(x, y)
        {name = "Pasture"; resource = "sheep"; color = sf::Color(138, 234, 146);}
};
class Field: public Tile{
public:
    explicit Field(float x = -1, float y = -1): Tile(x, y)
    {name = "Field"; resource = "hay"; color = sf::Color(246, 204, 87);}
};
class Forest: virtual public Tile{
public:
    explicit Forest(float x = -1, float y = -1): Tile(x, y)
        {name = "Forest"; resource = "wood"; color = sf::Color(48, 103, 91);}
};
class Mountain: virtual public Tile{
public:
    explicit Mountain(float x = -1, float y = -1): Tile(x, y)
        {name = "Mountain"; resource = "rock"; color = sf::Color(64, 71, 109);}
};
class Rocky_Jungle: public Forest, public Mountain{
    std::string resource2;
public:
    explicit Rocky_Jungle(float x = -1, float y = -1): Tile(x,y), resource2("wood")
        {name = "Rocky Jungle"; color = sf::Color(85, 180, 159);}
    void print() const override{
        std::cout << name << " gives you: " << resource << " + " << resource2 << "\n";
    }
};
