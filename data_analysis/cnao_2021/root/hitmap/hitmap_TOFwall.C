///////////////////////////////////////////////////////////////////////
// hit_map_10nov_slim.C
// Macro to reconstruct the hit map of the tofwall detector.
// Date: 22 November 2021
// Author: L. Marini
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


   //FILE .ROOT TO SAVE HISTOGRAM
   //TFile hfile("hitmap_hist.root", "RECREATE");
   
   //gStyle->SetOptStat(0);

   // HISTOGRAM
   TH2D *hist = new TH2D("hist", "Hit-map", 20, -0.5, 19.5, 20, 19.5, 39.5);
   TH1D *hist_project = new TH1D("hist_project", "Hit-map XY projections", 40, -0.5, 39.5);

   // INITIALIZE ARRAY
   Int_t Bar_TOF_X[20];
   Int_t Bar_TOF_Y[20];
   Int_t Bar_TOF[40];
   
   // LOOP ON ENTRIES
   for (Long64_t jentry=0; jentry<nentries/2; jentry++) 
   {
      Long64_t ientry = LoadTree(jentry);
      if (ientry < 0) break;
      nb = fChain->GetEntry(jentry);   nbytes += nb;
      
      // INIT VALUES
      memset(Bar_TOF_X, 0, 20*sizeof(Int_t));
      memset(Bar_TOF_Y, 0, 20*sizeof(Int_t));
      memset(Bar_TOF,   0, 40*sizeof(Int_t));
      
      Int_t status = 1;

      //**************************************
      // WD 165 - X VIEW (BAR 0 TO 7)
      
      for(int b=0; b<8; ++b)
      {
         if(board165_hit[2*b] && board165_hit[2*b+1])
         {
	    if(b==0)
	    {
	      Bar_TOF_X[0] = 1;
	      Bar_TOF[0]   = 1;
	    }
            else if(b==1)
	    {
	      Bar_TOF_X[1] = 1;
	      Bar_TOF[1]   = 1;
	    }
            else if(b==2)
	    {
	      Bar_TOF_X[2] = 1;
	      Bar_TOF[2]   = 1;
	    }
            else if(b==3)
	    {
	      Bar_TOF_X[3] = 1;
	      Bar_TOF[3]   = 1;
	    }
            else if(b==4)
	    {
	      Bar_TOF_X[4] = 1;
	      Bar_TOF[4]   = 1;
	    }
            else if(b==5)
	    {
	      Bar_TOF_X[5] = 1;
	      Bar_TOF[5]   = 1;
	    }
            else if(b==6)
	    {
	      Bar_TOF_X[6] = 1;
	      Bar_TOF[6]   = 1;
	    }
            else if(b==7)
	    {
	      Bar_TOF_X[7] = 1;
	      Bar_TOF[7]   = 1;
	    }
         }
      }
      
      //**************************************
      // WAVEDREAM 166 - X VIEW (BAR 8 TO 10); Y VIEW (BAR 28 to 30)
      
      // LOOP ON CHANNELS OF WAVEDREAM
      for (Int_t ch=0; ch<12; ch++) 
      {
       	status = 1;
	// 1° CHECK SIGNAL
        // FIND MIN AND MAX OF WAVEFORM
        Double_t min_166 = board166_waveform[ch][100]; // V AMPLITUDE MIN OF CHANNEL [V]
        Double_t max_166 = board166_waveform[ch][100]; // V AMPLITUDE MAX OF CHANNEL [V]
            
        for (Int_t t=100; t<900; t++) 
        { // LOOP ON SAMPLES OF WAVEFORM
           if (board166_waveform[ch][t] < min_166) min_166 = board166_waveform[ch][t];
           if (board166_waveform[ch][t] > max_166) max_166 = board166_waveform[ch][t];
        }
            
	if (max_166 - min_166 > 0.95) status = 0;  // NOISE
            
        if (max_166 - min_166 < 0.010) status = 0; // REMEMBER ZEROSUPPRESSION
               
        if (status==0) board166_hit[ch] = 0;
      
      } // CHANNEL LOOP
   
      for(int b=0; b<6; ++b)
      {
         if(board166_hit[2*b] && board166_hit[2*b+1])
         {
	   if(b==0)
	   {
	     Bar_TOF_X[8]  = 1;
	     Bar_TOF[8]    = 1;
	   }
	   else if(b==1)
	   {
	     Bar_TOF_X[9]  = 1;
	     Bar_TOF[9]    = 1;
	   } 
	   else if(b==2)
	   {
	     Bar_TOF_X[10] = 1;
	     Bar_TOF[10]   = 1;
	   }	   
	   if(b==3)
	   {
	     Bar_TOF_Y[8]  = 1;
	     Bar_TOF[28]   = 1;
	   }
	   else if(b==4)
	   {
	     Bar_TOF_Y[9]  = 1;
	     Bar_TOF[29]  = 1;
	   }
	   else if(b==5)
	   {
	     Bar_TOF_Y[10] = 1;
	     Bar_TOF[30]   = 1;
	   }
         }
      }
   
      //**************************************
      // WD 170 - X VIEW (BAR 11 TO 18)

      for(int b=0; b<8; ++b)
      {
         if(board170_hit[2*b] && board170_hit[2*b+1])
         {
	   if(b==0)
	   {
	     Bar_TOF_X[11] = 1;
	     Bar_TOF[11]   = 1;
	   }
	   else if(b==1)
	   {
	     Bar_TOF_X[12] = 1;
	     Bar_TOF[12]   = 1;
	   }
	   else if(b==2)
	   {
	     Bar_TOF_X[13] = 1;
	     Bar_TOF[13]   = 1;
	   }
	   else if(b==3)
	   {
	     Bar_TOF_X[14] = 1;
	     Bar_TOF[14]   = 1;
	   }
	   else if(b==4)
	   {
	     Bar_TOF_X[15] = 1;
	     Bar_TOF[15]   = 1;
	   }
	   else if(b==5)
	   {
	     Bar_TOF_X[16] = 1;
	     Bar_TOF[16]   = 1;
	   }
	   else if(b==6)
	   {
	     Bar_TOF_X[17] = 1;
	     Bar_TOF[17]   = 1;
	   }
	   else if(b==7)
	   {
	     Bar_TOF_X[18] = 1;
	     Bar_TOF[18]   = 1;
	   }
	 }
      }
   
      //**************************************
      // WD 167 - X VIEW (BAR 19); Y VIEW (BAR 20 TO 26)

      for(int b=0; b<8; ++b)
      {
         if(board167_hit[2*b] && board167_hit[2*b+1])
         {
	   if(b==0)
	   {
	     Bar_TOF_X[19] = 1;
	     Bar_TOF[19]   = 1;
	   }
	   else if(b==1)
	   {
	     Bar_TOF_Y[0] = 1;
	     Bar_TOF[20]  = 1;
	   }
	   else if(b==2)
	   {
	     Bar_TOF_Y[1] = 1;
	     Bar_TOF[21]  = 1;
	   }
	   else if(b==3)
	   {
	     Bar_TOF_Y[2] = 1;
	     Bar_TOF[22]  = 1;
	   }
	   else if(b==4)
	   {
	     Bar_TOF_Y[3] = 1;
	     Bar_TOF[23]  = 1;
	   }
	   else if(b==5)
	   {
	     Bar_TOF_Y[4] = 1;
	     Bar_TOF[24]  = 1;
	   }
	   else if(b==6)
	   {
	     Bar_TOF_Y[5] = 1;
	     Bar_TOF[25]  = 1;
	   }
	   else if(b==7)
	   {
	     Bar_TOF_Y[6] = 1;
	     Bar_TOF[26]  = 1;
	   }
	 }
      }

      //**************************************
      // WD 168 - Y VIEW (BAR 27 AND 31 TO 37)

      for(int b=0; b<8; ++b)
      {
         if(board168_hit[2*b] && board168_hit[2*b+1])
         {
	   if(b==0)
	   {
	     Bar_TOF_Y[7]  = 1;
	     Bar_TOF[27]   = 1;
	   }
	   else if(b==1)
	   {
	     Bar_TOF_Y[11] = 1;
	     Bar_TOF[31]   = 1;
	   }
	   else if(b==2)
	   {
	     Bar_TOF_Y[12] = 1;
	     Bar_TOF[32]   = 1;
	   }
	   else if(b==3)
	   {
	     Bar_TOF_Y[13] = 1;
	     Bar_TOF[33]   = 1;
	   }
	   else if(b==4)
	   {
	     Bar_TOF_Y[14] = 1;
	     Bar_TOF[34]   = 1;
	   }
	   else if(b==5)
	   {
	     Bar_TOF_Y[15] = 1;
	     Bar_TOF[35]   = 1;
	   }
	   else if(b==6)
	   {
	     Bar_TOF_Y[16] = 1;
	     Bar_TOF[36]   = 1;
	   }
	   else if(b==7)
	   {
	     Bar_TOF_Y[17] = 1;
	     Bar_TOF[37]   = 1;
	   }
         }
      }

      //**************************************
      // WD 158 - Y VIEW (BAR 38 TO 39)

      for(int b=0; b<2; ++b)
      {
         if(board158_hit[2*b] && board158_hit[2*b+1])
         {
	   if(b==0)
	   {
	     Bar_TOF_Y[18] = 1;
	     Bar_TOF[38]   = 1;
	   }
	   else if(b==1)
	   {
	     Bar_TOF_Y[19] = 1;
	     Bar_TOF[39]   = 1;
	   }
         }
      }

      // FILL THE HISTOGRAM 2D
      for (int i=0; i<20; ++i)
      {
	if (Bar_TOF_X[i] == 0) continue;
            for (int j=0; j<20; j++)
	    {
	      if (Bar_TOF_Y[j] == 0) continue;
	      hist->Fill(i, 20+j);
	    }
       }

      // FILL THE HISTOGRAM 1D
      for (int b=0; b<40; b++)
      {
	if (Bar_TOF[b] == 0) continue;
	hist_project->Fill(b);
      }

      
   } // END LOOP ON ENTRIES

   

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
   c1->SetLogz();
   hist->Draw("colz TEXT0 SAME");
   c1->SaveAs("hitmap_hist2d.pdf");
   

   TCanvas *c2 = new TCanvas("c2", "hitmap_projection",600,600);
   c2->SetTickx();
   c2->SetTicky();
   c2->SetLeftMargin(0.15);
   hist_project->GetXaxis()->SetTitle("Bar");
   hist_project->GetYaxis()->SetTitle("Entries");
   c2->SetLogy();
   hist_project->SetNdivisions(20,"X");
   hist_project->Draw("TEXT0 hist");
   c2->SaveAs("hitmap_hist_XY_proj.pdf");
   //hfile.Write();
}
