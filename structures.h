#pragma once

class Structure{
    static int total_structures;
    std::shared_ptr<Player> owner;
public:
    Structure(){
        total_structures++;
    };
    virtual ~Structure(){
        total_structures--;
    };
    virtual void purchase(std::vector<int>&) = 0;
};
int Structure::total_structures = 0;
class Road: public Structure{
    std::pair<float, float> start, finish;
public:
    void purchase(std::vector<int> &resoures) final{
        //cost 1 brick, 1 wood

        //if not enough resources throw error
        //else subtract resources from player
    }
};
class Settlement: public Structure{
    std::pair<float, float> place;
public:
    Settlement(float x, float y): Structure(), place(std::pair<float, float>(x, y)){}
};
class Town: public Settlement{
public:
    Town(float x, float y): Settlement(x, y){}
    void purchase(std::vector<int> &resoures) final{
        //cost 1 brick, 1 sheep, 1 hay, 1 wood
    }
};
class City: public Settlement{
public:
    void purchase(std::vector<int> &resoures) final{
        //cost 2 sheep, 3 rock

        //+check ca player sa aiba deja oras acolo
        //(delete town?)
    }
};