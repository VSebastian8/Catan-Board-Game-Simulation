#include "structures.h"
#include "errors.h"

int Structure::total_structures = 0;
Structure::Structure() : owner(nullptr){
    total_structures++;
}
Structure::~Structure() {
    total_structures--;
}
void Structure::show_total() {
    std::cout << total_structures << "\n";
}
bool Structure::is_town() const {
    return false;
}


void Settlement::produce(int x, int y, const std::string &res) {
    if(place.first == x + 1 && place.second == y + 1){
        int number;
        if(res == "brick")
            number = 0;
        else if(res == "sheep")
            number = 1;
        else if(res == "hay")
            number = 2;
        else if(res == "wood")
            number = 3;
        else if(res == "rock")
            number = 4;
        give_resource(number);
    }
}
bool Settlement::at(const int& x, const int& y) const {
    return(x == place.first && y == place.second);
}
bool Settlement::is_near(const int& x, const int& y) const {
    return((x == place.first && (y == place.second - 1 || y == place.second + 1)) ||
           (y == place.second && (x == place.first - 1 || x == place.first + 1)));
}


Town::Town(int x, int y) : Settlement(x, y){
    town_mark.setRadius(12.5);
    town_mark.setOrigin(sf::Vector2f(12.5, 12.5));
    town_mark.setPosition(float(400 + x * 100), float(200 + y * 100));
}
void Town::purchase(Player& p) {
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
void Town::show(sf::Color col, sf::RenderWindow* w){
    town_mark.setFillColor(col);
    w->draw(town_mark);
}
void Town::give_resource(const int& num) {
    owner->increase_res(num, 1);
}
bool Town::is_town() const { return true; }


City::City(int x, int y) : Settlement(x, y){
    city_mark.setSize(sf::Vector2f(25, 25));
    city_mark.setOrigin(sf::Vector2f(12.5, 12.5));
    city_mark.setPosition(float(400 + x * 100), float(200 + y * 100));
}
void City::purchase(Player& p) {
    //cost 2 sheep, 3 rock
    auto resources = p.get_resources();
    if(resources[1] < 2)
        throw resource_error("City", "sheep");
    if(resources[4] < 3)
        throw resource_error("City", "rocks");
    if(!p.town_at(place.first, place.second))
        throw lonely_city();
    p.decrease_res(1, 2);
    p.decrease_res(4, 3);
    owner = &p;
}
void City::show(sf::Color col, sf::RenderWindow* w){
    city_mark.setFillColor(col);
    w->draw(city_mark);
}
void City::give_resource(const int& num) {
    owner->increase_res(num, 2);
}


Road::Road(int x1, int y1, int x2, int y2) : Structure(), start(std::pair<int, int>(x1, y1)), finish(std::pair<int, int>(x2, y2)){
    if(x1 == x2 && y1 == y2)
        throw weird_road();
    if(std::abs(x1 - x2) + std::abs(y1 - y2) > 1)
        throw weird_road();
    calculate_shape();
}
void Road::calculate_shape() {
    if(start.first == finish.first) {
        road_mark.setSize(sf::Vector2f(20, 60));
        road_mark.setOrigin(sf::Vector2f(10, 30));
        road_mark.setPosition(float(400 + start.first * 100), float(250 + std::min(start.second, finish.second) * 100));
    }
    else if(start.second == finish.second) {
        road_mark.setSize(sf::Vector2f(60, 20));
        road_mark.setOrigin(sf::Vector2f(30, 10));
        road_mark.setPosition(float(450 + std::min(start.first, finish.first) * 100), float(200 + finish.second * 100));
    }
}
void Road::purchase(Player& p) {
    //cost 1 brick, 1 wood
    if(!p.connects_to(start.first, start.second, finish.first, finish.second))
        throw unconnected_road();
    auto resources = p.get_resources();
    if(resources[0] < 1)
        throw resource_error("Road", "bricks");
    if(resources[3] < 1)
        throw resource_error("Road", "wood");
    p.decrease_res(0, 1);
    p.decrease_res(3, 1);
    owner = &p;
}
void Road::show(sf::Color col, sf::RenderWindow* w){
    road_mark.setFillColor(col);
    w->draw(road_mark);
}
bool Road::at(const int& x, const int& y) const {
    return ((x == start.first && y == start.second) || (x == finish.first && y == finish.second));
}