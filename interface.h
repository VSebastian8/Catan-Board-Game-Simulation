class Game{
    int time;
    sf::Event e;
    sf::VideoMode vm;
    sf::RenderWindow* window;
    Board b_final, b_ocean, b_tiled, b_tiled2, b_scored, b_scored2;
    Legend legend;
public:
    Game(): time(0), e(sf::Event()), vm(sf::VideoMode(1500, 1000)),
            window(new sf::RenderWindow(vm, "Catran", sf::Style::Close | sf::Style::Titlebar)),
            b_final(Board(100, 400, 200)), b_ocean(Board(50, 100, 50)),
            b_tiled(Board(50, 575, 50)), b_tiled2(Board(50, 575, 50)),
            b_scored(Board(50, 1050, 50)), b_scored2(Board(50, 1050, 50)), legend(Legend(window)){}
    Game(const Game&) = delete;
    Game& operator=(const Game&) = delete;
    ~Game(){delete window;}
    static Game& get_game(){
        static Game game;
        return game;
    }
    void show_generation(int);
    void run();
    static void simulation(int, Player&, Player&, Player&);
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

    show_generation(10000);
    b_final.animate(window);
    Player p1("Ted", {4, 2, 3, 4, 5}), p2("Robin", {0, 2, 2, 5, 3}), p3("Barney", {1, 1, 1, 1, 1});

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
        window->draw(p1.show());
        window->draw(p2.show());
        window->draw(p3.show());
        window->display();
    }
}