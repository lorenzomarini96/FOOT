//---------------------------------------------------------------------------------------------
// MACRO COMPUTING FEATURES WAVEDREAM 165 - X VIEW (BAR 0 TO 7) AND SHOW THE DISTRIBUTION OF 
//
// - V AMPLITUDE FOR EACH CHANNEL
// - CHARGE FOR EACH CHANNEL
// - CHARGE FOR EACH BAR
// - TIME FOR EACH CHANNEL: time when the WF crosses Vth, evaluated through a 
//   linear interpolation of the two points nearest to the threshold.
// - DELTA TIME FOR EACH CHANNEL
//---------------------------------------------------------------------------------------------

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
   //TFile hfile("wd165.root","RECREATE");

   //gStyle->SetOptStat(0);
   gStyle->SetOptFit(10111);

   
   //==================================================================================
   // // WAVEDREAM 165 - X VIEW (BAR 0 TO 7)   
   //==================================================================================
   
   //------------------------------
   // HISTOGRAMS HITBARS 0 TO 7 WD165
   //------------------------------
   TH1D *hist_hit_165 = new TH1D("hist_hit_165", "Hit-map X-view bar 0 to 7", 8, -0.5, 7.5);

   //------------------------------
   // HISTOGRAMS V_AMPLITUDE WD165
   //------------------------------
   TH1F *hist_v_ampl_165[16];
   char name_v_ampl_165[20];
   char title_v_ampl_165[100];

   for (Int_t chn=0; chn<16; chn++) {
      sprintf(name_v_ampl_165, "hist_v_ampl_165%d", chn);
      sprintf(title_v_ampl_165, "WD165 - V_{ampl} of chn%d", chn);
      hist_v_ampl_165[chn] = new TH1F(name_v_ampl_165,title_v_ampl_165, 60, 0, 0.5);
      hist_v_ampl_165[chn]->GetXaxis()->SetTitle("V_ampl [mV]");
      hist_v_ampl_165[chn]->GetYaxis()->SetTitle("Entries");
      hist_v_ampl_165[chn]->GetXaxis()->SetTitleSize(0.05);
      hist_v_ampl_165[chn]->GetYaxis()->SetTitleSize(0.05);
   }

   //------------------------------
   // HISTOGRAMS CHARGHE WD165
   //------------------------------
   TH1F *hist_q_165[16];
   char name_q_165[20];
   char title_q_165[100];

   for (Int_t chn=0; chn<16; chn++) {
      sprintf(name_q_165, "hist_q_165%d", chn);
      sprintf(title_q_165, "WD165 - Q of chn%d", chn);
      hist_q_165[chn] = new TH1F(name_q_165,title_q_165, 50, 0.0, 50);
      hist_q_165[chn]->GetXaxis()->SetTitle("Q [a.u.]");
      hist_q_165[chn]->GetYaxis()->SetTitle("Entries");
      hist_q_165[chn]->GetXaxis()->SetTitleSize(0.05);
      hist_q_165[chn]->GetYaxis()->SetTitleSize(0.05);
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
      hist_q_bar_165[b] = new TH1F(name_q_bar_165,title_q_bar_165, 50, 0.0, 50);
      hist_q_bar_165[b]->GetXaxis()->SetTitle("Q [a.u.]");
      hist_q_bar_165[b]->GetYaxis()->SetTitle("Entries");
      hist_q_bar_165[b]->GetXaxis()->SetTitleSize(0.05);
      hist_q_bar_165[b]->GetYaxis()->SetTitleSize(0.05);
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
      hist_delta_time_165[b] = new TH1F(name_delta_time_165,title_delta_time_165, 40, -30.0, 30.0); // [ns]
      hist_delta_time_165[b]->GetXaxis()->SetTitle("#DeltaT [ns]");
      hist_delta_time_165[b]->GetYaxis()->SetTitle("Entries");
   }


   //==================================================================================
   // WAVEDREAM 166 - X VIEW (BAR 8 TO 10); Y VIEW (BAR 28 to 30)
   //==================================================================================
   
   //------------------------------
   // HISTOGRAMS HITBARS 8 TO 10 WD165
   //------------------------------
   TH1D *hist_hit_166_X = new TH1D("hist_hit_166_X", "Hit-map X-view bar 8 to 10", 3, 7.5, 9.5);

   //------------------------------
   // HISTOGRAMS HITBARS 28 TO 30 WD166
   //------------------------------
   TH1D *hist_hit_166_Y = new TH1D("hist_hit_166_Y", "Hit-map Y-view bar 28 to 30", 3, 27.5, 29.5);

   //------------------------------
   // HISTOGRAMS V_AMPLITUDE WD165
   //------------------------------
   TH1F *hist_v_ampl_166[16];
   char name_v_ampl_166[20];
   char title_v_ampl_166[100];

   for (Int_t chn=0; chn<16; chn++) {
      sprintf(name_v_ampl_166, "hist_v_ampl_166%d", chn);
      sprintf(title_v_ampl_166, "WD166 - V_{ampl} of chn%d", chn);
      hist_v_ampl_166[chn] = new TH1F(name_v_ampl_166,title_v_ampl_166, 60, 0, 0.5);
      hist_v_ampl_166[chn]->GetXaxis()->SetTitle("V_ampl [mV]");
      hist_v_ampl_166[chn]->GetYaxis()->SetTitle("Entries");
      hist_v_ampl_166[chn]->GetXaxis()->SetTitleSize(0.05);
      hist_v_ampl_166[chn]->GetYaxis()->SetTitleSize(0.05);
   }

   //------------------------------
   // HISTOGRAMS CHARGHE WD166
   //------------------------------
   TH1F *hist_q_166[16];
   char name_q_166[20];
   char title_q_166[100];

   for (Int_t chn=0; chn<16; chn++) {
      sprintf(name_q_166, "hist_q_166%d", chn);
      sprintf(title_q_166, "WD166 - Q of chn%d", chn);
      hist_q_166[chn] = new TH1F(name_q_166,title_q_166, 50, 0.0, 50);
      hist_q_166[chn]->GetXaxis()->SetTitle("Q [a.u.]");
      hist_q_166[chn]->GetYaxis()->SetTitle("Entries");
      hist_q_166[chn]->GetXaxis()->SetTitleSize(0.05);
      hist_q_166[chn]->GetYaxis()->SetTitleSize(0.05);
   }

   //------------------------------
   // HISTOGRAMS CHARGHE BAR WD165
   //------------------------------
   TH1F *hist_q_bar_166[16];
   char name_q_bar_166[20];
   char title_q_bar_166[100];

   for (Int_t b=0; b<16; b++) {
      sprintf(name_q_bar_166, "hist_q_bar_166%d", b);
      sprintf(title_q_bar_166, "WD166 - Q of bar%d", b);
      hist_q_bar_166[b] = new TH1F(name_q_bar_166,title_q_bar_166, 50, 0.0, 50);
      hist_q_bar_166[b]->GetXaxis()->SetTitle("Q [a.u.]");
      hist_q_bar_166[b]->GetYaxis()->SetTitle("Entries");
      hist_q_bar_166[b]->GetXaxis()->SetTitleSize(0.05);
      hist_q_bar_166[b]->GetYaxis()->SetTitleSize(0.05);
   }

   //------------------------------
   // HISTOGRAMS DELTA TIME WD166
   //------------------------------
   TH1F *hist_delta_time_166[16];
   char name_delta_time_166[20];
   char title_delta_time_166[100];

   for (Int_t b=0; b<8; b++) {
      sprintf(name_delta_time_166,"h_delta_time_166%d", b);
      sprintf(title_delta_time_166,"WD166 - #DeltaT WD166 of bar%d", b);
      hist_delta_time_166[b] = new TH1F(name_delta_time_166,title_delta_time_166, 50, -50.0, 50.0); // [ns]
      hist_delta_time_166[b]->GetXaxis()->SetTitle("#DeltaT [ns]");
      hist_delta_time_166[b]->GetYaxis()->SetTitle("Entries");
   }

   // LOOP ON ENTRIES
   for (Long64_t jentry=0; jentry<nentries; jentry++) {
      Long64_t ientry = LoadTree(jentry);
      if (ientry < 0) break;
      nb = fChain->GetEntry(jentry);   nbytes += nb;

      //==================================================================================
      // WAVEDREAM 165 - X VIEW (BAR 0 TO 7)
      //==================================================================================
      
      // INIT VALUES
      Int_t bar_TOF_X = -1;         // NUMBER OF THE BAR (X-VIEW)
      Int_t bar_TOF_Y = -1;         // NUMBER OF THE BAR (Y-VIEW)
      Double_t f_CFD = 0.3;         // FRACTION FOR COMPUTE THE TIME
      Double_t mean_time_X = -1.;   // MEAN TIME (LEFT AND RIGHT) ON THE BAR (X-VIEW)
      //Double_t mean_time_Y = -1.;   // MEAN TIME (LEFT AND RIGHT) ON THE BAR (Y-VIEW) 
      
      // LOOP ON CHANNELS OF WAVEDREAM
      for (Int_t chn=0; chn<16; chn++) {
         
         if (board165_hit[chn] == 1) {

            // INITIALIZE VALUES
            Double_t voltage_165 = 0.;                         
            Double_t v_base_165 = 0.;                          // V BASELINE (PEDESTAL) [mV]
            Double_t v_ampl_165;                               // V AMPLITUDE OF CHANNEL [mV]
            Double_t v_peak_165 = board165_waveform[chn][300]; // V MINIMUM (PEAK) [mV]
            Double_t v_th_165;                                 // V THRESHOLD [mV]
            Double_t time_165[16];                             // TIME OF WF OF CHANNEL [a.u.]
            Double_t delta_time_165;                           // ∆ TIME OF WF OF CHANNEL L AND R [a.u.]
            Double_t mean_time_165[8];                         // MEAN TIME OF L AND R CHANNELS [a.u.]
            Double_t q = 0.;                                   // CHARGE [a.u.]
            Double_t q_165[16];                                // CHARGE OF CHANNEL [a.u.]
            Double_t q_bar_165[8];                             // CHARGE OF CHARGE [a.u.]
            
            // V PEAK (MINIMUM)
            for (Int_t t=10; t<800; t++) { // LOOP ON SAMPLES OF WAVEFORM
               if (t > 50 && board165_waveform[chn][t] < v_peak_165) v_peak_165 = board165_waveform[chn][t];
               // APPROX FIRST 1/3 OF 1024 SAMPLES
               if (t <= 150) voltage_165 =  voltage_165 + board165_waveform[chn][t];
            }
 
            v_base_165 = voltage_165/140;                               // V BASELINE
            v_th_165 = v_base_165 - f_CFD * (v_base_165 - v_peak_165);  // V THRESHOLD
            v_ampl_165 = v_base_165 - v_peak_165;                       // V AMPLITUDE

            hist_v_ampl_165[chn]->Fill(v_ampl_165);

            // TIME OF CHANNEL
            for (Int_t t=10; t<900; t++) { // LOOP ON SAMPLES OF WAVEFORM
               if (board165_waveform[chn][t] == v_th_165) time_165[chn] = board165_time[chn][t]; 
               else if (board165_waveform[chn][t] < v_th_165) time_165[chn] = (board165_time[chn][t] + board165_time[chn][t-1])/2;
            }

            for (Int_t t=250; t<570; t++) q += v_base_165 - board165_waveform[chn][t];
            q_165[chn] = q;                                                // CHARGHE OF CHANNEL
            hist_q_165[chn]->Fill(q_165[chn]);
   
            if (chn%2!=0) {
               q_bar_165[chn/2] = sqrt(q_165[chn] * q_165[chn-1]);         // CHARGE OF BAR
               hist_q_bar_165[chn/2]->Fill(q_bar_165[chn/2]);
            }

            if (chn%2!=0) {
               mean_time_165[chn/2] = (time_165[chn] + time_165[chn-1])/2;  // MEAN TIME OF BAR
               mean_time_X = (time_165[chn] + time_165[chn-1])/2;
               //h_q_tot[chn/2]->Fill(q_bar[chn/2]);
            }

            if (chn%2!=0) {
               delta_time_165 = (time_165[chn] - time_165[chn-1]) * TMath::Power(10,9);   // DELTA TIME OF BAR (x10^9 to convert time from [s] in [ns])
               //cout << "delta_time_165 = " << delta_time_165[chn/2] << " [ns]" << endl;
               hist_delta_time_165[chn/2]->Fill(delta_time_165);
            }

            // BARS X TOFWALL
            if (chn==0)       bar_TOF_X = 0;
            else if (chn==1)  bar_TOF_X = 0;
            else if (chn==2)  bar_TOF_X = 1;
            else if (chn==3)  bar_TOF_X = 1;
            else if (chn==4)  bar_TOF_X = 2;
            else if (chn==5)  bar_TOF_X = 2;
            else if (chn==6)  bar_TOF_X = 3;
            else if (chn==7)  bar_TOF_X = 3;
            else if (chn==6)  bar_TOF_X = 3;
            else if (chn==8)  bar_TOF_X = 4;
            else if (chn==9)  bar_TOF_X = 4;
            else if (chn==10) bar_TOF_X = 5;
            else if (chn==11) bar_TOF_X = 5;
            else if (chn==12) bar_TOF_X = 6;
            else if (chn==13) bar_TOF_X = 6;
            else if (chn==14) bar_TOF_X = 7;
            else if (chn==15) bar_TOF_X = 7;
            
            if (0) {
               std::cout << "\n**************************"            << std::endl;
               std::cout << "\nWD 165"                                << std::endl;
               std::cout << "\nEntry           = " << jentry          << std::endl;
               std::cout << "\nchn             = " << chn             << std::endl;
               std::cout << "\nv_base_165      = " << v_base_165      << std::endl;
               std::cout << "\nv_peak_165      = " << v_peak_165      << std::endl;
               std::cout << "\nv_th_165        = " << v_th_165        << std::endl;
               std::cout << "\ntime_165        = " << time_165[chn]   << std::endl;
               std::cout << "\nv_ampl_165      = " << v_ampl_165      << std::endl;
               std::cout << "\nq_165           = " << q_165[chn]      << std::endl;
               std::cout << "\nBar (X)         = " << bar_TOF_X       << std::endl;
               if (chn%2!=0) {
                  std::cout << "\n-----------------------------"               << std::endl;
                  std::cout << "\ndelta_time_165  = " << delta_time_165        << std::endl;
                  std::cout << "\nq_bar_165       = " << q_bar_165[chn/2]      << std::endl;
                  std::cout << "\nmean_time_165   = " << mean_time_165[chn/2]  << std::endl;
                  std::cout << "\n-----------------------------"               << std::endl;
               }
            }           
         } // END if (board165_hit[ch] == 1)
      } // END LOOP ON CHANNELS

      // FILL THE HISTOGRAM
      if (bar_TOF_X > -1) hist_hit_165->Fill(bar_TOF_X);
      //if (mean_time_X > -1 && mean_time_Y > -1)  hist_mean_time->Fill(mean_time_X - mean_time_Y);
      

      //==================================================================================
      // WAVEDREAM 166 - X VIEW (BAR 8 TO 10); Y VIEW (BAR 28 to 30)
      //==================================================================================

      // INIT VALUES
      //Int_t bar_TOF_X = -1;         // NUMBER OF THE BAR (X-VIEW)
      //Int_t bar_TOF_Y = -1;         // NUMBER OF THE BAR (Y-VIEW)
      //Double_t f_CFD = 0.3;         // FRACTION FOR COMPUTE THE TIME
      //Double_t mean_time_X = -1.;   // MEAN TIME (LEFT AND RIGHT) ON THE BAR (X-VIEW)
      //Double_t mean_time_Y = -1.;   // MEAN TIME (LEFT AND RIGHT) ON THE BAR (Y-VIEW) 
      
      // LOOP ON CHANNELS OF WAVEDREAM
      for (Int_t chn=0; chn<12; chn++) {
         
         if (board165_hit[chn] == 1) {

            // INITIALIZE VALUES
            Double_t voltage_166 = 0.;                         
            Double_t v_base_166 = 0.;                          // V BASELINE (PEDESTAL) [mV]
            Double_t v_ampl_166;                               // V AMPLITUDE OF CHANNEL [mV]
            Double_t v_peak_166 = board166_waveform[chn][300]; // V MINIMUM (PEAK) [mV]
            Double_t v_th_166;                                 // V THRESHOLD [mV]
            Double_t time_166[12];                             // TIME OF WF OF CHANNEL [a.u.]
            Double_t delta_time_166;                           // ∆ TIME OF WF OF CHANNEL L AND R [a.u.]
            Double_t mean_time_166[6];                         // MEAN TIME OF L AND R CHANNELS [a.u.]
            Double_t q = 0.;                                   // CHARGE [a.u.]
            Double_t q_166[12];                                // CHARGE OF CHANNEL [a.u.]
            Double_t q_bar_166[6];                             // CHARGE OF CHARGE [a.u.]
            
            // V PEAK (MINIMUM)
            for (Int_t t=10; t<800; t++) { // LOOP ON SAMPLES OF WAVEFORM
               if (t > 50 && board166_waveform[chn][t] < v_peak_166) v_peak_166 = board166_waveform[chn][t];
               // APPROX FIRST 1/3 OF 1024 SAMPLES
               if (t <= 150) voltage_166 =  voltage_166 + board166_waveform[chn][t];
            }
 
            v_base_166 = voltage_166/140;                               // V BASELINE
            v_th_166 = v_base_166 - f_CFD * (v_base_166 - v_peak_166);  // V THRESHOLD
            v_ampl_166 = v_base_166 - v_peak_166;                       // V AMPLITUDE

            hist_v_ampl_166[chn]->Fill(v_ampl_166);

            // TIME OF CHANNEL
            for (Int_t t=10; t<900; t++) { // LOOP ON SAMPLES OF WAVEFORM
               if (board166_waveform[chn][t] == v_th_166) time_166[chn] = board166_time[chn][t]; 
               else if (board166_waveform[chn][t] < v_th_166) time_166[chn] = (board166_time[chn][t] + board166_time[chn][t-1])/2;
            }

            for (Int_t t=250; t<570; t++) q += v_base_166 - board166_waveform[chn][t];
            q_166[chn] = q;                                                // CHARGHE OF CHANNEL
            hist_q_166[chn]->Fill(q_166[chn]);
   
            if (chn%2!=0) {
               q_bar_166[chn/2] = sqrt(q_166[chn] * q_166[chn-1]);         // CHARGE OF BAR
               hist_q_bar_166[chn/2]->Fill(q_bar_166[chn/2]);
            }

            if (chn%2!=0) {
               mean_time_166[chn/2] = (time_166[chn] + time_166[chn-1])/2;  // MEAN TIME OF BAR
               mean_time_X = (time_166[chn] + time_166[chn-1])/2;
               //h_q_tot[chn/2]->Fill(q_bar[chn/2]);
            }

            if (chn%2!=0) {
               delta_time_166 = (time_166[chn] - time_166[chn-1]) * TMath::Power(10,9);   // DELTA TIME OF BAR (x10^9 to convert time from [s] in [ns])
               //cout << "delta_time_165 = " << delta_time_165[chn/2] << " [ns]" << endl;
               hist_delta_time_166[chn/2]->Fill(delta_time_166);
            }

            // BARS X TOFWALL
            if (chn==0)       bar_TOF_X = 8;
            else if (chn==1)  bar_TOF_X = 8;
            else if (chn==2)  bar_TOF_X = 9;
            else if (chn==3)  bar_TOF_X = 9;
            else if (chn==4)  bar_TOF_X = 10;
            else if (chn==5)  bar_TOF_X = 10;

            else if (chn==6)  bar_TOF_Y = 28;
            else if (chn==7)  bar_TOF_Y = 28;
            else if (chn==8)  bar_TOF_Y = 29;
            else if (chn==9)  bar_TOF_Y = 29;
            else if (chn==10) bar_TOF_Y = 30;
            else if (chn==11) bar_TOF_Y = 30;
   
            
            if (0) {
               std::cout << "\n**************************"            << std::endl;
               std::cout << "\nWD 166"                                << std::endl;
               std::cout << "\nEntry           = " << jentry          << std::endl;
               std::cout << "\nchn             = " << chn             << std::endl;
               std::cout << "\nv_base_166      = " << v_base_166      << std::endl;
               std::cout << "\nv_peak_166      = " << v_peak_166      << std::endl;
               std::cout << "\nv_th_166        = " << v_th_166        << std::endl;
               std::cout << "\ntime_166        = " << time_166[chn]   << std::endl;
               std::cout << "\nv_ampl_166      = " << v_ampl_166      << std::endl;
               std::cout << "\nq_166           = " << q_166[chn]      << std::endl;
               std::cout << "\nBar (X)         = " << bar_TOF_X       << std::endl;
               if (chn%2!=0) {
                  std::cout << "\n-----------------------------"               << std::endl;
                  std::cout << "\ndelta_time_166  = " << delta_time_166        << std::endl;
                  std::cout << "\nq_bar_166       = " << q_bar_166[chn/2]      << std::endl;
                  std::cout << "\nmean_time_166   = " << mean_time_166[chn/2]  << std::endl;
                  std::cout << "\n-----------------------------"               << std::endl;
               }
            }           
         } // END if (board165_hit[ch] == 1)
      } // END LOOP ON CHANNELS

      // FILL THE HISTOGRAM
      if (bar_TOF_X > -1) hist_hit_166->Fill(bar_TOF_X);
      //if (mean_time_X > -1 && mean_time_Y > -1)  hist_mean_time->Fill(mean_time_X - mean_time_Y);
   
   } // END LOOP ON ENTRIES
   
      
   //==================================================================================
   // WAVEDREAM 165 - X VIEW (BAR 0 TO 7)
   //==================================================================================

   //------------------------------
   // HITS ON BARS
   //------------------------------
   TCanvas *c_hit_165 = new TCanvas("c_hit_165", "hit", 1200, 1200);
   c_hit_165->SetTickx();
   c_hit_165->SetTicky();
   c_hit_165->SetLeftMargin(0.15);

   hist_hit_165->GetXaxis()->SetTitle("FronBar, LayerX");
   hist_hit_165->SetNdivisions(8,"X");
   hist_hit_165->GetYaxis()->SetTitle("Counts");
   hist_hit_165->Draw();

   //------------------------------
   // V AMPLITUDE
   //------------------------------
   TCanvas *c_v_ampl_165 = new TCanvas("c_v_ampl_165", "c_v_ampl_165", 1200, 1200);
   c_v_ampl_165->Divide(4,4);

   for (int chn=0; chn<16; chn++)
   {  
      c_v_ampl_165->cd(chn+1);

      //TString canvas_title_v_ampl_165 = Form("c_v_ampl_165%d", chn); 
      //TCanvas *c_v_ampl_165 = new TCanvas(canvas_title_v_ampl_165, canvas_title_v_ampl_165, 600, 600);
      c_v_ampl_165->SetTickx();
      c_v_ampl_165->SetTicky();
      c_v_ampl_165->SetLeftMargin(0.15);

      hist_v_ampl_165[chn]->SetMarkerStyle(20);
      hist_v_ampl_165[chn]->SetMarkerStyle(kFullCircle);
      hist_v_ampl_165[chn]->SetMarkerColor(kBlack);
      hist_v_ampl_165[chn]->SetFillColor(kCyan-9);
      //hist_v_ampl_165[chn]->Fit("landau", "Q");
      hist_v_ampl_165[chn]->Draw();
   }

   //------------------------------
   // CHARGE CHANNEL
   //------------------------------
   TCanvas *c_q_165 = new TCanvas("c_q_165", "c_q_165", 1200, 1200);
   c_q_165->Divide(4,4);

   for (int chn=0; chn<16; chn++)
   {  
      c_q_165->cd(chn+1);
      //TString canvas_title_q_165 = Form("c_q_165%d", chn); 
      //TCanvas *c_q_165 = new TCanvas(canvas_title_q_165, canvas_title_q_165, 600, 600);
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

   //------------------------------
   // CHARGHE BAR
   //------------------------------
   TCanvas *c_q_bar_165 = new TCanvas("c_q_bar_165", "c_q_bar_165", 1200, 1200);
   c_q_bar_165->Divide(4,2);

   for (int b=0; b<8; b++)
   {  
      c_q_bar_165->cd(b+1);
      //TString canvas_title_q_bar_165 = Form("c_q_bar_165%d", chn); 
      //TCanvas *c_q_165 = new TCanvas(canvas_title_q_bar_165, canvas_title_q_bar_165, 600, 600);
      c_q_bar_165->SetTickx();
      c_q_bar_165->SetTicky();
      c_q_bar_165->SetLeftMargin(0.15);

      hist_q_bar_165[b]->SetMarkerStyle(20);
      hist_q_bar_165[b]->SetMarkerStyle(kFullCircle);
      hist_q_bar_165[b]->SetMarkerColor(kBlack);
      hist_q_bar_165[b]->SetFillColor(45);
      hist_q_bar_165[b]->Fit("landau", "Q");
      hist_q_bar_165[b]->Draw();
   }
      
   //------------------------------
   // DELTA TIME
   //------------------------------
   TCanvas *c_delta_time_165 = new TCanvas("c_delta_time_165", "Delta t 165",1200,1200);
   c_delta_time_165->Divide(4,2);

   for (int b=0; b<8; b++)
   {  
      c_delta_time_165->cd(b+1);
      //TString canvas_title_delta_time_165 = Form("c_delta_time_165bar%d",b); 
      //TCanvas *c_delta_time_165 = new TCanvas(canvas_title_delta_time_165, canvas_title_delta_time_165, 600, 600);
      c_delta_time_165->SetTickx();
      c_delta_time_165->SetTicky();
      c_delta_time_165->SetLeftMargin(0.15);

      hist_delta_time_165[b]->SetMarkerStyle(20);
      hist_delta_time_165[b]->SetMarkerStyle(kFullCircle);
      hist_delta_time_165[b]->SetMarkerColor(kBlack);
      hist_delta_time_165[b]->SetFillColor(5); // Yellow
      hist_delta_time_165[b]->Fit("gaus", "Q");
      hist_delta_time_165[b]->Draw();
   }

   //==================================================================================
   // WAVEDREAM 166 
   //==================================================================================

   //------------------------------
   // HITS ON BARS
   //------------------------------
   TCanvas *c_hit_166_X = new TCanvas("c_hit_166_X", "hit", 1200, 1200);
   c_hit_166_X->SetTickx();
   c_hit_166_X->SetTicky();
   c_hit_166_X->SetLeftMargin(0.15);

   hist_hit_166_X->GetXaxis()->SetTitle("FrontBar, LayerX");
   hist_hit_166_X->SetNdivisions(8,"X");
   hist_hit_166_X->GetYaxis()->SetTitle("Counts");
   hist_hit_166_X->Draw();

   //------------------------------
   // HITS ON BARS
   //------------------------------
   TCanvas *c_hit_166_Y = new TCanvas("c_hit_166_Y", "hit", 1200, 1200);
   c_hit_166_Y->SetTickx();
   c_hit_166_Y->SetTicky();
   c_hit_166_Y->SetLeftMargin(0.15);

   hist_hit_166_Y->GetXaxis()->SetTitle("FrontBar, LayerY");
   hist_hit_166_Y->SetNdivisions(8,"X");
   hist_hit_166_Y->GetYaxis()->SetTitle("Counts");
   hist_hit_166_Y->Draw();

   //------------------------------
   // V AMPLITUDE
   //------------------------------
   TCanvas *c_v_ampl_166 = new TCanvas("c_v_ampl_166", "c_v_ampl_166", 1200, 1200);
   c_v_ampl_166->Divide(4,4);

   for (int chn=0; chn<16; chn++)
   {  
      c_v_ampl_166->cd(chn+1);

      //TString canvas_title_v_ampl_166 = Form("c_v_ampl_166%d", chn); 
      //TCanvas *c_v_ampl_166 = new TCanvas(canvas_title_v_ampl_166, canvas_title_v_ampl_166, 600, 600);
      c_v_ampl_166->SetTickx();
      c_v_ampl_166->SetTicky();
      c_v_ampl_166->SetLeftMargin(0.15);

      hist_v_ampl_166[chn]->SetMarkerStyle(20);
      hist_v_ampl_166[chn]->SetMarkerStyle(kFullCircle);
      hist_v_ampl_166[chn]->SetMarkerColor(kBlack);
      hist_v_ampl_166[chn]->SetFillColor(kCyan-9);
      //hist_v_ampl_166[chn]->Fit("landau", "Q");
      hist_v_ampl_166[chn]->Draw();
   }

   //------------------------------
   // CHARGE CHANNEL
   //------------------------------
   TCanvas *c_q_166 = new TCanvas("c_q_166", "c_q_166", 1200, 1200);
   c_q_166->Divide(4,4);

   for (int chn=0; chn<16; chn++)
   {  
      c_q_166->cd(chn+1);
      //TString canvas_title_q_166 = Form("c_q_166%d", chn); 
      //TCanvas *c_q_166 = new TCanvas(canvas_title_q_166, canvas_title_q_166, 600, 600);
      c_q_166->SetTickx();
      c_q_166->SetTicky();
      c_q_166->SetLeftMargin(0.15);

      hist_q_166[chn]->SetMarkerStyle(20);
      hist_q_166[chn]->SetMarkerStyle(kFullCircle);
      hist_q_166[chn]->SetMarkerColor(kBlack);
      hist_q_166[chn]->SetFillColor(45);
      //hist_q_166[chn]->Fit("landau", "Q");
      hist_q_166[chn]->Draw();
   }

   //------------------------------
   // CHARGHE BAR
   //------------------------------
   TCanvas *c_q_bar_166 = new TCanvas("c_q_bar_166", "c_q_bar_166", 1200, 1200);
   c_q_bar_166->Divide(4,2);

   for (int b=0; b<8; b++)
   {  
      c_q_bar_166->cd(b+1);
      //TString canvas_title_q_bar_166 = Form("c_q_bar_166%d", chn); 
      //TCanvas *c_q_166 = new TCanvas(canvas_title_q_bar_166, canvas_title_q_bar_166, 600, 600);
      c_q_bar_166->SetTickx();
      c_q_bar_166->SetTicky();
      c_q_bar_166->SetLeftMargin(0.15);

      hist_q_bar_166[b]->SetMarkerStyle(20);
      hist_q_bar_166[b]->SetMarkerStyle(kFullCircle);
      hist_q_bar_166[b]->SetMarkerColor(kBlack);
      hist_q_bar_166[b]->SetFillColor(45);
      hist_q_bar_166[b]->Fit("landau", "Q");
      hist_q_bar_166[b]->Draw();
   }
      
   //------------------------------
   // DELTA TIME
   //------------------------------
   TCanvas *c_delta_time_166 = new TCanvas("c_delta_time_166", "Delta t 166",1200,1200);
   c_delta_time_166->Divide(4,2);

   for (int b=0; b<8; b++)
   {  
      c_delta_time_166->cd(b+1);
      //TString canvas_title_delta_time_166 = Form("c_delta_time_166bar%d",b); 
      //TCanvas *c_delta_time_166 = new TCanvas(canvas_title_delta_time_166, canvas_title_delta_time_166, 600, 600);
      c_delta_time_166->SetTickx();
      c_delta_time_166->SetTicky();
      c_delta_time_166->SetLeftMargin(0.15);

      hist_delta_time_166[b]->SetMarkerStyle(20);
      hist_delta_time_166[b]->SetMarkerStyle(kFullCircle);
      hist_delta_time_166[b]->SetMarkerColor(kBlack);
      hist_delta_time_166[b]->SetFillColor(5); // Yellow
      hist_delta_time_166[b]->Fit("gaus", "Q");
      hist_delta_time_166[b]->Draw();
   }

   //------------------------------
   // SAVE HISROGRAMS IN ROOT FILE
   //------------------------------
   //hfile.Write();
}
