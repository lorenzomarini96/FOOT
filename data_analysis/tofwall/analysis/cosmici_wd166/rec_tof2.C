#define rec_cxx
#include "rec.h"
#include <TH2.h>
#include <TStyle.h>
#include <TCanvas.h>

void rec::Loop()
{
//   In a ROOT session, you can do:
//      root> .L rec_tof.C
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
   
   // LOOP ON CHANNELS 
   for (Int_t ch=0; ch<16; ch++)
   { 
      // CREATE CANVAS   
      TString canvas_title = Form("ch%d",ch); 
      TCanvas *c = new TCanvas(canvas_title, canvas_title, 600, 600);

      // CREATE HISTOGRAM
      TString hist_title = Form("WD166 - Ch%d",ch); 
      TH1F *hist = new TH1F(hist_title, hist_title, 100, -0.1, 0.4); 
      hist->GetXaxis()->SetTitle("Amplitude waveform [mV]");
      hist->GetYaxis()->SetTitle("Entries");

      // LOOP ON ENTRIES
      for (Long64_t jentry=0; jentry<nentries;jentry++) 
      {
         Long64_t ientry = LoadTree(jentry);
         if (ientry < 0) break;

         nb = fChain->GetEntry(jentry);   nbytes += nb;

         //--------------------------------------------------------
         // STEP 1
         float wave = 0; // [mV] INITIALIZE VALUE OF WAVEFORM
         float baseline = 0; // [mV] INITIALIZE BASELINE VALUE

         //--------------------------------------------------------
         // COMPUTE MINIMUM of waveform
         float minimo = board166_waveform[ch][0];

         for (int j=0; j<1024; j++) // LOOP ON COLUMNS OF board166_waveform[2]
         {
            if (board166_waveform[ch][j] < minimo)
            {
               minimo = board166_waveform[ch][j];
            }

            //--------------------------------------------------------
            // COMPUTE BASELINE (compute the mean values of the first 200 bins)      
            if (j <= 200) // APPROX FIRST 1/3 OF 1024 SAMPLES
            {  
               wave =  wave + board166_waveform[ch][j];
            }
         }

         // COMPUTE BASELINE (compute the mean values of the first 200 bins)
         baseline = wave/200; // [mV]
         //--------------------------------------------------------
         // COMPUTE AMPLITUDE [mV]
         float ampl = baseline - minimo;
            
         //--------------------------------------------------------
         // PRINT
         //--------------------------------------------------------
         cout << "\n--------------------------------\n" << endl;
         cout << "Channel        = " << ch       << endl;
         cout << "Evento         # " << jentry   << endl;
         cout << "Minimo    [mV] = " << minimo   << endl;
         cout << "Baseline  [mV] = " << baseline << endl;
         cout << "Amplitude [mV] = " << ampl     << endl;
         
         //--------------------------------------------------------
         // FILL HISTOGRAM
         hist->Fill(ampl);  
      }
      // DRAW HISTOGRAM
      //hist->Fit("landau");
      hist->Draw();
   } 
}



/*
COMMENTO GENERALE:

rec.C calcola, per ciascun canale della waveboard 166, e per ciascun evento rivelato nel TOFwall, 
il valore di ampiezza della waveform generata e che ha raggiunto il canale ch-esimo. 
Dopo di che si restrituisce gli istogrammi dei valori di ampiezza [in mV] per ciascun canale.

NB:
i plot sono riportati su 15 canvas differenti (una per canale). In questo modo è possibile lavorare 
su ciascuna di loro in maniera indipentende dalle altre.
/*