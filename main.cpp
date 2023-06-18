#include <SFML/Graphics.hpp>
#include <SFML/Window.hpp>
#include <iostream>
#include <vector>
#include <memory>
#include <limits>
#include <algorithm>
#include <rlutil.h>
#include "errors.h"
#include "tiles.h"
#include "board.h"
#include "interface.h"

int main () {
    info_console();
    try{
        auto& g = Game::get_game();
        g.run();
    }
    catch(font_error &err){
        std::cout << err.what() << "\n";
    }
    return 0;
}