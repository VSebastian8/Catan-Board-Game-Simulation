void Game::init_demo() {
// instruction_type
// turn 0 roll 1
// transaction_town 2 transaction_city 3 transaction_road 4

    std::ifstream data;
    data.open("assets/demo.txt");
    std::string command, structure_type;
    int player_index, x1, y1, x2, y2;
    if(data.is_open()) {
        while(data >> command) {
            if (command == "roll")
                demo_data.push_back(std::pair<int, std::vector<int>>(0, {}));
            else if(command == "turn") {
                data >> player_index;
                demo_data.push_back(std::pair<int, std::vector<int>>(1, {player_index}));
            }
            else if(command == "transaction") {
                data >> structure_type;
                if(structure_type == "Town"){
                    data >> x1 >> y1;
                    demo_data.push_back(std::pair<int, std::vector<int>>(2, {x1, y1}));
                }
                else if(structure_type == "City"){
                    data >> x1 >> y1;
                    demo_data.push_back(std::pair<int, std::vector<int>>(3, {x1, y1}));
                }
                else if(structure_type == "Road"){
                    data >> x1 >> y1 >> x2 >> y2;
                    demo_data.push_back(std::pair<int, std::vector<int>>(4, {x1, y1, x2, y2}));
                }
            }
        }
    }
    data.close();
    instruction = demo_data.begin();
}

void Game::simulation(const int timer){
    if(demo_time == timer){
        switch((*instruction).first){
            case 0:
                roll_dice();
                demo_time += roll_speed;
                break;
            case 1:
                make_turn(players[(*instruction).second[0]]);
                demo_time += turn_speed;
                break;
            case 2:
                transaction(*current_player, "Town", (*instruction).second);
                demo_time += transaction_speed;
                break;
            case 3:
                transaction(*current_player, "City", (*instruction).second);
                demo_time += transaction_speed;
                break;
            case 4:
                transaction(*current_player, "Road", (*instruction).second);
                demo_time += transaction_speed;
                break;
        }
        ++instruction;
    }
    if(instruction == demo_data.end()){
        rlutil::resetColor();
        std::cout << "Total structures: ";
        buildings[0].lock()->show_total();
        std::cout << "Buildings:\n";
        for(const auto& b : buildings){
            if(auto b2 = b.lock())
                b2->check();
        }
        calculate_win(players);
        done = true;
    }
    rlutil::resetColor();
}