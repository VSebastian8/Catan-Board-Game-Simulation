#pragma once

void Game::show_generation(bool& animate_generation) {
    window->clear();
    if (generation_timer < 60) {
        b_ocean.show(window);
        animation_text.setString("[GENERATING TILES]");
    } else if (generation_timer < 120) {
        b_ocean.show(window);
        b_tiled.show(window);
        animation_text.setString("[SHUFFLING TILES]");
    } else if (generation_timer < 180) {
        b_ocean.show(window);
        b_tiled2.show(window);
        animation_text.setString("[GENERATING ROLLS]");
    } else if (generation_timer < 240) {
        b_ocean.show(window);
        b_tiled2.show(window);
        b_scored.show(window);
        animation_text.setString("[SHUFFLING ROLLS]");
    } else if (generation_timer < 300) {
        b_ocean.show(window);
        b_tiled2.show(window);
        b_scored2.show(window);
        animation_text.setString("[FINISHING BOARD]");
    } else
        animate_generation = false;
    window->draw(animation_text);
    generation_timer++;
}

void Board::animate(sf::RenderWindow *w, bool& animate_board) {
    int speed = 2;
    w->clear();
    for (int k = 0; k < anim_j; k++) {
        for (int l = 0; l < dimension; l++) {
            w->draw(tiles[l][k]->show());
            w->draw(tiles[l][k]->show_disk());
            w->draw(tiles[l][k]->show_dice_value());
        }
    }
    for (int l = 0; l <= anim_i; l++) {
        w->draw(tiles[l][anim_j]->show());
        w->draw(tiles[l][anim_j]->show_disk());
        w->draw(tiles[l][anim_j]->show_dice_value());
    }
    board_timer++;
    if(board_timer == speed) {
        anim_i++;
        if (anim_i == dimension) {
            anim_i = 0;
            anim_j++;
            if (anim_j == dimension)
                animate_board = false;
        }
        board_timer = 0;
    }
}

void Game::dice_animation() {
    if(dice_timer > 0)
        dice_timer--;
    switch(dice_timer){
        case 60:
            dice_text.setString("Dice roll: [.]");
            break;
        case 55:
            dice_text.setString("Dice roll: [..]");
            break;
        case 50:
            dice_text.setString("Dice roll: [...]");
            break;
        case 45:
            dice_text.setString("Dice roll: " + std::to_string(dice1));
            break;
        case 35:
            dice_text.setString("Dice roll: " + std::to_string(dice1) + " [.]");
            break;
        case 30:
            dice_text.setString("Dice roll: " + std::to_string(dice1) + " [..]");
            break;
        case 25:
            dice_text.setString("Dice roll: " + std::to_string(dice1) + " [...]");
            break;
        case 20:
            dice_text.setString("Dice roll: " + std::to_string(dice1) + " " + std::to_string(dice2));
            break;
        case 5:
            dice_text.setString("Dice roll: " + std::to_string(dice1 + dice2));
            break;
    }
}