class Game{
    int time;
    sf::Event e;
    sf::VideoMode vm;
    sf::RenderWindow* window;
    Board b_final, b_ocean, b_tiled, b_tiled2, b_scored, b_scored2;
    std::vector<std::weak_ptr<Settlement>> buildings; //keeps track of all towns and cities
    sf::Font dice_font;
    sf::Text dice_text;
    int dice1;
    int dice2;
    int dice_timer;
    Legend legend;
public:
    Game(): time(0), e(sf::Event()), vm(sf::VideoMode(1500, 1000)),
            window(new sf::RenderWindow(vm, "Catran", sf::Style::Close | sf::Style::Titlebar)),
            b_final(Board(100, 400, 200)), b_ocean(Board(50, 100, 50)),
            b_tiled(Board(50, 575, 50)), b_tiled2(Board(50, 575, 50)),
            b_scored(Board(50, 1050, 50)), b_scored2(Board(50, 1050, 50)), legend(Legend(window)){
        dice1 = 0;
        dice2 = 0;
        dice_timer = 0;
        initialize_dice_text();
    }
    Game(const Game&) = delete;
    Game& operator=(const Game&) = delete;
    ~Game(){delete window;}
    static Game& get_game(){
        static Game game;
        return game;
    }
    void initialize_dice_text();
    void show_generation(int);
    void run();
    void transaction(Player&, const std::string&, const std::vector<float>&);
    void roll_dice();
    void dice_animation();
    void simulation(int, Player&, Player&, Player&);
};

void info_console(){
    rlutil::setColor(rlutil::YELLOW);
    std::vector<std::shared_ptr<Tile>> info_tiles = {std::make_shared<Forest>(), std::make_shared<Hill>(), std::make_shared<Pasture>(),
                                                     std::make_shared<Field>(), std::make_shared<Mountain>(), std::make_shared<Rocky_Jungle>()};
    for(const auto &i : info_tiles)
        std::cout << *i;
    std::cout << "\n";
    rlutil::resetColor();
}

void Game::run() {
    b_tiled.initialize(1);
    b_tiled2.initialize(2);
    b_scored.initialize(3);
    b_scored2.initialize(0);
    b_final.initialize();

    b_scored |= b_tiled2;
    b_scored2 |= b_scored;

   // show_generation(10000);
    b_final.animate(window);
    Player p1("Ted", {5, 5, 3, 4, 5}), p2("Robin", {5, 4, 4, 5, 3}), p3("Barney", {2, 2, 1, 3, 1});

    while(window->isOpen()) {
        if(time < 10000)
            time++;
        while(window->pollEvent(e)){
            switch(e.type)
            {
                case sf::Event::Closed : window->close(); break;
                default: break;
            }
        }
        window->clear();
        b_final.show(window);
        simulation(time, p1, p2, p3);
        legend.show_legend();
        dice_animation();
        window->draw(dice_text);

        p1.show_structures(window);
        p2.show_structures(window);
        p3.show_structures(window);
        window->draw(p1.show());
        window->draw(p2.show());
        window->draw(p3.show());
        window->display();
    }
}

void Game::transaction(Player &p, const std::string& type, const std::vector<float>& list) {
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
    if (!dice_font.loadFromFile( "georgia bold.ttf")){
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
    b_final.rolled_dice(dice1 + dice2);
}

void Game::dice_animation() {
    if(dice_timer > 0)
        dice_timer--;
    switch(dice_timer){
        case 90:
            dice_text.setString("Dice roll: [.]");
            break;
        case 85:
            dice_text.setString("Dice roll: [..]");
            break;
        case 80:
            dice_text.setString("Dice roll: [...]");
            break;
        case 60:
            dice_text.setString("Dice roll: " + std::to_string(dice1));
            break;
        case 55:
            dice_text.setString("Dice roll: " + std::to_string(dice1) + " [.]");
            break;
        case 50:
            dice_text.setString("Dice roll: " + std::to_string(dice1) + " [..]");
            break;
        case 45:
            dice_text.setString("Dice roll: " + std::to_string(dice1) + " [...]");
            break;
        case 30:
            dice_text.setString("Dice roll: " + std::to_string(dice1) + " " + std::to_string(dice2));
            break;
        case 5:
            dice_text.setString("Dice roll: " + std::to_string(dice1 + dice2));
            break;
    }
}
