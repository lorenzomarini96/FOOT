//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// clock_TW_v2.C																										    //
// Macro to analyze clock signals.																					    //
// Date: 06 December 2021																							    //
// Author: L. Marini																								    //
// Note: It is necessary to analyze all the CLKs event-by-event in order to align all the channels of the TW to the STC //
// How to use:
// 1) from shell: root -l file_1000_to_1020.root
// 2) rec->Draw("board173.waveform[7]:board173.time[7]", "Entry$==0", "L")
// Select the channel and the entry you want to plot.
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

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
	for (Long64_t jentry=0; jentry<1; jentry++)           // ONLY SECOND EVENT
   	{
    	Long64_t ientry = LoadTree(jentry);
      	if (ientry < 0) break;
      	nb = fChain->GetEntry(jentry);   nbytes += nb;
		
		//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
		// DEFINITION
		//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
    	
		//Int_t chn = 7;                                    // ARTIFACT
		Int_t chn = 1;                                      // GOOD SIGNAL
		Double_t f_CFD = 0.3;         					    // FRATION FOR COMPUTE TIME
        Double_t voltage_173[1023];                         
		Double_t time_173[1023];                         
		Double_t a_fit;
		Double_t b_fit;
    	Double_t v_base_173;                                 // V BASELINE (PEDESTAL) [V]
        Double_t sigma_v_base_173;                           // V BASELINE (PEDESTAL) [V]
    	Double_t v_ampl_173;                                 // V AMPLITUDE OF CHANNEL [V]
    	Double_t v_peak_173 = board173_waveform[chn][300];  // V MINIMUM (PEAK) [V]
		Double_t t_peak_173;                                 // TIME CORRESPONDING TO PEAK
        Double_t v_th_173;                                   // V THRESHOLD [V]
		Double_t time;                                       // TIME CORRESPONGIN TO V_TH
        Double_t t_stop_173;                                 // TIME STOP OF SIGNAL  [ns]
        Double_t i_start_173;                                // V START OF SIGNAL [V]
        Double_t i_stop_173;                                 // V STOP OF SIGNAL  [V]
        Double_t i_min_position_173;                         // POSITION OF V PEAK
            	
		// V BASELINE   
        TH1F *wf_173 = new TH1F("wf_173","wf_173", 30, 0.30, 0.60);
        
		for (Int_t i=10; i<250; i++) wf_173->Fill(board173_waveform[chn][i]);
        
		v_base_173       = wf_173->GetMean();
        sigma_v_base_173 = wf_173->GetRMS(); // TO EVALUATE THE NOISE (FOR FUTURES...)

        wf_173->SetDirectory(0); // remove an histogram from the gDirectory list
            	
		// V PEAK (MINIMUM)
        for (Int_t i=100; i<800; i++)
		{ // LOOP ON SAMPLES OF WAVEFORM
            if (board173_waveform[chn][i] < v_peak_173)
			{
				i_min_position_173 = i;
                v_peak_173 = board173_waveform[chn][i];
                t_peak_173 = board173_time[chn][i] * TMath::Power(10,9); // Convert time from [s] in [ns]
            }
        }

	    // V AMPLITUDE
        v_ampl_173 = v_base_173 - v_peak_173;
            
        // V THRESHOLD                                    
        v_th_173 = v_base_173 - f_CFD * (v_base_173 - v_peak_173);
				
		// TIME OF CHANNEL
		Int_t n = 5;
        for (Int_t i=i_min_position_173; ; i--)
		{
			// LOOP ON SAMPLES OF WAVEFORM
            if (board173_waveform[chn][i] > v_th_173)
			{
				TF1 *f_fit = new TF1("f_fit", "pol1", board173_time[chn][i-2], board173_time[chn][i+2]); // Range board173_waveform[chn][t-2], board173_waveform[chn][t+2]

				Double_t v_data_fit[5] = {board173_waveform[chn][i-2], board173_waveform[chn][i-1], board173_waveform[chn][i], board173_waveform[chn][i+1], board173_waveform[chn][i+2]};
				Double_t t_data_fit[5] = {board173_time[chn][i-2], board173_time[chn][i-1], board173_time[chn][i], board173_time[chn][i+1], board173_time[chn][i+2]};
						
				TGraph *gr = new TGraph(n,t_data_fit, v_data_fit);
				gr->Fit("f_fit","Qr");						
				a_fit = f_fit->GetParameter(1);
				b_fit = f_fit->GetParameter(0);

				time = (v_th_173 - b_fit)/a_fit * TMath::Power(10,9);

				delete f_fit;
				delete gr;
                break;
			}
        }
        
		// DATA TO PLOT
		for (Int_t i = 0; i< 1023; i++)
		{
			time_173[i]    = board173_time[chn][i] * TMath::Power(10,9);
			voltage_173[i] = board173_waveform[chn][i];
		}
		//***********************************************************************************************************

        std::cout << "\n**************************"                           << std::endl;
        std::cout << "\nSTART COUNTER"                                        << std::endl;
		std::cout << "\nWD 173"                                               << std::endl;
        std::cout << "\nEntry                    "       << jentry            << std::endl;
        std::cout << "\nchn                      "       << chn               << std::endl;
        std::cout << "\n------------------------------------------"           << std::endl;
        std::cout << "\nv_base_173         [V]   "       << v_base_173        << std::endl;
        std::cout << "\nsigma_v_base_173   [V]   "       << sigma_v_base_173  << std::endl;
        std::cout << "\nv_peak_173         [V]   "       << v_peak_173        << std::endl;
        std::cout << "\nt_peak_173         [ns]  "       << t_peak_173        << std::endl;
        std::cout << "\nv_ampl_173         [V]   "       << v_ampl_173        << std::endl;
        std::cout << "\nv_th_173           [V]   "       << v_th_173          << std::endl;
        std::cout << "\ntime               [ns]  "       << time              << std::endl;			
		
		//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
		// VISUALIZATION
		//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++

		TGraph *gr_WF_SC = new TGraph(1023, time_173, voltage_173);
	
		TCanvas *c_WF_SC = new TCanvas("c4"," ", 800,800);
		c_WF_SC->SetLeftMargin(0.15);
		c_WF_SC->SetTickx();
   		c_WF_SC->SetTicky(); 
		gr_WF_SC->GetXaxis()->SetTitle("Time [ns]");
		gr_WF_SC->GetYaxis()->SetTitle("Voltage [V]");
		gr_WF_SC->SetTitle(" ");
	
		// SHOW CFD METHOD??...
		
		gr_WF_SC->SetLineColor(4);
		gr_WF_SC->Draw("AC");
		//gr_WF_SC->Draw(); // Per artifacts
		
		if (1)
		{
			TLine *l_v_base = new TLine(0, v_base_173, 350, v_base_173);
			l_v_base->SetLineColor(kBlack);
			l_v_base->SetLineWidth(2);
			l_v_base->SetLineStyle(2); // 2 = --
			l_v_base->Draw();

			TLine *l_time = new TLine(time, 0.345, time, v_th_173);
			l_time->SetLineColor(kBlack);
			l_time->SetLineWidth(2);
			l_time->SetLineStyle(2); // 2 = --
			l_time->Draw();

			TLine *l_v_th_166 = new TLine(0, v_th_173, time, v_th_173);
			l_v_th_166->SetLineColor(kBlack);
			l_v_th_166->SetLineWidth(2);
			l_v_th_166->SetLineStyle(2); // 2 = --
			l_v_th_166->Draw();

			TLine *l_v_peak = new TLine(0, v_peak_173, t_peak_173, v_peak_173);
			l_v_peak->SetLineColor(kBlack);
			l_v_peak->SetLineWidth(2);
			l_v_peak->SetLineStyle(2); // 2 = --
			l_v_peak->Draw();

			TText *t_time = new TText(time - 0.10 * time, v_th_173 - 0.03, "Time");
			t_time->SetTextAlign(22);
			t_time->SetTextColor(kBlack);
			t_time->SetTextFont(45);
			t_time->SetTextSize(25);
			t_time->SetTextAngle(90);
			t_time->Draw();

			TText *t_v_base = new TText(time/2, v_base_173 + 0.03 * v_base_173, "V baseline");
			t_v_base->SetTextAlign(22);
			t_v_base->SetTextColor(kBlack);
			t_v_base->SetTextFont(45);
			t_v_base->SetTextSize(25);
			t_v_base->SetTextAngle(0);
			t_v_base->Draw();

			TText *t_v_th = new TText(time/2, v_th_173 + 0.01 * v_th_173, "V threshold");
			t_v_th->SetTextAlign(22);
			t_v_th->SetTextColor(kBlack);
			t_v_th->SetTextFont(45);
			t_v_th->SetTextSize(25);
			t_v_th->SetTextAngle(0);
			t_v_th->Draw();

			TText *t_v_peak = new TText(t_peak_173/2, v_peak_173 + 0.013 * v_peak_173,"V peak");
			t_v_peak->SetTextAlign(22);
			t_v_peak->SetTextColor(kBlack);
			t_v_peak->SetTextFont(45);
			t_v_peak->SetTextSize(25);
			t_v_peak->SetTextAngle(0);
			t_v_peak->Draw();
		}

		c_WF_SC->SaveAs("/Users/lorenzomarini/Desktop/clock_TEX/WF_SC_computed.tex");

		/*
		TLegend *legend = new TLegend(0.5,0.5,0.8,0.8);
		legend->AddEntry((TObject*)0, TString::Format("#chi^{2} / ndf = %.3f/%d"             , f_fit_N_TW_CLK_vs_ZeroCrossingPoint->GetChisquare(),  f_fit_N_TW_CLK_vs_ZeroCrossingPoint->GetNDF()), "");
		legend->AddEntry((TObject*)0, TString::Format("#varphi_{CLK,TW} = %.2f #pm %.2f [ns]", f_fit_N_TW_CLK_vs_ZeroCrossingPoint->GetParameter(0), f_fit_N_TW_CLK_vs_ZeroCrossingPoint->GetParError(0)), "");
		legend->AddEntry((TObject*)0, TString::Format("T_{CLK,TW} = %.2f #pm %.2f [ns]"      , f_fit_N_TW_CLK_vs_ZeroCrossingPoint->GetParameter(1), f_fit_N_TW_CLK_vs_ZeroCrossingPoint->GetParError(1)), "");	
		legend->SetBorderSize(0);
		legend->SetTextSize(0.033);
		legend->Draw();
		*/
	}

}
