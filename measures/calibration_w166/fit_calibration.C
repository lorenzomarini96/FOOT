// Define the fit function
Double_t fit(Double_t *x, Double_t *param) {
    return param[0]*x[0] + param[1];
}

void fit_calibration()
{
    // Add best-fit results (coefficient, chi2/ndf, pvalue) in a legend on the canvas.
    gStyle->SetOptFit(10111);
    

    //------------------------------------------------------------------------------------------
    // CHANNEL 0
    //------------------------------------------------------------------------------------------
    // Canvas
    TCanvas *c0 = new TCanvas("c0", "channel0");
    c0->SetTickx();
    c0->SetTicky();
    c0->SetLeftMargin(0.15);
    
    // Load data file. (Equivalently)
    TGraphErrors *gr0 = new TGraphErrors("data/calibration_w166_ch0.txt");

    // Plot
    gr0->Draw("A*");
    gr0->GetXaxis()->SetTitle("DRS Amplitude [mV]");
    gr0->GetYaxis()->SetTitle("Trigger value [mV]");
    gr0->SetTitle("Calibration w166 ch0");
    gr0->SetMarkerStyle(20);

    // Fit Function
    TF1 *fit_func = new TF1("fit_func", fit, 300, 650, 2); // 0, 10, = range and 2 = number params
    fit_func->SetParNames("a", "b");
    fit_func->SetLineColor(kBlue);
    fit_func->SetLineStyle(2); // style --
    // Make the fit
    gr0->Fit("fit_func", "R"); // to print the fit probability, parameter names/values, and errors,

    //========================================================================
    // Get the best fit parameters.
    double chi2 = fit_func->GetChisquare();
    double pvalue = fit_func->GetProb();
    double a = fit_func->GetParameter(0);
    double da = fit_func->GetParError(0);
    double b = fit_func->GetParameter(1);
    double db = fit_func->GetParError(1);

    // Add a legend to the histogram
    //TLegend *leg = new TLegend(0.5, 0.6, 0.8, 0.8); // Set the values of the legend boundries
    // Remove the margin around the legend.
    //leg->SetBorderSize(0);
    //leg->AddEntry(gr0, "Measured Data", "p"); // "p" = point (?). Try also "lpf"
    //leg->AddEntry(fit_func, "Fit Function", "l");
    //leg->AddEntry(&chi2, "chi2");
    //leg->Draw();


    // Save plot    
    c0->SaveAs("./figures/calibration_w166_ch0.pdf");

    //------------------------------------------------------------------------------------------
    // CHANNEL 1
    //------------------------------------------------------------------------------------------
    // Canvas
    TCanvas *c1 = new TCanvas("c1", "channel1");
    c1->cd();
    c1->SetTickx();
    c1->SetTicky();
    c1->SetLeftMargin(0.15);
    // Load data file
    TGraphErrors *gr1 = new TGraphErrors("data/calibration_w166_ch1.txt");
    // Plot
    gr1->Draw("A*");
    gr1->GetXaxis()->SetTitle("DRS Amplitude [mV]");
    gr1->GetYaxis()->SetTitle("Trigger value [mV]");
    gr1->SetTitle("Calibration w166 ch1");
    gr1->SetMarkerStyle(20);
    // Make the fit
    gr1->Fit("fit_func", "R");
    c1->SaveAs("./figures/calibration_w166_ch1.pdf");

    //------------------------------------------------------------------------------------------
    // CHANNEL 2
    //------------------------------------------------------------------------------------------
    // Canvas
    TCanvas *c2 = new TCanvas("c2", "channel2");
    c2->cd();
    c2->SetTickx();
    c2->SetTicky();
    c2->SetLeftMargin(0.15);
    // Load data file
    TGraphErrors *gr2 = new TGraphErrors("data/calibration_w166_ch2.txt");
    // Plot
    gr2->Draw("A*");
    gr2->GetXaxis()->SetTitle("DRS Amplitude [mV]");
    gr2->GetYaxis()->SetTitle("Trigger value [mV]");
    gr2->SetTitle("Calibration w166 ch2");
    gr2->SetMarkerStyle(20);
    // Make the fit
    gr2->Fit("fit_func", "R");
    c2->SaveAs("./figures/calibration_w166_ch2.pdf");

    //------------------------------------------------------------------------------------------
    // CHANNEL 3
    //------------------------------------------------------------------------------------------
    // Canvas
    TCanvas *c3 = new TCanvas("c3", "channel3");
    c3->cd();
    c3->SetTickx();
    c3->SetTicky();
    c3->SetLeftMargin(0.15);
    // Load data file
    TGraphErrors *gr3 = new TGraphErrors("data/calibration_w166_ch3.txt");
    // Plot
    gr3->Draw("A*");
    gr3->GetXaxis()->SetTitle("DRS Amplitude [mV]");
    gr3->GetYaxis()->SetTitle("Trigger value [mV]");
    gr3->SetTitle("Calibration w166 ch3");
    gr3->SetMarkerStyle(20);
    // Make the fit
    gr3->Fit("fit_func", "R");
    c3->SaveAs("./figures/calibration_w166_ch3.pdf");

    //------------------------------------------------------------------------------------------
    // CHANNEL 4
    //------------------------------------------------------------------------------------------
    // Canvas
    TCanvas *c4 = new TCanvas("c4", "channel4");
    c4->cd();
    c4->SetTickx();
    c4->SetTicky();
    c4->SetLeftMargin(0.15);
    // Load data file
    TGraphErrors *gr4 = new TGraphErrors("data/calibration_w166_ch4.txt");
    // Plot
    gr4->Draw("A*");
    gr4->GetXaxis()->SetTitle("DRS Amplitude [mV]");
    gr4->GetYaxis()->SetTitle("Trigger value [mV]");
    gr4->SetTitle("Calibration w166 ch4");
    gr4->SetMarkerStyle(20);
    // Make the fit
    gr4->Fit("fit_func", "R");
    c4->SaveAs("FIGURES/calibration_w166_ch4.pdf");

    //------------------------------------------------------------------------------------------
    // CHANNEL 5
    //------------------------------------------------------------------------------------------
    // Canvas
    TCanvas *c5 = new TCanvas("c5", "channel5");
    c5->cd();
    c5->SetTickx();
    c5->SetTicky();
    c5->SetLeftMargin(0.15);
    // Load data file
    TGraphErrors *gr5 = new TGraphErrors("data/calibration_w166_ch5.txt");
    // Plot
    gr5->Draw("A*");
    gr5->GetXaxis()->SetTitle("DRS Amplitude [mV]");
    gr5->GetYaxis()->SetTitle("Trigger value [mV]");
    gr5->SetTitle("Calibration w166 ch5");
    gr5->SetMarkerStyle(20);
    // Make the fit
    gr5->Fit("fit_func", "R");
    c5->SaveAs("./figures/calibration_w166_ch5.pdf");

    //------------------------------------------------------------------------------------------
    // CHANNEL 6
    //------------------------------------------------------------------------------------------
    // Canvas
    TCanvas *c6 = new TCanvas("c6", "channel6");
    c6->cd();
    c6->SetTickx();
    c6->SetTicky();
    c6->SetLeftMargin(0.15);
    // Load data file
    TGraphErrors *gr6 = new TGraphErrors("data/calibration_w166_ch6.txt");
    // Plot
    gr6->Draw("A*");
    gr6->GetXaxis()->SetTitle("DRS Amplitude [mV]");
    gr6->GetYaxis()->SetTitle("Trigger value [mV]");
    gr6->SetTitle("Calibration w166 ch6");
    gr6->SetMarkerStyle(20);
    // Make the fit
    gr6->Fit("fit_func", "R");
    c6->SaveAs("./figures/calibration_w166_ch6.pdf");

    //------------------------------------------------------------------------------------------
    // CHANNEL 7
    //------------------------------------------------------------------------------------------
    // Canvas
    TCanvas *c7 = new TCanvas("c7", "channel7");
    c7->cd();
    c7->SetTickx();
    c7->SetTicky();
    c7->SetLeftMargin(0.15);
    // Load data file
    TGraphErrors *gr7 = new TGraphErrors("data/calibration_w166_ch7.txt");
    // Plot
    gr7->Draw("A*");
    gr7->GetXaxis()->SetTitle("DRS Amplitude [mV]");
    gr7->GetYaxis()->SetTitle("Trigger value [mV]");
    gr7->SetTitle("Calibration w166 ch7");
    gr7->SetMarkerStyle(20);
    // Make the fit
    gr7->Fit("fit_func", "R");
    c7->SaveAs("./figures/calibration_w166_ch7.pdf");

    //------------------------------------------------------------------------------------------
    // CHANNEL 8
    //------------------------------------------------------------------------------------------
    // Canvas
    TCanvas *c8 = new TCanvas("c8", "channel8");
    c8->cd();
    c8->SetTickx();
    c8->SetTicky();
    c8->SetLeftMargin(0.15);
    // Load data file
    TGraphErrors *gr8 = new TGraphErrors("data/calibration_w166_ch8.txt");
    // Plot
    gr8->Draw("A*");
    gr8->GetXaxis()->SetTitle("DRS Amplitude [mV]");
    gr8->GetYaxis()->SetTitle("Trigger value [mV]");
    gr8->SetTitle("Calibration w166 ch8");
    gr8->SetMarkerStyle(20);
    // Make the fit
    gr8->Fit("fit_func", "R");
    c8->SaveAs("./figures/calibration_w166_ch8.pdf");

    //------------------------------------------------------------------------------------------
    // CHANNEL 9
    //------------------------------------------------------------------------------------------
    // Canvas
    TCanvas *c9 = new TCanvas("c9", "channel9");
    c9->cd();
    c9->SetTickx();
    c9->SetTicky();
    c9->SetLeftMargin(0.15);
    // Load data file
    TGraphErrors *gr9 = new TGraphErrors("data/calibration_w166_ch9.txt");
    // Plot
    gr9->Draw("A*");
    gr9->GetXaxis()->SetTitle("DRS Amplitude [mV]");
    gr9->GetYaxis()->SetTitle("Trigger value [mV]");
    gr9->SetTitle("Calibration w166 ch9");
    gr9->SetMarkerStyle(20);
    // Make the fit
    gr9->Fit("fit_func", "R");
    c9->SaveAs("./figures/calibration_w166_ch9.pdf");

    //------------------------------------------------------------------------------------------
    // CHANNEL 10
    //------------------------------------------------------------------------------------------
    // Canvas
    TCanvas *c10 = new TCanvas("c10", "channel10");
    c10->cd();
    c10->SetTickx();
    c10->SetTicky();
    c10->SetLeftMargin(0.15);
    // Load data file
    TGraphErrors *gr10 = new TGraphErrors("data/calibration_w166_ch10.txt");
    // Plot
    gr10->Draw("A*");
    gr10->GetXaxis()->SetTitle("DRS Amplitude [mV]");
    gr10->GetYaxis()->SetTitle("Trigger value [mV]");
    gr10->SetTitle("Calibration w166 ch10");
    gr10->SetMarkerStyle(20);
    // Make the fit
    gr10->Fit("fit_func", "R");
    c10->SaveAs("./figures/calibration_w166_ch10.pdf");

    //------------------------------------------------------------------------------------------
    // CHANNEL 11
    //------------------------------------------------------------------------------------------
    // Canvas
    TCanvas *c11 = new TCanvas("c11", "channel11");
    c11->cd();
    c11->SetTickx();
    c11->SetTicky();
    c11->SetLeftMargin(0.15);
    // Load data file
    TGraphErrors *gr11 = new TGraphErrors("data/calibration_w166_ch11.txt");
    // Plot
    gr11->Draw("A*");
    gr11->GetXaxis()->SetTitle("DRS Amplitude [mV]");
    gr11->GetYaxis()->SetTitle("Trigger value [mV]");
    gr11->SetTitle("Calibration w166 ch11");
    gr11->SetMarkerStyle(20);
    // Make the fit
    gr11->Fit("fit_func", "R");
    c11->SaveAs("./figures/calibration_w166_ch11.pdf");

}