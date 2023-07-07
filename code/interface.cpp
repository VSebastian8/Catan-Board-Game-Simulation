#include "interface.h"
#include "errors.h"
#include "legend.h"
#include "rlutil.h"
#include <random>

Game::Game() : time(0), demo_time(0), e(sf::Event()), vm(sf::VideoMode(1500, 1000)),
               window(new sf::RenderWindow(vm, "Catran", sf::Style::Close | sf::Style::Titlebar)),
               b_final(board_builder().tile(100).offset(400, 200).build()),
               b_ocean(board_builder().tile(50).offset(100, 50).build()),
               b_tiled(board_builder().tile(50).offset(575, 50).build()),
               b_tiled2(board_builder().tile(50).offset(575, 50).build()),
               b_scored(board_builder().tile(50).offset(1050, 50).build()),
               b_scored2(board_builder().tile(50).offset(1050, 50).build()),
               dice1(0), dice2(0), dice_timer(0), outline_timer(0), generation_timer(0){
    turn_speed = 25; //(numar secunde) / 30fps
    roll_speed = 200;
    transaction_speed = 100;
    done = false;
    paused = false;
    initialize_dice_text();
}
Game::~Game() {
    delete window;
}
Game &Game::get_game() {
    static Game game;
    return game;
}

void Game::run() {
    rlutil::saveDefaultColor();
    b_tiled.initialize(1);
    b_tiled2.initialize(2);
    b_scored.initialize(3);
    b_scored2.initialize(0);
    b_final.initialize();

    b_scored |= b_tiled2;
    b_scored2 |= b_scored;

    players = {Player("Ted", {3, 3, 3, 3, 3}), Player("Robin", {3, 3, 3, 3, 3}), Player("Barney", {3, 3, 3, 3, 3})};
    current_player = &players[0];

    init_demo();
    int cooldown = 0;

    Legend<double> lg_exit(window, "The demo is done now. You can exit whenever you're ready.", 61.5);
    lg_exit.init(500, 175);
    Legend<sf::CircleShape> lg_town(window, "Town", 12.5);
    lg_town.init(1280, 800);
    Legend<sf::RectangleShape> lg_city(window, "City", 25, 25);
    lg_city.init(1280, 850);
    Legend<sf::RectangleShape> lg_road(window, "Road", 60, 20);
    lg_road.init(1280, 900);

    bool animate_board = true, animate_generation = true;
    if (!animation_font.loadFromFile( "assets/georgia_bold.ttf"))    {
        rlutil::resetColor();
        throw font_error("georgia bold");
    }
    animation_text.setFont(animation_font);
    animation_text.setCharacterSize(24);
    animation_text.setPosition(610,  480);
    animation_text.setFillColor(sf::Color(120, 230, 223 ));

    window->setFramerateLimit(30);
    while(window->isOpen()) {
        while(window->pollEvent(e)){
            switch(e.type)
            {
                case sf::Event::Closed : window->close(); break;
                default: break;
            }
        }
        if(animate_generation){
            //-board generation
            if(!paused)
                show_generation(animate_generation);
            window->display();
        }
        else if(animate_board){
            //--board animation
            if(!paused)
                b_final.animate(window, animate_board);
            window->display();
        }
        else{
            //--demo thing
            window->clear();
            if(!done)
                simulation(time);
            else
                lg_exit.show();

            lg_town.show();
            lg_city.show();
            lg_road.show();

            b_final.show(window);
            if(!paused)
                dice_animation();
            window->draw(dice_text);

            for(auto p : players) {
                p.show_structures(window);
                window->draw(p.show());
            }
            if(time < 50000 && !paused)
                time++;
            if(outline_timer > 0 && !paused){
                outline_timer--;
                if(outline_timer == 0)
                    b_final.outline_tiles(-1);
            }
            window->display();
        }
        check_input(cooldown);
    }
}

bool on_board(const std::vector<int> &list) {
    if(list[0] < 1 || list[0] > 6 || list[1] < 1 || list[1] > 6)
        return false;
    if(list[0] == 1 || list[0] == 6)
        if(list[1] == 1 || list[1] == 6)
            return false;
    if(list.size() == 4){
        if(list[2] < 1 || list[2] > 6 || list[3] < 1 || list[3] > 6)
            return false;
        if(list[2] == 1 || list[2] == 6)
            if(list[3] == 1 || list[3] == 6)
                return false;
    }
    return true;
}
void Game::check_spots(const int& x, const int& y) {
    for(auto const& building : buildings)
        if (auto b = building.lock()){
            if(b->at(x, y))
                throw spot_taken();
            if(b->is_near(x, y))
                throw spot_neighbour();
        }
}

void Game::transaction(Player &p, const std::string& type, const std::vector<int>& list) {
    try{
        if(!on_board(list))
            throw bad_placement("Structure coordinates outside of the board");
        std::shared_ptr<Structure> s;
        if(type == "Town"){
            check_spots(list[0], list[1]);
            s = std::make_shared<Town>(list[0], list[1]);
        }
        else if(type == "Road"){
            s = std::make_shared<Road>(list[0], list[1], list[2], list[3]);
        }
        else if(type == "City"){
            s = std::make_shared<City>(list[0], list[1]);
        }
        s->purchase(p);
        //if we didn't throw any errors
        p.add_structure(s);
        //Downcasting deoarece buildings e vector de weak_ptr<Settlement>
        //nu retinem pointeri catre roads, doar catre towns/cities
        std::shared_ptr<Settlement> sb = std::dynamic_pointer_cast<Settlement, Structure>(s);
        //daca da fail, sb = null_ptr, nu il adaugam in buildings
        if(sb)
            buildings.push_back(std::weak_ptr<Settlement>(sb));
    }
    catch(resource_error& err){
        rlutil::setColor(rlutil::LIGHTCYAN);
        std::cout << err.what() << "\n";
    }
    catch(bad_placement& err){
        rlutil::setColor(rlutil::LIGHTRED);
        std::cout << err.what() << "\n";
    }
    rlutil::resetColor();
}


void Game::initialize_dice_text() {
    if (!dice_font.loadFromFile( "assets/georgia_bold.ttf")){
        throw font_error("georgia bold");
    }
    dice_text.setFont(dice_font);
    dice_text.setString("Dice roll: ");
    dice_text.setPosition(700, 920);
    dice_text.setCharacterSize(24);
    dice_text.setFillColor(sf::Color(227, 230, 193));
}
void Game::roll_dice() {
    static std::random_device rd;
    std::uniform_int_distribution<int> dist(1, 6);
    dice1 = dist(rd);
    dice2 = dist(rd);
    dice_timer = 100;
}
void Game::dice_effects(){
    auto important_points = b_final.rolled_dice(dice1 + dice2);
    for(const auto& element : important_points){
        for(const auto& building : buildings)
            if(auto b = building.lock()) {
                b->produce(element.first.first, element.first.second, element.second);
            }
    }
}

void Game::make_turn(Player &p) {
    current_player->set_turn(false);
    p.set_turn(true);
    current_player = &p;
}
void Game::calculate_win(std::vector<Player>& players) {
    std::vector<int> scores((int)players.size(), 0);
    int max_score = 0;
    for(unsigned int i = 0; i < players.size(); i++){
        scores[i] = players[i].get_score();
        max_score = std::max(max_score, scores[i]);
    }
    for(unsigned int i = 0; i < players.size(); i++){
        if(scores[i] == max_score){
            std::cout << players[i];
            players[i].set_turn(true);
        }
    }
}

void Game::check_input(int& cooldown) {
    if(cooldown > 0)
        cooldown--;
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::P) && cooldown == 0 && !done){
        paused = !paused;
        cooldown = 10;
        if(paused)
            std::cout << "Paused\n";
        else
            std::cout << "Resumed\n";
    }
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Q)){
        std::cout << "Exit\n";
        window->close();
    }
}