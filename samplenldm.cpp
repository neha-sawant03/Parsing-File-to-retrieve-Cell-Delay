/*Name = Neha Sawant
  Internet ID = sawan050
  Program Name = To parse file with string values to extract gate names and cell delay values
  */

#include <iostream>
#include <fstream>
#include <string>
#include <cstdlib>
#include <sstream>
#include <cstring>
#include <vector>

using namespace std;

struct {
    string cell;
} myStructure[6];

string* readFileToarr(string filename)
{   // final array to store gate names
    string arr[6];
    string line;
    size_t found;
    int getArrayLength;
    ifstream loadFile;

    //loads the files
    loadFile.open(filename.c_str(), ifstream::in);
    while (loadFile.fail()) {
        cout << "The file \"" << filename << "\" failed to open.\n\n";
        exit(EXIT_FAILURE);
    }
    //find 'cell (' pattern in file
    string pattern = "cell (";
    while (getline(loadFile, line)) {

        found = line.find(pattern);
        if (found != string::npos) {
            found = line.find(pattern, found + 1);

            stringstream ss(line);
            string word, final;
            while (!ss.eof()) {
                getline(ss, word, '{');
               
                cout << word;
            }
            int i = 0;
            stringstream ssin(word);
            while (ssin.good() && i < 6) {

                ssin >> arr[i];
                ++i;
            }
             getArrayLength = sizeof(arr) / sizeof(string) + 1;
           
            for (i = 0; i < 6; i++) {
                cout << arr[i] << endl;
            } 
        }
    }
    cout << "Number of gate types are" << " "<< getArrayLength << endl;
    cout << endl;
    loadFile.close();
    return arr;
}

 void readFileTodel(string filename)
{  
    
    ifstream load;

    load.open(filename.c_str(), ifstream::in);
    while (load.fail()) {
        cout << "The file \"" << filename << "\" failed to open.\n\n";
        exit(EXIT_FAILURE);
    }

    string line, lines; 
    string l;
   
    size_t found, foundn;
    string cellName; string firstWord, f;
    string pattern = "cell_delay(";
    while (getline(load, line)) {
        
        found = line.find(pattern);
        if (found != string::npos) {
            while (getline(load, lines, 'v'))     //(getline(load, lines, 'v' ))  works for getting values
            {
                
                foundn = lines.find("output_slew(");
                if (foundn != string::npos)

                {
                    size_t startPos = lines.find("s");
                    size_t endPos = lines.find(")", startPos);
                    cellName = lines.substr(startPos, endPos);
                    stringstream iss(cellName);
                    
                    while (!iss.eof()) {
                        getline(iss, firstWord, '\\');
                        string final; 
                        
                        char c;
                        stringstream iu(firstWord); std::vector<std::vector<double>> values;
                        while (getline(iu, f, '"')) { 
                            while (!iu.eof()) {
                                iu >> c;
                                //clean the line by eliminating the char symbols and store it in final variable
                                if (c != 's' && c != ' ' && c != '(' && c != '"' && c != ';' && c != ')') {
                                    final += c;
                                    continue;
                                }
                               
                                stringstream u(final);
                                std::vector<double> row;
                                std::string value;
                                while (!u.eof())
                                {   // split lines by commam and get the individual values
                                    getline(u, l, ',');
                                    // to convert string to double
                                    row.push_back(std::stod(l));
                                }
                                // Add the row to the 2D vector
                                values.push_back(row);
                            }
                           
                            for (const std::vector<double>& row : values) {
                                for (double value : row) {
                                    std::cout << value << " ";
                                }
                                std::cout << std::endl;
                            }

                            cout << endl;
                        }
                       
                    }

                }
            }

        }
    }
    load.close();
   
}

int main(int argc, char* argv[])
{
    string arr1[6];
    string* p;
    string ffileName;
    ffileName = argv[1];
    p = readFileToarr(ffileName);
        readFileTodel(ffileName);
    for (int i = 0; i < 6; i++)
    {
        myStructure[i].cell = p[i];
    }
    for (int i = 0; i < 6; i++)
    {
        cout << myStructure[i].cell;
        cout << endl;
    }
    return 0;
}


