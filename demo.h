void Game::simulation(const int timer, Player& p1, Player& p2, Player& p3){
    rlutil::setColor(rlutil::LIGHTRED);
    switch(timer){
        case 1: p1.set_turn(true);
            break;
        case 50:
            try{
                std::shared_ptr<Structure> s = std::make_shared<Town>(5, 5);
                s->purchase(p1);
                p1.add_structure(s);
            }
            catch(resource_error& err){ std::cout << err.what() << "\n"; }
            break;
        case 150: p1.set_turn(false); p2.set_turn(true);
            break;
        case 200:
            try{
                std::shared_ptr<Structure> s = std::make_shared<City>(4, 3);
                s->purchase(p2);
                p2.add_structure(s);
            }
            catch(resource_error& err){ std::cout << err.what() << "\n"; }
            catch(city_error& err){ std::cout << err.what() << "\n"; }
            break;
        case 300:
            try{
                std::shared_ptr<Structure> s = std::make_shared<Road>(4, 3, 4, 4);
                s->purchase(p2);
                p2.add_structure(s);
            }
            catch(resource_error& err){ std::cout << err.what() << "\n"; }
            catch(wrong_road_error& err){ std::cout << err.what() << "\n"; }
            break;
        case 450: p2.set_turn(false); p3.set_turn(true);
            break;
        case 500:
            try{
                std::shared_ptr<Structure> s = std::make_shared<Road>(1, 2, 1, 3);
                s->purchase(p3);
                p3.add_structure(s);
            }
            catch(resource_error& err){ std::cout << err.what() << "\n"; }
            catch(wrong_road_error& err){ std::cout << err.what() << "\n"; }
            break;
        case 550: p3.set_turn(false); p1.set_turn(true);
            break;
        case 600:
            try{
                std::shared_ptr<Structure> s = std::make_shared<City>(5, 5);
                s->purchase(p1);
                p1.add_structure(s);
            }
            catch(resource_error& err){ std::cout << err.what() << "\n"; }
            catch(city_error& err){ std::cout << err.what() << "\n"; }
            break;
        case 750:
            try{
                std::shared_ptr<Structure> s = std::make_shared<Road>(1, 3, 2, 3);
                s->purchase(p1);
                p1.add_structure(s);
            }
            catch(resource_error& err){
                std::cout << err.what() << "\n";
            }
            catch(wrong_road_error& err){
                std::cout << err.what() << "\n";
            }
            break;
        case 800: p1.set_turn(false); p2.set_turn(true);
            break;
        case 900:
            try{
                std::shared_ptr<Structure> s = std::make_shared<Town>(4, 3);
                s->purchase(p2);
                p2.add_structure(s);
            }
            catch(resource_error& err){ std::cout << err.what() << "\n"; }
            break;
        case 1000: p2.set_turn(false); p3.set_turn(true);
            break;
        case 1050:
            try{
                std::shared_ptr<Structure> s = std::make_shared<City>(1, 3);
                s->purchase(p3);
                p3.add_structure(s);
            }
            catch(resource_error& err){ std::cout << err.what() << "\n"; }
            catch(city_error& err){ std::cout << err.what() << "\n"; }
            break;
        case 1200:
            try{
                std::shared_ptr<Structure> s = std::make_shared<Road>(1, 3, 2, 4);
                s->purchase(p3);
                p3.add_structure(s);
            }
            catch(resource_error& err){ std::cout << err.what() << "\n"; }
            catch(wrong_road_error& err){ std::cout << err.what() << "\n"; }
            break;
        case 1300:
            try{
                std::shared_ptr<Structure> s = std::make_shared<Town>(2, 4);
                s->purchase(p3);
                p3.add_structure(s);
                rlutil::resetColor();
                std::cout << "Total structures: ";
                s->show_total();
            }
            catch(resource_error& err){ std::cout << err.what() << "\n"; }
            break;
        default: break;
    }
    rlutil::resetColor();
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