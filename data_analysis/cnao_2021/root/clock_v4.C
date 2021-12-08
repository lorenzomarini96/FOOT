//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// clock_v1.C																										    //
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


   	gStyle->SetOptFit(10111);   


  	// LOOP ON ENTRIES
  	//for (Long64_t jentry=0; jentry<nentries; jentry++)
	for (Long64_t jentry=0; jentry<1; jentry++)           // ONLY FIRST EVENT FOR THE MOMENT
   	{
    	Long64_t ientry = LoadTree(jentry);
      	if (ientry < 0) break;
      	nb = fChain->GetEntry(jentry);   nbytes += nb;
		
		// OBIETTIVO
		Double_t delta_CLK;

		//*********************************************************
		// TOF-WALL
		//*********************************************************
		Int_t n_points_TW = 0; // number of points for graphs of wf
		Int_t chn_TW_CLK = 16;	
		Double_t voltage_TW_CLK_NC[1023]; // No correction
		Double_t voltage_TW_CLK[1023];
		Double_t time_TW_CLK[1023];

		//*********************************************************
		// START COUNTER
		//*********************************************************
		Int_t n_points_SC = 0; // number of points for graphs of wf
      	Int_t chn_SC_CLK = 16;
		Double_t voltage_SC_CLK_NC[1023]; // No correction
		Double_t voltage_SC_CLK[1023];
		Double_t time_SC_CLK[1023];

		// CLOCK ANALYSIS
		Double_t min_SC_CLK;                  // V AMPLITUDE MIN OF CHANNEL [V]
		Double_t max_SC_CLK;                  // V AMPLITUDE MIN OF CHANNEL [V]
		Double_t a_fit_SC_CLK;
		Double_t sigma_a_fit_SC_CLK;
		Double_t b_fit_SC_CLK;
		Double_t sigma_b_fit_SC_CLK;
		Double_t phi_SC_CLK;		     
		Double_t zero_SC_CLK;                 // MEAN OF MAX AND MIN VALUES OF WF
		Double_t ZeroCrossingPoint[27];       // ZERO CROSSING POINT ON THE RISING EDGES OF WF [ns]
		Double_t sigma_ZeroCrossingPoint[27]; // ERROR ON ZERO CROSSING POINT [ns]
		Double_t N_SC_CLK[27];			      // NUMBER OF CLOCK CYCLES
		Double_t sigma_N_SC_CLK[27];	      // ERROR ON NUMBER OF CLOCK
		Int_t n_point_SC_phi = 27;
		
				
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
			
			
			cout << "\n----------------------------------------------------------------------------------------" << endl;
			cout << left<<setw(10)<<"i"<<left<<setw(20)<<"voltage_SC_CLK [V]"<<left<<setw(20)<<"time_SC_CLK [ns]"<<left<<setw(20)<<endl;
			cout << left<<setw(10)<<i<<left<<setw(20)<<voltage_SC_CLK[i]<<left<<setw(20)<<time_SC_CLK[i]<<left<<setw(20)<<endl;
			cout << "\n----------------------------------------------------------------------------------------" << endl;
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
		for (Int_t j=0; j<30; j++)
		{	
        	for (Int_t i=0; i<1000; i++) 
			{ 	
            	if (voltage_SC_CLK[i] > zero_SC_CLK)
				{
					TF1 *f_fit_SC_CLK = new TF1("f_fit_SC_CLK", "pol1", time_SC_CLK[i-2], time_SC_CLK[i+2]);
				
					Int_t n = 5;  // NUMBER OF POINTS FOR THE GRAPH
					Double_t v_data_SC_CLK[5] = {voltage_SC_CLK[i-2], voltage_SC_CLK[i-1], voltage_SC_CLK[i], voltage_SC_CLK[i+1], voltage_SC_CLK[i+2]};
					Double_t t_data_SC_CLK[5] = {time_SC_CLK[i-2], time_SC_CLK[i-1], time_SC_CLK[i], time_SC_CLK[i+1], time_SC_CLK[i+2]};
					
					if (0)
					{
						cout << "\nvoltage_SC_CLK["<<i-2<<"] = " <<voltage_SC_CLK[i-2] << endl;
						cout << "\nvoltage_SC_CLK["<<i-1<<"] = " << voltage_SC_CLK[i-1] << endl;
						cout << "\nvoltage_SC_CLK["<<i  <<"] = " << voltage_SC_CLK[i] << endl;
						cout << "\nvoltage_SC_CLK["<<i+1<<"] = " << voltage_SC_CLK[i+1] << endl;
						cout << "\nvoltage_SC_CLK["<<i+2<<"] = " << voltage_SC_CLK[i+2] << endl;

						cout << "\ntime_SC_CLK["<<i-2<<"] = " << time_SC_CLK[i-2] << endl;
						cout << "\ntime_SC_CLK["<<i-1<<"] = " << time_SC_CLK[i-1] << endl;
						cout << "\ntime_SC_CLK["<<i  <<"] = " << time_SC_CLK[i] << endl;
						cout << "\ntime_SC_CLK["<<i+1<<"] = " << time_SC_CLK[i+1] << endl;
						cout << "\ntime_SC_CLK["<<i+2<<"] = " << time_SC_CLK[i+2] << endl;
					}

					TGraph *gr_SC_CLK = new TGraph(n, t_data_SC_CLK, v_data_SC_CLK);

					gr_SC_CLK->Fit("f_fit_SC_CLK","Qr");			
					
					a_fit_SC_CLK = f_fit_SC_CLK->GetParameter(1);
					sigma_a_fit_SC_CLK = f_fit_SC_CLK->GetParError(1);
					b_fit_SC_CLK = f_fit_SC_CLK->GetParameter(0);
					sigma_b_fit_SC_CLK = f_fit_SC_CLK->GetParError(0);

					// ZERO CROSSING POINT
					ZeroCrossingPoint[j]       = (zero_SC_CLK - b_fit_SC_CLK)/a_fit_SC_CLK; // [ns]
					sigma_ZeroCrossingPoint[i] = ZeroCrossingPoint[j] * sqrt(pow(sigma_a_fit_SC_CLK/a_fit_SC_CLK,2) + pow(sigma_b_fit_SC_CLK/b_fit_SC_CLK,2)); // Somma in quadratura degli errori sui parametri di best-fit
					
					// NUMBER OF CLOCK CYCLES
					N_SC_CLK[j] = j+1;
					//sigma_N_SC_CLK[j] = 0.;

					delete f_fit_SC_CLK;
					delete gr_SC_CLK;
					
					i += 20;
					j +=1;
            	}
				if (N_SC_CLK[j] == 25) break;
        	}
		}
		
		// DATA CORRECTION (problem with the first element)
		Double_t new_ZeroCrossingPoint[25];
		Double_t new_N_SC_CLK[25];
		Int_t    new_n_point_SC_phi = n_point_SC_phi-2;

		for (Int_t i=0; i<25; i++)
		{	

			// SHIFT ALL ELEMENTS BY ONE POSITION
			new_N_SC_CLK[i] = N_SC_CLK[i];
			new_ZeroCrossingPoint[i] = ZeroCrossingPoint[i+1];

			cout << "\n***********************************************************" << endl;
			cout <<left<<setw(4)<< "*" <<left<<setw(10)<<"i"<<left<<setw(30)<<"ZeroCrossingPoint [ns]"<<left<<setw(30)<<"N_SC_CLK "<<left<<setw(4)<<"*"<<endl;
			cout <<left<<setw(4)<< "*" <<left<<setw(10)<<i<<left<<setw(30)<<new_ZeroCrossingPoint[i]<<left<<setw(30)<<new_N_SC_CLK[i]<<left<<setw(4)<<"*"<<endl;
			cout << "***********************************************************" << endl;
		}
		
		
		TGraph *gr_N_SC_CLK_vs_ZeroCrossingPoint = new TGraphErrors(new_n_point_SC_phi, new_N_SC_CLK, new_ZeroCrossingPoint);
		TF1 *f_fit_N_SC_CLK_vs_ZeroCrossingPoint = new TF1("f_fit_N_SC_CLK_vs_ZeroCrossingPoint", "pol1", new_N_SC_CLK[0], N_SC_CLK[24]);
		f_fit_N_SC_CLK_vs_ZeroCrossingPoint->SetParName(0, "#phi_{CLK,SC}");
		f_fit_N_SC_CLK_vs_ZeroCrossingPoint->SetParName(1, "T_{SC}");
		
		f_fit_N_SC_CLK_vs_ZeroCrossingPoint->SetLineStyle(2); // 2 = --
		gr_N_SC_CLK_vs_ZeroCrossingPoint->Fit("f_fit_N_SC_CLK_vs_ZeroCrossingPoint","Qr");
		
			
		//*********************************************************
		// TOF-WALL
		//*********************************************************

		// 1) CORRECTION OF WF
		enablesum = 0;

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
		
		// START COUNTER
		TCanvas *c4 = new TCanvas("c4"," ", 800,800);
		gr_N_SC_CLK_vs_ZeroCrossingPoint->GetXaxis()->SetTitle("Number of Cycles");
		gr_N_SC_CLK_vs_ZeroCrossingPoint->GetYaxis()->SetTitle("Zero-Crossing Time [ns]");
		gr_N_SC_CLK_vs_ZeroCrossingPoint->SetTitle(" ");
		gr_N_SC_CLK_vs_ZeroCrossingPoint->SetMarkerStyle(20);
		c4->SetLeftMargin(0.15);
		c4->SetTickx();
   		c4->SetTicky();

		gr_N_SC_CLK_vs_ZeroCrossingPoint->GetXaxis()->SetRangeUser(0, 30);
		gr_N_SC_CLK_vs_ZeroCrossingPoint->Draw("AP");
		
		TLegend *legend = new TLegend(0.5,0.5,0.8,0.8);
		Char_t message[80];
		//sprintf(message,"#splitline{#chi^{2}/ndf = %.4f/%.0f}{#phi_{CLK,SC} = }", f_fit_N_SC_CLK_vs_ZeroCrossingPoint->GetChisquare(), f_fit_N_SC_CLK_vs_ZeroCrossingPoint->GetNDF());
		sprintf(message,"#chi^{2}/ndf = %.4f/%d", f_fit_N_SC_CLK_vs_ZeroCrossingPoint->GetChisquare(), f_fit_N_SC_CLK_vs_ZeroCrossingPoint->GetNDF());
		legend->AddEntry((TObject*)0, "Some text", "");
		//sprintf(message,"#phi_{CLK,SC} = %.2f +/- %.2f", f_fit_N_SC_CLK_vs_ZeroCrossingPoint->GetParameter(0), f_fit_N_SC_CLK_vs_ZeroCrossingPoint->GetParError(0));
		//sprintf(message,"T_{CLK,SC} = %.2f +/- %.2f"   , f_fit_N_SC_CLK_vs_ZeroCrossingPoint->GetParameter(1), f_fit_N_SC_CLK_vs_ZeroCrossingPoint->GetParError(1));
		legend->SetBorderSize(0);
		legend->AddEntry(gr_N_SC_CLK_vs_ZeroCrossingPoint, message);
		legend->Draw();

		//TLatex *text = new TLatex(10, 200, "testo..." );
		//text->SetTextSize(0.05);
		//tex->SetTextColor(2);
		//text->Draw();
			
	}

}
