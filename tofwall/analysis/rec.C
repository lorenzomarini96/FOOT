#define rec_cxx
#include "rec.h"
#include <TH2.h>
#include <TStyle.h>
#include <TCanvas.h>

void rec::Loop()
{
//   In a ROOT session, you can do:
//      root> .L rec.C
//      root> rec t
//      root> t.GetEntry(12); // Fill t data members with entry number 12
//      root> t.Show();       // Show values of entry 12
//      root> t.Show(16);     // Read and show values of entry 16
//      root> t.Loop();       // Loop on all entries
//

//    This is the loop skeleton where:
//    jentry is the global entry number in the chain
//    ientry is the entry number in the current Tree
//    Note that the argument to GetEntry must be:
//    jentry for TChain::GetEntry
//    ientry for TTree::GetEntry and TBranch::GetEntry
//
//    To read only selected branches, Insert statements like:
//    METHOD1:
//    fChain->SetBranchStatus("*",0);  // disable all branches
//    fChain->SetBranchStatus("branchname",1);  // activate branchname
//    METHOD2: replace line
//    fChain->GetEntry(jentry);       //read all branches
//    by  b_branchname->GetEntry(ientry); //read only this branch

   if (fChain == 0) return;

   Long64_t nentries = fChain->GetEntriesFast();

   Long64_t nbytes = 0, nb = 0;
   
   //--------------------------------------------------------
   // CREATE HISTOGRAM
   // Canvas
   TCanvas *c1 = new TCanvas();
   TH1F *hist = new TH1F("hist", "Waveboard 166 channel 2", 100, -0.1, 0.4); 
   hist->GetXaxis()->SetTitle("Amplitude waveform [mV]");
   hist->GetYaxis()->SetTitle("Entries");
   //hist->SetMarkerStyle(20);
   //hist->SetMarkerStyle(kFullCircle);
   //hist->SetMarkerColor(kBlack);

   for (Long64_t jentry=0; jentry<nentries;jentry++) 
   {
      //cout << "jentry" << jentry << endl;
      Long64_t ientry = LoadTree(jentry);
      if (ientry < 0) break;

      nb = fChain->GetEntry(jentry);   nbytes += nb;
      // if (Cut(ientry) < 0) continue;

      //--------------------------------------------------------
      // STEP 1
      float wave = 0; // [mV] INITIALIZE VALUE OF WAVEFORM[2][j]
      float baseline = 0; // [mV] INITIALIZE BASELINE VALUE

      //--------------------------------------------------------
      // COMPUTE MINIMUM of waveform
      float minimo = board166_waveform[2][0];

      for (int j=0; j<1024; j++) // LOOP ON COLUMNS OF board166_waveform[2]
      {
         if (board166_waveform[2][j] < minimo)
         {
            minimo = board166_waveform[2][j];
         }

         //--------------------------------------------------------
         // COMPUTE BASELINE (compute the mean values of the first 200 bins)      
         if (j < 200) // APPROX FIRST 1/3 OF 1024 SAMPLES
         {  
            wave =  wave + board166_waveform[2][j];
         }
      }
      // COMPUTE BASELINE (compute the mean values of the first 200 bins)
      baseline = wave/200; // [mV]

      //--------------------------------------------------------
      // COMPUTE AMPLITUDE [mV]
      float ampl = baseline - minimo;
      
      /*
      cout << "\n--------------------------------\n" << jentry << endl;
      cout << "Evento     # "     << jentry   << endl;
      cout << "Minimo    [mV] = " << minimo   << endl;
      cout << "Baseline  [mV] = " << baseline << endl;
      cout << "Amplitude [mV] = " << ampl     << endl;
      */

      //--------------------------------------------------------
      // FILL HISTOGRAM
      hist->Fill(ampl);      
   }
   // DRAW HISTOGRAM
   hist->Draw();
   //hist->Draw("E"); TO ADD ERROR BARS
}
