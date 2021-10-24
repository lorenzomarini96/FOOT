void plot_data1(const char *filename)
{
    // Canvas
    double w = 1200;
    double h = 800;
    TCanvas *c = new TCanvas("c", filename, w, h);
    c->SetTickx();
    c->SetTicky();

    c->Divide(4,2);
    for(int i=1; i<9; i++)
    {
        c->cd(i);
        // Load data file. (Equivalently)
        TGraphErrors *gr = new TGraphErrors(filename);
        
        // Plot
        gr->Draw("A*");
        gr->GetXaxis()->SetTitle("X Axis [x]");
        gr->GetYaxis()->SetTitle("Y Axis [x]");
        gr->SetTitle(filename);
        gr->SetMarkerStyle(20);
    }
    c->SaveAs("plot_data1.pdf");
    //c->Print("plot_data1.tex") //DA PROVARE!

}


