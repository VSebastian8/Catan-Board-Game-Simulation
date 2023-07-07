#include "code/tiles.h"
#include "code/errors.h"
#include "code/interface.h"

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