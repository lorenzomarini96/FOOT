///////////////////////////////////////////////////////////////////////
// hit_map_10nov_slim.C
// Macro to analyze signals from Start Counter detector
// Date: 03 Dicember 2021
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

	gStyle->SetOptFit(10111);

   	// HISTOGRAMS DELTA TIME SC
   	TH1F *hist_time_SC = new TH1F("Time SC","Time SC", 200, 80, 130.0); // [ns]

   	Long64_t nentries = fChain->GetEntriesFast();
   	Long64_t nbytes = 0, nb = 0;
   	Double_t f_CFD = 0.3;
	Int_t status[8];
   
   	// LOOP ON ENTRIES
   	for (Long64_t jentry=0; jentry<nentries; jentry++)
   	{
    	Long64_t ientry = LoadTree(jentry);
      	if (ientry < 0) break;
      	nb = fChain->GetEntry(jentry);   nbytes += nb;	
		
	  	Int_t counter = 0; // Contatotre per contare il numero di canali accesi a seguito di un evento su SC
		Double_t time_mean_SC = 0;
		Double_t time_sum_SC  = 0;
		
      	// LOOP ON CHANNELS OF WAVEDREAM
      	for (Int_t chn=0; chn<8; chn++)
      	{	
       		status[chn] = 1;
			
			// 1° CHECK SIGNAL
        	// FIND MIN AND MAX OF WAVEFORM
        	Double_t min_SC = board173_waveform[chn][100]; // V AMPLITUDE MIN OF CHANNEL [V]
        	Double_t max_SC = board173_waveform[chn][100]; // V AMPLITUDE MAX OF CHANNEL [V]
            
        	for (Int_t t=100; t<900; t++) 
        	{ // LOOP ON SAMPLES OF WAVEFORM
           		if (board173_waveform[chn][t] < min_SC) min_SC = board173_waveform[chn][t];
           		if (board173_waveform[chn][t] > max_SC) max_SC = board173_waveform[chn][t];
        	}
            
			if (max_SC - min_SC > 0.95) status[chn] = 0;  // NOISE
            
        	if (max_SC - min_SC < 0.010) status[chn] = 0; // NOISE
               
        	if (status[chn]==0) board173_hit[chn] = 0;
		}

      	// LOOP ON CHANNELS OF WAVEDREAM
      	for (Int_t chn=0; chn<8; chn++)
		{
	        if (board173_hit[chn]==1)
			{
            	// INITIALIZE VALUES
            	Double_t voltage_SC;                         
				Double_t a_fit;
				Double_t b_fit;
            	Double_t v_base_SC;                               // V BASELINE (PEDESTAL) [V]
            	Double_t sigma_v_base_SC;                         // V BASELINE (PEDESTAL) [V]
            	Double_t v_ampl_SC;                               // V AMPLITUDE OF CHANNEL [V]
            	Double_t v_peak_SC = board173_waveform[chn][300]; // V MINIMUM (PEAK)[V]
            	Double_t t_peak_SC;                               // TIME CORRESPONDING TO PEAK
            	Double_t v_th_SC;                                 // V THRESHOLD [V]
            	Double_t t_stop_SC;                               // TIME STOP OF SIGNAL[ns]
            	Double_t i_start_SC;                              // V START OF SIGNAL[V]
            	Double_t i_stop_SC;                               // V STOP OF SIGNAL [V]
            	Double_t i_min_position_SC;                       // POSITION OF V PEAK
            	Double_t time_SC[8];                              // TIME OF WF OF CHANNEL [ns]

				// V BASELINE   
               	TH1F *wf_SC = new TH1F("wf_SC","wf_SC", 30, 0.30, 0.60);
               	for (Int_t t=10; t<250; t++) wf_SC->Fill(board173_waveform[chn][t]);
               	v_base_SC = wf_SC->GetMean();
               	sigma_v_base_SC = wf_SC->GetRMS(); // TO EVALUATE THE NOISE (FOR FUTURES...)

               	wf_SC->SetDirectory(0); // remove an histogram from the gDirectory list
            	
				// V PEAK (MINIMUM)
               	for (Int_t i=100; i<800; i++)
				{ // LOOP ON SAMPLES OF WAVEFORM
                  	if (board173_waveform[chn][i] < v_peak_SC) 
					{
						i_min_position_SC = i;
                    	v_peak_SC = board173_waveform[chn][i];
                    	t_peak_SC = board173_time[chn][i] * TMath::Power(10,9); // Convert time from [s] in [ns]
                	}
               	}

	            // V AMPLITUDE
               	v_ampl_SC = v_base_SC - v_peak_SC;
            
               	// V THRESHOLD                                    
               	v_th_SC = v_base_SC - f_CFD * (v_base_SC - v_peak_SC);

				// DATA TO FIT
				Int_t n = 5;

               	// TIME OF CHANNEL (CFD algorithm)
               	for (Int_t t=i_min_position_SC; ; t--) 
				{ // LOOP ON SAMPLES OF WAVEFORM
                  	if (board173_waveform[chn][t] > v_th_SC) 
					{
						TF1 *f_fit = new TF1("f_fit", "pol1", board173_time[chn][t-2], board173_time[chn][t+2]);

						Double_t v_data_fit[5] = {board173_waveform[chn][t-2], board173_waveform[chn][t-1], board173_waveform[chn][t] ,board173_waveform[chn][t+1], board173_waveform[chn][t+2]};
						Double_t t_data_fit[5] = {board173_time[chn][t-2], board173_time[chn][t-1], board173_time[chn][t] ,board173_time[chn][t+1], board173_time[chn][t+2]};
						
						TGraph *gr = new TGraph(n,t_data_fit, v_data_fit);
						gr->Fit("f_fit","Qr");						
						a_fit = f_fit->GetParameter(1);
						b_fit = f_fit->GetParameter(0);

						time_SC[chn] = (v_th_SC - b_fit)/a_fit * TMath::Power(10,9);

						delete f_fit;
						delete gr;
                     	break;
                  	}
				}
				
				time_sum_SC += time_SC[chn];
				//cout << "time_mean_SC [ns]  = " << time_mean_SC << endl; 
				//cout << "counter            = " << counter << endl; 	
				counter += 1;

				// MEAN TIME OF SC
				time_mean_SC = time_sum_SC/counter;

				if (1) 
				{
                	std::cout << "\nStart Counter"                              << std::endl;
                  	std::cout << "\nEntry               = " << jentry           << std::endl;
                  	std::cout << "\nchn                 = " << chn              << std::endl;
                  	//std::cout << "\nmin [V]             = " << min_SC           << std::endl;
                  	//std::cout << "\nmax [V]             = " << max_SC           << std::endl;
                  	std::cout << "\n------------------------------------------" << std::endl;
                  	std::cout << "\nv_base       [V]    = " << v_base_SC        << std::endl;
                  	std::cout << "\nsigma_v_base [V]    = " << sigma_v_base_SC  << std::endl;
                  	std::cout << "\nv_peak       [V]    = " << v_peak_SC        << std::endl;
                  	std::cout << "\nt_peak       [ns]   = " << t_peak_SC        << std::endl;
                	std::cout << "\nv_ampl       [V]    = " << v_ampl_SC        << std::endl;
                  	std::cout << "\nv_th         [V]    = " << v_th_SC          << std::endl;
                 	std::cout << "\ntime         [ns]   = " << time_SC[chn]     << std::endl;
					cout << "********************************************************" << endl;
					cout << "counter            = " << counter      << endl; 	
					cout << "time_sum_SC [ns]   = " << time_sum_SC  << endl;
					cout << "time_mean_SC [ns]  = " << time_mean_SC << endl; 
					cout << "********************************************************" << endl;
				}
			} // END if (board173_hit[chn]==1)	
			//if (time_mean_SC>0) hist_time_SC->Fill(time_mean_SC);
		} // END LOOP ON CHANNELS OF WAVEDREAM
		if (time_mean_SC>0) hist_time_SC->Fill(time_mean_SC);
		
   	} // END LOOP ON ENTRIES

   	//------------------------------
   	// HIST  TIME
   	//------------------------------
      	
   	TCanvas *c_time_SC = new TCanvas("time_SC", "time_SC", 1200, 1200);
	c_time_SC->SetTicky();
   	c_time_SC->SetTickx();
   	c_time_SC->SetLeftMargin(0.15);
   	hist_time_SC->GetXaxis()->SetTitle("T_{SC} [ns]");
   	hist_time_SC->SetFillColor(38);
   	hist_time_SC->GetYaxis()->SetTitle("Counts");
   	//hist_time_SC->Fit("gaus", "Q");
   	hist_time_SC->Draw();
	//c_time_SC->SaveAs("figures/delta_mean_time.pdf");
	 
}
