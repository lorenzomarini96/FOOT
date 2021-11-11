//------------------------------------------------------------------------
// MACRO COMPUTING, FOR EACH WAVEDREAM:
//
// - THE TIME DISTRIBUTION OF THE WAVEFORM
// - THE CHARGE DISTRIBUTION
// - THE DELTA TIME DISTRIBUTION
//------------------------------------------------------------------------

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

   //------------------------------
   // FILE .ROOT TO SAVE HISTOGRAMS
   //------------------------------
   //TFile hfile("time_resolution1.root","RECREATE");

   //gStyle->SetOptStat(0);
   gStyle->SetOptFit(10111);

   //**************************************
   // HISTOGRAM
   //**************************************
   
   //TH2D *hist = new TH2D("hist", "Hit-map Test Cosmic rays", 20, 0, 20, 20, 20, 40);
   TH2D *hist = new TH2D("hist", "Hit-map Test Cosmic rays", 20, -0.5, 19.5, 20, 19.5, 39.5);

   TH1F *hist_mean_time = new TH1F("hist_mean_time", "Delta mean time barX/Y", 60, -150, 150);
   //------------------------------
   // HISTOGRAMS DELTA TIME WD165
   //------------------------------
   TH1F *h_delta_time_165[16];
   char name_delta_time_165[20];
   char title_delta_time_165[100];

   for (Int_t b=0; b<8; b++) {
      sprintf(name_delta_time_165,"h_delta_time_165%d", b);
      sprintf(title_delta_time_165,"WD165 - Delta T WD165 of bar%d", b);
      h_delta_time_165[b] = new TH1F(name_delta_time_165,title_delta_time_165,60, -150, 150);
      h_delta_time_165[b]->GetXaxis()->SetTitle("Delta T [a.u.]");
      h_delta_time_165[b]->GetYaxis()->SetTitle("Entries");
   }

   // LOOP ON ENTRIES
   for (Long64_t jentry=0; jentry<nentries; jentry++) {
      Long64_t ientry = LoadTree(jentry);
      if (ientry < 0) break;
      nb = fChain->GetEntry(jentry);   nbytes += nb;
      
      std::cout << "\n**************************\n" << std::endl;
      std::cout << "Entry " << jentry << endl;
      std::cout << "\n**************************\n" << std::endl;
      
      // INIT VALUES
      Int_t bar_TOF_X = -1;         // NUMBER OF THE BAR (X-VIEW)
      Int_t bar_TOF_Y = -1;         // NUMBER OF THE BAR (Y-VIEW)
      Double_t f_CFD = 0.3;         // FRACTION FOR COMPUTE THE TIME
      Double_t mean_time_X = -1.;   // MEAN TIME (LEFT AND RIGHT) ON THE BAR (X-VIEW)
      Double_t mean_time_Y = -1.;   // MEAN TIME (LEFT AND RIGHT) ON THE BAR (Y-VIEW)

      //********************************************************************
      // WAVEDREAM 165 - X VIEW (BAR 0 TO 7)   
      
      // LOOP ON CHANNELS OF WAVEDREAM
      for (Int_t ch=0; ch<16; ch++) {
         
         if (board165_hit[ch] == 1) {

            // INITIALIZE VALUES
            Double_t voltage_165 = 0.;                         
            Double_t v_base_165 = 0.;                          // V BASELINE (PEDESTAL) [mV]
            Double_t v_ampl_165[16];                           // V AMPLITUDE OF CHANNEL [mV]
            Double_t v_peak_165 = board165_waveform[ch][300];  // V MINIMUM (PEAK) [mV]
            Double_t v_th_165;                                 // V THRESHOLD [mV]
            Double_t time_165[16];                             // TIME OF WF OF CHANNEL [a.u.]
            Double_t delta_time_165[8];                        // ∆ TIME OF WF OF CHANNEL L AND R [a.u.]
            Double_t mean_time_165[8];                         // MEAN TIME OF L AND R CHANNELS [a.u.]
            Double_t q_165[16];                                // CHARGE OF CHANNEL [a.u.]
            Double_t q_bar_165[8];                             // CHARGE OF CHARGE [a.u.]
            
            // V PEAK (MINIMUM)
            for (Int_t t=10; t<800; t++) {// LOOP ON SAMPLES OF WAVEFORM
               if (t > 50 && board165_waveform[ch][t] < v_peak_165) v_peak_165 = board165_waveform[ch][t];
               // APPROX FIRST 1/3 OF 1024 SAMPLES
               if (t <= 150) voltage_165 =  voltage_165 + board165_waveform[ch][t];
            }
 
            v_base_165 = voltage_165/140;                              // V BASELINE
            v_th_165 = v_base_165 - f_CFD * (v_base_165 - v_peak_165); // V THRESHOLD
            v_ampl_165[ch] = v_base_165 - v_peak_165;                  // V AMPLITUDE

            // TIME OF CHANNEL
            for (Int_t t=10; t<900; t++) {
               if (board165_waveform[ch][t] == v_th_165) time_165[ch] = t;
               else if (board165_waveform[ch][t] < v_th_165) time_165[ch] = (t+(t-1))/2;  // time_165[ch] = time when the WF crosses Vth, evaluated through a linear interpolation of the two points nearest to the threshold.
            }

            for (Int_t t=250; t<570; t++) {      
               q_165[ch] += (v_base_165 - board165_waveform[ch][t]);     // CHARGHE OF CHANNEL
            }
   
            if (ch%2!=0) {
               q_bar_165[ch/2] = sqrt(q_165[ch] * q_165[ch-1]);          // CHARGHE OF BAR
               //h_q_tot[ch/2]->Fill(q_bar[ch/2]);
            }

            if (ch%2!=0) {
               mean_time_165[ch/2] = (time_165[ch] + time_165[ch-1])/2;  // MEAN TIME OF BAR
               mean_time_X = (time_165[ch] + time_165[ch-1])/2;
               //h_q_tot[ch/2]->Fill(q_bar[ch/2]);
            }

            if (ch%2!=0) {
               delta_time_165[ch/2] = time_165[ch] - time_165[ch-1]; // DELTA TIME OF BAR
               h_delta_time_165[ch/2]->Fill(delta_time_165[ch/2]);
            }

            // BARS X TOFWALL
            if (ch==0)       bar_TOF_X = 0;
            else if (ch==1)  bar_TOF_X = 0;
            else if (ch==2)  bar_TOF_X = 1;
            else if (ch==3)  bar_TOF_X = 1;
            else if (ch==4)  bar_TOF_X = 2;
            else if (ch==5)  bar_TOF_X = 2;
            else if (ch==6)  bar_TOF_X = 3;
            else if (ch==7)  bar_TOF_X = 3;
            else if (ch==6)  bar_TOF_X = 3;
            else if (ch==8)  bar_TOF_X = 4;
            else if (ch==9)  bar_TOF_X = 4;
            else if (ch==10) bar_TOF_X = 5;
            else if (ch==11) bar_TOF_X = 5;
            else if (ch==12) bar_TOF_X = 6;
            else if (ch==13) bar_TOF_X = 6;
            else if (ch==14) bar_TOF_X = 7;
            else if (ch==15) bar_TOF_X = 7;
            
            std::cout << "\nWD 165"                               << std::endl;
            std::cout << "-----------------------------"          << std::endl;
            std::cout << "Ch                = " << ch             << std::endl;
            std::cout << "-----------------------------"          << std::endl;
            std::cout << "\nv_base_165      = " << v_base_165     << std::endl;
            std::cout << "\nv_peak_165      = " << v_peak_165     << std::endl;
            std::cout << "\nv_th_165        = " << v_th_165       << std::endl;
            std::cout << "\ntime_165        = " << time_165[ch]   << std::endl;
            std::cout << "\nv_ampl_165      = " << v_ampl_165[ch] << std::endl;
            std::cout << "\nq_165           = " << q_165[ch]      << std::endl;
            std::cout << "\nBar (X)         = " << bar_TOF_X      << std::endl;
             if (ch%2!=0) {
                std::cout << "\n-----------------------------"              << std::endl;
                std::cout << "\ndelta_time_165  = " << delta_time_165[ch/2] << std::endl;
                std::cout << "\nq_bar_165       = " << q_bar_165[ch/2]      << std::endl;
                std::cout << "\nmean_time_165   = " << mean_time_165[ch/2]  << std::endl;
                std::cout << "\n-----------------------------"              << std::endl;
             }           
         } // END if (board165_hit[ch] == 1)
      } // END LOOP ON CHANNELS

      //********************************************************************
      // WAVEDREAM 166 - X VIEW (BAR 8 TO 10); Y VIEW (BAR 28 to 30)
      //********************************************************************

      // LOOP ON CHANNELS OF WAVEDREAM
      for (Int_t ch=0; ch<16; ch++) {
         
         if (board166_hit[ch] == 1) {

            // INITIALIZE VALUES
            Double_t voltage_166 = 0.;                         
            Double_t v_base_166 = 0.;                          // V BASELINE (PEDESTAL) [mV]
            Double_t v_ampl_166[16];                           // V AMPLITUDE OF CHANNEL [mV]
            Double_t v_peak_166 = board166_waveform[ch][300];  // V MINIMUM (PEAK) [mV]
            Double_t v_th_166;                                 // V THRESHOLD [mV]
            Double_t time_166[16];                             // TIME OF WF OF CHANNEL [a.u.]
            Double_t delta_time_166[8];                        // ∆ TIME OF WF OF CHANNEL L AND R [a.u.]
            Double_t mean_time_166[8];                         // MEAN TIME OF L AND R CHANNELS [a.u.]
            Double_t q_166[16];                                // CHARGE OF CHANNEL [a.u.]
            Double_t q_bar_166[8];                             // CHARGE OF CHARGE [a.u.]
            
            // V PEAK (MINIMUM)
            for (Int_t t=10; t<800; t++) {// LOOP ON SAMPLES OF WAVEFORM
               if (t > 50 && board166_waveform[ch][t] < v_peak_166) v_peak_166 = board166_waveform[ch][t];
               // APPROX FIRST 1/3 OF 1024 SAMPLES
               if (t <= 150) voltage_166 =  voltage_166 + board166_waveform[ch][t];
            }
 
            v_base_166 = voltage_166/140;                              // V BASELINE
            v_th_166 = v_base_166 - f_CFD * (v_base_166 - v_peak_166); // V THRESHOLD
            v_ampl_166[ch] = v_base_166 - v_peak_166;                  // V AMPLITUDE

            // TIME OF CHANNEL
            for (Int_t t=10; t<900; t++) {
               if (board166_waveform[ch][t] == v_th_166) time_166[ch] = t;
               else if (board166_waveform[ch][t] < v_th_166) time_166[ch] = (t+(t-1))/2;  // time_165[ch] = time when the WF crosses Vth, evaluated through a linear interpolation of the two points nearest to the threshold.
            }

            // CHARGHE OF CHANNEL
            for (Int_t t=250; t<570; t++) {      
               q_166[ch] += (v_base_166 - board166_waveform[ch][t]);
            }
   
            // CHARGHE OF BAR
            if (ch%2!=0) {
               q_bar_166[ch/2] = sqrt(q_166[ch] * q_166[ch-1]); 
               //h_q_tot[ch/2]->Fill(q_bar[ch/2]);
            }

            // MEAN TIME OF BAR
            if (ch%2!=0) {
               //mean_time_166[ch/2] = (time_166[ch] + time_166[ch-1])/2;
               if (ch<6) mean_time_X = (time_166[ch] + time_166[ch-1])/2;
               else mean_time_Y = (time_166[ch] + time_166[ch-1])/2;
               //h_q_tot[ch/2]->Fill(q_bar[ch/2]);
            }

            // DELTA TIME OF BAR
            if (ch%2!=0) {
               delta_time_166[ch/2] = time_166[ch] - time_166[ch-1]; // QUALCOSA NO VA...
               //h_delta_time[ch/2]->Fill(q_bar[ch/2]);
            }

            // BARS X TOFWALL
            if (ch==0)       bar_TOF_X = 8;
            else if (ch==1)  bar_TOF_X = 8;
            else if (ch==2)  bar_TOF_X = 9;
            else if (ch==3)  bar_TOF_X = 9;
            else if (ch==4)  bar_TOF_X = 10;
            else if (ch==5)  bar_TOF_X = 10;

            else if (ch==6)  bar_TOF_Y = 28;
            else if (ch==7)  bar_TOF_Y = 28;
            else if (ch==8)  bar_TOF_Y = 29;
            else if (ch==9)  bar_TOF_Y = 29;
            else if (ch==10) bar_TOF_Y = 30;
            else if (ch==11) bar_TOF_Y = 30;
   
            std::cout << "\nWD 166"                               << std::endl;
            std::cout << "-----------------------------"          << std::endl;
            std::cout << "Ch                = " << ch             << std::endl;
            std::cout << "-----------------------------"          << std::endl;
            std::cout << "\nv_base_166      = " << v_base_166     << std::endl;
            std::cout << "\nv_peak_166      = " << v_peak_166     << std::endl;
            std::cout << "\nv_th_166        = " << v_th_166       << std::endl;
            std::cout << "\ntime_166        = " << time_166[ch]   << std::endl;
            std::cout << "\nv_ampl_166      = " << v_ampl_166[ch] << std::endl;
            std::cout << "\nq_166           = " << q_166[ch]      << std::endl;
            std::cout << "\nBar (X)         = " << bar_TOF_X      << std::endl;
             if (ch%2!=0) {
                std::cout << "\n-----------------------------"              << std::endl;
                std::cout << "\ndelta_time_166  = " << delta_time_166[ch/2] << std::endl;
                std::cout << "\nq_bar_166       = " << q_bar_166[ch/2]      << std::endl;
                std::cout << "\nmean_time_166   = " << mean_time_166[ch/2]  << std::endl;
                std::cout << "\n-----------------------------"              << std::endl;
            }                    
         } // END if (board166_hit[ch] == 1)
      } // END LOOP ON CHANNEL

      //********************************************************************
      // WAVEDREAM 170 - X VIEW (BAR 11 TO 18)
      //********************************************************************

      // LOOP ON CHANNELS OF WAVEDREAM
      for (Int_t ch=0; ch<16; ch++) {
         
         if (board170_hit[ch] == 1) {

            // INITIALIZE VALUES
            Double_t voltage_170 = 0.;                         
            Double_t v_base_170 = 0.;                          // V BASELINE (PEDESTAL) [mV]
            Double_t v_ampl_170[16];                           // V AMPLITUDE OF CHANNEL [mV]
            Double_t v_peak_170 = board170_waveform[ch][300];  // V MINIMUM (PEAK) [mV]
            Double_t v_th_170;                                 // V THRESHOLD [mV]
            Double_t time_170[16];                             // TIME OF WF OF CHANNEL [a.u.]
            Double_t delta_time_170[8];                        // ∆ TIME OF WF OF CHANNEL L AND R [a.u.]
            Double_t mean_time_170[8];                         // MEAN TIME OF L AND R CHANNELS [a.u.]
            Double_t q_170[16];                                // CHARGE OF CHANNEL [a.u.]
            Double_t q_bar_170[8];                             // CHARGE OF CHARGE [a.u.]
            
            // V PEAK (MINIMUM)
            for (Int_t t=10; t<800; t++) {// LOOP ON SAMPLES OF WAVEFORM
               if (t > 50 && board170_waveform[ch][t] < v_peak_170) v_peak_170 = board170_waveform[ch][t];
               // APPROX FIRST 1/3 OF 1024 SAMPLES
               if (t <= 150) voltage_170 =  voltage_170 + board170_waveform[ch][t];
            }
 
            v_base_170 = voltage_170/140;                              // V BASELINE
            v_th_170 = v_base_170 - f_CFD * (v_base_170 - v_peak_170); // V THRESHOLD
            v_ampl_170[ch] = v_base_170 - v_peak_170;                  // V AMPLITUDE

            // TIME OF CHANNEL
            for (Int_t t=10; t<900; t++) {
               if (board170_waveform[ch][t] == v_th_170) time_170[ch] = t;
               else if (board170_waveform[ch][t] < v_th_170) time_170[ch] = (t+(t-1))/2;  // time_170[ch] = time when the WF crosses Vth, evaluated through a linear interpolation of the two points nearest to the threshold.
            }

            for (Int_t t=250; t<570; t++) {      
               q_170[ch] += (v_base_170 - board170_waveform[ch][t]);     // CHARGHE OF CHANNEL
            }
   
            if (ch%2!=0) {
               q_bar_170[ch/2] = sqrt(q_170[ch] * q_170[ch-1]);          // CHARGHE OF BAR
               //h_q_tot[ch/2]->Fill(q_bar[ch/2]);
            }

            if (ch%2!=0) {
               //mean_time_170[ch/2] = (time_170[ch] + time_170[ch-1])/2;  // MEAN TIME OF BAR
               if (ch<3) mean_time_X = (time_170[ch] + time_170[ch-1])/2;
               else mean_time_Y = (time_170[ch] + time_170[ch-1])/2;
               //h_q_tot[ch/2]->Fill(q_bar[ch/2]);
            }

            // DELTA TIME OF BAR
            if (ch%2!=0) {
               delta_time_170[ch/2] = time_170[ch] - time_170[ch-1]; // QUALCOSA NO VA...
               //h_q_tot[ch/2]->Fill(q_bar[ch/2]);
            }
            
            // BARS X TOFWALL
            if (ch==0)       bar_TOF_X = 11;
            else if (ch==1)  bar_TOF_X = 11;
            else if (ch==2)  bar_TOF_X = 12;
            else if (ch==3)  bar_TOF_X = 12;
            else if (ch==4)  bar_TOF_X = 13;
            else if (ch==5)  bar_TOF_X = 13;
            else if (ch==6)  bar_TOF_X = 14;
            else if (ch==7)  bar_TOF_X = 14;
            else if (ch==8)  bar_TOF_X = 15;
            else if (ch==9)  bar_TOF_X = 15;
            else if (ch==10) bar_TOF_X = 16;
            else if (ch==11) bar_TOF_X = 16;
            else if (ch==12) bar_TOF_X = 17;
            else if (ch==13) bar_TOF_X = 17;
            else if (ch==14) bar_TOF_X = 18;
            else if (ch==15) bar_TOF_X = 18;

            std::cout << "\nWD 170"                               << std::endl;
            std::cout << "-----------------------------"          << std::endl;
            std::cout << "Ch                = " << ch             << std::endl;
            std::cout << "-----------------------------"          << std::endl;
            std::cout << "\nv_base_170      = " << v_base_170     << std::endl;
            std::cout << "\nv_peak_170      = " << v_peak_170     << std::endl;
            std::cout << "\nv_th_170        = " << v_th_170       << std::endl;
            std::cout << "\ntime_170        = " << time_170[ch]   << std::endl;
            std::cout << "\nv_ampl_170      = " << v_ampl_170[ch] << std::endl;
            std::cout << "\nq_170           = " << q_170[ch]      << std::endl;
            std::cout << "\nBar (X)         = " << bar_TOF_X      << std::endl;
             if (ch%2!=0) {
                std::cout << "\n-----------------------------"              << std::endl;
                std::cout << "\ndelta_time_170  = " << delta_time_170[ch/2] << std::endl;
                std::cout << "\nq_bar_170       = " << q_bar_170[ch/2]      << std::endl;
                std::cout << "\nmean_time_170   = " << mean_time_170[ch/2]  << std::endl;
                std::cout << "\n-----------------------------"              << std::endl;
            }
         } // END if (board170_hit[ch] == 1)
      } // END LOOP ON CHANNEL

      //********************************************************************
      // WAVEDREAM 167 - X VIEW (BAR 19); Y VIEW (BAR 20 TO 26)
      //********************************************************************

      // LOOP ON CHANNELS OF WAVEDREAM
      for (Int_t ch=0; ch<16; ch++) {
         
         if (board167_hit[ch] == 1) {

            // INITIALIZE VALUES
            Double_t voltage_167 = 0.;                         
            Double_t v_base_167 = 0.;                          // V BASELINE (PEDESTAL) [mV]
            Double_t v_ampl_167[16];                           // V AMPLITUDE OF CHANNEL [mV]
            Double_t v_peak_167 = board167_waveform[ch][300];  // V MINIMUM (PEAK) [mV]
            Double_t v_th_167;                                 // V THRESHOLD [mV]
            Double_t time_167[16];                             // TIME OF WF OF CHANNEL [a.u.]
            Double_t delta_time_167[8];                        // ∆ TIME OF WF OF CHANNEL L AND R [a.u.]
            Double_t mean_time_167[8];                         // MEAN TIME OF L AND R CHANNELS [a.u.]
            Double_t q_167[16];                                // CHARGE OF CHANNEL [a.u.]
            Double_t q_bar_167[8];                             // CHARGE OF CHARGE [a.u.]
            
            // V PEAK (MINIMUM)
            for (Int_t t=10; t<800; t++) {// LOOP ON SAMPLES OF WAVEFORM
               if (t > 50 && board167_waveform[ch][t] < v_peak_167) v_peak_167 = board167_waveform[ch][t];
               // APPROX FIRST 1/3 OF 1024 SAMPLES
               if (t <= 150) voltage_167 =  voltage_167 + board167_waveform[ch][t];
            }
 
            v_base_167 = voltage_167/140;                              // V BASELINE
            v_th_167 = v_base_167 - f_CFD * (v_base_167 - v_peak_167); // V THRESHOLD
            v_ampl_167[ch] = v_base_167 - v_peak_167;                  // V AMPLITUDE

            // TIME OF CHANNEL
            for (Int_t t=10; t<900; t++) {
               if (board167_waveform[ch][t] == v_th_167) time_167[ch] = t;
               else if (board167_waveform[ch][t] < v_th_167) time_167[ch] = (t+(t-1))/2;  // time_170[ch] = time when the WF crosses Vth, evaluated through a linear interpolation of the two points nearest to the threshold.
            }

            for (Int_t t=250; t<570; t++) {      
               q_167[ch] += (v_base_167 - board167_waveform[ch][t]);     // CHARGHE OF CHANNEL
            }
   
            if (ch%2!=0) {
               q_bar_167[ch/2] = sqrt(q_167[ch] * q_167[ch-1]);          // CHARGHE OF BAR
               //h_q_tot[ch/2]->Fill(q_bar[ch/2]);
            }

            if (ch%2!=0) {
               mean_time_167[ch/2] = (time_167[ch] + time_167[ch-1])/2;  // MEAN TIME OF BAR
               //h_q_tot[ch/2]->Fill(q_bar[ch/2]);
            }

            // DELTA TIME OF BAR
            if (ch%2!=0) {
               //delta_time_167[ch/2] = time_167[ch] - time_167[ch-1]; // QUALCOSA NO VA...
               if (ch<3) mean_time_X = (time_167[ch] + time_167[ch-1])/2;
               else mean_time_Y = (time_167[ch] + time_167[ch-1])/2;
               //h_q_tot[ch/2]->Fill(q_bar[ch/2]);
            }
            
            // BARS X TOFWALL
            if (ch==0)       bar_TOF_X = 19;
            else if (ch==1)  bar_TOF_X = 19;

            // BARS Y TOFWALL
            else if (ch==2)  bar_TOF_Y = 20;
            else if (ch==3)  bar_TOF_Y = 20;
            else if (ch==4)  bar_TOF_Y = 21;
            else if (ch==5)  bar_TOF_Y = 21;
            else if (ch==6)  bar_TOF_Y = 22;
            else if (ch==7)  bar_TOF_Y = 22;
            else if (ch==8)  bar_TOF_Y = 23;
            else if (ch==9)  bar_TOF_Y = 23;
            else if (ch==10) bar_TOF_Y = 24;
            else if (ch==11) bar_TOF_Y = 24;
            else if (ch==12) bar_TOF_Y = 25;
            else if (ch==13) bar_TOF_Y = 25;
            else if (ch==14) bar_TOF_Y = 26;
            else if (ch==15) bar_TOF_Y = 26;
            
            std::cout << "\nWD 167"                               << std::endl;
            std::cout << "-----------------------------"          << std::endl;
            std::cout << "Ch                = " << ch             << std::endl;
            std::cout << "-----------------------------"          << std::endl;
            std::cout << "\nv_base_167      = " << v_base_167     << std::endl;
            std::cout << "\nv_peak_167      = " << v_peak_167     << std::endl;
            std::cout << "\nv_th_167        = " << v_th_167       << std::endl;
            std::cout << "\ntime_167        = " << time_167[ch]   << std::endl;
            std::cout << "\nv_ampl_167      = " << v_ampl_167[ch] << std::endl;
            std::cout << "\nq_167           = " << q_167[ch]      << std::endl;
            if (ch==0 || ch==1) std::cout << "\nBar (X)         = " << bar_TOF_X      << std::endl;
            else std::cout << "\nBar (Y)         = " << bar_TOF_Y      << std::endl;
            if (ch%2!=0) {
               std::cout << "\n-----------------------------"              << std::endl;
               std::cout << "\ndelta_time_167  = " << delta_time_167[ch/2] << std::endl;
               std::cout << "\nq_bar_167       = " << q_bar_167[ch/2]      << std::endl;
               std::cout << "\nmean_time_167   = " << mean_time_167[ch/2]  << std::endl;
               std::cout << "\n-----------------------------"              << std::endl;
            }
         } // END if (board167_hit[ch] == 1)
      } // END LOOP ON CHANNEL

      //********************************************************************
      // WAVEDREAM 168 - Y VIEW (BAR 27 ANS 31 TO 37)
      //********************************************************************

      for (Int_t ch=0; ch<16; ch++){

         if (board168_hit[ch] == 1) {

            // INITIALIZE VALUES
            Double_t voltage_168 = 0.;                         
            Double_t v_base_168 = 0.;                          // V BASELINE (PEDESTAL) [mV]
            Double_t v_ampl_168[16];                           // V AMPLITUDE OF CHANNEL [mV]
            Double_t v_peak_168 = board168_waveform[ch][300];  // V MINIMUM (PEAK) [mV]
            Double_t v_th_168;                                 // V THRESHOLD [mV]
            Double_t time_168[16];                             // TIME OF WF OF CHANNEL [a.u.]
            Double_t delta_time_168[8];                        // ∆ TIME OF WF OF CHANNEL L AND R [a.u.]
            Double_t mean_time_168[8];                         // MEAN TIME OF L AND R CHANNELS [a.u.]
            Double_t q_168[16];                                // CHARGE OF CHANNEL [a.u.]
            Double_t q_bar_168[8];                             // CHARGE OF CHARGE [a.u.]
            
            // V PEAK (MINIMUM)
            for (Int_t t=10; t<800; t++) {// LOOP ON SAMPLES OF WAVEFORM
               if (t > 50 && board168_waveform[ch][t] < v_peak_168) v_peak_168 = board168_waveform[ch][t];
               // APPROX FIRST 1/3 OF 1024 SAMPLES
               if (t <= 150) voltage_168 =  voltage_168 + board168_waveform[ch][t];
            }
 
            v_base_168 = voltage_168/140;                              // V BASELINE
            v_th_168 = v_base_168 - f_CFD * (v_base_168 - v_peak_168); // V THRESHOLD
            v_ampl_168[ch] = v_base_168 - v_peak_168;                  // V AMPLITUDE

            // TIME OF CHANNEL
            for (Int_t t=10; t<900; t++) {
               if (board168_waveform[ch][t] == v_th_168) time_168[ch] = t;
               else if (board168_waveform[ch][t] < v_th_168) time_168[ch] = (t+(t-1))/2;  // time_170[ch] = time when the WF crosses Vth, evaluated through a linear interpolation of the two points nearest to the threshold.
            }

            for (Int_t t=250; t<570; t++) {      
               q_168[ch] += (v_base_168 - board168_waveform[ch][t]);     // CHARGHE OF CHANNEL
            }
   
            if (ch%2!=0) {
               q_bar_168[ch/2] = sqrt(q_168[ch] * q_168[ch-1]);          // CHARGHE OF BAR
               //h_q_tot[ch/2]->Fill(q_bar[ch/2]);
            }

            if (ch%2!=0) {
               //mean_time_168[ch/2] = (time_168[ch] + time_168[ch-1])/2;  // MEAN TIME OF BAR
               mean_time_Y = (time_168[ch] + time_168[ch-1])/2;
               //h_q_tot[ch/2]->Fill(q_bar[ch/2]);
            }

            // DELTA TIME OF BAR
            if (ch%2!=0) {
               delta_time_168[ch/2] = time_168[ch] - time_168[ch-1]; // QUALCOSA NO VA...
               //h_q_tot[ch/2]->Fill(q_bar[ch/2]);
            }
            
            // BARS Y TOFWALL
            if (ch==0)       bar_TOF_Y = 27;
            else if (ch==1)  bar_TOF_Y = 27;
            else if (ch==2)  bar_TOF_Y = 31;
            else if (ch==3)  bar_TOF_Y = 31;
            else if (ch==4)  bar_TOF_Y = 32;
            else if (ch==5)  bar_TOF_Y = 32;
            else if (ch==6)  bar_TOF_Y = 33;
            else if (ch==7)  bar_TOF_Y = 33;
            else if (ch==8)  bar_TOF_Y = 34;
            else if (ch==9)  bar_TOF_Y = 34;
            else if (ch==10) bar_TOF_Y = 35;
            else if (ch==11) bar_TOF_Y = 35;
            else if (ch==12) bar_TOF_Y = 36;
            else if (ch==13) bar_TOF_Y = 36;
            else if (ch==14) bar_TOF_Y = 37;
            else if (ch==15) bar_TOF_Y = 37;

            std::cout << "\nWD 168"                               << std::endl;
            std::cout << "-----------------------------"          << std::endl;
            std::cout << "Ch                = " << ch             << std::endl;
            std::cout << "-----------------------------"          << std::endl;
            std::cout << "\nv_base_168      = " << v_base_168     << std::endl;
            std::cout << "\nv_peak_168      = " << v_peak_168     << std::endl;
            std::cout << "\nv_th_168        = " << v_th_168       << std::endl;
            std::cout << "\ntime_168        = " << time_168[ch]   << std::endl;
            std::cout << "\nv_ampl_168      = " << v_ampl_168[ch] << std::endl;
            std::cout << "\nq_168           = " << q_168[ch]      << std::endl;
            std::cout << "\nBar (Y)         = " << bar_TOF_Y      << std::endl;
            if (ch%2!=0) {
               std::cout << "\n-----------------------------"              << std::endl;
               std::cout << "\ndelta_time_168  = " << delta_time_168[ch/2] << std::endl;
               std::cout << "\nq_bar_168       = " << q_bar_168[ch/2]      << std::endl;
               std::cout << "\nmean_time_168   = " << mean_time_168[ch/2]  << std::endl;
               std::cout << "\n-----------------------------"              << std::endl;
            }
         } // END if (board168_hit[ch] == 1)
      } // END LOOP ON CHANNEL

      //********************************************************************
      // WAVEDREAM 158 - Y VIEW (BAR 38 TO 39)
      //********************************************************************

      for (Int_t ch=0; ch<4; ch++) {
         
         if (board158_hit[ch] == 1) {

            // INITIALIZE VALUES
            Double_t voltage_158 = 0.;                         
            Double_t v_base_158 = 0.;                          // V BASELINE (PEDESTAL) [mV]
            Double_t v_ampl_158[16];                           // V AMPLITUDE OF CHANNEL [mV]
            Double_t v_peak_158 = board168_waveform[ch][300];  // V MINIMUM (PEAK) [mV]
            Double_t v_th_158;                                 // V THRESHOLD [mV]
            Double_t time_158[16];                             // TIME OF WF OF CHANNEL [a.u.]
            Double_t delta_time_158[8];                        // ∆ TIME OF WF OF CHANNEL L AND R [a.u.]
            Double_t mean_time_158[8];                         // MEAN TIME OF L AND R CHANNELS [a.u.]
            Double_t q_158[16];                                // CHARGE OF CHANNEL [a.u.]
            Double_t q_bar_158[8];                             // CHARGE OF CHARGE [a.u.]
            
            // V PEAK (MINIMUM)
            for (Int_t t=10; t<800; t++) {// LOOP ON SAMPLES OF WAVEFORM
               if (t > 50 && board158_waveform[ch][t] < v_peak_158) v_peak_158 = board158_waveform[ch][t];
               // APPROX FIRST 1/3 OF 1024 SAMPLES
               if (t <= 150) voltage_158 =  voltage_158 + board158_waveform[ch][t];
            }
 
            v_base_158 = voltage_158/140;                              // V BASELINE
            v_th_158 = v_base_158 - f_CFD * (v_base_158 - v_peak_158); // V THRESHOLD
            v_ampl_158[ch] = v_base_158 - v_peak_158;                  // V AMPLITUDE

            // TIME OF CHANNEL
            for (Int_t t=10; t<900; t++) {
               if (board158_waveform[ch][t] == v_th_158) time_158[ch] = t;
               else if (board158_waveform[ch][t] < v_th_158) time_158[ch] = (t+(t-1))/2;  // time_170[ch] = time when the WF crosses Vth, evaluated through a linear interpolation of the two points nearest to the threshold.
            }

            for (Int_t t=250; t<570; t++) {      
               q_158[ch] += (v_base_158 - board158_waveform[ch][t]);     // CHARGHE OF CHANNEL
            }
   
            if (ch%2!=0) {
               q_bar_158[ch/2] = sqrt(q_158[ch] * q_158[ch-1]);          // CHARGHE OF BAR
               //h_q_tot[ch/2]->Fill(q_bar[ch/2]);
            }

            if (ch%2!=0) {
               //mean_time_158[ch/2] = (time_158[ch] + time_158[ch-1])/2;  // MEAN TIME OF BAR
               mean_time_Y = (time_158[ch] + time_158[ch-1])/2;
               //h_q_tot[ch/2]->Fill(q_bar[ch/2]);
            }

            // DELTA TIME OF BAR
            if (ch%2!=0) {
               delta_time_158[ch/2] = time_158[ch] - time_158[ch-1]; // QUALCOSA NO VA...
               //h_q_tot[ch/2]->Fill(q_bar[ch/2]);
            }
                 
            // BARS Y TOFWALL
            if (ch==0) bar_TOF_Y = 38;
            else if (ch==1) bar_TOF_Y = 38;
            else if (ch==2) bar_TOF_Y = 39;
            else if (ch==3) bar_TOF_Y = 39;

            std::cout << "\nWD 158"                               << std::endl;
            std::cout << "-----------------------------"          << std::endl;
            std::cout << "Ch                = " << ch             << std::endl;
            std::cout << "-----------------------------"          << std::endl;
            std::cout << "\nv_base_158      = " << v_base_158     << std::endl;
            std::cout << "\nv_peak_158      = " << v_peak_158     << std::endl;
            std::cout << "\nv_th_158        = " << v_th_158       << std::endl;
            std::cout << "\ntime_158        = " << time_158[ch]   << std::endl;
            std::cout << "\nv_ampl_158      = " << v_ampl_158[ch] << std::endl;
            std::cout << "\nq_158           = " << q_158[ch]      << std::endl;
            std::cout << "\nBar (Y)         = " << bar_TOF_Y      << std::endl;
            if (ch%2!=0) {
               std::cout << "\n-----------------------------"              << std::endl;
               std::cout << "\ndelta_tiME_158  = " << delta_time_158[ch/2] << std::endl;
               std::cout << "\nq_bar_158       = " << q_bar_158[ch/2]      << std::endl;
               std::cout << "\nmean_time_158   = " << mean_time_158[ch/2]  << std::endl;
               std::cout << "\n-----------------------------"              << std::endl;
            }
         }  // END (board158_hit[ch] == 1)
      } // END LOOP ON CHANNEL

      // FILL THE HISTOGRAM
      if (bar_TOF_X > -1 && bar_TOF_Y > -1)      hist->Fill(bar_TOF_X, bar_TOF_Y);
      if (mean_time_X > -1 && mean_time_Y > -1)  hist_mean_time->Fill(mean_time_X - mean_time_Y);
      
         
   }// END LOOP ON ENTRIES
   
   //**************************************
   // CANVAS
   //**************************************
   TCanvas *c1 = new TCanvas("c1", "hitmap",600,600);
   c1->SetTickx();
   c1->SetTicky();
   c1->SetLeftMargin(0.15);

   hist->GetXaxis()->SetTitle("FronBar, LayerX");
   hist->SetNdivisions(20,"X");
   hist->GetYaxis()->SetTitle("RearBar, LayerY");
   hist->SetNdivisions(20,"Y");
   hist->GetZaxis()->SetTitle("# hits");
   hist->Draw("colz TEXT0 SAME");//https://root.cern.ch/doc/v608/classTHistPainter.html#HP15
   //hist->Draw("colz");
   
   //h_delta_time[0]->Draw();
      
   //**************************************
   // CANVAS
   //**************************************
   TCanvas *c_2 = new TCanvas("c_2", "Delta t",600,600);
   c_2->SetTickx();
   c_2->SetTicky();
   c_2->SetLeftMargin(0.15);

   for (int b=0; b<8; b++)
   {  
      //------------------------------
      // DELTA TIME
      //------------------------------
      TString canvas_title = Form("c_bar%d",b); 
      TCanvas *time = new TCanvas(canvas_title, canvas_title, 600, 600);
      time->SetTickx();
      time->SetTicky();
      time->SetLeftMargin(0.15);
      h_delta_time_165[b]->SetMarkerStyle(20);
      h_delta_time_165[b]->SetMarkerStyle(kFullCircle);
      h_delta_time_165[b]->SetMarkerColor(kBlack);
      h_delta_time_165[b]->Fit("gaus", "Q");
      h_delta_time_165[b]->Draw("E");

   }

   //**************************************
   // CANVAS
   //**************************************
   TCanvas *c3 = new TCanvas("c3", "delta_mean_time",600,600);
   c3->SetTickx();
   c3->SetTicky();
   c3->SetLeftMargin(0.15);

   hist_mean_time->GetXaxis()->SetTitle("Delta mean time");
   hist_mean_time->GetYaxis()->SetTitle("Counts");
   hist_mean_time->Draw();//https://root.cern.ch/doc/v608/classTHistPainter.html#HP15

   //------------------------------
   // SAVE HISROGRAMS IN ROOT FILE
   //------------------------------
   //hfile.Write();
}
