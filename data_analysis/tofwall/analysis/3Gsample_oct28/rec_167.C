//--------------------------------------------------------------------------------
// MACRO TO APPLY ON "cosmici_tot.root"
//--------------------------------------------------------------------------------
// Macro to make histogram of amplitude of the waveform of wavedream 167.
//
// Date: 
//
//  METHOD:
//  -------
//
//  1) Loop on all events recorded in the file <test_tot.root>
//    2) Loop on all channel
//       3) Loop on all samples of waveform
//          3.1) Find minimum
//          3.2) Compute baseline (as mean value of first 1/3 of 1024 samples)
//          3.3) Compute amplitude: [amplitude] = [baseline] - [minimum]
//          3.5) Compute charge: [charge] = [amplitude]x[time]
//          3.6) Fill the histogram of amplitude
//          3.7) Fill the histogram of charge
//       4) Landau Fit on the distributions
//
//--------------------------------------------------------------------------------


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
   TFile hfile("WD167.root","RECREATE");
   gStyle->SetOptFit(10111);

   //------------------------------
   // CANVAS AMPLLITUDE
   //------------------------------
   TCanvas *c_ampl = new TCanvas("c_ampl", "WD167 Amplitude", 600, 600);
   c_ampl->SetTickx();
   c_ampl->SetTicky();
   c_ampl->SetLeftMargin(0.15);

   //------------------------------
   // HISTOGRAMS AMPLITUDE
   //------------------------------
   TH1F *h_ampl[16];
   char name_ampl[20];
   char title_ampl[100];
   for (Int_t i=0;i<16;i++) 
   {
      sprintf(name_ampl,"h_ampl%d",i);
      sprintf(title_ampl,"WD167 - Amplitue WF of ch%d",i);
      h_ampl[i] = new TH1F(name_ampl,title_ampl,40, -0.05, 0.5);
      h_ampl[i]->GetXaxis()->SetTitle("Amplitude waveform [mV]");
      h_ampl[i]->GetYaxis()->SetTitle("Entries");
      h_ampl[i]->SetMarkerStyle(20);
      h_ampl[i]->SetMarkerStyle(kFullCircle);
      h_ampl[i]->SetMarkerColor(kBlack);
      //h_ampl[i]->SetFillColor(kCyan-9);
   }

   //------------------------------
   // CANVAS MIN POSITION
   //------------------------------
   TCanvas *c_q = new TCanvas("c_q", "WD167 Charge", 600, 600);
   c_q->SetTickx();
   c_q->SetTicky();
   c_q->SetLeftMargin(0.15);

   //------------------------------
   // HISTOGRAMS CHARGE
   //------------------------------
   TH1F *h_q[16];
   char name_q[20];
   char title_q[100];
   for (Int_t i=0;i<16;i++) 
   {
      sprintf(name_q,"h_q%d",i);
      sprintf(title_q,"WD167 - Charge WF of ch%d",i);
      h_q[i] = new TH1F(name_q,title_q,50, 0.0, 100);
      h_q[i]->GetXaxis()->SetTitle("Charge waveform [a.u.]");
      h_q[i]->GetYaxis()->SetTitle("Entries");
      h_q[i]->GetYaxis()->SetTitle("Entries");
      h_q[i]->SetMarkerStyle(20);
      h_q[i]->SetMarkerStyle(kFullCircle);
      h_q[i]->SetMarkerColor(kBlack);
      //h_q[i]->SetFillColor(45);
   }

   //------------------------------
   // CANVAS MIN POSITION
   //------------------------------
   TCanvas *c_min = new TCanvas("c_min", "WD167 Min Position", 600, 600);
   c_min->SetTickx();
   c_min->SetTicky();
   c_min->SetLeftMargin(0.15);

   //------------------------------
   // HISTOGRAMS MIN POSITION
   //------------------------------
   TH1F *h_w_min[16];
   char name_min[20];
   char title_min[100];
   for (Int_t i=0;i<16;i++) 
   {
      sprintf(name_min,"h_min%d",i);
      sprintf(title_min,"WD167 - Min position of ch%d",i);
      h_w_min[i] = new TH1F(name_min,title_min,100, 250, 450);
      h_w_min[i]->GetXaxis()->SetTitle("# samples [u.m.]");
      h_w_min[i]->GetYaxis()->SetTitle("Entries");
      h_w_min[i]->SetMarkerStyle(20);
      h_w_min[i]->SetMarkerStyle(kFullCircle);
      h_w_min[i]->SetMarkerColor(kBlack);
      //h_w_min[i]->SetFillColor(kGreen-9);
   }

   //------------------------------
   // FILE TO SAVE DATA
   //------------------------------
   fstream file;
   file.open("WD167.txt", ios::out);

   // NAME COLUMNS
   file << "n_event"<<left<<"\t";
   for (Int_t ch=0; ch<16; ch++)
   {
      file <<"\t" <<"ch" << ch <<left<< setw(14)<< "\t\t";
   }
   file << endl;

   //------------------------------
   // LOOP ON ENTRIES
   //------------------------------
   for (Long64_t jentry=0; jentry<nentries; jentry++)
   {
      Long64_t ientry = LoadTree(jentry);
      if (ientry < 0) break;

      nb = fChain->GetEntry(jentry);   nbytes += nb;

      // NAMEN RAWS
      file << jentry << "\t";

      //------------------------------
      // LOOP ON CHANNELS
      //------------------------------
      for (Int_t ch=0; ch<16; ch++)
      {
         //------------------------------
         // INITIALIZE VALUES
         //------------------------------
         Double_t wave = 0.;                         // [mV]   INITIALIZE VALUE OF WAVEFORM
         Double_t w_min = 0;                         // [s]    INITIALIZE COORESPONDING TIME VALUE OF MINIMUM
         Double_t baseline = 0.;                     // [mV]   INITIALIZE BASELINE VALUE
         Double_t ampl[11236][16];                   // [mV]   INITIALIZE AMPLITUDE VALUE
         Double_t new_ampl[11236][16];               // [mV]   INITIALIZE AMPLITUDE VALUE (removed same values)
         Double_t charge[11236][16];                 // [a.u.] INITIALIZE CHARGE MATRIX (removed same values)
         Double_t q = 0.;                            // [a.u.] INITIALIZE CHARGE VALUE
         Double_t min = board167_waveform[ch][300];  // INITIALIZE MIN VALUE (Usually located at 1/3 of range)
         Double_t q_L = 0.;                          // CHARGE COLLECTED IN LEFT CH OF BAR
         Double_t q_R = 0.;                          // CHARGE COLLECTED IN  RIGHT CH OF BAR
         //Double_t X_TOF_view[];
         //------------------------------
         // LOOP ON SAMPLES OF WAVEFORM
         //------------------------------
         for (Int_t w=10; w<=800; w++) // Don't 1024 (problem with last component!)
         {
            if (w > 50 && board167_waveform[ch][w] < min)
            {
               min = board167_waveform[ch][w];
               w_min = w;
            }
            // APPROX FIRST 1/3 OF 1024 SAMPLES
            if (w <= 170) {wave =  wave + board167_waveform[ch][w];}      
         }

         baseline = wave/170;                  // COMPUTE BASELINE  [mV] (mean values of the first 200 bins)
         ampl[jentry][ch] = baseline - min;    // COMPUTE AMPLITUDE [mV]
         
         for (Int_t w=250; w<570; w++)
         {
            //q += (board167_time[ch][w]-board167_time[ch][w-1])*ampl[jentry][ch];
            q += ampl[jentry][ch];
         }

         charge[jentry][ch] = q;               // COMPUTE CHARGE    [a.u.]

         if (jentry > 0 && ampl[jentry][ch]-ampl[jentry-1][ch]<0.001)
         {
            new_ampl[jentry][ch] = 0.;
         }
         else
         {
            new_ampl[jentry][ch] = ampl[jentry][ch];

            //------------------------------
            // FILLING HISTOGRAMS
            //------------------------------
            h_ampl[ch]->Fill(new_ampl[jentry][ch]);
            h_w_min[ch]->Fill(w_min);
            h_q[ch]->Fill(charge[jentry][ch]);

         }
      
         file << "\t\t" <<left<< setw(10) << new_ampl[jentry][ch] << "\t\t";

         //--------------------------------------------------------
         // PRINT
         //--------------------------------------------------------
         if (1) // 0 = no print; 1 = print.
         {
            cout << "\n-------------------------\n"                    << endl;
            cout << "Event                = " << jentry                << endl;
            cout << "Channel              = " << ch                    << endl;
            cout << "Minimum       [mV]   = " << min                   << endl;
            cout << "Min position  [a.u.] = " << w_min                 << endl;
            cout << "Baseline      [mV]   = " << baseline              << endl;
            cout << "Amplitude     [mV]   = " << ampl[jentry][ch]      << endl;
            cout << "NEW Amplitude [mV]   = " << new_ampl[jentry][ch]  << endl;
            cout << "Charge        [a.u.] = " << charge[jentry][ch]    << endl;
         }
      }
      file << endl;
   }
   file.close();
   
   //------------------------------
   // FIT
   //------------------------------
   for (int ch=2; ch<16; ch++)
   {  
      // AMPLITUDE
      h_ampl[ch]->Fit("landau", "Q");
      h_ampl[ch]->Draw("E");

      // MINIMUM POSITION
      h_w_min[ch]->Fit("gaus", "Q");
      h_w_min[ch]->Draw("E");

      // CHARGE
      h_q[ch]->Fit("landau", "Q");
      h_q[ch]->Draw("E");
   }   
   
   //------------------------------
   // SAVE HISROGRAMS IN ROOT FILE
   //------------------------------
   hfile.Write();
}
