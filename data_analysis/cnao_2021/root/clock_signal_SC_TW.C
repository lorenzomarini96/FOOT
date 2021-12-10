////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// clock_signal_SC_TW.C
// Macro to analyze clock signals.
// Date: 06 December 2021
// Author: L. Marini
// Description: Macro plotting the clock signals for Start Counter and Tof-wall. It also shows the effect of the correction
// of the waveforms caused by the artifact overflow.
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

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

  	// LOOP ON ENTRIES
  	//for (Long64_t jentry=0; jentry<nentries; jentry++)
	for (Long64_t jentry=0; jentry<1; jentry++)           // ONLY FIRST EVENT FOR THE MOMENT
   	{
    	Long64_t ientry = LoadTree(jentry);
      	if (ientry < 0) break;
      	nb = fChain->GetEntry(jentry);   nbytes += nb;

		//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
		// DEFINITION
		//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
		
		// START COUNTER
		Int_t n_points_SC = 0; // number of points for graphs.
      	Int_t chn_SC_CLK = 16;
		Double_t voltage_SC_CLK_NC[1023]; // No correction
		Double_t voltage_SC_CLK[1023];
		Double_t time_SC_CLK[1023];

		Int_t enablesum;

		// TOF-WALL
		Int_t n_points_TW = 0; // number of points for graphs.
		Int_t chn_TW_CLK = 16;	
		Double_t voltage_TW_CLK_NC[1023]; // No correction
		Double_t voltage_TW_CLK[1023];
		Double_t time_TW_CLK[1023];

		//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
		// PROCESSING
		//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++

		enablesum = 0;
		
		// START-COUNTER
		for (Int_t i=0; i<1023; i++)     
		{
			voltage_SC_CLK[i]    = board173_waveform[chn_SC_CLK][i]; // Final Amplitude
			voltage_SC_CLK_NC[i] = board173_waveform[chn_SC_CLK][i]; // Amplitude Not Corrected
			time_SC_CLK[i]       = board173_time[chn_SC_CLK][i] * TMath::Power(10,9);

			if (voltage_SC_CLK_NC[i] - voltage_SC_CLK_NC[i-1] >  0.5) enablesum -= 1;
			if (voltage_SC_CLK_NC[i] - voltage_SC_CLK_NC[i-1] < -0.5) enablesum += 1;
			voltage_SC_CLK[i] = voltage_SC_CLK_NC[i] + enablesum; // sommo 1V
			n_points_SC += 1;
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

		//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
		// VISUALIZATION
		//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++

		// START COUNTER

		// UN-CORRECTED SIGNAL
		TGraph *gr_SC_CLK_UC = new TGraph(n_points_SC, time_SC_CLK, voltage_SC_CLK_NC);
		
		TCanvas *c_uncorrect_SC = new TCanvas("uncorrect_SC","uncorrect_SC", 800,800);
		c_uncorrect_SC->SetLeftMargin(0.15);
		c_uncorrect_SC->SetTickx();
   		c_uncorrect_SC->SetTicky();
		gr_SC_CLK_UC->SetLineColor(4); // 4 = blue
		gr_SC_CLK_UC->GetXaxis()->SetTitle("Time [ns]");
		gr_SC_CLK_UC->GetYaxis()->SetTitle("Amplitude [V]");
		gr_SC_CLK_UC->SetTitle(" ");
		gr_SC_CLK_UC->GetXaxis()->SetLimits(0, 60);
		//gr_SC_CLK_UC->SetLineWidth(2);
		gr_SC_CLK_UC->Draw();

		// CORRECTED SIGNAL
		TGraph *gr_SC_CLK = new TGraph(n_points_SC, time_SC_CLK, voltage_SC_CLK);
		
		TCanvas *c_correct_SC = new TCanvas("c_correct_SC","c_correct_SC", 800,800);
		c_correct_SC->SetLeftMargin(0.15);
		c_correct_SC->SetTickx();
   		c_correct_SC->SetTicky();
		gr_SC_CLK->SetLineColor(4); // 4 = blue
		gr_SC_CLK->GetXaxis()->SetTitle("Time [ns]");
		gr_SC_CLK->GetYaxis()->SetTitle("Amplitude [V]");
		gr_SC_CLK->SetTitle(" ");
		gr_SC_CLK->GetXaxis()->SetLimits(0, 60);
		gr_SC_CLK->GetYaxis()->SetLimits(-0.6, 0-6);
		//gr_SC_CLK->SetLineWidth(2);
		gr_SC_CLK->Draw();

		// TOF-WALL
	
		// UN-CORRECTED SIGNAL
		TGraph *gr_TW_CLK_UC = new TGraph(1023, time_TW_CLK, voltage_TW_CLK_NC);
		
		TCanvas *c_uncorrect_TW = new TCanvas("c_uncorrect_TW"," ", 800,800);
		c_uncorrect_TW->SetLeftMargin(0.15);
		c_uncorrect_TW->SetTickx();
   		c_uncorrect_TW->SetTicky();
		gr_TW_CLK_UC->SetLineColor(2); // 4 = blue
		gr_TW_CLK_UC->GetXaxis()->SetTitle("Time [ns]");
		gr_TW_CLK_UC->GetYaxis()->SetTitle("Amplitude [V]");
		gr_TW_CLK_UC->SetTitle(" ");
		gr_TW_CLK_UC->GetXaxis()->SetLimits(0, 60);
		gr_TW_CLK_UC->GetYaxis()->SetLimits(-0.6, 0-6);
		//gr_TW_CLK_UC->SetLineWidth(2);
		gr_TW_CLK_UC->Draw();

		// CORRECTED SIGNAL
		TGraph *gr_TW_CLK = new TGraph(1023, time_TW_CLK, voltage_TW_CLK);
		
		TCanvas *correct_TW = new TCanvas("correct_TW"," ", 800,800);
		correct_TW->SetLeftMargin(0.15);
		correct_TW->SetTickx();
   		correct_TW->SetTicky();
		gr_TW_CLK->SetLineColor(2); // 4 = blue
		gr_TW_CLK->GetXaxis()->SetTitle("Time [ns]");
		gr_TW_CLK->GetYaxis()->SetTitle("Amplitude [V]");
		gr_TW_CLK->SetTitle(" ");
		gr_TW_CLK->GetXaxis()->SetLimits(0, 60);
		gr_TW_CLK->GetYaxis()->SetLimits(-0.6, 0-6);
		//gr_TW_CLK->SetLineWidth(2);
		gr_TW_CLK->Draw();

		TCanvas *c3 = new TCanvas("c3"," ", 800,800);
		
		c3->SetLeftMargin(0.15);
		c3->SetTickx();
   		c3->SetTicky();
		gr_SC_CLK->GetXaxis()->SetLimits(0, 60);
		//gr_SC_CLK->SetLineWidth(2);
		gr_TW_CLK->GetXaxis()->SetLimits(0, 60);
		//gr_TW_CLK->SetLineWidth(2);
		gr_SC_CLK->Draw("AC");
		gr_TW_CLK->Draw("CP");
		auto legend = new TLegend(0.1, 0.7, 0.44, 0.9);
   		legend->AddEntry(gr_SC_CLK,"Start Counter","l");
   		legend->AddEntry(gr_TW_CLK,"Tof-wall","l");
   		legend->Draw();
	}
}
