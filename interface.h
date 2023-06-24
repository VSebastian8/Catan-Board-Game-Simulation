#include <fstream>
class Game{
    int time, demo_time;
    sf::Event e;
    sf::VideoMode vm;
    sf::RenderWindow* window;
    Board b_final, b_ocean, b_tiled, b_tiled2, b_scored, b_scored2;
    std::vector<std::weak_ptr<Settlement>> buildings; //keeps track of all towns and cities
    std::vector<Player> players;
    Player* current_player = {};
    sf::Font dice_font;
    sf::Text dice_text;
    int dice1;
    int dice2;
    int dice_timer;
    int turn_speed, roll_speed, transaction_speed;
    std::vector<std::pair<int, std::vector<int>>> demo_data = {};
    std::vector<std::pair<int, std::vector<int>>>::iterator instruction;
    Legend legend;
    bool done;
public:
    Game(): time(0), demo_time(0), e(sf::Event()), vm(sf::VideoMode(1500, 1000)),
            window(new sf::RenderWindow(vm, "Catran", sf::Style::Close | sf::Style::Titlebar)),
            b_final(board_builder().tile(100).offset(400, 200).build()),
            b_ocean(board_builder().tile(50).offset(100, 50).build()),
            b_tiled(board_builder().tile(50).offset(575, 50).build()),
            b_tiled2(board_builder().tile(50).offset(575, 50).build()),
            b_scored(board_builder().tile(50).offset(1050, 50).build()),
            b_scored2(board_builder().tile(50).offset(1050, 50).build()), legend(Legend(window)){
        dice1 = 0;
        dice2 = 0;
        dice_timer = 0;
        turn_speed = 50;
        roll_speed = 400;
        transaction_speed = 200;
        done = false;
        initialize_dice_text();
    }
    Game(const Game&) = delete;
    Game& operator=(const Game&) = delete;
    ~Game(){delete window;}
    static Game& get_game(){
        static Game game;
        return game;
    }
    void init_demo();
    void simulation(int);
    void initialize_dice_text();
    void show_generation(int);
    void run();
    void transaction(Player&, const std::string&, const std::vector<int>&);
    void roll_dice();
    void dice_animation();
    static void calculate_win(const std::vector<Player>&);
    void make_turn(Player& p);
};

void Game::run() {
    b_tiled.initialize(1);
    b_tiled2.initialize(2);
    b_scored.initialize(3);
    b_scored2.initialize(0);
    b_final.initialize();

    b_scored |= b_tiled2;
    b_scored2 |= b_scored;

    show_generation(10000);
    b_final.animate(window);
    players = {Player("Ted", {3, 3, 3, 3, 3}), Player("Robin", {3, 3, 3, 3, 3}), Player("Barney", {3, 3, 3, 3, 3})};
    current_player = &players[0];

    init_demo();
    while(window->isOpen()) {
        while(window->pollEvent(e)){
            switch(e.type)
            {
                case sf::Event::Closed : window->close(); break;
                default: break;
            }
        }
        window->clear();
        b_final.show(window);
        if(!done)
            simulation(time);
        else
            legend.show_exit();
        legend.show_legend();
        dice_animation();
        window->draw(dice_text);

        for(auto p : players) {
            p.show_structures(window);
            window->draw(p.show());
        }
        window->display();
        if(time < 50000)
            time++;
    }
}

void Game::transaction(Player &p, const std::string& type, const std::vector<int>& list) {
    rlutil::setColor(rlutil::LIGHTRED);
    try{
        std::shared_ptr<Structure> s;
        if(type == "Town"){
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
        std::cout << err.what() << "\n";
    }
    catch(wrong_road_error& err){
        std::cout << err.what() << "\n";
    }
    catch(city_error& err){
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
    auto important_points = b_final.rolled_dice(dice1 + dice2);
    for(const auto& element : important_points){
        for(const auto& building : buildings)
            if(auto b = building.lock()) {
                b->produce(element.first.first, element.first.second, element.second);
            }
    }
}

void Game::calculate_win(const std::vector<Player>& players) {
    std::vector<int> scores((int)players.size(), 0);
    int max_score = 0;
    for(unsigned int i = 0; i < players.size(); i++){
        scores[i] = players[i].get_score();
        max_score = std::max(max_score, scores[i]);
    }
    for(unsigned int i = 0; i < players.size(); i++){
        if(scores[i] == max_score){
            std::cout << players[i];
        }
    }
}

void Game::make_turn(Player &p) {
    current_player->set_turn(false);
    p.set_turn(true);
    current_player = &p;
}


