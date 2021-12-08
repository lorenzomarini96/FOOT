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
		

		// START COUNTER
		Int_t n_points_SC = 0; // number of points for graphs.
      	Int_t chn_SC_CLK = 16;
		Double_t voltage_SC_CLK_NC[1023]; // No correction
		Double_t voltage_SC_CLK[1023];
		Double_t time_SC_CLK[1023];
				
		Int_t enablesum = 0;

		// TOF-WALL
		Int_t n_points_TW = 0; // number of points for graphs.
		Int_t chn_TW_CLK = 16;	
		Double_t voltage_TW_CLK_NC[1023]; // No correction
		Double_t voltage_TW_CLK[1023];
		Double_t time_TW_CLK[1023];
		

		// START-COUNTER
		for (Int_t i=0; i<1023; i++)     
		{
			voltage_SC_CLK[i]    = board173_waveform[chn_SC_CLK][i]; // Final Amplitude
			voltage_SC_CLK_NC[i] = board173_waveform[chn_SC_CLK][i]; // Amplitude Not Corrected
			time_SC_CLK[i]       = board173_time[chn_SC_CLK][i] * TMath::Power(10,9);

			if (voltage_SC_CLK_NC[i] - voltage_SC_CLK_NC[i-1] >  0.5) enablesum -= 1;
			if (voltage_SC_CLK_NC[i] - voltage_SC_CLK_NC[i-1] < -0.5) enablesum += 1;
			if (1)
			{
				voltage_SC_CLK[i] = voltage_SC_CLK_NC[i] + enablesum; // sommo 1V
				n_points_SC += 1;
			}

		}


		enablesum = 0;

		// TOF-WALL
		for (Int_t i=0; i<1023; i++)     
		{
			voltage_TW_CLK[i]    = board166_waveform[chn_TW_CLK][i]; // Final Amplitude
			voltage_TW_CLK_NC[i] = board166_waveform[chn_TW_CLK][i]; // Amplitude Not Corrected
			time_TW_CLK[i]       = board166_time[chn_TW_CLK][i] * TMath::Power(10,9);

			if (voltage_TW_CLK_NC[i] - voltage_TW_CLK_NC[i-1] >  0.5) enablesum -= 1;
			if (voltage_TW_CLK_NC[i] - voltage_TW_CLK_NC[i-1] < -0.5) enablesum += 1;
			voltage_TW_CLK[i] = voltage_TW_CLK_NC[i] + enablesum; // sommo 1V
			n_points_TW += 1;
		}

		TGraph *gr1 = new TGraph(n_points_SC, time_SC_CLK, voltage_SC_CLK);
		gr1->GetXaxis()->SetTitle("Time [ns]");
		gr1->GetYaxis()->SetTitle("Amplitude [V]");
		gr1->SetTitle(" ");
		gr1->SetLineColor(4);
		//gr1->GetXaxis()->SetRangeUser(-20., 350);
		//gr1->GetYaxis()->SetRangeUser(-0.75, -0.2);

		
		TGraph *gr2 = new TGraph(n_points_TW, time_TW_CLK, voltage_TW_CLK);
		gr2->GetXaxis()->SetTitle("Time [ns]");
		gr2->GetYaxis()->SetTitle("Amplitude [V]");
		gr2->SetTitle(" ");
		gr2->SetLineColor(2);
		//gr2->GetXaxis()->SetRangeUser(-20., 350);
		//gr2->GetYaxis()->SetRangeUser(-0.75, -0.2);
		
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
		c3->SetLeftMargin(0.15);
		c3->SetTickx();
   		c3->SetTicky();
		gr1->Draw("AC");
		gr2->Draw("CP");
		auto legend = new TLegend(0.1, 0.7, 0.44, 0.9);
   		//legend->SetHeader("Clock signals","C"); // option "C" allows to center the header
   		legend->AddEntry(gr1,"Start Counter","l");
   		legend->AddEntry(gr2,"Tof-wall","l");
   		legend->Draw();   		

		
	}

}
