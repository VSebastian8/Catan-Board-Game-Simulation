#pragma once

class Tile{
protected:
    std::string name, resource;
    const float x, y;
    const float width;
    const float disk_width;
    const sf::Vector2f dim;
    const sf::Vector2f disk_dim;
    int dice_roll;
    float x_offset;
    float y_offset;
    std::vector<std::pair<int, int>> adjacent_points;
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
    explicit Tile(float x = -1, float y = -1, float w = 100, int d = 0): x(x), y(y), width(w), disk_width(w / 4),
            dim(sf::Vector2f(w, w)), disk_dim(sf::Vector2f(w / 4, w / 4)),
            dice_roll(d), x_offset(0), y_offset(0), adjacent_points(std::vector<std::pair<int, int>>()),
            poz(sf::Vector2f(x * width,y * width)), color(sf::Color(0,0,0)){
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
    virtual std::shared_ptr<Tile> clone(){
            return std::make_shared<Tile>(*this);
    };
    void calculate_offset(sf::Vector2f place){
        x_offset = place.x;
        y_offset = place.y;
        text.setPosition(x_offset + x * width + width / 2, y_offset + y * width + width / 2);
    }
    sf::RectangleShape show(){
        square.setPosition(poz + sf::Vector2f(x_offset, y_offset));
        square.setFillColor(color);
        return square;
    }
    virtual sf::CircleShape show_disk(){
        if(dice_roll < 2 || dice_roll > 12)
            return sf::CircleShape();
        disk.setPosition(x_offset + x * width + width / 2, y_offset + y * width + width / 2);
        disk.setFillColor(sf::Color(220, 220, 220));
        return disk;
    }
    void initialize_text();
    int get_dice_value() const {return dice_roll;}
    void set_dice_value(int score){
        dice_roll = score;
        text.setString(std::to_string((int)dice_roll));
    }
    virtual sf::Text show_dice_value(){
        if(dice_roll < 2 || dice_roll > 12)
            return {};
        text.setFillColor(color);
        return text;
    }
    virtual void calculate_points(float d){
        if(x <= 0 || y <= 0 || x >= d || y >= d)
            throw tile_bounds("(" + std::to_string((int)x) + ", " + std::to_string((int)y) + ")");
        adjacent_points = {std::pair<int, int>(x - 1, y - 1), std::pair<int, int>(x, y - 1),
                            std::pair<int, int>(x, y - 1), std::pair<int, int>(x, y)};
    }
    void parse_points(){
        for(auto point : adjacent_points){
            std::cout << point.first << " " << point.second << "\n";
        }
    }
};

void Tile::initialize_text(){
    if (!font.loadFromFile( "georgia bold.ttf" )){
        rlutil::setColor(rlutil::WHITE);
        throw font_error("georgia bold");
    }
    text.setFont(font);
    text.setOrigin(0.1f * width, 0.15f * width);
    text.setString(std::to_string((int)dice_roll));
    text.setCharacterSize((unsigned int)(24 * width / 100));
    text.setPosition(x_offset + x * width + width / 2, y_offset + y * width + width / 2);
}

class Useless: virtual public Tile{
public:
    explicit Useless() = default;
    void calculate_points(float) override{} //nu calculam vectorul de puncte pentru desert si ocean
    sf::CircleShape show_disk() override{return sf::CircleShape();}
    sf::Text show_dice_value() override{return {};}
};
class Ocean: public Useless{
public:
    explicit Ocean(float x = -1, float y = -1, float w = 100): Tile(x, y, w, 0)
        {name = "Ocean"; resource = "none"; color = sf::Color(120, 230, 223 );}
    std::shared_ptr<Tile> clone() override
        {return std::make_shared<Ocean>(*this);}
};
class Desert: public Useless{
public:
    explicit Desert(float x = -1, float y = -1, float w = 100, int d = 0): Tile(x, y, w, d)
        {name = "Desert"; resource = "none"; color = sf::Color(	250, 213, 165);}
    std::shared_ptr<Tile> clone() override
        {return std::make_shared<Desert>(*this);}
};
class Hill: public Tile{
public:
    explicit Hill(float x = -1, float y = -1, float w = 100, int d = 0): Tile(x, y, w,d)
        {name = "Hill"; resource = "brick"; color = sf::Color(236, 148, 101);}
    std::shared_ptr<Tile> clone() override
        {return std::make_shared<Hill>(*this);}
};
class Pasture: public Tile{
public:
    explicit Pasture(float x = -1, float y = -1, float w = 100, int d = 0): Tile(x, y, w,d)
        {name = "Pasture"; resource = "sheep"; color = sf::Color(138, 234, 146);}
    std::shared_ptr<Tile> clone() override
        {return std::make_shared<Pasture>(*this);}
};
class Field: public Tile{
public:
    explicit Field(float x = -1, float y = -1, float w = 100, int d = 0): Tile(x, y, w, d)
    {name = "Field"; resource = "hay"; color = sf::Color(246, 204, 87);}
    std::shared_ptr<Tile> clone() override
        {return std::make_shared<Field>(*this);}
};
class Forest: virtual public Tile{
public:
    explicit Forest(float x = -1, float y = -1, float w = 100, int d = 0): Tile(x, y, w, d)
        {name = "Forest"; resource = "wood"; color = sf::Color(48, 103, 91);}
    std::shared_ptr<Tile> clone() override
        {return std::make_shared<Forest>(*this);}
};
class Mountain: virtual public Tile{
public:
    explicit Mountain(float x = -1, float y = -1, float w = 100, int d = 0): Tile(x, y, w,d)
        {name = "Mountain"; resource = "rock"; color = sf::Color(64, 71, 109);}
    std::shared_ptr<Tile> clone() override
        {return std::make_shared<Mountain>(*this);}
};
class Rocky_Jungle: public Forest, public Mountain{
    std::string resource2;
public:
    explicit Rocky_Jungle(float x = -1, float y = -1, float w = 100, int d = 0): Tile(x, y, w,d), resource2("wood")
        {name = "Rocky Jungle"; color = sf::Color(85, 180, 159);}
    void print() const override{
        std::cout << name << " gives you: " << resource << " + " << resource2 << "\n";
    }
    std::shared_ptr<Tile> clone() override
        {return std::make_shared<Rocky_Jungle>(*this);}
};
