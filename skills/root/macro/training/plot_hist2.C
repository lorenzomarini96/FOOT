void make_histogram(auto *hist)
{
    cout << "Passato" << endl;
}


void plot_hist2(const char *filename, const char *make_fit)
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
        // bins = 100 
        // title = filename
        // 0, 15 = estremi 

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
            /*
            - fit   = name of the function
            - gaus  = defined (internal root) function
            - 0, 15 = interval of fitting
            */
            fit->SetLineColor(kRed);
            fit->SetParameter(0,40);
            // Set the mean value
            fit->SetParameter(1,5);
            // Set the standard deviation
            fit->SetParameter(2,1);
            fit->SetParNames("Const", "Mean", "Std Dev");

            // 4) Make a fit using the ROOT function created
            hist->Fit("fit", "R");  // R = range of fit
            // Use "RQ", where Q means "quite" for no show the fit parameters
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
    //c->Print("plot_hist.tex") // DA PROVARE!


    // TODO.........................
    /* AGGIUNGERE FIT PER OGNI SINGOLO ISTOGRAMMA E IL PRINT DEI PARAMETRI
    DI BEST-FIT PER CIASCUNO DI LORO. CHI2-TEST ECC...
    TIPO:
    
    //===============================================================
    // Get best-fit parameters
    double chi2 = fit->GetChisquare();
    double par0 = fit->GetParameter(0); // value of 1st parameter
    double err0 = fit->GetParError(0);  // error on first parameter
    double par1 = fit->GetParameter(1); // value of 1st parameter
    double err1 = fit->GetParError(1);  // error on first parameter
    double par2 = fit->GetParameter(2); // value of 1st parameter
    double err2 = fit->GetParError(2);  // error on first parameter
    
    // Print values
    printf("chi2 = %.3f\n", chi2);
    //printf("par0 = %.3f +/- %.3f", (par0, err0)); Not works...
    printf("err0 = %.3f\n", err0);
    printf("par1 = %.3f\n", par1);
    printf("err1 = %.3f\n", err1);
    printf("par2 = %.3f\n", par2);
    printf("err2 = %.3f\n", err2);
    //===============================================================


    */


}


