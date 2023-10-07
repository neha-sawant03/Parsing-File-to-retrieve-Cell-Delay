#include <iostream>
#include <string>
#include <fstream>
#include <sstream>
#include <map>  // Include the map header
#include <vector>

#define GATE_DELAY_N 7

using namespace std;
int readFileTomap(string filename);

int main(int argc, char* argv[])
{
    string ffileName;
    ffileName = argv[1];
    readFileTomap(ffileName);
  
    return 0;
}

int readFileTomap(string filename)
{

    ifstream loadFile;
   
    vector<string> v;  // storing gate names
    std::map<int, vector<vector<double>> > map2;
    std::map<int, vector<vector<double>> > map3;
    // fills delay value in loop for one cell and then stores it in map2
    vector<double> d;  
    // fills slew value in loop for one cell and then stores it in map3
    vector<double> s;
    int db_idx = 0;
    int db_idx1 = 0;

    //loads the files
    loadFile.open(filename.c_str(), ifstream::in);
    if (loadFile.fail()) {
        cout << "The file \"" << filename << "\" failed to open.\n\n";
        exit(EXIT_FAILURE);
    }
    // Keep reading file while no stream error occurs
    // like end of file (eof) etc.
    while (loadFile.good()) {
        string first_word;
        loadFile >> first_word;

        // An info entry must start with "cell" as the first word exactly.
        if (first_word.compare("cell") == 0 ) {
            string second_word;
            loadFile >> second_word;

            // The gate name has the format of "(<gate_name>)".
            // So find the index till the closing bracket.
            // Make sure to ignore the first index which has the opening bracket (hence the 1 and -1)
            size_t delim_pos = second_word.find(")");
            string gate_name = second_word.substr(1, delim_pos - 1);

            cout << "Reading info about gate: " << gate_name << endl;
            v.push_back(gate_name);           

        }
        // The cell delays will start after the word "cell_delay(Timing_7_7)" exactly
        else if (first_word.compare("cell_delay(Timing_7_7)") == 0) {
            // Read 3 lines that contain the rest of above match, index 1 and index 2
            string tmp;
            getline(loadFile, tmp);
            getline(loadFile, tmp);
            getline(loadFile, tmp);

          
             // From here on the next 7 lines will contain our delays
            for (size_t i = 0; i < GATE_DELAY_N; i++) {
                getline(loadFile, tmp);
                d.clear();
                // The delays will be between " ". Find the opening ".
                size_t start_delim_idx = tmp.find("\"");

                // Find the closing ".
                // The second argument is where we want to start our search
                // Ignore the first match so we don't get the same index again
                size_t end_delim_idx = tmp.find("\"", start_delim_idx + 1);

                // The second arg in substr in no. of characters, not the ending index
                string data_str = tmp.substr(start_delim_idx + 1, end_delim_idx - start_delim_idx - 1);

                // Convert this remaining string to a stream so we can parse our data in doubles
                istringstream data_stream(data_str);
                for (size_t j = 0; j < GATE_DELAY_N; j++) {
                    double delay;
                    char delim;
                    data_stream >> delay >> delim;

                    d.push_back(delay);
                  
                }
                map2[db_idx].push_back(d);

            }

            // At the end of nested for loop we will have finised reading the 7x7 delays.
            // Increment our database pointer so we can store the next entry
            db_idx++;


        }
        else if (first_word.compare("output_slew(Timing_7_7)") == 0) {
            // Read 3 lines that contain the rest of above match, index 1 and index 2
            string tmp1;
            getline(loadFile, tmp1);
            getline(loadFile, tmp1);
            getline(loadFile, tmp1);

            
             // From here on the next 7 lines will contain our delays
            for (size_t i = 0; i < GATE_DELAY_N; i++) {
                getline(loadFile, tmp1);
                s.clear();
                // The delays will be between " ". Find the opening ".
                size_t start_delim_idx = tmp1.find("\"");

                // Find the closing ".
                // The second argument is where we want to start our search
                // Ignore the first match so we don't get the same index again
                size_t end_delim_idx = tmp1.find("\"", start_delim_idx + 1);

                // The second arg in substr in no. of characters, not the ending index
                string data_str = tmp1.substr(start_delim_idx + 1, end_delim_idx - start_delim_idx - 1);

                // Convert this remaining string to a stream so we can parse our data in doubles
                istringstream data_stream(data_str);
                for (size_t j = 0; j < GATE_DELAY_N; j++) {
                    double slew;
                    char delim;
                    data_stream >> slew >> delim;

                    s.push_back(slew);
                    
                }
                map3[db_idx1].push_back(s);

            }

            // At the end of nested for loop we will have finised reading the 7x7 delays.
            // Increment our database pointer so we can store the next entry
            db_idx1++;


        }
    }
    loadFile.close();

    // Finished reading, so the last index is just the length of our database
    size_t db_size = db_idx;
    size_t db_size1 = db_idx1;
    // Finally write our database into the output file
    ofstream ofs("nehassawant_pa0.txt");
    // Always check if file opened successfully
    if (!ofs.is_open()) {
        cout << "Error opening output file\n";
        return -1;
    }

    ofs << db_idx << endl;
    for (size_t db_idx = 0; db_idx < db_size; ++db_idx) {
        ofs << v[db_idx] << endl;       

        for (size_t i = 0; i < GATE_DELAY_N; ++i) {
            for (size_t j = 0; j < GATE_DELAY_N; ++j) {
                ofs << map2[db_idx][i][j];
                if (j < GATE_DELAY_N - 1) {
                    ofs << ",";
                }
                else {
                    ofs << ";" << endl;
                }
            }
        }
    }

    for (size_t db_idx1 = 0; db_idx1 < db_size1; ++db_idx1) {
        ofs << v[db_idx1] << endl;       // Option 1
        // ofs << gate_name_db[db_idx] << endl;            // Option 2

        for (size_t i = 0; i < GATE_DELAY_N; ++i) {
            for (size_t j = 0; j < GATE_DELAY_N; ++j) {
                ofs << map3[db_idx1][i][j];
                if (j < GATE_DELAY_N - 1) {
                    ofs << ",";
                }
                else {
                    ofs << ";" << endl;
                }
            }
        }
    }
    ofs.close();

    return 0;


}


