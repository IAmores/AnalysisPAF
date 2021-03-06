///////////////////////////////////////////////////////////////////////////////////////////
//
//  Lepton Selector: create vectors with selected leptons, veto (or fakeable) leptons...
//  
//  All SFs and variables are within the Lepton definition
//
//  To do: definition of veto leptons
//         
//
///////////////////////////////////////////////////////////////////////////////////////////


#include "LeptonSelector.h"

ClassImp(LeptonSelector);
LeptonSelector::LeptonSelector() : PAFChainItemSelector() {}
void LeptonSelector::Summary(){}

void LeptonSelector::Initialise(){
  // Initialise LeptonSelector
  gIsData        = GetParam<Bool_t>("IsData");
  gIsFastSim   = GetParam<Bool_t>("IsFastSim");
  gSelection     = GetParam<Int_t>("iSelection");
  localPath      = GetParam<TString>("WorkingDir");
  LepSF = new LeptonSF(localPath + "/InputFiles/");

  //<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<
  // Select SFs for each analysis !!!!!!
  if(gSelection == iStopSelec){
    LepSF->loadHisto(iMuonReco);
    LepSF->loadHisto(iElecReco);
    LepSF->loadHisto(iMuonIdSUSY,   iMedium);
    LepSF->loadHisto(iMuonIsoSUSY,  iWPforStop);
    LepSF->loadHisto(iElecIdSUSY,   iWPforStop);
    LepSF->loadHisto(iElecIsoSUSY,   iWPforStop);
    if(gIsFastSim){
      LepSF->loadHisto(iElecFastSim);
      LepSF->loadHisto(iMuonIdFastSim);
      LepSF->loadHisto(iMuonIsoFastSim);
    }
  }
  else if(gSelection == iTopSelec || gSelection == iTWSelec || gSelection == iWWSelec){
    LepSF->loadHisto(iMuonReco);
    LepSF->loadHisto(iElecReco);
    LepSF->loadHisto(iMuonId,   iTight);
    LepSF->loadHisto(iMuonIsoTightId,   iTight);
    LepSF->loadHisto(iElecId,   iTight);
  }
  else std::cout << ">>>>>>>>>>>> WRONG SELECTION!!!!!!!!" << std::endl;
  //<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<
 
  selLeptons = std::vector<Lepton>();
}

//################################################################
//## Definition of wps...
//################################################################
Bool_t LeptonSelector::getSIPcut(Float_t cut){ 
  if(sip > cut) return false;
  return true; 
}

Bool_t LeptonSelector::getGoodVertex(Int_t wp){ 
  if(type == 1){ //electrions
    if(wp == iTight){ 
      if(etaSC <= 1.479 && ((dxy >= 0.05) || (dz  >= 0.10))) return false;
      if(etaSC >  1.479 && ((dxy >= 0.10) || (dz  >= 0.20))) return false;
    }
  } 
  else{ // muons
    if(wp == iMedium && (dxy > 0.2  || dz > 0.5)) return false;
    if(wp == iTight  && (dxy > 0.05 || dz > 0.1)) return false;
  }
  return true;
}

Bool_t LeptonSelector::getRelIso03POG(Int_t wp){ 
  if(type == 1){ // electrons 
    if(wp == iWPforStop && RelIso03 > 0.12) return false; 
    if(etaSC <= 1.479){
      if(wp == iVeto   && RelIso03 > 0.1750) return false; 
      if(wp == iLoose  && RelIso03 > 0.0994) return false; 
      if(wp == iMedium && RelIso03 > 0.0695) return false; 
      if(wp == iTight  && RelIso03 > 0.0588) return false; 
    }
    else if(etaSC > 1.479){
      if(wp == iVeto   && RelIso03 > 0.1590) return false; 
      if(wp == iLoose  && RelIso03 > 0.1070) return false; 
      if(wp == iMedium && RelIso03 > 0.0821) return false; 
      if(wp == iTight  && RelIso03 > 0.0571) return false; 
    }
  }
  else{ // muons
    if(wp == iLoose  && RelIso03 > 0.10) return false; 
    if(wp == iTight  && RelIso03 > 0.05) return false; 
    if(wp == iWPforStop && RelIso03 > 0.12) return false; 
  }
  return true;
}

Bool_t LeptonSelector::getRelIso04POG(Int_t wp){ // wps for muons  
  if(type == 1) return false; // electrons
  if(wp == iLoose  && RelIso04 > 0.25) return false; 
  if(wp == iTight  && RelIso04 > 0.15) return false; 
  return true;
}

Bool_t LeptonSelector::getMuonId(Int_t wp){ 
  if(wp == iTight   && !tightVar)     return false;
  if(wp == iMedium  && !mediumMuonId) return false;
  return true;
}

Bool_t LeptonSelector::getElecCutBasedId(Int_t wp){ 
  if(wp == iTight   && tightVar < 3)     return false;
  if(wp == iMedium  && tightVar < 2)     return false;
  if(wp == iLoose   && tightVar < 1)     return false;
  if(wp == iVeto    && tightVar < 0)     return false;
  if(wp == iWPforStop){ // Spring 15
    if(TMath::Abs(etaSC) < 1.479){ // central
      if(TMath::Abs(sigmaIEtaIEta) > 0.0101 ) return false;
      if(TMath::Abs(dEtaSC) > 0.00926  ) return false;
      if(TMath::Abs(dPhiSC) > 0.0336 ) return false;
      if(HoE > 0.0597 ) return false;
      if(TMath::Abs(eImpI) > 0.012 ) return false;
      if(lostHits         > 2      ) return false;
    }
    else{ // forward
      if(TMath::Abs(sigmaIEtaIEta) > 0.0279 ) return false;
      if(TMath::Abs(dEtaSC) > 0.00724  ) return false;
      if(TMath::Abs(dPhiSC) > 0.0918 ) return false;
      if(HoE > 0.0615 ) return false;
      if(TMath::Abs(eImpI) > 0.00999 ) return false;
      if(lostHits         > 1      ) return false;
    }
  }   
    if (convVeto  == 0   ) return false;
  return true;
}

Bool_t LeptonSelector::getMultiIso(Int_t wp){ 
  if(wp == iVeryTight && !(miniIso < 0.09 && (ptRatio > 0.84 || ptRel > 7.2)) )     return false;
  if(wp == iTight   )     return false;
  if(wp == iMedium  )     return false;
  if(wp == iLoose   )     return false;
  if(wp == iVeto    )     return false;
  return true;
}


//################################################################
//## Lepton definitions for each analysis
//################################################################
//
// Use the functions above to define your objects
//
//=============================================================== SELECTED LEPTONS
Bool_t LeptonSelector::isGoodLepton(Lepton lep){ 
  Bool_t passId; Bool_t passIso;
  if(gSelection == iStopSelec){
    // Tight cut-based electrons, pT > 20, |eta| < 2.4, RelIso POG, tightIP2D, SIP3D > 4 
    // Medium Muon ID, RelIso POG, tightIP2D, SIP3D > 4  
    if(lep.isMuon){
      passId  = getMuonId(iMedium);
      passIso = getRelIso03POG(iWPforStop);
    }
    if(lep.isElec){
      passId = getElecCutBasedId(iWPforStop);
      passIso = getRelIso03POG(iWPforStop);
      if(TMath::Abs(etaSC) > 1.4442 && TMath::Abs(etaSC) < 1.566) return false;
    }
    if(lep.p.Pt() < 20 || TMath::Abs(lep.p.Eta()) > 2.4) return false;
    if(passId && passIso && getGoodVertex(iTight) && getSIPcut(4)) return true;
    else return false;
  }
  else if(gSelection == iTopSelec || gSelection == iTWSelec){
    // Tight cut-based electrons, pT > 20, |eta| < 2.4, RelIso POG, tightIP2D, SIP3D > 4 
    // Tight Muon ID, RelIso POG, tightIP2D, SIP3D > 4 
    if(lep.isMuon){
      passId  = getMuonId(iTight);
      passIso = getRelIso04POG(iTight);
    }
    if(lep.isElec){
      passId = getElecCutBasedId(iTight) && lostHits <= 1;
      passIso = getRelIso03POG(iTight);
      if(TMath::Abs(etaSC) > 1.4442 && TMath::Abs(etaSC) < 1.566) return false;
    }
    if(lep.p.Pt() < 20 || TMath::Abs(lep.p.Eta()) > 2.4) return false;
    if(passId && passIso && ( (lep.isElec && getGoodVertex(iTight)) || (lep.isMuon && getGoodVertex(iMedium) ))) return true;
    else return false;
  }
  else if(gSelection == iWWSelec){
    if(lep.isMuon){
      passId  = getMuonId(iTight);
      passIso = getRelIso04POG(iTight);
    }
    if(lep.isElec){
      passId = getElecCutBasedId(iTight);
      passIso = getRelIso03POG(iTight);
      if(TMath::Abs(etaSC) > 1.4442 && TMath::Abs(etaSC) < 1.566) return false;
    }
    if(lep.p.Pt() < 20 || TMath::Abs(lep.p.Eta()) > 2.4) return false;
    if(passId && passIso && getGoodVertex(iTight) && getSIPcut(4)) return true;
    else return false;
    return false;
  }
}

//============================================== VETO LEPTONS
Bool_t LeptonSelector::isVetoLepton(Lepton lep){ 
  Bool_t passId; Bool_t passIso;
  if(gSelection == iStopSelec){
    if(lep.isMuon){ 
      passId = true;
      passIso = getRelIso03POG(iWPforStop);
    }
    else{
      passId = getElecCutBasedId(iVeto);
      passIso = getRelIso03POG(iWPforStop);
    } 
    return passId && passIso && getGoodVertex(iTight) && getSIPcut(4);
  }
  else if(gSelection == iTopSelec){
    return true;
  }
  else if(gSelection == iWWSelec){
    return true;
  }
  else if(gSelection == ittHSelec){ //FO
    return true;
  }
  return false;
}

//============================================== Loose leptons (or other)
Bool_t LeptonSelector::isLooseLepton(Lepton lep){
  if(gSelection == ittHSelec){
    return true;
  }
}


///////////////////////////////////////////////////////////////////////////
// You do not want to change anything below this point 
///////////////////////////////////////////////////////////////////////////

void LeptonSelector::InsideLoop(){
  // Clear vectors...
  selLeptons.clear();
  looseLeptons.clear();
  genLeptons.clear();
  vetoLeptons.clear(); 
  nLeptonsFromTau = 0;
  // Loop over the leptons and select
  nLep     = Get<Int_t>("nLepGood"); 
  for(Int_t i = 0; i < nLep; i++){
    GetLeptonVariables(i);
    tL = Lepton(tP, charge, type);
    if(isGoodLepton(tL)){
      tL.SetSF(   LepSF->GetLeptonSF(     pt, eta, tL.type) ); // Set SF and error
      tL.SetSFerr(LepSF->GetLeptonSFerror(pt, eta, tL.type) );
      selLeptons.push_back(tL);
    }
    if(isVetoLepton(tL)){ // If you need to veto extra leptons...
      //tL.SetSF(1); tL.SetSFerr(1); // To be updated if ever needed
      vetoLeptons.push_back(tL);
    }
    if(isLooseLepton(tL)){ // A loose category... used in ttH, for example
      //tL.SetSF(1); tL.SetSFerr(1); // To be updated if ever needed
      looseLeptons.push_back(tL); 
    }
  }

  // Loop over the gen leptons and get gen info...
  if(!gIsData){  
    ngenLep         = Get<Int_t>("ngenLep");
    ngenLepFromTau  = Get<Int_t>("ngenLepFromTau");
    for(Int_t i = 0; i < ngenLep; i++){
      GetGenLeptonVariables(i);
      if(gpdgMId == 23 || gpdgMId == 24 || gpdgMId == 25){
        tL = Lepton(tP, charge, type);
        genLeptons.push_back(tL);
      } 
    }
    for(Int_t i = 0; i < ngenLepFromTau; i++){
      GetGenLepFromTauVariables(i);
      if(gpdgMId == 23 || gpdgMId == 24 || gpdgMId == 25){
        tL = Lepton(tP, charge, type);
        nLeptonsFromTau++;
        genLeptons.push_back(tL);
      } 
    } 
  }
  nSelLeptons = selLeptons.size();
  nVetoLeptons = vetoLeptons.size();;
  nGenLeptons  = genLeptons.size();
  
  // Set params for the next selectors
  SetParam("selLeptons",  selLeptons );
  SetParam("vetoLeptons", vetoLeptons);
  SetParam("looseLeptons", looseLeptons);
  SetParam("genLeptons",  genLeptons );
  SetParam("nLeptonsFromTau", nLeptonsFromTau);
  SetParam("nGenLeptons", nGenLeptons);
  SetParam("nSelLeptons", nSelLeptons);
  SetParam("nVetoLeptons", nVetoLeptons);
}

//################################################################
//## Get important variables
//################################################################
void LeptonSelector::GetLeptonVariables(Int_t i){ // Once per muon, get all the info
  tP.SetPxPyPzE(Get<Float_t>("LepGood_px", i), Get<Float_t>("LepGood_py", i), Get<Float_t>("LepGood_pz", i), Get<Float_t>("LepGood_energy", i));
  pt = tP.Pt();
  eta = tP.Eta();
  charge = Get<Int_t>("LepGood_charge", i); 
  type = TMath::Abs(Get<Int_t>("LepGood_pdgId",i)) == 11 ? 1 : 0;
  tightVar = Get<Int_t>("LepGood_tightId", i); 
  mediumMuonId = Get<Int_t>("LepGood_mediumMuonId",i); 
  etaSC = TMath::Abs(Get<Float_t>("LepGood_etaSc",i));
  RelIso03 = Get<Float_t>("LepGood_relIso03",i);
  RelIso04 = Get<Float_t>("LepGood_relIso04",i);
  ptRel = Get<Float_t>("LepGood_jetPtRelv2",i);
  ptRatio = Get<Float_t>("LepGood_jetPtRatiov2",i);
  miniIso =Get<Float_t>("LepGood_miniRelIso",i);
  dxy = TMath::Abs(Get<Float_t>("LepGood_dxy", i));
  dz  = TMath::Abs(Get<Float_t>("LepGood_dz", i));
  sigmaIEtaIEta = Get<Float_t>("LepGood_sigmaIEtaIEta", i);
  dEtaSC =    Get<Float_t>("LepGood_dEtaScTrkIn", i);
  dPhiSC =    Get<Float_t>("LepGood_dPhiScTrkIn", i);
  HoE    =    Get<Float_t>("LepGood_hadronicOverEm", i);
  eImpI  =    Get<Float_t>("LepGood_eInvMinusPInv", i);
  lostHits =  Get<Int_t>("LepGood_lostHits", i);
  convVeto =  Get<Int_t>("LepGood_convVeto", i);

  sip = Get<Float_t>("LepGood_sip3d",i);
  SF = 1;
}

void LeptonSelector::GetGenLeptonVariables(Int_t i){
  tP.SetPtEtaPhiM(Get<Float_t>("genLep_pt", i), Get<Float_t>("genLep_eta", i), Get<Float_t>("genLep_phi", i), Get<Float_t>("genLep_mass", i));
  charge = Get<Int_t>("genLep_charge", i);
  gpdgMId = TMath::Abs(Get<Int_t>("genLep_motherId", i));
  type = TMath::Abs(Get<Int_t>("genLep_pdgId",i)) == 11 ? 1 : 0;
}

void LeptonSelector::GetGenLepFromTauVariables(Int_t i){
  tP.SetPtEtaPhiM(Get<Float_t>("genLepFromTau_pt", i), Get<Float_t>("genLepFromTau_eta", i), Get<Float_t>("genLepFromTau_phi", i), Get<Float_t>("genLepFromTau_mass", i));
  charge = Get<Int_t>("genLepFromTau_charge", i);
  gpdgMId = TMath::Abs(Get<Int_t>("genLepFromTau_grandmotherId", i));
  type = TMath::Abs(Get<Int_t>("genLepFromTau_pdgId",i)) == 11 ? 1 : 0;
}
