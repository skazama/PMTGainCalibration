#include <iostream>
#include <fstream>
#include <string>
#include <cstdlib>
#include <vector>
#include <math.h>
#include <stdio.h>
#include <sstream>
#include <iomanip>
#include "unistd.h"
#include <TApplication.h>
#include "TH1.h"
#include "TGraphErrors.h"
#include <TFile.h>
#include <TTree.h>
#include <TSpectrum.h>
#include <TMath.h>
#include "TF1.h"
#include <TStyle.h>
#include "TCut.h"
#include <TROOT.h>
#include "TGaxis.h"
#include "TCanvas.h"
#include "TPad.h"
#include "TLegend.h"
#include "THStack.h"
#include "TColor.h"
#include "TLine.h"
#include "TGraphAsymmErrors.h"
#include "TPaveText.h"
#include "TLatex.h"
#include "TChain.h"
#include "TObject.h"
#include "TPaletteAxis.h"
#include "TROOT.h"

using namespace std;

double cal_err(double x, double dx, double y, double dy){

   double err = (1./(y*y))*sqrt(y*y*dx*dx + x*x*dy*dy);

   return err;
}

double convert_to_gain(double mu)
{
    double Z= 50.;
    double A= 10.;
    double e= 1.6021766208e-19;
    double f= 1e8;
    double r= 2.25/16384;
    double gain= mu * r / Z / A /f /e/1e6 ;
    return gain;
}

int main (int argc, char** argv) 
{
   TApplication theApp("app",NULL,NULL);

   char tempname[256];

   /// Timestampe ///
   int LED_mode = atoi(argv[4]); // 0:bottom, 1:top inner (36-126), 2:top outer (0-35)

   TFile *f_bg = new TFile(argv[1]);
   TTree *t = (TTree*)f_bg->Get("tree");
   Long64_t timestamp;
   t->SetBranchAddress("timestamp",&timestamp);
   t->GetEntry(0);

   char buff1[256];
   sprintf(buff1, "%d", timestamp);

   char buff2[256];
   sprintf(buff2, "%d", LED_mode);

   char out_filename[256] = "txt/gain_";
   strcat(out_filename, buff1);
   strcat(out_filename, "_");
   strcat(out_filename, buff2);
   strcat(out_filename, "_");
   strcat(out_filename, argv[5]);
   strcat(out_filename,".txt");

   /// Save gain information in txt file ///
   ofstream fout;
   fout.open(out_filename);

   TFile *f_signal = new TFile(argv[2]);
   TFile *m_file   = new TFile(argv[3],"RECREATE");

   Long64_t m_timestamp;
   Int_t m_pmt_id, m_LED_mode, m_IsOFF;
   Double_t m_gain, m_gain_err, m_lambda, m_lambda_err;

   TTree *tree = new TTree("gain","gain");

   tree->Branch("m_timestamp",&m_timestamp,"m_timestamp/L");
   tree->Branch("m_pmt_id",&m_pmt_id,"m_pmt_id/I");
   tree->Branch("m_gain",&m_gain,"m_gain/D");
   tree->Branch("m_gain_err",&m_gain_err,"m_gain_err/D");
   tree->Branch("m_lambda",&m_lambda,"m_lambda/D");
   tree->Branch("m_lambda_err",&m_lambda_err,"m_lambda_err/D");
   tree->Branch("m_IsOFF",&m_IsOFF,"m_IsOFF/I");
   tree->Branch("m_LED_mode",&m_LED_mode,"m_LED_mode/I");

   int n_PMTs=248;
   int n_end_pos=6;
   int end_pos[n_end_pos];

   TGraphErrors* gr_lambda_vs_end_pos[n_PMTs];

   double estimated_gain[n_PMTs][n_end_pos];
   double estimated_gain_err[n_PMTs][n_end_pos];
   double estimated_lambda[n_PMTs][n_end_pos];
   double estimated_lambda_err[n_PMTs][n_end_pos];

   for(int i=0; i<n_PMTs; i++)
   {
      for(int j=0; j<n_end_pos; j++)
      {
          estimated_gain[i][j]=0.;
          estimated_gain_err[i][j]=0.;
          estimated_lambda[i][j]=0.;
          estimated_lambda_err[i][j]=0.;
      }
   }

   for(int i=0; i<n_end_pos; i++)
   {
      end_pos[i] = 175 + i*2;
   }

   for(int ipmt=0; ipmt<n_PMTs; ++ipmt)
   {
      if(LED_mode == 0)
      {
         if(!(ipmt>=127)){continue;}
      }
      if(LED_mode == 1)
      {
         if(!(ipmt>=36 && ipmt<=126)){continue;}
      }
      if(LED_mode == 2)
      {
         if(!(ipmt<=35)){continue;}
      }

      sprintf(tempname, "gr_lambda_vs_end_pos_for_PMT%d",ipmt);
      gr_lambda_vs_end_pos[ipmt] = new TGraphErrors();
      gr_lambda_vs_end_pos[ipmt]->SetName(tempname);
   }

   /// TGraphErrors ///
   TGraphErrors* gr_pmt_gain_vs_PMTID_top_inner;
   TGraphErrors* gr_pmt_gain_vs_PMTID_top_outer;
   TGraphErrors* gr_pmt_gain_vs_PMTID_bot;
   TGraphErrors* gr_lambda_vs_PMTID_top_inner;
   TGraphErrors* gr_lambda_vs_PMTID_top_outer;
   TGraphErrors* gr_lambda_vs_PMTID_bot;

   /// TGraphErrors for Gain vs PMT ID ///
   gr_pmt_gain_vs_PMTID_top_inner = new TGraphErrors();
   gr_pmt_gain_vs_PMTID_top_inner->SetName("PMT_Gain_vs_PMTID_top_inner");

   gr_pmt_gain_vs_PMTID_top_outer = new TGraphErrors();
   gr_pmt_gain_vs_PMTID_top_outer->SetName("PMT_Gain_vs_PMTID_top_outner");
 
   gr_pmt_gain_vs_PMTID_bot = new TGraphErrors();
   gr_pmt_gain_vs_PMTID_bot->SetName("PMT_Gain_vs_PMTID_bot");

   /// TGraphErrors for Occupancy vs PMT ID///
   gr_lambda_vs_PMTID_top_inner = new TGraphErrors();
   gr_lambda_vs_PMTID_top_inner->SetName("Lambda_vs_PMTID_top_inner");

   gr_lambda_vs_PMTID_top_outer = new TGraphErrors();
   gr_lambda_vs_PMTID_top_outer->SetName("Lambda_vs_PMTID_top_outer");

   gr_lambda_vs_PMTID_bot = new TGraphErrors();
   gr_lambda_vs_PMTID_bot->SetName("Lambda_vs_PMTID_bot");

   for(int ipmt=0; ipmt<n_PMTs; ++ipmt)
   {
      if(LED_mode == 0)
      {
         if(!(ipmt>=127)){continue;}
      }
      if(LED_mode == 1)
      {
         if(!(ipmt>=36 && ipmt<=126)){continue;}
      }
      if(LED_mode == 2)
      {
         if(!(ipmt<=35)){continue;}
      }
	
      for(int itr_end_pos=0; itr_end_pos<n_end_pos; ++itr_end_pos)
      {
         sprintf(tempname,"BG_Area_of_PMT%d_EndPos%d", ipmt, end_pos[itr_end_pos]);
	 TH1D* h_bg = (TH1D*)f_bg->Get(tempname); 

         sprintf(tempname,"Signal_Area_of_PMT%d_EndPos%d", ipmt, end_pos[itr_end_pos]);
	 TH1D* h_signal = (TH1D*)f_signal->Get(tempname); 

         h_bg->GetXaxis()->SetRange(h_bg->GetXaxis()->FindBin(-1000), h_bg->GetXaxis()->FindBin(4000));
         h_signal->GetXaxis()->SetRange(h_signal->GetXaxis()->FindBin(-1000), h_signal->GetXaxis()->FindBin(4000));

	 double EB = h_bg->GetMean(); 
	 double ET = h_signal->GetMean();
 
	 double VB = h_bg->GetRMS(); 
	 double VT = h_signal->GetRMS(); 

	 VB = VB*VB;
	 VT = VT*VT;
	      
	 int n_point=0;
	 int max_point=0;
	 double max_lambda=0;

         TGraphErrors *gr_lambda_vs_threshold = new TGraphErrors();
	
         for(int itr_threshold=-300; itr_threshold<=50; itr_threshold++)
         {
            if(itr_threshold %2 != 0){continue;}

            int threshold_bin = h_bg->GetXaxis()->FindBin(itr_threshold);
            int end_bin       = h_bg->GetXaxis()->FindBin(-10000.);

            double Ab = h_bg->Integral(end_bin, threshold_bin);
            double As = h_signal->Integral(end_bin, threshold_bin);

	    int Nevt = h_bg->GetEntries();
            double lambda = -log(As/Ab);
            double lambda_stat_err = sqrt((exp(lambda) + 1. -2.*(Ab/Nevt))/Ab);
	
            gr_lambda_vs_threshold->SetPoint(n_point, itr_threshold, lambda);
            gr_lambda_vs_threshold->SetPointError(n_point, 0., lambda_stat_err);

	    if(lambda > max_lambda && lambda_stat_err/lambda < 0.01)
            {
	       max_lambda = lambda;
	       max_point = n_point;
            }

	    ++n_point;
	 }
 
         int start_point = max_point - 50;
         if(start_point < 2){start_point = 2;}

         for(int i=start_point; i<n_point-10; i++)
         {

            double lambda_0     = gr_lambda_vs_threshold->GetY()[i];
            double lambda_err_0 = gr_lambda_vs_threshold->GetEY()[i];

            double lambda_m2  = gr_lambda_vs_threshold->GetY()[i-2]; 
            double lambda_m1  = gr_lambda_vs_threshold->GetY()[i-1];
            double lambda_p1  = gr_lambda_vs_threshold->GetY()[i+1]; 
            double lambda_p2  = gr_lambda_vs_threshold->GetY()[i+2]; 
            double lambda_p10 = gr_lambda_vs_threshold->GetY()[i+10];
            double lambda_p20 = gr_lambda_vs_threshold->GetY()[i+20];

            double diff_m2 = fabs(lambda_0-lambda_m2)/lambda_0;
            double diff_m1 = fabs(lambda_0-lambda_m1)/lambda_0;
            double diff_p1 = fabs(lambda_0-lambda_p1)/lambda_0;
            double diff_p2 = fabs(lambda_0-lambda_p2)/lambda_0;
	
            int threshold_bin = h_bg->GetXaxis()->FindBin(gr_lambda_vs_threshold->GetX()[i]);
            int end_bin       = h_bg->GetXaxis()->FindBin(-10000.);
            double Ab = h_bg->Integral(end_bin, threshold_bin);
            double As = h_signal->Integral(end_bin, threshold_bin);
            double N = h_bg->GetEntries();
            double f = Ab/N;

	    if(lambda_0>0 && diff_m2<0.020 && diff_m1<0.020 && diff_p1<0.020 && diff_p2<0.020 && lambda_0>lambda_p10 && lambda_0>lambda_p20 && lambda_p10>lambda_p20 && lambda_err_0/lambda_0 < 0.01 && f >= 0.10)
            {
	       estimated_lambda[ipmt][itr_end_pos]     = lambda_0; 
	       estimated_lambda_err[ipmt][itr_end_pos] = lambda_err_0; 

	       double EPsi = (ET - EB)/lambda_0;
	       double VPsi = (VT - VB)/lambda_0 - EPsi*EPsi;

	       double EPsi_err = (lambda_0*(EPsi*EPsi + VPsi) + 2.*VB)/(N*lambda_0*lambda_0) + (EPsi*EPsi*(exp(lambda_0) + 1. - 2.*f))/(f*N*lambda_0*lambda_0);

               double gain          = convert_to_gain(EPsi);
               double gain_stat_err = convert_to_gain(sqrt(EPsi_err));

	       estimated_gain[ipmt][itr_end_pos]     = gain; 
	       estimated_gain_err[ipmt][itr_end_pos] = gain_stat_err;

	       gr_lambda_vs_end_pos[ipmt]->SetPoint(itr_end_pos, end_pos[itr_end_pos], lambda_0);	
	       gr_lambda_vs_end_pos[ipmt]->SetPointError(itr_end_pos, 0, lambda_err_0);	

               cout<<"Occupancty for "<<ipmt<<" (end_pos="<<end_pos[itr_end_pos]<<"): "<<lambda_0<<" +/- "<<lambda_err_0<<endl;
		 
	       break;
            }

         }
      }
      cout<<"##########################################################################################################"<<endl;
   }

   /// Calculating New Gains and filling them as Branch in TTree ///
   for(int i=0; i<n_PMTs; i++)
   {
      if(LED_mode == 0)
      {
         if(!(i>=127)){continue;}
      }
      if(LED_mode == 1)
      {
         if(!(i>=36 && i<=126)){continue;}
      }
      if(LED_mode == 2)
      {
         if(!(i<=35)){continue;}
      }

      TH1D* h_gain     = new TH1D("","",10000,0,10);
      TH1D* h_gain_err = new TH1D("","",10000,0,10);

      TH1D* h_lambda     = new TH1D("","",10000,0,10);
      TH1D* h_lambda_err = new TH1D("","",10000,0,10);

      double Gain       = 0.;
      double Gain_err   = 0.;
      double Lambda     = 0.;
      double Lambda_err = 0.;


      for(int itr_end_pos=0; itr_end_pos<n_end_pos; ++itr_end_pos)
      {
	 double gain          = estimated_gain[i][itr_end_pos];
	 double gain_stat_err = estimated_gain_err[i][itr_end_pos];

	 h_gain->Fill(gain);
	 h_gain_err->Fill(gain_stat_err);
	 h_lambda->Fill(estimated_lambda[i][itr_end_pos]);
	 h_lambda_err->Fill(estimated_lambda_err[i][itr_end_pos]);
      }

      Gain       = h_gain->GetMean();
      Gain_err   = h_gain->GetRMS() + h_gain_err->GetMean();
      Lambda     = h_lambda->GetMean();
      Lambda_err = h_lambda->GetRMS() + h_lambda_err->GetMean();
     
      cout<<"Gain for PMT"<<i<<" = "<<Gain<<" +/- "<<h_gain->GetRMS()<<" (sys.) +/- "<<h_gain_err->GetMean()<<" (stat.)"<<endl;
  
      if(Gain >= 0.1)
      {
         if(LED_mode==0)
         {
            gr_pmt_gain_vs_PMTID_bot->SetPoint(i-127, i, Gain);
            gr_pmt_gain_vs_PMTID_bot->SetPointError(i-127, 0., Gain_err);
            gr_lambda_vs_PMTID_bot->SetPoint(i-127, i, Lambda);
            gr_lambda_vs_PMTID_bot->SetPointError(i-127, 0., Lambda_err);
         }
         if(LED_mode==1)
         {
            gr_pmt_gain_vs_PMTID_top_inner->SetPoint(i-36, i, Gain);
            gr_pmt_gain_vs_PMTID_top_inner->SetPointError(i-36, 0., Gain_err);
            gr_lambda_vs_PMTID_top_inner->SetPoint(i-36, i, Lambda);
            gr_lambda_vs_PMTID_top_inner->SetPointError(i-36, 0., Lambda_err);
         }
         if(LED_mode==2)
         {
            gr_pmt_gain_vs_PMTID_top_outer->SetPoint(i, i, Gain);
            gr_pmt_gain_vs_PMTID_top_outer->SetPointError(i, 0., Gain_err);
            gr_lambda_vs_PMTID_top_outer->SetPoint(i, i, Lambda);
            gr_lambda_vs_PMTID_top_outer->SetPointError(i, 0., Lambda_err);
         }
      }
      if(Gain < 0.1)
      {
         cout<<"PMT"<<i<<" might be OFF"<<endl;

         if(LED_mode==0)
         {
            gr_pmt_gain_vs_PMTID_bot->SetPoint(i-127, i, 0.);
            gr_pmt_gain_vs_PMTID_bot->SetPointError(i-127, 0., 0.);
            gr_lambda_vs_PMTID_bot->SetPoint(i-127, i,  0.);
            gr_lambda_vs_PMTID_bot->SetPointError(i-127, 0., 0.);
         }
         if(LED_mode==1)
         {
            gr_pmt_gain_vs_PMTID_top_inner->SetPoint(i-36, i, 0.);
            gr_pmt_gain_vs_PMTID_top_inner->SetPointError(i-36, 0., 0.);
            gr_lambda_vs_PMTID_top_inner->SetPoint(i-36, i,  0.);
            gr_lambda_vs_PMTID_top_inner->SetPointError(i-36, 0., 0.);
         }
         if(LED_mode==2)
         {
            gr_pmt_gain_vs_PMTID_top_outer->SetPoint(i, i,  0.);
            gr_pmt_gain_vs_PMTID_top_outer->SetPointError(i, 0., 0.);
            gr_lambda_vs_PMTID_top_outer->SetPoint(i, i,  0.);
            gr_lambda_vs_PMTID_top_outer->SetPointError(i, 0., 0.);
         }
      }

      m_timestamp  = timestamp;
      m_pmt_id     = i;
      m_gain       = Gain;
      m_gain_err   = Gain_err;
      m_lambda     = Lambda; 
      m_lambda_err = Lambda_err; 
      m_LED_mode   = LED_mode;

      if(m_gain >  0.1)
      {
          m_IsOFF = 0;
          fout<<m_pmt_id<<" "<<m_gain<<" "<<m_gain_err<<" "<<m_lambda<<" "<<m_lambda_err<<endl;
      }
      if(m_gain <= 0.1)
      {
          m_IsOFF = 1;
          fout<<m_pmt_id<<" "<<0<<" "<<0<<" "<<0<<" "<<0<<endl;
      }	
 
      tree->Fill();
   }
   fout.close();

   if(LED_mode==0)
   {
      gr_pmt_gain_vs_PMTID_bot->Write();
      gr_lambda_vs_PMTID_bot->Write();
   }
   if(LED_mode==1)
   {
      gr_pmt_gain_vs_PMTID_top_inner->Write();
      gr_lambda_vs_PMTID_top_inner->Write();
   }
   if(LED_mode==2)
   {
      gr_pmt_gain_vs_PMTID_top_outer->Write();
      gr_lambda_vs_PMTID_top_outer->Write();
   }

   m_file->Write();
   m_file->Close();

// theApp.Run();

    return 0;
}
