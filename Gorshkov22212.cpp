#include <iostream>
#include <string>
#include <fstream>
#include <map>
#include <istream>
#include <vector>



using namespace std;

double inputWhat(const string& prompt, const string& sto) {
    string input;
    int value;
    size_t processed_chars;
    while (true) {
        std::cout << prompt;
        getline(cin, input);
        if (sto == "Repair") {
            if (input == "Y" || input == "y") {
                return true;
            }
            else if (input == "N" || input == "n") {
                return false;
            }
            else {
                std::cout << "Enter Y/N" << endl;
            }
        }
        else {
            try {
                if (sto == "Integer") {
                    value = stoi(input, &processed_chars);
                }
                else if (sto == "Double") {
                    value = stod(input, &processed_chars);
                }
                if (processed_chars == input.length() && value >= 0) {
                    return value;
                }
                else {
                    std::cout << "Enter correct number!" << endl;
                }
            }
            catch (...) {
                std::cout << "Enter correct number!" << endl;
            }
        }
    }
}



class Pipe {
private:
    struct parameters {
        string name;
        int length;
        int diameter;
        bool inRepair;
    };
    int id = 0;

    map<int, parameters> pipes;

public:

    void addPipe() {
        this->id = ++id;
        string name;
        cout << "Enter name: ";
        getline(cin, name);
        int length = inputWhat("Enter length: ", "Integer");
        int diameter = inputWhat("Enter diameter: ", "Integer");
        bool inRepair = inputWhat("Is the pipe being repaired? (Y/N):  ", "Repair");

        parameters pipe = { name, length, diameter, inRepair };
        pipes[id] = pipe;
    }

    void displayPipes() const {
        for (const auto& pipe : pipes) {
            cout << "Pipe ID: " << pipe.first << ", Pipe name: " << pipe.second.name << ", Length: " << pipe.second.length << ", Diameter: " << pipe.second.diameter
                << ", In Repair: " << (pipe.second.inRepair ? "Yes" : "No") << endl;
        }
    }

    void editPipe(int id) {
        if (pipes.find(id) != pipes.end()) {
            pipes[id].inRepair = !pipes[id].inRepair;
            cout << "Pipe with ID: " << id << " has been updated" << endl;
        }
        else {
            cout << "Pipe with ID: " << id << " not found" << endl;
        }
    }

    void deletePipe() {
        displayPipes();
        int delID = inputWhat("Enter ID: ", "Integer");
        if (pipes.erase(delID)) {
            cout << "Pipe with ID " << delID << " has been deleted" << endl;
        }
        else {
            cout << "Pipe with ID " << delID << " not found." << endl;
        }
    }

    void filterPipe() {
        if (pipes.empty()) {
            cout << "No pipes available.\n";
            return;
        }
        displayPipes();
        vector<int> filtered;
        string filterName;
        bool filterByName = inputWhat("Filter by name? (Y/N): ", "Repair");

        if (filterByName) {
            cout << "Enter name to filter by: ";
            getline(cin, filterName);

            for (int i = 1; i <= id; ++i) {
                if (pipes.find(i) != pipes.end() && pipes[i].name.find(filterName) != string::npos) {
                    cout << "ID: " << i
                        << ", Name: " << pipes[i].name
                        << ", Length: " << pipes[i].length
                        << ", Diameter: " << pipes[i].diameter
                        << ", In Repair: " << (pipes[i].inRepair ? "Yes" : "No") << endl;
                    filtered.push_back(i);
                }
            }
        }
        else {
            bool filterRepair = inputWhat("Repair status (Y/N): ", "Repair");

            for (int i = 1; i <= id; ++i) {
                if (pipes.find(i) != pipes.end() && pipes[i].inRepair == filterRepair) {
                    cout << "ID: " << i
                        << ", Name: " << pipes[i].name
                        << ", Length: " << pipes[i].length
                        << ", Diameter: " << pipes[i].diameter
                        << ", In Repair: " << (pipes[i].inRepair ? "Yes" : "No") << endl;
                    filtered.push_back(i);
                }
            }
        }

        // Выбор редактирования после фильтрации
        string choice;
        cout << "Edit all (A), specific by ID (S), or cancel (N)? (A/S/N): ";
        getline(cin, choice);
        if (choice == "A" || choice == "a") {
            bool newInRepair = inputWhat("Set repair status (Y/N): ", "Repair");
            for (const auto& i : filtered) {
                pipes[i].inRepair = newInRepair; // Обновление статуса для всех
            }
            cout << "All found pipes have been updated.\n";
        }
        else if (choice == "S" || choice == "s") {
            int pipeID = inputWhat("Enter pipe ID to edit: ", "Integer");
            editPipe(pipeID); // Редактирование конкретной трубы
        }
        else if (choice == "N" || choice == "n") {
            cout << "No changes made.\n";
        }
        else {
            cout << "Invalid choice.\n";
        }
    }

    void saveToFile(ofstream& outFile) {

        if (!pipes.empty()) {
            for (auto& pipe : pipes) {
                outFile << "Pipe ID: " << pipe.first << ", Pipe Name: " << pipe.second.name << ", Length: " << pipe.second.length << ", Diameter: " << pipe.second.diameter
                    << ", In Repair: " << (pipe.second.inRepair ? "Yes" : "No") << endl;
            }
        }
    }

    void loadFromFile(string fileName) {

        ifstream inFile(fileName);

        if (!inFile.is_open()) {
            std::cout << "Error: Unable to open file for loading" << endl;
            return;
        }

        string line;

        while (getline(inFile, line)) {
            if (line.empty()) continue;

            if (line.find("Pipe ID:") != string::npos) {
                int id;
                string name;
                double length, diameter;
                bool inRepair;

                size_t idPos = line.find("Pipe ID:") + 9;
                size_t namePos = line.find("Pipe Name:") + 11;
                size_t lengthPos = line.find("Length:") + 8;
                size_t diameterPos = line.find("Diameter:") + 10;
                size_t repairPos = line.find("In Repair:") + 11;

                id = stoi(line.substr(idPos, line.find(",", idPos) - idPos));
                name = line.substr(namePos, line.find(",", namePos) - namePos);
                length = stod(line.substr(lengthPos, line.find(",", lengthPos) - lengthPos));
                diameter = stod(line.substr(diameterPos, line.find(",", diameterPos) - diameterPos));
                string inRepairStr = line.substr(repairPos);

                inRepair = (inRepairStr == "Yes");

                parameters pipe = { name, length, diameter, inRepair };
                pipes[id] = pipe;
                
            }
        }

        inFile.close();
    }



};


class CS {
private:
    struct par {
        string Name;
        int Stations;
        int WorkingStations;
        int effectivity;
    };

    int id = 0;
    map<int, par> CSs;
public:
    
    void addCS() {
        this->id = ++id;
        string name;
        cout << "Enter name of CS: ";
        getline(cin, name);
        int WorkingStations;
        int effectivity;
        int Stations = inputWhat("Enter number of stations: ", "Integer");
       
        while (true)
        {
            WorkingStations = inputWhat("Enter numbber of working stations: ", "Integer");
            if (WorkingStations <= Stations) {
                break;
            }
        }

        while (true)
        {
            effectivity = inputWhat("Enter effectivity (0-100): ", "Integer");
            if (effectivity >= 0 && effectivity <= 100) {
                break;
            }
        }

        par cs = { name, Stations, WorkingStations, effectivity };
        CSs[id] = cs;
    }

    

    void showCS() {
        for (const auto& cs : CSs) {
            std::cout << "CS ID: " << cs.first << " CS name: " << cs.second.Name << ", the number of stations: " << cs.second.Stations << ", the number of working stations: " << cs.second.WorkingStations
                << ", Effectivity: " << cs.second.effectivity << endl;
        }
    }

    void deleteCS() {
        showCS();
        int delID;
        delID = inputWhat("Enter ID: ", "Integer");
        if (CSs.erase(delID)) {
            cout << "CS with ID " << delID << " deleted successfully." << endl;
        }
        else {
            cout << "CS with ID " << delID << " not found." << endl;
                
        }
    }

    void editCS(int editID) {
        if (CSs.find(editID) != CSs.end()) {
            while (true)
            {
                CSs[editID].WorkingStations = inputWhat("Enter numbber of working stations: ", "Integer");
                if (CSs[editID].WorkingStations <= CSs[editID].Stations) {
                    break;
                }
            }
        }
    }

    void filterCS() {
        if (CSs.empty()) {
            cout << "No pipes available.\n";
            return;
        }
        showCS();
        vector<int> filtered;
        string filterName;
        bool filterByName = inputWhat("Filter by name? (Y/N): ", "Repair");

        if (filterByName) {
            cout << "Enter name to filter by: ";
            getline(cin, filterName);

            for (int i = 1; i <= id; ++i) {
                if (CSs.find(i) != CSs.end() && CSs[i].Name.find(filterName) != string::npos) {
                    cout << "ID: " << i
                        << ", Name: " << CSs[i].Name
                        << ", Number of stations: " << CSs[i].Stations
                        << ", Number working stations: " << CSs[i].WorkingStations
                        << ", Effictivity: " << CSs[i].effectivity << endl;
                    filtered.push_back(i);
                }
            }
        }
        else {
            int filterCoef = inputWhat("Working stations/Stations * 100%: ", "Integer");

            for (int i = 1; i <= id; ++i) {
                if (CSs.find(i) != CSs.end() && (((CSs[i].WorkingStations * 100) / CSs[i].Stations) >= (filterCoef - 15)) && (((CSs[i].WorkingStations * 100) / CSs[i].Stations) <= (filterCoef + 15))) {
                    cout << "ID: " << i
                        << ", Name: " << CSs[i].Name
                        << ", Number of stations: " << CSs[i].Stations
                        << ", Number working stations: " << CSs[i].WorkingStations
                        << ", Effictivity: " << CSs[i].effectivity << endl;
                    filtered.push_back(i);
                }
            }
        }

        string choice;
        cout << "Edit all (A), specific by ID (S), or cancel (N)? (A/S/N): ";
        getline(cin, choice);
        int newEffectivity;
        if (choice == "A" || choice == "a") {
            while (true)
            {
                newEffectivity = inputWhat("Set effectuvuty: ", "Integer");
                if (newEffectivity <= 100 && newEffectivity >= 0) {
                    break;
                }
            }
            
            for (const auto& i : filtered) {
                CSs[i].effectivity = newEffectivity;
            }
            cout << "All found CSs have been updated.\n";
        }
        else if (choice == "S" || choice == "s") {
            int csID = inputWhat("Enter CS ID to edit: ", "Integer");
            editCS(csID);
        }
        else if (choice == "N" || choice == "n") {
            cout << "No changes made.\n";
        }
        else {
            cout << "Invalid choice.\n";
        }
    }

    void saveToFile(ofstream& outFile) {
        if (!CSs.empty()) {
            for (auto& station : CSs) {
                outFile << "CS name: " << station.second.Name << ", CS ID: " << station.first << ", the number of stations: " << station.second.Stations << ", the number of working stations: " << station.second.WorkingStations
                    << ", Effectivity: " << station.second.effectivity << endl;
            }
        }
    }

    void loadFromFile(string fileName) {

        ifstream inFile(fileName);

        if (!inFile.is_open()) {
            std::cout << "Error: Unable to open file for loading" << endl;
            return;
        }

        string line;

        while (getline(inFile, line)) {
            if (line.empty()) continue;

            if (line.find("CS name:") != string::npos) {
                string name;
                int id;
                double stationsCount, workingSts, effectivity;

                size_t namePos = line.find("CS name:") + 9;
                size_t idPos = line.find("CS ID:") + 7;
                size_t stationsCountPos = line.find("the number of stations:") + 24;
                size_t workingStsPos = line.find("the number of working stations:") + 32;
                size_t effectivityPos = line.find("Effectivity:") + 13;

                name = line.substr(namePos, line.find(",", namePos) - namePos);
                id = stoi(line.substr(idPos, line.find(",", idPos) - idPos));
                stationsCount = stod(line.substr(stationsCountPos, line.find(",", stationsCountPos) - stationsCountPos));
                workingSts = stod(line.substr(workingStsPos, line.find(",", workingStsPos) - workingStsPos));
                effectivity = stod(line.substr(effectivityPos));

                if (name.empty() || stationsCount < 0 || workingSts < 0 || effectivity < 0) {
                    std::cout << "Error loading data: Invalid values found in line: " << line << endl;
                    continue;
                }

                par cs = { name, stationsCount, workingSts, effectivity };
                CSs[id] = cs;

            }
        }

        inFile.close();
    }
};






void displayMenu() {
    std::cout << endl << "===Menu===" << endl
        << "1.Add pipe" << endl
        << "2.Add CS" << endl
        << "3.View all objects" << endl
        << "4.Edit pipe" << endl
        << "5.Edit CS" << endl
        << "6.Filter Pipe" << endl
        << "7.Filter CS" << endl
        << "8.Delete Pipe" << endl
        << "9.Delete CS" << endl
        << "10.Save" << endl
        << "11.Load" << endl
        << "0.Exit" << endl;
}


int main() {
    Pipe pipe;
    CS cs;
    int editID;
    bool flag = true;
    int id;
    string fileName;
    ofstream outFile;
    
    while (flag) {
        displayMenu();
        int choice = inputWhat("Your choice: ", "Integer");
        switch (choice)
        {
        case 1: {
            pipe.addPipe();
            break;
        }
        case 2: {
            cs.addCS();
            break;
        }
        case 3: {
            pipe.displayPipes();
            cs.showCS();
            break;
        }
        case 4:
            pipe.displayPipes();
            id = inputWhat("Enter pipe ID: ", "Integer");
            pipe.editPipe(id);
            break;
        case 5:
            cs.showCS();
            editID = inputWhat("Enter ID: ", "Integer");
            cs.editCS(editID);
            break;
        case 6:
            pipe.filterPipe();
            break;
        case 7:
            cs.filterCS();
            break;
        case 8:
            pipe.deletePipe();
            break;
        case 9:
            cs.deleteCS();
            break;
        case 10:
            cout << "Enter name of file: ";
            getline(cin, fileName);

            outFile.open(fileName);

            if (!outFile) {
                cout << "Error: Unable to open file for writing." << endl;
                break;
            }

            pipe.saveToFile(outFile);
            cs.saveToFile(outFile);
            outFile.close();
            cout << "Data saved to file: " << fileName << endl;
            break;
        case 11:
            cout << "Enter Name of File: ";
            getline(cin, fileName);

            pipe.loadFromFile(fileName);
            cs.loadFromFile(fileName);
            cout << "Data successfully loaded from output.txt" << endl;
            break;
        case 0:
            flag = false;
            break;
        default:
            std::cout << "Enter number from 0 to 7" << endl;
            break;
        }
    }
}