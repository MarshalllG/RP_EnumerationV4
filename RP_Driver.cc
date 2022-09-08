#include "RP_Enumeration.hh"
#include <iostream>
#include <cstdlib>

int main (int argc, char* argv[])
{
    string instance;
    if (argc == 1)
    {
        cout << "Name of the file containing the instance: ";
        cin >> instance;
    }
    else if (argc == 2)
        instance = argv[1];
    else
    {
        cerr << "Usage: " << argv[0] << " [input_file]" << endl;
        exit(1);
    }

    RP_Input in(instance);
    // cout << in;
    EnumerationRPOpt solver(in);

    if (solver.Search())
    {
        cout << "Best solution found " << endl;
        cout << solver.BestSolution()
            << " after  " << solver.NumSol() 
            << " iterations" << endl;
    }
    else
        cout << "No solution exists" << endl;

    return 0;
}