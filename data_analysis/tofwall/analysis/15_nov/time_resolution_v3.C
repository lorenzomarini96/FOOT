//---------------------------------------------------------------------------------------------
// MACRO COMPUTING FEATURES OF THE WAVEDREAMS - X/Y VIEW AND SHOW THE DISTRIBUTION OF 
//
// - AMPLITUDE V FOR EACH CHANNEL
// - CHARGE Q FOR EACH CHANNEL
// - CHARGE Q FOR EACH BAR
// - TIME T FOR EACH CHANNEL: time when the WF crosses Vth, evaluated through a 
//   linear interpolation of the two points nearest to the threshold.
// - DELTA TIME ∆T FOR EACH CHANNEL
//---------------------------------------------------------------------------------------------

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

   gStyle->SetOptFit(10111);

   //------------------------------
   // HISTOGRAMS HIT
   //------------------------------
   TH1D *hist_hit_X = new TH1D("hist_hit_X", "Hit-map X-view", 20, -0.5, 19.5);
   TH1D *hist_hit_Y = new TH1D("hist_hit_Y", "Hit-map Y-view", 20, 19.5, 39.5);
   TH2D *hist_hit_XY = new TH2D("hist_hit_XY", "Hit-map", 20, -0.5, 19.5, 20, 19.5, 39.5);
   TH2D *hist_hit_XY_3d = new TH2D("hist_hit_XY_3d", "Hit-map", 20, -0.5, 19.5, 20, 19.5, 39.5);

   //------------------------------
   // HIST FOR TIME RESOLUTION
   //------------------------------
   //TH1D *hist_mean_time = new TH1D("hist_mean_time", "#DeltaT_{TW} = (#frac{T_L + T_R}{2})_{front} - (#frac{T_L + T_R}{2})_{rear}", 40, -30.0, 30.0);
   TH1D *hist_mean_time = new TH1D("hist_mean_time", "#DeltaT_{TW} = #bar{T}_{front} - #bar{T}_{rear}", 50, -15.0, 15.0);
   

   //==================================================================================
   // WAVEDREAM 165 - X VIEW (BAR 0 TO 7)   
   //==================================================================================
   
   //------------------------------
   // HISTOGRAMS Q VS TIME WD165
   //------------------------------

   TH2D *hist_charge_time_165[8];
   char name_charge_time_165[20];
   char title_charge_time_165[100];

   for (Int_t b=0; b<8; b++) {
      sprintf(name_charge_time_165, "hist_charge_time_165_bar%d", b);
      sprintf(title_charge_time_165, "WD165 - Log(Q_L/Q_R) vs (t_L - t_R) b%d", b);
      hist_charge_time_165[b] = new TH2D(name_charge_time_165,title_charge_time_165, 30, -2.5, 2.5, 30, -5.0, 5.0);
      hist_charge_time_165[b]->GetXaxis()->SetTitle("Log(Q_L/Q_R)");
      hist_charge_time_165[b]->GetYaxis()->SetTitle("(t_L - t_R) [ns]");
      hist_charge_time_165[b]->GetXaxis()->SetTitleSize(0.05);
      hist_charge_time_165[b]->GetYaxis()->SetTitleSize(0.05);
   }

   //------------------------------
   // HISTOGRAMS V_AMPLITUDE WD165
   //------------------------------
   TH1F *hist_v_ampl_165[16];
   char name_v_ampl_165[20];
   char title_v_ampl_165[100];

   for (Int_t chn=0; chn<16; chn++) {
      sprintf(name_v_ampl_165, "hist_v_ampl_165%d", chn);
      sprintf(title_v_ampl_165, "WD165 - V_{ampl} of chn%d", chn);
      hist_v_ampl_165[chn] = new TH1F(name_v_ampl_165,title_v_ampl_165, 50, 0, 0.4);
      hist_v_ampl_165[chn]->GetXaxis()->SetTitle("V_ampl [mV]");
      hist_v_ampl_165[chn]->GetYaxis()->SetTitle("Entries");
      hist_v_ampl_165[chn]->GetXaxis()->SetTitleSize(0.05);
      hist_v_ampl_165[chn]->GetYaxis()->SetTitleSize(0.05);
   }

   //------------------------------
   // HISTOGRAMS CHARGHE WD165
   //------------------------------
   TH1F *hist_q_165[16];
   char name_q_165[20];
   char title_q_165[100];

   for (Int_t chn=0; chn<16; chn++) {
      sprintf(name_q_165, "hist_q_165%d", chn);
      sprintf(title_q_165, "WD165 - Q of chn%d", chn);
      hist_q_165[chn] = new TH1F(name_q_165,title_q_165, 40, 0.0, 40);
      hist_q_165[chn]->GetXaxis()->SetTitle("Q [a.u.]");
      hist_q_165[chn]->GetYaxis()->SetTitle("Entries");
      hist_q_165[chn]->GetXaxis()->SetTitleSize(0.05);
      hist_q_165[chn]->GetYaxis()->SetTitleSize(0.05);
   }

   //------------------------------
   // HISTOGRAMS CHARGHE BAR WD165
   //------------------------------
   TH1F *hist_q_bar_165[16];
   char name_q_bar_165[20];
   char title_q_bar_165[100];

   for (Int_t b=0; b<16; b++) {
      sprintf(name_q_bar_165, "hist_q_bar_165%d", b);
      sprintf(title_q_bar_165, "WD165 - Q of bar%d", b);
      hist_q_bar_165[b] = new TH1F(name_q_bar_165,title_q_bar_165, 30, 0.0, 40);
      hist_q_bar_165[b]->GetXaxis()->SetTitle("Q [a.u.]");
      hist_q_bar_165[b]->GetYaxis()->SetTitle("Entries");
      hist_q_bar_165[b]->GetXaxis()->SetTitleSize(0.05);
      hist_q_bar_165[b]->GetYaxis()->SetTitleSize(0.05);
   }

   //------------------------------
   // HISTOGRAMS DELTA TIME WD165
   //------------------------------
   TH1F *hist_delta_time_165[16];
   char name_delta_time_165[20];
   char title_delta_time_165[100];

   for (Int_t b=0; b<8; b++) {
      sprintf(name_delta_time_165,"h_delta_time_165%d", b);
      sprintf(title_delta_time_165,"WD165 - #DeltaT WD165 of bar%d", b);
      hist_delta_time_165[b] = new TH1F(name_delta_time_165,title_delta_time_165, 10, -5.0, 5.0); // [ns]
      hist_delta_time_165[b]->GetXaxis()->SetTitle("#DeltaT [ns]");
      hist_delta_time_165[b]->GetYaxis()->SetTitle("Entries");
   }

   //============================================================================================================================
   // WAVEDREAM 166 - X VIEW (BAR 8 TO 10); Y VIEW (BAR 28 to 30)

   //------------------------------
   // HISTOGRAMS Q VS TIME WD166
   //------------------------------

   TH2D *hist_charge_time_166[8];
   char name_charge_time_166[20];
   char title_charge_time_166[100];

   for (Int_t b=0; b<8; b++) {
      sprintf(name_charge_time_166, "hist_charge_time_166_bar%d", b);
      sprintf(title_charge_time_166, "WD166 - Log(Q_L/Q_R) vs (t_L - t_R) b%d", b);
      hist_charge_time_166[b] = new TH2D(name_charge_time_166,title_charge_time_166, 30, -2.5, 2.5, 30, -5.0, 5.0);
      hist_charge_time_166[b]->GetXaxis()->SetTitle("Log(Q_L/Q_R)");
      hist_charge_time_166[b]->GetYaxis()->SetTitle("(t_L - t_R) [ns]");
      hist_charge_time_166[b]->GetXaxis()->SetTitleSize(0.05);
      hist_charge_time_166[b]->GetYaxis()->SetTitleSize(0.05);
   }
   
   //------------------------------
   // HISTOGRAMS V_AMPLITUDE WD165
   //------------------------------
   TH1F *hist_v_ampl_166[16];
   char name_v_ampl_166[20];
   char title_v_ampl_166[100];

   for (Int_t chn=0; chn<16; chn++) {
      sprintf(name_v_ampl_166, "hist_v_ampl_166%d", chn);
      sprintf(title_v_ampl_166, "WD166 - V_{ampl} of chn%d", chn);
      hist_v_ampl_166[chn] = new TH1F(name_v_ampl_166,title_v_ampl_166, 50, 0, 0.4);
      hist_v_ampl_166[chn]->GetXaxis()->SetTitle("V_ampl [mV]");
      hist_v_ampl_166[chn]->GetYaxis()->SetTitle("Entries");
      hist_v_ampl_166[chn]->GetXaxis()->SetTitleSize(0.05);
      hist_v_ampl_166[chn]->GetYaxis()->SetTitleSize(0.05);
   }

   //------------------------------
   // HISTOGRAMS CHARGHE WD166
   //------------------------------
   TH1F *hist_q_166[16];
   char name_q_166[20];
   char title_q_166[100];

   for (Int_t chn=0; chn<16; chn++) {
      sprintf(name_q_166, "hist_q_166%d", chn);
      sprintf(title_q_166, "WD166 - Q of chn%d", chn);
      hist_q_166[chn] = new TH1F(name_q_166,title_q_166, 40, 0.0, 40);
      hist_q_166[chn]->GetXaxis()->SetTitle("Q [a.u.]");
      hist_q_166[chn]->GetYaxis()->SetTitle("Entries");
      hist_q_166[chn]->GetXaxis()->SetTitleSize(0.05);
      hist_q_166[chn]->GetYaxis()->SetTitleSize(0.05);
   }

   //------------------------------
   // HISTOGRAMS CHARGHE BAR WD165
   //------------------------------
   TH1F *hist_q_bar_166[16];
   char name_q_bar_166[20];
   char title_q_bar_166[100];

   for (Int_t b=0; b<16; b++) {
      sprintf(name_q_bar_166, "hist_q_bar_166%d", b);
      sprintf(title_q_bar_166, "WD166 - Q of bar%d", b);
      hist_q_bar_166[b] = new TH1F(name_q_bar_166,title_q_bar_166, 30, 0.0, 40);
      hist_q_bar_166[b]->GetXaxis()->SetTitle("Q [a.u.]");
      hist_q_bar_166[b]->GetYaxis()->SetTitle("Entries");
      hist_q_bar_166[b]->GetXaxis()->SetTitleSize(0.05);
      hist_q_bar_166[b]->GetYaxis()->SetTitleSize(0.05);
   }

   //------------------------------
   // HISTOGRAMS DELTA TIME WD166
   //------------------------------
   TH1F *hist_delta_time_166[16];
   char name_delta_time_166[20];
   char title_delta_time_166[100];

   for (Int_t b=0; b<8; b++) {
      sprintf(name_delta_time_166,"h_delta_time_166%d", b);
      sprintf(title_delta_time_166,"WD166 - #DeltaT WD166 of bar%d", b);
      hist_delta_time_166[b] = new TH1F(name_delta_time_166,title_delta_time_166, 10, -5.0, 5.0); // [ns]
      hist_delta_time_166[b]->GetXaxis()->SetTitle("#DeltaT [ns]");
      hist_delta_time_166[b]->GetYaxis()->SetTitle("Entries");
   }

   //============================================================================================================================
   // // WAVEDREAM 170 - X VIEW (BAR 11 TO 18)   

   //------------------------------
   // HISTOGRAMS Q VS TIME WD170
   //------------------------------
   
   TH2D *hist_charge_time_170[8];
   char name_charge_time_170[20];
   char title_charge_time_170[100];

   for (Int_t b=0; b<8; b++) {
      sprintf(name_charge_time_170, "hist_charge_time_170_bar%d", b);
      sprintf(title_charge_time_170, "WD170 - Log(Q_L/Q_R) vs (t_L - t_R) b%d", b);
      hist_charge_time_170[b] = new TH2D(name_charge_time_170,title_charge_time_170, 30, -2.5, 2.5, 30, -5.0, 5.0);
      hist_charge_time_170[b]->GetXaxis()->SetTitle("Log(Q_L/Q_R)");
      hist_charge_time_166[b]->GetYaxis()->SetTitle("(t_L - t_R) [ns]");
      hist_charge_time_166[b]->GetXaxis()->SetTitleSize(0.05);
      hist_charge_time_166[b]->GetYaxis()->SetTitleSize(0.05);
   }

   //------------------------------
   // HISTOGRAMS V_AMPLITUDE WD170
   //------------------------------
   TH1F *hist_v_ampl_170[16];
   char name_v_ampl_170[20];
   char title_v_ampl_170[100];

   for (Int_t chn=0; chn<16; chn++) {
      sprintf(name_v_ampl_170, "hist_v_ampl_170%d", chn);
      sprintf(title_v_ampl_170, "WD170 - V_{ampl} of chn%d", chn);
      hist_v_ampl_170[chn] = new TH1F(name_v_ampl_170,title_v_ampl_170, 50, 0, 0.4);
      hist_v_ampl_170[chn]->GetXaxis()->SetTitle("V_ampl [mV]");
      hist_v_ampl_170[chn]->GetYaxis()->SetTitle("Entries");
      hist_v_ampl_170[chn]->GetXaxis()->SetTitleSize(0.05);
      hist_v_ampl_170[chn]->GetYaxis()->SetTitleSize(0.05);
   }

   //------------------------------
   // HISTOGRAMS CHARGHE WD170
   //------------------------------
   TH1F *hist_q_170[16];
   char name_q_170[20];
   char title_q_170[100];

   for (Int_t chn=0; chn<16; chn++) {
      sprintf(name_q_170, "hist_q_170%d", chn);
      sprintf(title_q_170, "WD170 - Q of chn%d", chn);
      hist_q_170[chn] = new TH1F(name_q_170,title_q_170, 40, 0.0, 40);
      hist_q_170[chn]->GetXaxis()->SetTitle("Q [a.u.]");
      hist_q_170[chn]->GetYaxis()->SetTitle("Entries");
      hist_q_170[chn]->GetXaxis()->SetTitleSize(0.05);
      hist_q_170[chn]->GetYaxis()->SetTitleSize(0.05);
   }

   //------------------------------
   // HISTOGRAMS CHARGHE BAR WD170
   //------------------------------
   TH1F *hist_q_bar_170[16];
   char name_q_bar_170[20];
   char title_q_bar_170[100];

   for (Int_t b=0; b<16; b++) {
      sprintf(name_q_bar_170, "hist_q_bar_170%d", b);
      sprintf(title_q_bar_170, "WD170 - Q of bar%d", b);
      hist_q_bar_170[b] = new TH1F(name_q_bar_170,title_q_bar_170, 30, 0.0, 40);
      hist_q_bar_170[b]->GetXaxis()->SetTitle("Q [a.u.]");
      hist_q_bar_170[b]->GetYaxis()->SetTitle("Entries");
      hist_q_bar_170[b]->GetXaxis()->SetTitleSize(0.05);
      hist_q_bar_170[b]->GetYaxis()->SetTitleSize(0.05);
   }

   //------------------------------
   // HISTOGRAMS DELTA TIME WD170
   //------------------------------
   TH1F *hist_delta_time_170[16];
   char name_delta_time_170[20];
   char title_delta_time_170[100];

   for (Int_t b=0; b<8; b++) {
      sprintf(name_delta_time_170,"h_delta_time_170%d", b);
      sprintf(title_delta_time_170,"WD170 - #DeltaT WD170 of bar%d", b);
      hist_delta_time_170[b] = new TH1F(name_delta_time_170,title_delta_time_170, 10, -5.0, 5.0); // [ns]
      hist_delta_time_170[b]->GetXaxis()->SetTitle("#DeltaT [ns]");
      hist_delta_time_170[b]->GetYaxis()->SetTitle("Entries");
   }

   //==================================================================================
   // WAVEDREAM 167 - X VIEW (BAR 19); Y VIEW (BAR 20 TO 26)

   //------------------------------
   // HISTOGRAMS Q VS TIME WD167
   //------------------------------
   
   TH2D *hist_charge_time_167[8];
   char name_charge_time_167[20];
   char title_charge_time_167[100];

   for (Int_t b=0; b<8; b++) {
      sprintf(name_charge_time_167, "hist_charge_time_167_bar%d", b);
      sprintf(title_charge_time_167, "WD167 - Log(Q_L/Q_R) vs (t_L - t_R) b%d", b);
      hist_charge_time_167[b] = new TH2D(name_charge_time_167,title_charge_time_167, 30, -2.5, 2.5, 30, -5.0, 5.0);
      hist_charge_time_167[b]->GetXaxis()->SetTitle("Log(Q_L/Q_R)");
      hist_charge_time_167[b]->GetYaxis()->SetTitle("(t_L - t_R) [ns]");
      hist_charge_time_167[b]->GetXaxis()->SetTitleSize(0.05);
      hist_charge_time_167[b]->GetYaxis()->SetTitleSize(0.05);
   }
   
   //------------------------------
   // HISTOGRAMS V_AMPLITUDE WD167
   //------------------------------
   TH1F *hist_v_ampl_167[16];
   char name_v_ampl_167[20];
   char title_v_ampl_167[100];

   for (Int_t chn=0; chn<16; chn++) {
      sprintf(name_v_ampl_167, "hist_v_ampl_167%d", chn);
      sprintf(title_v_ampl_167, "WD167 - V_{ampl} of chn%d", chn);
      hist_v_ampl_167[chn] = new TH1F(name_v_ampl_167,title_v_ampl_167, 50, 0, 0.4);
      hist_v_ampl_167[chn]->GetXaxis()->SetTitle("V_ampl [mV]");
      hist_v_ampl_167[chn]->GetYaxis()->SetTitle("Entries");
      hist_v_ampl_167[chn]->GetXaxis()->SetTitleSize(0.05);
      hist_v_ampl_167[chn]->GetYaxis()->SetTitleSize(0.05);
   }

   //------------------------------
   // HISTOGRAMS CHARGHE WD167
   //------------------------------
   TH1F *hist_q_167[16];
   char name_q_167[20];
   char title_q_167[100];

   for (Int_t chn=0; chn<16; chn++) {
      sprintf(name_q_167, "hist_q_167%d", chn);
      sprintf(title_q_167, "WD167 - Q of chn%d", chn);
      hist_q_167[chn] = new TH1F(name_q_167,title_q_167, 40, 0.0, 40);
      hist_q_167[chn]->GetXaxis()->SetTitle("Q [a.u.]");
      hist_q_167[chn]->GetYaxis()->SetTitle("Entries");
      hist_q_167[chn]->GetXaxis()->SetTitleSize(0.05);
      hist_q_167[chn]->GetYaxis()->SetTitleSize(0.05);
   }

   //------------------------------
   // HISTOGRAMS CHARGHE BAR WD167
   //------------------------------
   TH1F *hist_q_bar_167[16];
   char name_q_bar_167[20];
   char title_q_bar_167[100];

   for (Int_t b=0; b<16; b++) {
      sprintf(name_q_bar_167, "hist_q_bar_167%d", b);
      sprintf(title_q_bar_167, "WD167 - Q of bar%d", b);
      hist_q_bar_167[b] = new TH1F(name_q_bar_167,title_q_bar_167, 30, 0.0, 40);
      hist_q_bar_167[b]->GetXaxis()->SetTitle("Q [a.u.]");
      hist_q_bar_167[b]->GetYaxis()->SetTitle("Entries");
      hist_q_bar_167[b]->GetXaxis()->SetTitleSize(0.05);
      hist_q_bar_167[b]->GetYaxis()->SetTitleSize(0.05);
   }

   //------------------------------
   // HISTOGRAMS DELTA TIME WD167
   //------------------------------
   TH1F *hist_delta_time_167[16];
   char name_delta_time_167[20];
   char title_delta_time_167[100];

   for (Int_t b=0; b<8; b++) {
      sprintf(name_delta_time_167,"h_delta_time_167%d", b);
      sprintf(title_delta_time_167,"WD167 - #DeltaT WD167 of bar%d", b);
      hist_delta_time_167[b] = new TH1F(name_delta_time_167,title_delta_time_167, 10, -5.0, 5.0); // [ns]
      hist_delta_time_167[b]->GetXaxis()->SetTitle("#DeltaT [ns]");
      hist_delta_time_167[b]->GetYaxis()->SetTitle("Entries");
   }
   
   //============================================================================================================================
   // WAVEDREAM 168 - Y VIEW (BAR 0 TO 7)

   //------------------------------
   // HISTOGRAMS Q VS TIME WD168
   //------------------------------
   
   TH2D *hist_charge_time_168[8];
   char name_charge_time_168[20];
   char title_charge_time_168[100];

   for (Int_t b=0; b<8; b++) {
      sprintf(name_charge_time_168, "hist_charge_time_168_bar%d", b);
      sprintf(title_charge_time_168, "WD168 - Log(Q_L/Q_R) vs (t_L - t_R) b%d", b);
      hist_charge_time_168[b] = new TH2D(name_charge_time_168,title_charge_time_168, 30, -2.5, 2.5, 30, -5.0, 5.0);
      hist_charge_time_168[b]->GetXaxis()->SetTitle("Log(Q_L/Q_R)");
      hist_charge_time_168[b]->GetYaxis()->SetTitle("(t_L - t_R) [ns]");
      hist_charge_time_168[b]->GetXaxis()->SetTitleSize(0.05);
      hist_charge_time_168[b]->GetYaxis()->SetTitleSize(0.05);
   }
   
   //------------------------------
   // HISTOGRAMS V_AMPLITUDE WD168
   //------------------------------
   TH1F *hist_v_ampl_168[16];
   char name_v_ampl_168[20];
   char title_v_ampl_168[100];

   for (Int_t chn=0; chn<16; chn++) {
      sprintf(name_v_ampl_168, "hist_v_ampl_168%d", chn);
      sprintf(title_v_ampl_168, "WD168 - V_{ampl} of chn%d", chn);
      hist_v_ampl_168[chn] = new TH1F(name_v_ampl_168,title_v_ampl_165, 50, 0, 0.4);
      hist_v_ampl_168[chn]->GetXaxis()->SetTitle("V_ampl [mV]");
      hist_v_ampl_168[chn]->GetYaxis()->SetTitle("Entries");
      hist_v_ampl_168[chn]->GetXaxis()->SetTitleSize(0.05);
      hist_v_ampl_168[chn]->GetYaxis()->SetTitleSize(0.05);
   }

   //------------------------------
   // HISTOGRAMS CHARGHE WD168
   //------------------------------
   TH1F *hist_q_168[16];
   char name_q_168[20];
   char title_q_168[100];

   for (Int_t chn=0; chn<16; chn++) {
      sprintf(name_q_168, "hist_q_168%d", chn);
      sprintf(title_q_168, "WD168 - Q of chn%d", chn);
      hist_q_168[chn] = new TH1F(name_q_168,title_q_168, 40, 0.0, 40);
      hist_q_168[chn]->GetXaxis()->SetTitle("Q [a.u.]");
      hist_q_168[chn]->GetYaxis()->SetTitle("Entries");
      hist_q_168[chn]->GetXaxis()->SetTitleSize(0.05);
      hist_q_168[chn]->GetYaxis()->SetTitleSize(0.05);
   }

   //------------------------------
   // HISTOGRAMS CHARGHE BAR WD168
   //------------------------------
   TH1F *hist_q_bar_168[16];
   char name_q_bar_168[20];
   char title_q_bar_168[100];

   for (Int_t b=0; b<16; b++) {
      sprintf(name_q_bar_168, "hist_q_bar_168%d", b);
      sprintf(title_q_bar_168, "WD168 - Q of bar%d", b);
      hist_q_bar_168[b] = new TH1F(name_q_bar_168,title_q_bar_168, 30, 0.0, 40);
      hist_q_bar_168[b]->GetXaxis()->SetTitle("Q [a.u.]");
      hist_q_bar_168[b]->GetYaxis()->SetTitle("Entries");
      hist_q_bar_168[b]->GetXaxis()->SetTitleSize(0.05);
      hist_q_bar_168[b]->GetYaxis()->SetTitleSize(0.05);
   }

   //------------------------------
   // HISTOGRAMS DELTA TIME WD168
   //------------------------------
   TH1F *hist_delta_time_168[16];
   char name_delta_time_168[20];
   char title_delta_time_168[100];

   for (Int_t b=0; b<8; b++) {
      sprintf(name_delta_time_168,"h_delta_time_168%d", b);
      sprintf(title_delta_time_168,"WD168 - #DeltaT WD168 of bar%d", b);
      hist_delta_time_168[b] = new TH1F(name_delta_time_168,title_delta_time_168, 10, -5.0, 5.0); // [ns]
      hist_delta_time_168[b]->GetXaxis()->SetTitle("#DeltaT [ns]");
      hist_delta_time_168[b]->GetYaxis()->SetTitle("Entries");
   }

   //============================================================================================================================
   // WAVEDREAM 158 - Y VIEW (BAR 38 TO 39)

   /*
   //------------------------------
   // HISTOGRAMS Q VS TIME WD158
   //------------------------------
   
   TH2D *hist_charge_time_158[8];
   char name_charge_time_158[20];
   char title_charge_time_158[100];

   for (Int_t b=0; b<8; b++) {
      sprintf(name_charge_time_158, "hist_charge_time_158_bar%d", b);
      sprintf(title_charge_time_158, "WD158 - Log(Q_L/Q_R) vs (t_L - t_R) b%d", b);
      hist_charge_time_165[b] = new TH2D(name_charge_time_158,title_charge_time_158, 30, -2.5, 2.5, 30, -5.0, 5.0);
      hist_charge_time_158[b]->GetXaxis()->SetTitle("Log(Q_L/Q_R)");
      hist_charge_time_158[b]->GetYaxis()->SetTitle("(t_L - t_R) [ns]");
      hist_charge_time_158[b]->GetXaxis()->SetTitleSize(0.05);
      hist_charge_time_158[b]->GetYaxis()->SetTitleSize(0.05);
   }
   */

   //------------------------------
   // HISTOGRAMS V_AMPLITUDE WD158
   //------------------------------
   TH1F *hist_v_ampl_158[16];
   char name_v_ampl_158[20];
   char title_v_ampl_158[100];

   for (Int_t chn=0; chn<16; chn++) {
      sprintf(name_v_ampl_158, "hist_v_ampl_158%d", chn);
      sprintf(title_v_ampl_158, "WD158 - V_{ampl} of chn%d", chn);
      hist_v_ampl_158[chn] = new TH1F(name_v_ampl_158,title_v_ampl_158, 50, 0, 0.4);
      hist_v_ampl_158[chn]->GetXaxis()->SetTitle("V_ampl [mV]");
      hist_v_ampl_158[chn]->GetYaxis()->SetTitle("Entries");
      hist_v_ampl_158[chn]->GetXaxis()->SetTitleSize(0.05);
      hist_v_ampl_158[chn]->GetYaxis()->SetTitleSize(0.05);
   }

   //------------------------------
   // HISTOGRAMS CHARGHE WD158
   //------------------------------
   TH1F *hist_q_158[16];
   char name_q_158[20];
   char title_q_158[100];

   for (Int_t chn=0; chn<16; chn++) {
      sprintf(name_q_158, "hist_q_158%d", chn);
      sprintf(title_q_158, "WD158 - Q of chn%d", chn);
      hist_q_158[chn] = new TH1F(name_q_158,title_q_158, 40, 0.0, 40);
      hist_q_158[chn]->GetXaxis()->SetTitle("Q [a.u.]");
      hist_q_158[chn]->GetYaxis()->SetTitle("Entries");
      hist_q_158[chn]->GetXaxis()->SetTitleSize(0.05);
      hist_q_158[chn]->GetYaxis()->SetTitleSize(0.05);
   }

   //------------------------------
   // HISTOGRAMS CHARGHE BAR WD158
   //------------------------------
   TH1F *hist_q_bar_158[16];
   char name_q_bar_158[20];
   char title_q_bar_158[100];

   for (Int_t b=0; b<16; b++) {
      sprintf(name_q_bar_158, "hist_q_bar_158%d", b);
      sprintf(title_q_bar_158, "WD158 - Q of bar%d", b);
      hist_q_bar_158[b] = new TH1F(name_q_bar_158,title_q_bar_158, 30, 0.0, 40);
      hist_q_bar_158[b]->GetXaxis()->SetTitle("Q [a.u.]");
      hist_q_bar_158[b]->GetYaxis()->SetTitle("Entries");
      hist_q_bar_158[b]->GetXaxis()->SetTitleSize(0.05);
      hist_q_bar_158[b]->GetYaxis()->SetTitleSize(0.05);
   }

   //------------------------------
   // HISTOGRAMS DELTA TIME WD158
   //------------------------------
   TH1F *hist_delta_time_158[16];
   char name_delta_time_158[20];
   char title_delta_time_158[100];

   for (Int_t b=0; b<8; b++) {
      sprintf(name_delta_time_158,"h_delta_time_158%d", b);
      sprintf(title_delta_time_158,"WD158 - #DeltaT WD158 of bar%d", b);
      hist_delta_time_158[b] = new TH1F(name_delta_time_158,title_delta_time_158, 10, -5.0, 5.0); // [ns]
      hist_delta_time_158[b]->GetXaxis()->SetTitle("#DeltaT [ns]");
      hist_delta_time_158[b]->GetYaxis()->SetTitle("Entries");
   }

   //============================================================================================================================
   // LOOP ON ENTRIES
   for (Long64_t jentry=0; jentry<nentries/2; jentry++) {
      Long64_t ientry = LoadTree(jentry);
      if (ientry < 0) break;
      nb = fChain->GetEntry(jentry);   nbytes += nb;

      cout << jentry << endl;
      
      // INIT VALUES
      Int_t bar_TOF_X = -1;         // NUMBER OF THE BAR (X-VIEW)
      Int_t bar_TOF_Y = -1;         // NUMBER OF THE BAR (Y-VIEW)
      Double_t f_CFD = 0.3;         // FRACTION FOR COMPUTE THE TIME
      Double_t mean_time_X = -1.;   // MEAN TIME (LEFT AND RIGHT) ON THE BAR (X-VIEW)
      Double_t mean_time_Y = -1.;   // MEAN TIME (LEFT AND RIGHT) ON THE BAR (Y-VIEW) 
      Int_t status = 1;             // POSSIBLE STATUS OF EVENT ("NOISE/ARTIFACT" == 0; "HIT" == 1)
      
      //==================================================================================
      // WAVEDREAM 165 - X VIEW (BAR 0 TO 7)

      // LOOP ON CHANNELS OF WAVEDREAM
      for (Int_t chn=0; chn<16; chn++) {
         
         if (board165_hit[chn] == 1) {

            // INITIALIZE VALUES
            Double_t min_165;                                  // V AMPLITUDE MIN OF CHANNEL [V]
            Double_t max_165;                                  // V AMPLITUDE MAX OF CHANNEL [V]
            Double_t voltage_165 = 0.;                         
            Double_t v_base_165 = 0.;                          // V BASELINE (PEDESTAL) [V]
            Double_t sigma_v_base_165 = 0.;                    // V BASELINE (PEDESTAL) [V]
            Double_t v_ampl_165;                               // V AMPLITUDE OF CHANNEL [V]
            Double_t v_peak_165 = board165_waveform[chn][300]; // V MINIMUM (PEAK) [V]
            Double_t v_th_165;                                 // V THRESHOLD [V]
            Double_t min_position_165 = 0;                     // POSITION OF V PEAK
            Double_t time_165[16];                             // TIME OF WF OF CHANNEL [a.u.]
            Double_t delta_time_165 = 0.;                      // ∆ TIME OF WF OF CHANNEL L AND R [a.u.]
            Double_t mean_time_165[8];                         // MEAN TIME OF L AND R CHANNELS [a.u.]
            Double_t q = 0.;                                   // CHARGE [a.u.]
            Double_t q_165[16];                                // CHARGE OF CHANNEL [a.u.]
            Double_t q_bar_165[8];                             // CHARGE OF CHARGE [a.u.]
            
            //*********************************************************************************
            // 1° CHECK SIGNAL

            // FIND MIN AND MAX OF WAVEFORM
            min_165 = board165_waveform[chn][0];
            max_165 = board165_waveform[chn][0];
            
            for (Int_t t=0; t<1023; t++) { // LOOP ON SAMPLES OF WAVEFORM
               if (board165_waveform[chn][t] < min_165) min_165 = board165_waveform[chn][t];
               if (board165_waveform[chn][t] > max_165) max_165 = board165_waveform[chn][t];
            }
            
            if (max_165 - min_165 > 0.95) { // 0.95 mV
               cout << "ATTENTION: OVERFLOW ARTIFACT!" << endl;
               status = 0; // NOISE
            }
            
            if (max_165 - min_165 < 0.01) { // 10 mV
               cout << "ATTENTION: NOISE!" << endl;
               status = 0; // NOISE
            }
            //*********************************************************************************

            // V PEAK (MINIMUM)
            for (Int_t t=10; t<800; t++) { // LOOP ON SAMPLES OF WAVEFORM
               if (t > 50 && board165_waveform[chn][t] < v_peak_165) {
                  v_peak_165 = board165_waveform[chn][t];
                  min_position_165 = t;
               }
            }
            
            // V BASELINE (NEW METHOD)
            TH1F *wf= new TH1F("wf","wf", 30, 0.30, 0.60);
            for (Int_t t=10; t<250; t++) {
               wf->Fill(board165_waveform[chn][t]);
            }
            
            v_base_165 = wf->GetMean();                                 // V BASELINE
            sigma_v_base_165 = wf->GetRMS();                            // RMS V BASELINE
            v_ampl_165 = v_base_165 - v_peak_165;                       // V AMPLITUDE                                                         
            v_th_165 = v_base_165 - f_CFD * (v_base_165 - v_peak_165);  // V THRESHOLD
            
            
            hist_v_ampl_165[chn]->Fill(v_ampl_165);
            // TIME OF CHANNEL
            /*
            for (Int_t t=335; t<900; t++) { // LOOP ON SAMPLES OF WAVEFORM
               if (board165_waveform[chn][t] == v_th_165) {
                  time_165[chn] = board165_time[chn][t] * TMath::Power(10,9); // [ns]
                  break;
               }
               else if (board165_waveform[chn][t] < v_th_165) {
                  time_165[chn] = (board165_time[chn][t] + board165_time[chn][t-1])/2 * TMath::Power(10,9); // [ns]
                  break;
               }
            }
            */
            for (Int_t t=min_position_165; ; t--) { // LOOP ON SAMPLES OF WAVEFORM
               if (board165_waveform[chn][t] > v_th_165) {
                  time_165[chn] = (board165_time[chn][t] + board165_time[chn][t+1])/2  * TMath::Power(10,9);
                  break;
               }
            }


            for (Int_t t=250; t<650; t++) q += v_base_165 - board165_waveform[chn][t];
            q_165[chn] = q;                                                // CHARGHE OF CHANNEL
            hist_q_165[chn]->Fill(q_165[chn]);
   
            if (chn%2!=0) {
               q_bar_165[chn/2] = sqrt(q_165[chn] * q_165[chn-1]);         // CHARGE OF BAR
               hist_q_bar_165[chn/2]->Fill(q_bar_165[chn/2]);
         
               mean_time_X = (time_165[chn] + time_165[chn-1])/2;  // MEAN TIME OF BAR [ns];               

               delta_time_165 = (time_165[chn] - time_165[chn-1]); // DELTA TIME OF BAR (x10^9 to convert time from [s] in [ns])
               
               if (1){
               //if (delta_time_165 < 3.0 && delta_time_165 > -3.0) {
                  hist_delta_time_165[chn/2]->Fill(delta_time_165);
                  hist_charge_time_165[chn/2]->Fill(log(q_165[chn]/q_165[chn-1]), delta_time_165);
               }
               else mean_time_X = -1;
            }

            // BARS X TOFWALL
            if (chn==0)       bar_TOF_X = 0;
            else if (chn==1)  bar_TOF_X = 0;
            else if (chn==2)  bar_TOF_X = 1;
            else if (chn==3)  bar_TOF_X = 1;
            else if (chn==4)  bar_TOF_X = 2;
            else if (chn==5)  bar_TOF_X = 2;
            else if (chn==6)  bar_TOF_X = 3;
            else if (chn==7)  bar_TOF_X = 3;
            else if (chn==6)  bar_TOF_X = 3;
            else if (chn==8)  bar_TOF_X = 4;
            else if (chn==9)  bar_TOF_X = 4;
            else if (chn==10) bar_TOF_X = 5;
            else if (chn==11) bar_TOF_X = 5;
            else if (chn==12) bar_TOF_X = 6;
            else if (chn==13) bar_TOF_X = 6;
            else if (chn==14) bar_TOF_X = 7;
            else if (chn==15) bar_TOF_X = 7;
            
            if (1) {
               std::cout << "\n**************************"              << std::endl;
               std::cout << "\nWD 165"                                  << std::endl;
               //std::cout << "\nWD TIME per t=650 = "  << board165_time[chn][650]  * TMath::Power(10,6)         << std::endl;
               std::cout << "\nEntry            = " << jentry           << std::endl;
               std::cout << "\nchn              = " << chn              << std::endl;
               std::cout << "\nmin_165 [V]     = " << min_165          << std::endl;
               std::cout << "\nmax_165 [V]     = " << max_165          << std::endl;
               std::cout << "\n------------------------------------------" << std::endl;
               std::cout << "\nv_base_165           [V]  = " << v_base_165   << std::endl;
               std::cout << "\nsigma_v_base_165     [V]  = " << sigma_v_base_165 << std::endl;
               
               std::cout << "\nv_peak_165 [V]   = " << v_peak_165       << std::endl;
               std::cout << "\nv_ampl_165 [V]   = " << v_ampl_165       << std::endl;
               std::cout << "\nmin_position_165 = " << min_position_165 << std::endl;
               std::cout << "\nv_th_165   [V]   = " << v_th_165         << std::endl;
               std::cout << "\ntime_165   [ns]  = " << time_165[chn]    << std::endl;
               std::cout << "\nq_165            = " << q_165[chn]       << std::endl;
               std::cout << "\nBar (X)          = " << bar_TOF_X        << std::endl;
               if (chn%2!=0) {
                  std::cout << "\n------------------------------------------" << std::endl;
                  std::cout << "\nq_bar_165                                 = " << q_bar_165[chn/2]      << std::endl;
                  //std::cout << "\nmean_time_165   = " << mean_time_165[chn/2]  << std::endl;
                  std::cout << "log(q_165[chn]/q_165[chn-1])                = " << log(q_165[chn] / q_165[chn-1]) << endl;
                  std::cout << "time_165["<<chn<<"] - time_165["<<chn-1<<"]         = " << delta_time_165     << endl;
                  std::cout << "mean_time_X WD165 [ns]                      = " << mean_time_X                    << endl;
                  std::cout << "\n------------------------------------------" << std::endl;
               }
            }           
         } // END if (board165_hit[ch] == 1)
      } // END LOOP ON CHANNELS

      //==================================================================================
      // WAVEDREAM 166 - X VIEW (BAR 8 TO 10); Y VIEW (BAR 28 to 30)
      
      // LOOP ON CHANNELS OF WAVEDREAM
      for (Int_t chn=0; chn<12; chn++) {
         
         if (board166_hit[chn] == 1) {

            // INITIALIZE VALUES
            Double_t min_166;                                  // V AMPLITUDE MIN OF CHANNEL [V]
            Double_t max_166;                                  // V AMPLITUDE MAX OF CHANNEL [V]
            Double_t voltage_166 = 0.;                         
            Double_t v_base_166 = 0.;                          // V BASELINE (PEDESTAL) [V]
            Double_t sigma_v_base_166 = 0.;
            Double_t v_ampl_166;                               // V AMPLITUDE OF CHANNEL [V]
            Double_t v_peak_166 = board166_waveform[chn][300]; // V MINIMUM (PEAK) [V]
            Double_t v_th_166;                                 // V THRESHOLD [V]
            Double_t min_position_166 = 0;
            Double_t time_166[12];                             // TIME OF WF OF CHANNEL [a.u.]
            Double_t delta_time_166;                           // ∆ TIME OF WF OF CHANNEL L AND R [a.u.]
            Double_t mean_time_166[6];                         // MEAN TIME OF L AND R CHANNELS [a.u.]
            Double_t q = 0.;                                   // CHARGE [a.u.]
            Double_t q_166[12];                                // CHARGE OF CHANNEL [a.u.]
            Double_t q_bar_166[6];                             // CHARGE OF CHARGE [a.u.]
            
            //*********************************************************************************
            // 1° CHECK SIGNAL

            // FIND MIN AND MAX OF WAVEFORM
            min_166 = board166_waveform[chn][0];
            max_166 = board166_waveform[chn][0];
            
            for (Int_t t=0; t<1023; t++) { // LOOP ON SAMPLES OF WAVEFORM
               if (board166_waveform[chn][t] < min_166) min_166 = board166_waveform[chn][t];
               if (board166_waveform[chn][t] > max_166) max_166 = board166_waveform[chn][t];
            }
            
            if (max_166 - min_166 > 0.95) { // 0.95 mV
               cout << "ATTENTION: OVERFLOW ARTIFACT!" << endl;
               status = 0; // NOISE
            }
            
            if (max_166 - min_166 < 0.01) { // 10 mV
               cout << "ATTENTION: NOISE!" << endl;
               status = 0; // NOISE
            }
            //*********************************************************************************
            
            // V PEAK (MINIMUM)
            for (Int_t t=10; t<800; t++) { // LOOP ON SAMPLES OF WAVEFORM
               if (t > 50 && board166_waveform[chn][t] < v_peak_166) {
                  v_peak_166 = board166_waveform[chn][t];
                  min_position_166 = t;
               }
            }
            
            // V BASELINE (NEW METHOD)
            TH1F *wf_166= new TH1F("wf_166","wf_166", 30, 0.30, 0.60);
            for (Int_t t=10; t<250; t++) {
               wf_166->Fill(board166_waveform[chn][t]);
            }
            
            v_base_166 = wf_166->GetMean();                             // V BASELINE
            sigma_v_base_166 = wf_166->GetRMS();                        // RMS V BASELINE
            v_ampl_166 = v_base_166 - v_peak_166;                       // V AMPLITUDE                                                         
            v_th_166 = v_base_166 - f_CFD * (v_base_166 - v_peak_166);  // V THRESHOLD
            
            

            hist_v_ampl_166[chn]->Fill(v_ampl_166);

            // TIME OF CHANNEL
            /*
            for (Int_t t=335; t<900; t++) { // LOOP ON SAMPLES OF WAVEFORM
               if (board166_waveform[chn][t] == v_th_166) {
                  time_166[chn] = board166_time[chn][t] * TMath::Power(10,9);
                  break;
               }
               else if (board166_waveform[chn][t] < v_th_166) {
                  time_166[chn] = (board166_time[chn][t] + board166_time[chn][t-1])/2 * TMath::Power(10,9);
                  break;
               }
            }
            */
            for (Int_t t=min_position_166; ; t--) { // LOOP ON SAMPLES OF WAVEFORM
               if (board166_waveform[chn][t] > v_th_166) {
                  time_166[chn] = (board166_time[chn][t] + board166_time[chn][t+1])/2  * TMath::Power(10,9);
                  break;
               }
            }

            for (Int_t t=250; t<570; t++) q += v_base_166 - board166_waveform[chn][t];
            q_166[chn] = q;                                                // CHARGHE OF CHANNEL
            hist_q_166[chn]->Fill(q_166[chn]);
   
            if (chn%2!=0) {
               q_bar_166[chn/2] = sqrt(q_166[chn] * q_166[chn-1]);         // CHARGE OF BAR
               hist_q_bar_166[chn/2]->Fill(q_bar_166[chn/2]);
            
               mean_time_166[chn/2] = (time_166[chn] + time_166[chn-1])/2;    // MEAN TIME OF BAR
               if (chn<6) mean_time_X = (time_166[chn] + time_166[chn-1])/2;  // MEAN TIME OF BAR [ns];
               else       mean_time_Y = (time_166[chn] + time_166[chn-1])/2;  // MEAN TIME OF BAR [ns];  // T_BAR = (T_L + T_R)/2

               delta_time_166 = (time_166[chn] - time_166[chn-1]);   // DELTA TIME OF BAR (x10^9 to convert time from [s] in [ns])
               
               if (1) {
               //if (delta_time_166 < 3.0 && delta_time_166 > -3.0) {
                  hist_delta_time_166[chn/2]->Fill(delta_time_166);
                  hist_charge_time_166[chn/2]->Fill(log(q_166[chn]/q_166[chn-1]), delta_time_166);
               }
               else mean_time_X = -1, mean_time_Y = -1;
            }

            if (v_ampl_166 > 0.) { // REMEMBER THE ZEROSUPPRESSION
               // BARS X TOFWALL
               if (chn==0)       bar_TOF_X = 8;
               else if (chn==1)  bar_TOF_X = 8;
               else if (chn==2)  bar_TOF_X = 9;
               else if (chn==3)  bar_TOF_X = 9;
               else if (chn==4)  bar_TOF_X = 10;
               else if (chn==5)  bar_TOF_X = 10;

               else if (chn==6)  bar_TOF_Y = 28;
               else if (chn==7)  bar_TOF_Y = 28;
               else if (chn==8)  bar_TOF_Y = 29;
               else if (chn==9)  bar_TOF_Y = 29;
               else if (chn==10) bar_TOF_Y = 30;
               else if (chn==11) bar_TOF_Y = 30;
            
               if (1) {
                  std::cout << "\n**************************"              << std::endl;
                  std::cout << "\nWD 166"                                  << std::endl;
                  std::cout << "\nEntry            = " << jentry           << std::endl;
                  std::cout << "\nchn              = " << chn              << std::endl;
                  std::cout << "\nmin_166          = " << min_166          << std::endl;
                  std::cout << "\nmax_166          = " << max_166          << std::endl;
                  std::cout << "\nv_base_166       = " << v_base_166       << std::endl;
                  std::cout << "\nv_peak_166       = " << v_peak_166       << std::endl;
                  std::cout << "\nmin_position_166 = " << min_position_166 << std::endl;
                  std::cout << "\nv_th_166         = " << v_th_166         << std::endl;
                  std::cout << "\ntime_166         = " << time_166[chn]    << std::endl;
                  std::cout << "\nv_ampl_166       = " << v_ampl_166       << std::endl;
                  std::cout << "\nq_166            = " << q_166[chn]       << std::endl;
                  std::cout << "\nBar (X)          = " << bar_TOF_X        << std::endl;
                  std::cout << "\nBar (Y)          = " << bar_TOF_Y        << std::endl;
                  if (chn%2!=0) {
                     std::cout << "\n------------------------------------------" << std::endl;
                     std::cout << "\nq_bar_166                                 = " << q_bar_166[chn/2]      << std::endl;
                     //std::cout << "\nmean_time_166   = " << mean_time_166[chn/2]  << std::endl;
                     std::cout << "time_166["<<chn<<"] - time_166["<<chn-1<<"]      = " << delta_time_166 << endl;
                     std::cout << "mean_time_X [ns]                            = " << mean_time_X    << endl;
                     std::cout << "mean_time_Y [ns]                            = " << mean_time_Y    << endl;
                     std::cout << "\n------------------------------------------"   << std::endl;
                  }
               }
            } // END if (v_ampl_166 > 0.015)     
         } // END if (board166_hit[ch] == 1)
      } // END LOOP ON CHANNELS

      //==================================================================================
      // WAVEDREAM 170 - X VIEW (BAR 11 TO 18)

      // LOOP ON CHANNELS OF WAVEDREAM
      for (Int_t chn=0; chn<16; chn++) {
         
         if (board170_hit[chn] == 1) {

            // INITIALIZE VALUES
            Double_t min_170;                                  // V AMPLITUDE MIN OF CHANNEL [V]
            Double_t max_170;                                  // V AMPLITUDE MAX OF CHANNEL [V]
            Double_t voltage_170 = 0.;                         
            Double_t v_base_170 = 0.;                          // V BASELINE (PEDESTAL) [V]
            Double_t sigma_v_base_170 = 0.;
            Double_t v_ampl_170;                               // V AMPLITUDE OF CHANNEL [V]
            Double_t v_peak_170 = board170_waveform[chn][300]; // V MINIMUM (PEAK) [V]
            Double_t v_th_170;                                 // V THRESHOLD [V]
            Double_t min_position_170 = 0;
            Double_t time_170[16];                             // TIME OF WF OF CHANNEL [a.u.]
            Double_t delta_time_170;                           // ∆ TIME OF WF OF CHANNEL L AND R [a.u.]
            Double_t mean_time_170[8];                         // MEAN TIME OF L AND R CHANNELS [a.u.]
            Double_t q = 0.;                                   // CHARGE [a.u.]
            Double_t q_170[16];                                // CHARGE OF CHANNEL [a.u.]
            Double_t q_bar_170[8];                             // CHARGE OF CHARGE [a.u.]
            
            //*********************************************************************************
            // 1° CHECK SIGNAL

            // FIND MIN AND MAX OF WAVEFORM
            min_170 = board170_waveform[chn][0];
            max_170 = board170_waveform[chn][0];
            
            for (Int_t t=0; t<1023; t++) { // LOOP ON SAMPLES OF WAVEFORM
               if (board170_waveform[chn][t] < min_170) min_170 = board170_waveform[chn][t];
               if (board170_waveform[chn][t] > max_170) max_170 = board170_waveform[chn][t];
            }
            
            if (max_170 - min_170 > 0.95) { // 0.95 mV
               cout << "ATTENTION: OVERFLOW ARTIFACT!" << endl;
               status = 0; // NOISE
            }
            
            if (max_170 - min_170 < 0.01) { // 10 mV
               cout << "ATTENTION: NOISE!" << endl;
               status = 0; // NOISE
            }
            //*********************************************************************************
            
            // V PEAK (MINIMUM)
            for (Int_t t=10; t<800; t++) { // LOOP ON SAMPLES OF WAVEFORM
               if (t > 50 && board170_waveform[chn][t] < v_peak_170) {
                  v_peak_170 = board170_waveform[chn][t];
                  min_position_170 = t;
               }
            }
            
            // V BASELINE (NEW METHOD)
            TH1F *wf_170 = new TH1F("wf_170","wf_170", 30, 0.30, 0.60);
            for (Int_t t=10; t<250; t++) {
               wf_170->Fill(board170_waveform[chn][t]);
            }
            
            v_base_170 = wf_170->GetMean();                             // V BASELINE
            sigma_v_base_170 = wf_170->GetRMS();                        // RMS V BASELINE
            v_ampl_170 = v_base_170 - v_peak_170;                       // V AMPLITUDE                                                         
            v_th_170 = v_base_170 - f_CFD * (v_base_170 - v_peak_170);  // V THRESHOLD
            

            hist_v_ampl_170[chn]->Fill(v_ampl_170);

            // TIME OF CHANNEL
            /*
            for (Int_t t=335; t<900; t++) { // LOOP ON SAMPLES OF WAVEFORM
               //cout << "board170_time[chn][t]" << board170_time[chn][t] << endl;
               if (board170_waveform[chn][t] == v_th_170) {
                  time_170[chn] = board170_time[chn][t] * TMath::Power(10,9);
                  break;
               }
               else if (board170_waveform[chn][t] < v_th_170) {
                  time_170[chn] = (board170_time[chn][t] + board170_time[chn][t-1])/2 * TMath::Power(10,9);
                  break;
               }
            }
            */
            for (Int_t t=min_position_170; ; t--) { // LOOP ON SAMPLES OF WAVEFORM
               if (board170_waveform[chn][t] > v_th_170) {
                  time_170[chn] = (board170_time[chn][t] + board170_time[chn][t+1])/2  * TMath::Power(10,9);
                  break;
               }
            }

            for (Int_t t=250; t<570; t++) q += v_base_170 - board170_waveform[chn][t];
            q_170[chn] = q;                                                // CHARGHE OF CHANNEL
            hist_q_170[chn]->Fill(q_170[chn]);
   
            if (chn%2!=0) {
               q_bar_170[chn/2] = sqrt(q_170[chn] * q_170[chn-1]);         // CHARGE OF BAR
               hist_q_bar_170[chn/2]->Fill(q_bar_170[chn/2]);

               mean_time_170[chn/2] = (time_170[chn] + time_170[chn-1])/2;  // MEAN TIME OF BAR
               mean_time_X = (time_170[chn] + time_170[chn-1])/2;  // MEAN TIME OF BAR [ns];           // T_BAR = (T_L + T_R)/2

               delta_time_170 = (time_170[chn] - time_170[chn-1]);   // DELTA TIME OF BAR (x10^9 to convert time from [s] in [ns])
               
               if (1) {
               //if (delta_time_170 < 3.0 && delta_time_170 > -3.0) {
                  hist_delta_time_170[chn/2]->Fill(delta_time_170);
                  hist_charge_time_170[chn/2]->Fill(log(q_170[chn]/q_170[chn-1]), delta_time_170);
               }
               else mean_time_X = -1;
            }

            // BARS X TOFWALL
            if (chn==0)       bar_TOF_X = 11;
            else if (chn==1)  bar_TOF_X = 11;
            else if (chn==2)  bar_TOF_X = 12;
            else if (chn==3)  bar_TOF_X = 12;
            else if (chn==4)  bar_TOF_X = 13;
            else if (chn==5)  bar_TOF_X = 13;
            else if (chn==6)  bar_TOF_X = 14;
            else if (chn==7)  bar_TOF_X = 14;
            else if (chn==8)  bar_TOF_X = 15;
            else if (chn==9)  bar_TOF_X = 15;
            else if (chn==10) bar_TOF_X = 16;
            else if (chn==11) bar_TOF_X = 16;
            else if (chn==12) bar_TOF_X = 17;
            else if (chn==13) bar_TOF_X = 17;
            else if (chn==14) bar_TOF_X = 18;
            else if (chn==15) bar_TOF_X = 18;   
            
            if (1) {
               std::cout << "\n**************************"              << std::endl;
               std::cout << "\nWD 170"                                  << std::endl;
               std::cout << "\nEntry            = " << jentry           << std::endl;
               std::cout << "\nchn              = " << chn              << std::endl;
               std::cout << "\nmin_170          = " << min_170          << std::endl;
               std::cout << "\nmax_170          = " << max_170          << std::endl;
               std::cout << "\nv_base_170 [mV]  = " << v_base_170       << std::endl;
               std::cout << "\nv_peak_170 [mV]  = " << v_peak_170       << std::endl;
               std::cout << "\nv_ampl_170 [mV]  = " << v_ampl_170       << std::endl;
               std::cout << "\nmin_position_170 = " << min_position_170 << std::endl;
               std::cout << "\nv_th_170   [mV]  = " << v_th_170         << std::endl;
               std::cout << "\ntime_170   [ns]  = " << time_170[chn]    << std::endl;
               std::cout << "\nq_170            = " << q_170[chn]       << std::endl;
               std::cout << "\nBar (X)          = " << bar_TOF_X        << std::endl;
               if (chn%2!=0) {
                  std::cout << "\n------------------------------------------" << std::endl;
                  //std::cout << "\ndelta_time_170  = " << delta_time_170        << std::endl;
                  std::cout << "\nq_bar_170                                 = " << q_bar_170[chn/2]      << std::endl;
                  //std::cout << "\nmean_time_170   = " << mean_time_170[chn/2]  << std::endl;
                  std::cout << "time_170["<<chn<<"] - time_170["<<chn-1<<"]       = " << delta_time_170     << endl;
                  std::cout << "mean_time_X [ns]                            = " << mean_time_X    << endl;
                  std::cout << "\n------------------------------------------" << std::endl;
               }
            }           
         } // END if (board170_hit[ch] == 1)
      } // END LOOP ON CHANNELS

      //==================================================================================
      // WAVEDREAM 167 - X VIEW (BAR 19); Y VIEW (BAR 20 TO 26)
      
      // LOOP ON CHANNELS OF WAVEDREAM
      for (Int_t chn=0; chn<16; chn++) {
         
         if (board167_hit[chn] == 1) {

            // INITIALIZE VALUES
            Double_t min_167;                                  // V AMPLITUDE MIN OF CHANNEL [V]
            Double_t max_167;                                  // V AMPLITUDE MAX OF CHANNEL [V]
            Double_t voltage_167 = 0.;                         
            Double_t v_base_167 = 0.;                          // V BASELINE (PEDESTAL) [mV]
            Double_t v_ampl_167;                               // V AMPLITUDE OF CHANNEL [mV]
            Double_t v_peak_167 = board167_waveform[chn][300]; // V MINIMUM (PEAK) [mV]
            Double_t v_th_167;                                 // V THRESHOLD [mV]
            Double_t min_position_167 = 0;
            Double_t time_167[16];                             // TIME OF WF OF CHANNEL [a.u.]
            Double_t delta_time_167;                           // ∆ TIME OF WF OF CHANNEL L AND R [a.u.]
            Double_t mean_time_167[8];                         // MEAN TIME OF L AND R CHANNELS [a.u.]
            Double_t q = 0.;                                   // CHARGE [a.u.]
            Double_t q_167[16];                                // CHARGE OF CHANNEL [a.u.]
            Double_t q_bar_167[8];                             // CHARGE OF CHARGE [a.u.]
            
            //*********************************************************************************
            // 1° CHECK SIGNAL

            // FIND MIN AND MAX OF WAVEFORM
            min_167 = board167_waveform[chn][0];
            max_167 = board167_waveform[chn][0];
            
            for (Int_t t=0; t<1023; t++) { // LOOP ON SAMPLES OF WAVEFORM
               if (board167_waveform[chn][t] < min_167) min_167 = board167_waveform[chn][t];
               if (board167_waveform[chn][t] > max_167) max_167 = board167_waveform[chn][t];
            }
            
            if (max_167 - min_167 > 0.95) { // 0.95 mV
               cout << "ATTENTION: OVERFLOW ARTIFACT!" << endl;
               status = 0; // NOISE
            }
            
            if (max_167 - min_167 < 0.01) { // 10 mV
               cout << "ATTENTION: NOISE!" << endl;
               status = 0; // NOISE
            }
            //*********************************************************************************   
                     
            // V PEAK (MINIMUM)
            for (Int_t t=10; t<800; t++) { // LOOP ON SAMPLES OF WAVEFORM
               if (t > 50 && board167_waveform[chn][t] < v_peak_167) {
                  v_peak_167 = board167_waveform[chn][t];
                  min_position_167 = t;
               }
            }
            
            // V BASELINE (NEW METHOD)
            TH1F *wf_167 = new TH1F("wf_167","wf_167", 30, 0.30, 0.60);
            for (Int_t t=10; t<250; t++) {
               wf_167->Fill(board167_waveform[chn][t]);
            }
            
            v_base_167 = wf_167->GetMean();                             // V BASELINE
            sigma_v_base_167 = wf_167->GetRMS();                        // RMS V BASELINE
            v_ampl_167 = v_base_167 - v_peak_167;                       // V AMPLITUDE                                                         
            v_th_167 = v_base_167 - f_CFD * (v_base_167 - v_peak_167);  // V THRESHOLD
            
            hist_v_ampl_167[chn]->Fill(v_ampl_167);

            // TIME OF CHANNEL
            /*
            for (Int_t t=335; t<900; t++) { // LOOP ON SAMPLES OF WAVEFORM
               if (board167_waveform[chn][t] == v_th_167) {
                  time_167[chn] = board167_time[chn][t] * TMath::Power(10,9);
                  break;
               }
               else if (board167_waveform[chn][t] < v_th_167) {
                  time_167[chn] = (board167_time[chn][t] + board167_time[chn][t-1])/2 * TMath::Power(10,9);
                  break;
               }
            }
            */
            for (Int_t t=min_position_167; ; t--) { // LOOP ON SAMPLES OF WAVEFORM
               if (board167_waveform[chn][t] > v_th_167) {
                  time_167[chn] = (board167_time[chn][t] + board167_time[chn][t+1])/2  * TMath::Power(10,9);
                  break;
               }
            }

            for (Int_t t=250; t<570; t++) q += v_base_167 - board167_waveform[chn][t];
            q_167[chn] = q;                                                // CHARGHE OF CHANNEL
            hist_q_167[chn]->Fill(q_167[chn]);
   
            if (chn%2!=0) {
               q_bar_167[chn/2] = sqrt(q_167[chn] * q_167[chn-1]);         // CHARGE OF BAR
               hist_q_bar_167[chn/2]->Fill(q_bar_167[chn/2]);

               mean_time_167[chn/2] = (time_167[chn] + time_167[chn-1])/2;  // MEAN TIME OF BAR
               if (chn<2) mean_time_X = (time_167[chn] + time_167[chn-1])/2;  // MEAN TIME OF BAR [ns];
               else       mean_time_Y = (time_167[chn] + time_167[chn-1])/2;  // MEAN TIME OF BAR [ns]; // T_BAR = (T_L + T_R)/2
            
               delta_time_167 = (time_167[chn] - time_167[chn-1]);   // DELTA TIME OF BAR (x10^9 to convert time from [s] in [ns])
               
               if (1) {
               //if (delta_time_167 < 3.0 && delta_time_167 > -3.0) {
                  hist_delta_time_167[chn/2]->Fill(delta_time_167);
                  hist_charge_time_167[chn/2]->Fill(log(q_167[chn]/q_167[chn-1]), delta_time_167);
               }
               else mean_time_X = -1, mean_time_Y = -1;
            }

            // BAR X TOFWALL
            if      (chn==0) bar_TOF_X = 19;
            else if (chn==1) bar_TOF_X = 19;

            // BARS Y TOFWALL
            else if (chn==2)  bar_TOF_Y = 20;
            else if (chn==3)  bar_TOF_Y = 20;
            else if (chn==4)  bar_TOF_Y = 21;
            else if (chn==5)  bar_TOF_Y = 21;
            else if (chn==6)  bar_TOF_Y = 22;
            else if (chn==7)  bar_TOF_Y = 22;
            else if (chn==8)  bar_TOF_Y = 23;
            else if (chn==9)  bar_TOF_Y = 23;
            else if (chn==10) bar_TOF_Y = 24;
            else if (chn==11) bar_TOF_Y = 24;
            else if (chn==12) bar_TOF_Y = 25;
            else if (chn==13) bar_TOF_Y = 25;
            else if (chn==14) bar_TOF_Y = 26;
            else if (chn==15) bar_TOF_Y = 26;
            
            if (1) {
               std::cout << "\n**************************"              << std::endl;
               std::cout << "\nWD 167"                                  << std::endl;
               std::cout << "\nEntry            = " << jentry           << std::endl;
               std::cout << "\nchn              = " << chn              << std::endl;
               std::cout << "\nmin_167          = " << min_167          << std::endl;
               std::cout << "\nmax_167          = " << max_167          << std::endl;
               std::cout << "\nv_base_167 [mV]  = " << v_base_167       << std::endl;
               std::cout << "\nv_peak_167 [mV]  = " << v_peak_167       << std::endl;
               std::cout << "\nv_ampl_167 [mV]  = " << v_ampl_167       << std::endl;
               std::cout << "\nmin_position_167 = " << min_position_167 << std::endl;
               std::cout << "\nv_th_167   [mV]  = " << v_th_167         << std::endl;
               std::cout << "\ntime_167   [ns]  = " << time_167[chn]    << std::endl;
               std::cout << "\nq_167            = " << q_167[chn]       << std::endl;
               std::cout << "\nBar (X)          = " << bar_TOF_X        << std::endl;
               std::cout << "\nBar (Y)          = " << bar_TOF_Y        << std::endl;
               if (chn%2!=0) {
                  std::cout << "\n------------------------------------------" << std::endl;
                  std::cout << "\nq_bar_167                                 = " << q_bar_167[chn/2] << std::endl;
                  //std::cout << "\nmean_time_167   = " << mean_time_167[chn/2]  << std::endl;
                  std::cout << "time_167["<<chn<<"] - time_167["<<chn-1<<"]        = " << delta_time_167   << endl;
                  std::cout << "mean_time_X [ns]                            = " << mean_time_X      << endl;
                  std::cout << "mean_time_Y [ns]                            = " << mean_time_Y      << endl;
                  std::cout << "\n------------------------------------------" << std::endl;
               }
            }           
         } // END if (board167_hit[ch] == 1)
      } // END LOOP ON CHANNELS

      //==================================================================================
      // WAVEDREAM 168 - Y VIEW (BAR 27 ANS 31 TO 37)
         
      // LOOP ON CHANNELS OF WAVEDREAM
      for (Int_t chn=0; chn<16; chn++) {
         
         if (board168_hit[chn] == 1) {

            // INITIALIZE VALUES
            Double_t min_168;                                  // V AMPLITUDE MIN OF CHANNEL [V]
            Double_t max_168;                                  // V AMPLITUDE MAX OF CHANNEL [V]
            Double_t voltage_168 = 0.;                         
            Double_t v_base_168 = 0.;                          // V BASELINE (PEDESTAL) [mV]
            Double_t v_ampl_168;                               // V AMPLITUDE OF CHANNEL [mV]
            Double_t v_peak_168 = board168_waveform[chn][300]; // V MINIMUM (PEAK) [mV]
            Double_t v_th_168;                                 // V THRESHOLD [mV]
            Double_t min_position_168 = 0;
            Double_t time_168[16];                             // TIME OF WF OF CHANNEL [a.u.]
            Double_t delta_time_168;                           // ∆ TIME OF WF OF CHANNEL L AND R [a.u.]
            Double_t mean_time_168[8];                         // MEAN TIME OF L AND R CHANNELS [a.u.]
            Double_t q = 0.;                                   // CHARGE [a.u.]
            Double_t q_168[16];                                // CHARGE OF CHANNEL [a.u.]
            Double_t q_bar_168[8];                             // CHARGE OF CHARGE [a.u.]
            
            //*********************************************************************************
            // 1° CHECK SIGNAL

            // FIND MIN AND MAX OF WAVEFORM
            min_168 = board168_waveform[chn][0];
            max_168 = board168_waveform[chn][0];
            
            for (Int_t t=0; t<1023; t++) { // LOOP ON SAMPLES OF WAVEFORM
               if (board168_waveform[chn][t] < min_168) min_168 = board168_waveform[chn][t];
               if (board168_waveform[chn][t] > max_168) max_168 = board168_waveform[chn][t];
            }
            
            if (max_168 - min_168 > 0.95) { // 0.95 mV
               cout << "ATTENTION: OVERFLOW ARTIFACT!" << endl;
               status = 0; // NOISE
            }
            
            if (max_168 - min_168 < 0.01) { // 10 mV
               cout << "ATTENTION: NOISE!" << endl;
               status = 0; // NOISE
            }
            //*********************************************************************************   

            // V PEAK (MINIMUM)
            for (Int_t t=10; t<800; t++) { // LOOP ON SAMPLES OF WAVEFORM
               if (t > 50 && board168_waveform[chn][t] < v_peak_168) {
                  v_peak_168 = board168_waveform[chn][t];
                  min_position_168 = t;
               }
               // APPROX FIRST 1/3 OF 1024 SAMPLES
               if (t <= 150) voltage_168 =  voltage_168 + board168_waveform[chn][t];
            }
 
            v_base_168 = voltage_168/140;                               // V BASELINE
            v_th_168 = v_base_168 - f_CFD * (v_base_168 - v_peak_168);  // V THRESHOLD
            v_ampl_168 = v_base_168 - v_peak_168;                       // V AMPLITUDE
            hist_v_ampl_168[chn]->Fill(v_ampl_168);

            
            // TIME OF CHANNEL
            /*
            for (Int_t t=335; t<900; t++) { // LOOP ON SAMPLES OF WAVEFORM
               if (board168_waveform[chn][t] == v_th_168) {
                  time_168[chn] = board168_time[chn][t]  * TMath::Power(10,9);
                  break;
               }
               else if (board168_waveform[chn][t] < v_th_168) {
                  time_168[chn] = (board168_time[chn][t] + board168_time[chn][t-1])/2  * TMath::Power(10,9);
                  break;
                  //cout << "board168_time[chn][t] = " << board168_time[chn][t] << endl;
                  //cout << "board168_time[chn][t-1] = " << board168_time[chn][t-1] << endl;
               }
            }
            */
            for (Int_t t=min_position_168; ; t--) { // LOOP ON SAMPLES OF WAVEFORM
               if (board168_waveform[chn][t] > v_th_168) {
                  time_168[chn] = (board168_time[chn][t] + board168_time[chn][t+1])/2  * TMath::Power(10,9);
                  break;
               }
            }                        

            for (Int_t t=250; t<570; t++) q += v_base_168 - board168_waveform[chn][t];
            q_168[chn] = q;                                                // CHARGHE OF CHANNEL
            hist_q_168[chn]->Fill(q_168[chn]);
   
            if (chn%2!=0) {
               q_bar_168[chn/2] = sqrt(q_168[chn] * q_168[chn-1]);         // CHARGE OF BAR
               hist_q_bar_168[chn/2]->Fill(q_bar_168[chn/2]);
         
               mean_time_168[chn/2] = (time_168[chn] + time_168[chn-1])/2;  // MEAN TIME OF BAR
               mean_time_Y = (time_168[chn] + time_168[chn-1])/2;  // MEAN TIME OF BAR [ns]
            
               delta_time_168 = (time_168[chn] - time_168[chn-1]);   // DELTA TIME OF BAR (x10^9 to convert time from [s] in [ns])
               
               if (1) {
               //if (delta_time_168 < 3.0 && delta_time_168 > -3.0) {
                  hist_delta_time_168[chn/2]->Fill(delta_time_168);
                  hist_charge_time_168[chn/2]->Fill(log(q_168[chn]/q_168[chn-1]), delta_time_168);
               }
               else mean_time_Y = -1;
            }

            // BARS Y TOFWALL
            if (chn==0)       bar_TOF_Y = 27;
            else if (chn==1)  bar_TOF_Y = 27;
            else if (chn==2)  bar_TOF_Y = 31;
            else if (chn==3)  bar_TOF_Y = 31;
            else if (chn==4)  bar_TOF_Y = 32;
            else if (chn==5)  bar_TOF_Y = 32;
            else if (chn==6)  bar_TOF_Y = 33;
            else if (chn==7)  bar_TOF_Y = 33;
            else if (chn==8)  bar_TOF_Y = 34;
            else if (chn==9)  bar_TOF_Y = 34;
            else if (chn==10) bar_TOF_Y = 35;
            else if (chn==11) bar_TOF_Y = 35;
            else if (chn==12) bar_TOF_Y = 36;
            else if (chn==13) bar_TOF_Y = 36;
            else if (chn==14) bar_TOF_Y = 37;
            else if (chn==15) bar_TOF_Y = 37;
            
            if (1) {
               std::cout << "\n**************************"              << std::endl;
               std::cout << "\nWD 168"                                  << std::endl;
               std::cout << "\nEntry            = " << jentry           << std::endl;
               std::cout << "\nchn              = " << chn              << std::endl;
               std::cout << "\nmin_168          = " << min_168          << std::endl;
               std::cout << "\nmax_168          = " << max_168          << std::endl;
               std::cout << "\nv_base_168 [mV]  = " << v_base_168       << std::endl;
               std::cout << "\nv_peak_168 [mV]  = " << v_peak_168       << std::endl;
               std::cout << "\nv_ampl_168 [mV]  = " << v_ampl_168       << std::endl;
               std::cout << "\nmin_position_168 = " << min_position_168 << std::endl;
               std::cout << "\nv_th_168   [mV]  = " << v_th_168         << std::endl;
               std::cout << "\ntime_168   [ns]  = " << time_168[chn]    << std::endl;
               std::cout << "\nq_168            = " << q_168[chn]       << std::endl;
               std::cout << "\nBar (Y)          = " << bar_TOF_Y        << std::endl;
               if (chn%2!=0) {
                  std::cout << "\n------------------------------------------" << std::endl;
                  //std::cout << "\ndelta_time_168  = " << delta_time_168        << std::endl;
                  std::cout << "\nq_bar_168                                       = " << q_bar_168[chn/2] << std::endl;
                  //std::cout << "\nmean_time_168   = " << mean_time_168[chn/2]  << std::endl;
                  std::cout << "time_168["<<chn<<"] - time_168["<<chn-1<<"]  [ns]      = " << delta_time_168  << endl;
                  std::cout << "mean_time_Y [ns]                             [ns] = " << mean_time_Y     << endl;
                  std::cout << "\n------------------------------------------" << std::endl;
               }
            }           
         } // END if (board168_hit[ch] == 1)
      } // END LOOP ON CHANNELS

      //==================================================================================
      // WAVEDREAM 158 - Y VIEW (BAR 38 TO 39)
         
      // LOOP ON CHANNELS OF WAVEDREAM
      for (Int_t chn=0; chn<16; chn++) {
         
         if (board158_hit[chn] == 1) {

            // INITIALIZE VALUES
            Double_t min_158;                                  // V AMPLITUDE MIN OF CHANNEL [V]
            Double_t max_158;                                  // V AMPLITUDE MAX OF CHANNEL [V]
            Double_t voltage_158 = 0.;                         
            Double_t v_base_158 = 0.;                          // V BASELINE (PEDESTAL) [mV]
            Double_t v_ampl_158;                               // V AMPLITUDE OF CHANNEL [mV]
            Double_t v_peak_158 = board158_waveform[chn][300]; // V MINIMUM (PEAK) [mV]
            Double_t v_th_158;                                 // V THRESHOLD [mV]
            Double_t min_position_158 = 0;
            Double_t time_158[16];                             // TIME OF WF OF CHANNEL [a.u.]
            Double_t delta_time_158;                           // ∆ TIME OF WF OF CHANNEL L AND R [a.u.]
            Double_t mean_time_158[8];                         // MEAN TIME OF L AND R CHANNELS [a.u.]
            Double_t q = 0.;                                   // CHARGE [a.u.]
            Double_t q_158[16];                                // CHARGE OF CHANNEL [a.u.]
            Double_t q_bar_158[8];                             // CHARGE OF CHARGE [a.u.]
            
            //*********************************************************************************
            // 1° CHECK SIGNAL

            // FIND MIN AND MAX OF WAVEFORM
            min_158 = board158_waveform[chn][0];
            max_158 = board158_waveform[chn][0];
            
            for (Int_t t=0; t<1023; t++) { // LOOP ON SAMPLES OF WAVEFORM
               if (board158_waveform[chn][t] < min_158) min_158 = board158_waveform[chn][t];
               if (board158_waveform[chn][t] > max_158) max_158 = board158_waveform[chn][t];
            }
            
            if (max_158 - min_158 > 0.95) { // 0.95 mV
               cout << "ATTENTION: OVERFLOW ARTIFACT!" << endl;
               status = 0; // NOISE
            }
            
            if (max_158 - min_158 < 0.01) { // 10 mV
               cout << "ATTENTION: NOISE!" << endl;
               status = 0; // NOISE
            }
            //*********************************************************************************   

            // V PEAK (MINIMUM)
            for (Int_t t=10; t<800; t++) { // LOOP ON SAMPLES OF WAVEFORM
               if (t > 50 && board158_waveform[chn][t] < v_peak_158) {
                  v_peak_158 = board158_waveform[chn][t];
                  min_position_158 = t;
               }
               // APPROX FIRST 1/3 OF 1024 SAMPLES
               if (t <= 150) voltage_158 =  voltage_158 + board158_waveform[chn][t];
            }
 
            v_base_158 = voltage_158/140;                               // V BASELINE
            v_th_158 = v_base_158 - f_CFD * (v_base_158 - v_peak_158);  // V THRESHOLD
            v_ampl_158 = v_base_158 - v_peak_158;                       // V AMPLITUDE
            
            hist_v_ampl_158[chn]->Fill(v_ampl_158);

            // TIME OF CHANNEL
            /*
            for (Int_t t=335; t<900; t++) { // LOOP ON SAMPLES OF WAVEFORM
               if (board158_waveform[chn][t] == v_th_158) {
                  time_158[chn] = board158_time[chn][t]  * TMath::Power(10,9); 
                  break;
               }
               else if (board158_waveform[chn][t] < v_th_158) {
                  time_158[chn] = (board158_time[chn][t] + board158_time[chn][t-1])/2  * TMath::Power(10,9);
                  break;
               }
            }
            */
            for (Int_t t=min_position_158; ; t--) { // LOOP ON SAMPLES OF WAVEFORM
               if (board158_waveform[chn][t] > v_th_158) {
                  time_158[chn] = (board158_time[chn][t] + board158_time[chn][t+1])/2  * TMath::Power(10,9);
                  break;
               }
            }

            for (Int_t t=250; t<570; t++) q += v_base_158 - board158_waveform[chn][t];
            q_158[chn] = q;                                                // CHARGHE OF CHANNEL
            hist_q_158[chn]->Fill(q_158[chn]);
   
            if (chn%2!=0) {
               q_bar_158[chn/2] = sqrt(q_158[chn] * q_158[chn-1]);         // CHARGE OF BAR
               hist_q_bar_158[chn/2]->Fill(q_bar_158[chn/2]);
            
               mean_time_158[chn/2] = (time_158[chn] + time_158[chn-1])/2;  // MEAN TIME OF BAR
               mean_time_Y = (time_158[chn] + time_158[chn-1])/2;  // MEAN TIME OF BAR [ns];
            
               delta_time_158 = (time_158[chn] - time_158[chn-1]);   // DELTA TIME OF BAR (x10^9 to convert time from [s] in [ns])
               
               if (1) {
               //if (delta_time_158 < 3.0 && delta_time_158 > -3.0) {
                  hist_delta_time_158[chn/2]->Fill(delta_time_158);
                  //hist_charge_time_158[chn/2]->Fill(log(q_158[chn]/q_158[chn-1]), delta_time_158);
               }
               else mean_time_Y = -1;
            }
            

            // BARS Y TOFWALL
            if (chn==0)      bar_TOF_Y = 38;
            else if (chn==1) bar_TOF_Y = 38;
            else if (chn==2) bar_TOF_Y = 39;
            else if (chn==3) bar_TOF_Y = 39;
            
            if (1) {
               std::cout << "\n**************************"              << std::endl;
               std::cout << "\nWD 158"                                  << std::endl;
               std::cout << "\nEntry            = " << jentry           << std::endl;
               std::cout << "\nchn              = " << chn              << std::endl;
               std::cout << "\nmin_158          = " << min_158          << std::endl;
               std::cout << "\nmax_158          = " << max_158          << std::endl;
               std::cout << "\nv_base_158 [mV]  = " << v_base_158       << std::endl;
               std::cout << "\nv_peak_158 [mV]  = " << v_peak_158       << std::endl;
               std::cout << "\nv_ampl_158 [mV]  = " << v_ampl_158       << std::endl;
               std::cout << "\nmin_position_158 = " << min_position_158 << std::endl;
               std::cout << "\nv_th_158   [mV]  = " << v_th_158         << std::endl;
               std::cout << "\ntime_158   [ns]  = " << time_158[chn]    << std::endl;
               std::cout << "\nq_158            = " << q_158[chn]       << std::endl;
               std::cout << "\nBar (Y)          = " << bar_TOF_Y        << std::endl;
               if (chn%2!=0) {
                  std::cout << "\n------------------------------------------" << std::endl;
                  //std::cout << "\ndelta_time_158  = " << delta_time_158        << std::endl;
                  std::cout << "\nq_bar_158                                   = " << q_bar_158[chn/2]  << std::endl;
                  //std::cout << "\nmean_time_158   = " << mean_time_158[chn/2]  << std::endl;
                  std::cout << "\ntime_158["<<chn<<"] - time_158["<<chn-1<<"]      = " << delta_time_158    << endl;
                  std::cout << "\nmean_time_Y [ns]                            = " << mean_time_Y       << endl;
                  std::cout << "\n------------------------------------------" << std::endl;
               }
            }           
         } // END if (board158_hit[ch] == 1)
      } // END LOOP ON CHANNELS


      // FILL THE HISTOGRAM
      if (bar_TOF_X > -1 && bar_TOF_Y > -1) hist_hit_XY_3d->Fill(bar_TOF_X, bar_TOF_Y);
      if (bar_TOF_X > -1 && bar_TOF_Y > -1) hist_hit_XY->Fill(bar_TOF_X, bar_TOF_Y);
      if (bar_TOF_X > -1) hist_hit_X->Fill(bar_TOF_X);
      if (bar_TOF_Y > -1) hist_hit_Y->Fill(bar_TOF_Y);

      if (mean_time_X > -1 && mean_time_Y > -1) {
         hist_mean_time->Fill(mean_time_Y - mean_time_X);
         cout << "\n*******************************************************" << endl;
         cout << "Fill Hitogram hist_mean_time with:" << endl;
         cout << "mean_time_Y - mean_time_X = " << mean_time_Y << "-" << mean_time_X << " = " << mean_time_Y - mean_time_X << endl;
         cout << "*******************************************************" << endl;
      }
      /*
      if (mean_time_Y - mean_time_X > 20) {
         cout << "\nEntrie" << jentry << endl;
         cout << "∆T = " << mean_time_Y - mean_time_X << endl;
      }
      */

      /*
      if (mean_time_X > -1 && mean_time_Y > -1) {

         Double_t errore_t[80];
         Double_t errore = 0;

         hist_mean_time->Fill(mean_time_Y - mean_time_X);
         if(mean_time_Y - mean_time_X > 20) {
            errore_t[errore] = jentry;
            cout << jentry << endl;
            errore++;

         }
      */

      
   } // END LOOP ON ENTRIES

   //============================================================================================================================

   //------------------------------
   // HITS ON BARS
   //------------------------------
   TCanvas *c_hit = new TCanvas("c_hit", "hit", 1200, 1200);
   c_hit->Divide(2,2);

   //------------------------------
   // HITS ON BARS X
   //------------------------------
   c_hit->cd(1);
   gPad->SetFrameFillColor(33);
   gPad->SetGrid();
   c_hit->SetTickx();
   c_hit->SetTicky();
   c_hit->SetLeftMargin(0.15);
   hist_hit_X->GetXaxis()->SetTitle("RearBar, LayerX");
   hist_hit_X->SetNdivisions(20,"X");
   hist_hit_X->SetFillColor(49);
   hist_hit_X->SetMinimum();
   hist_hit_X->GetYaxis()->SetTitle("Counts");
   hist_hit_X->Draw("bar2");

   //------------------------------
   // HITS ON BARS Y
   //------------------------------
   c_hit->cd(2);
   gPad->SetFrameFillColor(33);
   gPad->SetGrid();
   c_hit->SetTickx();
   c_hit->SetTicky();
   c_hit->SetLeftMargin(0.15);
   hist_hit_Y->GetXaxis()->SetTitle("FrontBar, LayerY");
   hist_hit_Y->SetNdivisions(20,"X");
   hist_hit_Y->SetFillColor(50);
   hist_hit_Y->GetYaxis()->SetTitle("Counts");
   hist_hit_Y->Draw("hbar2");

   //------------------------------
   // HITS ON BARS X-Y
   //------------------------------
   c_hit->cd(3);
   //TCanvas *c_hit_XY = new TCanvas("c_hit_XY", "hitmap",600,600);
   c_hit->SetTickx();
   c_hit->SetTicky();
   c_hit->SetLeftMargin(0.15);
   hist_hit_XY->GetXaxis()->SetTitle("RearBar, LayerX");
   hist_hit_XY->SetNdivisions(20,"X");
   hist_hit_XY->GetYaxis()->SetTitle("FrontBar, LayerY");
   hist_hit_XY->SetNdivisions(20,"Y");
   //hist_hit_XY->GetZaxis()->SetTitle("# hits");
   hist_hit_XY->Draw("colz TEXT0 SAME");//https://root.cern.ch/doc/v608/classTHistPainter.html#HP15
   //hist->Draw("colz");

   //------------------------------
   // HITS ON BARS X-Y 3D
   //------------------------------
   c_hit->cd(4);
   //TCanvas *c_hit_XY = new TCanvas("c_hit_XY", "hitmap",600,600);
   c_hit->SetTickx();
   c_hit->SetTicky();
   c_hit->SetLeftMargin(0.15);
   hist_hit_XY_3d->SetStats(0);
   hist_hit_XY_3d->GetXaxis()->SetTitle("RearBar, LayerX");
   hist_hit_XY_3d->SetNdivisions(10,"X");
   hist_hit_XY_3d->GetYaxis()->SetTitle("FrontBar, LayerY");
   hist_hit_XY_3d->SetNdivisions(10,"Y");
   //hist_hit_XY->GetZaxis()->SetTitle("# hits");
   hist_hit_XY_3d->Draw("LEGO2Z");
   
   c_hit->SaveAs("figures/hit_map.pdf");

   //------------------------------
   // HIST DELTA MEAN TIME
   //------------------------------
   TCanvas *c_delta_mean_time = new TCanvas("c_delta_mean_time", "c_delta_mean_time", 1200, 1200);
   c_delta_mean_time->cd(1);
   c_delta_mean_time->SetTickx();
   c_delta_mean_time->SetTicky();
   c_delta_mean_time->SetLeftMargin(0.15);
   hist_mean_time->GetXaxis()->SetTitle("#DeltaT_{TOFwall} [ns]");
   hist_mean_time->SetFillColor(38);
   hist_mean_time->GetYaxis()->SetTitle("Counts");
   hist_mean_time->Fit("gaus", "Q");
   hist_mean_time->Draw();

   c_delta_mean_time->SaveAs("figures/delta_mean_time.pdf");

   //==================================================================================
   // WAVEDREAM 165 - X VIEW (BAR 0 TO 7)

   //------------------------------
   // LOG(Q_L/Q_R) VS (T_L - T_R)
   //------------------------------
   TCanvas *c_q_vs_t_165 = new TCanvas("c_q_vs_t_165", "c_q_vs_t_165", 1200, 1200);
   c_q_vs_t_165->Divide(4,2);
   for (int b=0; b<8; b++) {

      c_q_vs_t_165->cd(b+1);
      c_q_vs_t_165->SetTickx();
      c_q_vs_t_165->SetTicky();

      hist_charge_time_165[b]->SetStats(0);
      hist_charge_time_165[b]->GetXaxis()->SetTitle("log(Q_L/Q_R)");
      hist_charge_time_165[b]->GetYaxis()->SetTitle("(t_L - t_R) [ns]");
      hist_charge_time_165[b]->Draw("colz");
   }
   c_q_vs_t_165->SaveAs("figures/q_vs_t_165.pdf");

   //------------------------------
   // V AMPLITUDE WD165
   //------------------------------
   TCanvas *c_v_ampl_165 = new TCanvas("c_v_ampl_165", "c_v_ampl_165", 1200, 1200);
   c_v_ampl_165->Divide(4,4);

   for (int chn=0; chn<16; chn++) {  
      c_v_ampl_165->cd(chn+1);

      //TString canvas_title_v_ampl_165 = Form("c_v_ampl_165%d", chn); 
      //TCanvas *c_v_ampl_165 = new TCanvas(canvas_title_v_ampl_165, canvas_title_v_ampl_165, 600, 600);
      c_v_ampl_165->SetTickx();
      c_v_ampl_165->SetTicky();
      c_v_ampl_165->SetLeftMargin(0.15);

      hist_v_ampl_165[chn]->SetMarkerStyle(20);
      hist_v_ampl_165[chn]->SetMarkerStyle(kFullCircle);
      hist_v_ampl_165[chn]->SetMarkerColor(kBlack);
      hist_v_ampl_165[chn]->SetFillColor(kCyan-9);
      //hist_v_ampl_165[chn]->Fit("landau", "Q");
      hist_v_ampl_165[chn]->Draw();
   }
   c_v_ampl_165->SaveAs("figures/v_ampl_165.pdf");

   //------------------------------
   // CHARGE CHANNEL WD165
   //------------------------------
   TCanvas *c_q_165 = new TCanvas("c_q_165", "c_q_165", 1200, 1200);
   c_q_165->Divide(4,4);

   for (int chn=0; chn<16; chn++) {  
      c_q_165->cd(chn+1);
      //TString canvas_title_q_165 = Form("c_q_165%d", chn); 
      //TCanvas *c_q_165 = new TCanvas(canvas_title_q_165, canvas_title_q_165, 600, 600);
      c_q_165->SetTickx();
      c_q_165->SetTicky();
      c_q_165->SetLeftMargin(0.15);

      hist_q_165[chn]->SetMarkerStyle(20);
      hist_q_165[chn]->SetMarkerStyle(kFullCircle);
      hist_q_165[chn]->SetMarkerColor(kBlack);
      hist_q_165[chn]->SetFillColor(45);
      //hist_q_165[chn]->Fit("landau", "Q");
      hist_q_165[chn]->Draw();
   }
   c_q_165->SaveAs("figures/q_165.pdf");

   //------------------------------
   // CHARGHE BAR WD165
   //------------------------------
   TCanvas *c_q_bar_165 = new TCanvas("c_q_bar_165", "c_q_bar_165", 1200, 1200);
   c_q_bar_165->Divide(4,2);

   for (int b=0; b<8; b++) {  
      c_q_bar_165->cd(b+1);
      //TString canvas_title_q_bar_165 = Form("c_q_bar_165%d", chn); 
      //TCanvas *c_q_165 = new TCanvas(canvas_title_q_bar_165, canvas_title_q_bar_165, 600, 600);
      c_q_bar_165->SetTickx();
      c_q_bar_165->SetTicky();
      c_q_bar_165->SetLeftMargin(0.15);

      hist_q_bar_165[b]->SetMarkerStyle(20);
      hist_q_bar_165[b]->SetMarkerStyle(kFullCircle);
      hist_q_bar_165[b]->SetMarkerColor(kBlack);
      hist_q_bar_165[b]->SetFillColor(45);
      hist_q_bar_165[b]->Fit("landau", "Q");
      hist_q_bar_165[b]->Draw();
   }
   c_q_bar_165->SaveAs("figures/q_bar_165.pdf");
      
   //------------------------------
   // DELTA TIME WD165
   //------------------------------
   TCanvas *c_delta_time_165 = new TCanvas("c_delta_time_165", "Delta t 165",1200,1200);
   c_delta_time_165->Divide(4,2);

   for (int b=0; b<8; b++) {  
      c_delta_time_165->cd(b+1);
      //TString canvas_title_delta_time_165 = Form("c_delta_time_165bar%d",b); 
      //TCanvas *c_delta_time_165 = new TCanvas(canvas_title_delta_time_165, canvas_title_delta_time_165, 600, 600);
      c_delta_time_165->SetTickx();
      c_delta_time_165->SetTicky();
      c_delta_time_165->SetLeftMargin(0.15);

      hist_delta_time_165[b]->SetMarkerStyle(20);
      hist_delta_time_165[b]->SetMarkerStyle(kFullCircle);
      hist_delta_time_165[b]->SetMarkerColor(kBlack);
      hist_delta_time_165[b]->SetFillColor(5); // Yellow
      hist_delta_time_165[b]->Fit("pol0", "Q");
      hist_delta_time_165[b]->Draw();
   }
   c_delta_time_165->SaveAs("figures/delta_time_165.pdf");

   //==================================================================================
   // WAVEDREAM 166

   //------------------------------
   // LOG(Q_L/Q_R) VS (T_L - T_R)
   //------------------------------
   TCanvas *c_q_vs_t_166 = new TCanvas("c_q_vs_t_166", "c_q_vs_t_166", 1200, 1200);
   c_q_vs_t_166->Divide(4,2);

   for (int b=0; b<8; b++) {
      c_q_vs_t_166->cd(b+1);
      c_q_vs_t_166->SetTickx();
      c_q_vs_t_166->SetTicky();

      hist_charge_time_166[b]->SetStats(0);
      hist_charge_time_166[b]->GetXaxis()->SetTitle("log(Q_L/Q_R)");
      hist_charge_time_166[b]->GetYaxis()->SetTitle("(t_L - t_R) [ns]");
      hist_charge_time_166[b]->Draw("colz");
   }
   c_q_vs_t_166->SaveAs("figures/q_vs_t_166.pdf");


   //------------------------------
   // V AMPLITUDE WD166
   //------------------------------
   TCanvas *c_v_ampl_166 = new TCanvas("c_v_ampl_166", "c_v_ampl_166", 1200, 1200);
   c_v_ampl_166->Divide(4,4);

   for (int chn=0; chn<16; chn++) {  
      c_v_ampl_166->cd(chn+1);

      //TString canvas_title_v_ampl_166 = Form("c_v_ampl_166%d", chn); 
      //TCanvas *c_v_ampl_166 = new TCanvas(canvas_title_v_ampl_166, canvas_title_v_ampl_166, 600, 600);
      c_v_ampl_166->SetTickx();
      c_v_ampl_166->SetTicky();
      c_v_ampl_166->SetLeftMargin(0.15);

      hist_v_ampl_166[chn]->SetMarkerStyle(20);
      hist_v_ampl_166[chn]->SetMarkerStyle(kFullCircle);
      hist_v_ampl_166[chn]->SetMarkerColor(kBlack);
      hist_v_ampl_166[chn]->SetFillColor(kCyan-9);
      //hist_v_ampl_166[chn]->Fit("landau", "Q");
      hist_v_ampl_166[chn]->Draw();
   }
   c_v_ampl_166->SaveAs("figures/v_ampl_166.pdf");

   //------------------------------
   // CHARGE CHANNEL WD166
   //------------------------------
   TCanvas *c_q_166 = new TCanvas("c_q_166", "c_q_166", 1200, 1200);
   c_q_166->Divide(4,4);

   for (int chn=0; chn<16; chn++) {  
      c_q_166->cd(chn+1);
      //TString canvas_title_q_166 = Form("c_q_166%d", chn); 
      //TCanvas *c_q_166 = new TCanvas(canvas_title_q_166, canvas_title_q_166, 600, 600);
      c_q_166->SetTickx();
      c_q_166->SetTicky();
      c_q_166->SetLeftMargin(0.15);

      hist_q_166[chn]->SetMarkerStyle(20);
      hist_q_166[chn]->SetMarkerStyle(kFullCircle);
      hist_q_166[chn]->SetMarkerColor(kBlack);
      hist_q_166[chn]->SetFillColor(45);
      //hist_q_166[chn]->Fit("landau", "Q");
      hist_q_166[chn]->Draw();
   }
   c_q_166->SaveAs("figures/q_166.pdf");

   //------------------------------
   // CHARGHE BAR WD166
   //------------------------------
   TCanvas *c_q_bar_166 = new TCanvas("c_q_bar_166", "c_q_bar_166", 1200, 1200);
   c_q_bar_166->Divide(4,2);

   for (int b=0; b<8; b++) {  
      c_q_bar_166->cd(b+1);
      //TString canvas_title_q_bar_166 = Form("c_q_bar_166%d", chn); 
      //TCanvas *c_q_166 = new TCanvas(canvas_title_q_bar_166, canvas_title_q_bar_166, 600, 600);
      c_q_bar_166->SetTickx();
      c_q_bar_166->SetTicky();
      c_q_bar_166->SetLeftMargin(0.15);

      hist_q_bar_166[b]->SetMarkerStyle(20);
      hist_q_bar_166[b]->SetMarkerStyle(kFullCircle);
      hist_q_bar_166[b]->SetMarkerColor(kBlack);
      hist_q_bar_166[b]->SetFillColor(45);
      hist_q_bar_166[b]->Fit("landau", "Q");
      hist_q_bar_166[b]->Draw();
   }
   c_q_bar_166->SaveAs("figures/q_bar_166-.pdf");
      
   //------------------------------
   // DELTA TIME WD166
   //------------------------------
   TCanvas *c_delta_time_166 = new TCanvas("c_delta_time_166", "Delta t 166",1200,1200);
   c_delta_time_166->Divide(4,2);

   for (int b=0; b<8; b++) {  
      c_delta_time_166->cd(b+1);
      //TString canvas_title_delta_time_166 = Form("c_delta_time_166bar%d",b); 
      //TCanvas *c_delta_time_166 = new TCanvas(canvas_title_delta_time_166, canvas_title_delta_time_166, 600, 600);
      c_delta_time_166->SetTickx();
      c_delta_time_166->SetTicky();
      c_delta_time_166->SetLeftMargin(0.15);

      hist_delta_time_166[b]->SetMarkerStyle(20);
      hist_delta_time_166[b]->SetMarkerStyle(kFullCircle);
      hist_delta_time_166[b]->SetMarkerColor(kBlack);
      hist_delta_time_166[b]->SetFillColor(5); // Yellow
      hist_delta_time_166[b]->Fit("pol0", "Q");
      hist_delta_time_166[b]->Draw();
   }
   c_delta_time_166->SaveAs("figures/delta_time_166.pdf");

   //==================================================================================
   // WAVEDREAM 170 - X VIEW (BAR 11 TO 18)

   //------------------------------
   // LOG(Q_L/Q_R) VS (T_L - T_R)
   //------------------------------
   TCanvas *c_q_vs_t_170 = new TCanvas("c_q_vs_t_170", "c_q_vs_t_170", 1200, 1200);
   c_q_vs_t_170->Divide(4,2);

   for (int b=0; b<8; b++) {
      c_q_vs_t_170->cd(b+1);
      c_q_vs_t_170->SetTickx();
      c_q_vs_t_170->SetTicky();

      hist_charge_time_170[b]->SetStats(0);
      hist_charge_time_170[b]->GetXaxis()->SetTitle("log(Q_L/Q_R)");
      hist_charge_time_170[b]->GetYaxis()->SetTitle("(t_L - t_R) [ns]");
      hist_charge_time_170[b]->Draw("colz");
   }
   c_q_vs_t_170->SaveAs("figures/q_vs_t_170.pdf");

   //------------------------------
   // V AMPLITUDE WD170
   //------------------------------
   TCanvas *c_v_ampl_170 = new TCanvas("c_v_ampl_170", "c_v_ampl_170", 1200, 1200);
   c_v_ampl_170->Divide(4,4);

   for (int chn=0; chn<16; chn++) {  
      c_v_ampl_170->cd(chn+1);

      //TString canvas_title_v_ampl_170 = Form("c_v_ampl_170%d", chn);
      //TCanvas *c_v_ampl_170 = new TCanvas(canvas_title_v_ampl_170, canvas_title_v_ampl_170, 600, 600);
      c_v_ampl_170->SetTickx();
      c_v_ampl_170->SetTicky();
      c_v_ampl_170->SetLeftMargin(0.15);

      hist_v_ampl_170[chn]->SetMarkerStyle(20);
      hist_v_ampl_170[chn]->SetMarkerStyle(kFullCircle);
      hist_v_ampl_170[chn]->SetMarkerColor(kBlack);
      hist_v_ampl_170[chn]->SetFillColor(kCyan-9);
      //hist_v_ampl_170[chn]->Fit("landau", "Q");
      hist_v_ampl_170[chn]->Draw();
   }
   c_v_ampl_170->SaveAs("figures/v_ampl_170.pdf");

   //------------------------------
   // CHARGE CHANNEL WD170
   //------------------------------
   TCanvas *c_q_170 = new TCanvas("c_q_170", "c_q_170", 1200, 1200);
   c_q_170->Divide(4,4);

   for (int chn=0; chn<16; chn++) {  
      c_q_170->cd(chn+1);
      //TString canvas_title_q_170 = Form("c_q_170%d", chn); 
      //TCanvas *c_q_170 = new TCanvas(canvas_title_q_170, canvas_title_q_170, 600, 600);
      c_q_170->SetTickx();
      c_q_170->SetTicky();
      c_q_170->SetLeftMargin(0.15);

      hist_q_170[chn]->SetMarkerStyle(20);
      hist_q_170[chn]->SetMarkerStyle(kFullCircle);
      hist_q_170[chn]->SetMarkerColor(kBlack);
      hist_q_170[chn]->SetFillColor(45);
      //hist_q_170[chn]->Fit("landau", "Q");
      hist_q_170[chn]->Draw();
   }
   c_q_170->SaveAs("figures/q_170.pdf");

   //------------------------------
   // CHARGHE BAR WD170
   //------------------------------
   TCanvas *c_q_bar_170 = new TCanvas("c_q_bar_170", "c_q_bar_170", 1200, 1200);
   c_q_bar_170->Divide(4,2);

   for (int b=0; b<8; b++) {  
      c_q_bar_170->cd(b+1);
      //TString canvas_title_q_bar_170 = Form("c_q_bar_170%d", chn);
      //TCanvas *c_q_170 = new TCanvas(canvas_title_q_bar_170, canvas_title_q_bar_170, 600, 600);
      c_q_bar_170->SetTickx();
      c_q_bar_170->SetTicky();
      c_q_bar_170->SetLeftMargin(0.15);

      hist_q_bar_170[b]->SetMarkerStyle(20);
      hist_q_bar_170[b]->SetMarkerStyle(kFullCircle);
      hist_q_bar_170[b]->SetMarkerColor(kBlack);
      hist_q_bar_170[b]->SetFillColor(45);
      hist_q_bar_170[b]->Fit("landau", "Q");
      hist_q_bar_170[b]->Draw();
   }
   c_q_bar_170->SaveAs("figures/q_bar_170.pdf");
      
   //------------------------------
   // DELTA TIME WD170
   //------------------------------
   TCanvas *c_delta_time_170 = new TCanvas("c_delta_time_170", "Delta t 170",1200,1200);
   c_delta_time_170->Divide(4,2);

   for (int b=0; b<8; b++) {  
      c_delta_time_170->cd(b+1);
      //TString canvas_title_delta_time_170 = Form("c_delta_time_170bar%d",b);
      //TCanvas *c_delta_time_170 = new TCanvas(canvas_title_delta_time_170, canvas_title_delta_time_170, 600, 600);
      c_delta_time_170->SetTickx();
      c_delta_time_170->SetTicky();
      c_delta_time_170->SetLeftMargin(0.15);

      hist_delta_time_170[b]->SetMarkerStyle(20);
      hist_delta_time_170[b]->SetMarkerStyle(kFullCircle);
      hist_delta_time_170[b]->SetMarkerColor(kBlack);
      hist_delta_time_170[b]->SetFillColor(5); // Yellow
      hist_delta_time_170[b]->Fit("pol0", "Q");
      hist_delta_time_170[b]->Draw();
   }
   c_delta_time_170->SaveAs("figures/delta_time_170.pdf");

   //==================================================================================
   // WAVEDREAM 167 

   //------------------------------
   // LOG(Q_L/Q_R) VS (T_L - T_R)
   //------------------------------
   TCanvas *c_q_vs_t_167 = new TCanvas("c_q_vs_t_167", "c_q_vs_t_167", 1200, 1200);
   c_q_vs_t_167->Divide(4,2);

   for (int b=0; b<8; b++) {
      c_q_vs_t_167->cd(b+1);
      c_q_vs_t_167->SetTickx();
      c_q_vs_t_167->SetTicky();

      hist_charge_time_167[b]->SetStats(0);
      hist_charge_time_167[b]->GetXaxis()->SetTitle("log(Q_L/Q_R)");
      hist_charge_time_167[b]->GetYaxis()->SetTitle("(t_L - t_R) [ns]");
      hist_charge_time_167[b]->Draw("colz");
   }
   c_q_vs_t_167->SaveAs("figures/q_vs_t_167.pdf");


   //------------------------------
   // V AMPLITUDE WD167
   //------------------------------
   TCanvas *c_v_ampl_167 = new TCanvas("c_v_ampl_167", "c_v_ampl_167", 1200, 1200);
   c_v_ampl_167->Divide(4,4);

   for (int chn=0; chn<16; chn++) {
      c_v_ampl_167->cd(chn+1);

      //TString canvas_title_v_ampl_167 = Form("c_v_ampl_167%d", chn); 
      //TCanvas *c_v_ampl_167 = new TCanvas(canvas_title_v_ampl_167, canvas_title_v_ampl_167, 600, 600);
      c_v_ampl_167->SetTickx();
      c_v_ampl_167->SetTicky();
      c_v_ampl_167->SetLeftMargin(0.15);

      hist_v_ampl_167[chn]->SetMarkerStyle(20);
      hist_v_ampl_167[chn]->SetMarkerStyle(kFullCircle);
      hist_v_ampl_167[chn]->SetMarkerColor(kBlack);
      hist_v_ampl_167[chn]->SetFillColor(kCyan-9);
      //hist_v_ampl_167[chn]->Fit("landau", "Q");
      hist_v_ampl_167[chn]->Draw();
   }
   c_v_ampl_167->SaveAs("figures/v_ampl_167.pdf");

   //------------------------------
   // CHARGE CHANNEL WD167
   //------------------------------
   TCanvas *c_q_167 = new TCanvas("c_q_167", "c_q_167", 1200, 1200);
   c_q_167->Divide(4,4);

   for (int chn=0; chn<16; chn++) {
      c_q_167->cd(chn+1);
      //TString canvas_title_q_167 = Form("c_q_167%d", chn); 
      //TCanvas *c_q_167 = new TCanvas(canvas_title_q_167, canvas_title_q_167, 600, 600);
      c_q_167->SetTickx();
      c_q_167->SetTicky();
      c_q_167->SetLeftMargin(0.15);

      hist_q_167[chn]->SetMarkerStyle(20);
      hist_q_167[chn]->SetMarkerStyle(kFullCircle);
      hist_q_167[chn]->SetMarkerColor(kBlack);
      hist_q_167[chn]->SetFillColor(45);
      //hist_q_167[chn]->Fit("landau", "Q");
      hist_q_167[chn]->Draw();
   }
   c_q_167->SaveAs("figures/q_167.pdf");

   //------------------------------
   // CHARGHE BAR WD167
   //------------------------------
   TCanvas *c_q_bar_167 = new TCanvas("c_q_bar_167", "c_q_bar_167", 1200, 1200);
   c_q_bar_167->Divide(4,2);

   for (int b=0; b<8; b++) {  
      c_q_bar_167->cd(b+1);
      //TString canvas_title_q_bar_167 = Form("c_q_bar_167%d", chn); 
      //TCanvas *c_q_167 = new TCanvas(canvas_title_q_bar_167, canvas_title_q_bar_167, 600, 600);
      c_q_bar_167->SetTickx();
      c_q_bar_167->SetTicky();
      c_q_bar_167->SetLeftMargin(0.15);

      hist_q_bar_167[b]->SetMarkerStyle(20);
      hist_q_bar_167[b]->SetMarkerStyle(kFullCircle);
      hist_q_bar_167[b]->SetMarkerColor(kBlack);
      hist_q_bar_167[b]->SetFillColor(45);
      hist_q_bar_167[b]->Fit("landau", "Q");
      hist_q_bar_167[b]->Draw();
   }
   c_q_bar_167->SaveAs("figures/q_bar_167.pdf");
      
   //------------------------------
   // DELTA TIME WD167
   //------------------------------
   TCanvas *c_delta_time_167 = new TCanvas("c_delta_time_167", "Delta t 167",1200,1200);
   c_delta_time_167->Divide(4,2);

   for (int b=0; b<8; b++) {  
      c_delta_time_167->cd(b+1);
      //TString canvas_title_delta_time_167 = Form("c_delta_time_167bar%d",b); 
      //TCanvas *c_delta_time_167 = new TCanvas(canvas_title_delta_time_167, canvas_title_delta_time_167, 600, 600);
      c_delta_time_167->SetTickx();
      c_delta_time_167->SetTicky();
      c_delta_time_167->SetLeftMargin(0.15);

      hist_delta_time_167[b]->SetMarkerStyle(20);
      hist_delta_time_167[b]->SetMarkerStyle(kFullCircle);
      hist_delta_time_167[b]->SetMarkerColor(kBlack);
      hist_delta_time_167[b]->SetFillColor(5); // Yellow
      hist_delta_time_167[b]->Fit("pol0", "Q");
      hist_delta_time_167[b]->Draw();
   }
   c_delta_time_167->SaveAs("figures/delta_time_167.pdf");

   //==================================================================================
   // WAVEDREAM 168


   //------------------------------
   // LOG(Q_L/Q_R) VS (T_L - T_R)
   //------------------------------
   TCanvas *c_q_vs_t_168 = new TCanvas("c_q_vs_t_168", "c_q_vs_t_168", 1200, 1200);
   c_q_vs_t_168->Divide(4,2);

   for (int b=0; b<8; b++) {
      c_q_vs_t_168->cd(b+1);
      c_q_vs_t_168->SetTickx();
      c_q_vs_t_168->SetTicky();

      hist_charge_time_168[b]->SetStats(0);
      hist_charge_time_168[b]->GetXaxis()->SetTitle("log(Q_L/Q_R)");
      hist_charge_time_168[b]->GetYaxis()->SetTitle("(t_L - t_R) [ns]");
      hist_charge_time_168[b]->Draw("colz");
   }
   c_q_vs_t_168->SaveAs("figures/q_vs_t_168.pdf");

   //------------------------------
   // V AMPLITUDE WD168
   //------------------------------
   TCanvas *c_v_ampl_168 = new TCanvas("c_v_ampl_168", "c_v_ampl_168", 1200, 1200);
   c_v_ampl_168->Divide(4,4);

   for (int chn=0; chn<16; chn++) {
      c_v_ampl_168->cd(chn+1);

      //TString canvas_title_v_ampl_168 = Form("c_v_ampl_168%d", chn); 
      //TCanvas *c_v_ampl_168 = new TCanvas(canvas_title_v_ampl_168, canvas_title_v_ampl_168, 600, 600);
      c_v_ampl_168->SetTickx();
      c_v_ampl_168->SetTicky();
      c_v_ampl_168->SetLeftMargin(0.15);

      hist_v_ampl_168[chn]->SetMarkerStyle(20);
      hist_v_ampl_168[chn]->SetMarkerStyle(kFullCircle);
      hist_v_ampl_168[chn]->SetMarkerColor(kBlack);
      hist_v_ampl_168[chn]->SetFillColor(kCyan-9);
      //hist_v_ampl_168[chn]->Fit("landau", "Q");
      hist_v_ampl_168[chn]->Draw();
   }
   c_v_ampl_168->SaveAs("figures/v_ampl_168.pdf");

   //------------------------------
   // CHARGE CHANNEL WD168
   //------------------------------
   TCanvas *c_q_168 = new TCanvas("c_q_168", "c_q_168", 1200, 1200);
   c_q_168->Divide(4,4);

   for (int chn=0; chn<16; chn++) {
      c_q_168->cd(chn+1);
      //TString canvas_title_q_168 = Form("c_q_168%d", chn); 
      //TCanvas *c_q_168 = new TCanvas(canvas_title_q_168, canvas_title_q_168, 600, 600);
      c_q_168->SetTickx();
      c_q_168->SetTicky();
      c_q_168->SetLeftMargin(0.15);

      hist_q_168[chn]->SetMarkerStyle(20);
      hist_q_168[chn]->SetMarkerStyle(kFullCircle);
      hist_q_168[chn]->SetMarkerColor(kBlack);
      hist_q_168[chn]->SetFillColor(45);
      //hist_q_168[chn]->Fit("landau", "Q");
      hist_q_168[chn]->Draw();
   }
   c_q_168->SaveAs("figures/q_168.pdf");

   //------------------------------
   // CHARGHE BAR WD168
   //------------------------------
   TCanvas *c_q_bar_168 = new TCanvas("c_q_bar_168", "c_q_bar_168", 1200, 1200);
   c_q_bar_168->Divide(4,2);

   for (int b=0; b<8; b++) {  
      c_q_bar_168->cd(b+1);
      //TString canvas_title_q_bar_168 = Form("c_q_bar_168%d", chn);
      //TCanvas *c_q_168 = new TCanvas(canvas_title_q_bar_168, canvas_title_q_bar_168, 600, 600);
      c_q_bar_168->SetTickx();
      c_q_bar_168->SetTicky();
      c_q_bar_168->SetLeftMargin(0.15);

      hist_q_bar_168[b]->SetMarkerStyle(20);
      hist_q_bar_168[b]->SetMarkerStyle(kFullCircle);
      hist_q_bar_168[b]->SetMarkerColor(kBlack);
      hist_q_bar_168[b]->SetFillColor(45);
      hist_q_bar_168[b]->Fit("landau", "Q");
      hist_q_bar_168[b]->Draw();
   }
   c_q_bar_168->SaveAs("figures/q_bar_168.pdf");
      
   //------------------------------
   // DELTA TIME WD168
   //------------------------------
   TCanvas *c_delta_time_168 = new TCanvas("c_delta_time_168", "Delta t 168",1200,1200);
   c_delta_time_168->Divide(4,2);

   for (int b=0; b<8; b++) {  
      c_delta_time_168->cd(b+1);
      //TString canvas_title_delta_time_168 = Form("c_delta_time_168bar%d",b); 
      //TCanvas *c_delta_time_168 = new TCanvas(canvas_title_delta_time_168, canvas_title_delta_time_168, 600, 600);
      c_delta_time_168->SetTickx();
      c_delta_time_168->SetTicky();
      c_delta_time_168->SetLeftMargin(0.15);

      hist_delta_time_168[b]->SetMarkerStyle(20);
      hist_delta_time_168[b]->SetMarkerStyle(kFullCircle);
      hist_delta_time_168[b]->SetMarkerColor(kBlack);
      hist_delta_time_168[b]->SetFillColor(5); // Yellow
      hist_delta_time_168[b]->Fit("pol0", "Q");
      hist_delta_time_168[b]->Draw();
   }
   c_delta_time_168->SaveAs("figures/delta_time_168.pdf");

   //==================================================================================
   // WAVEDREAM 158

   //------------------------------
   // V AMPLITUDE WD158
   //------------------------------
   TCanvas *c_v_ampl_158 = new TCanvas("c_v_ampl_158", "c_v_ampl_158", 1200, 1200);
   c_v_ampl_158->Divide(4,4);

   for (int chn=0; chn<16; chn++) {
      c_v_ampl_158->cd(chn+1);

      //TString canvas_title_v_ampl_158 = Form("c_v_ampl_158%d", chn);
      //TCanvas *c_v_ampl_158 = new TCanvas(canvas_title_v_ampl_158, canvas_title_v_ampl_158, 600, 600);
      c_v_ampl_158->SetTickx();
      c_v_ampl_158->SetTicky();
      c_v_ampl_158->SetLeftMargin(0.15);

      hist_v_ampl_158[chn]->SetMarkerStyle(20);
      hist_v_ampl_158[chn]->SetMarkerStyle(kFullCircle);
      hist_v_ampl_158[chn]->SetMarkerColor(kBlack);
      hist_v_ampl_158[chn]->SetFillColor(kCyan-9);
      //hist_v_ampl_158[chn]->Fit("landau", "Q");
      hist_v_ampl_158[chn]->Draw();
   }
   c_v_ampl_158->SaveAs("figures/v_ampl_158.pdf");

   //------------------------------
   // CHARGE CHANNEL WD158
   //------------------------------
   TCanvas *c_q_158 = new TCanvas("c_q_158", "c_q_158", 1200, 1200);
   c_q_158->Divide(4,4);

   for (int chn=0; chn<16; chn++) {
      c_q_158->cd(chn+1);
      //TString canvas_title_q_158 = Form("c_q_158%d", chn);
      //TCanvas *c_q_158 = new TCanvas(canvas_title_q_158, canvas_title_q_158, 600, 600);
      c_q_158->SetTickx();
      c_q_158->SetTicky();
      c_q_158->SetLeftMargin(0.15);

      hist_q_158[chn]->SetMarkerStyle(20);
      hist_q_158[chn]->SetMarkerStyle(kFullCircle);
      hist_q_158[chn]->SetMarkerColor(kBlack);
      hist_q_158[chn]->SetFillColor(45);
      //hist_q_158[chn]->Fit("landau", "Q");
      hist_q_158[chn]->Draw();
   }
   c_q_158->SaveAs("figures/q_158.pdf");

   //------------------------------
   // CHARGHE BAR WD158
   //------------------------------
   TCanvas *c_q_bar_158 = new TCanvas("c_q_bar_158", "c_q_bar_158", 1200, 1200);
   c_q_bar_158->Divide(4,2);

   for (int b=0; b<8; b++) {  
      c_q_bar_158->cd(b+1);
      //TString canvas_title_q_bar_158 = Form("c_q_bar_158%d", chn);
      //TCanvas *c_q_158 = new TCanvas(canvas_title_q_bar_158, canvas_title_q_bar_158, 600, 600);
      c_q_bar_158->SetTickx();
      c_q_bar_158->SetTicky();
      c_q_bar_158->SetLeftMargin(0.15);

      hist_q_bar_158[b]->SetMarkerStyle(20);
      hist_q_bar_158[b]->SetMarkerStyle(kFullCircle);
      hist_q_bar_158[b]->SetMarkerColor(kBlack);
      hist_q_bar_158[b]->SetFillColor(45);
      hist_q_bar_158[b]->Fit("landau", "Q");
      hist_q_bar_158[b]->Draw();
   }
   c_q_bar_158->SaveAs("figures/q_bar_158.pdf");
      
   //------------------------------
   // DELTA TIME WD158
   //------------------------------
   TCanvas *c_delta_time_158 = new TCanvas("c_delta_time_158", "Delta t 158",1200,1200);
   c_delta_time_158->Divide(4,2);

   for (int b=0; b<8; b++) {  
      c_delta_time_158->cd(b+1);
      //TString canvas_title_delta_time_158 = Form("c_delta_time_158bar%d",b);
      //TCanvas *c_delta_time_158 = new TCanvas(canvas_title_delta_time_158, canvas_title_delta_time_158, 600, 600);
      c_delta_time_158->SetTickx();
      c_delta_time_158->SetTicky();
      c_delta_time_158->SetLeftMargin(0.15);

      hist_delta_time_158[b]->SetMarkerStyle(20);
      hist_delta_time_158[b]->SetMarkerStyle(kFullCircle);
      hist_delta_time_158[b]->SetMarkerColor(kBlack);
      hist_delta_time_158[b]->SetFillColor(5); // Yellow
      hist_delta_time_158[b]->Fit("pol0", "Q");
      hist_delta_time_158[b]->Draw();
   }
   c_delta_time_158->SaveAs("figures/delta_time_158.pdf");


   //------------------------------
   // SAVE HISROGRAMS IN ROOT FILE
   //------------------------------
   //hfile.Write();
}
