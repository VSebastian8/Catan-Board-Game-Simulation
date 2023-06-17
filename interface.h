class Game{
    sf::Event e;
    sf::VideoMode vm;
    sf::RenderWindow* window;
    Board b_final, b_ocean, b_tiled, b_tiled2, b_scored, b_scored2;
public:
    Game(): e(sf::Event()), vm(sf::VideoMode(1500, 1000)),
            window(new sf::RenderWindow(vm, "Catran", sf::Style::Close | sf::Style::Titlebar)),
            b_final(Board(false, 100, 400, 200)), b_ocean(Board(false, 50, 100, 50)),
            b_tiled(Board(false, 50, 575, 50)), b_tiled2(Board(false, 50, 575, 50)),
            b_scored(Board(false, 50, 1050, 50)), b_scored2(Board(false, 50, 1050, 50)){}
    Game(const Game&) = delete;
    Game& operator=(const Game&) = delete;
    ~Game(){delete window;}
    static Game& get_game(){
        static Game game;
        return game;
    }
    void show_generation(int timer = 5000);
    void run() {
        b_tiled.initialize(1);
        b_tiled2.initialize(2);
        b_scored.initialize(3);
        b_scored2.initialize(0);
        b_final.initialize();

        b_scored |= b_tiled2;
        b_scored2 |= b_scored;

        show_generation(10000);

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
            window->display();
        }
    }
};

void info_console(){
    rlutil::setColor(rlutil::YELLOW);
    std::vector<std::shared_ptr<Tile>> info_tiles = {std::make_shared<Forest>(), std::make_shared<Hill>(), std::make_shared<Pasture>(),
                                                     std::make_shared<Field>(), std::make_shared<Mountain>(), std::make_shared<Rocky_Jungle>()};
    for(const auto &i : info_tiles)
        std::cout << *i;
    rlutil::setColor(rlutil::WHITE);
}

void Game::show_generation(int timer) {
    if (window->isOpen())
    {
        window->clear();
        b_ocean.show(window);
        window->display();
        rlutil::msleep(timer/5);

        window->clear();
        b_ocean.show(window);
        b_tiled.show(window);
        window->display();
        rlutil::msleep(timer/5);

        window->clear();
        b_ocean.show(window);
        b_tiled2.show(window);
        window->display();
        rlutil::msleep(timer/5);

        window->clear();
        b_ocean.show(window);
        b_tiled2.show(window);
        b_scored.show(window);
        window->display();
        rlutil::msleep(timer/5);

        window->clear();
        b_ocean.show(window);
        b_tiled2.show(window);
        b_scored2.show(window);
        window->display();
        rlutil::msleep(timer/5);
    }
}