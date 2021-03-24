#include <string>
#include <iostream>
#include <fstream>

/*

Erik Walinda
Kyoto University
Graduate School of Medicine

Read gromacs gmx mindist output distance xvg file and report the fraction of that group 1 was bound to group 2.
Cut-off distance 0.6 nm.

*/

using namespace std;

int process (char *filename) {
    // Open the gromacs-created xvg file. 
    ifstream in(filename);

    // We want to read time t and distance x(t) from gromacs output
    float time, distance;
    
    // Count the number of dt time steps in the output file 
    int timeStep, bound, unbound;
    timeStep = 0;
    bound = 0;
    unbound = 0;

    string qlline; // query line to check and skip the gromacs header 

    while (!in.eof())
        while (getline(in, qlline)) {
        if (qlline.find("#") == 0) {
            // Skip header lines starting with #
            cout << "(Skipping line): " << qlline << endl;
        }

        else if (qlline.find("@") == 0) {
            // Skip header lines starting with @
            cout << "(Skipping line): " << qlline << endl;
        }

        else {
            // Read time and distance from filestream into floats
            in >> time >> distance;
            cout << "[Evaluating line]: " << "Time: " << time << " Distance: " << distance << endl;
            
            // Count the number of dt frames
            timeStep++;

            // Check if binding occurred. Otherwise not bound. Cutoff at 0.6 nm for now.
            if (distance <= 0.6) {
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

    boundTime   = float(bound)   / float(timeStep);
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
