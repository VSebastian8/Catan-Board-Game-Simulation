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
};
int Structure::total_structures = 0;
void Structure::show_total() {
    std::cout << total_structures << "\n";
}

class Road: public Structure{
    std::pair<float, float> start, finish;
public:
    Road(float x1, float y1, float x2, float y2): Structure(), start(std::pair<float, float>(x1, y1)), finish(std::pair<float, float>(x2, y2)){
        if(x1 == x2 && y1 == y2)
            throw wrong_road_error();
        if(std::abs(x1 - x2) + std::abs(y1 - y2) > 1)
            throw wrong_road_error();
    }
    void purchase(Player& p) final;
};

class Settlement: public Structure{
    std::pair<float, float> place;
public:
    Settlement(float x, float y): Structure(), place(std::pair<float, float>(x, y)){}
};

class Town: public Settlement{
public:
    Town(float x, float y): Settlement(x, y){}
    void purchase(Player& p) final;
};

class City: public Settlement{
public:
    City(float x, float y): Settlement(x, y){}
    void purchase(Player& p) final;
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
    p.decrease_res(1, 2);
    p.decrease_res(4, 3);
    owner = &p;

    //+check ca player sa aiba deja oras acolo
    //(delete town?)
}