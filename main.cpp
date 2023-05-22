#include <SFML/Graphics.hpp>
#include <SFML/Window.hpp>
#include <iostream>
#include <vector>
#include <memory>
#include <limits>
#include <algorithm>
#include <rlutil.h>
#include <random>
#include "tiles.h"
#include "board.h"
#include "interface.h"

int main () {
    info_console();
    auto& g = Game::get_game();
    g.run();

    return 0;
}