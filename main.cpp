#include <iostream>
#include <vector>
#include <memory>

class Tile{
protected:
    std::string name, resource;
public:
    Tile() = default;
    virtual ~Tile() = default;
    friend std::ostream& operator <<(std::ostream& out, const Tile& tile){
        tile.show();
        return out;
    }
    virtual void show() const {
        std::cout << name << " gives you: " << resource << "\n";
    }
};
class Hill: public Tile{
public:
    Hill(){name = "Hill"; resource = "brick";}
};
class Field: public Tile{
public:
    Field(){name = "Field"; resource = "hay";}
};
class Pasture: public Tile{
public:
    Pasture(){name = "Pasture"; resource = "sheep";}
};
class Forest: virtual public Tile{
public:
    Forest(){name = "Forest"; resource = "wood";}
};
class Mountain: virtual public Tile{
public:
    Mountain(){name = "Mountain"; resource = "rock";}
};
class Rocky_Jungle: public Forest, public Mountain{
    std::string resource2;
public:
    Rocky_Jungle(){name = "Rocky Jungle"; resource2 = "wood";}
    void show() const override{
        std::cout << name << " gives you: " << resource << " + " << resource2 << "\n";
    }
};

int main () {
    std::vector<std::shared_ptr<Tile>> tiles = {std::make_shared<Hill>(), std::make_shared<Field>(), std::make_shared<Pasture>(),
            std::make_shared<Forest>(), std::make_shared<Mountain>(), std::make_shared<Rocky_Jungle>()};
    for(const auto &i : tiles)
        std::cout << *i;
    return 0;
}