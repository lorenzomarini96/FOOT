///////////////////////////////////////////////////////////////////////
// clock_v1.C
// Macro to analyze clock signals.
// Date: 06 December 2021
// Author: L. Marini
///////////////////////////////////////////////////////////////////////

#define rec_cxx
#include "rec.h"
#include <TH2.h>
#include <TStyle.h>
#include <TCanvas.h>
#include "TStopwatch.h"

void rec::Loop()
{
   	if (fChain == 0) return;

   	Long64_t nentries = fChain->GetEntriesFast();
   	Long64_t nbytes = 0, nb = 0;


   	gStyle->SetOptFit(10111);   


  	// LOOP ON ENTRIES
  	//for (Long64_t jentry=0; jentry<nentries; jentry++)
	for (Long64_t jentry=0; jentry<1; jentry++)           // ONLY FIRST EVENT FOR THE MOMENT
   	{
    	Long64_t ientry = LoadTree(jentry);
      	if (ientry < 0) break;
      	nb = fChain->GetEntry(jentry);   nbytes += nb;
		
		Int_t n_points = 0; // number of points for graphs.

		// START COUNTER
      	Int_t chn_SC_CLK = 16;	
		Double_t delta_voltage_SC_CLK = 0.;
		Double_t voltage_SC_CLK_NC[1000]; // No correction
		Double_t voltage_SC_CLK[1000];
		Double_t time_SC_CLK[1000];
		Double_t min_SC_CLK;
		Double_t max_SC_CLK;
		
		// START COUNTER
		
		
		Int_t enablesum = 0;
		for (Int_t i=1; i<1000; i++)     
		{
			voltage_SC_CLK[i]    = board173_waveform[chn_SC_CLK][i]; // Final Amplitude
			voltage_SC_CLK_NC[i] = board173_waveform[chn_SC_CLK][i]; // Amplitude Not Corrected
			time_SC_CLK[i]       = board173_time[chn_SC_CLK][i]; // * TMath::Power(10,9);
		
			if (voltage_SC_CLK_NC[i] - voltage_SC_CLK_NC[i-1] >  0.6) enablesum = - 1;
			if (voltage_SC_CLK_NC[i] - voltage_SC_CLK_NC[i-1] < -0.6) enablesum = + 1;

			voltage_SC_CLK[i] = voltage_SC_CLK_NC[i] + enablesum; // sommo 1V

			n_points += 1;
			cout << "∆V    [V]                      = " << voltage_SC_CLK_NC[i] - voltage_SC_CLK_NC[i-1]  << endl;
			cout << "voltage_SC_CLK["<<i<<"]    [V] = " << voltage_SC_CLK[i]    << endl;
			cout << "voltage_SC_CLK_NC["<<i<<"] [V] = " << voltage_SC_CLK_NC[i] << endl;
			cout << "time_SC_CLK["<<i<<"]       [s] = " << time_SC_CLK[i]       << endl;
			cout << "\n----------------------------------------------------\n"  << endl;
		}

		TGraph *gr1 = new TGraph(n_points, time_SC_CLK, voltage_SC_CLK);
		gr1->GetXaxis()->SetTitle("Time [ns]");
		gr1->GetYaxis()->SetTitle("Amplitude [V]");
		gr1->SetLineColor(4);

		TGraph *gr2 = new TGraph(n_points, time_SC_CLK, voltage_SC_CLK_NC);
		gr2->GetXaxis()->SetTitle("Time [ns]");
		gr2->GetYaxis()->SetTitle("Amplitude [V]");
		gr2->SetLineColor(2);
		
		TCanvas *c1 = new TCanvas("c1"," ", 800,800);
		c1->SetTickx();
   		c1->SetTicky();
   		c1->SetLeftMargin(0.15);
		gr1->Draw("AC");

		TCanvas *c2 = new TCanvas("c2"," ", 800,800);
		c2->SetTickx();
   		c2->SetTicky();
   		c2->SetLeftMargin(0.15);
		gr2->Draw("AC");
		
		TCanvas *c3 = new TCanvas("c3"," ", 800,800);
		gr1->Draw("AC");
		gr2->Draw("CP");   		
	
		
	}

}
