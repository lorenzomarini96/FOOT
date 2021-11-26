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


void rec::Loop()
{
   if (fChain == 0) return;

   Long64_t nentries = fChain->GetEntriesFast();
   Long64_t nbytes = 0, nb = 0;


   //FILE .ROOT TO SAVE HISTOGRAM
   //TFile hfile("hitmap_hist.root", "RECREATE");
   
   gStyle->SetOptFit(10111);

   // HISTOGRAM
   TH2D *hist = new TH2D("hist", "Hit-map", 20, -0.5, 19.5, 20, 19.5, 39.5);
   TH1D *hist_project = new TH1D("hist_project", "Hit-map XY projections", 40, -0.5, 39.5);


   	//------------------------------
   	// HIST FOR TIME RESOLUTION
   	//------------------------------
   	TH1D *hist_mean_time = new TH1D("hist_mean_time", "#DeltaT_{TW} = #bar{T}_{front} - #bar{T}_{rear}", 150, -1.5, 1.5);
   

   	//============================================================================================================================
   	// WAVEDREAM 166 - X VIEW (BAR 8 TO 10); Y VIEW (BAR 28 to 30)

	//***********************************************************************************************************************
   	//------------------------------
   	// HISTOGRAMS Q VS TIME WD166
   	//------------------------------

   	TH2D *hist_charge_time_166[8];
   	char name_charge_time_166[20];
   	char title_charge_time_166[100];

   	for (Int_t b=0; b<6; b++) 
	{	
		if (b<3)
		{
			int x_bar = b+8;
		    sprintf(name_charge_time_166, "hist_charge_time_166_bar%d", b);
      		sprintf(title_charge_time_166, "Bar %d", x_bar);
		}
		else
		{
			int y_bar = b+25;
			sprintf(name_charge_time_166, "hist_charge_time_166_bar%d", y_bar);
      		sprintf(title_charge_time_166, "Bar %d", y_bar);
		}
      	hist_charge_time_166[b] = new TH2D(name_charge_time_166,title_charge_time_166, 50, -2.0, 2.0, 50, -5.0, 5.0);
      	hist_charge_time_166[b]->GetXaxis()->SetTitle("Log(Q_L/Q_R)");
      	hist_charge_time_166[b]->GetYaxis()->SetTitle("(t_L - t_R) [ns]");
   	}
   
   	//------------------------------
   	// HISTOGRAMS CHARGHE WD166
   	//------------------------------
   	TH1F *hist_q_166[16];
   	char name_q_166[20];
   	char title_q_166[100];

   	for (Int_t chn=0; chn<12; chn++) 
	{
      	sprintf(name_q_166, "hist_q_166%d", chn);
      	sprintf(title_q_166, "WD166 - Q of chn%d", chn);
      	hist_q_166[chn] = new TH1F(name_q_166,title_q_166, 100, 0.0, 25);
      	hist_q_166[chn]->GetXaxis()->SetTitle("Q [a.u.]");
      	hist_q_166[chn]->GetYaxis()->SetTitle("Entries");
   	}

   	//------------------------------
   	// HISTOGRAMS CHARGHE BAR WD165
   	//------------------------------
   	TH1F *hist_q_bar_166[16];
   	char name_q_bar_166[20];
   	char title_q_bar_166[100];

   	for (Int_t b=0; b<6; b++) 
	{
		if (b<3)
		{
			int x_bar = b+8;
		    sprintf(name_q_bar_166, "hist_q_bar_166%d", x_bar);
      		sprintf(title_q_bar_166, "Bar%d", x_bar);
		}
		else
		{
			int y_bar = b+25;
			sprintf(name_q_bar_166, "hist_q_bar_166%d", y_bar);
      		sprintf(title_q_bar_166, "Bar%d", y_bar);
		}
      	hist_q_bar_166[b] = new TH1F(name_q_bar_166,title_q_bar_166, 60, 0, 20);
      	hist_q_bar_166[b]->GetXaxis()->SetTitle("Q [a.u.]");
      	hist_q_bar_166[b]->GetYaxis()->SetTitle("Entries");
   	}

   	//------------------------------
   	// HISTOGRAMS DELTA TIME WD166
   	//------------------------------
   	TH1F *hist_delta_time_166[16];
   	char name_delta_time_166[20];
   	char title_delta_time_166[100];

   	for (Int_t b=0; b<6; b++) 
	{
      	//sprintf(name_delta_time_166,"h_delta_time_166%d", b);
      	//sprintf(title_delta_time_166,"WD166 - #DeltaT WD166 of bar%d", b);
		if (b<3)
		{
			int x_bar = b+3;
		    sprintf(name_delta_time_166, "h_delta_time_166%d", b);
      		sprintf(title_delta_time_166, "Bar%d", b);
		}
		else
		{
			int y_bar = b+23;
			sprintf(name_delta_time_166, "h_delta_time_166%d", b);
      		sprintf(title_delta_time_166, "Bar%d", b);
		}
     	hist_delta_time_166[b] = new TH1F(name_delta_time_166,title_delta_time_166, 20, -10.0, 10.0); // [ns]
      	hist_delta_time_166[b]->GetXaxis()->SetTitle("#DeltaT [ns]");
      	hist_delta_time_166[b]->GetYaxis()->SetTitle("Entries");
   	}

	//***********************************************************************************************************************

   // INITIALIZE ARRAY
   Int_t Bar_TOF_X[20];
   Int_t Bar_TOF_Y[20];
   Int_t Bar_TOF[40];
   Double_t f_CFD = 0.3;         // FRACTION FOR COMPUTE THE TIME
   //Double_t Time_Bar_TOF_X[20];
   //Double_t Time_Bar_TOF_Y[20];
   Double_t mean_time_166[8];


   // LOOP ON ENTRIES
   for (Long64_t jentry=0; jentry<nentries; jentry++)
   {
    	Long64_t ientry = LoadTree(jentry);
      	if (ientry < 0) break;
      	nb = fChain->GetEntry(jentry);   nbytes += nb;
      
      	// INIT VALUES
      	memset(Bar_TOF_X,      0, 20*sizeof(Int_t));
      	memset(Bar_TOF_Y,      0, 20*sizeof(Int_t));
      	memset(Bar_TOF,        0, 40*sizeof(Int_t));
		//memset(Time_Bar_TOF_X, 0, 20*sizeof(Int_t)); // O sizeof(Double_t)?
		//memset(Time_Bar_TOF_Y, 0, 20*sizeof(Int_t)); // O sizeof(Double_t)?

		Double_t Mean_Time_Bar_9_X = 0;
   		Double_t Mean_Time_Bar_9_Y = 0;
      
     	Int_t status = 1; 

		//**************************************
      	// WAVEDREAM 166 - X VIEW (BAR 8 TO 10); Y VIEW (BAR 28 to 30)
      
      	// LOOP ON CHANNELS OF WAVEDREAM
      	for (Int_t chn=0; chn<12; chn++) 
      	{
       		status = 1;
			// 1° CHECK SIGNAL
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
            	Double_t time_166[16];                             // TIME OF WF OF CHANNEL [a.u.]
            	Double_t delta_time_166;                           // ∆ TIME OF WF OF CHANNEL L AND R [a.u.]
            	Double_t mean_time_166[8];                         // MEAN TIME OF L AND R CHANNELS [a.u.]
				Double_t mean_time;
            	Double_t q = 0.;                                   // CHARGE [a.u.]
            	Double_t q_166[16];                                // CHARGE OF CHANNEL [a.u.]
            	Double_t q_bar_166[8];                             // CHARGE OF CHARGE [a.u.]
			

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

	        	// CHARGHE OF CHANNEL - METODO TRAPEZI: (b+B)*h/2
               	for (Int_t i=150; i<900; i++) 
				{
					q += ((v_base_166 - board166_waveform[chn][i]) + (v_base_166 - board166_waveform[chn][i+1])) * (board166_time[chn][i+1] - board166_time[chn][i])/2 * TMath::Power(10,9);;
				}
               	q_166[chn] = q;
				
				// DATA TO FIT
			
				Int_t n = 5;

               	// TIME OF CHANNEL
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

                     	//time_166[chn] = (board166_time[chn][t] + board166_time[chn][t+1])/2 * TMath::Power(10,9); // SVILUPPARE UN METODO PIU' RAFFINATO
                     	break;
                  	}
               	}

			    if (chn%2!=0)
				{
                	// CHARGE OF BAR
                	q_bar_166[chn/2] = sqrt(q_166[chn] * q_166[chn-1]);
                	// MEAN TIME OF BAR [ns];
                	mean_time_166[chn/2] = (time_166[chn] + time_166[chn-1])/2;
                	// DELTA TIME OF BAR [ns]
                	delta_time_166 = time_166[chn] - time_166[chn-1];
               	}

				if (chn==3) Mean_Time_Bar_9_X = (time_166[3] + time_166[2])/2;
				else if (chn==9) Mean_Time_Bar_9_Y = (time_166[9] + time_166[8])/2;

               	//***********************************************************************************************************
               	// FILL THE HISTOGRAM
               	if (v_ampl_166>0.010 && q_166[chn]>0. && delta_time_166>-10.0 && delta_time_166<10.0) 
				{  	
                  	// Q CHARGE CHANNEL
                  	hist_q_166[chn]->Fill(q_166[chn]);
                  	
					if (chn%2!=0) 
					{
						// Q CHARGE BAR
                     	hist_q_bar_166[chn/2]->Fill(q_bar_166[chn/2]);
                     	// ∆T TIME CHANNELS
                     	hist_delta_time_166[chn/2]->Fill(delta_time_166);
                     	// ∆T VS LOG(Q_L/Q_R)
                     	hist_charge_time_166[chn/2]->Fill(log(q_166[chn]/q_166[chn-1]), time_166[chn] - time_166[chn-1]);
                  	}
               	}
               	//else if (delta_time_166<-5.0 && delta_time_166>5.0) mean_time_X = -1;

				if (Mean_Time_Bar_9_X>0 && Mean_Time_Bar_9_Y>0 && q_bar_166[chn/2]>10) hist_mean_time->Fill(Mean_Time_Bar_9_Y - Mean_Time_Bar_9_X);
               //***********************************************************************************************************

				if (0) 
				{
                	std::cout << "\n**************************"              << std::endl;
                	std::cout << "\nWD 166"                                  << std::endl;
                  	std::cout << "\nEntry                   = " << jentry           << std::endl;
                  	std::cout << "\nchn                     = " << chn              << std::endl;
                  	std::cout << "\nmin_166 [V]             = " << min_166          << std::endl;
                  	std::cout << "\nmax_166 [V]             = " << max_166          << std::endl;
                  	std::cout << "\n------------------------------------------" << std::endl;
                  	std::cout << "\nv_base_166       [V]    = " << v_base_166   << std::endl;
                  	std::cout << "\nsigma_v_base_166 [V]    = " << sigma_v_base_166 << std::endl;
                  	std::cout << "\nv_peak_166       [V]    = " << v_peak_166         << std::endl;
                  	std::cout << "\nt_peak_166       [ns]   = " << t_peak_166         << std::endl;
                  	std::cout << "\nv_ampl_166       [V]    = " << v_ampl_166         << std::endl;
                  	std::cout << "\nv_th_166         [V]    = " << v_th_166           << std::endl;
                  	std::cout << "\ntime_166         [ns]   = " << time_166[chn]      << std::endl;
                  	std::cout << "\nq_166            [a.u.] = " << q_166[chn]         << std::endl;
                  	if (chn%2!=0) 
					{
                     	std::cout << "\n------------------------------------------" << std::endl;
                     	std::cout << "\nq_bar_166                                 = " << q_bar_166[chn/2]      << std::endl;
                     	//std::cout << "\nmean_time_166   = " << mean_time_165[chn/2]  << std::endl;
                     	std::cout << "log(q_166[chn]/q_166[chn-1])                = " << log(q_166[chn] / q_166[chn-1]) << endl;
                     	std::cout << "time_166["<<chn<<"] - time_166["<<chn-1<<"]         = " << delta_time_166     << endl;
                     	//std::cout << "mean_time_X WD165 [ns]                      = " << mean_time_X                    << endl;
                     	std::cout << "\n------------------------------------------" << std::endl;
                  	}
					std::cout << "\nMean_Time_Bar_9_X        [ns] = " << Mean_Time_Bar_9_X << std::endl;
					std::cout << "\nMean_Time_Bar_9_Y        [ns] = " << Mean_Time_Bar_9_Y << std::endl;
               	}
			} // END if (board166_hit[chn] == 1 && board166_hit[chn+1] == 1)
      	} // END CHANNEL LOOP

      	//**************************************
	  	// HITMAP
	  	//**************************************

      	// WD 165 - X VIEW (BAR 0 TO 7)
      
      	for(int b=0; b<8; ++b)
      	{
         	if(board165_hit[2*b] && board165_hit[2*b+1])
    		{
	    		if(b==0)
	    		{
	      			Bar_TOF_X[0] = 1;
	      			Bar_TOF[0]   = 1;
	    		}
            	else if(b==1)
	    		{
	      			Bar_TOF_X[1] = 1;
	      			Bar_TOF[1]   = 1;
	    		}
            	else if(b==2)
	    		{
	      			Bar_TOF_X[2] = 1;
	      			Bar_TOF[2]   = 1;
	    		}
            	else if(b==3)
	    		{
	      			Bar_TOF_X[3] = 1;
	      			Bar_TOF[3]   = 1;
	    		}
            	else if(b==4)
	    		{
	      			Bar_TOF_X[4] = 1;
	      			Bar_TOF[4]   = 1;
	    		}
            	else if(b==5)
	    		{
	      			Bar_TOF_X[5] = 1;
	      			Bar_TOF[5]   = 1;
	    		}
            	else if(b==6)
	    		{
	      			Bar_TOF_X[6] = 1;
	      			Bar_TOF[6]   = 1;
	    		}
            	else if(b==7)
	    		{
	      			Bar_TOF_X[7] = 1;
	      			Bar_TOF[7]   = 1;
	    		}
         	}
      	}
      
      	//**************************************
      	// WAVEDREAM 166 - X VIEW (BAR 8 TO 10); Y VIEW (BAR 28 to 30)
      
      	// LOOP ON CHANNELS OF WAVEDREAM
      	for (Int_t ch=0; ch<12; ch++) 
      	{
       		status = 1;
			// 1° CHECK SIGNAL
        	// FIND MIN AND MAX OF WAVEFORM
        	Double_t min_166 = board166_waveform[ch][100]; // V AMPLITUDE MIN OF CHANNEL [V]
        	Double_t max_166 = board166_waveform[ch][100]; // V AMPLITUDE MAX OF CHANNEL [V]
            
        	for (Int_t t=100; t<900; t++) 
        	{ // LOOP ON SAMPLES OF WAVEFORM
           		if (board166_waveform[ch][t] < min_166) min_166 = board166_waveform[ch][t];
           		if (board166_waveform[ch][t] > max_166) max_166 = board166_waveform[ch][t];
        	}
            
			if (max_166 - min_166 > 0.95) status = 0;  // NOISE
            
        	if (max_166 - min_166 < 0.010) status = 0; // REMEMBER ZEROSUPPRESSION
               
        	if (status==0) board166_hit[ch] = 0;
      
    	} // CHANNEL LOOP
   
      	for(int b=0; b<6; ++b)
      	{
        	if(board166_hit[2*b] && board166_hit[2*b+1])
         	{
	   			if(b==0)
	   			{
	     			Bar_TOF_X[8]  = 1;
	     			Bar_TOF[8]    = 1;
					//Time_Bar_TOF_X[8] = mean_time_166[0];
	   			}
	   			else if(b==1)
	   			{
	     			Bar_TOF_X[9]  = 1;
	     			Bar_TOF[9]    = 1;
					//Time_Bar_TOF_X[9] = mean_time_166[1];
	   			} 
	   			else if(b==2)
	   			{
	     			Bar_TOF_X[10] = 1;
	     			Bar_TOF[10]   = 1;
					//Time_Bar_TOF_X[10] = mean_time_166[2];
	   			}	   
	   			if(b==3)
	   			{
	     			Bar_TOF_Y[8]  = 1;
	     			Bar_TOF[28]   = 1;
					//Time_Bar_TOF_Y[8] = mean_time_166[3];
	   			}
	   			else if(b==4)
	   			{
	     			Bar_TOF_Y[9]  = 1;
	     			Bar_TOF[29]  = 1;
					//Time_Bar_TOF_Y[9] = mean_time_166[4];
	   			}
	   			else if(b==5)
	   			{
	     			Bar_TOF_Y[10] = 1;
	     			Bar_TOF[30]   = 1;
					//Time_Bar_TOF_Y[10] = mean_time_166[5];
	   			}
         	}
      	}
   
      	//**************************************
      	// WD 170 - X VIEW (BAR 11 TO 18)

      	for(int b=0; b<8; ++b)
      	{
        	if(board170_hit[2*b] && board170_hit[2*b+1])
         	{
	   			if(b==0)
	   			{
	     			Bar_TOF_X[11] = 1;
	     			Bar_TOF[11]   = 1;
	   			}
	   			else if(b==1)
	   			{
	     			Bar_TOF_X[12] = 1;
	     			Bar_TOF[12]   = 1;
	   			}
	   			else if(b==2)
	   			{
	     			Bar_TOF_X[13] = 1;
	     			Bar_TOF[13]   = 1;
	   			}
	   			else if(b==3)
	   			{
	     			Bar_TOF_X[14] = 1;
	     			Bar_TOF[14]   = 1;
	   			}
	   			else if(b==4)
	   			{
	     			Bar_TOF_X[15] = 1;
	     			Bar_TOF[15]   = 1;
	   			}
	   			else if(b==5)
	   			{
	     			Bar_TOF_X[16] = 1;
	     			Bar_TOF[16]   = 1;
	   			}
	   			else if(b==6)
	   			{
	     			Bar_TOF_X[17] = 1;
	     			Bar_TOF[17]   = 1;
	   			}
	   			else if(b==7)
	   			{
	     			Bar_TOF_X[18] = 1;
	     			Bar_TOF[18]   = 1;
	   			}
	 		}
    	}
   
      	//**************************************
      	// WD 167 - X VIEW (BAR 19); Y VIEW (BAR 20 TO 26)

      	for(int b=0; b<8; ++b)
      	{
        	if(board167_hit[2*b] && board167_hit[2*b+1])
         	{
	   			if(b==0)
	   			{
	     			Bar_TOF_X[19] = 1;
	     			Bar_TOF[19]   = 1;
	   			}
	   			else if(b==1)
	   			{
	     			Bar_TOF_Y[0] = 1;
	     			Bar_TOF[20]  = 1;
	   			}
	   			else if(b==2)
	   			{
	     			Bar_TOF_Y[1] = 1;
	     			Bar_TOF[21]  = 1;
	   			}
	   			else if(b==3)
	   			{
	     			Bar_TOF_Y[2] = 1;
	     			Bar_TOF[22]  = 1;
	   			}
	   			else if(b==4)
	   			{
	     			Bar_TOF_Y[3] = 1;
	     			Bar_TOF[23]  = 1;
	   			}
	   			else if(b==5)
	   			{
	     			Bar_TOF_Y[4] = 1;
	     			Bar_TOF[24]  = 1;
	   			}
	   			else if(b==6)
	   			{
	     			Bar_TOF_Y[5] = 1;
	     			Bar_TOF[25]  = 1;
	   			}
	  	 		else if(b==7)
	   			{
	     			Bar_TOF_Y[6] = 1;
	     			Bar_TOF[26]  = 1;
	   			}
	 		}
      	}

      	//**************************************
      	// WD 168 - Y VIEW (BAR 27 AND 31 TO 37)

      	for(int b=0; b<8; ++b)
      	{
        	if(board168_hit[2*b] && board168_hit[2*b+1])
         	{
	   			if(b==0)
	   			{
	     			Bar_TOF_Y[7]  = 1;
	     			Bar_TOF[27]   = 1;
	   			}
	   			else if(b==1)
	  			{
	     			Bar_TOF_Y[11] = 1;
	     			Bar_TOF[31]   = 1;
	   			}
	   			else if(b==2)
	   			{
	     			Bar_TOF_Y[12] = 1;
	     			Bar_TOF[32]   = 1;
	   			}
	   			else if(b==3)
	   			{
	     			Bar_TOF_Y[13] = 1;
	     			Bar_TOF[33]   = 1;
	   			}
	   			else if(b==4)
	   			{
	     			Bar_TOF_Y[14] = 1;
	     			Bar_TOF[34]   = 1;
	   			}
	   			else if(b==5)
	   			{
	     			Bar_TOF_Y[15] = 1;
	     			Bar_TOF[35]   = 1;
	   			}
	   			else if(b==6)
	   			{
	     			Bar_TOF_Y[16] = 1;
	     			Bar_TOF[36]   = 1;
	   			}
	   			else if(b==7)
	   			{
	     			Bar_TOF_Y[17] = 1;
	     			Bar_TOF[37]   = 1;
	   			}
         	}
      	}

      	//**************************************
      	// WD 158 - Y VIEW (BAR 38 TO 39)

      	for(int b=0; b<2; ++b)
      	{
        	if(board158_hit[2*b] && board158_hit[2*b+1])
         	{
	   			if(b==0)
	   			{
	     			Bar_TOF_Y[18] = 1;
	     			Bar_TOF[38]   = 1;
	   			}
	   			else if(b==1)
	   			{
	     			Bar_TOF_Y[19] = 1;
	     			Bar_TOF[39]   = 1;
	   			}
        	}
    	}

      	// FILL THE HISTOGRAM 2D
      	for (int i=0; i<20; ++i)
      	{
			if (Bar_TOF_X[i] == 0) continue;
            for (int j=0; j<20; j++)
	    	{
	      		if (Bar_TOF_Y[j] == 0) continue;
	      			hist->Fill(i, 20+j);
	    	}
       	}

      	// FILL THE HISTOGRAM 1D
      	for (int b=0; b<40; b++)
      	{
			if (Bar_TOF[b] == 0) continue;
			hist_project->Fill(b);
      	}

      	// FILL THE HISTOGRAM DELTA TIME
      	//for (int i=0; i<20; ++i)
      	//{
		//	if (Time_Bar_TOF_X[i] == 0) continue;
        //    for (int j=0; j<20; j++)
	    //	{
	    //  		if (Time_Bar_TOF_Y[j] == 0) continue;
		//		hist_mean_time->Fill(Time_Bar_TOF_X[i] - Time_Bar_TOF_Y[20+j]);
	    //	}
       	//}


      
   	} // END LOOP ON ENTRIES

   // CANVAS
   TCanvas *c1 = new TCanvas("c1", "hitmap",600,600);
   c1->SetTickx();
   c1->SetTicky();
   c1->SetLeftMargin(0.15);
   hist->GetXaxis()->SetTitle("FronBar, LayerX");
   hist->SetNdivisions(20,"X");
   hist->GetYaxis()->SetTitle("RearBar, LayerY");
   hist->SetNdivisions(20,"Y");
   hist->GetZaxis()->SetTitle("# hits");
   c1->SetLogz();
   hist->Draw("colz TEXT0 SAME");
   c1->SaveAs("figures/hitmap_hist2d.pdf");
   

   TCanvas *c2 = new TCanvas("c2", "hitmap_projection",600,600);
   c2->SetTickx();
   c2->SetTicky();
   c2->SetLeftMargin(0.15);
   hist_project->GetXaxis()->SetTitle("Bar");
   hist_project->GetYaxis()->SetTitle("Entries");
   c2->SetLogy();
   hist_project->SetNdivisions(20,"X");
   hist_project->Draw("TEXT0 hist");
   c2->SaveAs("figures/hitmap_hist_XY_proj.pdf");


   	//------------------------------
   	// HIST DELTA MEAN TIME
   	//------------------------------
	
   	TCanvas *c_delta_mean_time = new TCanvas("c_delta_mean_time", "c_delta_mean_time", 1200, 1200);
   	c_delta_mean_time->cd(1);
   	c_delta_mean_time->SetTickx();
   	c_delta_mean_time->SetTicky();
   	c_delta_mean_time->SetLeftMargin(0.15);
   	hist_mean_time->GetXaxis()->SetTitle("#DeltaT_{TOFwall} bar9 Y/X[ns]");
   	hist_mean_time->SetFillColor(38);
   	hist_mean_time->GetYaxis()->SetTitle("Counts");
   	hist_mean_time->Fit("gaus", "Q");
   	hist_mean_time->Draw();
	c_delta_mean_time->SaveAs("figures/delta_mean_time.pdf");
	

	//==================================================================================
   	// WAVEDREAM 166 - X VIEW (BAR 0 TO 7)

   	//------------------------------
   	// CHARGHE BAR WD166
   	//------------------------------
   	for (int b=0; b<6; b++) 
	{  
    	TString canvas_title_q_bar_166 = Form("c_q_bar_166%d", b); 
      	TCanvas *c_q_bar_166 = new TCanvas(canvas_title_q_bar_166, canvas_title_q_bar_166, 600, 600);
		/*
		if (b<3)
		{
			int x_bar = b+3;
			TString c_q_bar_166 = Form("c_q_bar_166%d", x_bar); 	
			TCanvas *c_q_bar_166 = new TCanvas(canvas_title_q_bar_166, canvas_title_q_bar_166, 600, 600);
		}
		else
		{
			int y_bar = b+23;
			TString canvas_title_q_vs_t_166 = Form("c_q_bar_166%d", y_bar); 	
			TCanvas *c_q_bar_166 = new TCanvas(canvas_title_q_bar_166, canvas_title_q_bar_166, 600, 600);
		}
		*/
		c_q_bar_166->SetTickx();
		c_q_bar_166->SetTicky();
		c_q_bar_166->SetLeftMargin(0.15);
		c_q_bar_166->SetLogy();
		//hist_q_bar_166[b]->SetMarkerStyle(20);
		hist_q_bar_166[b]->SetMarkerStyle(kFullCircle);
		hist_q_bar_166[b]->SetMarkerColor(kBlack);
		hist_q_bar_166[b]->SetNdivisions(20,"X");
		hist_q_bar_166[b]->Draw();
		c_q_bar_166->SaveAs("figures/"+canvas_title_q_bar_166+".pdf");
	}

   	//------------------------------
   	// LOG(Q_L/Q_R) VS (T_L - T_R)
   	//------------------------------
   	for (int b=0; b<6; b++) 
	{	
		/*
		if (b<3)
		{
			int x_bar = b+3;
			TString canvas_title_q_vs_t_166 = Form("c_q_vs_t_166%d", x_bar); 	
		}
		else
		{
			int y_bar = b+23;
			TString canvas_title_q_vs_t_166 = Form("c_q_vs_t_166%d", y_bar);
		}
		*/
		TString canvas_title_q_vs_t_166 = Form("c_q_vs_t_166%d", b); 
      	TCanvas *c_q_vs_t_166 = new TCanvas(canvas_title_q_vs_t_166, canvas_title_q_vs_t_166, 600, 600);
      	c_q_vs_t_166->SetTickx();
      	c_q_vs_t_166->SetTicky();
		c_q_vs_t_166->SetLogz();
		c_q_vs_t_166->SetGrid();
      	hist_charge_time_166[b]->GetXaxis()->SetTitle("log(Q_L/Q_R)");
      	hist_charge_time_166[b]->GetYaxis()->SetTitle("(t_L - t_R) [ns]");
		hist_charge_time_166[b]->Draw();
		hist_charge_time_166[b]->Draw("colz");
		c_q_vs_t_166->SaveAs("figures/"+canvas_title_q_vs_t_166+".pdf");
   	}
}
