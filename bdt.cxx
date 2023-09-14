#include "BDT.h"
using namespace std;

Int_t main(Int_t argc, char* argv[])
{
    string file = "", tree = "Calib_Hit";
    Int_t train = 0, bdt = 0, help = 0;

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
            cout << "    With default tree \"Calib_Hit\": iBDT -v f [file]" << endl;
            cout << "    With other specified tree:     iBDT -v -f [file] -t [tree]" << endl << endl;
            break;
        }

        else if (string(argv[i]) == string("-f"))
            file = string(argv[i + 1]);

        else if (string(argv[i]) == string("-t"))
            tree = string(argv[i + 1]);

        else if (string(argv[i]) == string("-r"))
            train = 1;

        else if (string(argv[i]) == string("-v"))
            bdt = 1;
    }

    BDT* b = new BDT();

    if (train == 1)
    {
        cout << "----> Training and testing..." << endl;
        cout << "----> Tree: " << tree << endl;

        b->AddVar("COG_X_overall",      'D');
        b->AddVar("COG_Y_overall",      'D');
        b->AddVar("COG_Z_overall",      'D');
        b->AddVar("E1E9",               'D');
        b->AddVar("E9E25",              'D');
        b->AddVar("E9E49",              'D');
        b->AddVar("Edep",               'D');
        b->AddVar("Emean",              'D');
        b->AddVar("FD_2D_mean",         'D');
        b->AddVar("FD_3D_mean",         'D');
        b->AddVar("FD_2D_rms",          'D');
        b->AddVar("FD_3D_rms",          'D');
        b->AddVar("hit_layer",          'D');
//        b->AddVar("hit_time_mean",      'D');
//        b->AddVar("hit_time_rms",       'D');
        b->AddVar("nhits",              'I');
        b->AddVar("ntrack",             'D');
        b->AddVar("shower_density",     'D');
        b->AddVar("shower_end",         'I');
        b->AddVar("shower_layer",       'D');
        b->AddVar("shower_layer_ratio", 'D');
        b->AddVar("shower_length",      'D');
        b->AddVar("shower_radius",      'D');
        b->AddVar("shower_start",       'I');
//        b->AddVar("shower_time_mean",   'D');
//        b->AddVar("shower_time_rms",    'D');
        b->AddVar("xwidth",             'D');
        b->AddVar("ywidth",             'D');
        b->AddVar("zwidth",             'D');

        // Signal
        b->AddTrainSig("/lustre/collider/chenjiyuan/ahcal-pid/run/geometry/output/rec_sel_train_pion.root", tree);
        b->AddTestSig ("/lustre/collider/chenjiyuan/ahcal-pid/run/geometry/output/rec_sel_test_pion.root",  tree);

        // Background
        b->AddTrainBkg("/lustre/collider/chenjiyuan/ahcal-pid/run/geometry/output/rec_sel_train_muon.root", tree);
        b->AddTrainBkg("/lustre/collider/chenjiyuan/ahcal-pid/run/geometry/output/rec_sel_train_e.root",    tree);
        b->AddTestBkg ("/lustre/collider/chenjiyuan/ahcal-pid/run/geometry/output/rec_sel_test_muon.root",  tree);
        b->AddTestBkg ("/lustre/collider/chenjiyuan/ahcal-pid/run/geometry/output/rec_sel_test_e.root",     tree);

        b->TrainBDT();

        cout << "----> Training and testing finished!" << endl;
	}

    else if (bdt == 1 && file != "")
    {
        cout << "----> Classifying..." << endl;
        cout << "----> File: " << file << endl;
        cout << "----> Tree: " << tree << endl << endl;

        b->BDTNtuple(file, tree);

        cout << "----> Classification finished!" << endl;
    }

    else if (help == 0)
    {
        cout << "Invalid input." << endl;
        cout << "Run \"iBDT -h[elp]\" to display help information." << endl << endl;
    }

    delete b;
    return 0;
}
