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
   //Double_t board_waveform[18][11236] = board165_waveform[18][1024];
   /*
   if (WD_name=="WD157"){board_waveform[18][1024] = board157_waveform[18][1024];}
   else if (WD_name=="WD158"){board_waveform[18][1024] = board158_waveform[18][1024];}
   else if (WD_name=="WD165"){board_waveform[18][1024] = board165_waveform[18][1024];}
   else if (WD_name=="WD166"){board_waveform[18][1024] = board166_waveform[18][1024];}
   else if (WD_name=="WD167"){board_waveform[18][1024] = board167_waveform[18][1024];}
   else if (WD_name=="WD168"){board_waveform[18][1024] = board168_waveform[18][1024];}
   */
   // GENERAL INFO
   Int_t WD = 165;
   char file_root[20] = "cosmici_tot.root";

   if (fChain == 0) return;
   Long64_t nentries = fChain->GetEntriesFast();
   Long64_t nbytes = 0, nb = 0;

   //------------------------------
   // FILE .ROOT TO SAVE HISTOGRAMS
   //------------------------------
   //TFile hfile("WD165_v2.root","RECREATE");
   gStyle->SetOptFit(10111);

   
   //****************************************************************
   // HISTOGRAMS
   //****************************************************************

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
      h_ampl[i] = new TH1F(name_ampl,title_ampl,40, 0., 0.5);
      h_ampl[i]->GetXaxis()->SetTitle("Amplitude waveform [mV]");
      h_ampl[i]->GetYaxis()->SetTitle("Entries");
   }

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
   }

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
         Double_t min = board165_waveform[ch][300];     // MIN VALUE (Usually located at 1/3 of range)
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
               h_w_min[ch]->Fill(w_min);
            }
            if (ch%2!=0 && charge[jentry][ch]>0.)
            {
               q_bar_X_TOF[ch/2] = sqrt(charge[jentry][ch] * charge[jentry][ch-1]); // COMPUTE TOT CHARGE COLLECTED IN A BAR
               h_q_tot[ch/2]->Fill(q_bar_X_TOF[ch/2]);
            }
         }
      
         file << "\t\t" <<left<< setw(10) << ampl[jentry][ch] << "\t\t";

         //------------------------------
         // PRINT
         //------------------------------
         if (ch%2==0)
         { 
            cout << left<< setw(15) << jentry <<left<< setw(15) << ch <<left<< setw(15) << baseline <<left<< setw(15) << min <<left<< setw(15) << new_ampl[jentry][ch]<< setw(15) << charge[jentry][ch] <<left<< setw(15) <<"   " <<left<< setw(15) << "   " <<left<< setw(15) << endl;
         }
         else
         {
            cout << left<< setw(15) << jentry <<left<< setw(15) << ch <<left<< setw(15) << baseline <<left<< setw(15) << min <<left<< setw(15) << new_ampl[jentry][ch]<< setw(15) << "   " <<left<< setw(15) << charge[jentry][ch] <<left<< setw(15) << q_bar_X_TOF[ch/2] <<left<< setw(15) << endl;
         }
      }// END LOOP ON CHANNEL
      file << endl;
   }// END LOOP ON ENTRIES
   file.close();
   
   //****************************************************************
   // HISTOGRAMS AND FITS
   //****************************************************************
   for (int ch=0; ch<16; ch++)
   {  
      //------------------------------
      // AMPLITUDE
      //------------------------------
      TString canvas_title_ampl = Form("c_ampl_ch%d",ch); 
      TCanvas *c_ampl = new TCanvas(canvas_title_ampl, canvas_title_ampl, 600, 600);
      c_ampl->SetTickx();
      c_ampl->SetTicky();
      c_ampl->SetLeftMargin(0.15);
      h_ampl[ch]->SetMarkerStyle(20);
      h_ampl[ch]->SetMarkerStyle(kFullCircle);
      h_ampl[ch]->SetMarkerColor(kBlack);
      h_ampl[ch]->Fit("landau", "Q");
      h_ampl[ch]->Draw("E");
      // SAVE FIG
      //char name_fig_ampl[20];
      //sprintf(name_fig_ampl,"PLOTS/165/ampl_ch%d.pdf",ch);
      //c_ampl->SaveAs(name_fig_ampl);
      //c_ampl->SaveAs("PLOTS/165/ampl_ch%d",ch);
      
      //------------------------------
      // MINIMUM POSITION
      //------------------------------
      TString canvas_title_w_min = Form("w_min_ch%d",ch); 
      TCanvas *c_w_min = new TCanvas(canvas_title_w_min, canvas_title_w_min, 600, 600);
      c_w_min->SetTickx();
      c_w_min->SetTicky();
      c_w_min->SetLeftMargin(0.15);
      h_w_min[ch]->SetMarkerStyle(20);
      h_w_min[ch]->SetMarkerStyle(kFullCircle);
      h_w_min[ch]->SetMarkerColor(kBlack);
      h_w_min[ch]->Fit("gaus", "Q");
      h_w_min[ch]->Draw("E");
      // SAVE FIG
      //char name_fig_w_min[20];
      //sprintf(name_fig_w_min,"PLOTS/165/w_min_ch%d.pdf",ch);
      //c_w_min->SaveAs(name_fig_w_min);
      //c_w_min->SaveAs("PLOTS/165/w_min_ch%d",ch);
      
      //------------------------------
      // CHARGE
      //------------------------------
      TString canvas_title_q = Form("c_q_ch%d",ch); 
      TCanvas *c_q = new TCanvas(canvas_title_q, canvas_title_q, 600, 600);      
      c_q->SetTickx();
      c_q->SetTicky();
      c_q->SetLeftMargin(0.15);
      h_q[ch]->SetMarkerStyle(20);
      h_q[ch]->SetMarkerStyle(kFullCircle);
      h_q[ch]->SetMarkerColor(kBlack);
      h_q[ch]->Fit("landau", "Q");
      h_q[ch]->Draw("E");
      // SAVE FIG
      //char name_fig_q[20];
      //sprintf(name_fig_q,"PLOTS/165/q_ch%d.pdf",ch);
      //c_q->SaveAs(name_fig_q);

      //------------------------------
      // TOT CHARGE
      //------------------------------
      if (ch%2!=0)
      {
         TString canvas_title_q_tot = Form("c_q_tot_bar%d",ch/2); 
         TCanvas *c_q_tot = new TCanvas(canvas_title_q_tot, canvas_title_q_tot, 600, 600);      
         c_q_tot->SetTickx();
         c_q_tot->SetTicky();
         c_q_tot->SetLeftMargin(0.15);
         h_q_tot[ch/2]->SetMarkerStyle(20);
         h_q_tot[ch/2]->SetMarkerStyle(kFullCircle);
         h_q_tot[ch/2]->SetMarkerColor(kBlack);
         h_q_tot[ch/2]->Fit("landau", "Q");
         h_q_tot[ch/2]->Draw("E");
         // SAVE FIG
         //char name_fig_q_tot[20];
         //sprintf(name_fig_q_tot,"PLOTS/165/q_tot_ch%d.pdf",ch);
         //c_q_tot->SaveAs(name_fig_q_tot);
      }  
   }
   
   //------------------------------
   // SAVE HISROGRAMS IN ROOT FILE
   //------------------------------
   //hfile.Write();
}
