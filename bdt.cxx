#include "BDT.h"
using namespace std;

Int_t main(Int_t argc, Char_t* argv[])
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
            cout << "Discard hits with E <= 0:" << endl;
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

        // COG_X
        b->AddVar("COG_X_5_0",          'D');
        b->AddVar("COG_X_5_1",          'D');
        b->AddVar("COG_X_5_2",          'D');
        b->AddVar("COG_X_5_3",          'D');
        b->AddVar("COG_X_5_4",          'D');
        b->AddVar("COG_X_5_5",          'D');
        b->AddVar("COG_X_5_6",          'D');
        b->AddVar("COG_X_5_7",          'D');
        b->AddVar("COG_X_mean",         'D');

        // COG_Y
        b->AddVar("COG_Y_5_0",          'D');
        b->AddVar("COG_Y_5_1",          'D');
        b->AddVar("COG_Y_5_2",          'D');
        b->AddVar("COG_Y_5_3",          'D');
        b->AddVar("COG_Y_5_4",          'D');
        b->AddVar("COG_Y_5_5",          'D');
        b->AddVar("COG_Y_5_6",          'D');
        b->AddVar("COG_Y_5_7",          'D');
        b->AddVar("COG_Y_mean",         'D');

        // COG_Z
        b->AddVar("COG_Z_5_0",          'D');
        b->AddVar("COG_Z_5_1",          'D');
        b->AddVar("COG_Z_5_2",          'D');
        b->AddVar("COG_Z_5_3",          'D');
        b->AddVar("COG_Z_5_4",          'D');
        b->AddVar("COG_Z_5_5",          'D');
        b->AddVar("COG_Z_5_6",          'D');
        b->AddVar("COG_Z_5_7",          'D');
        b->AddVar("COG_Z_mean",         'D');

        b->AddVar("E1E9",               'D');
        b->AddVar("E9E25",              'D');
        b->AddVar("E9E49",              'D');
        b->AddVar("Ecell_max",          'D');
        b->AddVar("Ecell_max_25",       'D');
        b->AddVar("Ecell_max_49",       'D');
        b->AddVar("Ecell_max_9",        'D');
        b->AddVar("Edep",               'D');
        b->AddVar("Emean",              'D');

        // FD_2D
//        b->AddVar("FD_2D_10",           'D');
//        b->AddVar("FD_2D_100",          'D');
//        b->AddVar("FD_2D_110",          'D');
//        b->AddVar("FD_2D_12",           'D');
//        b->AddVar("FD_2D_120",          'D');
//        b->AddVar("FD_2D_130",          'D');
//        b->AddVar("FD_2D_140",          'D');
//        b->AddVar("FD_2D_15",           'D');
//        b->AddVar("FD_2D_150",          'D');
//        b->AddVar("FD_2D_2",            'D');
//        b->AddVar("FD_2D_20",           'D');
//        b->AddVar("FD_2D_3",            'D');
//        b->AddVar("FD_2D_30",           'D');
//        b->AddVar("FD_2D_4",            'D');
//        b->AddVar("FD_2D_40",           'D');
//        b->AddVar("FD_2D_5",            'D');
//        b->AddVar("FD_2D_50",           'D');
//        b->AddVar("FD_2D_6",            'D');
//        b->AddVar("FD_2D_60",           'D');
//        b->AddVar("FD_2D_7",            'D');
//        b->AddVar("FD_2D_70",           'D');
//        b->AddVar("FD_2D_8",            'D');
//        b->AddVar("FD_2D_80",           'D');
//        b->AddVar("FD_2D_9",            'D');
//        b->AddVar("FD_2D_90",           'D');
        b->AddVar("FD_2D_mean",         'D');
        b->AddVar("FD_2D_rms",          'D');

        // FD_3D
//        b->AddVar("FD_3D_10",           'D');
//        b->AddVar("FD_3D_100",          'D');
//        b->AddVar("FD_3D_110",          'D');
//        b->AddVar("FD_3D_12",           'D');
//        b->AddVar("FD_3D_120",          'D');
//        b->AddVar("FD_3D_130",          'D');
//        b->AddVar("FD_3D_140",          'D');
//        b->AddVar("FD_3D_15",           'D');
//        b->AddVar("FD_3D_150",          'D');
//        b->AddVar("FD_3D_2",            'D');
//        b->AddVar("FD_3D_20",           'D');
//        b->AddVar("FD_3D_3",            'D');
//        b->AddVar("FD_3D_30",           'D');
//        b->AddVar("FD_3D_4",            'D');
//        b->AddVar("FD_3D_40",           'D');
//        b->AddVar("FD_3D_5",            'D');
//        b->AddVar("FD_3D_50",           'D');
//        b->AddVar("FD_3D_6",            'D');
//        b->AddVar("FD_3D_60",           'D');
//        b->AddVar("FD_3D_7",            'D');
//        b->AddVar("FD_3D_70",           'D');
//        b->AddVar("FD_3D_8",            'D');
//        b->AddVar("FD_3D_80",           'D');
//        b->AddVar("FD_3D_9",            'D');
//        b->AddVar("FD_3D_90",           'D');
        b->AddVar("FD_3D_mean",         'D');
        b->AddVar("FD_3D_rms",          'D');

        b->AddVar("hit_layer",          'I');
//        b->AddVar("hit_time_mean",      'D');
//        b->AddVar("hit_time_rms",       'D');
        b->AddVar("nhits",              'I');
        b->AddVar("ntrack",             'D');
        b->AddVar("shower_density",     'D');
        b->AddVar("shower_end",         'I');
        b->AddVar("shower_layer",       'I');
        b->AddVar("shower_layer_ratio", 'D');
        b->AddVar("shower_length",      'D');
        b->AddVar("shower_radius",      'D');
        b->AddVar("shower_start",       'I');
//        b->AddVar("shower_time_mean",   'D');
//        b->AddVar("shower_time_rms",    'D');
        b->AddVar("xwidth",             'D');
        b->AddVar("ywidth",             'D');
        b->AddVar("zdepth",             'D');

        // Signal
        b->AddTrainSig("/lustre/collider/chenjiyuan/ahcal-pid/run/geometry/output/x40_y40_layer40/rec_sel_train_pion.root", tree);
        b->AddTestSig ("/lustre/collider/chenjiyuan/ahcal-pid/run/geometry/output/x40_y40_layer40/rec_sel_test_pion.root",  tree);

        // Background
//        b->AddTrainBkg("/lustre/collider/chenjiyuan/ahcal-pid/run/geometry/output/x40_y40_layer40/rec_sel_train_muon.root", tree);
        b->AddTrainBkg("/lustre/collider/chenjiyuan/ahcal-pid/run/geometry/output/x40_y40_layer40/rec_sel_train_e.root",    tree);
//        b->AddTestBkg ("/lustre/collider/chenjiyuan/ahcal-pid/run/geometry/output/x40_y40_layer40/rec_sel_test_muon.root",  tree);
        b->AddTestBkg ("/lustre/collider/chenjiyuan/ahcal-pid/run/geometry/output/x40_y40_layer40/rec_sel_test_e.root",     tree);

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
