#include <iostream>
#include <fstream>
#include <string>
#include <cstdlib>
#include <vector>
#include <math.h>
#include <stdio.h>
#include <stdio.h>
#include <sstream>
#include <iomanip>
#include "unistd.h"
#include <TApplication.h>
#include "TH1.h"
#include "TGraphErrors.h"
#include <TFile.h>
#include <TStyle.h>
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
#include "TVector.h"

using namespace std;

double cal_err(double x, double dx, double y, double dy){

   double err = (1./(y*y))*sqrt(y*y*dx*dx + x*x*dy*dy);

   return err;
}

void GetComparison(TGraphErrors* gr_pre, TGraphErrors* gr_now, TGraphErrors* gr_diff, bool IsGain, bool IsTop, char pre_time[256], char current_time[256]){

   char tempname[256];
   sprintf(tempname, "c_%d_%d", IsGain, IsTop); 

   TCanvas *c = new TCanvas(tempname, tempname, 0,70,2000,500);
   c->Divide(2,1);
   c->SetFillColor(0);

   TPad *pad1 = new TPad("pad1","",0,0,1,1);
   TPad *pad2 = new TPad("pad2","",0,0,1,1);

   pad1->SetLeftMargin(0.07);
   pad1->SetRightMargin(0.05);
   pad1->SetTopMargin(0.05);
   pad1->SetBottomMargin(0.31);
   pad1->SetGridy();

   pad2->SetLeftMargin(0.07);
   pad2->SetRightMargin(0.05);
   pad2->SetTopMargin(0.71);
   pad2->SetBottomMargin(0.10);

   pad1->SetFillStyle(4000);
   pad2->SetFillStyle(4000);
   pad1->Draw();
   pad1->cd();

   gr_pre->SetMarkerStyle(20);
   gr_pre->SetMarkerColor(1);
   gr_pre->SetLineColor(1);
   
   gr_now->SetMarkerStyle(24);
   gr_now->SetMarkerColor(2);
   gr_now->SetLineColor(2);
   
   gr_pre->GetXaxis()->SetLabelSize(0.);
   gr_pre->GetYaxis()->SetTitleOffset(0.50);

   if(IsTop==true) 
   {
      gr_pre->GetXaxis()->SetRangeUser(0,126);
      gr_now->GetXaxis()->SetRangeUser(0,126);
      gr_diff->GetXaxis()->SetRangeUser(0,126);
   }
   if(IsTop==false)
   {
      gr_pre->GetXaxis()->SetRangeUser(127,247);
      gr_now->GetXaxis()->SetRangeUser(127,247);
      gr_diff->GetXaxis()->SetRangeUser(127,247);
   }

   if(IsGain==true)
   {
      gr_pre->GetYaxis()->SetTitle("Gain (#times 10^{6})");
      gr_pre->SetMinimum(0.5);
      gr_pre->SetMaximum(6.5);
      gr_diff->GetYaxis()->SetTitle("#Delta Gain (%)");
   }
   if(IsGain==false)
   {
      gr_pre->GetYaxis()->SetTitle("#lambda (PE/trigger)");
      gr_pre->SetMinimum(1.5);
      gr_pre->SetMaximum(4.5);
      gr_diff->GetYaxis()->SetTitle("#Delta #lambda (%)");
   }

   gr_pre->Draw("AP");
   gr_now->Draw("P");

   TLegend *leg = new TLegend(0.88,0.83,0.95,0.95);
   leg->AddEntry(gr_pre, pre_time,"lp");
   leg->AddEntry(gr_now, current_time,"lp");
   leg->SetTextSize(0.05);
   leg->SetTextFont(42);
   leg->SetFillColor(0);
   leg->SetLineColor(1);
   leg->Draw();

   pad2->Draw();
   pad2->cd();
   pad2->SetGridy();

   gr_diff->SetMarkerStyle(20);
   gr_diff->SetMarkerColor(1);
   gr_diff->SetLineColor(1);

   gr_diff->Draw("AP");
   gr_diff->SetMinimum(-20);
   gr_diff->SetMaximum(20);
   gr_diff->GetYaxis()->SetTitleSize(0.04);
   gr_diff->GetYaxis()->SetTitleOffset(0.55);
   gr_diff->GetXaxis()->SetTitleOffset(0.65);
   gr_diff->GetXaxis()->SetTitle("PMT ID");
   gr_diff->GetYaxis()->CenterTitle(true);
   gr_diff->GetYaxis()->SetLabelSize(0.03);

   char *temp_plot = ".png";
   sprintf(tempname, "plots/gain_comparison_top_"); 
   strcat (tempname, current_time);
   strcat (tempname, temp_plot);
   if(IsTop==true  && IsGain==true)c->SaveAs(tempname);

   sprintf(tempname, "plots/gain_comparison_bot_"); 
   strcat (tempname, current_time);
   strcat (tempname, temp_plot);
   if(IsTop==false && IsGain==true)c->SaveAs(tempname);

   sprintf(tempname, "plots/lambda_comparison_top_"); 
   strcat (tempname, current_time);
   strcat (tempname, temp_plot);
   if(IsTop==true  && IsGain==false)c->SaveAs(tempname);

   sprintf(tempname, "plots/lambda_comparison_bot_"); 
   strcat (tempname, current_time);
   strcat (tempname, temp_plot);
   if(IsTop==false && IsGain==false)c->SaveAs(tempname);
 
}

int main (int argc, char** argv) 
{
   TApplication theApp("app",NULL,NULL);

   gStyle->SetOptTitle(1); 
   gStyle->SetOptStat(0); 
   gStyle->SetTitleFont(42);

   TStyle *Style = new TStyle("XENON Style","XENON Style");
   Int_t font=42; // Helvetica
   Style->SetTextFont(font);

   TFile *m_file;
   ifstream fin(argv[1]);
   TString fname;

   TChain *chain = new TChain("gain","gain");

   while(fin >> fname)
   {
      cout<<"chain->Add("<<fname<<")"<<endl;
      chain->Add(fname);
   }

   /// Time evolution of Gain and Lambda ///
   TFile *m_file_out = new TFile(argv[4],"recreate");
   char tempname[256];

   TGraphErrors* gr_pmt_gain_vs_time[248];
   TGraphErrors* gr_lambda_vs_time[248];
 
   for(int i=0; i<=247; i++)
   {
      /// Gain ///
      sprintf(tempname,"m_pmt_id == %d && m_gain > 0.10", i);
      TCut cut = tempname;

      Long64_t N = chain->Draw("m_timestamp:m_gain:0:m_gain_err",cut,"goff");
      gr_pmt_gain_vs_time[i] = new TGraphErrors(N,chain->GetV1(),chain->GetV2(),chain->GetV3(),chain->GetV4());

      sprintf(tempname, "Gain_vs_time_for_PMT%d",i);
      gr_pmt_gain_vs_time[i]->SetName(tempname);
      gr_pmt_gain_vs_time[i]->SetTitle(tempname);

      /// Lambda ///
      sprintf(tempname,"m_pmt_id == %d && m_lambda > 0.10", i);
      cut = tempname;

      N = chain->Draw("m_timestamp:m_lambda:0:m_lambda_err",cut,"goff");
      gr_lambda_vs_time[i] = new TGraphErrors(N,chain->GetV1(),chain->GetV2(),chain->GetV3(),chain->GetV4());

      sprintf(tempname, "Lambda_vs_time_for_PMT%d",i);
      gr_lambda_vs_time[i]->SetName(tempname);
      gr_lambda_vs_time[i]->SetTitle(tempname);
   }

   for(int i=0; i<=247; i++)
   {
      sprintf(tempname, "Gain_vs_Time_for_PMT%d",i);
      TCanvas *c = new TCanvas(tempname,tempname);
      c->SetFillColor(0);
      c->SetGridy();

      gr_pmt_gain_vs_time[i]->SetMarkerStyle(20);
      gr_pmt_gain_vs_time[i]->GetXaxis()->SetTimeDisplay(1);
      gr_pmt_gain_vs_time[i]->GetXaxis()->SetNdivisions(-10,1);
      gr_pmt_gain_vs_time[i]->GetXaxis()->SetTimeFormat("%d/%m/%y");
      gr_pmt_gain_vs_time[i]->GetXaxis()->SetTimeOffset(0,"gmt");
      gr_pmt_gain_vs_time[i]->GetXaxis()->SetTitle("date (day/month/year)");
      gr_pmt_gain_vs_time[i]->GetYaxis()->SetTitle("Gain (#times 10^{6})");
      gr_pmt_gain_vs_time[i]->GetXaxis()->SetLabelSize(0.03);
      gr_pmt_gain_vs_time[i]->GetYaxis()->SetLabelSize(0.03);

      double mean = gr_pmt_gain_vs_time[i]->GetMean(2);
      double rms  = gr_pmt_gain_vs_time[i]->GetRMS(2);

      gr_pmt_gain_vs_time[i]->SetMinimum(0.5*mean);
      gr_pmt_gain_vs_time[i]->SetMaximum(1.5*mean);

      gr_pmt_gain_vs_time[i]->Draw("APL");
      gr_pmt_gain_vs_time[i]->Write();

      c->Update();

      if(mean > 0.1)
      {
         TLine *line[3];

         line[0] = new TLine(c->GetUxmin(), mean ,      c->GetUxmax(), mean);
         line[1] = new TLine(c->GetUxmin(), 1.05*mean , c->GetUxmax(), 1.05*mean);
         line[2] = new TLine(c->GetUxmin(), 0.95*mean , c->GetUxmax(), 0.95*mean);

         for(int j=0; j<3; j++)
         {
            line[j]->SetLineColor(2);
            if(j==0)line[j]->SetLineStyle(1);
            if(j!=0)line[j]->SetLineStyle(2);
            line[j]->Draw();
         }

         double fit_start = gr_pmt_gain_vs_time[i]->GetX()[0];
         double fit_end   = gr_pmt_gain_vs_time[i]->GetX()[gr_pmt_gain_vs_time[i]->GetN()-1];

         TF1 *pol0 = new TF1("pol0","pol0", fit_start, fit_end);
         gr_pmt_gain_vs_time[i]->Fit(pol0,"QNEMR+");

         double fit_mean     = pol0->GetParameter(0);
         double fit_mean_err = pol0->GetParError(0);

         TLatex latex[3];

         sprintf(tempname, "Mean = %.2f #times 10^{6}",mean);
         latex[0].SetTextFont(42);
         latex[0].SetTextSize(0.03);
         latex[0].DrawLatex(gr_pmt_gain_vs_time[i]->GetX()[0],1.4*mean,tempname);

         sprintf(tempname, "RMS/mean = %.2f %%",100.*rms/mean);
         latex[1].SetTextFont(42);
         latex[1].SetTextSize(0.03);
         latex[1].DrawLatex(gr_pmt_gain_vs_time[i]->GetX()[0],1.3*mean,tempname);

         sprintf(tempname, "stat #plus sys uncertainty = %.2f %%",100.*fit_mean_err/fit_mean);
         latex[2].SetTextFont(42);
         latex[2].SetTextSize(0.03);
         latex[2].DrawLatex(gr_pmt_gain_vs_time[i]->GetX()[0],1.2*mean,tempname);

	 for(int j=0; j<gr_pmt_gain_vs_time[i]->GetN(); j++)
         {
	     if(gr_pmt_gain_vs_time[i]->GetY()[j] < 0.95*mean || gr_pmt_gain_vs_time[i]->GetY()[j] > 1.05*mean )
	     {
		cout<<"PMT "<<i<<" has bad gain "<<endl;
	     }
         }
      }

      sprintf(tempname, "plots/Gain_vs_Time_for_PMT%d.png",i);
      c->SaveAs(tempname);
      c->Clear();
   } 

   for(int i=0; i<=247; i++)
   {
      sprintf(tempname, "Lambda_vs_Time_for_PMT%d",i);
      TCanvas *c = new TCanvas(tempname,tempname);
      c->SetFillColor(0);
      c->SetGridy();

      gr_lambda_vs_time[i]->SetMarkerStyle(20);
      gr_lambda_vs_time[i]->GetXaxis()->SetTimeDisplay(1);
      gr_lambda_vs_time[i]->GetXaxis()->SetNdivisions(-10,1);
      gr_lambda_vs_time[i]->GetXaxis()->SetTimeFormat("%d/%m/%y");
      gr_lambda_vs_time[i]->GetXaxis()->SetTimeOffset(0,"gmt");
      gr_lambda_vs_time[i]->GetXaxis()->SetTitle("date (day/month/year)");
      gr_lambda_vs_time[i]->GetYaxis()->SetTitle("#lambda (PE/trigger)");
      gr_lambda_vs_time[i]->GetXaxis()->SetLabelSize(0.03);
      gr_lambda_vs_time[i]->GetYaxis()->SetLabelSize(0.03);

      double mean = gr_lambda_vs_time[i]->GetMean(2);
      double rms  = gr_lambda_vs_time[i]->GetRMS(2);

      gr_lambda_vs_time[i]->SetMinimum(0.5*mean);
      gr_lambda_vs_time[i]->SetMaximum(1.5*mean);

      gr_lambda_vs_time[i]->Draw("APL");
      gr_lambda_vs_time[i]->Write();

      c->Update();

      if(mean > 0.1)
      {
         TLine *line[3];
         
         line[0] = new TLine(c->GetUxmin(), mean ,      c->GetUxmax(), mean);
         line[1] = new TLine(c->GetUxmin(), 1.05*mean , c->GetUxmax(), 1.05*mean);
         line[2] = new TLine(c->GetUxmin(), 0.95*mean , c->GetUxmax(), 0.95*mean);

         for(int j=0; j<3; j++)
         {
           line[j]->SetLineColor(2);
            if(j==0)line[j]->SetLineStyle(1);
            if(j!=0)line[j]->SetLineStyle(2);
            line[j]->Draw();
         }

         double fit_start = gr_lambda_vs_time[i]->GetX()[0];
         double fit_end   = gr_lambda_vs_time[i]->GetX()[gr_lambda_vs_time[i]->GetN()-1];

         TF1 *pol0 = new TF1("pol0","pol0", fit_start, fit_end);
         gr_lambda_vs_time[i]->Fit(pol0,"QNEMR+");

         double fit_mean     = pol0->GetParameter(0);
         double fit_mean_err = pol0->GetParError(0);

         TLatex latex[3];

         sprintf(tempname, "Mean = %.2f (PE/trigger)",mean);
         latex[0].SetTextFont(42);
         latex[0].SetTextSize(0.03);
         latex[0].DrawLatex(gr_lambda_vs_time[i]->GetX()[0],1.4*mean,tempname);

         sprintf(tempname, "RMS/mean = %.2f %%",100.*rms/mean);
         latex[1].SetTextFont(42);
         latex[1].SetTextSize(0.03);
         latex[1].DrawLatex(gr_lambda_vs_time[i]->GetX()[0],1.3*mean,tempname);

         sprintf(tempname, "stat #plus sys uncertainty = %.2f %%",100.*fit_mean_err/fit_mean);
         latex[2].SetTextFont(42);
         latex[2].SetTextSize(0.03);
         latex[2].DrawLatex(gr_lambda_vs_time[i]->GetX()[0],1.2*mean,tempname);
      
         for(int j=0; j<gr_lambda_vs_time[i]->GetN(); j++)
         {
             if(gr_lambda_vs_time[i]->GetY()[j] < 0.95*mean || gr_lambda_vs_time[i]->GetY()[j] > 1.05*mean )
             {
                cout<<"PMT "<<i<<" has bad lambda"<<endl;
             }
         }
      }


      sprintf(tempname, "plots/Lambda_vs_Time_for_PMT%d.png",i);
      c->SaveAs(tempname);
      c->Clear();
   } 

   TFile *m_file_pre;
   TFile *m_file_current;

   m_file_pre     = new TFile(argv[2]);
   m_file_current = new TFile(argv[3]);

   TTree* tree_pre     = (TTree*)m_file_pre->Get("gain");
   TTree* tree_current = (TTree*)m_file_current->Get("gain");

   /// Comparison between current data and previous data ///
   /// Gain ///
   TGraphErrors* gr_pmt_gain_vs_PMTID_top;
   TGraphErrors* gr_pmt_gain_vs_PMTID_bot;
   TGraphErrors* gr_pmt_gain_vs_PMTID_top_pre;
   TGraphErrors* gr_pmt_gain_vs_PMTID_bot_pre;

   gr_pmt_gain_vs_PMTID_top     = new TGraphErrors();
   gr_pmt_gain_vs_PMTID_bot     = new TGraphErrors();
   gr_pmt_gain_vs_PMTID_top_pre = new TGraphErrors();
   gr_pmt_gain_vs_PMTID_bot_pre = new TGraphErrors();

   gr_pmt_gain_vs_PMTID_top->SetName("PMT_Gain_vs_PMTID_top");
   gr_pmt_gain_vs_PMTID_bot->SetName("PMT_Gain_vs_PMTID_bot");
   gr_pmt_gain_vs_PMTID_top_pre->SetName("PMT_Gain_vs_PMTID_top_pre");
   gr_pmt_gain_vs_PMTID_bot_pre->SetName("PMT_Gain_vs_PMTID_bot_pre");

   /// Lambda ///
   TGraphErrors* gr_lambda_vs_PMTID_top;
   TGraphErrors* gr_lambda_vs_PMTID_bot;
   TGraphErrors* gr_lambda_vs_PMTID_top_pre;
   TGraphErrors* gr_lambda_vs_PMTID_bot_pre;

   gr_lambda_vs_PMTID_top     = new TGraphErrors();
   gr_lambda_vs_PMTID_bot     = new TGraphErrors();
   gr_lambda_vs_PMTID_top_pre = new TGraphErrors();
   gr_lambda_vs_PMTID_bot_pre = new TGraphErrors();

   gr_lambda_vs_PMTID_top->SetName("Lambda_vs_PMTID_top");
   gr_lambda_vs_PMTID_bot->SetName("Lambda_vs_PMTID_bot");
   gr_lambda_vs_PMTID_top_pre->SetName("Lambda_vs_PMTID_top_pre");
   gr_lambda_vs_PMTID_bot_pre->SetName("Lambda_vs_PMTID_bot_pre");

   /// Difference (previous, current) ///
   TGraphErrors* gr_pmt_gain_vs_PMTID_top_diff;
   TGraphErrors* gr_pmt_gain_vs_PMTID_bot_diff;
   TGraphErrors* gr_lambda_vs_PMTID_top_diff;
   TGraphErrors* gr_lambda_vs_PMTID_bot_diff;

   gr_pmt_gain_vs_PMTID_top_diff = new TGraphErrors();
   gr_pmt_gain_vs_PMTID_bot_diff = new TGraphErrors();
   gr_pmt_gain_vs_PMTID_top_diff->SetName("PMT_Gain_vs_PMTID_top_diff");
   gr_pmt_gain_vs_PMTID_bot_diff->SetName("PMT_Gain_vs_PMTID_bot_diff");

   gr_lambda_vs_PMTID_top_diff = new TGraphErrors();
   gr_lambda_vs_PMTID_bot_diff = new TGraphErrors();
   gr_lambda_vs_PMTID_top_diff->SetName("Lambda_vs_PMTID_top_diff");
   gr_lambda_vs_PMTID_bot_diff->SetName("Lambda_vs_PMTID_bot_diff");

   Long64_t N;
   /// Gain ///
   N = tree_pre->Draw("m_pmt_id:m_gain:0:m_gain_err","m_pmt_id<=126","goff");
   gr_pmt_gain_vs_PMTID_top_pre = new TGraphErrors(N,tree_pre->GetV1(),tree_pre->GetV2(),tree_pre->GetV3(),tree_pre->GetV4());

   N = tree_pre->Draw("m_pmt_id:m_gain:0:m_gain_err","m_pmt_id>126","goff");
   gr_pmt_gain_vs_PMTID_bot_pre = new TGraphErrors(N,tree_pre->GetV1(),tree_pre->GetV2(),tree_pre->GetV3(),tree_pre->GetV4());

   N = tree_current->Draw("m_pmt_id:m_gain:0:m_gain_err","m_pmt_id<=126","goff");
   gr_pmt_gain_vs_PMTID_top = new TGraphErrors(N,tree_current->GetV1(),tree_current->GetV2(),tree_current->GetV3(),tree_current->GetV4());

   N = tree_current->Draw("m_pmt_id:m_gain:0:m_gain_err","m_pmt_id>126","goff");
   gr_pmt_gain_vs_PMTID_bot = new TGraphErrors(N,tree_current->GetV1(),tree_current->GetV2(),tree_current->GetV3(),tree_current->GetV4());

   /// Lambda ///
   N = tree_pre->Draw("m_pmt_id:m_lambda:0:m_lambda_err","m_pmt_id<=126","goff");
   gr_lambda_vs_PMTID_top_pre = new TGraphErrors(N,tree_pre->GetV1(),tree_pre->GetV2(),tree_pre->GetV3(),tree_pre->GetV4());

   N = tree_pre->Draw("m_pmt_id:m_lambda:0:m_lambda_err","m_pmt_id>126","goff");
   gr_lambda_vs_PMTID_bot_pre = new TGraphErrors(N,tree_pre->GetV1(),tree_pre->GetV2(),tree_pre->GetV3(),tree_pre->GetV4());

   N = tree_current->Draw("m_pmt_id:m_lambda:0:m_lambda_err","m_pmt_id<=126","goff");
   gr_lambda_vs_PMTID_top = new TGraphErrors(N,tree_current->GetV1(),tree_current->GetV2(),tree_current->GetV3(),tree_current->GetV4());

   N = tree_current->Draw("m_pmt_id:m_lambda:0:m_lambda_err","m_pmt_id>126","goff");
   gr_lambda_vs_PMTID_bot = new TGraphErrors(N,tree_current->GetV1(),tree_current->GetV2(),tree_current->GetV3(),tree_current->GetV4());

   /// difference ///
   /// Top PMTs ///

   TH1D* h_diff_gain   = new TH1D("","", 1000,0,10);
   TH1D* h_diff_lambda = new TH1D("","", 1000,0,10);

   for(int i=0; i<=126; i++)
   {
      double current_gain       = gr_pmt_gain_vs_PMTID_top->GetY()[i];
      double current_gain_err   = gr_pmt_gain_vs_PMTID_top->GetEY()[i];
      double current_lambda     = gr_lambda_vs_PMTID_top->GetY()[i];
      double current_lambda_err = gr_lambda_vs_PMTID_top->GetEY()[i];

      double pre_gain           = gr_pmt_gain_vs_PMTID_top_pre->GetY()[i];
      double pre_gain_err       = gr_pmt_gain_vs_PMTID_top_pre->GetEY()[i];
      double pre_lambda         = gr_lambda_vs_PMTID_top_pre->GetY()[i];
      double pre_lambda_err     = gr_lambda_vs_PMTID_top_pre->GetEY()[i];

      double diff_gain     = 100.*(pre_gain - current_gain)/current_gain;
      double diff_gain_err = 100.*cal_err(pre_gain, pre_gain_err, current_gain, current_gain_err);

      double diff_lambda     = 100.*(pre_lambda - current_lambda)/current_lambda;
      double diff_lambda_err = 100.*cal_err(pre_lambda, pre_lambda_err, current_lambda, current_lambda_err);

      cout<<"PMT "<<i<<", pre = "<<pre_gain<<", now = "<<current_gain<<", diff = "<<diff_gain<<" %"<<endl;
  
      if(current_gain>0.1 && pre_gain>0.1)
      {
         h_diff_gain->Fill(fabs(diff_gain));
         h_diff_lambda->Fill(fabs(diff_lambda));
      }

      gr_pmt_gain_vs_PMTID_top_diff->SetPoint(i, i, diff_gain);
      gr_pmt_gain_vs_PMTID_top_diff->SetPointError(i, 0., diff_gain_err);
      gr_lambda_vs_PMTID_top_diff->SetPoint(i, i, diff_lambda);
      gr_lambda_vs_PMTID_top_diff->SetPointError(i, 0., diff_lambda_err);
   }

   /// Bottom PMTs ///
   for(int i=127; i<=247; i++)
   {
      double current_gain       = gr_pmt_gain_vs_PMTID_bot->GetY()[i-127];
      double current_gain_err   = gr_pmt_gain_vs_PMTID_bot->GetEY()[i-127];
      double current_lambda     = gr_lambda_vs_PMTID_bot->GetY()[i-127];
      double current_lambda_err = gr_lambda_vs_PMTID_bot->GetEY()[i-127];

      double pre_gain           = gr_pmt_gain_vs_PMTID_bot_pre->GetY()[i-127];
      double pre_gain_err       = gr_pmt_gain_vs_PMTID_bot_pre->GetEY()[i-127];
      double pre_lambda         = gr_lambda_vs_PMTID_bot_pre->GetY()[i-127];
      double pre_lambda_err     = gr_lambda_vs_PMTID_bot_pre->GetEY()[i-127];

      double diff_gain     = 100.*(pre_gain - current_gain)/current_gain;
      double diff_gain_err = 100.*cal_err(pre_gain, pre_gain_err, current_gain, current_gain_err);

      double diff_lambda     = 100.*(pre_lambda - current_lambda)/current_lambda;
      double diff_lambda_err = 100.*cal_err(pre_lambda, pre_lambda_err, current_lambda, current_lambda_err);

      cout<<"PMT "<<i<<", pre = "<<pre_gain<<", now = "<<current_gain<<", diff = "<<diff_gain<<" %"<<endl;

      if(current_gain>0.1 && pre_gain>0.1)
      {
         h_diff_gain->Fill(fabs(diff_gain));
         h_diff_lambda->Fill(fabs(diff_lambda));
      }

      gr_pmt_gain_vs_PMTID_bot_diff->SetPoint(i-127, i, diff_gain);
      gr_pmt_gain_vs_PMTID_bot_diff->SetPointError(i-127, 0., diff_gain_err);
      gr_lambda_vs_PMTID_bot_diff->SetPoint(i-127, i, diff_lambda);
      gr_lambda_vs_PMTID_bot_diff->SetPointError(i-127, 0., diff_lambda_err);
   }

   cout<<"Gain   difference = "<<h_diff_gain->GetMean()<<" +/- "<<h_diff_gain->GetRMS()<<endl;
   cout<<"Lambda difference = "<<h_diff_lambda->GetMean()<<" +/- "<<h_diff_lambda->GetRMS()<<endl;


   char *pre_time     = argv[5];
   char *current_time = argv[6];

   /// Gain ///
   GetComparison(gr_pmt_gain_vs_PMTID_top_pre, gr_pmt_gain_vs_PMTID_top, gr_pmt_gain_vs_PMTID_top_diff, true, true, pre_time, current_time);
   GetComparison(gr_pmt_gain_vs_PMTID_bot_pre, gr_pmt_gain_vs_PMTID_bot, gr_pmt_gain_vs_PMTID_bot_diff, true, false, pre_time, current_time);
   
   /// Lambda ///
   GetComparison(gr_lambda_vs_PMTID_top_pre, gr_lambda_vs_PMTID_top, gr_lambda_vs_PMTID_top_diff, false, true, pre_time, current_time);
   GetComparison(gr_lambda_vs_PMTID_bot_pre, gr_lambda_vs_PMTID_bot, gr_lambda_vs_PMTID_bot_diff, false, false, pre_time, current_time);

//   theApp.Run();

   m_file_out->Write();
   m_file_out->Close();


   return 0;
}
