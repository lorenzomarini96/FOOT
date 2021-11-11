///////////////////////////////////////////////////////////////////////
// hit_map_10nov_slim.C
// Macro to reconstruct the hit map of the tofwall detector.
// Date: 11 November 2021
// Author: Lorenzo Marini
///////////////////////////////////////////////////////////////////////

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

   // HISTOGRAM
   TH2D *hist = new TH2D("hist", "Hit-map Test Cosmic rays", 20, -0.5, 19.5, 20, 19.5, 39.5);

   // LOOP ON ENTRIES
   for (Long64_t jentry=0; jentry<nentries; jentry++) {

      Long64_t ientry = LoadTree(jentry);
      if (ientry < 0) break;
      nb = fChain->GetEntry(jentry);   nbytes += nb;
      
      // INIT VALUES
      Int_t bar_TOF_X = -1;
      Int_t bar_TOF_Y = -1;

      //**************************************
      // WD 165 - X VIEW (BAR 0 TO 7)
      
      for (Int_t ch=0; ch<16; ch++) {
         
         if (board165_hit[ch] == 1) {

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
         }
      }
      
      //**************************************
      // WD 166 - X VIEW (BAR 8 TO 10); Y VIEW (BAR 28 to 30)

      for (Int_t ch=0; ch<12; ch++) {

         if (board166_hit[ch] == 1) {
         
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
         }  
      }

      //**************************************
      // WD 170 - X VIEW (BAR 11 TO 18)

      for (Int_t ch=0; ch<16; ch++) {

         if (board170_hit[ch] == 1) {

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
         }
      }

      //**************************************
      // WD 167 - X VIEW (BAR 19); Y VIEW (BAR 20 TO 26)

      for (Int_t ch=0; ch<16; ch++) {
      
         if (board167_hit[ch] == 1) {
               
            if      (ch==0) bar_TOF_X = 19;
            else if (ch==1) bar_TOF_X = 19;

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
         }
      } 

      //**************************************
      // WD 168 - Y VIEW (BAR 27 ANS 31 TO 37)

      for (Int_t ch=0; ch<16; ch++) {

         if (board168_hit[ch] == 1) {
            
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
         }
      }

      //**************************************
      // WD 158 - Y VIEW (BAR 38 TO 39)

      for (Int_t ch=0; ch<4; ch++) { 

         if (board158_hit[ch] == 1) {

            if (ch==0)      bar_TOF_Y = 38;
            else if (ch==1) bar_TOF_Y = 38;
            else if (ch==2) bar_TOF_Y = 39;
            else if (ch==3) bar_TOF_Y = 39;
         }
      }

      // FILL THE HISTOGRAM
      if (bar_TOF_X > -1 && bar_TOF_Y > -1) hist->Fill(bar_TOF_X, bar_TOF_Y);
   }

   // CANVAS
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
}
