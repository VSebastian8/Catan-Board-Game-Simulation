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
#include "player.h"
#include "structures.h"
#include "board.h"
#include "legend.h"
#include "interface.h"
#include "demo.h"

int main () {
    rlutil::saveDefaultColor();
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