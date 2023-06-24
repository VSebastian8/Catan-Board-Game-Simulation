#pragma once

void Game::show_generation(int timer) {
    sf::Text animation_text;
    sf::Font animation_font;
    if (!animation_font.loadFromFile( "assets/georgia_bold.ttf"))    {
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

void Board::animate(sf::RenderWindow *w) {
    for(int j = 0; j < dimension; j++)
        for(int i = 0; i < dimension; i++){
            w->clear();
            for(int k = 0; k < j; k++){
                for(int l = 0; l < dimension; l++){
                    w->draw(tiles[l][k]->show());
                    w->draw(tiles[l][k]->show_disk());
                    w->draw(tiles[l][k]->show_dice_value());
                }
            }
            for(int l = 0; l <= i; l++){
                w->draw(tiles[l][j]->show());
                w->draw(tiles[l][j]->show_disk());
                w->draw(tiles[l][j]->show_dice_value());
            }
            w->display();
            rlutil::msleep(60);
        }
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