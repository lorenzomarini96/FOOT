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
   
  	//gStyle->SetOptStat(0);

   	// HISTOGRAM
   	TH2D *hist = new TH2D("hist", "Hit-map", 20, -0.5, 19.5, 20, 19.5, 39.5);
   	TH1D *hist_project = new TH1D("hist_project", "Hit-map XY projections", 40, -0.5, 39.5);
	
	//------------------------------
   	// HIST FOR TIME RESOLUTION
   	//------------------------------
   	TH1D *hist_mean_time = new TH1D("hist_mean_time", "#DeltaT_{TW} = #bar{T}_{front} - #bar{T}_{rear}", 50, -15.0, 15.0);
   

   	//============================================================================================================================
   	// WAVEDREAM 166 - X VIEW (BAR 8 TO 10); Y VIEW (BAR 28 to 30)

   	//------------------------------
   	// HISTOGRAMS Q VS TIME WD166
   	//------------------------------

   	TH2D *hist_charge_time_166[8];
   	char name_charge_time_166[20];
   	char title_charge_time_166[100];

   	for (Int_t b=0; b<8; b++) 
	{
      	sprintf(name_charge_time_166, "hist_charge_time_166_bar%d", b);
     	sprintf(title_charge_time_166, "WD166 - Log(Q_L/Q_R) vs (t_L - t_R) b%d", b);
      	hist_charge_time_166[b] = new TH2D(name_charge_time_166,title_charge_time_166, 20, -1, 1, 20, -10.0, 10.0);
      	hist_charge_time_166[b]->GetXaxis()->SetTitle("Log(Q_L/Q_R)");
      	hist_charge_time_166[b]->GetYaxis()->SetTitle("(t_L - t_R) [ns]");
   	}
   
   	//------------------------------
   	// HISTOGRAMS V_AMPLITUDE WD166
   	//------------------------------
   	TH1F *hist_v_ampl_166[16];
   	char name_v_ampl_166[20];
   	char title_v_ampl_166[100];

   	for (Int_t chn=0; chn<12; chn++) 
	{
      	sprintf(name_v_ampl_166, "hist_v_ampl_166%d", chn);
      	sprintf(title_v_ampl_166, "WD166 - V_{ampl} of chn%d", chn);
      	hist_v_ampl_166[chn] = new TH1F(name_v_ampl_166,title_v_ampl_166, 40, 0, 1.0);
      	hist_v_ampl_166[chn]->GetXaxis()->SetTitle("V_ampl [mV]");
      	hist_v_ampl_166[chn]->GetYaxis()->SetTitle("Entries");
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
      	hist_q_166[chn] = new TH1F(name_q_166,title_q_166, 50, 0.0, 25);
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
		if (b<6)
		{
			sprintf(name_q_bar_166, "hist_q_bar_166%d", b+6);
			sprintf(title_q_bar_166, "WD166 - Q of bar%d", b+6);
		}
      	else 
		{
			sprintf(name_q_bar_166, "hist_q_bar_166%d", b+22);
			sprintf(title_q_bar_166, "WD166 - Q of bar%d", b+22);
		}
      	hist_q_bar_166[b] = new TH1F(name_q_bar_166,title_q_bar_166, 50, 0.0, 25);
      	hist_q_bar_166[b]->GetXaxis()->SetTitle("Q [a.u.]");
      	hist_q_bar_166[b]->GetYaxis()->SetTitle("Entries");
   	}

   	//------------------------------
   	// HISTOGRAMS DELTA TIME WD166
   	//------------------------------
   	TH1F *hist_delta_time_166[16];
   	char name_delta_time_166[20];
   	char title_delta_time_166[100];

   	for (Int_t b=0; b<8; b++) 
	{
      	sprintf(name_delta_time_166,"h_delta_time_166%d", b);
      	sprintf(title_delta_time_166,"WD166 - #DeltaT WD166 of bar%d", b);
     	hist_delta_time_166[b] = new TH1F(name_delta_time_166,title_delta_time_166, 20, -10.0, 10.0); // [ns]
      	hist_delta_time_166[b]->GetXaxis()->SetTitle("#DeltaT [ns]");
    	hist_delta_time_166[b]->GetYaxis()->SetTitle("Entries");
   	}

   	//==================================================================================
   	// WAVEDREAM 165 - X VIEW (BAR 0 TO 7)   
   	//==================================================================================
   
   	//------------------------------
   	// HISTOGRAMS Q VS TIME WD165
   	//------------------------------

   	TH2D *hist_charge_time_165[8];
   	char name_charge_time_165[20];
   	char title_charge_time_165[100];

   	for (Int_t b=0; b<8; b++) 
	{
      	sprintf(name_charge_time_165, "hist_charge_time_165_bar%d", b);
      	sprintf(title_charge_time_165, "WD165 - Log(Q_L/Q_R) vs (t_L - t_R) b%d", b);
    	hist_charge_time_165[b] = new TH2D(name_charge_time_165,title_charge_time_165, 30, -2.5, 2.5, 30, -10.0, 10.0);
      	hist_charge_time_165[b]->GetXaxis()->SetTitle("Log(Q_L/Q_R)");
      	hist_charge_time_165[b]->GetYaxis()->SetTitle("(t_L - t_R) [ns]");
      	//hist_charge_time_165[b]->GetXaxis()->SetTitleSize(0.05);
      	//hist_charge_time_165[b]->GetYaxis()->SetTitleSize(0.05);
   	}

   	//------------------------------
   	// HISTOGRAMS CHARGHE WD165
   	//------------------------------
   	TH1F *hist_q_165[16];
   	char name_q_165[20];
   	char title_q_165[100];

   	for (Int_t chn=0; chn<16; chn++) 
	{
      	sprintf(name_q_165, "hist_q_165_ch%d", chn);
      	sprintf(title_q_165, "WD165 - Q of chn%d", chn);
      	hist_q_165[chn] = new TH1F(name_q_165,title_q_165, 100, 0.0, 30);
      	hist_q_165[chn]->GetXaxis()->SetTitle("Q [a.u.]");
      	hist_q_165[chn]->GetYaxis()->SetTitle("Entries");
      	//hist_q_165[chn]->GetXaxis()->SetTitleSize(0.05);
      	//hist_q_165[chn]->GetYaxis()->SetTitleSize(0.05);
   	}

   	//------------------------------
   	// HISTOGRAMS CHARGHE BAR WD165
   	//------------------------------
   	TH1F *hist_q_bar_165[16];
   	char name_q_bar_165[20];
   	char title_q_bar_165[100];

   	for (Int_t b=0; b<16; b++) {
      	sprintf(name_q_bar_165, "hist_q_bar_165%d", b);
     	sprintf(title_q_bar_165, "WD165 - Q of bar%d", b);
      	hist_q_bar_165[b] = new TH1F(name_q_bar_165,title_q_bar_165, 100, 0.0, 30);
      	hist_q_bar_165[b]->GetXaxis()->SetTitle("Q [a.u.]");
      	hist_q_bar_165[b]->GetYaxis()->SetTitle("Entries");
      	//hist_q_bar_165[b]->GetXaxis()->SetTitleSize(0.05);
      	//hist_q_bar_165[b]->GetYaxis()->SetTitleSize(0.05);
   	}

   	//------------------------------
   	// HISTOGRAMS DELTA TIME WD165
   	//------------------------------
   	TH1F *hist_delta_time_165[16];
   	char name_delta_time_165[20];
   	char title_delta_time_165[100];

   	for (Int_t b=0; b<8; b++) {
      	sprintf(name_delta_time_165,"h_delta_time_165%d", b);
      	sprintf(title_delta_time_165,"WD165 - #DeltaT WD165 of bar%d", b);
      	hist_delta_time_165[b] = new TH1F(name_delta_time_165,title_delta_time_165, 10, -5.0, 5.0); // [ns]
      	hist_delta_time_165[b]->GetXaxis()->SetTitle("#DeltaT [ns]");
      	hist_delta_time_165[b]->GetYaxis()->SetTitle("Entries");
   	}

   	// INITIALIZE ARRAY
   	Int_t Bar_TOF_X[20];
   	Int_t Bar_TOF_Y[20];
   	Int_t Bar_TOF[40];

	Int_t Time_Bar_TOF_X[20];
   	Int_t Time_Bar_TOF_Y[20];
   
   	Double_t f_CFD = 0.3;         // FRACTION FOR COMPUTE THE TIME

	
   	// LOOP ON ENTRIES
   	for (Long64_t jentry=0; jentry<nentries/2; jentry++) 
   	{
		Long64_t ientry = LoadTree(jentry);
      	if (ientry < 0) break;
      	nb = fChain->GetEntry(jentry);   nbytes += nb;
      
      	// INIT VALUES
      	memset(Bar_TOF_X, 0, 20*sizeof(Int_t));
      	memset(Bar_TOF_Y, 0, 20*sizeof(Int_t));
      	memset(Bar_TOF,   0, 40*sizeof(Int_t));

		memset(Time_Bar_TOF_X, 0, 20*sizeof(Int_t));
      	memset(Time_Bar_TOF_Y, 0, 20*sizeof(Int_t));

		Double_t mean_time_X = -1.;   // MEAN TIME (LEFT AND RIGHT) ON THE BAR (X-VIEW)

      	Int_t status = 1;

      	//**************************************
      	// WD 165 - X VIEW (BAR 0 TO 7)
      	
		// LOOP ON CHANNELS OF WAVEDREAM
      	for (Int_t chn=0; chn<16; chn++) 
      	{
       		status = 1;
			
			//****************************************************************************************
			// 1° CHECK SIGNAL
        	// FIND MIN AND MAX OF WAVEFORM
        	Double_t min_165 = board165_waveform[chn][100]; // V AMPLITUDE MIN OF CHANNEL [V]
        	Double_t max_165 = board165_waveform[chn][100]; // V AMPLITUDE MAX OF CHANNEL [V]
            
        	for (Int_t i=100; i<900; i++)
        	{ // LOOP ON SAMPLES OF WAVEFORM
           		if (board165_waveform[chn][i] < min_165) min_165 = board165_waveform[chn][i];
           		if (board165_waveform[chn][i] > max_165) max_165 = board165_waveform[chn][i];
        	}
            
			if (max_165 - min_165 > 0.95) status = 0;  // NOISE
            
        	if (max_165 - min_165 < 0.010) status = 0; // REMEMBER ZEROSUPPRESSION
               
        	if (status==0) board165_hit[chn] = 0;
			//***************************************************************************************

	        if (board165_hit[chn] == 1 && board165_hit[chn+1] == 1)
			//if (board165_hit[chn] == 1)
			{
            	// INITIALIZE VALUES
            	Double_t voltage_165;                         
            	Double_t v_base_165;                               // V BASELINE (PEDESTAL) [V]
            	Double_t sigma_v_base_165;                         // V BASELINE (PEDESTAL) [V]
            	Double_t v_ampl_165;                               // V AMPLITUDE OF CHANNEL [V]
            	Double_t v_peak_165 = board165_waveform[chn][300]; // V MINIMUM (PEAK) [V]
            	Double_t t_peak_165;                               // TIME CORRESPONDING TO PEAK
            	Double_t v_th_165;                                 // V THRESHOLD [V]
            	//Double_t t_start_165;                              // TIME START OF SIGNAL [ns]
            	Double_t t_stop_165;                               // TIME STOP OF SIGNAL  [ns]
            	Double_t i_start_165;                              // V START OF SIGNAL [V]
            	Double_t i_stop_165;                               // V STOP OF SIGNAL  [V]
            	Double_t i_min_position_165;                       // POSITION OF V PEAK
            	Double_t time_165[16];                             // TIME OF WF OF CHANNEL [a.u.]
            	Double_t delta_time_165;                           // ∆ TIME OF WF OF CHANNEL L AND R [a.u.]
            	Double_t mean_time_165[8];                         // MEAN TIME OF L AND R CHANNELS [a.u.]
            	Double_t q = 0.;                                   // CHARGE [a.u.]
            	Double_t q_165[16];                                // CHARGE OF CHANNEL [a.u.]
            	Double_t q_bar_165[8];                             // CHARGE OF CHARGE [a.u.]

				// V BASELINE   
               	TH1F *wf_165 = new TH1F("wf_165","wf_165", 30, 0.30, 0.60);
               	for (Int_t t=10; t<250; t++) wf_165->Fill(board165_waveform[chn][t]);
               	v_base_165 = wf_165->GetMean();
               	sigma_v_base_165 = wf_165->GetRMS(); // TO EVALUATE THE NOISE (FOR FUTURES...)

               	wf_165->SetDirectory(0); // remove an histogram from the gDirectory list
            	
				// V PEAK (MINIMUM)
               	for (Int_t i=50; i<800; i++)
				{ // LOOP ON SAMPLES OF WAVEFORM
                  	if (board165_waveform[chn][i] < v_peak_165) 
					{
						i_min_position_165 = i;
                    	v_peak_165 = board165_waveform[chn][i];
                    	t_peak_165 = board165_time[chn][i] * TMath::Power(10,9); // Convert time from [s] in [ns]
                	}
               	}

	            // V AMPLITUDE
               	v_ampl_165 = v_base_165 - v_peak_165;                     
            
               	// V THRESHOLD                                    
               	v_th_165 = v_base_165 - f_CFD * (v_base_165 - v_peak_165);

	        	// CHARGHE OF CHANNEL - METODO TRAPEZI: (b+B)*h/2
               	for (Int_t i=150; i<900; i++) q += ((v_base_165 - board165_waveform[chn][i]) + (v_base_165 - board165_waveform[chn][i+1])) * (board165_time[chn][i+1] - board165_time[chn][i])/2 * TMath::Power(10,9);;
               	q_165[chn] = q;
				

               	// TIME OF CHANNEL
               	for (Int_t t=i_min_position_165; ; t--) { // LOOP ON SAMPLES OF WAVEFORM
                  	if (board165_waveform[chn][t] > v_th_165) 
					{
                     	time_165[chn] = (board165_time[chn][t] + board165_time[chn][t+1])/2 * TMath::Power(10,9); // SVILUPPARE UN METODO PIU' RAFFINATO
                     	break;
                  	}
               	}

			    if (chn%2!=0) 
				{
                	// CHARGE OF BAR
                	q_bar_165[chn/2] = sqrt(q_165[chn] * q_165[chn-1]);
                	// MEAN TIME OF BAR [ns];
                	mean_time_X = (time_165[chn] + time_165[chn-1])/2;               
                	// DELTA TIME OF BAR [ns]
                	delta_time_165 = (time_165[chn] - time_165[chn-1]);
               	}

               	//***********************************************************************************************************
               	// FILL THE HISTOGRAM
               	if (v_ampl_165>0.010 && q_165[chn]>0. && delta_time_165>-10.0 && delta_time_165<10.0) 
				{  
                	// V AMPLITUDE CHANNEL
                  	//hist_v_ampl_165[chn]->Fill(v_ampl_165);
                  	// Q CHARGE CHANNEL
                  	hist_q_165[chn]->Fill(q_165[chn]);
                  	
					if (chn%2!=0) 
					{
						// Q CHARGE BAR
                     	hist_q_bar_165[chn/2]->Fill(q_bar_165[chn/2]);
                     	// ∆T TIME CHANNELS
                     	hist_delta_time_165[chn/2]->Fill(delta_time_165);
                     	// ∆T VS LOG(Q_L/Q_R)
                     	hist_charge_time_165[chn/2]->Fill(log(q_165[chn]/q_165[chn-1]), delta_time_165);
                  	}
               	}
               	//else if (delta_time_165<-5.0 && delta_time_165>5.0) mean_time_X = -1;
               //***********************************************************************************************************

				if (1) 
				{
                	std::cout << "\n**************************"              << std::endl;
                	std::cout << "\nWD 165"                                  << std::endl;
                  	std::cout << "\nEntry                   = " << jentry           << std::endl;
                  	std::cout << "\nchn                     = " << chn              << std::endl;
                  	std::cout << "\nmin_165 [V]             = " << min_165          << std::endl;
                  	std::cout << "\nmax_165 [V]             = " << max_165          << std::endl;
                  	std::cout << "\n------------------------------------------" << std::endl;
                  	std::cout << "\nv_base_165       [V]    = " << v_base_165   << std::endl;
                  	std::cout << "\nsigma_v_base_165 [V]    = " << sigma_v_base_165 << std::endl;
                  	std::cout << "\nv_peak_165       [V]    = " << v_peak_165         << std::endl;
                  	std::cout << "\nt_peak_165       [ns]   = " << t_peak_165         << std::endl;
                  	std::cout << "\nv_ampl_165       [V]    = " << v_ampl_165         << std::endl;
                  	std::cout << "\nv_th_165         [V]    = " << v_th_165           << std::endl;
                  	std::cout << "\ntime_165         [ns]   = " << time_165[chn]      << std::endl;
                  	std::cout << "\nq_165            [a.u.] = " << q_165[chn]         << std::endl;
                  	if (chn%2!=0) 
					{
                     	std::cout << "\n------------------------------------------" << std::endl;
                     	std::cout << "\nq_bar_165                                 = " << q_bar_165[chn/2]      << std::endl;
                     	//std::cout << "\nmean_time_165   = " << mean_time_165[chn/2]  << std::endl;
                     	std::cout << "log(q_165[chn]/q_165[chn-1])                = " << log(q_165[chn] / q_165[chn-1]) << endl;
                     	std::cout << "time_165["<<chn<<"] - time_165["<<chn-1<<"]         = " << delta_time_165     << endl;
                     	//std::cout << "mean_time_X WD165 [ns]                      = " << mean_time_X                    << endl;
                     	std::cout << "\n------------------------------------------" << std::endl;
                  	}
               	}

			} // END if (board165_hit[chn] == 1 && board165_hit[chn+1] == 1)
      	} // END CHANNEL LOOP

	  	// HITMAP
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


	        if (board166_hit[chn] == 1 && board166_hit[chn+1] == 1)
			//if (board166_hit[chn] == 1)
			{
            	// INITIALIZE VALUES
            	Double_t voltage_166;                         
            	Double_t v_base_166;                               // V BASELINE (PEDESTAL) [V]
            	Double_t sigma_v_base_166;                         // V BASELINE (PEDESTAL) [V]
            	Double_t v_ampl_166;                               // V AMPLITUDE OF CHANNEL [V]
            	Double_t v_peak_166 = board166_waveform[chn][300]; // V MINIMUM (PEAK) [V]
            	Double_t t_peak_166;                               // TIME CORRESPONDING TO PEAK
            	Double_t v_th_166;                                 // V THRESHOLD [V]
            	//Double_t t_start_166;                              // TIME START OF SIGNAL [ns]
            	Double_t t_stop_166;                               // TIME STOP OF SIGNAL  [ns]
            	Double_t i_start_166;                              // V START OF SIGNAL [V]
            	Double_t i_stop_166;                               // V STOP OF SIGNAL  [V]
            	Double_t i_min_position_166;                       // POSITION OF V PEAK
            	Double_t time_166[16];                             // TIME OF WF OF CHANNEL [a.u.]
            	Double_t delta_time_166;                           // ∆ TIME OF WF OF CHANNEL L AND R [a.u.]
            	Double_t mean_time_166[8];                         // MEAN TIME OF L AND R CHANNELS [a.u.]
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
               	for (Int_t i=50; i<800; i++)
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
               	for (Int_t i=150; i<900; i++) q += ((v_base_166 - board166_waveform[chn][i]) + (v_base_166 - board166_waveform[chn][i+1])) * (board166_time[chn][i+1] - board166_time[chn][i])/2 * TMath::Power(10,9);;
               	q_166[chn] = q;
				

               	// TIME OF CHANNEL
               	for (Int_t t=i_min_position_166; ; t--) { // LOOP ON SAMPLES OF WAVEFORM
                  	if (board166_waveform[chn][t] > v_th_166) 
					{
                     	time_166[chn] = (board166_time[chn][t] + board166_time[chn][t+1])/2 * TMath::Power(10,9); // SVILUPPARE UN METODO PIU' RAFFINATO
                     	break;
                  	}
               	}

			    if (chn%2!=0)
				{
                	// CHARGE OF BAR
                	q_bar_166[chn/2] = sqrt(q_166[chn] * q_166[chn-1]);
                	// MEAN TIME OF BAR [ns];
                	mean_time_X = (time_166[chn] + time_166[chn-1])/2;               
                	// DELTA TIME OF BAR [ns]
                	delta_time_166 = (time_166[chn] - time_166[chn-1]);
               	}

               	//***********************************************************************************************************
               	// FILL THE HISTOGRAM
               	if (v_ampl_166>0.010 && q_166[chn]>0. && delta_time_166>-10.0 && delta_time_166<10.0) 
				{  
                	// V AMPLITUDE CHANNEL
                  	//hist_v_ampl_165[chn]->Fill(v_ampl_165);
                  	// Q CHARGE CHANNEL
                  	hist_q_166[chn]->Fill(q_166[chn]);
                  	
					if (chn%2!=0) 
					{
						// Q CHARGE BAR
                     	hist_q_bar_166[chn/2]->Fill(q_bar_166[chn/2]);
                     	// ∆T TIME CHANNELS
                     	hist_delta_time_166[chn/2]->Fill(delta_time_166);
                     	// ∆T VS LOG(Q_L/Q_R)
                     	hist_charge_time_166[chn/2]->Fill(log(q_166[chn]/q_166[chn-1]), delta_time_166);
                  	}
               	}
               	//else if (delta_time_166<-5.0 && delta_time_166>5.0) mean_time_X = -1;
               //***********************************************************************************************************

				if (1) 
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
               	}
			} // END if (board166_hit[chn] == 1 && board166_hit[chn+1] == 1)
      	} // END CHANNEL LOOP
   
      	for(int b=0; b<6; ++b)
      	{
        	if(board166_hit[2*b] && board166_hit[2*b+1])
        	{
	   			if(b==0)
	   			{
	     			Bar_TOF_X[8]  = 1;
	     			Bar_TOF[8]    = 1;
	   			}
	   			else if(b==1)
	   			{	
	     			Bar_TOF_X[9]  = 1;
	     			Bar_TOF[9]    = 1;
	   			} 	
	   			else if(b==2)
	   			{
	     			Bar_TOF_X[10] = 1;
	     			Bar_TOF[10]   = 1;
	   			}	   
	   			if(b==3)
	   			{
	     			Bar_TOF_Y[8]  = 1;
	     			Bar_TOF[28]   = 1;
	   			}
	   			else if(b==4)
	   			{
	     			Bar_TOF_Y[9]  = 1;
	     			Bar_TOF[29]  = 1;
	   			}
	   			else if(b==5)
	   			{	
	     			Bar_TOF_Y[10] = 1;
	     			Bar_TOF[30]   = 1;
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
		
		// FILL THE HISTOGRAM ∆T
      	for (int i=0; i<20; ++i)
      		{
				if (Time_Bar_TOF_X[i] == 0) continue;
            	for (int j=0; j<20; j++)
	    		{
	      			if (Time_Bar_TOF_Y[j] == 0) continue;
	      			//hist->Fill(i, 20+j);
	    		}
       		}

      
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
   	//c1->SaveAs("hitmap_hist2d.pdf");
   

   	TCanvas *c2 = new TCanvas("c2", "hitmap_projection",600,600);
   	c2->SetTickx();
   	c2->SetTicky();
   	c2->SetLeftMargin(0.15);
   	hist_project->GetXaxis()->SetTitle("Bar");
   	hist_project->GetYaxis()->SetTitle("Entries");
   	c2->SetLogy();
   	hist_project->SetNdivisions(20,"X");
   	hist_project->Draw("TEXT0 hist");
   	//c2->SaveAs("hitmap_hist_XY_proj.pdf");
   	//hfile.Write();


	//------------------------------
   	// HIST DELTA MEAN TIME
   	//------------------------------
	/*
   	TCanvas *c_delta_mean_time = new TCanvas("c_delta_mean_time", "c_delta_mean_time", 1200, 1200);
   	c_delta_mean_time->SetTickx();
   	c_delta_mean_time->SetTicky();
   	c_delta_mean_time->SetLeftMargin(0.15);
   	hist_mean_time->GetXaxis()->SetTitle("#DeltaT_{TOFwall} [ns]");
   	hist_mean_time->SetFillColor(38);
   	hist_mean_time->GetYaxis()->SetTitle("Counts");
   	//hist_mean_time->Fit("gaus", "Q");
   	hist_mean_time->Draw();

   	//c_delta_mean_time->SaveAs("delta_mean_time.pdf");
	*/

	/*
	//==================================================================================
   	// WAVEDREAM 165 - X VIEW (BAR 0 TO 7)

   	//------------------------------
   	// LOG(Q_L/Q_R) VS (T_L - T_R)
   	//------------------------------
   	//TCanvas *c_q_vs_t_165 = new TCanvas("c_q_vs_t_165", "c_q_vs_t_165", 1200, 1200);
   	//c_q_vs_t_165->Divide(4,2);
   	for (int b=0; b<8; b++) 
	{
      	//c_q_vs_t_165->cd(b+1);
		TString canvas_title_q_vs_t_165 = Form("c_q_vs_t_165%d", b); 
      	TCanvas *c_q_vs_t_165 = new TCanvas(canvas_title_q_vs_t_165, canvas_title_q_vs_t_165, 600, 600);
      	c_q_vs_t_165->SetTickx();
      	c_q_vs_t_165->SetTicky();

      	hist_charge_time_165[b]->SetStats(0);
      	hist_charge_time_165[b]->GetXaxis()->SetTitle("log(Q_L/Q_R)");
      	hist_charge_time_165[b]->GetYaxis()->SetTitle("(t_L - t_R) [ns]");
      	hist_charge_time_165[b]->Draw("colz");
   	}
   	//c_q_vs_t_165->SaveAs("figures/q_vs_t_165.pdf");

   	//------------------------------
   	// CHARGE CHANNEL WD165
   	//------------------------------
   	//TCanvas *c_q_165 = new TCanvas("c_q_165", "c_q_165", 1200, 1200);
   	//c_q_165->Divide(4,4);

   	for (int chn=0; chn<16; chn++) {  
      	//c_q_165->cd(chn+1);
      	TString canvas_title_q_165 = Form("c_q_165%d", chn); 
      	TCanvas *c_q_165 = new TCanvas(canvas_title_q_165, canvas_title_q_165, 600, 600);
      	c_q_165->SetTickx();
      	c_q_165->SetTicky();
      	c_q_165->SetLeftMargin(0.15);

      	hist_q_165[chn]->SetMarkerStyle(20);
      	hist_q_165[chn]->SetMarkerStyle(kFullCircle);
      	hist_q_165[chn]->SetMarkerColor(kBlack);
      	hist_q_165[chn]->SetFillColor(45);
      	//hist_q_165[chn]->Fit("landau", "Q");
      	hist_q_165[chn]->Draw();
   	}
   	//c_q_165->SaveAs("figures/q_165.pdf");

   	//------------------------------
   	// CHARGHE BAR WD165
   	//------------------------------
   	//TCanvas *c_q_bar_165 = new TCanvas("c_q_bar_165", "c_q_bar_165", 1200, 1200);
   	//c_q_bar_165->Divide(4,2);

   	for (int b=0; b<8; b++) {  
      	//c_q_bar_165->cd(b+1);
      	TString canvas_title_q_bar_165 = Form("c_q_bar_165%d", b); 
      	TCanvas *c_q_bar_165 = new TCanvas(canvas_title_q_bar_165, canvas_title_q_bar_165, 600, 600);
      	c_q_bar_165->SetTickx();
      	c_q_bar_165->SetTicky();
      	c_q_bar_165->SetLeftMargin(0.15);

      	hist_q_bar_165[b]->SetMarkerStyle(20);
      	hist_q_bar_165[b]->SetMarkerStyle(kFullCircle);
      	hist_q_bar_165[b]->SetMarkerColor(kBlack);
      	hist_q_bar_165[b]->SetFillColor(45);
      	//hist_q_bar_165[b]->Fit("landau", "Q");
      	hist_q_bar_165[b]->Draw();
   	}
   	//c_q_bar_165->SaveAs("figures/q_bar_165.pdf");
      
   	//------------------------------
   	// DELTA TIME WD165
   	//------------------------------
   	//TCanvas *c_delta_time_165 = new TCanvas("c_delta_time_165", "Delta t 165",1200,1200);
   	//c_delta_time_165->Divide(4,2);

   	for (int b=0; b<8; b++) {  
      	//c_delta_time_165->cd(b+1);
      	TString canvas_title_delta_time_165 = Form("c_delta_time_165bar%d",b); 
      	TCanvas *c_delta_time_165 = new TCanvas(canvas_title_delta_time_165, canvas_title_delta_time_165, 600, 600);
      	c_delta_time_165->SetTickx();
      	c_delta_time_165->SetTicky();
      	c_delta_time_165->SetLeftMargin(0.15);

      	hist_delta_time_165[b]->SetMarkerStyle(20);
      	hist_delta_time_165[b]->SetMarkerStyle(kFullCircle);
      	hist_delta_time_165[b]->SetMarkerColor(kBlack);
      	hist_delta_time_165[b]->SetFillColor(5); // Yellow
      	//hist_delta_time_165[b]->Fit("pol0", "Q");
      	hist_delta_time_165[b]->Draw();
   	}
   	//c_delta_time_165->SaveAs("figures/delta_time_165.pdf");

	*/








	//==================================================================================
   	// WAVEDREAM 166 - X VIEW (BAR 0 TO 7)

   	//------------------------------
   	// LOG(Q_L/Q_R) VS (T_L - T_R)
   	//------------------------------
   	//TCanvas *c_q_vs_t_166 = new TCanvas("c_q_vs_t_166", "c_q_vs_t_166", 1200, 1200);
   	//c_q_vs_t_166->Divide(4,2);
   	for (int b=0; b<12; b++) 
	{
      	//c_q_vs_t_166->cd(b+1);
		TString canvas_title_q_vs_t_166 = Form("c_q_vs_t_166%d", b); 
      	TCanvas *c_q_vs_t_166 = new TCanvas(canvas_title_q_vs_t_166, canvas_title_q_vs_t_166, 600, 600);
      	c_q_vs_t_166->SetTickx();
      	c_q_vs_t_166->SetTicky();
		c_q_vs_t_166->SetLogz();
      	
		hist_charge_time_166[b]->SetStats(0);
      	hist_charge_time_166[b]->GetXaxis()->SetTitle("log(Q_L/Q_R)");
      	hist_charge_time_166[b]->GetYaxis()->SetTitle("(t_L - t_R) [ns]");
      	
   	}
   	//c_q_vs_t_166->SaveAs("figures/q_vs_t_165.pdf");

   	//------------------------------
   	// CHARGE CHANNEL WD166
   	//------------------------------
   	//TCanvas *c_q_166 = new TCanvas("c_q_166", "c_q_166", 1200, 1200);
   	//c_q_166->Divide(4,4);
	/*
   	for (int chn=0; chn<12; chn++) {  
      	//c_q_166->cd(chn+1);
      	TString canvas_title_q_166 = Form("c_q_166%d", chn); 
      	TCanvas *c_q_166 = new TCanvas(canvas_title_q_166, canvas_title_q_166, 600, 600);
      	c_q_166->SetTickx();
      	c_q_166->SetTicky();
      	c_q_166->SetLeftMargin(0.15);
		c_q_166->SetLogy();
      	
		hist_q_166[chn]->SetMarkerStyle(20);
      	hist_q_166[chn]->SetMarkerStyle(kFullCircle);
      	hist_q_166[chn]->SetMarkerColor(kBlack);
      	//hist_q_166[chn]->SetFillColor(45);
      	//hist_q_166[chn]->Fit("landau", "Q");
		hist_q_166[chn]->SetNdivisions(25,"X");
      	hist_q_166[chn]->Draw();
		hist->GetZaxis()->SetTitle("# hits");
   	}
   	//c_q_166->SaveAs("figures/q_165.pdf");
	*/

   	//------------------------------
   	// CHARGHE BAR WD166
   	//------------------------------
   	//TCanvas *c_q_bar_166 = new TCanvas("c_q_bar_166", "c_q_bar_166", 1200, 1200);
   	//c_q_bar_166->Divide(4,2);

   	for (int b=0; b<6; b++) {  
      	//c_q_bar_166->cd(b+1);
      	TString canvas_title_q_bar_166 = Form("c_q_bar_166%d", b); 
      	TCanvas *c_q_bar_166 = new TCanvas(canvas_title_q_bar_166, canvas_title_q_bar_166, 600, 600);
      	c_q_bar_166->SetTickx();
      	c_q_bar_166->SetTicky();
      	c_q_bar_166->SetLeftMargin(0.15);
		c_q_bar_166->SetLogy();

      	hist_q_bar_166[b]->SetMarkerStyle(20);
      	hist_q_bar_166[b]->SetMarkerStyle(kFullCircle);
      	hist_q_bar_166[b]->SetMarkerColor(kBlack);
		hist_q_bar_166[b]->SetNdivisions(25,"X");
      	//hist_q_bar_166[b]->SetFillColor(45);
      	//hist_q_bar_166[b]->Fit("landau", "Q");
      	hist_q_bar_166[b]->Draw();
   	}
   	//c_q_bar_166->SaveAs("figures/q_bar_165.pdf");
      
   	//------------------------------
   	// DELTA TIME WD165
   	//------------------------------
   	//TCanvas *c_delta_time_166 = new TCanvas("c_delta_time_166", "Delta t 166",1200,1200);
   	//c_delta_time_166->Divide(4,2);

   	for (int b=0; b<6; b++) {  
      	//c_delta_time_166->cd(b+1);
      	TString canvas_title_delta_time_166 = Form("c_delta_time_166bar%d",b); 
      	TCanvas *c_delta_time_166 = new TCanvas(canvas_title_delta_time_166, canvas_title_delta_time_166, 600, 600);
      	c_delta_time_166->SetTickx();
      	c_delta_time_166->SetTicky();
      	c_delta_time_166->SetLeftMargin(0.15);

      	hist_delta_time_166[b]->SetMarkerStyle(20);
      	hist_delta_time_166[b]->SetMarkerStyle(kFullCircle);
      	hist_delta_time_166[b]->SetMarkerColor(kBlack);
      	hist_delta_time_166[b]->SetFillColor(5); // Yellow
      	//hist_delta_time_166[b]->Fit("pol0", "Q");
      	hist_delta_time_166[b]->Draw();
   	}
   	//c_delta_time_166->SaveAs("figures/delta_time_165.pdf");

}
