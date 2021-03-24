#include <string>
#include <iostream>
#include <fstream>

/*

2021/3/5

Erik Walinda
Kyoto University
Graduate School of Medicine

Read gromacs mindist output xvg file and report the fraction of time bound

Last update:
2021/3/8  Command line input
          Can be compiled with nvcc

*/

using namespace std;

int process (char *filename) {
    // Open the File 
    ifstream in(filename);

    // We want to read time and distance from gromacs output
    float time, distance;
    
    // Count the number of dt time steps in the output file 
    int timeStep, bound, unbound;
    timeStep = 0;
    bound = 0;
    unbound = 0;

    // A query string
    string qlline; // query line to check and skip the gromacs header 

    while (!in.eof())
        while (getline(in, qlline)) {
        if (qlline.find("#") == 0) {
            // Skip header
            cout << "(Skipping line): " << qlline << endl;
        }

        else if (qlline.find("@") == 0) {
            // Skip header
            cout << "(Skipping line): " << qlline << endl;
        }

        else {
            // Read time and distance from filestream into floats
            in >> time >> distance;
            cout << "[Evaluating line]: " << "Time: " << time << " Distance: " << distance << endl;
            
            // Count the number of dt frames
            timeStep++;

            // Check if binding occurred
            // Otherwise report non-bound
            // Cutoff at 6 nm for now
            if (distance < 0.6) {
                bound++;
            }
            else if (distance > 0.6) {
                unbound++;
            }
        }
    }

    // Report
    float boundTime;
    float unboundTime;

    boundTime = float(bound) / float(timeStep);
    unboundTime = float(unbound) / float(timeStep);
    
    cout << "[Fraction of time bound:   ] " << boundTime << endl;
    cout << "[Fraction of time unbound: ] " << unboundTime << endl;

    in.close();

    return 0;
}

int main(int argc, char *argv[]) {
  if (argc > 1) {
    process(argv[1]); // filename of the xvg file to be processed. 
  }
  else {
    cerr << "Please specify the filename to be processed. [filename.xvg]" << endl;
  }
}