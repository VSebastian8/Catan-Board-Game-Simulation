void Game::simulation(const int timer, Player& p1, Player& p2, Player& p3){
    switch(timer){
        case 1:
            p1.set_turn(true);
            break;
        case 50:
            transaction(p1, "Town", {5, 5});
            break;
        case 150:
            roll_dice();
            break;
        case 450:
            p1.set_turn(false); p2.set_turn(true);
            break;
        case 500:
            transaction(p2, "City", {4, 3});
            break;
        case 650:
            roll_dice();
            break;
        case 950:
            transaction(p2, "Road", {4, 3, 4, 4});
            break;
        case 1100:
            p2.set_turn(false); p3.set_turn(true);
            break;
        case 1150:
            transaction(p3, "Town", {3, 2});
            break;
        case 1250:
            roll_dice();
            break;
        case 1450:
            transaction(p3, "Road", {2, 2, 3, 2});
            break;
        case 1600:
            p3.set_turn(false); p1.set_turn(true);
            break;
        case 1650:
            transaction(p1, "City", {5, 5});
            break;
        case 1800:
            roll_dice();
            break;
        case 2100:
            transaction(p1, "Road", {1, 3, 2, 3});
            break;
        case 2250:
            p1.set_turn(false); p2.set_turn(true);
            break;
        case 2300:
            transaction(p2, "Town", {4, 3});
            break;
        case 2500:
            roll_dice();
            break;
        case 2800:
            p2.set_turn(false); p3.set_turn(true);
            break;
        case 2900:
            transaction(p3, "City", {1, 3});
            break;
        case 3100:
            transaction(p3, "Road", {1, 3, 2, 4});
            break;
        case 3500:
            roll_dice();
            break;
        case 3700:
            transaction(p3, "Town", {2, 4});
            break;
        case 4000:
            p3.set_turn(false); p1.set_turn(true);
            break;
        case 4100:
            transaction(p1, "Town", {6, 4});
            break;
        case 4300:
            roll_dice();
            break;
        case 4600:
            transaction(p1, "Road", {5, 5, 5, 4});
            break;
        case 4700:
            p1.set_turn(false); p2.set_turn(true);
            break;
        case 4800:
            transaction(p2, "Town", {5, 2});
            break;
        case 5000:
            roll_dice();
            break;
        case 5300:
            p2.set_turn(false); p3.set_turn(true);
            break;
        case 5400:
            roll_dice();
            break;
        case 5700:
            transaction(p3, "City", {3, 2});
            break;
        case 6000:
            p3.set_turn(false);
            break;
        case 7000:
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