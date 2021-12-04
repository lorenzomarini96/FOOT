///////////////////////////////////////////////////////////////////////
// hit_map_10nov_slim.C
// Macro to reconstruct the hit map of the tofwall detector.
// Date: 22 November 2021
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

   	//------------------------------
   	// HIST FOR TIME RESOLUTION
   	//------------------------------
   	TH1D *hist_TOF = new TH1D("hist_TOF", "hist_TOF", 100, 19.5, 21.5); // [ns]
   

   	Double_t f_CFD = 0.3;         // FRATION FOR COMPUTE TIME
	Int_t status[8];
   
  	// LOOP ON ENTRIES
  	for (Long64_t jentry=0; jentry<nentries; jentry++)
   	{
    	Long64_t ientry = LoadTree(jentry);
      	if (ientry < 0) break;
      	nb = fChain->GetEntry(jentry);   nbytes += nb;

		Double_t time_TOF = 0.;
		Double_t time_TW  = 0.;
		//Double_t time_mean??????  = 0.;

		//****************************************************************************
		// TOF-WALL
		//****************************************************************************
      
      	// LOOP ON CHANNELS OF WAVEDREAM 166 - X VIEW (BAR 8 TO 10); Y VIEW (BAR 28 to 30)
		// 1° CHECK SIGNAL
      	for (Int_t chn=0; chn<12; chn++) 
      	{
       		Int_t status = 1;

        	// FIND MIN AND MAX OF WAVEFORM
        	Double_t min_166 = board166_waveform[chn][100]; // V AMPLITUDE MIN OF CHANNEL [V]
        	Double_t max_166 = board166_waveform[chn][100]; // V AMPLITUDE MAX OF CHANNEL [V]
            
        	for (Int_t t=100; t<900; t++) 
        	{ // LOOP ON SAMPLES OF WAVEFORM
           		if (board166_waveform[chn][t] < min_166) min_166 = board166_waveform[chn][t];
           		if (board166_waveform[chn][t] > max_166) max_166 = board166_waveform[chn][t];
        	}
            
			if (max_166 - min_166 > 0.95) status = 0;  // NOISE
            
        	if (max_166 - min_166 < 0.010) status = 0; // REMEMBER ZEROSUPPRESSION
               
        	if (status==0) board166_hit[chn] = 0;
		}
		
		Double_t Mean_Time_Bar_9_X = 0;                    // T_bar_layer = (T_L + T_R)/2
   		Double_t Mean_Time_Bar_9_Y = 0;                    // T_bar_front = (T_L + T_R)/2

		// LOOP ON CHANNELS OF WAVEDREAM
      	for (Int_t chn=0; chn<12; chn++) 
      	{
			if (board166_hit[chn] == 1)
			{
            	// INITIALIZE VALUES

            	Double_t voltage_166;                         
				Double_t voltage_fit[4];                           // ARRAY OF VOLTAGE TO MAKE FIT (TIME RESOLUTION)
				Double_t a_fit;
				Double_t b_fit;
            	Double_t v_base_166;                               // V BASELINE (PEDESTAL) [V]
            	Double_t sigma_v_base_166;                         // V BASELINE (PEDESTAL) [V]
            	Double_t v_ampl_166;                               // V AMPLITUDE OF CHANNEL [V]
            	Double_t v_peak_166 = board166_waveform[chn][300]; // V MINIMUM (PEAK) [V]
            	Double_t t_peak_166;                               // TIME CORRESPONDING TO PEAK
            	Double_t v_th_166;                                 // V THRESHOLD [V]
            	Double_t t_stop_166;                               // TIME STOP OF SIGNAL  [ns]
            	Double_t i_start_166;                              // V START OF SIGNAL [V]
            	Double_t i_stop_166;                               // V STOP OF SIGNAL  [V]
            	Double_t i_min_position_166;                       // POSITION OF V PEAK
            	Double_t time_166[16];                             // TIME OF WF OF CHANNEL [ns]
				Double_t mean_time;
				//Double_t time_TW;                                  // TIME OF TOWFWALL: (T_bar_front + T_bar_layer)/2
				
            	
				// V BASELINE   
               	TH1F *wf_166 = new TH1F("wf_166","wf_166", 30, 0.30, 0.60);
               	for (Int_t t=10; t<250; t++) wf_166->Fill(board166_waveform[chn][t]);
               	v_base_166 = wf_166->GetMean();
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
               	for (Int_t t=i_min_position_166; ; t--) { // LOOP ON SAMPLES OF WAVEFORM
                  	if (board166_waveform[chn][t] > v_th_166) 
					{
						TF1 *f_fit = new TF1("f_fit", "pol1", board166_time[chn][t-2], board166_time[chn][t+2]); // Range board166_waveform[chn][t-2], board166_waveform[chn][t+2]
						

						Double_t v_data_fit[5] = {board166_waveform[chn][t-2], board166_waveform[chn][t-1], board166_waveform[chn][t] ,board166_waveform[chn][t+1], board166_waveform[chn][t+2]};
						Double_t t_data_fit[5] = {board166_time[chn][t-2], board166_time[chn][t-1], board166_time[chn][t] ,board166_time[chn][t+1], board166_time[chn][t+2]};
						
						TGraph *gr = new TGraph(n,t_data_fit, v_data_fit);
						gr->Fit("f_fit","Qr");						
						a_fit = f_fit->GetParameter(1);
						b_fit = f_fit->GetParameter(0);

						time_166[chn] = (v_th_166 - b_fit)/a_fit * TMath::Power(10,9);

						delete f_fit;
						delete gr;
                     	break;
                  	}
               	}
				
				// MEAN TIME OF BAR [ns];
				if (chn==3)      
				{
					Mean_Time_Bar_9_X = (time_166[3] + time_166[2])/2;  // T_bar_layer = (T_R + T_L) / 2
					//cout << "Mean_Time_Bar_9_X = " << Mean_Time_Bar_9_X << endl;
				}
				else if (chn==9)
				{
					Mean_Time_Bar_9_Y = (time_166[9] + time_166[8])/2;  // T_bar_front = (T_R + T_L) / 2
					//cout << "Mean_Time_Bar_9_Y" << Mean_Time_Bar_9_Y << endl;
				} 

				// TIME OF TOFWALL
				if (Mean_Time_Bar_9_X > 0. && Mean_Time_Bar_9_Y > 0.) 
				{
					time_TW = (Mean_Time_Bar_9_X + Mean_Time_Bar_9_Y)/2;
					//cout << "time_TW = (Mean_Time_Bar_9_X + Mean_Time_Bar_9_Y)/2 = " << time_TW << endl;
				}
               //***********************************************************************************************************

				if (0)
				//if (Mean_Time_Bar_9_X > 0. && Mean_Time_Bar_9_Y > 0.) 
				{
                	std::cout << "\n**************************"       << std::endl;
                	std::cout << "\nWD 166"                           << std::endl;
                  	std::cout << "\nEntry                   = "       << jentry             << std::endl;
                  	std::cout << "\nchn                     = "       << chn                << std::endl;
                  	std::cout << "\n------------------------------------------"             << std::endl;
                  	std::cout << "\nv_base_166       [V]    = "       << v_base_166         << std::endl;
                  	std::cout << "\nsigma_v_base_166 [V]    = "       << sigma_v_base_166   << std::endl;
                  	std::cout << "\nv_peak_166       [V]    = "       << v_peak_166         << std::endl;
                  	std::cout << "\nt_peak_166       [ns]   = "       << t_peak_166         << std::endl;
                  	std::cout << "\nv_ampl_166       [V]    = "       << v_ampl_166         << std::endl;
                  	std::cout << "\nv_th_166         [V]    = "       << v_th_166           << std::endl;
                  	std::cout << "\ntime_166         [ns]   = "       << time_166[chn]      << std::endl;
					std::cout << "\nMean_Time_Bar_9_X    [ns] = " << Mean_Time_Bar_9_X  << std::endl;
					std::cout << "\nMean_Time_Bar_9_Y    [ns] = " << Mean_Time_Bar_9_Y  << std::endl;
					std::cout << "\ntime_TOF             [ns] = " << time_TOF           << std::endl;
               	}
				/*
				if (Mean_Time_Bar_9_X > 0. && Mean_Time_Bar_9_Y > 0.)
				{
					cout << "********************************************************" << endl;
					cout << "TOFWALL" << endl;
					cout << "time_TW [ns]  = " << time_TW << endl; 
				}
				*/
			} // END if (board166_hit[chn] == 1 && board166_hit[chn+1] == 1)
      	} // END CHANNEL LOOP
		
		//****************************************************************************
		// START-COUNTER
		//****************************************************************************
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
				//if (time_sum_SC > 0.) time_mean_SC = time_sum_SC/counter;
				time_mean_SC = time_sum_SC/counter;

				if (0) 
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
					//cout << "********************************************************" << endl;
					//cout << "counter            = " << counter      << endl; 	
					//cout << "time_sum_SC [ns]   = " << time_sum_SC  << endl;
					//cout << "time_mean_SC [ns]  = " << time_mean_SC << endl; 
					//cout << "********************************************************" << endl;
				}
			} // END if (board173_hit[chn]==1)	
		} // END LOOP ON CHANNELS OF WAVEDREAM

		//if (Mean_Time_Bar_9_X > 0. && Mean_Time_Bar_9_Y > 0. && time_mean_SC > 0.)
		if (0)
		{
			time_TOF = time_TW - time_mean_SC;
			hist_TOF->Fill(time_TOF);

			cout << "********************************************************" << endl;
			cout << "START COUNTER" << endl;
			cout << "time_mean_SC [ns]  = " << time_mean_SC << endl; 
			cout << "-------------------------------------------------------------\n" << endl;
			cout << "TOFWALL" << endl;
			cout << "time_TW [ns]  = " << time_TW << endl;
			cout << "\n-------------------------------------------------------------\n" << endl;
			cout << "time_TOF = time_TW - time_mean_SC = " << time_TW << " - " << time_mean_SC << " = " << time_TOF << endl;
			cout << "********************************************************" << endl;
		}
		/*
		if (time_mean_SC > 0. && time_TW > 0. )
		{
			Double_t time_TOF;
			time_TOF = time_TW - time_mean_SC;

			cout << "****************************************************************" << endl;
			cout << "*                            TOF                               *" << endl;
			cout << "****************************************************************" << endl;
			cout << "time_TOF = time_TW - time_mean_SC = " << time_TOF << " - " << time_mean_SC << " = " << time_TOF << endl;
			hist_TOF->Fill(time_TOF);
		}
		*/



   	} // END LOOP ON ENTRIES

   	//------------------------------
   	// HIST TOF
   	//------------------------------
	
   	TCanvas *c_TOF = new TCanvas("c_delta_mean_time", "c_delta_mean_time", 1200, 1200);
   	c_TOF->SetTickx();
   	c_TOF->SetTicky();
   	c_TOF->SetLeftMargin(0.15);
   	hist_TOF->GetXaxis()->SetTitle("TOF = T_{TW} - T_{SC} [ns]");
   	
   	hist_TOF->GetYaxis()->SetTitle("Counts");
   	hist_TOF->Fit("gaus", "Q");
   	hist_TOF->Draw();
	c_TOF->SaveAs("figures/c_TOF.pdf");
	
}
