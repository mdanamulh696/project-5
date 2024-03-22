/***********************************************************************
Matr.-Nr:                     3085596
Nachname/Surname:             Haque
Vorname/Given name:           Md Anamul
Uni-Email:                    md.haque.696@stud.uni-due.de
Studiengang/Course of studis: ISE(Software Engineering)
***********************************************************************/
#include<string>
#include<iomanip>
#include<fstream>
#include<sstream>
#include<iostream>

const int annualaverageconsumptionperperson = 200;
const int annualaverageconsumptionpersquaremeter = 9;
const int annual_average_consumption_per_person_with_electric_water_heating = 550;
const int columnwidth = 35;
using namespace std;

enum  Use
{
    once,
    daily,
    mo_fr,
    sa_su,
    weekly
};

class Consumer
{
 private:
    string description;
    float watt;
    float watt_standby;
    float hours;
    Use use;
    Consumer* next;

    static int totalConsumers;

 public:
    Consumer() 
    { 
        totalConsumers++;
        next = nullptr; 
    }
    // getter method
    string get_description() { return description; }
    float get_watt() { return watt; }
    float get_watt_standby() { return watt_standby; }
    float get_hours() { return hours; }
    Use get_use() { return use; }
    Consumer* get_next() { return next; }
    static int getTotalConsumers() { return totalConsumers; }
    // setter method
    void set_description(string description) { this->description = description; }
    void set_watt(float watt) { this->watt = watt; }
    void set_watt_standby(float watt_standby) { this->watt_standby = watt_standby; }
    void set_hours(float hours) { this->hours = hours; }
    void set_use(Use use) { this->use = use; }
    void set_next(Consumer* next) { this->next = next; }
    // Public member functions
    float annual_hours_of_use();
    float annual_hours_of_standby();
    float annual_kwh();
    void  input();
    void  print(int index);
    void  input_use(string c);

    ~Consumer() 
    {
        cout << "Consumer " << description << " at address " << this << " deleted\n";
        totalConsumers--;
    }
};

int Consumer::totalConsumers = 0;

void Consumer::input()
{
    cout << "what is the description of the power consumer? ";
    cin.ignore();
    getline(cin, description);
    cout << "how many watt it will have? ";
    cin >> watt;
    cout << "how many watt standby it will have? ";
    cin >> watt_standby;

    this->input_use("how often is it used?\n(d) daily\n(m) mo_fr\n(o) once\n(s) sa_su\n(w) weekly? ");
    cout << "how many hours will it be operating? ";
    cin >> hours;

    this->set_description(description);
    this->set_watt(watt);
    this->set_watt_standby(watt_standby);
    this->set_hours(hours);
}

void Consumer::input_use(string c)
{
    cout << "how often it will be used?";
    cout << "(d) daily";
    cout << "(m) mo_fr";
    cout << "(o) once";
    cout << "(s) sa_su";
    cout << "(w) weekly";

    cout << c;
    char choice;
    cin >> choice;

    switch (choice) {
    case 'd': use = daily;  break;
    case 'm': use = mo_fr;  break;
    case 'o': use = once;   break;
    case 's': use = sa_su;  break;
    case 'w': use = weekly; break;
    default:
        cout << "invalid choice. using 'once' as default." << endl;
        use = once;
        break;
    }
    this->use = use;
}

float Consumer::annual_hours_of_use()
{
    switch (this->use)
    {
    case once: return this->hours;
    case daily: return this->hours * 365;
    case mo_fr: return this->hours * 260;
    case sa_su: return this->hours * 104;
    case weekly: return this->hours * 52;
    default: return 0;
    }
}

float Consumer::annual_hours_of_standby() { return 8760 - this->annual_hours_of_use(); }

float Consumer::annual_kwh() { return (((this->annual_hours_of_use() * this->watt) + (this->annual_hours_of_standby() * this->watt_standby)) / 1000); }

void Consumer::print(int index)
{
    cout << right << setw(columnwidth) << index << ": " << left << this->description << endl;
    cout << right << setw(columnwidth) << "  (this" << ": " << left << this << ")" << endl;
    cout << right << setw(columnwidth) << "power consumption" << ": " << left << this->watt << " w" << endl;
    cout << right << setw(columnwidth) << "power consumption standby" << ": " << left << this->watt_standby << " w" << endl;
    cout << right << setw(columnwidth) << "annual hours of use" << ": " << left << this->annual_hours_of_use() << " h" << endl;
    cout << right << setw(columnwidth) << "annual hours of standby" << ": " << left << annual_hours_of_standby() << " h" << endl;
}


class Household
{
 private:
    float price_kWh;
    string power_supplier;
    int persons;
    int square_meters;
    bool electric_water_heating;
    Consumer* consumers; // Assuming you have a 'consumer' class defined

 public:
    Household() { consumers = nullptr; }

    // Getter methods
    float get_price_kWh() { return price_kWh; }
    string get_power_supplier() { return power_supplier; }
    int get_persons() { return persons; }
    int get_square_meters() { return square_meters; }
    bool get_electric_water_heating() { return electric_water_heating; }
    Consumer* get_consumers() { return consumers; } // Assuming you have a 'get_consumers()' method in the consumer class

    // Setter methods
    void set_price_kWh(float price) { price_kWh = price; }
    void set_power_supplier(string& supplier) { power_supplier = supplier; }
    void set_persons(int numPersons) { persons = numPersons; }
    void set_square_meters(int area) { square_meters = area; }
    void set_electric_water_heating(bool heating) { electric_water_heating = heating; }
    void set_consumers(Consumer* newConsumers) { consumers = newConsumers; } // Assuming you have a 'set_consumers()' method in the consumer class

    // Public member functions
    void add_consumer_to_household(Consumer* newconsumer, bool readingFromFile);
    Consumer* move_up(int k); // 9. move_up power consumer from consumer list
    Household* copy_consumers(Household* source_house);
    void input_household();
    void printhousehold(int number_of_household);

    ~Household()
    {
        // Delete all power consumers
        Consumer* currentConsumer = consumers;
        while (currentConsumer != nullptr)
        {
            Consumer* nextConsumer = currentConsumer->get_next();
            delete currentConsumer;
            currentConsumer = nextConsumer;
        }
        cout << "Household at address " << this << " deleted\n";

    }

};

void Household::add_consumer_to_household(Consumer* newconsumer, bool readingFromFile)
{
    if (readingFromFile)
    {
        if (!this->consumers)
        {
            this->consumers = newconsumer;
        }
        else
        {
            Consumer* temp = this->consumers;
            while (temp->get_next()) temp = temp->get_next();
            temp->set_next(newconsumer);
        }
    }
    else
    {
        if (!this->consumers)
        {
            this->consumers = newconsumer;
        }
        else
        {
            newconsumer->set_next(this->consumers);
            this->consumers = newconsumer;
        }
    }
}


// 9. move_up power consumer from consumer list
Consumer* Household::move_up(int k)
{
    Consumer* current_head = this->consumers;
    if (k <= 1)
    {
        return current_head;
    }

    else
    {
        Consumer* current = current_head;
        Consumer* next_consumer = current->get_next();

        if (k == 2 && next_consumer != nullptr)
        {
            current->set_next(next_consumer->get_next());
            next_consumer->set_next(current_head);
            current_head = next_consumer;
            return current_head;
        }

        else
        {

            while (current != nullptr)
            {
                int i = 1;
                while (i < k - 2)
                {
                    if (current->get_next() == nullptr)
                    {
                        return current_head;
                    }
                    current = current->get_next();
                    i++;
                }

                if (current->get_next() == nullptr || current->get_next()->get_next() == nullptr)
                {
                    return current_head;
                }

                Consumer* kth_node = current->get_next()->get_next();
                current->get_next()->set_next(kth_node->get_next());
                kth_node->set_next(current->get_next());
                current->set_next(kth_node);

                return current_head;
            }

        }
    }

}

Household* Household::copy_consumers(Household* source_house)
{
    // copy all consumers from (source_house to destination_house)
    Consumer* temp_house_consumer = source_house->consumers;
    Consumer* destination_house_consumer = this->consumers;
    Consumer* temp_head = nullptr;
    Consumer* temp_tail = nullptr;

    if (temp_house_consumer == nullptr)
    {
        return this;
    }
    while (temp_house_consumer != nullptr)
    {

        Consumer* new_consumer = new Consumer;
        new_consumer->set_description(temp_house_consumer->get_description());
        new_consumer->set_watt(temp_house_consumer->get_watt());
        new_consumer->set_watt_standby(temp_house_consumer->get_watt_standby());
        new_consumer->set_hours(temp_house_consumer->get_hours());
        new_consumer->set_use(temp_house_consumer->get_use());
        new_consumer->set_next(nullptr);

        if (temp_head == nullptr)
        {
            temp_head = new_consumer;
            temp_tail = new_consumer;
        }
        else
        {
            temp_tail->set_next(new_consumer);
            temp_tail = new_consumer;
        }

        temp_house_consumer = temp_house_consumer->get_next();
    }
    temp_tail->set_next(this->consumers);
    this->consumers = temp_head;
    return this;
}

void Household::input_household()
{
    char hotwater;

    cout << "how many square meters does the household have: ";
    cin >> this->square_meters;
    cout << "how many persons live in this household?";
    cin >> this->persons;
    cout << "is hot water heated using electricity? (y(es) or n(o)) ";
    cin >> hotwater;
    cout << "what is the price for one kwh in eur? ";
    cin >> this->price_kWh;
    cout << "who is the power supplier? ";
    cin.ignore();
    getline(cin, this->power_supplier);

    if (hotwater == 'y')
        this->electric_water_heating = true;
    else
        this->electric_water_heating = false;
}

// funtion print household
void Household::printhousehold(int number_of_household)
{
    string heater = (this->get_electric_water_heating()) ? "yes" : "no";

    cout << "h o u s e h o l d  n o  " << " " << number_of_household << "  p o w e r  c o n s u m p t i o n" << endl;
    cout << "---------------------------------------------------------------------" << endl;
    cout << right << setw(columnwidth) << "  (this" << ": " << left << this << endl;
    cout << right << setw(columnwidth) << "price for one kwh" << ": " << left << setprecision(2) << fixed << this->price_kWh << " ct/kwh" << endl;
    cout << right << setw(columnwidth) << "power supplier" << ": " << left << this->power_supplier << endl;
    cout << right << setw(columnwidth) << "square meters" << ": " << left << this->square_meters << " qm" << endl;
    cout << right << setw(columnwidth) << "persons" << ": " << left << this->persons << endl;
    cout << right << setw(columnwidth) << "water heated using electricity" << ": " << left << heater << endl;
    cout << right << setw(columnwidth) << "list of consumers" << ": " << endl;
    cout << "---------------------------------------------------------------------" << endl;

    float powerconsumptionsquare_meters = (this->square_meters * annualaverageconsumptionpersquaremeter);
    float powerconsumptionpersons = (this->persons * (this->electric_water_heating ? annual_average_consumption_per_person_with_electric_water_heating : annualaverageconsumptionperperson)); // using ternary operator
    float totalconsumption = (powerconsumptionsquare_meters + powerconsumptionpersons);
    float totalcosts = (totalconsumption * (this->price_kWh));
    int index = 1;

    Consumer* currentconsumer = this->consumers;

    while (currentconsumer != nullptr)
    {
        totalconsumption += currentconsumer->annual_kwh();
        currentconsumer->print(index);
        cout << right << setw(columnwidth) << "annual consumption" << ": " << left << currentconsumer->annual_kwh() << " kwh" << endl;
        cout << right << setw(columnwidth) << "annual costs" << ": " << left << (currentconsumer->annual_kwh() * this->price_kWh) << " eur" << endl;
        currentconsumer = currentconsumer->get_next();
        index++;
    }

    cout << string(columnwidth * 2, '-') << endl;
    cout << right << setw(columnwidth) << "power consumption square meters" << ": " << left << powerconsumptionsquare_meters << " kwh" << endl;
    cout << right << setw(columnwidth) << "power consumption all persons" << ": " << left << powerconsumptionpersons << " kwh" << endl;
    cout << right << setw(columnwidth) << "total annual power consumption" << ": " << left << totalconsumption << " kwh" << endl;
    cout << right << setw(columnwidth) << "total annual power costs" << ": " << left << (totalconsumption * this->price_kWh) << " eur" << endl;
}

class Address
{
 private:
    string street;
    string no;
    string zip;
    string city;
 public:
    Address() { street = no = zip = city = ""; }

    Address(string street, string no, string zip, string city)
    {
        this->street = street;
        this->no = no;
        this->zip = zip;
        this->city = city;
    }
    // getter method
    string get_street() { return this->street; }
    string get_no() { return this->no; }
    string get_zip() { return this->zip; }
    string get_city() { return this->city; }
    string to_string() { return this->street + " " + this->no + ", " + this->zip + " " + this->city; }

    void input()
    {
        cin.ignore();
        cout << "what is the street name? ";
        getline(cin, street);
        cout << "what is house number? ";
        getline(cin, no);
        cout << "what is zip code? ";
        getline(cin, zip);
        cout << "what is the city name? ";
        getline(cin, this->city);
    }

    ~Address()
    {
        cout << "Address " << this->street << ' ' << this->no << ", " << this->zip << ' ' << this->city << " at address " << this << " deleted\n";
    }
};

class House
{
 private:
    int number_of_households;
    Address address;
    Household** household;
 public:
    House(int n, Address a)
    {
        this->number_of_households = n;
        this->address = a;
        this->household = new Household * [number_of_households];
        for (int i = 0; i < number_of_households; i++)
        {
            this->household[i] = nullptr;
        }
    }
    // public member function
    Household* get_household(int n) { return this->household[n]; }
    Household* operator[](int n) { return this->household[n]; }
    Address get_address() { return address; }
    void set_household(Household* household, int n) { this->household[n] = household; }
    void write_use_to_file(ofstream& ofs, Use curr);
    void readcityinformation(ifstream& ifs, string& taskidentifier, char separator, int& length);
    void readhouseholdinformation(string line, char separator, int startindex, int& index, Household& household);
    Use  readfrequencyofuse(string curr);
    void readconsumerinformation(string line, char separator, int startindex, Consumer& consumer);
    void print_all();
    void write_data_to_file(string filename, char separator);
    void read_from_file(string filename, string& taskidentifier, char separator, int& length);

    ~House()
    {
        // Delete all households
        for (int i = 0; i < number_of_households; i++)
        {
            delete household[i];
        }

        cout << "House at address " << this << " deleted\n";
        delete[] household;
    }
};



void House::write_use_to_file(ofstream& ofs, Use curr) // write the frequency of use to a file
{
    if (curr == Use::once)
        ofs << "once";
    else if (curr == Use::daily)
        ofs << "daily";
    else if (curr == Use::mo_fr)
        ofs << "monday to friday";
    else if (curr == Use::sa_su)
        ofs << "saturday and sunday";
    else
        ofs << "weekly";
}

void House::readcityinformation(ifstream& ifs, string& taskidentifier, char separator, int& length)
{
    // read a line from the input stream
    string line;
    getline(ifs, line);

    // extract task identifier
    int i = 0;
    string curr = "";
    while (line[i] != separator)
    {
        curr += line[i];
        i++;
    }
    taskidentifier = curr;
    curr = "";
    i++;

    // extract length
    while (line[i] != separator)
    {
        curr += line[i];
        i++;
    }
    i++;
    length = stoi(curr);
    curr = "";

    string street, houseNo, zipCode, cityName;

    // extract street
    while (line[i] != separator)
    {
        curr += line[i];
        i++;
    }
    street = curr;
    curr = "";
    i++;

    // extract house number
    while (line[i] != separator)
    {
        curr += line[i];
        i++;
    }
    houseNo = curr;
    curr = "";
    i++;

    // extract zip code
    while (line[i] != separator)
    {
        curr += line[i];
        i++;
    }
    zipCode = curr;
    curr = "";
    i++;

    // extract city name
    while (i < line.length())
    {
        curr += line[i];
        i++;
    }
    cityName = curr;
    curr = "";
    i++;

    Address newAddress(street, houseNo, zipCode, cityName);
    this->address = newAddress;
}

void House::readhouseholdinformation(string line, char separator, int startindex, int& index, Household& household)
{
    // extract index
    string curr = "";
    int i = startindex;
    while (line[i] != separator)
    {
        curr += line[i];
        i++;
    }
    i++;
    index = stoi(curr);
    curr = "";

    // extract hot water indicator
    while (line[i] != separator)
    {
        curr += line[i];
        i++;
    }
    i++;
    bool hotwater = ((curr == "true") ? true : false);
    household.set_electric_water_heating(hotwater);

    curr = "";

    // extract number of people
    while (line[i] != separator)
    {
        curr += line[i];
        i++;
    }
    int numberofpeople = stoi(curr);
    household.set_persons(numberofpeople);
    curr = "";
    i++;

    while (line[i] != separator)
    {
        curr += line[i];
        i++;
    }
    int square_meters = stoi(curr);
    household.set_square_meters(square_meters);
    curr = "";
    i++;

    while (line[i] != separator)
    {
        curr += line[i];
        i++;
    }
    float price_kwh = stof(curr);
    household.set_price_kWh(price_kwh);
    curr = "";
    i++;

    while (i < line.length())
    {
        curr += line[i];
        i++;
    }

    string power_supplier = curr;
    household.set_power_supplier(power_supplier);
}

// convert a string to an enumerated type representing frequency of use
Use House::readfrequencyofuse(string curr)
{
    if (curr == "once")                      return Use::once;
    else if (curr == "daily")                return Use::daily;
    else if (curr == "monday to friday")     return Use::mo_fr;
    else if (curr == "saturday and sunday")  return Use::sa_su;
    else                                     return Use::weekly;
}

void House::readconsumerinformation(string line, char separator, int startindex, Consumer& consumer)
{
    // extract description
    string curr = "";
    int i = startindex;
    while (line[i] != separator)
    {
        curr += line[i];
        i++;
    }
    i++;
    consumer.set_description(curr);
    curr = "";

    // extract hours
    while (line[i] != separator)
    {
        curr += line[i];
        i++;
    }
    i++;
    consumer.set_hours(stof(curr));
    curr = "";

    // extract use (frequency of use)
    while (line[i] != separator)
    {
        curr += line[i];
        i++;
    }
    Use use = readfrequencyofuse(curr);
    consumer.set_use(use);
    i++;
    curr = "";

    // extract watt
    while (line[i] != separator)
    {
        curr += line[i];
        i++;
    }
    consumer.set_watt(stof(curr));
    i++;
    curr = "";

    // extract standby watt
    while (i < line.length())
    {
        curr += line[i];
        i++;
    }
    consumer.set_watt_standby(stof(curr));
}

void House::print_all()
{
    cout << " ======================================================================\n";
    cout << "                                 H O U S E\n";
    cout << " ======================================================================\n";
    cout << "                                 (this: " << this->household << '\n';
    cout << "                               address: " << this->address.to_string() << '\n';
    cout << "                  number of households: " << this->number_of_households << '\n';   
    cout << "         total number of all consumers: " << Consumer::getTotalConsumers() << '\n';

    for (int i = 0; i < this->number_of_households; i++)
    {
        if (this->household[i] != nullptr)
        {
            this->household[i]->printhousehold(i);
        }
    }
}

void House::read_from_file(string filename, string& taskidentifier, char separator, int& length)
{
    // open the input file
    ifstream ifs(filename);
    if (!ifs.is_open())
    {
        cout << "could not open file " << filename << "!\n";
        return;
    }
    string line;

    cout << "input file " << filename << " opened...\n";

    // read city information from the file
    this->readcityinformation(ifs, taskidentifier, separator, length);
    this->number_of_households = length;
    this->household = new Household* [number_of_households];
    for (int i = 0; i < number_of_households; i++)
    {
        this->household[i] = nullptr;
    }
    // read data for each household and consumer from the file
    string curr = "";
    int i = 0;
    int index;
    int totalConsumers = 0;
    while (getline(ifs, line))
    {
        i = 0;
        // extract the type (household or consumer)
        while (i < line.length() && line[i] != separator)
        {
            curr += line[i];
            i++;
        }
        i++;
        if (curr == "household")
        {
            // create a new household and read its information
            Household* house = new Household;
            house->set_consumers(nullptr);

            readhouseholdinformation(line, separator, i, index, *house);
            this->household[index] = house;
        }
        else if (curr == "consumer")
        {
            // create a new consumer and read its information
            Consumer* consumer = new Consumer;
            readconsumerinformation(line, separator, i, *consumer);
            this->household[index]->add_consumer_to_household(consumer, true);
            totalConsumers++;
        }
        curr = "";

    }

    // close the input file
    ifs.close();
    cout << "input file " << filename << " closed...\n";
}

void House::write_data_to_file(string filename, char separator) // write household and consumer data to a file
{
    // open the output file
    ofstream ofs(filename);

    cout << "output file " << filename << " opened...\n";

    // write header information to the file
    ofs << "A5" << separator << this->number_of_households << separator << address.get_street() << separator << address.get_no() << separator << address.get_zip() << separator << address.get_city() << '\n';

    // iterate through each household
    for (int i = 0; i < this->number_of_households; i++)
    {
        // check if the household exists
        if (this->household[i])
        {
            // write household information to the file
            ofs << "household" << separator << to_string(i) << separator;
            if (this->household[i]->get_electric_water_heating())
                ofs << "true";
            else
                ofs << "false";
            ofs << separator;
            ofs << to_string(this->household[i]->get_persons()) << separator << to_string(this->household[i]->get_square_meters()) << separator;
            ofs << to_string(this->household[i]->get_price_kWh()) << separator << this->household[i]->get_power_supplier() << '\n';

            // iterate through each consumer in the household
            Consumer* curr = this->household[i]->get_consumers();
            while (curr)
            {
                // write consumer information to the file
                ofs << "consumer" << separator << curr->get_description() << separator << curr->get_hours() << separator;
                write_use_to_file(ofs, curr->get_use());
                ofs << separator << curr->get_watt() << separator << curr->get_watt_standby() << '\n';

                // move to the next consumer
                curr->set_next(curr->get_next());
                curr = curr->get_next();
            }
        }
    }

    // close the output file
    cout << "output file " << filename << " closed...\n";
    ofs.close();
}

// main function
int main()
{
    string filename;
    string taskidentifer;
    int length, n;
    House* house = nullptr;
    Address a;
   
    while (true)
    {
        cout << "q quit" << endl;
        cout << "d delete house" << endl;
        cout << "h house initialisation" << endl;
        cout << "i input power consumer" << endl;
        cout << "u move up power consumer" << endl;
        cout << "p print household" << endl;
        cout << "a print all households" << endl;
        cout << "n new household" << endl;
        cout << "c copy all cosumers (added to already existing ones)" << endl;
        cout << "r read data from file" << endl;
        cout << "w write data into file\n>> ";

        char choice;
        cin >> choice;

        if (choice == 'q')
        {
            break;
        }
        else if (choice == 'd')
        {
            house->~House();
        }
        else if (choice == 'h')
        {
            int number_of_households;
            cout << "how many households does the house have? ";
            cin >> number_of_households;
            a.input();
            house = new House(number_of_households, a);
            // Address *b = new Address;
            // delete b ;
        }
        else if (choice == 'i')
        {
            int num;
            cout << "number of household?";
            cin >> num;
            Consumer* new_consumer = new Consumer;
            new_consumer->input();
            Household* h = house->get_household(num);
            h->add_consumer_to_household(new_consumer, false);
        }
        else if (choice == 'u')
        {
            int k, num;
            cout << "number of household?";
            cin >> num;
            cout << "which one? ";
            cin >> k;
            Household* h = house->get_household(num);
            h->move_up(k);
        }
        else if (choice == 'p')
        {
            int number;
            cout << "number of household";
            cin >> number;
            Household* h = house->get_household(number);
            h->printhousehold(number);
        }
        else if (choice == 'a')
        {
            if (house == nullptr)
            {
                cout << "house is a nullptr, please first choose h to initialise a new house\n";
            }
            else
                house->print_all();
        }

        else if (choice == 'n')
        {
            int x;
            cout << "number of household? ";
            cin >> x;

            Household* h = house->get_household(x);

            if (h != nullptr)
            {
                cout << "household already exists" << endl;
            }
            else
            {
                Household* new_household = new Household;
                new_household->input_household();
                house->set_household(new_household, x);
            }
        }
        else if (choice == 'c')
        {
            int num1, num2;
            cout << "number of household from which to copy consumers?";
            cin >> num1;
            cout << "number of household to copy to?";
            cin >> num2;
            Household* h1 = house->get_household(num1);
            Household* h2 = house->get_household(num2);

            if (h1 && h2)
            {
                h2->copy_consumers(h1);
            }

        }
        else if (choice == 'r')
        {
            string filename;
            char separator;
            cout << "input file name: ";
            cin >> filename;
            cout << "input separator character: ";
            cin >> separator;
            ifstream ifs(filename);
            house->read_from_file(filename, taskidentifer, separator, length);
        }
        else if (choice == 'w')
        {
            string filename;
            char separator;
            cout << "output file name: ";
            cin >> filename;
            cout << "output separator character: ";
            cin >> separator;
            house->write_data_to_file(filename, separator);
        }
        else
        {
            cout << "sorry wrong choice" << endl;
        }
    } 
    return 0;
}