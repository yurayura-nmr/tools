#include <string>
#include <iostream>
#include <fstream>
#include <iomanip>

/*

Erik Walinda
Kyoto University
Graduate School of Medicine

Read APBS 1.3 potential (kT/e) files and compute difference map.
Manually remove header and footer lines and keep only numbers for processing.

Example: ./deltaEpot pH5.dx pH6.dx
Suggestion: avoid confusion by putting lower pH first always.

*/

using namespace std;

int process (char *filename_1, char *filename_2) {
    // Open the gromacs-created xvg file. [first pH, second pH] 
    ifstream in1(filename_1);
    ifstream in2(filename_2);
    ofstream out("difference.dx");
    out << "(add header and footer lines before viewing in pymol)" << endl;
    out << "(d1" << right << setw(12) << "d2" << right << setw(12) << "d3)" << right << setw(12) << endl;

    // We want to read time t and distance x(t) from gromacs output
    // For each data point we need three numbers.
    double in1_a1, in1_a2, in1_a3;
    double in2_a1, in2_a2, in2_a3;
   
    // Count the number of lines containing data 
    int timeStep;
    timeStep = 0;
    double diff_a1, diff_a2, diff_a3;

    string qlline, qlline2; // query line to check and skip the gromacs header 

    while (!in1.eof())
        while (!in2.eof())
            while (getline(in1, qlline)) {
                while (getline(in2, qlline2)) {
                    // Read time and distance from filestream into doubles
                    in1 >> in1_a1 >> in1_a2 >> in1_a3;
                    in2 >> in2_a1 >> in2_a2 >> in2_a3;

                    // Calculate difference map 
                    diff_a1 = in1_a1 - in2_a1;
                    diff_a2 = in1_a2 - in2_a2;
                    diff_a3 = in1_a3 - in2_a3;
            
                    // Report
                    //cout << "[Read line 1st pH ]: " << "# " << timeStep << " a1: " << in1_a1 << " a2: " << in1_a2 << " a3: " << in1_a3 << endl;
                    //cout << "[Read line 2nd pH ]: " << "# " << timeStep << " a1: " << in2_a1 << " a2: " << in2_a2 << " a3: " << in2_a3 << endl;
                    //cout << "[ Evaluating line ]: " << "# " << timeStep << " a1: " << diff_a1 << " a2: " << diff_a2 << " a3: " << diff_a3 << endl;
                    out << diff_a1 << setw(12) << diff_a2 << setw(12) << diff_a3 << setw(12) << endl;

                    timeStep++;

                }
	    }

    in1.close();
    in2.close();
    return 0;
}

int main(int argc, char *argv[]) {
  if (argc > 2) {
    process(argv[1], argv[2]); // filename of the xvg file to be processed. 
  }
  else {
    cerr << "Please specify the filename to be processed. [pH3.dx pH6.dx]" << endl;
  }
}
