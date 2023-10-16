#include "BDT.h"

BDT::BDT() {}

BDT::~BDT() {}

Int_t BDT::TrainBDT()
{
    TMVA::Tools::Instance();

    vector<TTree*> trsig;
    vector<TTree*> trbkg;
    vector<TTree*> tesig;
    vector<TTree*> tebkg;

    for (auto i : train_sig)
    {
        TFile* f = TFile::Open(i.first, "READ");
        TTree* t = (TTree*) f->Get(i.second);
        trsig.emplace_back(t);
    }

    for (auto j : train_bkg)
    {
        TFile* f = TFile::Open(j.first, "READ");
        TTree* t = (TTree*) f->Get(j.second);
        trbkg.emplace_back(t);
    }

    for (auto k : test_sig)
    {
        TFile* f = TFile::Open(k.first, "READ");
        TTree* t = (TTree*) f->Get(k.second);
        tesig.emplace_back(t);
    }

    for (auto l : test_bkg)
    {
        TFile* f = TFile::Open(l.first, "READ");
        TTree* t = (TTree*) f->Get(l.second);
        tebkg.emplace_back(t);
    }

    // Create a ROOT output file where ntuples, histograms, etc. are stored
    TString outfileName( "TMVAMulticlass.root" );
    TFile* outputFile = TFile::Open( outfileName, "RECREATE" );

    TMVA::Factory* factory = new TMVA::Factory( "TMVAMulticlass", outputFile,
                                               "!V:!Silent:Color:DrawProgressBar:Transformations=I;D;P;G,D:AnalysisType=multiclass" );

    TMVA::DataLoader* dataloader = new TMVA::DataLoader("dataset");
    for (auto i : var)
        dataloader->AddVariable(i.first, i.second);

    // Signal and background trees should be added here
    for (auto i : trsig)
        dataloader->AddSignalTree(i, 1.0, TMVA::Types::kTraining);
    for (auto j : trbkg)
        dataloader->AddBackgroundTree(j, 1.0, TMVA::Types::kTraining);
    for (auto k : tesig)
        dataloader->AddSignalTree(k, 1.0, TMVA::Types::kTesting);
    for (auto l : tebkg)
        dataloader->AddBackgroundTree(l, 1.0, TMVA::Types::kTesting);

    dataloader->PrepareTrainingAndTestTree( "", "SplitMode=Random:NormMode=NumEvents:!V" );

    factory->BookMethod( dataloader,  TMVA::Types::kBDT, "BDTG", "!H:!V:NTrees=1000:BoostType=Grad:Shrinkage=0.10:UseBaggedBoost:BaggedSampleFraction=0.50:nCuts=20:MaxDepth=2" );

    // Now you can ask the factory to train, test, and evaluate the variables

    // Training, using the set of training events
    factory->TrainAllMethods();

    // Evaluation, using the set of test events
    factory->TestAllMethods();

    // Evaluate and compare performance of all configured MVAs
    factory->EvaluateAllMethods();

    // --------------------------------------------------------------

    // Save the output
    outputFile->Close();

    cout << "==> ROOT file written: " << outputFile->GetName() << endl;
    cout << "==> TMVAClassification finished!" << endl;

    delete factory;
    delete dataloader;

    // Launch the GUI for the root macros
//    if (!gROOT->IsBatch()) TMVA::TMVAGui( outfileName );

    return 0;	
}

Int_t BDT::BDTNtuple(const string& fname, const string& tname)
{
    EnableImplicitMT();
    string outname = fname;
    outname = outname.substr(outname.find_last_of('/') + 1);
    outname = "bdt_" + outname;

    // Loading the library
    TMVA::Tools::Instance();

    // Default MVA methods to be trained + tested
    map<string, Int_t> Use;

    // Cut optimisation
    Use["BDTG"] = 1;
    cout << "==> Start TMVAMulticlassApplication" << endl;
    TMVA::Reader* reader = new TMVA::Reader( "!Color:!Silent" );

    Float_t  bdt_COG_X_5_0;
    Float_t  bdt_COG_X_5_1;
    Float_t  bdt_COG_X_5_2;
    Float_t  bdt_COG_X_5_3;
    Float_t  bdt_COG_X_5_4;
    Float_t  bdt_COG_X_5_5;
    Float_t  bdt_COG_X_5_6;
    Float_t  bdt_COG_X_5_7;
    Float_t  bdt_COG_X_overall;

    Float_t  bdt_COG_Y_5_0;
    Float_t  bdt_COG_Y_5_1;
    Float_t  bdt_COG_Y_5_2;
    Float_t  bdt_COG_Y_5_3;
    Float_t  bdt_COG_Y_5_4;
    Float_t  bdt_COG_Y_5_5;
    Float_t  bdt_COG_Y_5_6;
    Float_t  bdt_COG_Y_5_7;
    Float_t  bdt_COG_Y_overall;

    Float_t  bdt_COG_Z_5_0;
    Float_t  bdt_COG_Z_5_1;
    Float_t  bdt_COG_Z_5_2;
    Float_t  bdt_COG_Z_5_3;
    Float_t  bdt_COG_Z_5_4;
    Float_t  bdt_COG_Z_5_5;
    Float_t  bdt_COG_Z_5_6;
    Float_t  bdt_COG_Z_5_7;
    Float_t  bdt_COG_Z_overall;

    Float_t  bdt_E1E9;
    Float_t  bdt_E9E25;
    Float_t  bdt_E9E49;
    Float_t  bdt_Ecell_max;
    Float_t  bdt_Ecell_max_25;
    Float_t  bdt_Ecell_max_49;
    Float_t  bdt_Ecell_max_9;
    Float_t  bdt_Edep;
    Float_t  bdt_Emean;

//    Float_t  bdt_FD_2D_10;
//    Float_t  bdt_FD_2D_100;
//    Float_t  bdt_FD_2D_110;
//    Float_t  bdt_FD_2D_12;
//    Float_t  bdt_FD_2D_120;
//    Float_t  bdt_FD_2D_130;
//    Float_t  bdt_FD_2D_140;
//    Float_t  bdt_FD_2D_15;
//    Float_t  bdt_FD_2D_150;
//    Float_t  bdt_FD_2D_2;
//    Float_t  bdt_FD_2D_20;
//    Float_t  bdt_FD_2D_3;
//    Float_t  bdt_FD_2D_30;
//    Float_t  bdt_FD_2D_4;
//    Float_t  bdt_FD_2D_40;
//    Float_t  bdt_FD_2D_5;
//    Float_t  bdt_FD_2D_50;
//    Float_t  bdt_FD_2D_6;
//    Float_t  bdt_FD_2D_60;
//    Float_t  bdt_FD_2D_7;
//    Float_t  bdt_FD_2D_70;
//    Float_t  bdt_FD_2D_8;
//    Float_t  bdt_FD_2D_80;
//    Float_t  bdt_FD_2D_9;
//    Float_t  bdt_FD_2D_90;
    Float_t  bdt_FD_2D_mean;
    Float_t  bdt_FD_2D_rms;

//    Float_t  bdt_FD_3D_10;
//    Float_t  bdt_FD_3D_100;
//    Float_t  bdt_FD_3D_110;
//    Float_t  bdt_FD_3D_12;
//    Float_t  bdt_FD_3D_120;
//    Float_t  bdt_FD_3D_130;
//    Float_t  bdt_FD_3D_140;
//    Float_t  bdt_FD_3D_15;
//    Float_t  bdt_FD_3D_150;
//    Float_t  bdt_FD_3D_2;
//    Float_t  bdt_FD_3D_20;
//    Float_t  bdt_FD_3D_3;
//    Float_t  bdt_FD_3D_30;
//    Float_t  bdt_FD_3D_4;
//    Float_t  bdt_FD_3D_40;
//    Float_t  bdt_FD_3D_5;
//    Float_t  bdt_FD_3D_50;
//    Float_t  bdt_FD_3D_6;
//    Float_t  bdt_FD_3D_60;
//    Float_t  bdt_FD_3D_7;
//    Float_t  bdt_FD_3D_70;
//    Float_t  bdt_FD_3D_8;
//    Float_t  bdt_FD_3D_80;
//    Float_t  bdt_FD_3D_9;
//    Float_t  bdt_FD_3D_90;
    Float_t  bdt_FD_3D_mean;
    Float_t  bdt_FD_3D_rms;

    Float_t  bdt_hit_layer;
    Float_t  bdt_hit_time_mean;
    Float_t  bdt_hit_time_rms;
    Float_t  bdt_nhits;
    Float_t  bdt_ntrack;
    Float_t  bdt_shower_density;
    Float_t  bdt_shower_end;
    Float_t  bdt_shower_layer;
    Float_t  bdt_shower_layer_ratio;
    Float_t  bdt_shower_length;
    Float_t  bdt_shower_radius;
    Float_t  bdt_shower_start;
    Float_t  bdt_shower_time_mean;
    Float_t  bdt_shower_time_rms;
    Float_t  bdt_xwidth;
    Float_t  bdt_ywidth;
    Float_t  bdt_zdepth;

    reader->AddVariable("COG_X_5_0",          &bdt_COG_X_5_0);
    reader->AddVariable("COG_X_5_1",          &bdt_COG_X_5_1);
    reader->AddVariable("COG_X_5_2",          &bdt_COG_X_5_2);
    reader->AddVariable("COG_X_5_3",          &bdt_COG_X_5_3);
    reader->AddVariable("COG_X_5_4",          &bdt_COG_X_5_4);
    reader->AddVariable("COG_X_5_5",          &bdt_COG_X_5_5);
    reader->AddVariable("COG_X_5_6",          &bdt_COG_X_5_6);
    reader->AddVariable("COG_X_5_7",          &bdt_COG_X_5_7);
    reader->AddVariable("COG_X_overall",      &bdt_COG_X_overall);

    reader->AddVariable("COG_Y_5_0",          &bdt_COG_X_5_0);
    reader->AddVariable("COG_Y_5_1",          &bdt_COG_X_5_1);
    reader->AddVariable("COG_Y_5_2",          &bdt_COG_X_5_2);
    reader->AddVariable("COG_Y_5_3",          &bdt_COG_X_5_3);
    reader->AddVariable("COG_Y_5_4",          &bdt_COG_X_5_4);
    reader->AddVariable("COG_Y_5_5",          &bdt_COG_X_5_5);
    reader->AddVariable("COG_Y_5_6",          &bdt_COG_X_5_6);
    reader->AddVariable("COG_Y_5_7",          &bdt_COG_X_5_7);
    reader->AddVariable("COG_Y_overall",      &bdt_COG_Y_overall);

    reader->AddVariable("COG_Z_5_0",          &bdt_COG_X_5_0);
    reader->AddVariable("COG_Z_5_1",          &bdt_COG_X_5_1);
    reader->AddVariable("COG_Z_5_2",          &bdt_COG_X_5_2);
    reader->AddVariable("COG_Z_5_3",          &bdt_COG_X_5_3);
    reader->AddVariable("COG_Z_5_4",          &bdt_COG_X_5_4);
    reader->AddVariable("COG_Z_5_5",          &bdt_COG_X_5_5);
    reader->AddVariable("COG_Z_5_6",          &bdt_COG_X_5_6);
    reader->AddVariable("COG_Z_5_7",          &bdt_COG_X_5_7);
    reader->AddVariable("COG_Z_overall",      &bdt_COG_Z_overall);

    reader->AddVariable("E1E9",               &bdt_E1E9);
    reader->AddVariable("E9E25",              &bdt_E9E25);
    reader->AddVariable("E9E49",              &bdt_E9E49);
    reader->AddVariable("Ecell_max",          &bdt_Ecell_max);
    reader->AddVariable("Ecell_max_25",       &bdt_Ecell_max_25);
    reader->AddVariable("Ecell_max_49",       &bdt_Ecell_max_49);
    reader->AddVariable("Ecell_max_9",        &bdt_Ecell_max_9);
    reader->AddVariable("Edep",               &bdt_Edep);
    reader->AddVariable("Emean",              &bdt_Emean);

//    reader->AddVariable("FD_2D_10",           &bdt_FD_2D_10);
//    reader->AddVariable("FD_2D_100",          &bdt_FD_2D_100);
//    reader->AddVariable("FD_2D_110",          &bdt_FD_2D_110);
//    reader->AddVariable("FD_2D_12",           &bdt_FD_2D_12);
//    reader->AddVariable("FD_2D_120",          &bdt_FD_2D_120);
//    reader->AddVariable("FD_2D_130",          &bdt_FD_2D_130);
//    reader->AddVariable("FD_2D_140",          &bdt_FD_2D_140);
//    reader->AddVariable("FD_2D_15",           &bdt_FD_2D_15);
//    reader->AddVariable("FD_2D_150",          &bdt_FD_2D_150);
//    reader->AddVariable("FD_2D_2",            &bdt_FD_2D_2);
//    reader->AddVariable("FD_2D_20",           &bdt_FD_2D_20);
//    reader->AddVariable("FD_2D_3",            &bdt_FD_2D_3);
//    reader->AddVariable("FD_2D_30",           &bdt_FD_2D_30);
//    reader->AddVariable("FD_2D_4",            &bdt_FD_2D_4);
//    reader->AddVariable("FD_2D_40",           &bdt_FD_2D_40);
//    reader->AddVariable("FD_2D_5",            &bdt_FD_2D_5);
//    reader->AddVariable("FD_2D_50",           &bdt_FD_2D_50);
//    reader->AddVariable("FD_2D_6",            &bdt_FD_2D_6);
//    reader->AddVariable("FD_2D_60",           &bdt_FD_2D_60);
//    reader->AddVariable("FD_2D_7",            &bdt_FD_2D_7);
//    reader->AddVariable("FD_2D_70",           &bdt_FD_2D_70);
//    reader->AddVariable("FD_2D_8",            &bdt_FD_2D_8);
//    reader->AddVariable("FD_2D_80",           &bdt_FD_2D_80);
//    reader->AddVariable("FD_2D_9",            &bdt_FD_2D_9);
//    reader->AddVariable("FD_2D_90",           &bdt_FD_2D_90);
    reader->AddVariable("FD_2D_mean",         &bdt_FD_2D_mean);
    reader->AddVariable("FD_2D_rms",          &bdt_FD_2D_rms);

//    reader->AddVariable("FD_3D_10",           &bdt_FD_3D_10);
//    reader->AddVariable("FD_3D_100",          &bdt_FD_3D_100);
//    reader->AddVariable("FD_3D_110",          &bdt_FD_3D_110);
//    reader->AddVariable("FD_3D_12",           &bdt_FD_3D_12);
//    reader->AddVariable("FD_3D_120",          &bdt_FD_3D_120);
//    reader->AddVariable("FD_3D_130",          &bdt_FD_3D_130);
//    reader->AddVariable("FD_3D_140",          &bdt_FD_3D_140);
//    reader->AddVariable("FD_3D_15",           &bdt_FD_3D_15);
//    reader->AddVariable("FD_3D_150",          &bdt_FD_3D_150);
//    reader->AddVariable("FD_3D_2",            &bdt_FD_3D_2);
//    reader->AddVariable("FD_3D_20",           &bdt_FD_3D_20);
//    reader->AddVariable("FD_3D_3",            &bdt_FD_3D_3);
//    reader->AddVariable("FD_3D_30",           &bdt_FD_3D_30);
//    reader->AddVariable("FD_3D_4",            &bdt_FD_3D_4);
//    reader->AddVariable("FD_3D_40",           &bdt_FD_3D_40);
//    reader->AddVariable("FD_3D_5",            &bdt_FD_3D_5);
//    reader->AddVariable("FD_3D_50",           &bdt_FD_3D_50);
//    reader->AddVariable("FD_3D_6",            &bdt_FD_3D_6);
//    reader->AddVariable("FD_3D_60",           &bdt_FD_3D_60);
//    reader->AddVariable("FD_3D_7",            &bdt_FD_3D_7);
//    reader->AddVariable("FD_3D_70",           &bdt_FD_3D_70);
//    reader->AddVariable("FD_3D_8",            &bdt_FD_3D_8);
//    reader->AddVariable("FD_3D_80",           &bdt_FD_3D_80);
//    reader->AddVariable("FD_3D_9",            &bdt_FD_3D_9);
//    reader->AddVariable("FD_3D_90",           &bdt_FD_3D_90);
    reader->AddVariable("FD_3D_mean",         &bdt_FD_3D_mean);
    reader->AddVariable("FD_3D_rms",          &bdt_FD_3D_rms);

    reader->AddVariable("hit_layer",          &bdt_hit_layer);
    reader->AddVariable("hit_time_mean",      &bdt_hit_time_mean);
    reader->AddVariable("hit_time_rms",       &bdt_hit_time_rms);
    reader->AddVariable("nhits",              &bdt_nhits);
    reader->AddVariable("ntrack",             &bdt_ntrack);
    reader->AddVariable("shower_density",     &bdt_shower_density);
    reader->AddVariable("shower_end",         &bdt_shower_end);
    reader->AddVariable("shower_layer",       &bdt_shower_layer);
    reader->AddVariable("shower_layer_ratio", &bdt_shower_layer_ratio);
    reader->AddVariable("shower_length",      &bdt_shower_length);
    reader->AddVariable("shower_radius",      &bdt_shower_radius);
    reader->AddVariable("shower_start",       &bdt_shower_start);
    reader->AddVariable("shower_time_mean",   &bdt_shower_time_mean);
    reader->AddVariable("shower_time_rms",    &bdt_shower_time_rms);
    reader->AddVariable("xwidth",             &bdt_xwidth);
    reader->AddVariable("ywidth",             &bdt_ywidth);
    reader->AddVariable("zdepth",             &bdt_zdepth);

    reader->BookMVA("BDTG method", TString("dataset/weights/TMVAMulticlass_BDTG.weights.xml"));
    cout << "Booked" << endl;

    vector<string> rdf_input = {};

    rdf_input.emplace_back("COG_X_5_0");
    rdf_input.emplace_back("COG_X_5_1");
    rdf_input.emplace_back("COG_X_5_2");
    rdf_input.emplace_back("COG_X_5_3");
    rdf_input.emplace_back("COG_X_5_4");
    rdf_input.emplace_back("COG_X_5_5");
    rdf_input.emplace_back("COG_X_5_6");
    rdf_input.emplace_back("COG_X_5_7");
    rdf_input.emplace_back("COG_X_overall");

    rdf_input.emplace_back("COG_Y_5_0");
    rdf_input.emplace_back("COG_Y_5_1");
    rdf_input.emplace_back("COG_Y_5_2");
    rdf_input.emplace_back("COG_Y_5_3");
    rdf_input.emplace_back("COG_Y_5_4");
    rdf_input.emplace_back("COG_Y_5_5");
    rdf_input.emplace_back("COG_Y_5_6");
    rdf_input.emplace_back("COG_Y_5_7");
    rdf_input.emplace_back("COG_Y_overall");

    rdf_input.emplace_back("COG_Z_5_0");
    rdf_input.emplace_back("COG_Z_5_1");
    rdf_input.emplace_back("COG_Z_5_2");
    rdf_input.emplace_back("COG_Z_5_3");
    rdf_input.emplace_back("COG_Z_5_4");
    rdf_input.emplace_back("COG_Z_5_5");
    rdf_input.emplace_back("COG_Z_5_6");
    rdf_input.emplace_back("COG_Z_5_7");
    rdf_input.emplace_back("COG_Z_overall");

    rdf_input.emplace_back("E1E9");
    rdf_input.emplace_back("E9E25");
    rdf_input.emplace_back("E9E49");
    rdf_input.emplace_back("Ecell_max");
    rdf_input.emplace_back("Ecell_max_25");
    rdf_input.emplace_back("Ecell_max_49");
    rdf_input.emplace_back("Ecell_max_9");
    rdf_input.emplace_back("Edep");
    rdf_input.emplace_back("Emean");

//    rdf_input.emplace_back("FD_2D_10");
//    rdf_input.emplace_back("FD_2D_100");
//    rdf_input.emplace_back("FD_2D_110");
//    rdf_input.emplace_back("FD_2D_12");
//    rdf_input.emplace_back("FD_2D_120");
//    rdf_input.emplace_back("FD_2D_130");
//    rdf_input.emplace_back("FD_2D_140");
//    rdf_input.emplace_back("FD_2D_15");
//    rdf_input.emplace_back("FD_2D_150");
//    rdf_input.emplace_back("FD_2D_2");
//    rdf_input.emplace_back("FD_2D_20");
//    rdf_input.emplace_back("FD_2D_3");
//    rdf_input.emplace_back("FD_2D_30");
//    rdf_input.emplace_back("FD_2D_4");
//    rdf_input.emplace_back("FD_2D_40");
//    rdf_input.emplace_back("FD_2D_5");
//    rdf_input.emplace_back("FD_2D_50");
//    rdf_input.emplace_back("FD_2D_6");
//    rdf_input.emplace_back("FD_2D_60");
//    rdf_input.emplace_back("FD_2D_7");
//    rdf_input.emplace_back("FD_2D_70");
//    rdf_input.emplace_back("FD_2D_8");
//    rdf_input.emplace_back("FD_2D_80");
//    rdf_input.emplace_back("FD_2D_9");
//    rdf_input.emplace_back("FD_2D_90");
    rdf_input.emplace_back("FD_2D_mean");
    rdf_input.emplace_back("FD_2D_rms");

//    rdf_input.emplace_back("FD_3D_10");
//    rdf_input.emplace_back("FD_3D_100");
//    rdf_input.emplace_back("FD_3D_110");
//    rdf_input.emplace_back("FD_3D_12");
//    rdf_input.emplace_back("FD_3D_120");
//    rdf_input.emplace_back("FD_3D_130");
//    rdf_input.emplace_back("FD_3D_140");
//    rdf_input.emplace_back("FD_3D_15");
//    rdf_input.emplace_back("FD_3D_150");
//    rdf_input.emplace_back("FD_3D_2");
//    rdf_input.emplace_back("FD_3D_20");
//    rdf_input.emplace_back("FD_3D_3");
//    rdf_input.emplace_back("FD_3D_30");
//    rdf_input.emplace_back("FD_3D_4");
//    rdf_input.emplace_back("FD_3D_40");
//    rdf_input.emplace_back("FD_3D_5");
//    rdf_input.emplace_back("FD_3D_50");
//    rdf_input.emplace_back("FD_3D_6");
//    rdf_input.emplace_back("FD_3D_60");
//    rdf_input.emplace_back("FD_3D_7");
//    rdf_input.emplace_back("FD_3D_70");
//    rdf_input.emplace_back("FD_3D_8");
//    rdf_input.emplace_back("FD_3D_80");
//    rdf_input.emplace_back("FD_3D_9");
//    rdf_input.emplace_back("FD_3D_90");
    rdf_input.emplace_back("FD_3D_mean");
    rdf_input.emplace_back("FD_3D_rms");

    rdf_input.emplace_back("hit_layer");
    rdf_input.emplace_back("hit_time_mean");
    rdf_input.emplace_back("hit_time_rms");
    rdf_input.emplace_back("nhits");
    rdf_input.emplace_back("ntrack");
    rdf_input.emplace_back("shower_density");
    rdf_input.emplace_back("shower_end");
    rdf_input.emplace_back("shower_layer");
    rdf_input.emplace_back("shower_layer_ratio");
    rdf_input.emplace_back("shower_length");
    rdf_input.emplace_back("shower_radius");
    rdf_input.emplace_back("shower_start");
    rdf_input.emplace_back("shower_time_mean");
    rdf_input.emplace_back("shower_time_rms");
    rdf_input.emplace_back("xwidth");
    rdf_input.emplace_back("ywidth");
    rdf_input.emplace_back("zdepth");

    RDataFrame df(tname, fname);

    auto bdtout = df.Define("Response_Sig", [&]
        (Double_t COG_X_5_0,
         Double_t COG_X_5_1,
         Double_t COG_X_5_2,
         Double_t COG_X_5_3,
         Double_t COG_X_5_4,
         Double_t COG_X_5_5,
         Double_t COG_X_5_6,
         Double_t COG_X_5_7,
         Double_t COG_X_overall,
         Double_t COG_Y_5_0,
         Double_t COG_Y_5_1,
         Double_t COG_Y_5_2,
         Double_t COG_Y_5_3,
         Double_t COG_Y_5_4,
         Double_t COG_Y_5_5,
         Double_t COG_Y_5_6,
         Double_t COG_Y_5_7,
         Double_t COG_Y_overall,
         Double_t COG_Z_5_0,
         Double_t COG_Z_5_1,
         Double_t COG_Z_5_2,
         Double_t COG_Z_5_3,
         Double_t COG_Z_5_4,
         Double_t COG_Z_5_5,
         Double_t COG_Z_5_6,
         Double_t COG_Z_5_7,
         Double_t COG_Z_overall,
         Double_t E1E9,
         Double_t E9E25,
         Double_t E9E49,
         Double_t Ecell_max,
         Double_t Ecell_max_25,
         Double_t Ecell_max_49,
         Double_t Ecell_max_9,
         Double_t Edep,
         Double_t Emean,
//         Double_t FD_2D_10,
//         Double_t FD_2D_100,
//         Double_t FD_2D_110,
//         Double_t FD_2D_12,
//         Double_t FD_2D_120,
//         Double_t FD_2D_130,
//         Double_t FD_2D_140,
//         Double_t FD_2D_15,
//         Double_t FD_2D_150,
//         Double_t FD_2D_2,
//         Double_t FD_2D_20,
//         Double_t FD_2D_3,
//         Double_t FD_2D_30,
//         Double_t FD_2D_4,
//         Double_t FD_2D_40,
//         Double_t FD_2D_5,
//         Double_t FD_2D_50,
//         Double_t FD_2D_6,
//         Double_t FD_2D_60,
//         Double_t FD_2D_7,
//         Double_t FD_2D_70,
//         Double_t FD_2D_8,
//         Double_t FD_2D_80,
//         Double_t FD_2D_9,
//         Double_t FD_2D_90,
         Double_t FD_2D_mean,
         Double_t FD_2D_rms,
//         Double_t FD_3D_10,
//         Double_t FD_3D_100,
//         Double_t FD_3D_110,
//         Double_t FD_3D_12,
//         Double_t FD_3D_120,
//         Double_t FD_3D_130,
//         Double_t FD_3D_140,
//         Double_t FD_3D_15,
//         Double_t FD_3D_150,
//         Double_t FD_3D_2,
//         Double_t FD_3D_20,
//         Double_t FD_3D_3,
//         Double_t FD_3D_30,
//         Double_t FD_3D_4,
//         Double_t FD_3D_40,
//         Double_t FD_3D_5,
//         Double_t FD_3D_50,
//         Double_t FD_3D_6,
//         Double_t FD_3D_60,
//         Double_t FD_3D_7,
//         Double_t FD_3D_70,
//         Double_t FD_3D_8,
//         Double_t FD_3D_80,
//         Double_t FD_3D_9,
//         Double_t FD_3D_90,
         Double_t FD_3D_mean,
         Double_t FD_3D_rms,
         Double_t hit_layer,
         Double_t hit_time_mean,
         Double_t hit_time_rms,
         Int_t    nhits,
         Int_t    ntrack,
         Double_t shower_density,
         Int_t    shower_end,
         Double_t shower_layer,
         Double_t shower_layer_ratio,
         Double_t shower_length,
         Double_t shower_radius,
         Int_t    shower_start,
         Double_t shower_time_mean,
         Double_t shower_time_rms,
         Double_t xwidth,
         Double_t ywidth,
         Double_t zdepth)
    {
        bdt_COG_X_5_0          = COG_X_5_0;
        bdt_COG_X_5_1          = COG_X_5_1;
        bdt_COG_X_5_2          = COG_X_5_2;
        bdt_COG_X_5_3          = COG_X_5_3;
        bdt_COG_X_5_4          = COG_X_5_4;
        bdt_COG_X_5_5          = COG_X_5_5;
        bdt_COG_X_5_6          = COG_X_5_6;
        bdt_COG_X_5_7          = COG_X_5_7;
        bdt_COG_X_overall      = COG_X_overall;

        bdt_COG_Y_5_0          = COG_Y_5_0;
        bdt_COG_Y_5_1          = COG_Y_5_1;
        bdt_COG_Y_5_2          = COG_Y_5_2;
        bdt_COG_Y_5_3          = COG_Y_5_3;
        bdt_COG_Y_5_4          = COG_Y_5_4;
        bdt_COG_Y_5_5          = COG_Y_5_5;
        bdt_COG_Y_5_6          = COG_Y_5_6;
        bdt_COG_Y_5_7          = COG_Y_5_7;
        bdt_COG_Y_overall      = COG_Y_overall;

        bdt_COG_Z_5_0          = COG_Z_5_0;
        bdt_COG_Z_5_1          = COG_Z_5_1;
        bdt_COG_Z_5_2          = COG_Z_5_2;
        bdt_COG_Z_5_3          = COG_Z_5_3;
        bdt_COG_Z_5_4          = COG_Z_5_4;
        bdt_COG_Z_5_5          = COG_Z_5_5;
        bdt_COG_Z_5_6          = COG_Z_5_6;
        bdt_COG_Z_5_7          = COG_Z_5_7;
        bdt_COG_Z_overall      = COG_Z_overall;

        bdt_E1E9               = E1E9;
        bdt_E9E25              = E9E25;
        bdt_E9E49              = E9E49;
        bdt_Ecell_max          = Ecell_max;
        bdt_Ecell_max_25       = Ecell_max_25;
        bdt_Ecell_max_49       = Ecell_max_49;
        bdt_Ecell_max_9        = Ecell_max_9;
        bdt_Edep               = Edep;
        bdt_Emean              = Emean;

//        bdt_FD_2D_10           = FD_2D_10;
//        bdt_FD_2D_100          = FD_2D_100;
//        bdt_FD_2D_110          = FD_2D_110;
//        bdt_FD_2D_12           = FD_2D_12;
//        bdt_FD_2D_120          = FD_2D_120;
//        bdt_FD_2D_130          = FD_2D_130;
//        bdt_FD_2D_140          = FD_2D_140;
//        bdt_FD_2D_15           = FD_2D_15;
//        bdt_FD_2D_150          = FD_2D_150;
//        bdt_FD_2D_2            = FD_2D_2;
//        bdt_FD_2D_20           = FD_2D_20;
//        bdt_FD_2D_3            = FD_2D_3;
//        bdt_FD_2D_30           = FD_2D_30;
//        bdt_FD_2D_4            = FD_2D_4;
//        bdt_FD_2D_40           = FD_2D_40;
//        bdt_FD_2D_5            = FD_2D_5;
//        bdt_FD_2D_50           = FD_2D_50;
//        bdt_FD_2D_6            = FD_2D_6;
//        bdt_FD_2D_60           = FD_2D_60;
//        bdt_FD_2D_7            = FD_2D_7;
//        bdt_FD_2D_70           = FD_2D_70;
//        bdt_FD_2D_8            = FD_2D_8;
//        bdt_FD_2D_80           = FD_2D_80;
//        bdt_FD_2D_9            = FD_2D_9;
//        bdt_FD_2D_90           = FD_2D_90;
        bdt_FD_2D_mean         = FD_2D_mean;
        bdt_FD_2D_rms          = FD_2D_rms;

//        bdt_FD_3D_10           = FD_3D_10;
//        bdt_FD_3D_100          = FD_3D_100;
//        bdt_FD_3D_110          = FD_3D_110;
//        bdt_FD_3D_12           = FD_3D_12;
//        bdt_FD_3D_120          = FD_3D_120;
//        bdt_FD_3D_130          = FD_3D_130;
//        bdt_FD_3D_140          = FD_3D_140;
//        bdt_FD_3D_15           = FD_3D_15;
//        bdt_FD_3D_150          = FD_3D_150;
//        bdt_FD_3D_2            = FD_3D_2;
//        bdt_FD_3D_20           = FD_3D_20;
//        bdt_FD_3D_3            = FD_3D_3;
//        bdt_FD_3D_30           = FD_3D_30;
//        bdt_FD_3D_4            = FD_3D_4;
//        bdt_FD_3D_40           = FD_3D_40;
//        bdt_FD_3D_5            = FD_3D_5;
//        bdt_FD_3D_50           = FD_3D_50;
//        bdt_FD_3D_6            = FD_3D_6;
//        bdt_FD_3D_60           = FD_3D_60;
//        bdt_FD_3D_7            = FD_3D_7;
//        bdt_FD_3D_70           = FD_3D_70;
//        bdt_FD_3D_8            = FD_3D_8;
//        bdt_FD_3D_80           = FD_3D_80;
//        bdt_FD_3D_9            = FD_3D_9;
//        bdt_FD_3D_90           = FD_3D_90;
        bdt_FD_3D_mean         = FD_3D_mean;
        bdt_FD_3D_rms          = FD_3D_rms;

        bdt_hit_layer          = hit_layer;
        bdt_hit_time_mean      = hit_time_mean;
        bdt_hit_time_rms       = hit_time_rms;
        bdt_nhits              = nhits;
        bdt_ntrack             = ntrack;
        bdt_shower_density     = shower_density;
        bdt_shower_end         = shower_end;
        bdt_shower_layer       = shower_layer;
        bdt_shower_layer_ratio = shower_layer_ratio;
        bdt_shower_length      = shower_length;
        bdt_shower_radius      = shower_radius;
        bdt_shower_start       = shower_start;
        bdt_shower_time_mean   = shower_time_mean;
        bdt_shower_time_rms    = shower_time_rms;
        bdt_xwidth             = xwidth;
        bdt_ywidth             = ywidth;
        bdt_zdepth             = zdepth;
        return (reader->EvaluateMulticlass( "BDTG method" ))[0];
    }, rdf_input)
    .Snapshot(tname, outname);
    return 1;
}
