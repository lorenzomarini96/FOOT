#include <vector>
#include <iostream>
#include "TROOT.h"
#include "TMath.h"
#include "TChain.h"


void tree_tofwall()
{
    // Open the TFile which contains the TTree
    TFile *file = new TFile("test4.root");
    file->ls();

    // Retrieve the TTree (via the name)
    TTree *rec = (TTree*)file->Get("rec");

    // Stampa le informazioni riassuntive del TTree
    rec->Print(); 

    // Create the appropriate variables to store the data in the leaves
    Double_t board166;
    Double_t waveform[18][1024];

    rec->SetBranchAddress("board166", &board166); //Branch name = "board166"
   
    // READ ALL ENTRIES
    //int nentries = rec->GetEntries();

    // CREATE HISTOGRAM
    TH1F *hist = new TH1F("hist", "Histogram", 200, 0.33, 0.45); 

    for (int n=0; n<rec->GetEntries(); n++) // LOOP ON ENTRIES
    {
        cout << "Event " << n << endl;
        //cout << waveform[2][1024][n] << endl; I NEED SOMETHING JUST LIKE THIS TO LOOP ON ENTRIES n
        //rec->GetEntry(n);
        float wave = 0.0;
        float baseline = 0.445;  // mV

        // COMPUTE MINIMUM of waveform[2]
        float minimo = waveform[2][0];
        for (int j=0; j<1024; j++) // LOOP ON COLUMNS OF waveform[2]
        {
            if (waveform[2][j] < minimo)
            {
                minimo = waveform[2][j];
            }
            
            // COMPUTE BASELINE (compute the mean values of the first 300 bins)          
            if (j < 300) // APPROX FIRST 1/3 OF 1024 SAMPLES
            {
                //double wave = wave + waveform[2][j];
                //cout << "wave = " << wave << endl;
                //baseline = wave/300; // [mV]
                //cout << "Baseline = " << baseline << endl;
            }
        }
        
        cout << "Evento " << n <<"\nminimo =" << minimo << endl;

        //baseline = wave/300; // [mV]
        //cout << "Baseline = " << baseline << endl;

        // COMPUTE AMPLITUDE (n-event)
        //float minimo = 0.20; // [mV]
        // (canale 2).
        float ampl = baseline - minimo;

        // FILL HISTOGRAM
        //hist->Fill(ampl);
        hist->Fill(minimo);
        
    }
    
    // DRAW HISTOGRAM
    hist->Draw();
    //cout << "# ENTRIES = "<< nentries << endl;
}



/*

COMMENTO: se provo a riavviare una sessione di root e a lanciare ili file b.C => i valori di 
waveform[2][x] sono tutti nulli. (Boh). L'unico modo che ho trovato per risolvere la cosa è inserire
la riga //rec->GetEntry(n); nel for loop sulle entri. A quel punto root va in crash, ma rilanciando
rilanciando il programma finalmente funziona e i valori di waveform sono quelli giusti.


*/