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
        if(honey_jars >= 1000)
            return true;
        return false;
    }
    static void win(){
        rlutil::setColor(rlutil::LIGHTRED);
        std::cout << "You are so sweet!";
    }
    [[nodiscard]] bool request_purchase(const int price) const{
        if(honey_jars >= price)
            return true;
        return false;
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
    Bee(){ //constructor fara parametrii folosit la initializarea array-ului cu o albina lenesa
       price = 0;
       rate = 0;
    }
    explicit Bee(int p){
       rate = 0.1;  // once every 100ms - all bees have the same rate but this could change
       price = p;
    }
    ~Bee() = default;
    Bee(const Bee& other) : price{other.price}, rate{other.rate}
    {
        std::cout << "copy constructor";
    }
    friend std::ostream& operator <<(std::ostream& os, const Bee& b) {
        return os << "This bee costed this much: " <<  b.price << "\n";
    }
    Bee& operator=(const Bee& other) = default;
    [[nodiscard]] double produce() const{
        std::random_device rd;   // non-deterministic generator
        std::mt19937 gen(rd());

        if(gen() % 100 < 2)
            return rate * 2; //fiecare albina are probabilitatea de 2% de a dubla productia
        return rate;
    }
};
class Bees{
    int number, current_price;
    double increment, total_profit;
    Bee* ob;
public:
    Bees(){
        number = 0;
        increment = 1.5;
        total_profit = 0;
        current_price = 10;
        ob = new Bee[1];
        ob[0] = Bee();
    }
    ~Bees() {
        delete[] ob;
    }
    Bees(const Bees &other): number{other.number}, current_price{other.current_price}, increment{other.increment}{
        total_profit = 0;
        ob = new Bee[number];
        for(int i = 0; i < number; i++) {
            ob[i] = other.ob[i];
        }
    }
    Bees& operator=(const Bees& other){
        if(this==&other)
            return *this;

        delete[] this->ob;
        this->ob=new Bee[this->number=other.number];
        for(int i=0;i<this->number;++i)
            this->ob[i]=other.ob[i];

        this->number = other.number;
        this->current_price = other.current_price;
        this->increment = other.increment;
        this->total_profit = 0;

        return *this;
    }
    friend std::ostream& operator <<(std::ostream& os, const Bees& b) {
        rlutil::setColor(rlutil::YELLOW);
        os << "Bees: ";
        rlutil::setColor(rlutil::LIGHTCYAN);
        os << b.number;
        rlutil::setColor(rlutil::YELLOW);
        os << "   |  (price) ";
        rlutil::setColor(rlutil::LIGHTCYAN);
        os << b.current_price << "\n";
        return os;
    }
    void calculate_current_price(){
        current_price = (int)(current_price * increment);
    }
    void purchase(Wallet& w){
        if(w.request_purchase(current_price)){
            w.decrease(current_price);

            Bee* temp = new Bee[number + 1];
            for(int i = 0; i < number; i++) {
                temp[i] = ob[i];
            }
            temp[number] = Bee(current_price);
            number++;
            delete[] ob;
            ob = temp;
            calculate_current_price();
        }
    }
    void calculate_profit(){
        total_profit = 0;
        for(int i = 0; i <= number; i++){
            total_profit += ob[i].produce();
        }
    }
    void add_to_wallet(Wallet& w) const{
        w.increase(total_profit);
    }
    [[nodiscard]] bool inactivity_check(const int counter) const{
        if(number == 0) // daca nu avem albine
            if(counter >= 600) { // daca a trecut 1 minut de la ultimul click
                rlutil::setColor(rlutil::LIGHTRED);
                std::cout << "Kicked for inactivity :(";
                return true;
            }
        return false;
    }
};

class Hive{
    int number, price;
    double increment, profit;
public:
    Hive(){
        number = 0;
        price = 100;
        increment = 1.2;
        profit = 0.5;
    }
    ~Hive() = default;
    friend std::ostream& operator <<(std::ostream& os, const Hive& h) {
        rlutil::setColor(rlutil::YELLOW);
        os << "Hives: ";
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

void show_screen(const Wallet& w, const Bees& b, const Hive& h){
    rlutil::cls();
    rlutil::setColor(rlutil::YELLOW);
    std::cout << "                 H o n e y   C l i c k e r            \n";
    rlutil::setColor(rlutil::BROWN);
    std::cout << "[click] C   |   [exit] Q   |   [bee] B   |   [hive] H \n \n";
    rlutil::setColor(rlutil::YELLOW);
    std::cout << w;
    std::cout << b;
    std::cout << h;
}

int main () {
    Wallet wall;
    Bees bees;
    Hive hive;
    rlutil::msleep(300);
    int click_cooldown = 0;

    while(true){
        int ch = rlutil::nb_getch(); //input de la tastatura
        click_cooldown++; //previne spam si inactivitate
        if (ch == 67 || ch == 99){ // input == C || c
            if(click_cooldown >= 2)
                wall.click();
            click_cooldown = 0;
        }
        if (ch == 66 || ch == 98){ // input == B || b
                bees.purchase(wall);
        }
        if (ch == 72 || ch == 104){ // input == H || h
                hive.purchase(wall);
        }
        bees.calculate_profit();
        bees.add_to_wallet(wall);
        hive.produce(wall);
        show_screen(wall, bees, hive);
        if(wall.check_win()){
            Wallet::win();
            return 0;
        }
        if (ch == 81 || ch == 113){ // input == Q || q
            return 0;
        }
        if(bees.inactivity_check(click_cooldown))
            return 0;
        rlutil::msleep(100);
    }
}