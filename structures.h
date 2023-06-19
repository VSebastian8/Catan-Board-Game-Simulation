#pragma once

class Structure{
    static int total_structures;
protected:
    Player* owner;
public:
    Structure(): owner(nullptr){
        total_structures++;
    };
    virtual ~Structure(){
        total_structures--;
    };
    virtual void purchase(Player&) = 0;
    static void show_total();
    virtual void show(sf::Color, sf::RenderWindow*) = 0;
    virtual bool is_town(float, float){ return false; }
};
int Structure::total_structures = 0;
void Structure::show_total() {
    std::cout << total_structures << "\n";
}

class Road: public Structure{
    std::pair<float, float> start, finish;
    sf::RectangleShape road_mark;
public:
    Road(float x1, float y1, float x2, float y2): Structure(), start(std::pair<float, float>(x1, y1)), finish(std::pair<float, float>(x2, y2)){
        if(x1 == x2 && y1 == y2)
            throw wrong_road_error();
        if(std::abs(x1 - x2) + std::abs(y1 - y2) > 1)
            throw wrong_road_error();
        calculate_shape();
    }
    void purchase(Player&) final;
    void show(sf::Color, sf::RenderWindow*) final;
    void calculate_shape();
};

class Settlement: public Structure{
protected:
    std::pair<float, float> place;
public:
    Settlement(float x, float y): Structure(), place(std::pair<float, float>(x, y)){}
};

class Town: public Settlement{
    sf::CircleShape town_mark;
public:
    Town(float x, float y): Settlement(x, y){
        town_mark.setRadius(12.5);
        town_mark.setOrigin(sf::Vector2f(12.5, 12.5));
        town_mark.setPosition(400 + x * 100, 200 + y * 100);
    }
    void purchase(Player& p) final;
    void show(sf::Color, sf::RenderWindow*) final;
    bool is_town(float x, float y) final { return (place.first == x && place.second == y); }
};

class City: public Settlement{
    sf::RectangleShape city_mark;
public:
    City(float x, float y): Settlement(x, y){
        city_mark.setSize(sf::Vector2f(25, 25));
        city_mark.setOrigin(sf::Vector2f(12.5, 12.5));
        city_mark.setPosition(400 + x * 100, 200 + y * 100);
    }
    void purchase(Player& p) final;
    void show(sf::Color, sf::RenderWindow*) final;
};

void Road::purchase(Player &p) {
    //cost 1 brick, 1 wood
    auto resources = p.get_resources();
    if(resources[0] < 1)
        throw resource_error("Road", "bricks");
    if(resources[3] < 1)
        throw resource_error("Road", "wood");
    p.decrease_res(0, 1);
    p.decrease_res(3, 1);
    owner = &p;
}

void Town::purchase(Player &p) {
    //cost 1 brick, 1 sheep, 1 hay, 1 wood
    auto resources = p.get_resources();
    if(resources[0] < 1)
        throw resource_error("Town", "bricks");
    if(resources[1] < 1)
        throw resource_error("Town", "sheep");
    if(resources[2] < 1)
        throw resource_error("Town", "hay");
    if(resources[3] < 1)
        throw resource_error("Town", "wood");
    p.decrease_res(0, 1);
    p.decrease_res(1, 1);
    p.decrease_res(2, 1);
    p.decrease_res(3, 1);
    owner = &p;
}

void City::purchase(Player &p) {
    //cost 2 sheep, 3 rock
    auto resources = p.get_resources();
    if(resources[1] < 2)
        throw resource_error("City", "sheep");
    if(resources[4] < 3)
        throw resource_error("City", "rocks");
    if(!p.town_at(place.first, place.second))
        throw city_error();
    p.decrease_res(1, 2);
    p.decrease_res(4, 3);
    owner = &p;
}
void Road::show(sf::Color col, sf::RenderWindow* w){
    road_mark.setFillColor(col);
    w->draw(road_mark);
}

void Road::calculate_shape() {
    if(start.first == finish.first) {
        road_mark.setSize(sf::Vector2f(20, 60));
        road_mark.setOrigin(sf::Vector2f(10, 30));
        road_mark.setPosition(400 + start.first * 100, 250 + std::min(start.second, finish.second) * 100);
    }
    else if(start.second == finish.second) {
        road_mark.setSize(sf::Vector2f(60, 20));
        road_mark.setOrigin(sf::Vector2f(30, 10));
        road_mark.setPosition(450 + std::min(start.first, finish.first) * 100, 200 + finish.second * 100);
    }
}

void Town::show(sf::Color col, sf::RenderWindow* w){
    town_mark.setFillColor(col);
    w->draw(town_mark);
}
void City::show(sf::Color col, sf::RenderWindow* w){
    city_mark.setFillColor(col);
    w->draw(city_mark);
}
void Player::show_structures(sf::RenderWindow* window) {
    for(const auto& s : structures){
        s->show(color, window);
    }
}
bool Player::town_at(float x, float y){
    for (unsigned i = 0; i < structures.size(); ++i){
        if(structures[i]->is_town(x, y)){
            structures.erase (structures.begin() + i);
            return true;
        }
    }
    return false;
}