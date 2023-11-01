#include "Select.h"

Int_t main(Int_t argc, char* argv[])
{
    string file = "", tree = "Calib_Hit";
    Int_t help = 0;

    for (Int_t i = 1; i < argc; i++)
    {
        if (string(argv[i]) == string("-h") || string(argv[i]) == string("-help"))
        {
            help = 1;
            cout << endl;
            cout << "Help information" << endl << endl;
            cout << "Discard hits with E = 0:" << endl;
            cout << "    With default tree \"Calib_Hit\": iSel -f [file]" << endl;
            cout << "    With other specified tree:     iSel -f [file] -t [tree]" << endl;
            cout << "Reconstruct variables:" << endl;
            cout << "    With default tree \"Calib_Hit\": iRec -f [file]" << endl;
            cout << "    With other specified tree:     iRec -f [file] -t [tree]" << endl;
            cout << "PID with BDT:" << endl;
            cout << "    With default tree \"Calib_Hit\": iBDT -r" << endl;
            cout << "    With other specified tree:     iBDT -r -t [tree]" << endl;
            cout << "Classification:" << endl;
            cout << "    With default tree \"Calib_Hit\": iBDT -v -f [file]" << endl;
            cout << "    With other specified tree:     iBDT -v -f [file] -t [tree]" << endl << endl;
            break;
        }

        else if (string(argv[i]) == string("-f"))
            file = string(argv[i + 1]);

        else if (string(argv[i]) == string("-t"))
            tree = string(argv[i + 1]);
    }

    Select* s = new Select();

    if (file != "")
    {
        cout << "--> Discarding empty hits..." << endl;
        cout << "--> File: " << file << endl;
        cout << "--> Tree: " << tree << endl << endl;

        s->ValidHits(file, tree);

        cout << "--> Selection finished!" << endl;
    }

    else if (help == 0)
    {
        cout << "Invalid input." << endl;
        cout << "Run \"iSel -h[elp]\" to display help information." << endl << endl;
    }

    delete s;
    return 0;
}
