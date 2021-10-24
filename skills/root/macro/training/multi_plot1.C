void multi_plot(const char *dirname, const char *ext)
{   
    // 1) CANVAS
    double w = 500;
    double h = 500;
    TCanvas *c = new TCanvas("c", "X", w, h);


    // LOOP ON ALL FILES IN "data_new" DIRECTORY
    TSystemDirectory dir(dirname, dirname);
    TList *files = dir.GetListOfFiles();
    if (files)
    {
        TSystemFile *file;
        TString fname;
        TIter next(files);
        while ((file=(TSystemFile*)next()))
        {
            fname = file->GetName();
            if (!file->IsDirectory() && fname.EndsWith(ext))
            {
                cout << fname.Data() << endl;

                //TGraphErrors *gr = new TGraphErrors(fname.Data());

             }

            TGraphErrors *gr = new TGraphErrors("data_xy/data_xy_1.txt");
            // Plot
            gr->Draw("A*");
            gr->GetXaxis()->SetTitle("X Axis [x]");
            gr->GetYaxis()->SetTitle("Y Axis [x]");
            gr->SetTitle("x vs y");

    
    
        }

    }


   
}