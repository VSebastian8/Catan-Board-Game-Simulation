#include <SFML/Graphics.hpp>
#include <SFML/Window.hpp>
#include <iostream>
#include <vector>
#include <memory>
#include <limits>
#include <algorithm>
#include <rlutil.h>

#include "code/errors.h"
#include "code/tiles.h"
#include "code/player.h"
#include "code/structures.h"
#include "code/board.h"
#include "code/legend.h"
#include "code/interface.h"
#include "code/animations.h"
#include "code/demo.h"

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