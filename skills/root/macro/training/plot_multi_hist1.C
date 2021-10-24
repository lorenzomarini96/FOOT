
void plot_hist_fit(const char *filename, const char *make_fit="fit")
{
    //--------------------------------------------------------------------
    // CANVAS
    //--------------------------------------------------------------------
    double w = 2000;
    double h = 1500;
    TCanvas *c = new TCanvas("c", filename, w, h);
    gStyle->SetOptFit(101111);
    c->SetTickx();
    c->SetTicky();
    c->Divide(4,2);

    for(int i=1; i<9; i++)
    {
        c->cd(i);

        TH1F *hist = new TH1F("hist", filename, 100, 0, 15);

        //--------------------------------------------------------------------
        // Read the data file and directly fill the data into the histogram
        //--------------------------------------------------------------------
        fstream file;
        file.open(filename, ios::in);
    
        double value;
        while(1)
        {
            file >> value;
            hist->Fill(value);
            if(file.eof()) break;
        }
        file.close();
        
        if (strcmp(make_fit, "fit") == 0)
        {
            //-------------------------------------------------------------------- 
            // FIT
            //--------------------------------------------------------------------

            // To make the fit after
            TF1 *fit = new TF1("fit", "gaus", 0, 15);
 
            fit->SetLineColor(kRed);
            fit->SetParameter(0,40);
            fit->SetParameter(1,5);
            fit->SetParameter(2,1);
            fit->SetParNames("Const", "Mean", "Std Dev");
            hist->Fit("fit", "R");  // R = range of fit
            //--------------------------------------------------------------------
            // PLOT
            //--------------------------------------------------------------------
            hist->Draw();
            hist->GetXaxis()->SetTitle("Data [x]");
            hist->GetYaxis()->SetTitle("Counts");
            hist->SetFillColor(kCyan-9);
            hist->SetTitle(filename);
        }

        else
        {
            //--------------------------------------------------------------------
            // PLOT
            //--------------------------------------------------------------------
            hist->Draw();
            hist->GetXaxis()->SetTitle("Data [x]");
            hist->GetYaxis()->SetTitle("Counts");
            hist->SetFillColor(kCyan-9);
            hist->SetTitle(filename);
        }
        
    }
    //c->SaveAs("plot_hist.pdf");


}

// FONTE https://root-forum.cern.ch/t/open-files-in-a-directory-with-a-for-loop/12471


// MAIN
void plot_multi_hist(const char *dirname, const char *ext)
{
    // LOOP ON ALL FILE ON GIVEN DIR
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
            }
            plot_hist_fit(fname, make_fit);
        }

}
