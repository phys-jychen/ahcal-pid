#include "Variables.h"
using namespace std;

Int_t main(Int_t argc, char* argv[])
{
    string file = "", tree = "";
    Int_t help = 0;

    for (Int_t i = 1; i < argc; i++)
    {
        if (string(argv[i]) == string("-h") || string(argv[i]) == string("-help"))
        {
            help = 1;
            cout << endl;
            cout << "Help information" << endl << endl;
            cout << "Reconstruct variables: iRec -f [file] -t [tree]" << endl;
            cout << "PID with BDT:          iBDT -t [tree] -train" << endl;
            cout << "Classification:        iBDT -v -f [file] -t [tree]" << endl << endl;
            break;
        }

        else if (string(argv[i]) == string("-f"))
            file = string(argv[i + 1]);

        else if (string(argv[i]) == string("-t"))
            tree = string(argv[i + 1]);
    }

    Variables* v = new Variables();

    if (file != "" && tree != "")
    {
        cout << "Reconstructing variables..." << endl;
        cout << "File: " << file << endl;
        cout << "Tree: " << tree << endl << endl;

        v->GenNtuple(file, tree);
    }

    else if (help == 0)
    {
        cout << "Invalid input." << endl;
        cout << "Run \"iRec -h\" to display help information." << endl << endl;
    }

    delete v;
    return 0;
}
