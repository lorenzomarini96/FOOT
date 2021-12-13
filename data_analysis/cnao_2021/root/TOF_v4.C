///////////////////////////////////////////////////////////////////////
// hit_map_10nov_slim.C
// Macro to reconstruct the hit map of the tofwall detector.
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

	//------------------------------
   	// HIST FOR TIME RESOLUTION
   	//------------------------------
   	TH1D *hist_TOF = new TH1D("hist_TOF", "hist_TOF", 25, 17, 17.7); // [ns]

   	//------------------------------
   	// HISTOGRAMS CHARGHE BAR WD166
   	//------------------------------
   	TH1F *hist_q_bar_166[16];
   	char name_q_bar_166[20];
   	char title_q_bar_166[100];

   	for (Int_t b=0; b<6; b++) 
	{
		if (b<3)
		{
			int x_bar = b + 8;
		    sprintf(name_q_bar_166, "hist_q_bar_166%d", x_bar);
      		sprintf(title_q_bar_166, "Bar%d", x_bar);
		}
		else
		{
			int y_bar = b + 25;
			sprintf(name_q_bar_166, "hist_q_bar_166%d", y_bar);
      		sprintf(title_q_bar_166, "Bar%d", y_bar);
		}
      	hist_q_bar_166[b] = new TH1F(name_q_bar_166,title_q_bar_166, 60, 0, 20);
      	hist_q_bar_166[b]->GetXaxis()->SetTitle("Q [a.u.]");
      	hist_q_bar_166[b]->GetYaxis()->SetTitle("Entries");
   	}

  	//------------------------------
   	// HISTOGRAMS Q VS TOF
   	//------------------------------
	TH2D *hist_charge_TOF_X = new TH2D("hist_charge_TOF_X", "hist_charge_TOF_X", 200, 12.5, 22.5, 100, 0.0, 20.0);
	TH2D *hist_charge_TOF_Y = new TH2D("hist_charge_TOF_Y", "hist_charge_TOF_Y", 200, 12.5, 22.5, 100, 0.0, 20.0);
	
   	Double_t f_CFD = 0.3;         // FRATION FOR COMPUTE TIME
	Int_t status[8];

  	// LOOP ON ENTRIES
  	for (Long64_t jentry=0; jentry<nentries/10; jentry++)
   	{
    	Long64_t ientry = LoadTree(jentry);
      	if (ientry < 0) break;
      	nb = fChain->GetEntry(jentry);   nbytes += nb;	


		Double_t TOF_9_X[nentries/10];
		Double_t TOF_9_Y[nentries/10];
		Double_t Q_9_X[nentries/10];
		Double_t Q_9_Y[nentries/10];

		//Double_t q_9_X;
		//Double_t q_9_Y;
		//Double_t tof_9_Y;
		//Double_t tof_9_Y;


		//if (tcb17_trigger_type == 1)
		//{
		//****************************************************************************************************
		// CLOCK ANALYSY CORRECTION
		//****************************************************************************************************
	
		//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
		// DEFINITION
		//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++

		Double_t delta_CLK_X = 0; // [ns] REAR,  X-VIEW
		Double_t delta_CLK_Y = 0; // [ns] FRONT, Y-VIEW

		//*********************************************************
		// START COUNTER
		//*********************************************************
		Int_t n_points_SC = 0; 					// number of points for graphs of wf
      	Int_t chn_SC_CLK = 16;
		Double_t voltage_SC_CLK_NC[1023]; 		// No correction
		Double_t voltage_SC_CLK[1023];
		Double_t time_SC_CLK[1023];

		// CLOCK ANALYSIS
		Double_t min_SC_CLK;                  	// V AMPLITUDE MIN OF CHANNEL [V]
		Double_t max_SC_CLK;                  	// V AMPLITUDE MIN OF CHANNEL [V]
		Double_t a_fit_SC_CLK;
		Double_t sigma_a_fit_SC_CLK;
		Double_t b_fit_SC_CLK;
		Double_t sigma_b_fit_SC_CLK;
		Double_t phi_SC_CLK;		     
		Double_t zero_SC_CLK;                 	// MEAN OF MAX AND MIN VALUES OF WF
		Double_t ZeroCrossingPoint_SC[27];      // ZERO CROSSING POINT ON THE RISING EDGES OF WF [ns]
		Double_t sigma_ZeroCrossingPoint_SC[27];// ERROR ON ZERO CROSSING POINT [ns]
		Double_t N_SC_CLK[27];			      	// NUMBER OF CLOCK CYCLES
		Double_t sigma_N_SC_CLK[27];		  	// NUMBER OF CLOCK CYCLES
		Int_t n_point_SC_phi = 27;		

		//*********************************************************
		// TOF-WALL REAR
		//*********************************************************
		Int_t n_points_TW_X = 0; 					// number of points for graphs of wf
      	Int_t chn_TW_CLK_X = 16;
		Double_t voltage_TW_CLK_NC_X[1023];		    // No correction
		Double_t voltage_TW_CLK_X[1023];
		Double_t time_TW_CLK_X[1023];

		// CLOCK ANALYSIS
		Double_t min_TW_CLK_X;                  	// V AMPLITUDE MIN OF CHANNEL [V]
		Double_t max_TW_CLK_X;                  	// V AMPLITUDE MIN OF CHANNEL [V]
		Double_t a_fit_TW_CLK_X;
		Double_t sigma_a_fit_TW_CLK_X;
		Double_t b_fit_TW_CLK_X;
		Double_t sigma_b_fit_TW_CLK_X;
		Double_t phi_TW_CLK_X;		     
		Double_t zero_TW_CLK_X;                     // MEAN OF MAX AND MIN VALUES OF WF
		Double_t ZeroCrossingPoint_TW_X[27];        // ZERO CROSSING POINT ON THE RISING EDGES OF WF [ns]
		Double_t sigma_ZeroCrossingPoint_TW_X[27];  // ERROR ON ZERO CROSSING POINT [ns]
		Double_t N_TW_CLK_X[27];			     	// NUMBER OF CLOCK CYCLES
		Double_t sigma_N_TW_CLK_X[27];		  	    // ERROR ON NUMBER OF CLOCK CYCLES
		Int_t n_point_TW_phi_X = 27;

		//*********************************************************
		// TOF-WALL REAR FRONT
		//*********************************************************
		Int_t n_points_TW_Y = 0; 					// number of points for graphs of wf
      	Int_t chn_TW_CLK_Y = 17;
		Double_t voltage_TW_CLK_NC_Y[1023];		// No correction
		Double_t voltage_TW_CLK_Y[1023];
		Double_t time_TW_CLK_Y[1023];

		// CLOCK ANALYSIS
		Double_t min_TW_CLK_Y;                  	// V AMPLITUDE MIN OF CHANNEL [V]
		Double_t max_TW_CLK_Y;                  	// V AMPLITUDE MIN OF CHANNEL [V]
		Double_t a_fit_TW_CLK_Y;
		Double_t sigma_a_fit_TW_CLK_Y;
		Double_t b_fit_TW_CLK_Y;
		Double_t sigma_b_fit_TW_CLK_Y;
		Double_t phi_TW_CLK_Y;		     
		Double_t zero_TW_CLK_Y;                   // MEAN OF MAX AND MIN VALUES OF WF
		Double_t ZeroCrossingPoint_TW_Y[27];      // ZERO CROSSING POINT ON THE RISING EDGES OF WF [ns]
		Double_t sigma_ZeroCrossingPoint_TW_Y[27];// ERROR ON ZERO CROSSING POINT [ns]
		Double_t N_TW_CLK_Y[27];			     	// NUMBER OF CLOCK CYCLES
		Double_t sigma_N_TW_CLK_Y[27];		  	// ERROR ON NUMBER OF CLOCK CYCLES
		Int_t n_point_TW_phi_Y = 27;
		
		//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
		// PROCESSING
		//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++

		//*********************************************************
		// START-COUNTER
		//*********************************************************

		//---------------------------------------------------------
		// 1) CORRECTION OF WF
		//---------------------------------------------------------
		Int_t enablesum = 0;

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

		//---------------------------------------------------------
        // 2) FIND MIN AND MAX OF WAVEFORM
		//---------------------------------------------------------
        min_SC_CLK = voltage_SC_CLK[100]; // V AMPLITUDE MIN OF CHANNEL [V]
        max_SC_CLK = voltage_SC_CLK[100]; // V AMPLITUDE MAX OF CHANNEL [V]
            
        for (Int_t i=0; i<1000; i++)
        { 		
			// LOOP ON SAMPLES OF WAVEFORM
           	if (voltage_SC_CLK[i] < min_SC_CLK) min_SC_CLK = voltage_SC_CLK[chn_SC_CLK];
        	if (voltage_SC_CLK[i] > max_SC_CLK) max_SC_CLK = voltage_SC_CLK[i];
		}
		zero_SC_CLK = (min_SC_CLK + max_SC_CLK)/2;

		//---------------------------------------------------------
		// 3) ZERO CROSSING POINT
		//---------------------------------------------------------
		Int_t j = 0;

        for (Int_t i=0; i<1000; i++) 
		{ 	
            if (voltage_SC_CLK[i] > zero_SC_CLK)
			{
				TF1 *f_fit_SC_CLK = new TF1("f_fit_SC_CLK", "pol1", time_SC_CLK[i-2], time_SC_CLK[i+2]);
				
				Int_t n = 5;  // NUMBER OF POINTS FOR THE GRAPH
				Double_t v_data_SC_CLK[5] = {voltage_SC_CLK[i-2], voltage_SC_CLK[i-1], voltage_SC_CLK[i], voltage_SC_CLK[i+1], voltage_SC_CLK[i+2]};
				Double_t t_data_SC_CLK[5] = {time_SC_CLK[i-2], time_SC_CLK[i-1], time_SC_CLK[i], time_SC_CLK[i+1], time_SC_CLK[i+2]};

				TGraph *gr_SC_CLK = new TGraph(n, t_data_SC_CLK, v_data_SC_CLK);

				gr_SC_CLK->Fit("f_fit_SC_CLK","Qr");			
					
				a_fit_SC_CLK       = f_fit_SC_CLK->GetParameter(1);
				sigma_a_fit_SC_CLK = f_fit_SC_CLK->GetParError(1);
				b_fit_SC_CLK       = f_fit_SC_CLK->GetParameter(0);
				sigma_b_fit_SC_CLK = f_fit_SC_CLK->GetParError(0);

				// ZERO CROSSING POINT
				ZeroCrossingPoint_SC[j]       = (zero_SC_CLK - b_fit_SC_CLK)/a_fit_SC_CLK; // [ns]
				sigma_ZeroCrossingPoint_SC[j] = ZeroCrossingPoint_SC[j] * sqrt(pow(sigma_a_fit_SC_CLK/a_fit_SC_CLK,2) + pow(sigma_b_fit_SC_CLK/b_fit_SC_CLK,2)); // Somma in quadratura degli errori sui parametri di best-fit

				// NUMBER OF CLOCK CYCLES
				N_SC_CLK[j] = j+1;

				delete f_fit_SC_CLK;
				delete gr_SC_CLK;
					
				i += 20;
				j += 1;
            }
			if (N_SC_CLK[j] == 25) break;
        }

		// DATA CORRECTION (problem with the first element)
		Double_t new_ZeroCrossingPoint_SC[25];
		Double_t new_sigma_ZeroCrossingPoint_SC[25];
		Double_t new_N_SC_CLK[25];
		Double_t new_sigma_N_SC_CLK[25];
		Int_t    new_n_point_SC_phi = n_point_SC_phi-2;

		for (Int_t i=0; i<25; i++)
		{	
			// SHIFT ALL ELEMENTS BY ONE POSITION
			new_N_SC_CLK[i] 				  = N_SC_CLK[i];
			new_sigma_N_SC_CLK[i] 			  = 0;
			new_ZeroCrossingPoint_SC[i]		  = ZeroCrossingPoint_SC[i+1];
			new_sigma_ZeroCrossingPoint_SC[i] = sigma_ZeroCrossingPoint_SC[i];
		}

		TGraph *gr_N_SC_CLK_vs_ZeroCrossingPoint = new TGraphErrors(new_n_point_SC_phi, new_N_SC_CLK, new_ZeroCrossingPoint_SC, new_sigma_N_SC_CLK, new_sigma_ZeroCrossingPoint_SC);
		TF1 *f_fit_N_SC_CLK_vs_ZeroCrossingPoint = new TF1("f_fit_N_SC_CLK_vs_ZeroCrossingPoint", "pol1", 0, N_SC_CLK[24]);
		gr_N_SC_CLK_vs_ZeroCrossingPoint->Fit("f_fit_N_SC_CLK_vs_ZeroCrossingPoint","Qr");

		phi_SC_CLK = f_fit_N_SC_CLK_vs_ZeroCrossingPoint->GetParameter(0);
		
		//*********************************************************
		// TOF-WALL REAR
		//*********************************************************

		//---------------------------------------------------------
		// 1) CORRECTION OF WF
		//---------------------------------------------------------
		enablesum = 0;

		for (Int_t i=0; i<1023; i++)     
		{
			voltage_TW_CLK_X[i]    = board166_waveform[chn_TW_CLK_X][i]; // Final Amplitude
			voltage_TW_CLK_NC_X[i] = board166_waveform[chn_TW_CLK_X][i]; // Amplitude Not Corrected
			time_TW_CLK_X[i]       = board166_time[chn_TW_CLK_X][i] * TMath::Power(10,9);

			if (voltage_TW_CLK_NC_X[i] - voltage_TW_CLK_NC_X[i-1] >  0.5) enablesum -= 1;
			if (voltage_TW_CLK_NC_X[i] - voltage_TW_CLK_NC_X[i-1] < -0.5) enablesum += 1;

			voltage_TW_CLK_X[i] = voltage_TW_CLK_NC_X[i] + enablesum; // sommo 1V
			n_points_TW_X += 1;
		}

		//---------------------------------------------------------
        // 2) FIND MIN AND MAX OF WAVEFORM
		//---------------------------------------------------------
        min_TW_CLK_X = voltage_TW_CLK_X[100]; // V AMPLITUDE MIN OF CHANNEL [V]
        max_TW_CLK_X = voltage_TW_CLK_X[100]; // V AMPLITUDE MAX OF CHANNEL [V]
            
        for (Int_t i=0; i<1000; i++)
        { 		
			// LOOP ON SAMPLES OF WAVEFORM
           	if (voltage_TW_CLK_X[i] < min_TW_CLK_X) min_TW_CLK_X = voltage_TW_CLK_X[i];
        	if (voltage_TW_CLK_X[i] > max_TW_CLK_X) max_TW_CLK_X = voltage_TW_CLK_X[i];
		}

		zero_TW_CLK_X = (min_TW_CLK_X + max_TW_CLK_X)/2;

		//---------------------------------------------------------
		// 3) ZERO CROSSING POINT
		//---------------------------------------------------------
        Int_t j_TW_X = 0;

		for (Int_t i=0; i<1000; i++)
		{ 	
            if (voltage_TW_CLK_X[i] > zero_TW_CLK_X)
			{
				TF1 *f_fit_TW_CLK_X = new TF1("f_fit_TW_CLK_X", "pol1", time_TW_CLK_X[i-2], time_TW_CLK_X[i+2]);
				
				Int_t n = 5;  // NUMBER OF POINTS FOR THE GRAPH
				Double_t v_data_TW_CLK_X[5] = {voltage_TW_CLK_X[i-2], voltage_TW_CLK_X[i-1], voltage_TW_CLK_X[i], voltage_TW_CLK_X[i+1], voltage_TW_CLK_X[i+2]};
				Double_t t_data_TW_CLK_X[5] = {time_TW_CLK_X[i-2], time_TW_CLK_X[i-1], time_TW_CLK_X[i], time_TW_CLK_X[i+1], time_TW_CLK_X[i+2]};

				TGraph *gr_TW_CLK_X = new TGraph(n, t_data_TW_CLK_X, v_data_TW_CLK_X);

				gr_TW_CLK_X->Fit("f_fit_TW_CLK_X","Qr");			
					
				a_fit_TW_CLK_X       = f_fit_TW_CLK_X->GetParameter(1);
				sigma_a_fit_TW_CLK_X = f_fit_TW_CLK_X->GetParError(1);
				b_fit_TW_CLK_X       = f_fit_TW_CLK_X->GetParameter(0);
				sigma_b_fit_TW_CLK_X = f_fit_TW_CLK_X->GetParError(0);

				// ZERO CROSSING POINT
				ZeroCrossingPoint_TW_X[j_TW_X]       = (zero_TW_CLK_X - b_fit_TW_CLK_X)/a_fit_TW_CLK_X; // [ns]
				sigma_ZeroCrossingPoint_TW_X[j_TW_X] = ZeroCrossingPoint_TW_X[j_TW_X] * sqrt(pow(sigma_a_fit_TW_CLK_X/a_fit_TW_CLK_X,2) + pow(sigma_b_fit_TW_CLK_X/b_fit_TW_CLK_X,2)); // Somma in quadratura degli errori sui parametri di best-fit

				// NUMBER OF CLOCK CYCLES
				N_TW_CLK_X[j_TW_X] = j_TW_X + 1;

				delete f_fit_TW_CLK_X;
				delete gr_TW_CLK_X;
					
				i += 20;
				j_TW_X += 1;
            }
			if (N_TW_CLK_X[j_TW_X] == 25) break;
		}

		// DATA CORRECTION (problem with the first element)
		Double_t new_ZeroCrossingPoint_TW_X[25];
		Double_t new_sigma_ZeroCrossingPoint_TW_X[25];
		Double_t new_N_TW_CLK_X[25];
		Double_t new_sigma_N_TW_CLK_X[25];
		Int_t    new_n_point_TW_phi_X = n_point_TW_phi_X-2;

		for (Int_t i=0; i<25; i++)
		{	
			// SHIFT ALL ELEMENTS BY ONE POSITION
			new_N_TW_CLK_X[i] 				    = N_TW_CLK_X[i];
			new_sigma_N_TW_CLK_X[i] 			= 0;
			new_ZeroCrossingPoint_TW_X[i]		= ZeroCrossingPoint_TW_X[i+1];
			new_sigma_ZeroCrossingPoint_TW_X[i] = sigma_ZeroCrossingPoint_TW_X[i+1];
		}

		// TOF-WALL
		TGraph *gr_N_TW_CLK_vs_ZeroCrossingPoint_X = new TGraphErrors(new_n_point_TW_phi_X, new_N_TW_CLK_X, new_ZeroCrossingPoint_TW_X, new_sigma_N_TW_CLK_X, new_sigma_ZeroCrossingPoint_TW_X);
		TF1 *f_fit_N_TW_CLK_vs_ZeroCrossingPoint_X = new TF1("f_fit_N_TW_CLK_vs_ZeroCrossingPoint_X", "pol1", 0, N_TW_CLK_X[24]);
		gr_N_TW_CLK_vs_ZeroCrossingPoint_X->Fit("f_fit_N_TW_CLK_vs_ZeroCrossingPoint_X","Qr");

		phi_TW_CLK_X = f_fit_N_TW_CLK_vs_ZeroCrossingPoint_X->GetParameter(0);

		delta_CLK_X = phi_TW_CLK_X - phi_SC_CLK;

		//*********************************************************
		// TOF-WALL FRONT
		//*********************************************************

		//---------------------------------------------------------
		// 1) CORRECTION OF WF
		//---------------------------------------------------------
		enablesum = 0;

		for (Int_t i=0; i<1023; i++)     
		{
			voltage_TW_CLK_Y[i]    = board166_waveform[chn_TW_CLK_Y][i]; // Final Amplitude
			voltage_TW_CLK_NC_Y[i] = board166_waveform[chn_TW_CLK_Y][i]; // Amplitude Not Corrected
			time_TW_CLK_Y[i]       = board166_time[chn_TW_CLK_Y][i] * TMath::Power(10,9);

			if (voltage_TW_CLK_NC_Y[i] - voltage_TW_CLK_NC_Y[i-1] >  0.5) enablesum -= 1;
			if (voltage_TW_CLK_NC_Y[i] - voltage_TW_CLK_NC_Y[i-1] < -0.5) enablesum += 1;

			voltage_TW_CLK_Y[i] = voltage_TW_CLK_NC_Y[i] + enablesum; // sommo 1V
			n_points_TW_Y += 1;
		}

		//---------------------------------------------------------
        // 2) FIND MIN AND MAX OF WAVEFORM
		//---------------------------------------------------------
        min_TW_CLK_Y = voltage_TW_CLK_Y[100]; // V AMPLITUDE MIN OF CHANNEL [V]
        max_TW_CLK_Y = voltage_TW_CLK_Y[100]; // V AMPLITUDE MAX OF CHANNEL [V]
            
        for (Int_t i=0; i<1000; i++)
        { 		
			// LOOP ON SAMPLES OF WAVEFORM
           	if (voltage_TW_CLK_Y[i] < min_TW_CLK_Y) min_TW_CLK_Y = voltage_TW_CLK_Y[i];
        	if (voltage_TW_CLK_Y[i] > max_TW_CLK_Y) max_TW_CLK_Y = voltage_TW_CLK_Y[i];
		}

		zero_TW_CLK_Y = (min_TW_CLK_Y + max_TW_CLK_Y)/2;

		//---------------------------------------------------------
		// 3) ZERO CROSSING POINT
		//---------------------------------------------------------
        Int_t j_TW_Y = 0;

		for (Int_t i=0; i<1000; i++)
		{ 	
            if (voltage_TW_CLK_Y[i] > zero_TW_CLK_Y)
			{
				TF1 *f_fit_TW_CLK_Y = new TF1("f_fit_TW_CLK_Y", "pol1", time_TW_CLK_Y[i-2], time_TW_CLK_Y[i+2]);
				
				Int_t n_Y = 5;  // NUMBER OF POINTS FOR THE GRAPH
				Double_t v_data_TW_CLK_Y[5] = {voltage_TW_CLK_Y[i-2], voltage_TW_CLK_Y[i-1], voltage_TW_CLK_Y[i], voltage_TW_CLK_Y[i+1], voltage_TW_CLK_Y[i+2]};
				Double_t t_data_TW_CLK_Y[5] = {time_TW_CLK_Y[i-2], time_TW_CLK_Y[i-1], time_TW_CLK_Y[i], time_TW_CLK_Y[i+1], time_TW_CLK_Y[i+2]};

				TGraph *gr_TW_CLK_Y = new TGraph(n_Y, t_data_TW_CLK_Y, v_data_TW_CLK_Y);

				gr_TW_CLK_Y->Fit("f_fit_TW_CLK_Y","Qr");			
					
				a_fit_TW_CLK_Y       = f_fit_TW_CLK_Y->GetParameter(1);
				sigma_a_fit_TW_CLK_Y = f_fit_TW_CLK_Y->GetParError(1);
				b_fit_TW_CLK_Y       = f_fit_TW_CLK_Y->GetParameter(0);
				sigma_b_fit_TW_CLK_Y = f_fit_TW_CLK_Y->GetParError(0);

				// ZERO CROSSING POINT
				ZeroCrossingPoint_TW_Y[j_TW_Y]       = (zero_TW_CLK_Y - b_fit_TW_CLK_Y)/a_fit_TW_CLK_Y; // [ns]
				sigma_ZeroCrossingPoint_TW_Y[j_TW_Y] = ZeroCrossingPoint_TW_Y[j_TW_Y] * sqrt(pow(sigma_a_fit_TW_CLK_Y/a_fit_TW_CLK_Y,2) + pow(sigma_b_fit_TW_CLK_Y/b_fit_TW_CLK_Y,2)); // Somma in quadratura degli errori sui parametri di best-fit

				// NUMBER OF CLOCK CYCLES
				N_TW_CLK_Y[j_TW_Y] = j_TW_Y + 1;

				delete f_fit_TW_CLK_Y;
				delete gr_TW_CLK_Y;
					
				i += 20;
				j_TW_Y += 1;
            }
			if (N_TW_CLK_Y[j_TW_Y] == 25) break;
		}

		// DATA CORRECTION (problem with the first element)
		Double_t new_ZeroCrossingPoint_TW_Y[25];
		Double_t new_sigma_ZeroCrossingPoint_TW_Y[25];
		Double_t new_N_TW_CLK_Y[25];
		Double_t new_sigma_N_TW_CLK_Y[25];
		Int_t    new_n_point_TW_phi_Y = n_point_TW_phi_Y-2;

		for (Int_t i=0; i<25; i++)
		{	
			// SHIFT ALL ELEMENTS BY ONE POSITION
			new_N_TW_CLK_Y[i] 			     	= N_TW_CLK_Y[i];
			new_sigma_N_TW_CLK_Y[i] 			= 0;
			new_ZeroCrossingPoint_TW_Y[i]		= ZeroCrossingPoint_TW_Y[i+1];
			new_sigma_ZeroCrossingPoint_TW_Y[i] = sigma_ZeroCrossingPoint_TW_Y[i+1];
		}

		// TOF-WALL
		TGraph *gr_N_TW_CLK_vs_ZeroCrossingPoint_Y = new TGraphErrors(new_n_point_TW_phi_Y, new_N_TW_CLK_Y, new_ZeroCrossingPoint_TW_Y, new_sigma_N_TW_CLK_Y, new_sigma_ZeroCrossingPoint_TW_Y);
		TF1 *f_fit_N_TW_CLK_vs_ZeroCrossingPoint_Y = new TF1("f_fit_N_TW_CLK_vs_ZeroCrossingPoint_Y", "pol1", 0, N_TW_CLK_Y[24]);
		gr_N_TW_CLK_vs_ZeroCrossingPoint_Y->Fit("f_fit_N_TW_CLK_vs_ZeroCrossingPoint_Y","Qr");

		phi_TW_CLK_Y = f_fit_N_TW_CLK_vs_ZeroCrossingPoint_Y->GetParameter(0);

		delta_CLK_Y = phi_TW_CLK_Y - phi_SC_CLK;

		//****************************************************************************

		//****************************************************************************
		// TOF-WALL ANALYSIS
		//****************************************************************************

		Double_t time_TOF = 0.;
		Double_t time_TW  = 0.;
		Double_t time_TW_all  = 0.;
		Double_t Mean_Time_Bar_9_X = 0;                    // T_bar_layer = (T_L + T_R)/2
   		Double_t Mean_Time_Bar_9_Y = 0;                    // T_bar_front = (T_L + T_R)/2
      
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
		

		// LOOP ON CHANNELS OF WAVEDREAM
      	for (Int_t chn=0; chn<12; chn++) 
      	{
			if (board166_hit[chn] == 1)
			{
            	// INITIALIZE VALUES

            	Double_t voltage_166;                         
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

				// TIME OF CHANNEL
				Int_t n = 5;
               	for (Int_t i=i_min_position_166; ; i--) { // LOOP ON SAMPLES OF WAVEFORM
                  	if (board166_waveform[chn][i] > v_th_166) 
					{
						TF1 *f_fit = new TF1("f_fit", "pol1", board166_time[chn][i-2], board166_time[chn][i+2]); // Range board166_waveform[chn][t-2], board166_waveform[chn][t+2]

						Double_t v_data_fit[5] = {board166_waveform[chn][i-2], board166_waveform[chn][i-1], board166_waveform[chn][i], board166_waveform[chn][i+1], board166_waveform[chn][i+2]};
						Double_t t_data_fit[5] = {board166_time[chn][i-2], board166_time[chn][i-1], board166_time[chn][i], board166_time[chn][i+1], board166_time[chn][i+2]};
						
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
				
				// CHARGE OF BAR
				if (chn%2!=0) q_bar_166[chn/2] = sqrt(q_166[chn] * q_166[chn-1]);
               	
				// MEAN TIME OF BAR [ns]
				if (chn==3)      
				{
					Mean_Time_Bar_9_X = (time_166[3] + time_166[2])/2; // T_bar_layer = (T_R + T_L) / 2
					Q_9_X[jentry]     = q_bar_166[1];
				}
				else if (chn==9) 
				{
					Mean_Time_Bar_9_Y = (time_166[9] + time_166[8])/2; // T_bar_front = (T_R + T_L) / 2
					Q_9_Y[jentry]     = q_bar_166[4];
				}
				
               	//***********************************************************************************************************
               	// FILL THE HISTOGRAM
               	if (v_ampl_166>0.010 && q_166[chn]>0. && chn%2!=0) hist_q_bar_166[chn/2]->Fill(q_bar_166[chn/2]);

				// TIME OF TOFWALL
				//if (Mean_Time_Bar_9_X > 0. && Mean_Time_Bar_9_Y > 0. && delta_CLK_X > 0. && delta_CLK_Y > 0. && q_bar_166[1] > 13.0 & q_bar_166[4] > 13.0) // CUT ON THE FRAGMENT 
				if (Mean_Time_Bar_9_X > 0. && Mean_Time_Bar_9_Y > 0. && delta_CLK_X > 0. && delta_CLK_Y > 0. && q_bar_166[1] > 0.0 & q_bar_166[4] > 0.0)     // WITH FRAGMENT
				{
					// delta_CLK_X = phi_TW_CLK_X - phi_SC_CLK;
					// delta_CLK_Y = phi_TW_CLK_Y - phi_SC_CLK;
					time_TW     = (Mean_Time_Bar_9_X - delta_CLK_X + Mean_Time_Bar_9_Y - delta_CLK_Y)/2;	
				}

				// TIME OF TOFWALL
				if (Mean_Time_Bar_9_X > 0. && Mean_Time_Bar_9_Y > 0. && delta_CLK_X > 0. && delta_CLK_Y > 0.) 
				{
					time_TW_all  = (Mean_Time_Bar_9_X - delta_CLK_X + Mean_Time_Bar_9_Y - delta_CLK_Y)/2;	
				}
				
               //***********************************************************************************************************

				if (0)
				{
                	std::cout << "\n**************************"                           << std::endl;
                	std::cout << "\nTOF-WALL"                                               << std::endl;
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
                  	std::cout << "\ntime_166           [ns]  "       << time_166[chn]     << std::endl;
					std::cout << "\n------------------------------------------"           << std::endl;
					std::cout << "\nMean_Time_Bar_9_X  [ns]  " 		 << Mean_Time_Bar_9_X << std::endl;
					std::cout << "\nMean_Time_Bar_9_Y  [ns]  "       << Mean_Time_Bar_9_Y << std::endl;
					std::cout << "\ntime_TOF           [ns]  "       << time_TOF          << std::endl;
               	}
			} // END if (board166_hit[chn] == 1 && board166_hit[chn+1] == 1)
      	} // END CHANNEL LOOP
		
		//****************************************************************************
		// START-COUNTER
		//****************************************************************************
		Double_t time_mean_SC = 0;
		Double_t time_sum_SC  = 0; // Per calcolo media ponderata del tempo (pesata sui valori di ampiezza)
		Double_t ampl_sum_SC  = 0; // Per calcolo media ponderata del tempo (pesata sui valori di ampiezza)

		//---------------------------------------------------------
		// 1) CORRECTION OF WF
		//---------------------------------------------------------
		Int_t enablesum_SC = 0;
		Double_t voltage_SC[8][1023];

     	// LOOP ON CHANNELS OF WAVEDREAM
      	for (Int_t chn=0; chn<8; chn++)
      	{	
			for (Int_t i=1; i<1023; i++)     
			{
				voltage_SC[chn][i] = board173_waveform[chn][i]; // Final Amplitude
				if (board173_waveform[chn][i] - board173_waveform[chn][i-1] >  0.5) enablesum_SC -= 1;
				if (board173_waveform[chn][i] - board173_waveform[chn][i-1] < -0.5) enablesum_SC += 1;

				voltage_SC[chn][i] = board173_waveform[chn][i] + enablesum_SC; // sommo 1V
			}
			
       		status[chn] = 1;
			
			// 1° CHECK SIGNAL
        	// FIND MIN AND MAX OF WAVEFORM
        	Double_t min_SC = voltage_SC[chn][100]; // V AMPLITUDE MIN OF CHANNEL [V]
        	Double_t max_SC = voltage_SC[chn][100]; // V AMPLITUDE MAX OF CHANNEL [V]
            
        	for (Int_t i=100; i<900; i++) 
        	{ // LOOP ON SAMPLES OF WAVEFORM
           		if (voltage_SC[chn][i] < min_SC) min_SC = voltage_SC[chn][i];
           		if (voltage_SC[chn][i] > max_SC) max_SC = voltage_SC[chn][i];
        	}

			if (max_SC - min_SC > 0.95)  status[chn] = 0;  // NOISE
        	if (max_SC - min_SC < 0.010) status[chn] = 0; // NOISE
               
        	//if (status[chn]==0) board173_hit[chn] = 0;

	        //if (board173_hit[chn]==1)
			if (status[chn] == 1)
			{
            	// INITIALIZE VALUES   
				Double_t a_fit;
				Double_t b_fit;
            	Double_t v_base_SC;                               // V BASELINE (PEDESTAL) [V]
            	Double_t sigma_v_base_SC;                         // V BASELINE (PEDESTAL) [V]
            	Double_t v_ampl_SC;                               // V AMPLITUDE OF CHANNEL [V]
            	Double_t v_peak_SC = voltage_SC[chn][300];        // V MINIMUM (PEAK)[V]
            	Double_t t_peak_SC;                               // TIME CORRESPONDING TO PEAK
            	Double_t v_th_SC;                                 // V THRESHOLD [V]
            	Double_t t_stop_SC;                               // TIME STOP OF SIGNAL[ns]
            	Double_t i_start_SC;                              // V START OF SIGNAL[V]
            	Double_t i_stop_SC;                               // V STOP OF SIGNAL [V]
            	Double_t i_min_position_SC;                       // POSITION OF V PEAK
            	Double_t time_SC[8];                              // TIME OF WF OF CHANNEL [ns]

				// V BASELINE   
               	TH1F *wf_SC = new TH1F("wf_SC","wf_SC", 30, 0.30, 0.60);
               	for (Int_t i=30; i<250; i++) wf_SC->Fill(voltage_SC[chn][i]);
				v_base_SC       = wf_SC->GetMean();
               	sigma_v_base_SC = wf_SC->GetRMS(); // TO EVALUATE THE NOISE (FOR FUTURES...)

               	wf_SC->SetDirectory(0); // remove an histogram from the gDirectory list
            	
				// V PEAK (MINIMUM)
               	for (Int_t i=100; i<800; i++)
				{ // LOOP ON SAMPLES OF WAVEFORM
                  	if (voltage_SC[chn][i] < v_peak_SC) 
					{
						i_min_position_SC = i;
                    	v_peak_SC = voltage_SC[chn][i];
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
               	for (Int_t i=i_min_position_SC; ; i--) 
				{ // LOOP ON SAMPLES OF WAVEFORM
                  	if (voltage_SC[chn][i] > v_th_SC) 
					{
						TF1 *f_fit = new TF1("f_fit", "pol1", board173_time[chn][i-2], board173_time[chn][i+2]);

						Double_t v_data_fit[5] = {voltage_SC[chn][i-2], voltage_SC[chn][i-1], voltage_SC[chn][i], voltage_SC[chn][i+1], voltage_SC[chn][i+2]};
						Double_t t_data_fit[5] = {board173_time[chn][i-2], board173_time[chn][i-1], board173_time[chn][i], board173_time[chn][i+1], board173_time[chn][i+2]};
						
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

				// MEAN TIME OF SC
				time_sum_SC += time_SC[chn]*v_ampl_SC;
				ampl_sum_SC += v_ampl_SC;
				time_mean_SC = time_sum_SC/ampl_sum_SC;

				if (0) 
				{
                	std::cout << "\nSTART COUNTER"                              << std::endl;
                  	std::cout << "\nEntry               " << jentry             << std::endl;
                  	std::cout << "\nchn                 " << chn                << std::endl;
                  	std::cout << "\nmin [V]             " << min_SC             << std::endl;
                  	std::cout << "\nmax [V]             " << max_SC             << std::endl;
                  	std::cout << "\n------------------------------------------" << std::endl;
                  	std::cout << "\nv_base       [V]    " << v_base_SC          << std::endl;
                  	std::cout << "\nsigma_v_base [V]    " << sigma_v_base_SC    << std::endl;
                  	std::cout << "\nv_peak       [V]    " << v_peak_SC          << std::endl;
                  	std::cout << "\nt_peak       [ns]   " << t_peak_SC          << std::endl;
                	std::cout << "\nv_ampl       [V]    " << v_ampl_SC          << std::endl;
                  	std::cout << "\nv_th         [V]    " << v_th_SC            << std::endl;
                 	std::cout << "\ntime         [ns]   " << time_SC[chn]       << std::endl;
					std::cout << "\n------------------------------------------" << std::endl;
					std::cout << "time_sum_SC    [ns]   " << time_sum_SC        << std::endl;
					std::cout << "time_mean_SC   [ns]   " << time_mean_SC       << std::endl;
					std::cout << "*********************************************" << std::endl;
				}
			} // END if (status[chn]==1)	
		} // END LOOP ON CHANNELS OF WAVEDREAM

		if (time_TW > 0. && time_mean_SC > 0. && delta_CLK_X > 0. && delta_CLK_Y > 0.)
		{
			time_TOF = time_TW - time_mean_SC;
			hist_TOF->Fill(time_TOF);

			hist_charge_TOF_X->Fill(time_TOF, Q_9_X[jentry]);
			hist_charge_TOF_Y->Fill(time_TOF, Q_9_Y[jentry]);

			if (1)
			{
				std::cout << "*************************************************************"     << std::endl;
				std::cout << "ENTRY                   " << jentry << endl;
				std::cout << "-------------------------------------------------------------\n"   << std::endl;
				std::cout << "CLOCK" << endl;
				std::cout << "delta_CLK_X [ns]        " << delta_CLK_X << endl;
				std::cout << "delta_CLK_Y [ns]        " << delta_CLK_Y << endl;
				std::cout << "-------------------------------------------------------------\n"   << std::endl;
				std::cout << "START COUNTER" << endl;
				std::cout << "time_mean_SC [ns]       " << time_mean_SC << endl; 
				std::cout << "-------------------------------------------------------------\n"   << std::endl;
				std::cout << "TOFWALL" << endl;
				std::cout << "Mean_Time_Bar_9_X [ns]  " << Mean_Time_Bar_9_X << endl;
				std::cout << "Mean_Time_Bar_9_Y [ns]  " << Mean_Time_Bar_9_Y << endl;
				std::cout << "time_TW [ns]            " << time_TW << endl;
				std::cout << "\n-------------------------------------------------------------\n" << std::endl;
				std::cout << "time_TOF [ns]           " << time_TW << " - " << time_mean_SC << " = " << time_TOF << std::endl;
			}
		}        	
		//} // END if (tcb17_trigger_type == 1)
   	} // END LOOP ON ENTRIES

	//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
	// VISUALIZATION
	//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++

   	//------------------------------
   	// HIST TOF
   	//------------------------------
   	TCanvas *c_TOF = new TCanvas("c_TOF", "c_TOF", 1200, 1200);
   	c_TOF->SetTickx();
   	c_TOF->SetTicky();
   	c_TOF->SetLeftMargin(0.15);
   	hist_TOF->GetXaxis()->SetTitle("TOF = T_{TW} - T_{SC} [ns]");
   	hist_TOF->GetYaxis()->SetTitle("Counts");
   	hist_TOF->Fit("gaus", "Q");
   	hist_TOF->Draw();
	//c_TOF->SaveAs("figures/c_TOF.pdf");

   	//------------------------------
   	// CHARGHE BAR WD166
   	//------------------------------
   	for (int b=0; b<6; b++) 
	{  
    	TString canvas_title_q_bar_166 = Form("c_q_bar_166%d", b); 
      	TCanvas *c_q_bar_166 = new TCanvas(canvas_title_q_bar_166, canvas_title_q_bar_166, 600, 600);
		c_q_bar_166->SetTickx();
		c_q_bar_166->SetTicky();
		c_q_bar_166->SetLeftMargin(0.15);
		c_q_bar_166->SetLogy();
		hist_q_bar_166[b]->SetMarkerStyle(kFullCircle);
		hist_q_bar_166[b]->SetMarkerColor(kBlack);
		hist_q_bar_166[b]->SetNdivisions(20,"X");
		hist_q_bar_166[b]->Draw();
		//c_q_bar_166->SaveAs("figures/"+canvas_title_q_bar_166+".pdf");
	}

	//------------------------------
   	// TOF vs Q (X-VIEW)
   	//------------------------------	
    TCanvas *c_q_vs_tof_X = new TCanvas("c_q_vs_TOF_X", "c_q_vs_TOF_X", 600, 600);
    c_q_vs_tof_X->SetTickx();
	c_q_vs_tof_X->SetTicky();
	c_q_vs_tof_X->SetLogz();
	hist_charge_TOF_X->GetXaxis()->SetTitle("TOF = T_{TW} - T_{SC} [ns]");
	hist_charge_TOF_X->GetYaxis()->SetTitle("Q_{bar9,X} [a.u.]");
	hist_charge_TOF_X->Draw();
	hist_charge_TOF_X->Draw("colz");
	//c_q_vs_t_166->SaveAs("figures/"+canvas_title_q_vs_t_166+".pdf");

	//------------------------------
   	// TOF vs Q (Y-VIEW)
   	//------------------------------	
    TCanvas *c_q_vs_tof_Y = new TCanvas("c_q_vs_TOF_Y", "c_q_vs_TOF_Y", 600, 600);
    c_q_vs_tof_Y->SetTickx();
	c_q_vs_tof_Y->SetTicky();
	c_q_vs_tof_Y->SetLogz();
	hist_charge_TOF_Y->GetXaxis()->SetTitle("TOF = T_{TW} - T_{SC} [ns]");
	hist_charge_TOF_Y->GetYaxis()->SetTitle("Q_{bar9,Y} [a.u.]");
	hist_charge_TOF_Y->Draw();
	hist_charge_TOF_Y->Draw("colz");
	//c_q_vs_t_166->SaveAs("figures/"+canvas_title_q_vs_t_166+".pdf");   	

}
