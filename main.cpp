#include <iostream>
#include <vector>
#include <memory>
#include <rlutil.h>

#ifdef __linux__
#include <X11/Xlib.h>
#endif

#include <SFML/Graphics.hpp>
#include <SFML/System.hpp>
#include <SFML/Window.hpp>
#include "tiles.h"
#include "board.h"


class Game{
    sf::Event e;
    sf::VideoMode vm;
    sf::RenderWindow* window;
    Board b;
public:
    Game(): e(sf::Event()), vm(sf::VideoMode(900, 900)),
            window(new sf::RenderWindow(vm, "Catran", sf::Style::Close | sf::Style::Titlebar)),
            b(Board()){}
    void run(){
        b.initialize();
        while(window->isOpen()) {
            while(window->pollEvent(e)){
                switch(e.type)
                {
                    case sf::Event::Closed : window->close(); break;
                    default: break;
                }
            }
            window->clear();
            b.show(window);
            window->display();
        }
    }
};
void info_console(){
    rlutil::setColor(rlutil::YELLOW);
    std::vector<std::shared_ptr<Tile>> info_tiles = {std::make_shared<Forest>(), std::make_shared<Hill>(), std::make_shared<Pasture>(),
                                       std::make_shared<Field>(), std::make_shared<Mountain>(), std::make_shared<Rocky_Jungle>()};
    for(const auto &i : info_tiles)
        std::cout << *i;
    rlutil::setColor(rlutil::WHITE);
}

int main () {
#ifdef __linux__
    XInitThreads();
#endif

    info_console();
    Game g;
    g.run();

    return 0;
}