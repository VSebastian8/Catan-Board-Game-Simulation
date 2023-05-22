#pragma once

class Tile{
protected:
    std::string name, resource;
    const float x, y;
    std::vector<std::pair<int, int>> adjacent_points;
    const float dice_roll;
    static const float width;
    static const float disk_width;
    static const sf::Vector2f dim;
    static const sf::Vector2f disk_dim;
    sf::RectangleShape square;
    sf::CircleShape disk;
    sf::Vector2f poz;
    sf::Color color;
    sf::Text text;
    sf::Font font;

    virtual void print() const {
        std::cout << name << " gives you: " << resource << "\n";
    }
public:
    explicit Tile(float x = -1, float y = -1, float d = 0): x(x), y(y), adjacent_points(std::vector<std::pair<int, int>>()),
            dice_roll(d), poz(sf::Vector2f((y + 1) * width,(x + 1) * width)), color(sf::Color(0,0,0)){
        square.setOrigin(0, 0);
        square.setSize(dim);
        disk.setOrigin(disk_dim);
        disk.setRadius(disk_width);
        initialize_text();
    } 
    virtual ~Tile() = default;
    friend std::ostream& operator <<(std::ostream& out, const Tile& tile) {
        tile.print();
        return out;
    }
    sf::RectangleShape show(){
        square.setPosition(poz);
        square.setFillColor(color);
        return square;
    }
    virtual sf::CircleShape show_disk(){
        disk.setPosition((y + 1) * width + width / 2,(x + 1) * width + width / 2);
        disk.setFillColor(sf::Color(220, 220, 220));
        return disk;
    }
    void initialize_text();
    virtual sf::Text show_dice_value(){
        text.setFillColor(color);
        return text;
    }
    virtual void calculate_points(){
        //try catch sa nu fie sub 0 sau peste board_dim?
        adjacent_points = {std::pair<int, int>(x - 1, y - 1), std::pair<int, int>(x, y - 1),
                            std::pair<int, int>(x, y - 1), std::pair<int, int>(x, y)};
    }
    void parse_points(){
        for(auto point : adjacent_points){
            std::cout << point.first << " " << point.second << "\n";
        }
    }
};
const float Tile::width = 100;
const float Tile::disk_width = 25;
const sf::Vector2f Tile::dim = sf::Vector2f(100, 100);
const sf::Vector2f Tile::disk_dim = sf::Vector2f(25, 25);

void Tile::initialize_text() {
    if (!font.loadFromFile( "assets/georgia bold.ttf" ))
    {
        std::cout << "Error loading text \n";
        //   throw FileError("font file error");
    }
    text.setFont(font);
    text.setOrigin(10, 15);
    text.setString(std::to_string((int)dice_roll));
    text.setCharacterSize(24);
    text.setPosition((y + 1) * width + width / 2, (x + 1) * width + width / 2);
}

class Useless: virtual public Tile{
public:
    explicit Useless() = default;
    void calculate_points() override{} //nu calculam vectorul de puncte pentru desert si ocean
    sf::CircleShape show_disk() override{return sf::CircleShape();}
    sf::Text show_dice_value() override{return {};}
};
class Ocean: public Useless{
public:
    explicit Ocean(float x = -1, float y = -1, float d = 0): Tile(x, y, d)
        {name = "Ocean"; resource = "none"; color = sf::Color(120, 230, 223 );}
};
class Desert: public Useless{
public:
    explicit Desert(float x = -1, float y = -1, float d = 0): Tile(x, y, d)
        {name = "Desert"; resource = "none"; color = sf::Color(	250, 213, 165);}
};
class Hill: public Tile{
public:
    explicit Hill(float x = -1, float y = -1, float d = 0): Tile(x, y, d)
        {name = "Hill"; resource = "brick"; color = sf::Color(236, 148, 101);}
};
class Pasture: public Tile{
public:
    explicit Pasture(float x = -1, float y = -1, float d = 0): Tile(x, y, d)
        {name = "Pasture"; resource = "sheep"; color = sf::Color(138, 234, 146);}
};
class Field: public Tile{
public:
    explicit Field(float x = -1, float y = -1, float d = 0): Tile(x, y, d)
    {name = "Field"; resource = "hay"; color = sf::Color(246, 204, 87);}
};
class Forest: virtual public Tile{
public:
    explicit Forest(float x = -1, float y = -1, float d = 0): Tile(x, y, d)
        {name = "Forest"; resource = "wood"; color = sf::Color(48, 103, 91);}
};
class Mountain: virtual public Tile{
public:
    explicit Mountain(float x = -1, float y = -1, float d = 0): Tile(x, y, d)
        {name = "Mountain"; resource = "rock"; color = sf::Color(64, 71, 109);}
};
class Rocky_Jungle: public Forest, public Mountain{
    std::string resource2;
public:
    explicit Rocky_Jungle(float x = -1, float y = -1, float d = 0): Tile(x, y, d), resource2("wood")
        {name = "Rocky Jungle"; color = sf::Color(85, 180, 159);}
    void print() const override{
        std::cout << name << " gives you: " << resource << " + " << resource2 << "\n";
    }
};
