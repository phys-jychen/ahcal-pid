#include "Select.h"

Select::Select() {}

Select::~Select() {}

Int_t Select::ValidHits(const string& file, const string& tree)
{
    DisableImplicitMT();
    RDataFrame* dm = new RDataFrame(tree, file);
    string outname = file;
    outname = outname.substr(outname.find_last_of('/') + 1);
    outname = "sel_" + outname;
    auto fout = dm->Define("nhits", "(Int_t) Hit_X.size()")
    /*
    .Define("Hit_Time_nonzero", [] (vector<Double_t> Hit_Time, vector<Double_t> Hit_Energy, Int_t nhits)
    {
        vector<Double_t> Hit_Time_nonzero;
        for (Int_t i = 0; i < nhits; i++)
            if (Hit_Energy.at(i) > 0)
                Hit_Time_nonzero.emplace_back(Hit_Time.at(i));
        return Hit_Time_nonzero;
    }, {"Hit_Time", "Hit_Energy", "nhits"})
    */
    .Define("Hit_X_nonzero", [] (vector<Double_t> Hit_X, vector<Double_t> Hit_Energy, Int_t nhits)
    {
        vector<Double_t> Hit_X_nonzero;
        for (Int_t i = 0; i < nhits; i++)
            if (Hit_Energy.at(i) > 0)
                Hit_X_nonzero.emplace_back(Hit_X.at(i));
        return Hit_X_nonzero;
    }, {"Hit_X", "Hit_Energy", "nhits"})
    .Define("Hit_Y_nonzero", [] (vector<Double_t> Hit_Y, vector<Double_t> Hit_Energy, Int_t nhits)
    {
        vector<Double_t> Hit_Y_nonzero;
        for (Int_t i = 0; i < nhits; i++)
            if (Hit_Energy.at(i) > 0)
                Hit_Y_nonzero.emplace_back(Hit_Y.at(i));
        return Hit_Y_nonzero;
    }, {"Hit_Y", "Hit_Energy", "nhits"})
    .Define("Hit_Z_nonzero", [] (vector<Double_t> Hit_Z, vector<Double_t> Hit_Energy, Int_t nhits)
    {
        vector<Double_t> Hit_Z_nonzero;
        for (Int_t i = 0; i < nhits; i++)
            if (Hit_Energy.at(i) > 0)
                Hit_Z_nonzero.emplace_back(Hit_Z.at(i));
        return Hit_Z_nonzero;
    }, {"Hit_Z", "Hit_Energy", "nhits"})
    .Define("Hit_Theta_nonzero", [] (vector<Double_t> Hit_X_nonzero, vector<Double_t> Hit_Y_nonzero, vector<Double_t> Hit_Z_nonzero)
    {
        vector<Double_t> theta = {};
        for (Int_t i = 0; i < Hit_X_nonzero.size(); i++)
        {
            if (Hit_Z_nonzero.at(i) == 0)
                theta.emplace_back(PiOver2());
            else
            {
                Double_t rho = Sqrt(Power(Hit_X_nonzero.at(i), 2) + Power(Hit_Y_nonzero.at(i), 2));
                Double_t angle = ATan2(rho, Hit_Z_nonzero.at(i));
                theta.emplace_back(angle);
            }
        }
        return theta;
    }, {"Hit_X_nonzero", "Hit_Y_nonzero", "Hit_Z_nonzero"})
    .Define("Hit_Phi_nonzero", [] (vector<Double_t> Hit_X_nonzero, vector<Double_t> Hit_Y_nonzero)
    {
        vector<Double_t> phi = {};
        for (Int_t i = 0; i < Hit_X_nonzero.size(); i++)
        {
            if (Hit_X_nonzero.at(i) == 0)
            {
                if (Hit_Y_nonzero.at(i) >= 0)
                    phi.emplace_back(0);
                else
                    phi.emplace_back(Pi());
            }
            else
            {
                Double_t angle = ATan2(Hit_Y_nonzero.at(i), Hit_X_nonzero.at(i));
                phi.emplace_back(angle);
            }
        }
        return phi;
    }, {"Hit_X_nonzero", "Hit_Y_nonzero"})
    .Define("Hit_Energy_nonzero", [] (vector<Double_t> Hit_Energy, Int_t nhits)
    {
        vector<Double_t> Hit_Energy_nonzero;
        for (Int_t i = 0; i < nhits; i++)
            if (Hit_Energy.at(i) > 0)
                Hit_Energy_nonzero.emplace_back(Hit_Energy.at(i));
        return Hit_Energy_nonzero;
    }, {"Hit_Energy", "nhits"})
    .Snapshot(tree, outname);
    delete dm;

    TFile* f = new TFile((TString) outname, "READ");
    TTree* t = f->Get<TTree>((TString) tree);
    t->SetBranchStatus("*", 1);
//    vector<TString> deactivate = { "CellID", "Hit_Energy", "Hit_Time", "Hit_X", "Hit_Y", "Hit_Z", "nhits" };
    vector<TString> deactivate = { "CellID", "Hit_Energy", "Hit_X", "Hit_Y", "Hit_Z", "nhits" };
    for (TString de : deactivate)
        t->SetBranchStatus(de, 0);
    TFile* fnew = new TFile((TString) outname, "RECREATE");
    TTree* tnew = t->CloneTree();
    tnew->Write(0, TObject::kWriteDelete, 0);
    f->Close();
    fnew->Close();

    return 1;
}
