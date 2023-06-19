class Game{
    int time;
    sf::Event e;
    sf::VideoMode vm;
    sf::RenderWindow* window;
    Board b_final, b_ocean, b_tiled, b_tiled2, b_scored, b_scored2;
public:
    Game(): time(0), e(sf::Event()), vm(sf::VideoMode(1500, 1000)),
            window(new sf::RenderWindow(vm, "Catran", sf::Style::Close | sf::Style::Titlebar)),
            b_final(Board(100, 400, 200)), b_ocean(Board(50, 100, 50)),
            b_tiled(Board(50, 575, 50)), b_tiled2(Board(50, 575, 50)),
            b_scored(Board(50, 1050, 50)), b_scored2(Board(50, 1050, 50)){}
    Game(const Game&) = delete;
    Game& operator=(const Game&) = delete;
    ~Game(){delete window;}
    static Game& get_game(){
        static Game game;
        return game;
    }
    void show_generation(int);
    void run();
    static void simulation(int, Player&, Player&);
};

void info_console(){
    rlutil::setColor(rlutil::YELLOW);
    std::vector<std::shared_ptr<Tile>> info_tiles = {std::make_shared<Forest>(), std::make_shared<Hill>(), std::make_shared<Pasture>(),
                                                     std::make_shared<Field>(), std::make_shared<Mountain>(), std::make_shared<Rocky_Jungle>()};
    for(const auto &i : info_tiles)
        std::cout << *i;
    std::cout << "\n";
    rlutil::setColor(rlutil::WHITE);
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
    Player p1("Sebi"), p2("Cristiana");
    std::cout << p1 << p2;

    while(window->isOpen()) {
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
        simulation(time, p1, p2);

        window->draw(p1.show());
        window->draw(p2.show());
        window->display();
    }
}

void Game::simulation(const int timer, Player& p1, Player& p2){
    if(timer == 1)
        p1.set_turn(true);
    if(timer == 50) {
        p1.set_turn(false);
        p2.set_turn(true);
    }
}

void Game::show_generation(int timer) {
    sf::Text animation_text;
    sf::Font animation_font;
    if (!animation_font.loadFromFile( "georgia bold.ttf"))    {
        rlutil::setColor(rlutil::WHITE);
        throw font_error("georgia bold");
    }
    animation_text.setFont(animation_font);
    animation_text.setString("[GENERATING TILES]");
    animation_text.setCharacterSize(24);
    animation_text.setPosition(610,  480);
    animation_text.setFillColor(sf::Color(120, 230, 223 ));

    if (window->isOpen())
    {
        window->clear();
        b_ocean.show(window);
        window->draw(animation_text);
        window->display();
        rlutil::msleep(timer/5);

        window->clear();
        b_ocean.show(window);
        b_tiled.show(window);
        animation_text.setString("[SHUFFLING TILES]");
        window->draw(animation_text);
        window->display();
        rlutil::msleep(timer/5);

        window->clear();
        b_ocean.show(window);
        b_tiled2.show(window);
        animation_text.setString("[GENERATING ROLLS]");
        window->draw(animation_text);
        window->display();
        rlutil::msleep(timer/5);

        window->clear();
        b_ocean.show(window);
        b_tiled2.show(window);
        b_scored.show(window);
        animation_text.setString("[SHUFFLING ROLLS]");
        window->draw(animation_text);
        window->display();
        rlutil::msleep(timer/5);

        window->clear();
        b_ocean.show(window);
        b_tiled2.show(window);
        b_scored2.show(window);
        animation_text.setString("[FINISHING BOARD]");
        window->draw(animation_text);
        window->display();
        rlutil::msleep(timer/5);
    }
}