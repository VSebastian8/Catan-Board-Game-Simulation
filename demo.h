void Game::simulation(const int timer, Player& p1, Player& p2, Player& p3){
    switch(timer){
        case 1:
            p1.set_turn(true);
            break;
        case 50:
            transaction(p1, "Town", {5, 5});
            break;
        case 150:
            p1.set_turn(false); p2.set_turn(true);
            break;
        case 200:
            roll_dice();
            break;
        case 400:
            transaction(p2, "City", {4, 3});
            break;
        case 550:
            roll_dice();
            break;
        case 750:
            transaction(p2, "Road", {4, 3, 4, 4});
            break;
        case 900:
            roll_dice();
            break;
        case 1100:
            p2.set_turn(false); p3.set_turn(true);
            break;
        case 1150:
            transaction(p3, "Road", {1, 2, 1, 3});
            break;
        case 1300:
            p3.set_turn(false); p1.set_turn(true);
            break;
        case 1350:
            transaction(p1, "City", {5, 5});
            break;
        case 1500:
            roll_dice();
            break;
        case 1700:
            transaction(p1, "Road", {1, 3, 2, 3});
            break;
        case 1850:
            p1.set_turn(false); p2.set_turn(true);
            break;
        case 1900:
            transaction(p2, "Town", {4, 3});
            break;
        case 2100:
            roll_dice();
            break;
        case 2300:
            p2.set_turn(false); p3.set_turn(true);
            break;
        case 2400:
            transaction(p3, "City", {1, 3});
            break;
        case 2600:
            transaction(p3, "Road", {1, 3, 2, 4});
            break;
        case 2800:
            roll_dice();
            break;
        case 3000:
            transaction(p3, "Town", {2, 4});
            break;
        case 4000:
            rlutil::resetColor();
            std::cout << "Total structures: ";
            buildings[0].lock()->show_total();
            std::cout << "Buildings:\n";
            for(const auto& b : buildings){
                if(auto b2 = b.lock())
                    b2->check();
            }
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