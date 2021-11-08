/*

 Name:           read_binary.C
 Created by:     Stefan Ritt <stefan.ritt@psi.ch>
 Date:           December 21st, 2016

 Purpose:        Example program under ROOT to read a binary data file written
 by the DRSOsc program. Decode time and voltages from waveforms
 and display them as a graph. Put values into a ROOT Tree for
 further analysis.

 To run it, do:

 - Crate a file test.dat via the "Save" button in the browser connected
 to the wds server
 - start ROOT
 root [0] .L read_binary.C+
 root [1] decode("test.dat");

 */


#include <string.h>
#include <stdio.h>
#include "TFile.h"
#include "TH1D.h"
#include "TTree.h"
#include "TString.h"
#include "TGraph.h"
#include "TCanvas.h"
#include "Getline.h"

typedef struct {
   char           tag[3];
   char           version;
} FHEADER;

typedef struct {
   char           time_header[4];
} THEADER;

typedef struct {
  char           bn[2];
  unsigned short board_serial_number;
} BHEADER;

typedef struct {
  float          board_temperature;
  float          board_range;
  unsigned short sampling_frequency;
  unsigned short flags;
} DRSBHEADER;

typedef struct {
   char           event_header[4];
   unsigned short event_serial_number;
   unsigned short trigger_type;
   unsigned int serial_trigger_data;
} EHEADER;

typedef struct {
   char           c[1];
   char           cn[3];
} CHEADER;

typedef struct {
   unsigned short frontendsettings;
   unsigned short trigger_cell;
} DRSCHEADER;

/*-----------------------------------------------------------------------------*/

void decode(const char *filename) {
   FHEADER  fh;
   THEADER  th;
   BHEADER  bh;
   DRSBHEADER  drsbh;
   EHEADER  eh;
   CHEADER  ch;
   DRSCHEADER drsch[18];

   unsigned short voltage[1024];
   unsigned short adc_voltage[2048];
   unsigned char tdc_data[512];
   unsigned long trg_data[512];
   unsigned long scaler_data[18];
   unsigned long scaler_time;
   bool triggered[18];
   Double_t header_array[4];

   typedef struct {
     Double_t waveform[18][1024];
     Double_t time[18][1024];
     UShort_t adc_waveform[16][2048];
     UChar_t tdc_waveform[16][512];
     ULong_t trigger_data[512];
     ULong_t scaler[16];
     ULong_t event_time;
     Double_t rate[16];
     UShort_t hit[16];
   } WDBDATA;
   std::map<unsigned short, WDBDATA> data;
   

   typedef struct {
     ULong_t in_waveform[16][128];
     ULong_t out_waveform[128];
     ULong_t gent_waveform[32];
     UInt_t trigger_counter[64];
     UInt_t trigger_type;
     UInt_t total_time;
     UInt_t live_time;
     UInt_t event_counter;
     UInt_t scifi_scaler[44];
     UInt_t scifi_coincidence[441];
   } TCBDATA;
   std::map<unsigned short, TCBDATA> data_tcb;

   typedef struct{
      float bin_width[18][1024];
   } WDBBIN;
   std::map<unsigned short, WDBBIN> bins;

   int i, j, b, chn, n, chn_index;
   double t1, t2, dt;
   char rootfile[256];

   // open the binary waveform file
   FILE *f = fopen(Form("%s", filename), "r");
   if (f == NULL) {
      printf("Cannot find file \'%s\'\n", filename);
      return;
   }

   FILE *f2 = fopen("test.dat", "wb");

   //open the root file
   strcpy(rootfile, filename);
   if (strchr(rootfile, '.'))
      *strchr(rootfile, '.') = 0;
   strcat(rootfile, ".root");
   TFile *outfile = new TFile(rootfile, "RECREATE");

   // define the rec tree
   TTree *rec = new TTree("rec","rec");

   // create canvas
   TCanvas *c1 = new TCanvas();

   // create graph
   TH1D * h = new TH1D("h", "h", 8, -0.5, 7.5);

   // read file header
   fread(&fh, sizeof(fh), 1, f);
   if (fh.tag[0] != 'W' || fh.tag[1] != 'D' || fh.tag[2] != 'Q') {
      printf("Found invalid file header in file \'%s\', aborting.\n", filename);
      return;
   }

   if (fh.version != '0') {
      printf("Found invalid file version \'%c\' in file \'%s\', should be \'8\', aborting.\n", fh.version, filename);
      return;
   }

   // read time header
   fread(&th, sizeof(th), 1, f);
   if (memcmp(th.time_header, "TIME", 4) != 0) {
      printf("Invalid time header in file \'%s\', aborting.\n", filename);
      return;
   }

   for (b = 0 ; ; b++) {
      // read board header
      fread(&bh, sizeof(bh), 1, f);
      if (memcmp(bh.bn, "B#", 2) == 0) {

         printf("Found data for board #%d\n", bh.board_serial_number);

         //branch output tree to house the data
         rec->Branch(Form("board%02d", bh.board_serial_number), &data[bh.board_serial_number] ,"waveform[18][1024]/D:time[18][1024]/D:adc_waveform[16][2048]/s:tdc_waveform[16][512]/b:trigger_data[512]/l:scaler[16]/l:event_time/l:rate[16]/D:hit[16]/s");

         //reset timers and scalers
         for(int i=0; i<16; i++)
            data[bh.board_serial_number].scaler[i]=0;
         data[bh.board_serial_number].event_time=0;

         // read time bin widths
         memset(bins[bh.board_serial_number].bin_width, sizeof(WDBBIN), 0);
         for (chn=0 ; chn<18 ; chn++) {
            fread(&ch, sizeof(ch), 1, f);
            if (ch.c[0] != 'C') {
               // event header found
               fseek(f, -4, SEEK_CUR);
               break;
            }
            i = (ch.cn[1] - '0')*10 + ch.cn[2] - '0';
            printf("Found timing calibration for channel #%d\n", i);
            fread(bins[bh.board_serial_number].bin_width[i], sizeof(float), 1024, f);
         }
      } else if (memcmp(bh.bn, "T#", 2) == 0) {
         printf("expecting TCB %d\n", bh.board_serial_number);
         //branch output tree to house the data
         rec->Branch(Form("tcb%02d", bh.board_serial_number), &data_tcb[bh.board_serial_number] ,"in_waveform[16][128]/l:out_waveform[128]/l:gent_waveform[32]/l:trigger_counter[64]/i:trigger_type/i:total_time/i:live_time/i:event_counter/i:scifi_scaler[44]/i:scifi_coincidence[441]/i");
      } else {
         // probably event header found
         fseek(f, -4, SEEK_CUR);
         break;
      }

   }

   // loop over all events in data file
   for (n=0 ; ; n++) {
      // read event header
      i = fread(&eh, sizeof(eh), 1, f);
      if (i < 1 || memcmp(eh.event_header, "EHDR", 4) != 0)
         break;

      printf("Found event #%d, type =%d , serial = %d\n", eh.event_serial_number, eh.trigger_type, eh.serial_trigger_data);

      //RESET ALL WFM



      // loop over all boards in data file
      for (b=0 ; !feof(f); b++) {

         // read board header
         int bhsize = fread(&bh, sizeof(bh), 1, f);
         if (memcmp(bh.bn, "B#", 2) == 0) {

            // read board header
            fread(&drsbh, sizeof(drsbh), 1, f);

            WDBBIN &this_bins =  bins[bh.board_serial_number];
            WDBDATA &this_data =  data[bh.board_serial_number];
            for (i=0; i<18; i++)
            {
               triggered[i]=false;
               data[bh.board_serial_number].hit[i] = 0;
            }


            // reach channel data
            for (chn=0 ; !feof(f) ; chn++) {

               // read channel header
               fread(&ch, sizeof(ch), 1, f);
               if (ch.c[0] != 'C' && ch.c[0] != 'A' && (ch.c[0] != 'T' || ch.cn[0] == '#') && ch.c[0] != 'S') {
                  // event header found
                  fseek(f, -4, SEEK_CUR);
                  break;
               }
               chn_index = (ch.cn[1] - '0')*10 + ch.cn[2] - '0';
               triggered[chn_index]=true;
               data[bh.board_serial_number].hit[chn_index]= 1;

               if(ch.c[0] == 'C'){
                  // read trigger cell and frontend settings
                  fread(drsch+chn_index, sizeof(drsch[0]), 1, f);

                  fread(voltage, sizeof(short), 1024, f);

                  for (i=0 ; i<1024 ; i++) {
                     data[bh.board_serial_number].waveform[chn_index][i] = (voltage[i] / 65536. + drsbh.board_range/1000.0 - 0.5);
                     // calculate time for this cell
                     for (j=0,data[bh.board_serial_number].time[chn_index][i]=0 ; j<i ; j++)
                        this_data.time[chn_index][i] += this_bins.bin_width[chn_index][(j+drsch[chn_index].trigger_cell) % 1024];
                  }



               } else if(ch.c[0] == 'A') {
                  //ADC
                  fread(adc_voltage, sizeof(short), 2048, f);
                  WDBDATA &this_data =  data[bh.board_serial_number];
                  for (i=0 ; i<2048 ; i++) {
                     this_data.adc_waveform[chn_index][i] = adc_voltage[i];
                  }
               } else if(ch.c[0] == 'T') {
                  if(ch.cn[0] == '0'){
                     //TDC
                     fread(tdc_data, sizeof(char), 512, f);
                     WDBDATA &this_data =  data[bh.board_serial_number];
                     for (i=0 ; i<512 ; i++) {
                        this_data.tdc_waveform[chn_index][i] = tdc_data[i];
                     }
                  } else if(ch.cn[0] == 'R'){
                     //TRG
                     fread(trg_data, sizeof(long), 512, f);
                     WDBDATA &this_data =  data[bh.board_serial_number];
                     for(int i=0; i<512; i++){
                        this_data.trigger_data[i] = trg_data[i];
                        //printf("%x\n", trg_data[i]);
                     }
                  }
               } else if(ch.c[0] == 'S') {
                  //Scaler
                  WDBDATA &this_data =  data[bh.board_serial_number];
                  fread(scaler_data, sizeof(long), 19, f);
                  fread(&scaler_time, sizeof(long), 1, f);

                  Double_t factor = 80e6 / (scaler_time - this_data.event_time);
                  
                  for (int i=0 ; i<16 ; i++) {
                     this_data.rate[i] = (scaler_data[i+3]-this_data.scaler[i])*factor;
                     this_data.scaler[i] = scaler_data[i+3];
                  }
                  this_data.event_time = scaler_time;
               }
            }// end for channels

         } else if (memcmp(bh.bn, "T#", 2) == 0) {
            //printf("Trigger board\n");
            TCBDATA &this_data =  data_tcb[bh.board_serial_number];

            unsigned int nBanks;
            fread(&nBanks, sizeof(nBanks), 1, f);
            //printf("with %u banks\n", nBanks);
            for(unsigned int iBank=0; iBank<nBanks; iBank++){
               char bankName[4];
               unsigned int bankSize;
               fread(&bankName, sizeof(char), 4, f);
               fread(&bankSize, sizeof(bankSize), 1, f);
               printf("tcb %d: got bank %c%c%c%c size:%u\n", bh.board_serial_number, bankName[0], bankName[1], bankName[2], bankName[3], bankSize);
               unsigned int temp[8184];
               fread(&temp, sizeof(unsigned int), bankSize, f);
               //for(int i=0; i<bankSize; i++)
               //   printf("%4d: %08x\n", i, temp[i]);

               if(bankName[0]=='T' && bankName[1]=='I' && bankName[2]=='N' && bankName[3]=='P'){
                  int i=1;
                  int nserdes = 0;
                  while(i<bankSize){
                     printf("serdes %d:\n", nserdes);
                     for(int j=0; j<128; j++){
                        uint64_t val = temp[i+128+(j+temp[0])%128];
                        val = val <<32;
                        val |= temp[i+(j+temp[0])%128];
                        this_data.in_waveform[nserdes][j] = val;
                        //printf("%016lx %08x %08x\n", val, temp[i+128+(j+temp[0])%128],  temp[i+(j+temp[0])%128]);
                     }
                     nserdes++;
                     i+=128*2;
                  }
               } else if (bankName[0]=='G' && bankName[1]=='E' && bankName[2]=='N' && bankName[3]=='T'){
                  for(int i=0; i<32; i++){
                     uint64_t val = temp[33+(i+temp[0])%32];
                     val = val <<32;
                     val |= temp[1+(i+temp[0])%32];
                     //printf("%016llx %08x %08x\n", val, temp[33+(i+temp[0])%32],  temp[1+(i+temp[0])%32]);
                     this_data.gent_waveform[i] = val;
                  }
               } else if (bankName[0]=='T' && bankName[1]=='R' && bankName[2]=='G' && bankName[3]=='I'){
                  this_data.trigger_type = temp[0];
                  this_data.total_time = temp[3];
                  this_data.live_time = temp[4];
                  this_data.event_counter = temp[5];
               } else if (bankName[0]=='T' && bankName[1]=='R' && bankName[2]=='G' && bankName[3]=='C'){
                  for(int i=0; i<bankSize; i++){
                  //   printf("%4d: %08x\n", i, temp[i]);
                     this_data.trigger_counter[i] = temp[i];
                  }
               } else if (bankName[0]=='T' && bankName[1]=='X' && bankName[2]=='E' && bankName[3]=='C'){
                  /*printf("TXEC:\n");
                  int tc = temp[0];
                  for(int i=0; i<(bankSize-1)/2; i++){
                     printf("%4d: %08x %08X\n", i, temp[(i+tc)%128+1], temp[(i+tc)%128+129]);
                  }*/
               } else if (bankName[0]=='T' && bankName[1]=='S' && bankName[2]=='C' && bankName[3]=='F'){ // SciFi
                  for(int i=0; i<bankSize; i++)
                     this_data.scifi_scaler[i] = temp[i];
               } else if (bankName[0]=='T' && bankName[1]=='S' && bankName[2]=='F' && bankName[3]=='C'){ // SciFi Coincidence
                  for(int i=0; i<bankSize; i++)
                     this_data.scifi_coincidence[i] = temp[i];
               }
            }
         } else {
            //probably next event, rewind
            fseek(f, -4, SEEK_CUR);
            break;
         }



      } //end loop on boards

      for (auto it: data){
         // align cell #0 of all channels
         WDBDATA &this_data =  it.second;
         t1 = data.begin()->second.time[16][(1024-drsch[16].trigger_cell) % 1024];
         for (chn=1 ; chn<18 ; chn++) {
            t2 = this_data.time[chn][(1024-drsch[chn].trigger_cell) % 1024];
            dt = t1 - t2;
            for (i=0 ; i<1024 ; i++)
               this_data.time[chn][i] += dt;
         }

         for (i=0; i<18; i++)
         {
            if (triggered[i])
            {
               WDBDATA &this_data =  data[bh.board_serial_number];
               header_array[0] = (Double_t) i;
               header_array[1] = (Double_t) bh.board_serial_number;
               header_array[2] = (Double_t) n;
               header_array[3] = 0;

               //printf("Writing %lu events \n", sizeof(data[b].waveform[i])/8);
               fwrite(header_array,sizeof(Double_t),4,f2);
               fwrite(this_data.waveform[i], sizeof(Double_t), sizeof(this_data.waveform[i])/8, f2);
               fwrite(this_data.time[i], sizeof(Double_t), sizeof(this_data.time[i])/8, f2);
            }
         }
      }

      // fill root tree
      rec->Fill();

      //getchar();

   }// end loop on events


   // print number of events
   printf("%d events processed, \"%s\" written.\n", n, rootfile);

   // save and close root file
   rec->Write();
   outfile->Close();
   fclose(f2);
}
