#include <iostream>
#include <vector>
#include <memory>
#define NOMINMAX
#include <limits>
#include <rlutil.h>
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