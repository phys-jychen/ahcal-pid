#include "Select.h"
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
            cout << "Discard hits with E = 0: iSel -f [file] -t [tree]" << endl;
            cout << "Reconstruct variables:   iRec -f [file] -t [tree]" << endl;
            cout << "PID with BDT:            iBDT -r -t [tree]" << endl;
            cout << "Classification:          iBDT -v -f [file] -t [tree]" << endl << endl;
            break;
        }

        else if (string(argv[i]) == string("-f"))
            file = string(argv[i + 1]);

        else if (string(argv[i]) == string("-t"))
            tree = string(argv[i + 1]);
    }

    Select* s = new Select();

    if (file != "" && tree != "")
    {
        cout << "-----> Discarding empty hits..." << endl;
        cout << "-----> File: " << file << endl;
        cout << "-----> Tree: " << tree << endl << endl;

        s->ValidHits(file, tree);

        cout << "-----> Selection finished!" << endl;
    }

    else if (help == 0)
    {
        cout << "Invalid input." << endl;
        cout << "Run \"iSel -h[elp]\" to display help information." << endl << endl;
    }

    delete s;
    return 0;
}
