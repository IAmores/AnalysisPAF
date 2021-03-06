#pragma once

#include "PAFChainItemSelector.h"
#include "Functions.h"
#include <iostream>
#include <vector>

enum eChannels{iUnkChan, iElMu, iMuon, iElec, nChannels};
enum eLevels  {idilepton, iZVeto, iMETcut, i2jets, i1btag, nLevels};
enum eSysts   {inorm, nSysts};
const int nWeights = 248;
const TString gChanLabel[nChannels] = {"X", "ElMu", "Muon","Elec"};
const TString sCut[nLevels] = {"dilepton", "ZVeto", "MET", "2jets", "1btag"};
const TString gSys[nSysts] = {"0"};


class TopAnalysis : public PAFChainItemSelector{
  public:
    TopAnalysis();
    virtual ~TopAnalysis(){}
    virtual void InsideLoop();
    virtual void Initialise();
    virtual void Summary();
    std::vector<Lepton> genLeptons  ;
    std::vector<Lepton> selLeptons  ;
    std::vector<Lepton> vetoLeptons ;
    std::vector<Jet> selJets ;
    std::vector<Jet> Jets15  ;

    TTree* fTree;
    void SetLeptonVariables();
    void SetJetVariables();
    void SetEventVariables();

    Bool_t makeHistos = false;
    Bool_t makeTree   = false;

    void GetLeptonVariables(std::vector<Lepton> selLeptons, std::vector<Lepton> VetoLeptons);
    void GetJetVariables(std::vector<Jet> selJets, std::vector<Jet> cleanedJets15, Float_t ptCut = 30);
    void GetMET();

    Float_t TrigSF;
    Float_t TrigSF_Up;
    Float_t TrigSF_Down;
    Float_t PUSF;
    Float_t PUSF_Up;
    Float_t PUSF_Down;
    Int_t   gChannel;
    Bool_t  passMETfilters;
    Bool_t  passTrigger;
    Bool_t  isSS;
    Float_t NormWeight;

    void InitHistos();
    void FillHistos(Int_t ch, Int_t cut);
  
    //Variables
    Float_t TWeight;   // Total nominal weight
    Float_t TMll;      // Invariant mass
    Float_t TMET;      // MET
    Float_t TMET_Phi;  // MET phi

    Int_t   TNVetoLeps;
    Int_t   TNSelLeps;
    Int_t   TChannel;
    Float_t TLep_Pt[10];    
    Float_t TLep_Eta[10];
    Float_t TLep_Phi[10];
    Float_t TLep_E[10];
    Float_t TLep_Charge[10];

    Int_t TNJets;            // Jets...
    Int_t TNBtags;
    Float_t TJet_Pt[20];
    Float_t TJet_Eta[20];
    Float_t TJet_Phi[20];
    Float_t TJet_E[20];
    Int_t TJet_isBJet[20];
    Float_t THT;       // HT

    // For systematics...
    Int_t   TNJetsJESUp;
    Int_t   TNJetsJESDown;
    Int_t   TNJetsJER;
    Int_t   TNBtagsUp;
    Int_t   TNBtagsDown;
    Int_t   TNBtagsMisTagUp;
    Int_t   TNBtagsMisTagDown;
    Int_t   TNBtagsJESUp;
    Int_t   TNBtagsJESDown;
    Float_t TJetJESUp_Pt[20];
    Float_t TJetJESDown_Pt[20];
    Float_t TJetJER_Pt[20];
    Float_t THTJESUp;
    Float_t THTJESDown;

    Int_t   TNISRJets;
    Float_t TMETJESUp;
    Float_t TMETJESDown;
    Float_t TMT2llJESUp;
    Float_t TMT2llJESDown;

    Float_t  TWeight_LepEffUp;
    Float_t  TWeight_LepEffDown;
    Float_t  TWeight_TrigUp;
    Float_t  TWeight_TrigDown;
    Float_t  TWeight_FSUp;
    Float_t  TWeight_PUDown;
    Float_t  TWeight_PUUp;
    Float_t  TWeight_FSDown;

// Histograms
//=====================================================0
  TH1F* fHLHEweights[nChannels][nLevels][nSysts];
  TH1F* fHMET[nChannels][nLevels][nSysts];
  TH1F* fHLep0Eta[nChannels][nLevels][nSysts];
  TH1F* fHLep1Eta[nChannels][nLevels][nSysts];
  TH1F* fHDelLepPhi[nChannels][nLevels][nSysts];
  TH1F* fHHT[nChannels][nLevels][nSysts];
  TH1F* fHJet0Eta[nChannels][nLevels][nSysts];
  TH1F* fHJet1Eta[nChannels][nLevels][nSysts];

  TH1F* fHDiLepPt[nChannels][nLevels][nSysts];
  TH1F* fHLep0Pt[nChannels][nLevels][nSysts];
  TH1F* fHLep1Pt[nChannels][nLevels][nSysts];
  TH1F* fHJet0Pt[nChannels][nLevels][nSysts];
  TH1F* fHJet1Pt[nChannels][nLevels][nSysts];
  TH1F* fHNJets[nChannels][nLevels][nSysts];
  TH1F* fHNBtagJets[nChannels][nLevels][nSysts];

  TH1F* fHInvMass[nChannels][nLevels][nSysts];
  TH1F* fHInvMass2[nChannels][nLevels][nSysts];
  TH1F* fHNBtagsNJets[nChannels][nLevels][nSysts];
  TH1F* fHJetCSV[nChannels][nLevels][nSysts];
  TH1F* fHvertices[nChannels][nLevels][nSysts]; 

  TH1F* fhDummy;
  TH1F*  fHyields[nChannels][nSysts];
  TH1F*  fHSSyields[nChannels][nSysts];

  protected:
    Bool_t  gIsData;
    Bool_t  gDoSyst;
    Int_t   gSelection;
    TString gSampleName;
    Bool_t  gIsTTbar;
    Bool_t  gIsTW;

    ClassDef(TopAnalysis, 0);
};
