#include <iostream>
#include <random>
#include <rlutil.h>

class Wallet{
    double honey_jars;
public:
    Wallet(){
        honey_jars = 0;
    }
    ~Wallet() = default;
    Wallet(const Wallet& other) = default;

    friend std::ostream& operator <<(std::ostream& os, const Wallet& w) {
        rlutil::setColor(rlutil::YELLOW);
        os << "Honey Jars: ";
        rlutil::setColor(rlutil::LIGHTCYAN);
        os << (int)(w.honey_jars) << "\n";
        return os;
    }
    void click(){
        honey_jars++;
    }
    [[nodiscard]] bool check_win() const{
        return honey_jars >= 1000;
    }
    static void win(){
        rlutil::setColor(rlutil::LIGHTRED);
        std::cout << "You are so sweet!\n";
    }
    [[nodiscard]] bool request_purchase(const int price) const{
        return honey_jars >= price;
    }
    void increase(const double profit){
        honey_jars += profit;
    }
    void decrease(const int price){
        honey_jars -= price;
    }
};

class Bee{
    int price;
    double rate;
public:
    Bee(){ // constructor fara parametrii folosit la initializarea array-ului cu o albina lenesa
       price = 0;
       rate = 0;
    }
    explicit Bee(int p){
       rate = 0.1;  // per 100ms
       price = p;
    }
    ~Bee() = default;
    Bee(const Bee& other) = default;

    friend std::ostream& operator <<(std::ostream& os, const Bee& b) {
        return os << "This bee costed this much: " <<  b.price << "\n";
    }
    Bee& operator=(const Bee& other) = default;
    [[nodiscard]] double produce() const{
        static std::random_device rd;   // non-deterministic generator
        static std::mt19937 gen(rd());
        if(gen() % 100 < 2)
            return rate * 2; // fiecare albina are probabilitatea de 2% de a dubla productia
        return rate;
    }
};
class Hive{
    int number, current_price;
    double increment, total_profit;
    Bee* bees_array; // compunere
public:
    Hive(){
        number = 0;
        increment = 1.5;
        total_profit = 0;
        current_price = 10;
        bees_array = new Bee[1];
        bees_array[0] = Bee();
        std::cout << "constructor\n";
    }
    ~Hive() {
        delete[] bees_array;
        rlutil::setColor(rlutil::LIGHTRED);
        std::cout << "Exit\n";
    }
    Hive(const Hive &other): number{other.number}, current_price{other.current_price}, increment{other.increment}{
        total_profit = 0;
        bees_array = new Bee[number];
        for(int i = 0; i < number; i++) {
            bees_array[i] = other.bees_array[i];
        }
        std::cout << "copy constructor\n";
    }

    friend std::ostream& operator <<(std::ostream& os, const Hive& b) {
        rlutil::setColor(rlutil::YELLOW);
        os << "Bees: ";
        rlutil::setColor(rlutil::LIGHTCYAN);
        os << b.number;
        rlutil::setColor(rlutil::YELLOW);
        os << "  |  (price) ";
        rlutil::setColor(rlutil::LIGHTCYAN);
        os << b.current_price << "\n";
        return os;
    }
    Hive& operator=(const Hive& other){
        if(this==&other)
            return *this;

        delete[] this->bees_array;
        this->bees_array=new Bee[this->number=other.number];
        for(int i=0;i<this->number;++i)
            this->bees_array[i]=other.bees_array[i];

        this->number = other.number;
        this->current_price = other.current_price;
        this->increment = other.increment;
        this->total_profit = 0;
        return *this;
    }
    void calculate_current_price(){
        current_price = (int)(current_price * increment);
    }
    void purchase(Wallet& w){
        if(w.request_purchase(current_price)){
            w.decrease(current_price);
            Bee* temp = new Bee[number + 1];
            for(int i = 0; i < number; i++) {
                temp[i] = bees_array[i];
            }
            temp[number] = Bee(current_price);
            number++;
            delete[] bees_array;
            bees_array = temp;
            calculate_current_price();
        }
    }
    void calculate_profit(){
        total_profit = 0;
        for(int i = 0; i <= number; i++){
            total_profit += bees_array[i].produce();
        }
    }
    void add_to_wallet(Wallet& w) const{
        w.increase(total_profit);
    }
    [[nodiscard]] bool inactivity_check(const int counter) const{
        if(number == 0) // daca nu avem albine
            if(counter >= 600) { // daca a trecut 1 minut de la ultimul click
                rlutil::setColor(rlutil::LIGHTRED);
                std::cout << "Kicked for inactivity :( \n";
                return true;
            }
        return false;
    }
};

class FancyBees{
    int number, price;
    double increment, profit;
public:
    FancyBees(){
        number = 0;
        price = 100;
        increment = 1.2;
        profit = 0.5;
    }
    ~FancyBees() = default;
    FancyBees(const FancyBees& other) = default;

    friend std::ostream& operator <<(std::ostream& os, const FancyBees& h) {
        rlutil::setColor(rlutil::YELLOW);
        os << "FancyBees: ";
        rlutil::setColor(rlutil::LIGHTCYAN);
        os << h.number;
        rlutil::setColor(rlutil::YELLOW);
        os << "  |  (price) ";
        rlutil::setColor(rlutil::LIGHTCYAN);
        os << h.price << "\n";
        rlutil::setColor(rlutil::YELLOW);
        return os;
    }
    void purchase(Wallet& w) {
        if (w.request_purchase(price)) {
            number++;
            w.decrease(price);
            price = (int) (price * increment);
        }
    }
    void produce(Wallet& w) const{
        w.increase(number * profit);
    }
};

void show_screen(const Wallet& w, const Hive& b, const FancyBees& h){
    rlutil::cls();
    rlutil::setColor(rlutil::YELLOW);
    std::cout << "                 H o n e y   C l i c k e r            \n";
    rlutil::setColor(rlutil::BROWN);
    std::cout << "[click] C   |   [exit] Q   |   [bee] B   |   [fancy bee] F \n \n";
    rlutil::setColor(rlutil::YELLOW);
    std::cout << w;
    std::cout << b;
    std::cout << h;
}

int main () {
    Wallet wall;
    Hive hive;
    FancyBees fancy;
    rlutil::msleep(300);
    int click_cooldown = 0;

    while(true){
        char ch = std::tolower(rlutil::nb_getch()); // input de la tastatura
        click_cooldown++; // previne spam si inactivitate
        if (ch == 'c'){
            if(click_cooldown >= 2)
                wall.click();
            click_cooldown = 0;
        }
        if (ch == 'b'){
                hive.purchase(wall);
        }
        if (ch == 'f'){
                fancy.purchase(wall);
        }

        hive.calculate_profit();
        hive.add_to_wallet(wall);
        fancy.produce(wall);
        show_screen(wall, hive, fancy);

        if(wall.check_win()){
            Wallet::win();
            return 0;
        }
        if (ch == 'q'){
            return 0;
        }
        if(hive.inactivity_check(click_cooldown))
            return 0;
        rlutil::msleep(100);
    }
}