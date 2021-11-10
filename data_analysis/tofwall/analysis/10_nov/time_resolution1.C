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

   gStyle->SetOptStat(0);
   gStyle->SetPadGridX(kTRUE); // draw horizontal and vertical grids
   gStyle->SetPadGridY(kTRUE);

   //**************************************
   // CANVAS
   //**************************************
   TCanvas *c1 = new TCanvas("c1", "hitmap",600,600);
   c1->SetTickx();
   c1->SetTicky();
   c1->SetLeftMargin(0.15);

   //**************************************
   // HISTOGRAM
   //**************************************
   
   //TH2D *hist = new TH2D("hist", "Hit-map Test Cosmic rays", 20, 0, 20, 20, 20, 40);
   TH2D *hist = new TH2D("hist", "Hit-map Test Cosmic rays", 20, -0.5, 19.5, 20, 19.5, 39.5);

   //------------------------------
   // HISTOGRAMS DELTA TIME
   //------------------------------
   TH1F *h_delta_time[16];
   char name_delta_time[20];
   char title_delta_time[100];

   for (Int_t b=0; b<8; b++) {
      sprintf(name_delta_time,"h_delta_time%d", b);
      sprintf(title_delta_time,"WD165 - Delta T WF of bar%d", b);
      h_delta_time[b] = new TH1F(name_delta_time,title_delta_time,100, 250, 450);
      h_delta_time[b]->GetXaxis()->SetTitle("Delta T [a.u.]");
      h_delta_time[b]->GetYaxis()->SetTitle("Entries");
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
      Int_t bar_TOF_X = -1;
      Int_t bar_TOF_Y = -1;
      Double_t f_CFD = 0.3; // FRACTION FOR COMPUTE THE TIME 

      //********************************************************************
      // WAVEDREAM 165 - X VIEW (BAR 0 TO 7)
      //********************************************************************   
      
      // LOOP ON CHANNELS OF WAVEDREAM
      for (Int_t ch=0; ch<16; ch++) {
         
         if (board165_hit[ch] == 1) {

            // INITIALIZE VALUES
            Double_t voltage_165 = 0.;                         
            Double_t v_base_165 = 0.;                          // V BASELINE (PEDESTAL) [mV]
            Double_t v_ampl_165[16];                           // V AMPLITUDE OF CHANNEL [mV]
            Double_t v_peak_165 = board165_waveform[ch][300];  // V MINIMUM (PEAK) [mV]
            Double_t v_th;                                     // V THRESHOLD [mV]
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
 
            v_base_165 = voltage_165/140;                          // V BASELINE
            v_th = v_base_165 - f_CFD * (v_base_165 - v_peak_165); // V THRESHOLD
            v_ampl_165[ch] = v_base_165 - v_peak_165;              // V AMPLITUDE

            // TIME OF CHANNEL
            for (Int_t t=10; t<900; t++) {
               if (board165_waveform[ch][t] == v_th) time_165[ch] = t;
               else if (board165_waveform[ch][t] < v_th) time_165[ch] = (t+(t-1))/2;  // time_165[ch] = time when the WF crosses Vth, evaluated through a linear interpolation of the two points nearest to the threshold.
            }

            // CHARGHE OF CHANNEL
            for (Int_t t=250; t<570; t++) {      
               q_165[ch] += (v_base_165 - board165_waveform[ch][t]);
            }
   
            // CHARGHE OF BAR
            if (ch%2!=0) {
               q_bar_165[ch/2] = sqrt(q_165[ch] * q_165[ch-1]); 
               //h_q_tot[ch/2]->Fill(q_bar[ch/2]);
            }

            // MEAN TIME OF BAR
            if (ch%2!=0) {
               mean_time_165[ch/2] = (time_165[ch] + time_165[ch-1])/2; 
               //h_q_tot[ch/2]->Fill(q_bar[ch/2]);
            }

            // DELTA TIME OF BAR
            if (ch%2!=0) {
               delta_time_165[ch/2] = time_165[ch] - time_165[ch-1]; // QUALCOSA NO VA...
               //h_q_tot[ch/2]->Fill(q_bar[ch/2]);
            }

            // BAR TOFWALL
            if (ch==0) bar_TOF_X = 0;
            else if (ch==1) bar_TOF_X = 0;
            else if (ch==2) bar_TOF_X = 1;
            else if (ch==3) bar_TOF_X = 1;
            else if (ch==4) bar_TOF_X = 2;
            else if (ch==5) bar_TOF_X = 2;
            else if (ch==6) bar_TOF_X = 3;
            else if (ch==7) bar_TOF_X = 3;
            else if (ch==6) bar_TOF_X = 3;
            else if (ch==8) bar_TOF_X = 4;
            else if (ch==9) bar_TOF_X = 4;
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
            std::cout << "\nv_th            = " << v_th           << std::endl;
            std::cout << "\ntime_165        = " << time_165[ch]   << std::endl;
            std::cout << "\nv_ampl_165      = " << v_ampl_165[ch] << std::endl;
            std::cout << "\nq_165           = " << q_165[ch]      << std::endl;
            std::cout << "\nBar             = " << bar_TOF_X      << std::endl;
             if (ch%2!=0) {
                std::cout << "\n-----------------------------"              << std::endl;
                std::cout << "\ndelta_time_165  = " << time_165[ch/2]       << std::endl;
                std::cout << "\nq_bar_165       = " << q_bar_165[ch/2]      << std::endl;
                std::cout << "\nmean_time_165   = " << mean_time_165[ch/2]  << std::endl;
                std::cout << "\n-----------------------------"              << std::endl;
             }           
         } // END IF 
      } // END LOOP ON CHANNELS

      //********************************************************************
      // WAVEDREAM 166 - X VIEW (BAR 8 TO 10); Y VIEW (BAR 28 to 30)
      //********************************************************************

      for (Int_t ch=0; ch<12; ch++){

         // INITIALIZE VALUES
         Double_t voltage_166 = 0.;                         
         Double_t baseline_166 = 0.;                     
         Double_t ampl_166[nentries][16];                                  
         Double_t min_166 = board166_waveform[ch][300];  
         
         // LOOP ON SAMPLES OF WAVEFORM
         for (Int_t v=10; v<800; v++){

            if (v > 50 && board166_waveform[ch][v] < min_166){min_166 = board166_waveform[ch][v];}
            // APPROX FIRST 1/3 OF 1024 SAMPLES
            if (v <= 150) {voltage_166 =  voltage_166 + board166_waveform[ch][v];}      
         }
         baseline_166 = voltage_166/140;

         // AMPLITUDE (EVENTO J-ESIMO, CANALE CH)    
         ampl_166[jentry][ch] = baseline_166 - min_166;  
         
         if (board166_hit[ch] == 1 && ampl_166[jentry][ch] > 0.020)
         {
            if (1)
            {
               if (ch==0) bar_TOF_X = 8;
               else if (ch==1) bar_TOF_X = 8;
               else if (ch==2) bar_TOF_X = 9;
               else if (ch==3) bar_TOF_X = 9;
               else if (ch==4) bar_TOF_X = 10;
               else if (ch==5) bar_TOF_X = 10;
               else if (ch==6) bar_TOF_Y = 28;
               else if (ch==7) bar_TOF_Y = 28;
               else if (ch==8) bar_TOF_Y = 29;
               else if (ch==9) bar_TOF_Y = 29;
               else if (ch==10) bar_TOF_Y = 30;
               else if (ch==11) bar_TOF_Y = 30;
               /*
               std::cout << "WD 166" << std::endl;
               std::cout << "V_ampl["<<jentry<<"]["<<ch<<"] = " << ampl_166[jentry][ch] << std::endl;
               std::cout << "Barra = " << bar_TOF_X << std::endl;
               std::cout << "------------------------------\n" << std::endl;
               */
            }                    
         }  
      }// END LOOP ON CHANNEL

      //********************************************************************
      // WAVEDREAM 170 - X VIEW (BAR 11 TO 18)
      //********************************************************************

      for (Int_t ch=0; ch<16; ch++){

         // INITIALIZE VALUES
         Double_t voltage_170 = 0.;                         
         Double_t baseline_170 = 0.;                     
         Double_t ampl_170[nentries][16];                   
         Double_t min_170 = board170_waveform[ch][300];  
         
         // LOOP ON SAMPLES OF WAVEFORM
         for (Int_t v=10; v<800; v++){
         
            if (v > 50 && board170_waveform[ch][v] < min_170){min_170 = board170_waveform[ch][v];}
            // APPROX FIRST 1/3 OF 1024 SAMPLES
            if (v <= 150) {voltage_170 =  voltage_170 + board170_waveform[ch][v];}      
         }
         baseline_170 = voltage_170/140;

         // AMPLITUDE (EVENTO J-ESIMO, CANALE CH)    
         ampl_170[jentry][ch] = baseline_170 - min_170;
         
         if (board170_hit[ch] == 1)
         {
            if (1)
            {  
               if (ch==0) bar_TOF_X = 11;
               else if (ch==1) bar_TOF_X = 11;
               else if (ch==2) bar_TOF_X = 12;
               else if (ch==3) bar_TOF_X = 12;
               else if (ch==4) bar_TOF_X = 13;
               else if (ch==5) bar_TOF_X = 13;
               else if (ch==6) bar_TOF_X = 14;
               else if (ch==7) bar_TOF_X = 14;
               else if (ch==8) bar_TOF_X = 15;
               else if (ch==9) bar_TOF_X = 15;
               else if (ch==10) bar_TOF_X = 16;
               else if (ch==11) bar_TOF_X = 16;
               else if (ch==12) bar_TOF_X = 17;
               else if (ch==13) bar_TOF_X = 17;
               else if (ch==14) bar_TOF_X = 18;
               else if (ch==15) bar_TOF_X = 18;
               /*
               std::cout << "WD 157" << std::endl;
               std::cout << "V_ampl["<<jentry<<"]["<<ch<<"] = " << ampl_157[jentry][ch] << std::endl;
               std::cout << "Barra = " << bar_TOF_X << std::endl;
               std::cout << "------------------------------\n" << std::endl;
               */
            }
         }
      }// END LOOP ON CHANNEL

      //********************************************************************
      // WAVEDREAM 167 - X VIEW (BAR 19); Y VIEW (BAR 20 TO 26)
      //********************************************************************

      for (Int_t ch=0; ch<16; ch++){

         // INITIALIZE VALUES
         Double_t voltage_167 = 0.;                         
         Double_t baseline_167 = 0.;                     
         Double_t ampl_167[nentries][16];           
         Double_t min_167 = board167_waveform[ch][300];  
         
         // LOOP ON SAMPLES OF WAVEFORM
         for (Int_t v=10; v<800; v++){
         
            if (v > 50 && board167_waveform[ch][v] < min_167){min_167 = board167_waveform[ch][v];}
            // APPROX FIRST 1/3 OF 1024 SAMPLES
            if (v <= 150) {voltage_167 =  voltage_167 + board167_waveform[ch][v];}      
         }
         baseline_167 = voltage_167/140;

         // AMPLITUDE (EVENTO J-ESIMO, CANALE CH)    
         ampl_167[jentry][ch] = baseline_167 - min_167;  
         
         if (board167_hit[ch] == 1)
         {
            if (1)
            {
               if (ch==0) bar_TOF_X = 19;
               else if (ch==1) bar_TOF_X = 19;
               else if (ch==2) bar_TOF_Y = 20;
               else if (ch==3) bar_TOF_Y = 20;
               else if (ch==4) bar_TOF_Y = 21;
               else if (ch==5) bar_TOF_Y = 21;
               else if (ch==6) bar_TOF_Y = 22;
               else if (ch==7) bar_TOF_Y = 22;
               else if (ch==8) bar_TOF_Y = 23;
               else if (ch==9) bar_TOF_Y = 23;
               else if (ch==10) bar_TOF_Y = 24;
               else if (ch==11) bar_TOF_Y = 24;
               else if (ch==12) bar_TOF_Y = 25;
               else if (ch==13) bar_TOF_Y = 25;
               else if (ch==14) bar_TOF_Y = 26;
               else if (ch==15) bar_TOF_Y = 26;
               /*
               std::cout << "WD 167" << std::endl;
               std::cout << "V_ampl["<<jentry<<"]["<<ch<<"] = " << ampl_158[jentry][ch] << std::endl;
               std::cout << "Barra = " << bar_TOF_X << std::endl;
               std::cout << "------------------------------\n" << std::endl;
               */
            }
         }
      }// END LOOP ON CHANNEL

      //********************************************************************
      // WAVEDREAM 168 - Y VIEW (BAR 27 ANS 31 TO 37)
      //********************************************************************

      for (Int_t ch=0; ch<16; ch++){

         // INITIALIZE VALUES
         Double_t voltage_168 = 0.;                         
         Double_t baseline_168 = 0.;                     
         Double_t ampl_168[nentries][16];
         Double_t min_168 = board168_waveform[ch][300];  
         
         // LOOP ON SAMPLES OF WAVEFORM
         for (Int_t v=10; v<800; v++){
         
            if (v > 50 && board168_waveform[ch][v] < min_168){min_168 = board168_waveform[ch][v];}
            // APPROX FIRST 1/3 OF 1024 SAMPLES
            if (v <= 150) {voltage_168 =  voltage_168 + board168_waveform[ch][v];}      
         }
         baseline_168 = voltage_168/140;

         // AMPLITUDE (EVENTO J-ESIMO, CANALE CH)    
         ampl_168[jentry][ch] = baseline_168 - min_168;  
         
         if (board168_hit[ch] == 1)
         //if (jentry > 0 && abs(ampl_168[jentry][ch]-ampl_168[jentry-1][ch])>0.00001 && ampl_168[jentry][ch]>0.)
         {
            if (1)
            {
               if (ch==0) bar_TOF_Y = 27;
               else if (ch==1) bar_TOF_Y = 27;
               else if (ch==2) bar_TOF_Y = 31;
               else if (ch==3) bar_TOF_Y = 31;
               else if (ch==4) bar_TOF_Y = 32;
               else if (ch==5) bar_TOF_Y = 32;
               else if (ch==6) bar_TOF_Y = 33;
               else if (ch==7) bar_TOF_Y = 33;
               else if (ch==8) bar_TOF_Y = 34;
               else if (ch==9) bar_TOF_Y = 34;
               else if (ch==10) bar_TOF_Y = 35;
               else if (ch==11) bar_TOF_Y = 35;
               else if (ch==12) bar_TOF_Y = 36;
               else if (ch==13) bar_TOF_Y = 36;
               else if (ch==14) bar_TOF_Y = 37;
               else if (ch==15) bar_TOF_Y = 37;
               /*
               std::cout << "WD 168" << std::endl;
               std::cout << "V_ampl["<<jentry<<"]["<<ch<<"] = " << ampl_168[jentry][ch] << std::endl;
               std::cout << "Barra = " << bar_TOF_Y << std::endl;
               std::cout << "------------------------------\n" << std::endl;
               */
            }
         }
      }// END LOOP ON CHANNEL

      //********************************************************************
      // WAVEDREAM 158 - Y VIEW (BAR 38 TO 39)
      //********************************************************************

      for (Int_t ch=0; ch<4; ch++){

         // INITIALIZE VALUES
         Double_t voltage_158 = 0.;                        
         Double_t baseline_158 = 0.;                
         Double_t ampl_158[nentries][16];
         Double_t min_158 = board158_waveform[ch][300];  
         
         // LOOP ON SAMPLES OF WAVEFORM
         for (Int_t v=10; v<800; v++){
         
            if (v > 50 && board158_waveform[ch][v] < min_158){min_158 = board158_waveform[ch][v];}
            // APPROX FIRST 1/3 OF 1024 SAMPLES
            if (v <= 150) {voltage_158 =  voltage_158 + board158_waveform[ch][v];}
         }
         baseline_158 = voltage_158/140;

         // AMPLITUDE (EVENTO J-ESIMO, CANALE CH)    
         ampl_158[jentry][ch] = baseline_158 - min_158;  
         
         if (board158_hit[ch] == 1)
         //if (jentry > 0 && abs(ampl_168[jentry][ch]-ampl_168[jentry-1][ch])>0.00001 && ampl_168[jentry][ch]>0.)
         {
            if (1)
            {                 
               if (ch==0) bar_TOF_Y= 38;
               else if (ch==1) bar_TOF_Y= 38;
               else if (ch==2) bar_TOF_Y = 39;
               else if (ch==3) bar_TOF_Y = 39;
               /*
               std::cout << "WD 158" << std::endl;
               std::cout << "V_ampl["<<jentry<<"]["<<ch<<"] = " << ampl_158[jentry][ch] << std::endl;
               std::cout << "Barra = " << bar_TOF_Y << std::endl;
               std::cout << "------------------------------\n" << std::endl;
               */
            }
         }
      }// END LOOP ON CHANNEL

      // FILL THE HISTOGRAM
      if (bar_TOF_X > -1 && bar_TOF_Y > -1) {
         hist->Fill(bar_TOF_X, bar_TOF_Y);
      }
      else {
         //std::cout << "No Match X-Y Bar for event # " << jentry << endl;
      }
   
   }// END LOOP ON ENTRIES
   hist->GetXaxis()->SetTitle("FronBar, LayerX");
   hist->SetNdivisions(20,"X");
   hist->GetYaxis()->SetTitle("RearBar, LayerY");
   hist->SetNdivisions(20,"Y");
   hist->GetZaxis()->SetTitle("# hits");
   hist->Draw("colz TEXT0 SAME");//https://root.cern.ch/doc/v608/classTHistPainter.html#HP15
   //hist->Draw("colz");
}
