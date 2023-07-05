#pragma once

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
    int outline_timer;

    virtual void print() const {
        std::cout << name << " gives you: " << resource << "\n";
    }
public:
    explicit Tile(int x = -1, int y = -1, float w = 100, int d = 0): x(x), y(y), width(w), disk_width(w / 4),
            dim(sf::Vector2f(w, w)), disk_dim(sf::Vector2f(w / 4, w / 4)),
            dice_roll(d), x_offset(0), y_offset(0), adjacent_points(std::vector<std::pair<int, int>>()),
            poz(sf::Vector2f(float(x) * width,float(y) * width)), color(sf::Color(0,0,0)), disk_color(sf::Color(220, 220, 220)){
        square.setOrigin(0, 0);
        square.setSize(dim);
        disk.setOrigin(disk_dim);
        disk.setRadius(disk_width);
        outline_timer = 0;
        initialize_text();
    }
    Tile(std::string name, std::string res): name(std::move(name)), resource(std::move(res)),
        x(-1), y(-1), width(25), disk_width(8), dim(sf::Vector2f(25, 25)), disk_dim(sf::Vector2f(8, 8)),
        dice_roll(0), x_offset(0), y_offset(0), adjacent_points(std::vector<std::pair<int, int>>()),
        poz(sf::Vector2f((float)x * width, (float)y * width)), color(sf::Color(0,0,0)),
        disk_color(sf::Color(220, 220, 220)), outline_timer(0){}
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
        text.setPosition(x_offset + (float)x * width + width / 2, y_offset + (float)y * width + width / 2);
    }
    sf::RectangleShape show(){
        square.setPosition(poz + sf::Vector2f(x_offset, y_offset));
        square.setFillColor(color);
        return square;
    }
    virtual sf::CircleShape show_disk(){
        if(dice_roll < 2 || dice_roll > 12)
            return sf::CircleShape();
        disk.setPosition(x_offset + (float)x * width + width / 2, y_offset + (float)y * width + width / 2);
        disk.setFillColor(disk_color);
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
        disk.setOutlineThickness(0);
        if(outline_timer > 0){
            outline_timer--;
            if(outline_timer < 230) {
                disk.setOutlineThickness(3);
            }
        }
        disk.setOutlineColor(sf::Color(181, 112, 230));
        text.setFillColor(color);
        return text;
    }
    void outline_disk(int dice){
        if(dice_roll == dice) {
            outline_timer = 280;
        }
    }
    virtual void calculate_points(int d){
        if(x <= 0 || y <= 0 || x >= d || y >= d)
            throw tile_bounds("(" + std::to_string((int)x) + ", " + std::to_string((int)y) + ")");
        adjacent_points = {std::pair<int, int>(x - 1, y - 1), std::pair<int, int>(x, y - 1),
                            std::pair<int, int>(x - 1, y), std::pair<int, int>(x, y)};
    }
    virtual void parse_points( std::vector<std::pair<std::pair<int, int>, std::string>>& data, int& dice){
        if(dice_roll == dice)
            for(auto point : adjacent_points){
                data.emplace_back(point, resource);
            }
    }
};

void Tile::initialize_text(){
    if (!font.loadFromFile( "assets/georgia_bold.ttf" )){
        rlutil::setColor(rlutil::WHITE);
        throw font_error("georgia bold");
    }
    text.setFont(font);
    text.setOrigin(0.1f * width, 0.15f * width);
    text.setString(std::to_string((int)dice_roll));
    text.setCharacterSize((unsigned int)(24 * width / 100));
    text.setPosition(x_offset + (float)x * width + width / 2, y_offset + (float)y * width + width / 2);
}

class Useless: virtual public Tile{
public:
    explicit Useless() = default;
    void calculate_points(int) override{} //nu calculam vectorul de puncte pentru desert si ocean
    sf::CircleShape show_disk() override{return sf::CircleShape();}
    sf::Text show_dice_value() override{return {};}
};
class Ocean: public Useless{
public:
    explicit Ocean(int x = -1, int y = -1, float w = 100): Tile(x, y, w, 0)
        {name = "Ocean"; resource = "none"; color = sf::Color(120, 230, 223 );}
    std::shared_ptr<Tile> clone() override
        {return std::make_shared<Ocean>(*this);}
};
class Desert: public Useless{
public:
    explicit Desert(int x = -1, int y = -1, float w = 100, int d = 0): Tile(x, y, w, d)
        {name = "Desert"; resource = "none"; color = sf::Color(	227, 230, 193);}
    std::shared_ptr<Tile> clone() override
        {return std::make_shared<Desert>(*this);}
};
class Hill: public Tile{
public:
    explicit Hill(int x = -1, int y = -1, float w = 100, int d = 0): Tile(x, y, w,d)
        {name = "Hill"; resource = "brick"; color = sf::Color(236, 148, 101);}
    std::shared_ptr<Tile> clone() override
        {return std::make_shared<Hill>(*this);}
};
class Pasture: public Tile{
public:
    explicit Pasture(int x = -1, int y = -1, float w = 100, int d = 0): Tile(x, y, w,d)
        {name = "Pasture"; resource = "sheep"; color = sf::Color(138, 234, 146);}
    std::shared_ptr<Tile> clone() override
        {return std::make_shared<Pasture>(*this);}
};
class Field: public Tile{
public:
    explicit Field(int x = -1, int y = -1, float w = 100, int d = 0): Tile(x, y, w, d)
    {name = "Field"; resource = "hay"; color = sf::Color(246, 204, 87);}
    std::shared_ptr<Tile> clone() override
        {return std::make_shared<Field>(*this);}
};
class Forest: virtual public Tile{
public:
    explicit Forest(int x = -1, int y = -1, float w = 100, int d = 0): Tile(x, y, w, d)
        {name = "Forest"; resource = "wood"; color = sf::Color(48, 103, 91);}
    std::shared_ptr<Tile> clone() override
        {return std::make_shared<Forest>(*this);}
};
class Mountain: virtual public Tile{
public:
    explicit Mountain(int x = -1, int y = -1, float w = 100, int d = 0): Tile(x, y, w,d)
        {name = "Mountain"; resource = "rock"; color = sf::Color(64, 71, 109);}
    std::shared_ptr<Tile> clone() override
        {return std::make_shared<Mountain>(*this);}
};
class Rocky_Jungle: public Forest, public Mountain{
    std::string resource2;
public:
    explicit Rocky_Jungle(int x = -1, int y = -1, float w = 100, int d = 0): Tile(x, y, w,d), resource2("wood")
        {name = "Rocky Jungle"; color = sf::Color(85, 180, 159);}
    void print() const override{
        std::cout << name << " gives you: " << resource << " + " << resource2 << "\n";
    }
    void parse_points( std::vector<std::pair<std::pair<int, int>, std::string>>& data, int& dice) override{
        if(dice_roll == dice)
            for(auto point : adjacent_points){
                data.emplace_back(point, resource);
                data.emplace_back(point, resource2);
            }
    }
    std::shared_ptr<Tile> clone() override
        {return std::make_shared<Rocky_Jungle>(*this);}
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

void info_console(){
    rlutil::setColor(rlutil::YELLOW);
    std::vector<Tile> info_tiles = {tile_factory::forest(), tile_factory::hill(), tile_factory::pasture(), tile_factory::field(),
                                    tile_factory::mountain(), tile_factory::rocky_jungle()};
    for(const auto &i : info_tiles)
        std::cout << i;
    rlutil::resetColor();
    std::cout << "You can pause or quit anytime by pressing P/Q\n\n";
}