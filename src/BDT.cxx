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
    //if (!gROOT->IsBatch()) TMVA::TMVAGui( outfileName );

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
    Float_t  bdt_COG_X_overall;
    Float_t  bdt_COG_Y_overall;
    Float_t  bdt_COG_Z_overall;
//    Float_t  bdt_E1E9;
//    Float_t  bdt_E9E25;
//    Float_t  bdt_E9E49;
    Float_t  bdt_Edep;
    Float_t  bdt_Emean;
//    Float_t  bdt_FD_2D;
//    Float_t  bdt_FD_3D;
    Float_t  bdt_hit_layer;
//    Float_t  bdt_hit_time_mean;
//    Float_t  bdt_hit_time_rms;
    Float_t  bdt_nhits;
    Float_t  bdt_ntrack;
    Float_t  bdt_shower_density;
    Float_t  bdt_shower_end;
    Float_t  bdt_shower_layer;
    Float_t  bdt_shower_layer_ratio;
    Float_t  bdt_shower_length;
    Float_t  bdt_shower_radius;
    Float_t  bdt_shower_start;
//    Float_t  bdt_shower_time_mean;
//    Float_t  bdt_shower_time_rms;
    Float_t  bdt_xwidth;
    Float_t  bdt_ywidth;
    Float_t  bdt_zwidth;

    reader->AddVariable("COG_X_overall",      &bdt_COG_X_overall);
    reader->AddVariable("COG_Y_overall",      &bdt_COG_Y_overall);
    reader->AddVariable("COG_Z_overall",      &bdt_COG_Z_overall);
//    reader->AddVariable("E1E9",               &bdt_E1E9);
//    reader->AddVariable("E9E25",              &bdt_E9E25);
//    reader->AddVariable("E9E49",              &bdt_E9E49);
    reader->AddVariable("Edep",               &bdt_Edep);
    reader->AddVariable("Emean",              &bdt_Emean);
//    reader->AddVariable("FD_2D",              &bdt_FD_2D);
//    reader->AddVariable("FD_3D",              &bdt_FD_3D);
    reader->AddVariable("hit_layer",          &bdt_hit_layer);
//    reader->AddVariable("hit_time_mean",      &bdt_hit_time_mean);
//    reader->AddVariable("hit_time_rms",       &bdt_hit_time_rms);
    reader->AddVariable("nhits",              &bdt_nhits);
    reader->AddVariable("ntrack",             &bdt_ntrack);
    reader->AddVariable("shower_density",     &bdt_shower_density);
    reader->AddVariable("shower_end",         &bdt_shower_end);
    reader->AddVariable("shower_layer",       &bdt_shower_layer);
    reader->AddVariable("shower_layer_ratio", &bdt_shower_layer_ratio);
    reader->AddVariable("shower_length",      &bdt_shower_length);
    reader->AddVariable("shower_radius",      &bdt_shower_radius);
    reader->AddVariable("shower_start",       &bdt_shower_start);
//    reader->AddVariable("shower_time_mean",   &bdt_shower_time_mean);
//    reader->AddVariable("shower_time_rms",    &bdt_shower_time_rms);
    reader->AddVariable("xwidth",             &bdt_xwidth);
    reader->AddVariable("ywidth",             &bdt_ywidth);
    reader->AddVariable("zwidth",             &bdt_zwidth);

    reader->BookMVA("BDTG method", TString("dataset/weights/TMVAMulticlass_BDTG.weights.xml"));
    cout << "Booked" << endl;

    vector<string> rdf_input = {};
    rdf_input.emplace_back("COG_X_overall");
    rdf_input.emplace_back("COG_Y_overall");
    rdf_input.emplace_back("COG_Z_overall");
//    rdf_input.emplace_back("E1E9");
//    rdf_input.emplace_back("E9E25");
//    rdf_input.emplace_back("E9E49");
    rdf_input.emplace_back("Edep");
    rdf_input.emplace_back("Emean");
//    rdf_input.emplace_back("FD_2D");
//    rdf_input.emplace_back("FD_3D");
    rdf_input.emplace_back("hit_layer");
//    rdf_input.emplace_back("hit_time_mean");
//    rdf_input.emplace_back("hit_time_rms");
    rdf_input.emplace_back("nhits");
    rdf_input.emplace_back("ntrack");
    rdf_input.emplace_back("shower_density");
    rdf_input.emplace_back("shower_end");
    rdf_input.emplace_back("shower_layer");
    rdf_input.emplace_back("shower_layer_ratio");
    rdf_input.emplace_back("shower_length");
    rdf_input.emplace_back("shower_radius");
    rdf_input.emplace_back("shower_start");
//    rdf_input.emplace_back("shower_time_mean");
//    rdf_input.emplace_back("shower_time_rms");
    rdf_input.emplace_back("xwidth");
    rdf_input.emplace_back("ywidth");
    rdf_input.emplace_back("zwidth");

    RDataFrame df(tname, fname);

    auto bdtout = df.Define("Response_sig", [&]
        (Double_t COG_X_overall,
         Double_t COG_Y_overall,
         Double_t COG_Z_overall,
//         Double_t E1E9,
//         Double_t E9E25,
//         Double_t E9E49,
         Double_t Edep,
         Double_t Emean,
//         Double_t FD_2D,
//         Double_t FD_3D,
         Double_t hit_layer,
//         Double_t hit_time_mean,
//         Double_t hit_time_rms,
         Int_t    nhits,
         Int_t    ntrack,
         Double_t shower_density,
         Int_t    shower_end,
         Double_t shower_layer,
         Double_t shower_layer_ratio,
         Double_t shower_length,
         Double_t shower_radius,
         Int_t    shower_start,
//         Double_t shower_time_mean,
//         Double_t shower_time_rms,
         Double_t xwidth,
         Double_t ywidth,
         Double_t zwidth)
    {
        bdt_COG_X_overall      = COG_X_overall;
        bdt_COG_Y_overall      = COG_Y_overall;
        bdt_COG_Z_overall      = COG_Z_overall;
//        bdt_E1E9               = E1E9;
//        bdt_E9E25              = E9E25;
//        bdt_E9E49              = E9E49;
        bdt_Edep               = Edep;
        bdt_Emean              = Emean;
//        bdt_FD_2D              = FD_2D;
//        bdt_FD_3D              = FD_3D;
        bdt_hit_layer          = hit_layer;
//        bdt_hit_time_mean      = hit_time_mean;
//        bdt_hit_time_rms       = hit_time_rms;
        bdt_nhits              = nhits;
        bdt_ntrack             = ntrack;
        bdt_shower_density     = shower_density;
        bdt_shower_end         = shower_end;
        bdt_shower_layer       = shower_layer;
        bdt_shower_layer_ratio = shower_layer_ratio;
        bdt_shower_length      = shower_length;
        bdt_shower_radius      = shower_radius;
        bdt_shower_start       = shower_start;
//        bdt_shower_time_mean   = shower_time_mean;
//        bdt_shower_time_rms    = shower_time_rms;
        bdt_xwidth             = xwidth;
        bdt_ywidth             = ywidth;
        bdt_zwidth             = zwidth;
        return (reader->EvaluateMulticlass( "BDTG method" ))[0];
    }, rdf_input)
    .Snapshot(tname, outname);
    return 1;
}
