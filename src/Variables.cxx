#include "Variables.h"

Int_t NHScaleV2(RVec<Double_t> const& pos_x, RVec<Double_t> const& pos_y, RVec<Double_t> const& pos_z, Int_t const& RatioX, Int_t const& RatioY, Int_t const& RatioZ)
{
    Int_t ReScaledNH = 0;
    Int_t tmpI = 0;
    Int_t tmpJ = 0;
    Int_t tmpK = 0;
    Double_t tmpEn = 0;
    Int_t NewCellID0 = 0;
    const Double_t Bias = -342.55;
    const Double_t Width = 40.3;
    const Double_t Thick = 30;
    const Int_t NumHit = pos_x.size();

    std::map<Double_t, Double_t> testIDtoEnergy;

    for (Int_t i = 0; i < NumHit; i++)
    {
        Double_t x = pos_x.at(i);
        Double_t y = pos_y.at(i);
        Double_t z = pos_z.at(i);

        tmpI = (Int_t ((x - Bias) / Width) + Int_t(Abs(x) / x)) / RatioX;
        tmpJ = (Int_t ((y - Bias) / Width) + Int_t(Abs(y) / y)) / RatioY;
        tmpK = (Int_t) (z / Thick) / RatioZ;
        tmpEn = 1;

        NewCellID0 = (tmpK << 24) + (tmpJ << 12) + tmpI;

        if (testIDtoEnergy.find(NewCellID0) == testIDtoEnergy.end())
            testIDtoEnergy[NewCellID0] = tmpEn;
        else
            testIDtoEnergy[NewCellID0] += tmpEn;
    }

    ReScaledNH = testIDtoEnergy.size();
    return ReScaledNH;
}

Variables::Variables() {}

Variables::~Variables() {}

Int_t Variables::GenNtuple(const string& file, const string& tree)
{
    const Int_t nlayer = 40;
    const Int_t thick = 30;
    //EnableImplicitMT();
    DisableImplicitMT();
    RDataFrame* dm = new RDataFrame(tree, file);
    string outname = file;
    outname = outname.substr(outname.find_last_of('/') + 1);
    outname = "pid_" + outname;
    auto fout = dm->Define("nhits", "(Int_t) Hit_X_nonzero.size()")
    // Theta of the hits [spherical co-ordinate, origin = (0, 0, 0)]
    .Define("Hit_Theta", [] (vector<Double_t> Hit_X_nonzero, vector<Double_t> Hit_Y_nonzero, vector<Double_t> Hit_Z_nonzero, Int_t nhits)
    {
        vector<Double_t> theta = {};
        for (Int_t i = 0; i < nhits; i++)
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
    }, {"Hit_X_nonzero", "Hit_Y_nonzero", "Hit_Z_nonzero", "nhits"})
    // Phi of the hits [spherical co-ordinate, origin = (0, 0, 0)]
    .Define("Hit_Phi", [] (vector<Double_t> Hit_X_nonzero, vector<Double_t> Hit_Y_nonzero, Int_t nhits)
    {
        vector<Double_t> phi = {};
        for (Int_t i = 0; i < nhits; i++)
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
    }, {"Hit_X_nonzero", "Hit_Y_nonzero", "nhits"})
    // Transfer z position to the layer number
    .Define("layer", [] (vector<Double_t> Hit_Z_nonzero, Int_t nhits)
    {
        vector<Int_t> layer = {};
        for (Int_t i = 0; i < nhits; i++)
            layer.emplace_back((Int_t) round(Hit_Z_nonzero.at(i) / thick));
        return layer;
    }, {"Hit_Z_nonzero", "nhits"})
    // Number of hits with non-zero energy deposition on each layer
    .Define("hits_on_layer", [] (vector<Int_t> layer, vector<Double_t> Hit_Energy_nonzero, Int_t nhits)
    {
        vector<Int_t> hits_on_layer(nlayer);
        for (Int_t i = 0; i < nhits; i++)
        {
            Int_t ilayer = layer.at(i);
            hits_on_layer.at(ilayer)++;
        }
        return hits_on_layer;
    }, {"layer", "Hit_Energy_nonzero", "nhits"})
    // Energy deposition on each layer
    .Define("layer_energy", [] (vector<Int_t> layer, vector<Double_t> Hit_Energy_nonzero, Int_t nhits)
    {
        vector<Double_t> layer_energy(nlayer);
        for (Int_t i = 0; i < nhits; i++)
            layer_energy.at(layer.at(i)) += Hit_Energy_nonzero.at(i);
        return layer_energy;
    }, {"layer", "Hit_Energy_nonzero", "nhits"})
    // RMS width in x direction
    .Define("xwidth", [] (vector<Double_t> Hit_X_nonzero)
    {
        TH1D* h1 = new TH1D("h1", "", 100, -400, 400);
        for (Double_t i : Hit_X_nonzero)
            h1->Fill(i);
        Double_t xwidth = h1->GetRMS();
        delete h1;
        return xwidth;
    }, {"Hit_X_nonzero"})
    // RMS width in y direction
    .Define("ywidth", [] (vector<Double_t> Hit_Y_nonzero)
    {
        TH1D* h1 = new TH1D("h1", "", 100, -400, 400);
        for (Double_t i : Hit_Y_nonzero)
            h1->Fill(i);
        Double_t ywidth = h1->GetRMS();
        delete h1;
        return ywidth;
    }, {"Hit_Y_nonzero"})
    // RMS depth
    .Define("zwidth", [] (vector<Double_t> Hit_Z_nonzero)
    {
        TH1D* h1 = new TH1D("h1", "", 100, 0, 1200);
        for (Double_t i : Hit_Z_nonzero)
            h1->Fill(i);
        Double_t zwidth = h1->GetRMS();
        delete h1;
        return zwidth;
    }, {"Hit_Z_nonzero"})
    // Total energy deposition
    .Define("Edep", [] (vector<Double_t> Hit_Energy_nonzero)
    {
        Double_t sum = 0;
        for (Double_t i : Hit_Energy_nonzero)
            sum += i;
        return sum;
    }, {"Hit_Energy_nonzero"})
    // Average energy deposition of the hits
    .Define("Emean", "Edep / nhits")
    // RMS value of the positions of all the hits on a layer
    .Define("layer_rms", [] (vector<Double_t> Hit_X_nonzero, vector<Double_t> Hit_Y_nonzero, vector<Int_t> layer, vector<Double_t> Hit_Energy_nonzero, Int_t nhits)->vector<Double_t>
    {
        vector<Double_t> layer_rms(nlayer);
        vector<TH2D*> hvec;
        for (Int_t i = 0; i < nlayer; i++)
            hvec.emplace_back(new TH2D("h" + TString(to_string(i)) + "_rms", "Layer RMS", 100, -400, 400, 100, -400, 400));
        for (Int_t i = 0; i < nhits; i++)
        {
            Int_t ilayer = layer.at(i);
            hvec.at(ilayer)->Fill(Hit_X_nonzero.at(i), Hit_Y_nonzero.at(i), Hit_Energy_nonzero.at(i));
        }
        for (Int_t i = 0; i < hvec.size(); i++)
        {
            if (hvec.at(i)->GetEntries() < 4)
                layer_rms.at(i) = 0.0;
            else
                layer_rms.at(i) = hvec.at(i)->GetRMS();
            delete hvec.at(i);
        }
        vector<TH2D*>().swap(hvec);
        return layer_rms;
    }, {"Hit_X_nonzero", "Hit_Y_nonzero", "layer", "Hit_Energy_nonzero", "nhits"})
    // The layer where the shower begins; otherwise it is set to be 42
    .Define("shower_start", [] (vector<Int_t> hits_on_layer, vector<Double_t> layer_rms)
    {
        Int_t shower_start = nlayer + 2;
        for (Int_t i = 0; i < nlayer - 3; i++)
            if (hits_on_layer.at(i) >= 4 && hits_on_layer.at(i + 1) >= 4 && hits_on_layer.at(i + 2) >= 4 && hits_on_layer.at(i + 3) >= 4)
            {
                shower_start = i;
                break;
            }
        return shower_start;
    }, {"hits_on_layer", "layer_rms"})
    // The layer where the shower ends
    .Define("shower_end", [] (vector<Int_t> hits_on_layer, vector<Double_t> layer_rms, Int_t shower_start)
    {
        Int_t shower_end = nlayer + 2;
        if (shower_start == nlayer + 2)
            return shower_end;
        for (Int_t i = shower_start; i < hits_on_layer.size() - 3; i++)
            if (hits_on_layer.at(i) < 4 && hits_on_layer.at(i + 1) < 4)
            {
                shower_end = i;
                break;
            }
        if (shower_end == nlayer + 2)
            shower_end = nlayer;
        return shower_end;
    }, {"hits_on_layer", "layer_rms", "shower_start"})
    // Shower radius of the shower (between beginning and ending layers)
    .Define("shower_radius", [] (vector<Double_t> Hit_X_nonzero, vector<Double_t> Hit_Y_nonzero, vector<Int_t> layer, Int_t beginning, Int_t ending, Int_t nhits)
    {
        Double_t d2 = 0;
        Int_t hits = 0;
        for (Int_t i = 0; i < nhits; i++)
            if (layer.at(i) >= beginning && layer.at(i) < ending)
            {
                hits++;
                d2 += Power(Hit_X_nonzero.at(i), 2) + Power(Hit_Y_nonzero.at(i), 2);
            }
        if (hits == 0)
            return 0.0;
        Double_t radius = Sqrt(d2 / hits);
        return radius;
    }, {"Hit_X_nonzero", "Hit_Y_nonzero", "layer", "shower_start", "shower_end", "nhits"})
    // RMS value of the shower in x direction
    .Define("layer_xwidth", [] (vector<Double_t> Hit_X_nonzero, vector<Int_t> layer, Int_t nhits)
    {
        vector<Double_t> layer_xwidth(nlayer);
        vector<TH1D*> h;
        for (Int_t l = 0; l < nlayer; l++)
            h.emplace_back(new TH1D(TString("h") + TString(to_string(l)), "test", 100, -400, 400));
        for (Int_t i = 0; i < nhits; i++)
        {
            Int_t ilayer = layer.at(i);
            h.at(ilayer)->Fill(Hit_X_nonzero.at(i));
        }
        for (Int_t i = 0; i < h.size(); i++)
        {
            layer_xwidth.at(i) = h.at(i)->GetRMS();
            delete h.at(i);
        }
        vector<TH1D*>().swap(h);
        return layer_xwidth;
    }, {"Hit_X_nonzero", "layer", "nhits"})
    // RMS value of the shower in y direction
    .Define("layer_ywidth", [] (vector<Double_t> Hit_Y_nonzero, vector<Int_t> layer, Int_t nhits)
    {
        vector<Double_t> layer_ywidth(nlayer);
        vector<TH1D*> h;
        for (Int_t l = 0; l < nlayer; l++)
            h.emplace_back(new TH1D(TString("h") + TString(to_string(l)), "test", 100, -400, 400));
        for (Int_t i = 0; i < nhits; i++)
        {
            Int_t ilayer = layer.at(i);
            h.at(ilayer)->Fill(Hit_Y_nonzero.at(i));
        }
        for (Int_t i = 0; i < h.size(); i++)
        {
            layer_ywidth.at(i) = h.at(i)->GetRMS();
            delete h.at(i);
        }
        vector<TH1D*>().swap(h);
        return layer_ywidth;
    }, {"Hit_Y_nonzero", "layer", "nhits"})
    // Number of layers with xwidth, ywidth >= 60 mm
    .Define("shower_layer", [] (vector<Double_t> layer_xwidth, vector<Double_t> layer_ywidth)
    {
        Double_t shower_layer = 0;
        for (Int_t i = 0; i < nlayer; i++)
            if (layer_xwidth.at(i) >= 60 && layer_ywidth.at(i) >= 60)
                shower_layer++;
        return shower_layer;
    }, {"layer_xwidth", "layer_ywidth"})
    // Number of layers with at least one hit
    .Define("hit_layer", [] (vector<Int_t> layer)
    {
        Double_t hit_layer = 0;
        unordered_map<Int_t, Int_t> map_layer_hit;
        for (Double_t i : layer)
            map_layer_hit[i]++;
        for (Int_t i = 0; i < nlayer; i++)
            if (map_layer_hit.count(i) > 0)
                hit_layer++;
        return hit_layer;
    }, {"layer"})
    // The proportion of layers with xwidth, ywidth >= 60 mm within the layers with at least one hit
    .Define("shower_layer_ratio", "shower_layer / hit_layer")
    // Average number of hits in the 3*3 cells around a given one
    .Define("shower_density", [] (vector<Double_t> Hit_X_nonzero, vector<Double_t> Hit_Y_nonzero, vector<Int_t> layer, vector<Double_t> Hit_Energy_nonzero, Int_t nhits)
    {
        const Double_t bias = 342.55;
        const Double_t width = 40.3;
        Double_t shower_density = 0.0;
        unordered_map<Int_t, Int_t> map_CellID;
        for (Int_t j = 0; j < nhits; j++)
        {
            Int_t x = round((Hit_X_nonzero.at(j) + bias) / width);
            Int_t y = round((Hit_Y_nonzero.at(j) + bias) / width);
            Int_t z = layer.at(j);
            Int_t index = z * 100000 + x * 100 + y;
            map_CellID[index] += 1;
        }
        for (Int_t i = 0; i < nhits; i++)
        {
            if (Hit_Energy_nonzero.at(i) == 0.0)
                continue;
            Int_t x = round((Hit_X_nonzero.at(i) + bias) / width);
            Int_t y = round((Hit_Y_nonzero.at(i) + bias) / width);
            Int_t z = layer.at(i);
            for (Int_t ix = x - 1; ix <= x + 1; ix++)
            {
                if (ix < 0 || ix > 17)
                    continue;
                for (Int_t iy = y - 1; iy <= y + 1; iy++)
                {
                    if (iy < 0 || iy > 17)
                        continue;
                    Int_t tmp = z * 100000 + ix * 100 + iy;
                    shower_density += map_CellID[tmp];
                }
            }
        }
        shower_density /= nhits;
        return shower_density;
    }, {"Hit_X_nonzero", "Hit_Y_nonzero", "layer", "Hit_Energy_nonzero", "nhits"})
    // Energy deposition of the central cell divided by the total energy deposition in the 3*3 cells around it
    .Define("E1E9", [] (vector<Double_t> Hit_X_nonzero, vector<Double_t> Hit_Y_nonzero, vector<Int_t> layer, vector<Double_t> Hit_Energy_nonzero, Int_t nhits)
    {
        if (nhits == 0)
            return 0.0;
        const Double_t bias = 342.55;
        const Double_t width = 40.3;
        Double_t E1E9 = 0.0;
        unordered_map<Int_t, Double_t> map_cellid;
        for (Int_t i = 0; i < nhits; i++)
        {
            Int_t x = round((Hit_X_nonzero.at(i) + bias) / width);
            Int_t y = round((Hit_Y_nonzero.at(i) + bias) / width);
            Int_t z = layer.at(i);
            Int_t index = z * 100000 + x * 100 + y;
            map_cellid[index] += Hit_Energy_nonzero.at(i);
        }
        for (Int_t j = 0; j < nhits; j++)
        {
            if (Hit_Energy_nonzero.at(j) == 0.0)
                continue;
            Int_t x = round((Hit_X_nonzero.at(j) + bias) / width);
            Int_t y = round((Hit_Y_nonzero.at(j) + bias) / width);
            Int_t z = layer.at(j);
            Int_t index = z * 100000 + x * 100 + y;
            Double_t tempE1 = map_cellid[index];
            Double_t tempE9 = map_cellid[index];
            for (Int_t ix = x - 1; ix <= x + 1; ix++)
            {
                if (ix < 0 || ix > 17)
                    continue;
                for (Int_t iy = y - 1; iy <= y + 1; iy++)
                {
                    if (iy < 0 || iy > 17)
                        continue;
                    Int_t tmp = z * 100000 + ix * 100 + iy;
                    tempE9 += map_cellid[tmp];
                }
            }
            E1E9 += tempE1 / tempE9;
        }
        E1E9 /= nhits;
        return E1E9;
    }, {"Hit_X_nonzero", "Hit_Y_nonzero", "layer", "Hit_Energy_nonzero", "nhits"})
    // Energy deposition of the central 3*3 cells divided by the total energy deposition in the 5*5 cells around it
    .Define("E9E25", [] (vector<Double_t> Hit_X_nonzero, vector<Double_t> Hit_Y_nonzero, vector<Int_t> layer, vector<Double_t> Hit_Energy_nonzero, Int_t nhits)
    {
        if (nhits == 0)
            return 0.0;
        const Double_t bias = 342.55;
        const Double_t width = 40.3;
        Double_t E9E25 = 0.0;
        unordered_map<Int_t, Double_t> map_cellid;
        for (Int_t i = 0; i < nhits; i++)
        {
            Int_t x = round((Hit_X_nonzero.at(i) + bias) / width);
            Int_t y = round((Hit_Y_nonzero.at(i) + bias) / width);
            Int_t z = layer.at(i);
            Int_t index = z * 100000 + x * 100 + y;
            map_cellid[index] += Hit_Energy_nonzero.at(i);
        }
        for (Int_t j = 0; j < nhits; j++)
        {
            if (Hit_Energy_nonzero.at(j) == 0.0)
                continue;
            Int_t x = round((Hit_X_nonzero.at(j) + bias) / width);
            Int_t y = round((Hit_Y_nonzero.at(j) + bias) / width);
            Int_t z = layer.at(j);
            Int_t index = z * 100000 + x * 100 + y;
            Double_t tempE9 = map_cellid[index];
            Double_t tempE25 = map_cellid[index];
            for (Int_t ix = x - 2; ix <= x + 2; ix++)
            {
                if (ix < 0 || ix > 17)
                    continue;
                for (Int_t iy = y - 2; iy <= y + 2; iy++)
                {
                    if (iy < 0 || iy > 17)
                        continue;
                    Int_t tmp = z * 100000 + ix * 100 + iy;
                    if (ix >= x - 1 && ix <= x + 1 && iy >= y - 1 && iy <= y + 1)
                        tempE9 += map_cellid[tmp];
                    tempE25 += map_cellid[tmp];
                }
            }
            E9E25 += tempE9 / tempE25;
        }
        E9E25 /= nhits;
        return E9E25;
    }, {"Hit_X_nonzero", "Hit_Y_nonzero", "layer", "Hit_Energy_nonzero", "nhits"})
    // Energy deposition of the central 3*3 cells divided by the total energy deposition in the 7*7 cells around it
    .Define("E9E49", [] (vector<Double_t> Hit_X_nonzero, vector<Double_t> Hit_Y_nonzero, vector<Int_t> layer, vector<Double_t> Hit_Energy_nonzero, Int_t nhits)
    {
        if (nhits == 0)
            return 0.0;
        const Double_t bias = 342.55;
        const Double_t width = 40.3;
        Double_t E9E49 = 0.0;
        unordered_map<Int_t, Double_t> map_cellid;
        for (Int_t i = 0; i < nhits; i++)
        {
            Int_t x = round((Hit_X_nonzero.at(i) + bias) / width);
            Int_t y = round((Hit_Y_nonzero.at(i) + bias) / width);
            Int_t z = layer.at(i);
            Int_t index = z * 100000 + x * 100 + y;
            map_cellid[index] += Hit_Energy_nonzero.at(i);
        }
        for (Int_t j = 0; j < nhits; j++)
        {
            if (Hit_Energy_nonzero.at(j) == 0.0)
                continue;
            Int_t x = round((Hit_X_nonzero.at(j) + bias) / width);
            Int_t y = round((Hit_Y_nonzero.at(j) + bias) / width);
            Int_t z = layer.at(j);
            Int_t index = z * 100000 + x * 100 + y;
            Double_t tempE9 = map_cellid[index];
            Double_t tempE49 = map_cellid[index];
            for (Int_t ix = x - 3; ix <= x + 3; ix++)
            {
                if (ix < 0 || ix > 17)
                    continue;
                for (Int_t iy = y - 3; iy <= y + 3; iy++)
                {
                    if (iy < 0 || iy > 17)
                        continue;
                    Int_t tmp = z * 100000 + ix * 100 + iy;
                    if (ix >= x - 1 && ix <= x + 1 && iy >= y - 1 && iy <= y + 1)
                        tempE9 += map_cellid[tmp];
                    tempE49 += map_cellid[tmp];
                }
            }
            E9E49 += tempE9 / tempE49;
        }
        E9E49 /= nhits;
        return E9E49;
    }, {"Hit_X_nonzero", "Hit_Y_nonzero", "layer", "Hit_Energy_nonzero", "nhits"})
    // The distance between the layer with largest RMS value of position and the beginning layer
    .Define("shower_length", [] (vector<Double_t> layer_rms, Int_t shower_start)
    {
        Double_t shower_length = 0.0;
        Double_t startz = shower_start * thick;
        Int_t max_layer = 0;
        Double_t max_rms = 0.0;
        for (Int_t i = 0; i < layer_rms.size(); i++)
            if (layer_rms.at(i) > max_rms)
            {
                max_layer = i;
        	    max_rms = layer_rms.at(i);
            }
        //auto maxPosition = max_element(layer_rms.begin() + shower_start, layer_rms.end());
        //Int_t max_layer = maxPosition - layer_rms.begin();
        if (shower_start >= max_layer)
            shower_length = 0.0;
        else
            shower_length = (max_layer - shower_start) * thick;
        return shower_length;
    }, {"layer_rms", "shower_start"})
    // 2-dimensional fractal dimension
    .Define("FD_2D", [] (RVec<Double_t> const& pos_x, RVec<Double_t> const& pos_y, RVec<Double_t> const& pos_z, Int_t nhits)
    {
        vector<Int_t> scale = { 2, 3, 4, 5, 6, 7, 8, 9, 10, 12, 15, 20, 30, 40, 50, 60, 70, 80, 90, 100, 110, 120, 130, 140, 150 };
        const Int_t num = scale.size();
        vector<Double_t> fd_2d(num);
        vector<Int_t> NResizeHit(num);
        for (Int_t i = 0; i < num; i++)
        {
            if (nhits == 0)
            {
                fd_2d.at(i) = -1;
                continue;
            }
            NResizeHit.at(i) = NHScaleV2(pos_x, pos_y, pos_z, scale.at(i), scale.at(i), 1);
            fd_2d.at(i) = Log((Double_t) nhits / NResizeHit.at(i)) / Log((Double_t) scale.at(i));
        }
        return fd_2d;
    }, {"Hit_X_nonzero", "Hit_Y_nonzero", "Hit_Z_nonzero", "nhits"})
    // 3-dimensional fractal dimension
    .Define("FD_3D", [] (RVec<Double_t> const& pos_x, RVec<Double_t> const& pos_y, RVec<Double_t> const& pos_z, Int_t nhits)
    {
        vector<Int_t> scale = { 2, 3, 4, 5, 6, 7, 8, 9, 10, 12, 15, 20, 30, 40, 50, 60, 70, 80, 90, 100, 110, 120, 130, 140, 150 };
        const Int_t num = scale.size();
        vector<Double_t> fd_3d(num);
        vector<Int_t> NResizeHit(num);
        for (Int_t i = 0; i < num; i++)
        {
            if (nhits == 0)
            {
                fd_3d.at(i) = -1;
                continue;
            }
            NResizeHit.at(i) = NHScaleV2(pos_x, pos_y, pos_z, scale.at(i), scale.at(i), scale.at(i));
            fd_3d.at(i) = Log((Double_t) nhits / NResizeHit.at(i)) / Log((Double_t) scale.at(i));
        }
        return fd_3d;
    }, {"Hit_X_nonzero", "Hit_Y_nonzero", "Hit_Z_nonzero", "nhits"})
    // Average value of all the 2-dimensional fractal dimensions
    .Define("FD_2D_mean", [] (vector<Double_t> FD_2D)
    {
        const Int_t num = FD_2D.size();
        Double_t total = 0;
        for (Int_t i = 0; i < num; i++)
            total += FD_2D.at(i);
        Double_t FD_2D_mean = total / num;
        return FD_2D_mean;
    }, {"FD_2D"})
    // Average value of all the 3-dimensional fractal dimensions
    .Define("FD_3D_mean", [] (vector<Double_t> FD_3D)
    {
        const Int_t num = FD_3D.size();
        Double_t total = 0;
        for (Int_t i = 0; i < num; i++)
            total += FD_3D.at(i);
        Double_t FD_3D_mean = total / num;
        return FD_3D_mean;
    }, {"FD_3D"})
    // RMS value of all the 2-dimensional fractal dimensions
    .Define("FD_2D_rms", [] (vector<Double_t> FD_2D)
    {
        const Int_t num = FD_2D.size();
        Double_t total2 = 0;
        for (Int_t i = 0; i < num; i++)
            total2 += Power(FD_2D.at(i), 2);
        Double_t FD_2D_rms = Sqrt(total2 / num);
        return FD_2D_rms;
    }, {"FD_2D"})
    // RMS value of all the 3-dimensional fractal dimensions
    .Define("FD_3D_rms", [] (vector<Double_t> FD_3D)
    {
        const Int_t num = FD_3D.size();
        Double_t total2 = 0;
        for (Int_t i = 0; i < num; i++)
            total2 += Power(FD_3D.at(i), 2);
        Double_t FD_3D_rms = Sqrt(total2 / num);
        return FD_3D_rms;
    }, {"FD_3D"})
    // Centre of gravity of each layer, in x direction
    .Define("COG_X", [] (vector<Double_t> Hit_X_nonzero, vector<Int_t> layer, vector<Double_t> Hit_Energy_nonzero, vector<Double_t> layer_energy, Int_t nhits)
    {
        vector<Double_t> cog_x(nlayer);
        for (Int_t i = 0; i < nhits; i++)
            cog_x.at(layer.at(i)) += Hit_X_nonzero.at(i) * Hit_Energy_nonzero.at(i);
        for (Int_t j = 0; j < cog_x.size(); j++)
        {
            if (cog_x.at(j) == 0)
                continue;
            else
                cog_x.at(j) /= layer_energy.at(j);
        }
        return cog_x;
    }, {"Hit_X_nonzero", "layer", "Hit_Energy_nonzero", "layer_energy", "nhits"})
    // Centre of gravity of each layer, in y direction
    .Define("COG_Y", [] (vector<Double_t> Hit_Y_nonzero, vector<Int_t> layer, vector<Double_t> Hit_Energy_nonzero, vector<Double_t> layer_energy, Int_t nhits)
    {
        vector<Double_t> cog_y(nlayer);
        for (Int_t i = 0; i < nhits; i++)
            cog_y.at(layer.at(i)) += Hit_Y_nonzero.at(i) * Hit_Energy_nonzero.at(i);
        for (Int_t j = 0; j < cog_y.size(); j++)
        {
            if (cog_y.at(j) == 0)
                continue;
            else
                cog_y.at(j) /= layer_energy.at(j);
        }
        return cog_y;
    }, {"Hit_Y_nonzero", "layer", "Hit_Energy_nonzero", "layer_energy", "nhits"})
    // Centre of gravity of every 4 layers, in x direction
    .Define("COG_X_4", [] (vector<Double_t> Hit_X_nonzero, vector<Int_t> layer, vector<Double_t> Hit_Energy_nonzero, vector<Double_t> layer_energy, Int_t nhits)
    {
        vector<Double_t> cog_x_4(nlayer / 4);
        vector<Double_t> energy_4layer(nlayer / 4);
        for (Int_t i = 0; i < nhits; i++)
            cog_x_4.at(layer.at(i) / 4) += Hit_X_nonzero.at(i) * Hit_Energy_nonzero.at(i);
        for (Int_t j = 0; j < nlayer; j++)
            energy_4layer.at(j / 4) += layer_energy.at(j);
        for (Int_t k = 0; k < nlayer / 4; k++)
        {
            if (cog_x_4.at(k) == 0)
                continue;
            else
                cog_x_4.at(k) /= energy_4layer.at(k);
        }
        return cog_x_4;
    }, {"Hit_X_nonzero", "layer", "Hit_Energy_nonzero", "layer_energy", "nhits"})
    // Centre of gravity of every 4 layers, in y direction
    .Define("COG_Y_4", [] (vector<Double_t> Hit_Y_nonzero, vector<Int_t> layer, vector<Double_t> Hit_Energy_nonzero, vector<Double_t> layer_energy, Int_t nhits)
    {
        vector<Double_t> cog_y_4(nlayer / 4);
        vector<Double_t> energy_4layer(nlayer / 4);
        for (Int_t i = 0; i < nhits; i++)
            cog_y_4.at(layer.at(i) / 4) += Hit_Y_nonzero.at(i) * Hit_Energy_nonzero.at(i);
        for (Int_t j = 0; j < nlayer; j++)
            energy_4layer.at(j / 4) += layer_energy.at(j);
        for (Int_t k = 0; k < nlayer / 4; k++)
        {
            if (cog_y_4.at(k) == 0)
                continue;
            else
                cog_y_4.at(k) /= energy_4layer.at(k);
        }
        return cog_y_4;
    }, {"Hit_Y_nonzero", "layer", "Hit_Energy_nonzero", "layer_energy", "nhits"})
    // Centre of gravity of every 4 layers, in z direction
    .Define("COG_Z_4", [] (vector<Double_t> Hit_Z_nonzero, vector<Int_t> layer, vector<Double_t> Hit_Energy_nonzero, vector<Double_t> layer_energy, Int_t nhits)
    {
        vector<Double_t> cog_z_4(nlayer / 4);
        vector<Double_t> energy_4layer(nlayer / 4);
        for (Int_t i = 0; i < nhits; i++)
            cog_z_4.at(layer.at(i) / 4) += Hit_Z_nonzero.at(i) * Hit_Energy_nonzero.at(i);
        for (Int_t j = 0; j < nlayer; j++)
            energy_4layer.at(j / 4) += layer_energy.at(j);
        for (Int_t k = 0; k < nlayer / 4; k++)
        {
            if (cog_z_4.at(k) == 0)
                continue;
            else
                cog_z_4.at(k) /= energy_4layer.at(k);
        }
        return cog_z_4;
    }, {"Hit_Z_nonzero", "layer", "Hit_Energy_nonzero", "layer_energy", "nhits"})
    // The overall centre of gravity, in x direction
    .Define("COG_X_overall", [] (vector<Double_t> Hit_X_nonzero, vector<Double_t> Hit_Energy_nonzero, Double_t Edep, Int_t nhits)
    {
        Double_t cog_x_overall = 0;
        if (Edep == 0)
            return cog_x_overall;
        else
        {
            for (Int_t i = 0; i < nhits; i++)
                cog_x_overall += Hit_X_nonzero.at(i) * Hit_Energy_nonzero.at(i);
            cog_x_overall /= Edep;
            return cog_x_overall;
        }
    }, {"Hit_X_nonzero", "Hit_Energy_nonzero", "Edep", "nhits"})
    // The overall centre of gravity, in y direction
    .Define("COG_Y_overall", [] (vector<Double_t> Hit_Y_nonzero, vector<Double_t> Hit_Energy_nonzero, Double_t Edep, Int_t nhits)
    {
        Double_t cog_y_overall = 0;
        if (Edep == 0)
            return cog_y_overall;
        else
        {
            for (Int_t i = 0; i < nhits; i++)
                cog_y_overall += Hit_Y_nonzero.at(i) * Hit_Energy_nonzero.at(i);
            cog_y_overall /= Edep;
            return cog_y_overall;
        }
    }, {"Hit_Y_nonzero", "Hit_Energy_nonzero", "Edep", "nhits"})
    // The overall centre of gravity, in z direction
    .Define("COG_Z_overall", [] (vector<Double_t> Hit_Z_nonzero, vector<Double_t> Hit_Energy_nonzero, Double_t Edep, Int_t nhits)
    {
        Double_t cog_z_overall = 0;
        if (Edep == 0)
            return cog_z_overall;
        else
        {
            for (Int_t i = 0; i < nhits; i++)
                cog_z_overall += Hit_Z_nonzero.at(i) * Hit_Energy_nonzero.at(i);
            cog_z_overall /= Edep;
            return cog_z_overall;
        }
    }, {"Hit_Z_nonzero", "Hit_Energy_nonzero", "Edep", "nhits"})
    //
    .Define("hclx", [] (vector<Double_t> Hit_X_nonzero, vector<Double_t> Hit_Y_nonzero, vector<Double_t> Hit_Z_nonzero, vector<Double_t> Hit_Energy_nonzero)
    {
        vector<Double_t> hclx;
        Hough* httool = new Hough(Hit_X_nonzero, Hit_Y_nonzero, Hit_Z_nonzero, Hit_Energy_nonzero);
        hclx = httool->GetHclX();
        delete httool;
        return hclx;
    }, {"Hit_X_nonzero", "Hit_Y_nonzero", "Hit_Z_nonzero", "Hit_Energy_nonzero"})
    // 
    .Define("hcly", [] (vector<Double_t> Hit_X_nonzero, vector<Double_t> Hit_Y_nonzero, vector<Double_t> Hit_Z_nonzero, vector<Double_t> Hit_Energy_nonzero)
    {
        vector<Double_t> hcly;
        Hough* httool = new Hough(Hit_X_nonzero, Hit_Y_nonzero, Hit_Z_nonzero, Hit_Energy_nonzero);
        hcly = httool->GetHclY();
        delete httool;
        return hcly;
    }, {"Hit_X_nonzero", "Hit_Y_nonzero", "Hit_Z_nonzero", "Hit_Energy_nonzero"})
    // 
    .Define("hclz", [] (vector<Double_t> Hit_X_nonzero, vector<Double_t> Hit_Y_nonzero, vector<Double_t> Hit_Z_nonzero, vector<Double_t> Hit_Energy_nonzero)
    {
        vector<Double_t> hclz;
        Hough* httool = new Hough(Hit_X_nonzero, Hit_Y_nonzero, Hit_Z_nonzero, Hit_Energy_nonzero);
        hclz = httool->GetHclZ();
        delete httool;
        return hclz;
    }, {"Hit_X_nonzero", "Hit_Y_nonzero", "Hit_Z_nonzero", "Hit_Energy_nonzero"})
    //
    .Define("hcle", [] (vector<Double_t> Hit_X_nonzero, vector<Double_t> Hit_Y_nonzero, vector<Double_t> Hit_Z_nonzero, vector<Double_t> Hit_Energy_nonzero)
    {
        vector<Double_t> hcle;
        Hough* httool = new Hough(Hit_X_nonzero, Hit_Y_nonzero, Hit_Z_nonzero, Hit_Energy_nonzero);
        hcle = httool->GetHclE();
        delete httool;
        return hcle;
    }, {"Hit_X_nonzero", "Hit_Y_nonzero", "Hit_Z_nonzero", "Hit_Energy_nonzero"})
    // The number of tracks of an event, with Hough transformation applied
    .Define("ntrack", [] (vector<Double_t> Hit_X_nonzero, vector<Double_t> Hit_Y_nonzero, vector<Double_t> Hit_Z_nonzero, vector<Double_t> Hit_Energy_nonzero)
    {
        Int_t ntrack = 0;
        Hough* httool = new Hough(Hit_X_nonzero, Hit_Y_nonzero, Hit_Z_nonzero, Hit_Energy_nonzero);
        ntrack = httool->GetNtrack();
        delete httool;
        return ntrack;
    }, {"Hit_X_nonzero", "Hit_Y_nonzero", "Hit_Z_nonzero", "Hit_Energy_nonzero"})
    /*
    // The average time of all the hits
    .Define("hit_time_mean", [] (vector<Double_t> hit_time, Int_t nhits)
    {
        Double_t tot = 0;
        for (Double_t& i : hit_time)
            tot += i;
        return tot / nhits;
    }, {"Hit_Time_nonzero", "nhits"})
    // The RMS time of all the hits
    .Define("hit_time_rms", [] (vector<Double_t> hit_time, Int_t nhits)
    {
        Double_t tot2 = 0;
        for (Double_t& i : hit_time)
            tot2 += i * i;
        return Sqrt(tot2 / nhits);
    }, {"Hit_Time_nonzero", "nhits"})
    // The average time of all the hits between beginning and ending layers
    .Define("shower_time_mean", [] (vector<Double_t> hit_time, vector<Int_t> layer, Int_t beginning, Int_t ending, Int_t nhits)
    {
        Double_t tot = 0;
        Int_t hits = 0;
        for (Int_t i = 0; i < nhits; i++)
            if (layer.at(i) >= beginning && layer.at(i) < ending)
            {
                hits++;
                tot += hit_time.at(i), 2;
            }
        if (hits == 0)
            return 0.0;
        else
            return tot / nhits;
    }, {"Hit_Time_nonzero", "layer", "shower_start", "shower_end", "nhits"})
    // The RMS time of all the hits between beginning and ending layers
    .Define("shower_time_rms", [] (vector<Double_t> hit_time, vector<Int_t> layer, Int_t beginning, Int_t ending, Int_t nhits)
    {
        Double_t tot2 = 0;
        Int_t hits = 0;
        for (Int_t i = 0; i < nhits; i++)
            if (layer.at(i) >= beginning && layer.at(i) < ending)
            {
                hits++;
                tot2 += Power(hit_time.at(i), 2);
            }
        if (hits == 0)
            return 0.0;
        else
            return Sqrt(tot2 / nhits);
    }, {"Hit_Time_nonzero", "layer", "shower_start", "shower_end", "nhits"})
    */
    //.Range(1)
    .Snapshot(tree, outname);
    delete dm;

    TFile* f = new TFile((TString) outname, "READ");
    TTree* t = f->Get<TTree>((TString) tree);
    t->SetBranchStatus("*", 1);
    vector<TString> deactivate = { "CellID", "Hit_Energy_nonzero", /*"Hit_Time_nonzero",*/ "Hit_X_nonzero", "Hit_Y_nonzero", "Hit_Z_nonzero" };
    for (TString de : deactivate)
        t->SetBranchStatus(de, 0);
    TFile* fnew = new TFile((TString) outname, "RECREATE");
    TTree* tnew = t->CloneTree();
    tnew->Write();
    f->Close();
    fnew->Close();

    return 1;
}
