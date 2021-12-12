//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// clock_TW_v2.C																										    //
// Macro to analyze clock signals.																					    //
// Date: 06 December 2021																							    //
// Author: L. Marini																								    //
// Note: It is necessary to analyze all the CLKs event-by-event in order to align all the channels of the TW to the STC //
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
	for (Long64_t jentry=1; jentry<2; jentry++)           // ONLY SECOND EVENT
   	{
    	Long64_t ientry = LoadTree(jentry);
      	if (ientry < 0) break;
      	nb = fChain->GetEntry(jentry);   nbytes += nb;
		
		//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
		// DEFINITION
		//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
    	
		Int_t chn = 3;
		Double_t f_CFD = 0.3;         					  // FRATION FOR COMPUTE TIME
        Double_t voltage_166[1023];                         
		Double_t time_166[1023];                         
		Double_t a_fit;
		Double_t b_fit;
    	Double_t v_base_166;                               // V BASELINE (PEDESTAL) [V]
        Double_t sigma_v_base_166;                         // V BASELINE (PEDESTAL) [V]
    	Double_t v_ampl_166;                               // V AMPLITUDE OF CHANNEL [V]
    	Double_t v_peak_166 = board166_waveform[chn][300]; // V MINIMUM (PEAK) [V]
		Double_t t_peak_166;                               // TIME CORRESPONDING TO PEAK
        Double_t v_th_166;                                 // V THRESHOLD [V]
		Double_t time;                                     // TIME CORRESPONGIN TO V_TH
        Double_t t_stop_166;                               // TIME STOP OF SIGNAL  [ns]
        Double_t i_start_166;                              // V START OF SIGNAL [V]
        Double_t i_stop_166;                               // V STOP OF SIGNAL  [V]
        Double_t i_min_position_166;                       // POSITION OF V PEAK
            	
		// V BASELINE   
        TH1F *wf_166 = new TH1F("wf_166","wf_166", 30, 0.30, 0.60);
        
		for (Int_t i=10; i<250; i++) wf_166->Fill(board166_waveform[chn][i]);
        
		v_base_166       = wf_166->GetMean();
        sigma_v_base_166 = wf_166->GetRMS(); // TO EVALUATE THE NOISE (FOR FUTURES...)

        wf_166->SetDirectory(0); // remove an histogram from the gDirectory list
            	
		// V PEAK (MINIMUM)
        for (Int_t i=100; i<800; i++)
		{ // LOOP ON SAMPLES OF WAVEFORM
            if (board166_waveform[chn][i] < v_peak_166) 
			{
				i_min_position_166 = i;
                v_peak_166 = board166_waveform[chn][i];
                t_peak_166 = board166_time[chn][i] * TMath::Power(10,9); // Convert time from [s] in [ns]
            }
        }

	    // V AMPLITUDE
        v_ampl_166 = v_base_166 - v_peak_166;                     
            
        // V THRESHOLD                                    
        v_th_166 = v_base_166 - f_CFD * (v_base_166 - v_peak_166);
				
		// TIME OF CHANNEL
		Int_t n = 5;
        for (Int_t i=i_min_position_166; ; i--) 
		{
			// LOOP ON SAMPLES OF WAVEFORM
            if (board166_waveform[chn][i] > v_th_166) 
			{
				TF1 *f_fit = new TF1("f_fit", "pol1", board166_time[chn][i-2], board166_time[chn][i+2]); // Range board166_waveform[chn][t-2], board166_waveform[chn][t+2]

				Double_t v_data_fit[5] = {board166_waveform[chn][i-2], board166_waveform[chn][i-1], board166_waveform[chn][i], board166_waveform[chn][i+1], board166_waveform[chn][i+2]};
				Double_t t_data_fit[5] = {board166_time[chn][i-2], board166_time[chn][i-1], board166_time[chn][i], board166_time[chn][i+1], board166_time[chn][i+2]};
						
				TGraph *gr = new TGraph(n,t_data_fit, v_data_fit);
				gr->Fit("f_fit","Qr");						
				a_fit = f_fit->GetParameter(1);
				b_fit = f_fit->GetParameter(0);

				time = (v_th_166 - b_fit)/a_fit * TMath::Power(10,9);

				delete f_fit;
				delete gr;
                break;
			}
        }
        
		// DATA TO PLOT
		for (Int_t i = 0; i< 1023; i++)
		{
			time_166[i]    = board166_time[chn][i] * TMath::Power(10,9);
			voltage_166[i] = board166_waveform[chn][i];
		}
		//***********************************************************************************************************

        std::cout << "\n**************************"                           << std::endl;
        std::cout << "\nTOF-WALL"                                             << std::endl;
		std::cout << "\nWD 166"                                               << std::endl;
        std::cout << "\nEntry                    "       << jentry            << std::endl;
        std::cout << "\nchn                      "       << chn               << std::endl;
        std::cout << "\n------------------------------------------"           << std::endl;
        std::cout << "\nv_base_166         [V]   "       << v_base_166        << std::endl;
        std::cout << "\nsigma_v_base_166   [V]   "       << sigma_v_base_166  << std::endl;
        std::cout << "\nv_peak_166         [V]   "       << v_peak_166        << std::endl;
        std::cout << "\nt_peak_166         [ns]  "       << t_peak_166        << std::endl;
        std::cout << "\nv_ampl_166         [V]   "       << v_ampl_166        << std::endl;
        std::cout << "\nv_th_166           [V]   "       << v_th_166          << std::endl;
        std::cout << "\ntime               [ns]  "       << time              << std::endl;			
		
		//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
		// VISUALIZATION
		//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++

		TGraph *gr_WF_TW = new TGraph(1023, time_166, voltage_166);
	
		TCanvas *c_WF_TW = new TCanvas("c4"," ", 800,800);
		c_WF_TW->SetLeftMargin(0.15);
		c_WF_TW->SetTickx();
   		c_WF_TW->SetTicky(); 
		gr_WF_TW->GetXaxis()->SetTitle("Time [ns]");
		gr_WF_TW->GetYaxis()->SetTitle("Voltage [V]");
		gr_WF_TW->SetTitle(" ");
	
		// SHOW CFD METHOD??...
		
		gr_WF_TW->SetLineColor(2);
		gr_WF_TW->Draw("AC");
		

		if (0)
		{
			TLine *l_v_base = new TLine(0, v_base_166, 350, v_base_166);
			l_v_base->SetLineColor(kBlack);
			l_v_base->SetLineWidth(2);
			l_v_base->SetLineStyle(2); // 2 = --
			l_v_base->Draw();

			TLine *l_time = new TLine(time, 0.035, time, v_th_166);
			l_time->SetLineColor(kBlack);
			l_time->SetLineWidth(2);
			l_time->SetLineStyle(2); // 2 = --
			l_time->Draw();

			TLine *l_v_th_166 = new TLine(0, v_th_166, time, v_th_166);
			l_v_th_166->SetLineColor(kBlack);
			l_v_th_166->SetLineWidth(2);
			l_v_th_166->SetLineStyle(2); // 2 = --
			l_v_th_166->Draw();

			TText *t_time = new TText(time - 0.10 * time,v_th_166/2,"Time");
			t_time->SetTextAlign(22);
			t_time->SetTextColor(kBlack);
			t_time->SetTextFont(45);
			t_time->SetTextSize(25);
			t_time->SetTextAngle(90);
			t_time->Draw();

			TText *t_v_base = new TText(time/2, v_base_166+0.03 * v_base_166,"V baseline");
			t_v_base->SetTextAlign(22);
			t_v_base->SetTextColor(kBlack);
			t_v_base->SetTextFont(45);
			t_v_base->SetTextSize(25);
			t_v_base->SetTextAngle(0);
			t_v_base->Draw();

			TText *t_v_th = new TText(time/2, v_th_166 + 0.04 * v_th_166,"V threshold");
			t_v_th->SetTextAlign(22);
			t_v_th->SetTextColor(kBlack);
			t_v_th->SetTextFont(45);
			t_v_th->SetTextSize(25);
			t_v_th->SetTextAngle(0);
			t_v_th->Draw();
		}

		c_WF_TW->SaveAs("/Users/lorenzomarini/Desktop/clock_TEX/WF_TW_CFD.tex");

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
