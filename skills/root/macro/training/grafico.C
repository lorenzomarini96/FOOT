// grafico.C

// Define the fit function
Double_t fit(Double_t *x, Double_t *param) {
    return param[0]*x[0]*x[0] + param[1]*x[0] + param[2];
}

// Data
double x[5] = {1, 2, 3, 4, 5};
double dx[5] = {0.01, 0.01, 0.01, 0.01, 0.01};
double y[5] = {1, 4, 9, 13, 26};
double dy[5] = {0.5, 0.5, 0.5, 0.5, 0.5};
    

// create a canvas
TCanvas *c1 = new TCanvas("c1", "Title");
c1->SetTickx();
c1->SetTicky();
c1->SetLeftMargin(0.15);
c1->Divide(2,2); // set subdivisions, called pads

    
void plot(int i, double x, double dx, double y, double dy)
{
    c1->cd(i); // change to pad 1 of canvas c1
    // draw fit results on plot
    gStyle->SetOptFit(101111);

    // Data 
    //TGraphErrors *gr = new TGraphErrors(5, x, y, dx, dy);
    
    
    // Plot
    gr->Draw("A*");
    gr->GetXaxis()->SetTitle("X Axis [x]");
    gr->GetYaxis()->SetTitle("Y Axis [x]");
    gr->SetTitle("x vs y");
    gr->GetXaxis()->SetTitleSize(0.05);
    gr->GetYaxis()->SetTitleSize(0.05);
    gr->GetXaxis()->SetLabelSize(0.05);
    gr->GetYaxis()->SetLabelSize(0.05);
    
    
    // Fit Function
    TF1 *fit_func = new TF1("fit_func", fit, 0, 10, 3); // 0, 10, = range and 3 = number params
    fit_func->SetParNames("a", "b", "c");
    fit_func->SetLineColor(kBlue);
    // Make the fit
    gr->Fit("fit_func", "R");
    //  to print the fit probability, parameter names/values, and errors, use:
    

}
void grafico()
{   
    for (int i=0; i<4; i++)
    {
        plotplot(double x, double dx, double y, double dy);
    }
}
    
