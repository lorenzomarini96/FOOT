//--------------------------------------------------------------------------------
// MACRO TO APPLY ON "cosmici_tot.root"
//--------------------------------------------------------------------------------
// Macro to make histogram of amplitude of the waveform of wavedream 165.
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
#include <math.h>

void rec::Loop()
{
   // GENERAL INFO
   Int_t WD = 165;
   char file_root[20] = "cosmici_tot.root";


   if (fChain == 0) return;
   Long64_t nentries = fChain->GetEntriesFast();
   Long64_t nbytes = 0, nb = 0;

   //------------------------------
   // FILE .ROOT TO SAVE HISTOGRAMS
   //------------------------------
   TFile hfile("WD165_v2.root","RECREATE");
   gStyle->SetOptFit(10111);

   //------------------------------
   // CANVAS AMPLLITUDE
   //------------------------------
   TCanvas *c_ampl = new TCanvas("c_ampl", "WD165 Amplitude", 600, 600);
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
      sprintf(title_ampl,"WD165 - Amplitue WF of ch%d",i);
      h_ampl[i] = new TH1F(name_ampl,title_ampl,40, -0.05, 0.5);
      h_ampl[i]->GetXaxis()->SetTitle("Amplitude waveform [mV]");
      h_ampl[i]->GetYaxis()->SetTitle("Entries");
      h_ampl[i]->SetMarkerStyle(20);
      h_ampl[i]->SetMarkerStyle(kFullCircle);
      h_ampl[i]->SetMarkerColor(kBlack);
      h_ampl[i]->SetFillColor(kCyan-9);
   }

   //------------------------------
   // CANVAS CHARGE POSITION
   //------------------------------
   TCanvas *c_q = new TCanvas("c_q", "WD165 Charge", 600, 600);
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
      sprintf(title_q,"WD165 - Charge WF of ch%d",i);
      h_q[i] = new TH1F(name_q,title_q,50, 0.0, 80);
      h_q[i]->GetXaxis()->SetTitle("Charge [a.u.]");
      h_q[i]->GetYaxis()->SetTitle("Entries");
      h_q[i]->SetMarkerStyle(20);
      h_q[i]->SetMarkerStyle(kFullCircle);
      h_q[i]->SetMarkerColor(kBlack);
      h_q[i]->SetFillColor(45);
   }

   //------------------------------
   // CANVAS TOT CHARGE
   //------------------------------
   TCanvas *c_q_tot = new TCanvas("c_q_tot", "WD165 TOT Charge", 600, 600);
   c_q_tot->SetTickx();
   c_q_tot->SetTicky();
   c_q_tot->SetLeftMargin(0.15);

   //------------------------------
   // HISTOGRAMS TOT CHARGE
   //------------------------------
   TH1F *h_q_tot[8];
   char name_q_tot[20];
   char title_q_tot[100];
   for (Int_t i=0;i<8;i++) 
   {
      sprintf(name_q_tot,"h_q_tot%d",i);
      sprintf(title_q_tot,"WD165 - TOT Charge bar%d",i);
      h_q_tot[i] = new TH1F(name_q_tot,title_q_tot,50, 0.0, 60);
      h_q_tot[i]->GetXaxis()->SetTitle("Charge [a.u.]");
      h_q_tot[i]->GetYaxis()->SetTitle("Entries");
      h_q_tot[i]->SetMarkerStyle(20);
      h_q_tot[i]->SetMarkerStyle(kFullCircle);
      h_q_tot[i]->SetMarkerColor(kBlack);
      h_q_tot[i]->SetFillColor(45);
   }

   //------------------------------
   // CANVAS MIN POSITION
   //------------------------------
   TCanvas *c_min = new TCanvas("c_min", "WD165 Min Position", 600, 600);
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
      sprintf(title_min,"WD165 - Min position of ch%d",i);
      h_w_min[i] = new TH1F(name_min,title_min,100, 250, 450);
      h_w_min[i]->GetXaxis()->SetTitle("# samples [u.m.]");
      h_w_min[i]->GetYaxis()->SetTitle("Entries");
      h_w_min[i]->SetMarkerStyle(20);
      h_w_min[i]->SetMarkerStyle(kFullCircle);
      h_w_min[i]->SetMarkerColor(kBlack);
      h_w_min[i]->SetFillColor(kGreen-9);
   }

   //------------------------------
   // FILES TO SAVE DATA
   //------------------------------
   fstream file;
   file.open("WD165_v2.txt", ios::out);

   fstream file_qbar;
   file_qbar.open("WD165_qbar.txt", ios::out);

   // NAME COLUMNS
   file << "n_event"<<left<<"\t";
   for (Int_t ch=0; ch<16; ch++)
   {
      file <<"\t" <<"ch" << ch <<left<< setw(14)<< "\t\t";
   }
   file << endl;

   // NAME COLUMNS
   for (Int_t b=0; b<8; b++)
   {
      file_qbar <<"\t" <<"#bar" << b <<left<< setw(14)<< "\t\t";
   }
   file_qbar << endl;

   //------------------------------
   // LOOP ON ENTRIES
   //------------------------------
   for (Long64_t jentry=0; jentry<nentries; jentry++)
   {
      Long64_t ientry = LoadTree(jentry);
      if (ientry < 0) break;

      nb = fChain->GetEntry(jentry);   nbytes += nb;

      // NAMEN RAWS
      //file << jentry << "\t";
      //file_qbar << jentry << "\t";

      cout << "\n*********************************************************************************************************************" << endl;
      cout << "WaveDREAM " << WD << endl;
      cout << left<<setw(15)<<"Event"<<left<<setw(15)<<"Ch"<<left<<setw(15)<<"Vbase[mV]"<<left<<setw(15)<<"Vmin[mV]"<<left<<setw(15)<<"Vampl[mV]"<<left<<setw(15)<<"Q_L[a.u.]"<<left<<setw(15)<<"Q_R[a.u.]"<<left<<setw(15)<<"Qtot[a.u.]"<<left<<setw(15)<<endl;
      cout << "*********************************************************************************************************************\n" << endl;

      //------------------------------
      // LOOP ON CHANNELS
      //------------------------------
      for (Int_t ch=0; ch<16; ch++)
      {
         //------------------------------
         // INITIALIZE VALUES
         //------------------------------
         Double_t wave = 0.;                         // [mV]   VALUE OF WAVEFORM
         Double_t w_min = 0;                         // [s]    COORESPONDING TIME VALUE OF MINIMUM
         Double_t baseline = 0.;                     // [mV]   BASELINE VALUE
         Double_t ampl[11236][16];                   // [mV]   AMPLITUDE VALUE
         Double_t new_ampl[11236][16];               // [mV]   AMPLITUDE VALUE (removed same values)
         Double_t charge[11236][16];                 // [a.u.] CHARGE MATRIX (removed same values)
         Double_t q = 0.;                            // [a.u.] CHARGE VALUE
         Double_t min = board165_waveform[ch][300];  // MIN VALUE (Usually located at 1/3 of range)
         Double_t q_bar_X_TOF[20];                   // ARRAY OF COLLECTED CHARGHE IN BAR OF X-VIEW
         Double_t q_bar_Y_TOF[20];                   // ARRAY OF COLLECTED CHARGHE IN BAR OF Y-VIEW
         //------------------------------
         // LOOP ON SAMPLES OF WAVEFORM
         //------------------------------
         for (Int_t w=10; w<=800; w++)
         {
            if (w > 50 && board165_waveform[ch][w] < min)
            {
               min = board165_waveform[ch][w];
               w_min = w;
            }
            // APPROX FIRST 1/3 OF 1024 SAMPLES
            if (w <= 150) {wave =  wave + board165_waveform[ch][w];}      
         }

         baseline = wave/140;                  // COMPUTE BASELINE  [mV] (mean values 150-10=140 bins)
         ampl[jentry][ch] = baseline - min;    // COMPUTE AMPLITUDE [mV]

         if (jentry > 0 && abs(ampl[jentry][ch]-ampl[jentry-1][ch])<0.00001) // SOLVE PROBLEM OF REPEATED WF?
         {  
            // DO NOT CONSIDER THE EVENT
            new_ampl[jentry][ch] = 0.;
            baseline             = 0.;
            min                  = 0.;
            w_min                = 0;
            q                    = 0.;
            charge[jentry][ch]   = 0.;
            q_bar_X_TOF[ch/2]    = 0.;
         }
         else
         {
            new_ampl[jentry][ch] = ampl[jentry][ch];

            for (Int_t w=250; w<570; w++)
            {
               //q += (board167_time[ch][w]-board167_time[ch][w-1])*ampl[jentry][ch];
               //q += ampl[jentry][ch];
               q += (baseline - board165_waveform[ch][w]);
            }
            charge[jentry][ch] = q;  // COMPUTE CHARGE [a.u.]
         
            //------------------------------
            // FILLING HISTOGRAMS
            //------------------------------
            if (new_ampl[jentry][ch] > 0. && charge[jentry][ch]>0.)
            {
               h_ampl[ch]->Fill(new_ampl[jentry][ch]);
               h_q[ch]->Fill(charge[jentry][ch]);   
            }
            //h_w_min[ch]->Fill(w_min);

            if (ch%2!=0 && charge[jentry][ch]>0.)
            {
               q_bar_X_TOF[ch/2] = sqrt(charge[jentry][ch] * charge[jentry][ch-1]); // COMPUTE TOT CHARGE COLLECTED IN A BAR
               h_q_tot[ch/2]->Fill(q_bar_X_TOF[ch/2]);
            }
         }
      
         file << "\t\t" <<left<< setw(10) << ampl[jentry][ch] << "\t\t";


         //--------------------------------------------------------
         // PRINT
         //--------------------------------------------------------
         if (ch%2==0)
         { 
            cout << left<< setw(15) << jentry <<left<< setw(15) << ch <<left<< setw(15) << baseline <<left<< setw(15) << min <<left<< setw(15) << new_ampl[jentry][ch]<< setw(15) << charge[jentry][ch] <<left<< setw(15) <<"   " <<left<< setw(15) << "   " <<left<< setw(15) << endl;
         }
         else
         {
            cout << left<< setw(15) << jentry <<left<< setw(15) << ch <<left<< setw(15) << baseline <<left<< setw(15) << min <<left<< setw(15) << new_ampl[jentry][ch]<< setw(15) << "   " <<left<< setw(15) << charge[jentry][ch] <<left<< setw(15) << q_bar_X_TOF[ch/2] <<left<< setw(15) << endl;
         }
        
        
         if (0) // 0 = no print; 1 = print.
         {
            cout << "\n-------------------------------\n"                << endl;
            cout << "# Event                = " << jentry                << endl;
            cout << "# Channel              = " << ch                    << endl;
            cout << "# Minimum       [mV]   = " << min                   << endl;
            cout << "# Min position  [a.u.] = " << w_min                 << endl;
            cout << "# Baseline      [mV]   = " << baseline              << endl;
            cout << "# Amplitude     [mV]   = " << new_ampl[jentry][ch]  << endl;
            if (ch%2==0){cout << "# Charge_L      [a.u.] = " << charge[jentry][ch] << endl;}
            else {cout << "# Charge_R      [a.u.] = " << charge[jentry][ch] << endl;}
            if (ch%2!=0)
            {
               cout << "\n******************************\n"          << endl;
               cout << "# Event             = " << jentry            << endl;
               cout << "# Charge TOT [a.u.] = " << q_bar_X_TOF[ch/2] << endl;
               cout << "\n******************************\n"          << endl;
            }
         }
      }// END LOOP ON CHANNEL
      file << endl;
   }// END LOOP ON ENTRIES
   file.close();
   
   //------------------------------
   // FIT
   //------------------------------
   for (int ch=0; ch<16; ch++)
   {  
      // AMPLITUDE
      h_ampl[ch]->Draw("E");
      h_ampl[ch]->Fit("landau", "Q");
      //h_ampl[ch]->Draw("E");

      // MINIMUM POSITION
      //h_w_min[ch]->Fit("gaus", "Q");
      //h_w_min[ch]->Draw("E");

      // CHARGE
      h_q[ch]->Fit("landau", "Q");
      h_q[ch]->Draw("E");

      if (ch%2!=0)
      {
         // TOT CHARGE
         h_q_tot[ch/2]->Fit("landau", "Q");
         h_q_tot[ch/2]->Draw("E");
      }
   }   
   
   //------------------------------
   // SAVE HISROGRAMS IN ROOT FILE
   //------------------------------
   hfile.Write();
}
