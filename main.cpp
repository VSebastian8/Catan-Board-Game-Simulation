#include <iostream>
#include <random>
#include <rlutil.h>

using namespace std;

class Wallet{
    double honey_jars;
public:
    Wallet(){
        honey_jars = 0;
    }
    ~Wallet() = default;
    friend ostream& operator <<(ostream& os, const Wallet& w) {
        return os << "Honey Jars: " << (int)(w.honey_jars) << "\n";
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
        cout << "You are so sweet!";
    }
    [[nodiscard]] bool request_purchase(const int price) const{
        if(honey_jars >= price)
            return true;
        return false;
    }
    friend class Bees;
    friend class Hive;
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
        cout << "copy constructor";
    }
    friend ostream& operator <<(ostream& os, const Bee& b) {
        return os << "This bee costed this much: " <<  b.price << "\n";
    }
    [[nodiscard]] double produce() const{
        random_device rd;   // non-deterministic generator
        mt19937 gen(rd());

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

    friend ostream& operator <<(ostream& os, const Bees& b) {
        return os << "Bees: " << b.number << "  |  (price) " << b.current_price << "\n";
    }
    void calculate_current_price(){
        current_price = (int)(current_price * increment);
    }
    void purchase(Wallet& w){
        if(w.request_purchase(current_price)){
        w.honey_jars -= current_price;

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
    void increase(Wallet& w) const{
        w.honey_jars += total_profit;
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
    friend ostream& operator <<(ostream& os, const Hive& h) {
        return os << "Hives: " << h.number << "  |  (price) " << h.price << "\n";
    }

    [[nodiscard]] bool request_purchase(const Wallet& w) const{
        if(w.honey_jars >= price)
            return true;
        return false;
    }
    void purchase(Wallet& w){
        number++;
        w.honey_jars -= price;
        price = (int)(price * increment);
    }
    void produce(Wallet& w) const{
        w.honey_jars += number * profit;
    }
};

void show_screen(const Wallet& w, const Bees& b, const Hive& h){
    rlutil::cls();
    cout << "                 H o n e y   C l i c k e r            \n";
    cout << "[click] C   |   [exit] Q   |   [bee] B   |   [hive] H \n \n";
    cout << w;
    cout << b;
    cout << h;
}

int main () {
    Wallet wall;
    Bees bees;
    Hive hive;
    rlutil::msleep(300);
    int click_cooldown = 0;

    while(true){
        int ch = rlutil::nb_getch(); //input de la tastatura
        click_cooldown++; //previne spam
        if (ch == 67 || ch == 99){ // input == C || c
            if(click_cooldown >= 2)
                wall.click();
            click_cooldown = 0;
        }
        if (ch == 66 || ch == 98){ // input == B || b
                bees.purchase(wall);
        }
        if (ch == 72 || ch == 104){ // input == H || h
            if (hive.request_purchase(wall))
                hive.purchase(wall);
        }
        bees.calculate_profit();
        bees.increase(wall);
        hive.produce(wall);
        show_screen(wall, bees, hive);
        if(wall.check_win()){
            Wallet::win();
            return 0;
        }
        if (ch == 81 || ch == 113){ // input == Q || q
            return 0;
        }
        rlutil::msleep(100);
    }
}