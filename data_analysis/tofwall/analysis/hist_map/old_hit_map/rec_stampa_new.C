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

   //**************************************
   // CANVAS
   //**************************************
   TCanvas *c1 = new TCanvas();
   c1->SetTickx();
   c1->SetTicky();
   c1->SetLeftMargin(0.15);

   //**************************************
   // HISTOGRAM
   //**************************************
   
   TH2D *hist = new TH2D("hist", "HISTOGRAM", 20, 0, 20, 20, 20, 40);

   // LOOP ON ENTRIES 
   for (Long64_t jentry=0; jentry<nentries;jentry++) {
      Long64_t ientry = LoadTree(jentry);
      if (ientry < 0) break;
      nb = fChain->GetEntry(jentry);   nbytes += nb;

      /*
      cout << "**************************" << endl;
      cout << "Entry " << jentry << endl;
      cout << "**************************" << endl;
      */

      Int_t bar_TOF_X = -1;
      Int_t bar_TOF_Y = -1;


      //********************************************************************
      // WAVEDREAM 165 - X VIEW (BAR 0 TO 7)
      //********************************************************************   
      
      // LOOP ON CHANNELS OF WAVEDREAM
      for (Int_t ch=0; ch<16; ch++){
   
         // INITIALIZE VALUES
         Double_t voltage_165 = 0.;                         
         Double_t baseline_165 = 0.;                     
         Double_t ampl_165[nentries][16];
         Double_t min_165 = board165_waveform[ch][300];  
         
         // LOOP ON SAMPLES OF WAVEFORM
         for (Int_t v=10; v<800; v++){
         
            if (v > 50 && board165_waveform[ch][v] < min_165){min_165 = board165_waveform[ch][v];}
            // APPROX FIRST 1/3 OF 1024 SAMPLES
            if (v <= 150) {voltage_165 =  voltage_165 + board165_waveform[ch][v];}      
         }
         baseline_165 = voltage_165/140;

         // AMPLITUDE (EVENTO J-ESIMO, CANALE CH)    
         ampl_165[jentry][ch] = baseline_165 - min_165;  
         
         if (board165_hit[ch] == 1)
         {
            // BAR TOFWALL
            if (1)
            {
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
               /*
               cout << "WD 165" << endl;
               cout << "V_ampl["<<jentry<<"]["<<ch<<"] = " << ampl_165[jentry][ch] << endl;
               cout << "Barra = " << bar_TOF_X << endl;
               cout << "------------------------------\n" << endl;
               */
            }
         }
      }

      //********************************************************************
      // WAVEDREAM 166 - X VIEW (BAR 8 TO 10)
      //********************************************************************

      for (Int_t ch=0; ch<6; ch++){

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
               /*
               cout << "WD 166" << endl;
               cout << "V_ampl["<<jentry<<"]["<<ch<<"] = " << ampl_166[jentry][ch] << endl;
               cout << "Barra = " << bar_TOF_X << endl;
               cout << "------------------------------\n" << endl;
               */
            }                    
         }  
      }// END LOOP ON CHANNEL

      //********************************************************************
      // WAVEDREAM 157 - X VIEW (BAR 11 TO 18)
      //********************************************************************

      for (Int_t ch=0; ch<16; ch++){

         // INITIALIZE VALUES
         Double_t voltage_157 = 0.;                         
         Double_t baseline_157 = 0.;                     
         Double_t ampl_157[nentries][16];                   
         Double_t min_157 = board157_waveform[ch][300];  
         
         // LOOP ON SAMPLES OF WAVEFORM
         for (Int_t v=10; v<800; v++){
         
            if (v > 50 && board157_waveform[ch][v] < min_157){min_157 = board157_waveform[ch][v];}
            // APPROX FIRST 1/3 OF 1024 SAMPLES
            if (v <= 150) {voltage_157 =  voltage_157 + board157_waveform[ch][v];}      
         }
         baseline_157 = voltage_157/140;

         // AMPLITUDE (EVENTO J-ESIMO, CANALE CH)    
         ampl_157[jentry][ch] = baseline_157 - min_157;  
         
         if (board157_hit[ch] == 1)
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
               cout << "WD 157" << endl;
               cout << "V_ampl["<<jentry<<"]["<<ch<<"] = " << ampl_157[jentry][ch] << endl;
               cout << "Barra = " << bar_TOF_X << endl;
               cout << "------------------------------\n" << endl;
               */
            }
         }
      }// END LOOP ON CHANNEL

      //********************************************************************
      // WAVEDREAM 158 - X VIEW (BAR 19)
      //********************************************************************

      for (Int_t ch=4; ch<6; ch++){

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
         {
            if (1)
            {
               if (ch==4) bar_TOF_X = 19;
               else if (ch==5) bar_TOF_X = 19;
               /*
               cout << "WD 158" << endl;
               cout << "V_ampl["<<jentry<<"]["<<ch<<"] = " << ampl_158[jentry][ch] << endl;
               cout << "Barra = " << bar_TOF_X << endl;
               cout << "------------------------------\n" << endl;
               */
            }
         }
      }// END LOOP ON CHANNEL


      //********************************************************************
      // WAVEDREAM 167 - Y VIEW (BAR 20 TO 26)
      //********************************************************************

      for (Int_t ch=2; ch<16; ch++){

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
               if (ch==2) bar_TOF_Y = 20;
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
               cout << "WD 167" << endl;
               cout << "V_ampl["<<jentry<<"]["<<ch<<"] = " << ampl_167[jentry][ch] << endl;
               cout << "Barra = " << bar_TOF_Y << endl;
               cout << "------------------------------\n" << endl;
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
               cout << "WD 168" << endl;
               cout << "V_ampl["<<jentry<<"]["<<ch<<"] = " << ampl_168[jentry][ch] << endl;
               cout << "Barra = " << bar_TOF_Y << endl;
               cout << "------------------------------\n" << endl;
               */
            }
         }
      }// END LOOP ON CHANNEL


      //********************************************************************
      // WAVEDREAM 166 - Y VIEW (BAR 28 to 30)
      //********************************************************************

      for (Int_t ch=6; ch<12; ch++){

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
         //if (jentry > 0 && abs(ampl_168[jentry][ch]-ampl_168[jentry-1][ch])>0.00001 && ampl_168[jentry][ch]>0.)
         {
            if (1)
            {                 
               if (ch==6) bar_TOF_Y = 28;
               else if (ch==7) bar_TOF_Y = 28;
               else if (ch==8) bar_TOF_Y = 29;
               else if (ch==9) bar_TOF_Y = 29;
               else if (ch==10) bar_TOF_Y = 30;
               else if (ch==11) bar_TOF_Y = 30;
               /*
               cout << "WD 166" << endl;
               cout << "V_ampl["<<jentry<<"]["<<ch<<"] = " << ampl_166[jentry][ch] << endl;
               cout << "Barra = " << bar_TOF_Y << endl;
               cout << "------------------------------\n" << endl;
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
               cout << "WD 158" << endl;
               cout << "V_ampl["<<jentry<<"]["<<ch<<"] = " << ampl_158[jentry][ch] << endl;
               cout << "Barra = " << bar_TOF_Y << endl;
               cout << "------------------------------\n" << endl;
               */
            }
         }
      }// END LOOP ON CHANNEL

      // FILL THE HISTOGRAM
      if (bar_TOF_X > -1 && bar_TOF_Y > -1) {
         hist->Fill(bar_TOF_X, bar_TOF_Y);
      }
      else {
         //cout << "No Match X-Y Bar for event # " << jentry << endl;
      }
   
   }// END LOOP ON ENTRIES
   hist->GetXaxis()->SetTitle("FronBar, LayerX");
   hist->GetXaxis()->SetRangeUser(0, 19);
   hist->GetYaxis()->SetTitle("RearBar, LayerY");
   hist->GetYaxis()->SetRangeUser(20, 39);
   hist->GetZaxis()->SetTitle("# hits");
   //hist->SetBarOffset(-0.5);
   hist->Draw("colz");
}
