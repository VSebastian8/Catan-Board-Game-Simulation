#include <iostream>
#include <vector>
#include <memory>
#include <rlutil.h>
#include <limits>
#include <SFML/Graphics.hpp>
#include <SFML/Window.hpp>
#include "tiles.h"
#include "board.h"
#include "interface.h"

int main () {
    info_console();
    auto& g = Game::get_game();
    g.run();

    return 0;
}