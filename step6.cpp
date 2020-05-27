#include <string>
#include <sstream>
#include <fstream>
#include <map>
#include <iostream>

using namespace std;

namespace Helpers {

    void putKV(string pair, map<string, string>& m, char delim) {
        string key;
        string value;
        stringstream ss(pair);
        getline(ss, key, delim);
        getline(ss, value, delim);
        m[key] = value;
    }

    bool getValue(map<string, string>& m, string key, bool defvalue) {
        try {
            string value = m.at(key);
            if (value == "true" || value == "1") return true;
            else if (value == "false" || value == "0") return false;
            else return defvalue;
        }
        catch (const std::exception&) {
            return defvalue;
        }
    }

    string getValue(map<string, string>& m, string key, const char* defvalue) {
        try {
            string value = m.at(key);
            return (value.empty() ? defvalue : value);
        }
        catch (const std::exception&) {
            return defvalue;
        }
    }

    int getValue(map<string, string>& m, string key, int defvalue) {
        try {
            return stoi(m[key]);
        }
        catch (const std::exception& e) {
            return defvalue;
        }
    }

}


class AbstractCoffeeMachine {
protected:
    string colour;
    int price;
    bool canMakeCappuccino;
    string model;
    string typeOfCoffee;
    int numberOfCups;
public:
    AbstractCoffeeMachine(int price = 4500, int weight = 3, int power = 230, string category = "coffeemachines",
        string colour = "white", bool canMakeCappuccino = true, string model = "Vitek VT-1525",
        string typeOfCoffee = "grinded", int numberOfCups = 1) :
        price(price), colour(colour), canMakeCappuccino(canMakeCappuccino),
        model(model), typeOfCoffee(typeOfCoffee), numberOfCups(numberOfCups) {}

    AbstractCoffeeMachine(const AbstractCoffeeMachine& c) :
        price(c.price), colour(c.colour), canMakeCappuccino(c.canMakeCappuccino), model(c.model),
        typeOfCoffee(c.typeOfCoffee), numberOfCups(c.numberOfCups) {}


    virtual void makeEspresso() {
        cout << "This method should be overriden for children";
    }
    virtual ~AbstractCoffeeMachine() {
        cout << "Calling virtual destructor of abstract coffeee machine" << endl;
    }
};


class CoffeeMachine : public AbstractCoffeeMachine {
    int pressure; // pressure (bars)
    int temperature; // temperature (degrees of Celsius)
    bool hasGrinder; // a machine can have integrated grinder or not

    // Warming the machine
    void warm(int temp) {
        while (temperature < temp) {
            temperature++;
            cout << "Warming. Current temperature is " << temperature << endl;
        }
    }

public:

    CoffeeMachine(int price = 4500, int weight = 3, int power = 230, string category = "Laptops",
        string colour = "white", bool canMakeCappuccino = true, string model = "Vitek VT-1525",
        string typeOfCoffee = "grinded", int numberOfCups = 1, int temperature = 10, int pressure = 15, bool hasGrinder = false) :
        AbstractCoffeeMachine(price, weight, power, category, colour, canMakeCappuccino, model, typeOfCoffee, numberOfCups),
        temperature(temperature), pressure(pressure), hasGrinder(hasGrinder) {}

    CoffeeMachine(map<string, string>& m) {
        price = Helpers::getValue(m, "price", 2500);
        pressure = Helpers::getValue(m, "pressure", 15);
        temperature = Helpers::getValue(m, "temperature", 30);
        colour = Helpers::getValue(m, "colour", "black");
        model = Helpers::getValue(m, "model", "Vitek VT-1525");
        numberOfCups = Helpers::getValue(m, "numberOfCups", 2);
        canMakeCappuccino = Helpers::getValue(m, "canMakeCappuccino", true);
        typeOfCoffee = Helpers::getValue(m, "typeOfCoffee", "grinded");
        hasGrinder = Helpers::getValue(m, "hasGrinder", false);

    }


    CoffeeMachine(const CoffeeMachine& c) : AbstractCoffeeMachine(c),
        pressure(c.pressure), temperature(c.temperature), hasGrinder(c.hasGrinder) {}
    ~CoffeeMachine() {}
    // Make a cup of coffee
    void makeEspresso(bool hot = false) {
        cout << "Process of making coffee started" << endl;
        warm(hot ? 120 : 100);
        // It's just a notification to show how to use fields of class
        if (!canMakeCappuccino) cout << "Attention. This instance can't make cappuccino" << endl;
        cout << "Activating pump" << endl;
        cout << "Deactivating pump" << endl;
        cout << "Your drink is ready" << endl;
    }
    // Definition of a friendly operator function.
    friend ostream& operator<<(ostream& output, const CoffeeMachine& machine);



    static CoffeeMachine from_file(char delim = ':', string fname = "coffee.txt") {
        map<string, string> m;
        int j = 0;
        fstream file;
        file.open(fname);
        if (file.is_open()) {
            string line;
            while (getline(file, line) && j < 11) {
                Helpers::putKV(line, m, delim);
                j++;
            }
        }
        return CoffeeMachine(m);
    }
};

// Friendly operator function
ostream& operator<<(ostream& output, const CoffeeMachine& machine) {
    output << "Pressure: " << machine.pressure << endl;
    output << "Temperature: " << machine.temperature << endl;
    output << "Colour: " << machine.colour << endl;
    output << "Number of cups: " << machine.numberOfCups << endl;
    output << "Price: " << machine.price << endl;
    output << "Model: " << machine.model << endl;
    output << "Type of coffee: " << machine.typeOfCoffee << endl;
    output << (machine.canMakeCappuccino ? "This instance can make cappuccino" : "This instance can't make cappuccino") << endl;
    output << (machine.hasGrinder ? "This instance has integrated grinder" : "This instance has no integrated grinder") << endl;
    return output;
}

int main() {
    // Init from file
    CoffeeMachine c = CoffeeMachine::from_file();
    CoffeeMachine c2 = c;
    // Default initialization
    CoffeeMachine c3;
    cout << c << endl;
    cout << c2 << endl;
    cout << c3 << endl;
    // Calling method of the class
    c.makeEspresso();
}