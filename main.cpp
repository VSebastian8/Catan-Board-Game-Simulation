#include <bits/stdc++.h>
#include <windows.h>

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
    friend class Bee;
    friend class Hive;
};
//Future Compunere: class Bees care are un array de obiecte Bee
class Bee{
    int number, price;
    double increment, profit;
public:
    Bee(){
       number = 0;
       price = 10;
       increment = 1.5;
       profit = 0.1;
    }
    ~Bee() = default;
    Bee(const Bee& other) : number{other.number}, price{other.price}, increment{other.increment}, profit{other.profit}
    {
        cout << "copy constructor";
    }
    friend ostream& operator <<(ostream& os, const Bee& b) {
        return os << "Bees: " << b.number << "  |  (price) " << b.price << "\n";
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

void show_screen(const Wallet& w, const Bee& b, const Hive& h){
    system("cls"); //clears screen
    cout << "                          H o n e y   C l i c k e r            \n";
    cout << "[click] Left Click   |   [exit] Q   |   [bee] B   |   [hive] H \n \n";
    cout << w;
    cout << b;
    cout << h;
}

int main () {
    Wallet wall;
    Bee bee;
    Hive hive;
    Sleep(100);

    while(true){
        if (GetAsyncKeyState(VK_LBUTTON)){
            wall.click(); //click detection
        }
        if (GetAsyncKeyState(0x42)) {
            if (bee.request_purchase(wall))
                bee.purchase(wall);
        }
        if (GetAsyncKeyState(0x48)) {
            if (hive.request_purchase(wall))
                hive.purchase(wall);
        }
        bee.produce(wall);
        hive.produce(wall);
        show_screen(wall, bee, hive);
        if(wall.check_win()){
            Wallet::win();
            return 0;
        }
        if (GetAsyncKeyState(0x51)){
            return 0; //q for exit
        }
        Sleep(100);
    }
}