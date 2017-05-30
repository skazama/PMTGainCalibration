//////////////////////////////////////////////////////////
// This class has been automatically generated on
// Sat Apr 22 18:51:05 2017 by ROOT version 5.34/34
// from TTree T1/Level1 Quantities
// found on file: xe100_120515_1950.root
//////////////////////////////////////////////////////////

#ifndef T1_h
#define T1_h

#include <TROOT.h>
#include <TChain.h>
#include <TFile.h>

// Header file for the classes stored in the TTree if any.
#include <vector>
#include <vector>

// Fixed size dimensions of array or collections stored in the TTree if any.

class T1 {
public :
   TTree          *fChain;   //!pointer to the analyzed TTree or TChain
   Int_t           fCurrent; //!current Tree number in a TChain

   // Declaration of leaf types
   Char_t          Filename[25];
   Int_t           EventId;
   Int_t           TimeSec;
   Int_t           TimeMicroSec;
   vector<int>     *Channels;
   vector<vector<float> > *BaselinesAvg;
   vector<vector<float> > *BaselinesMax;
   Float_t         AreaTotRaw;
   Float_t         AreaTotRawVeto;
   vector<float>   *AreaRaw;
   Float_t         AbsAreaTotRaw;
   vector<float>   *AbsAreaRaw;
   Int_t           NbNonZeroLast;
   Int_t           NbS1Peaks;
   vector<int>     *S1sPeak;
   vector<int>     *S1sLeftEdge;
   vector<int>     *S1sRightEdge;
   vector<float>   *S1sWidth;
   vector<float>   *S1sLowWidth;
   vector<float>   *S1sFilteredLowWidth;
   vector<float>   *S1sHeight;
   vector<float>   *S1sMinimum;
   vector<float>   *S1sMeanTime;
   vector<float>   *S1sTimeSpread;
   vector<int>     *S1sSaturation;
   vector<int>     *S1Order;
   vector<int>     *S1SizeOrder;
   Int_t           NbS2Peaks;
   vector<int>     *S2sPeak;
   vector<int>     *S2sLeftEdge;
   vector<int>     *S2sRightEdge;
   vector<float>   *S2sWidth;
   vector<float>   *S2sLowWidth;
   vector<float>   *S2sHeight;
   vector<int>     *S2sSaturation;
   vector<int>     *S2Order;
   Int_t           NbS1PeaksVeto;
   vector<int>     *S1sPeakVeto;
   vector<int>     *S1sLeftEdgeVeto;
   vector<int>     *S1sRightEdgeVeto;
   vector<float>   *S1sWidthVeto;
   vector<float>   *S1sLowWidthVeto;
   vector<float>   *S1sHeightVeto;
   vector<int>     *S1OrderVeto;
   vector<vector<float> > *S1sRaw;
   vector<vector<float> > *S2sRaw;
   vector<vector<float> > *S1sRawVeto;
   vector<vector<int> > *S1sMaxPosition;
   vector<vector<int> > *S1sMaxPositionVeto;

   // List of branches
   TBranch        *b_Filename;   //!
   TBranch        *b_EventId;   //!
   TBranch        *b_TimeSec;   //!
   TBranch        *b_TimeMicroSec;   //!
   TBranch        *b_Channels;   //!
   TBranch        *b_BaselinesAvg;   //!
   TBranch        *b_BaselinesMax;   //!
   TBranch        *b_AreaTotRaw;   //!
   TBranch        *b_AreaTotRawVeto;   //!
   TBranch        *b_AreaRaw;   //!
   TBranch        *b_AbsAreaTotRaw;   //!
   TBranch        *b_AbsAreaRaw;   //!
   TBranch        *b_NbNonZeroLast;   //!
   TBranch        *b_NbS1Peaks;   //!
   TBranch        *b_S1sPeak;   //!
   TBranch        *b_S1sLeftEdge;   //!
   TBranch        *b_S1sRightEdge;   //!
   TBranch        *b_S1sWidth;   //!
   TBranch        *b_S1sLowWidth;   //!
   TBranch        *b_S1sFilteredLowWidth;   //!
   TBranch        *b_S1sHeight;   //!
   TBranch        *b_S1sMinimum;   //!
   TBranch        *b_S1sMeanTime;   //!
   TBranch        *b_S1sTimeSpread;   //!
   TBranch        *b_S1sSaturation;   //!
   TBranch        *b_S1Order;   //!
   TBranch        *b_S1SizeOrder;   //!
   TBranch        *b_NbS2Peaks;   //!
   TBranch        *b_S2sPeak;   //!
   TBranch        *b_S2sLeftEdge;   //!
   TBranch        *b_S2sRightEdge;   //!
   TBranch        *b_S2sWidth;   //!
   TBranch        *b_S2sLowWidth;   //!
   TBranch        *b_S2sHeight;   //!
   TBranch        *b_S2sSaturation;   //!
   TBranch        *b_S2Order;   //!
   TBranch        *b_NbS1PeaksVeto;   //!
   TBranch        *b_S1sPeakVeto;   //!
   TBranch        *b_S1sLeftEdgeVeto;   //!
   TBranch        *b_S1sRightEdgeVeto;   //!
   TBranch        *b_S1sWidthVeto;   //!
   TBranch        *b_S1sLowWidthVeto;   //!
   TBranch        *b_S1sHeightVeto;   //!
   TBranch        *b_S1OrderVeto;   //!
   TBranch        *b_S1sRaw;   //!
   TBranch        *b_S2sRaw;   //!
   TBranch        *b_S1sRawVeto;   //!
   TBranch        *b_S1sMaxPosition;   //!
   TBranch        *b_S1sMaxPositionVeto;   //!

   T1(TTree *tree=0);
   virtual ~T1();
   virtual Int_t    Cut(Long64_t entry);
   virtual Int_t    GetEntry(Long64_t entry);
   virtual Long64_t LoadTree(Long64_t entry);
   virtual void     Init(TTree *tree);
   virtual void     Loop();
   virtual Bool_t   Notify();
   virtual void     Show(Long64_t entry = -1);
};

#endif

#ifdef T1_cxx
T1::T1(TTree *tree) : fChain(0) 
{
// if parameter tree is not specified (or zero), connect the file
// used to generate this class and read the Tree.
   if (tree == 0) {
      TFile *f = (TFile*)gROOT->GetListOfFiles()->FindObject("xe100_120515_1950.root");
      if (!f || !f->IsOpen()) {
         f = new TFile("xe100_120515_1950.root");
      }
      f->GetObject("T1",tree);

   }
   Init(tree);
}

T1::~T1()
{
   if (!fChain) return;
   delete fChain->GetCurrentFile();
}

Int_t T1::GetEntry(Long64_t entry)
{
// Read contents of entry.
   if (!fChain) return 0;
   return fChain->GetEntry(entry);
}
Long64_t T1::LoadTree(Long64_t entry)
{
// Set the environment to read one entry
   if (!fChain) return -5;
   Long64_t centry = fChain->LoadTree(entry);
   if (centry < 0) return centry;
   if (fChain->GetTreeNumber() != fCurrent) {
      fCurrent = fChain->GetTreeNumber();
      Notify();
   }
   return centry;
}

void T1::Init(TTree *tree)
{
   // The Init() function is called when the selector needs to initialize
   // a new tree or chain. Typically here the branch addresses and branch
   // pointers of the tree will be set.
   // It is normally not necessary to make changes to the generated
   // code, but the routine can be extended by the user if needed.
   // Init() will be called many times when running on PROOF
   // (once per file to be processed).

   // Set object pointer
   Channels = 0;
   BaselinesAvg = 0;
   BaselinesMax = 0;
   AreaRaw = 0;
   AbsAreaRaw = 0;
   S1sPeak = 0;
   S1sLeftEdge = 0;
   S1sRightEdge = 0;
   S1sWidth = 0;
   S1sLowWidth = 0;
   S1sFilteredLowWidth = 0;
   S1sHeight = 0;
   S1sMinimum = 0;
   S1sMeanTime = 0;
   S1sTimeSpread = 0;
   S1sSaturation = 0;
   S1Order = 0;
   S1SizeOrder = 0;
   S2sPeak = 0;
   S2sLeftEdge = 0;
   S2sRightEdge = 0;
   S2sWidth = 0;
   S2sLowWidth = 0;
   S2sHeight = 0;
   S2sSaturation = 0;
   S2Order = 0;
   S1sPeakVeto = 0;
   S1sLeftEdgeVeto = 0;
   S1sRightEdgeVeto = 0;
   S1sWidthVeto = 0;
   S1sLowWidthVeto = 0;
   S1sHeightVeto = 0;
   S1OrderVeto = 0;
   S1sRaw = 0;
   S2sRaw = 0;
   S1sRawVeto = 0;
   S1sMaxPosition = 0;
   S1sMaxPositionVeto = 0;
   // Set branch addresses and branch pointers
   if (!tree) return;
   fChain = tree;
   fCurrent = -1;
   fChain->SetMakeClass(1);

   fChain->SetBranchAddress("Filename", Filename, &b_Filename);
   fChain->SetBranchAddress("EventId", &EventId, &b_EventId);
   fChain->SetBranchAddress("TimeSec", &TimeSec, &b_TimeSec);
   fChain->SetBranchAddress("TimeMicroSec", &TimeMicroSec, &b_TimeMicroSec);
   fChain->SetBranchAddress("Channels", &Channels, &b_Channels);
   fChain->SetBranchAddress("BaselinesAvg", &BaselinesAvg, &b_BaselinesAvg);
   fChain->SetBranchAddress("BaselinesMax", &BaselinesMax, &b_BaselinesMax);
   fChain->SetBranchAddress("AreaTotRaw", &AreaTotRaw, &b_AreaTotRaw);
   fChain->SetBranchAddress("AreaTotRawVeto", &AreaTotRawVeto, &b_AreaTotRawVeto);
   fChain->SetBranchAddress("AreaRaw", &AreaRaw, &b_AreaRaw);
   fChain->SetBranchAddress("AbsAreaTotRaw", &AbsAreaTotRaw, &b_AbsAreaTotRaw);
   fChain->SetBranchAddress("AbsAreaRaw", &AbsAreaRaw, &b_AbsAreaRaw);
   fChain->SetBranchAddress("NbNonZeroLast", &NbNonZeroLast, &b_NbNonZeroLast);
   fChain->SetBranchAddress("NbS1Peaks", &NbS1Peaks, &b_NbS1Peaks);
   fChain->SetBranchAddress("S1sPeak", &S1sPeak, &b_S1sPeak);
   fChain->SetBranchAddress("S1sLeftEdge", &S1sLeftEdge, &b_S1sLeftEdge);
   fChain->SetBranchAddress("S1sRightEdge", &S1sRightEdge, &b_S1sRightEdge);
   fChain->SetBranchAddress("S1sWidth", &S1sWidth, &b_S1sWidth);
   fChain->SetBranchAddress("S1sLowWidth", &S1sLowWidth, &b_S1sLowWidth);
   fChain->SetBranchAddress("S1sFilteredLowWidth", &S1sFilteredLowWidth, &b_S1sFilteredLowWidth);
   fChain->SetBranchAddress("S1sHeight", &S1sHeight, &b_S1sHeight);
   fChain->SetBranchAddress("S1sMinimum", &S1sMinimum, &b_S1sMinimum);
   fChain->SetBranchAddress("S1sMeanTime", &S1sMeanTime, &b_S1sMeanTime);
   fChain->SetBranchAddress("S1sTimeSpread", &S1sTimeSpread, &b_S1sTimeSpread);
   fChain->SetBranchAddress("S1sSaturation", &S1sSaturation, &b_S1sSaturation);
   fChain->SetBranchAddress("S1Order", &S1Order, &b_S1Order);
   fChain->SetBranchAddress("S1SizeOrder", &S1SizeOrder, &b_S1SizeOrder);
   fChain->SetBranchAddress("NbS2Peaks", &NbS2Peaks, &b_NbS2Peaks);
   fChain->SetBranchAddress("S2sPeak", &S2sPeak, &b_S2sPeak);
   fChain->SetBranchAddress("S2sLeftEdge", &S2sLeftEdge, &b_S2sLeftEdge);
   fChain->SetBranchAddress("S2sRightEdge", &S2sRightEdge, &b_S2sRightEdge);
   fChain->SetBranchAddress("S2sWidth", &S2sWidth, &b_S2sWidth);
   fChain->SetBranchAddress("S2sLowWidth", &S2sLowWidth, &b_S2sLowWidth);
   fChain->SetBranchAddress("S2sHeight", &S2sHeight, &b_S2sHeight);
   fChain->SetBranchAddress("S2sSaturation", &S2sSaturation, &b_S2sSaturation);
   fChain->SetBranchAddress("S2Order", &S2Order, &b_S2Order);
   fChain->SetBranchAddress("NbS1PeaksVeto", &NbS1PeaksVeto, &b_NbS1PeaksVeto);
   fChain->SetBranchAddress("S1sPeakVeto", &S1sPeakVeto, &b_S1sPeakVeto);
   fChain->SetBranchAddress("S1sLeftEdgeVeto", &S1sLeftEdgeVeto, &b_S1sLeftEdgeVeto);
   fChain->SetBranchAddress("S1sRightEdgeVeto", &S1sRightEdgeVeto, &b_S1sRightEdgeVeto);
   fChain->SetBranchAddress("S1sWidthVeto", &S1sWidthVeto, &b_S1sWidthVeto);
   fChain->SetBranchAddress("S1sLowWidthVeto", &S1sLowWidthVeto, &b_S1sLowWidthVeto);
   fChain->SetBranchAddress("S1sHeightVeto", &S1sHeightVeto, &b_S1sHeightVeto);
   fChain->SetBranchAddress("S1OrderVeto", &S1OrderVeto, &b_S1OrderVeto);
   fChain->SetBranchAddress("S1sRaw", &S1sRaw, &b_S1sRaw);
   fChain->SetBranchAddress("S2sRaw", &S2sRaw, &b_S2sRaw);
   fChain->SetBranchAddress("S1sRawVeto", &S1sRawVeto, &b_S1sRawVeto);
   fChain->SetBranchAddress("S1sMaxPosition", &S1sMaxPosition, &b_S1sMaxPosition);
   fChain->SetBranchAddress("S1sMaxPositionVeto", &S1sMaxPositionVeto, &b_S1sMaxPositionVeto);
   Notify();
}

Bool_t T1::Notify()
{
   // The Notify() function is called when a new file is opened. This
   // can be either for a new TTree in a TChain or when when a new TTree
   // is started when using PROOF. It is normally not necessary to make changes
   // to the generated code, but the routine can be extended by the
   // user if needed. The return value is currently not used.

   return kTRUE;
}

void T1::Show(Long64_t entry)
{
// Print contents of entry.
// If entry is not specified, print current entry
   if (!fChain) return;
   fChain->Show(entry);
}
Int_t T1::Cut(Long64_t entry)
{
// This function may be called from Loop.
// returns  1 if entry is accepted.
// returns -1 otherwise.
   return 1;
}
#endif // #ifdef T1_cxx
