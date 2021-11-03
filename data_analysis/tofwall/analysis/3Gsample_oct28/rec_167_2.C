//--------------------------------------------------------------------------------
// MACRO DA APPLICARE SU "test_tot.root"
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
//          3.4) Fille the histogram of amplitude
//       4) Landau Fit on the distribution
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
   TCanvas *c = new TCanvas("c", "WD167 Amplitude", 600, 600);

   //------------------------------
   // HISTOGRAMS AMPLITUDE
   //------------------------------
   TH1F *h[12];
   char name[20];
   char title[100];
   for (Int_t i=0;i<12;i++) 
   {
      sprintf(name,"h%d",i);
      sprintf(title,"WD167 - Amplitue WF of ch%d",i);
      h[i] = new TH1F(name,title,40, -0.05, 0.5);
      h[i]->GetXaxis()->SetTitle("Amplitude waveform [mV]");
      h[i]->GetYaxis()->SetTitle("Entries");
      h[i]->SetFillColor(kCyan-9);
   }

   //------------------------------
   // CANVAS MIN POSITION
   //------------------------------
   TCanvas *c_min = new TCanvas("c_min", "WD167 Min Position", 600, 600);

   //------------------------------
   // HISTOGRAMS MIN POSITION
   //------------------------------
   TH1F *h_w_min[12];
   char name_min[20];
   char title_min[100];
   for (Int_t i=0;i<12;i++) 
   {
      sprintf(name_min,"h_min%d",i);
      sprintf(title_min,"WD167 - Min position of ch%d",i);
      h_w_min[i] = new TH1F(name_min,title_min,100, 250, 450);
      h_w_min[i]->GetXaxis()->SetTitle("# samples [u.m.]");
      h_w_min[i]->GetYaxis()->SetTitle("Entries");
      h_w_min[i]->SetFillColor(kGreen-9);
   }

   //------------------------------
   // FILE TO SAVE DATA
   //------------------------------
   fstream file;
   file.open("WD167.txt", ios::out);

   // NAME COLUMNS
   file << "n_event"<<left<<"\t";
   for (Int_t ch=0; ch<12; ch++)
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
      for (Int_t ch=0; ch<12; ch++)
      {
         float wave = 0.;           // [mV] INITIALIZE VALUE OF WAVEFORM
         float w_min = 0;           // [s]  INITIALIZE COORESPONDING TIME VALUE OF MINIMUM
         float baseline = 0.;       // [mV] INITIALIZE BASELINE VALUE
         float ampl[5005][12];      // [mV] INITIALIZE AMPLITUDE VALUE
         float new_ampl[5005][12];  // [mV] INITIALIZE AMPLITUDE VALUE (removed same values)

         // COMPUTE MINIMUM OF WAVEFORM
         float minimo = board167_waveform[ch][300]; // (Usually located at 1/3 of range)

         for (Int_t w=10; w<=1023; w++) // LOOP ON COLUMNS OF board167_waveform[ch] //NB: use 1023 instead 1024 (problem with last component!)
         {
            if (w > 50 && board167_waveform[ch][w] < minimo)
            {
               minimo = board167_waveform[ch][w];
               w_min = w;
            }
            // APPROX FIRST 1/3 OF 1024 SAMPLES
            if (w <= 170) {wave =  wave + board167_waveform[ch][w];}      
         }
         
         //------------------------------
         // FILLING HISTOGRAMS
         //------------------------------
         //h_w_min->Fill(w_min);
         //h_w_min->Fit("gaus", "Q");

         baseline = wave/170;                  // COMPUTE BASELINE [mV] (mean values of the first 200 bins)
         ampl[jentry][ch] = baseline - minimo; // COMPUTE AMPLITUDE [mV]
         
         //------------------------------
         // FILLING HISTOGRAMS
         //------------------------------
         if (jentry > 0 && ampl[jentry][ch]-ampl[jentry-1][ch]<0.001)
         {
            new_ampl[jentry][ch] = 0.;
         }
         else
         {
            new_ampl[jentry][ch] = ampl[jentry][ch];
            h[ch]->Fill(new_ampl[jentry][ch]);
         
            h_w_min[ch]->Fill(w_min);
            //cout << "new_ampl["<<jentry<<"]["<<ch<< "] = " << new_ampl[jentry][ch] << endl;
         }
      
         file << "\t\t" <<left<< setw(10) << new_ampl[jentry][ch] << "\t\t";

         //--------------------------------------------------------
         // PRINT
         //--------------------------------------------------------
         if (1) // 0 = no print; 1 = print.
         {
            cout << "\n-------------------------\n"          << endl;
            cout << "Event          = " << jentry            << endl;
            cout << "Channel        = " << ch                << endl;
            cout << "Minimum   [mV] = " << minimo            << endl;
            cout << "Min position   = " << w_min             << endl;
            cout << "Baseline  [mV] = " << baseline          << endl;
            cout << "Amplitude [mV] = " << ampl[jentry][ch]  << endl;
            cout << "NEW Amplitude [mV] = " << new_ampl[jentry][ch]  << endl;
         }
      }
      file << endl;
   }
   file.close();
   
   //------------------------------
   // FIT
   //------------------------------
   for (Int_t ch=2; ch<12; ch++)
   {
      h[ch]->Fit("landau", "Q");
      h_w_min[ch]->Fit("gaus", "Q");
   }   
   
   //------------------------------
   // SAVE HISROGRAMS IN ROOT FILE
   //------------------------------
   hfile.Write();
}
