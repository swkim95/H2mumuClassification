#include "./SetupTree.C"

#include <TStyle.h>
#include <TChain.h>
#include <TFile.h>
#include <TH1D.h>
#include <TGraphAsymmErrors.h>
#include <TMultiGraph.h>
#include <TLegend.h>
#include <TCanvas.h>
#include <TROOT.h>
#include <THStack.h>
#include <TMath.h>
#include <TText.h>
#include <TPad.h>
#include <TPaveText.h>
#include <TLorentzVector.h>
#include <TString.h>

#include <iostream>
#include <string>
#include <fstream>
#include <sstream>
#include <iomanip>
#include <vector>

using namespace std;
TString _dataset = "Higgs2mumu";
TString _trig = "HLT_IsoMu24_eta2p1_v*"; //single mu trigger for mumu, 4mu, 3mu1e
double intLumi = 14817;
double _etaCut = 2.4;

TH1D* fillHist( TString, TString, TString, TString, double, int, double, double, bool );

void makeMode(vector<TString> &mode) {
	mode.push_back("cktpt");
	mode.push_back("eta");
	mode.push_back("trackerLayers");
	mode.push_back("pixelHits");
	mode.push_back("RelIso");
	mode.push_back("dxyVTX");
	mode.push_back("dzVTX");
	mode.push_back("muonHits");
	mode.push_back("nMatches");
	mode.push_back("cktptReso");
}

void massSpec( double _ptCut = 30.0, TString _channel = "mumu" ) 
{   
	vector<TString> mode;
	makeMode(mode);

    TString _ptCutname = "";
    ostringstream name0;
    name0 << "_pt" << (int)_ptCut;
    _ptCutname = TString(name0.str());
    cout << "_ptCutname = " << _ptCutname << endl;

    TFile* f = new TFile("rootfiles/variableSpec_"+_channel+_ptCutname+".root", "recreate");
    gStyle->SetOptStat(0);

    int nbins[10] = {100, 48, 13, 10, 10, 40, 100, 55, 6, 30};
    double xbin1s[10] = {30, -2.4, 5, 0, 0, -0.2, -0.5, 0, 0, 0};
    double xbin2s[10] = {80, 2.4, 18, 10, 0.1, 0.2, 0.5, 55, 6, 0.3};
	f->cd();

    // Higgs to mumu histogram, dimuon invariant mass plot
    TH1D* hdata1 = fillHist(_dataset+"_Run2012", _trig, "diMuonMass", _channel, _ptCut, 100, 100, 150, true);
	hdata1->Write();
    // DY to mumu histogram, dimuon invariant mass plot
    TH1D* hDY = fillHist("DYJets", _trig, "diMuonMass", _channel, _ptCut, 100, 100, 150, true);
	hDY->Write();

	// Higgs & DY single muon variable histogram
	for(int i = 0; i < mode.size(); ++i) {
		TString optPlot = mode[i];
		int nbin = nbins[i];
		double xbin1 = xbin1s[i];
		double xbin2 = xbin2s[i];
		TH1D* hHiggsMuPlus = fillHist(_dataset+"_Run2012", _trig, optPlot, _channel, _ptCut, nbin, xbin1, xbin2, true);
		TH1D* hHiggsMuMinus = fillHist(_dataset+"_Run2012", _trig, optPlot, _channel, _ptCut, nbin, xbin1, xbin2, false);
		TH1D* hDYMuPlus = fillHist("DYJets", _trig, optPlot, _channel, _ptCut, nbin, xbin1, xbin2, true);
		TH1D* hDYMuMinus = fillHist("DYJets", _trig, optPlot, _channel, _ptCut, nbin, xbin1, xbin2, false);
		
		hHiggsMuPlus->Write();
		hHiggsMuMinus->Write();
		hDYMuPlus->Write();
		hDYMuMinus->Write();
	}

    f->Close();
}

TH1D* fillHist( TString _fname, TString trig, TString _optPlot, TString _channel, double _ptCut, int nbin, double xbin1, double xbin2, bool isPlus ) {
    
	// Loading ntuples
	TChain* sample = new TChain("recoTree/DYTree");
    SetupTree(_fname, sample);
	TString variable = _optPlot;
	
	// Deciding plot mode
	if( _optPlot != "diMuonMass") {
		if (isPlus) { _optPlot = "_MuPlus_" + _optPlot; }
		else { _optPlot = "_MuMinus_" + _optPlot; }
	}
	else { _optPlot = "_"+_optPlot; }

	// Preparing histogram 
    TH1D* _h1 = new TH1D( _fname + _optPlot, _fname + _optPlot, nbin, xbin1, xbin2);

    for( int i = 0; i < sample->GetEntries(); i++ ) {
        sample->GetEntry(i);

        if( i % 100000 == 0 ) cout << "Processing " << _fname << _optPlot << " evt # = " << i << endl;
		if( i == sample->GetEntries()-1) cout << "Processing " << _fname << _optPlot << " last evt #  = " << i+1 << endl;

		bool isTriggered = false;
		
		for( int k = 0; k < hlt_ntrig; k++ ) {
			if( TString(hlt_trigName->at((unsigned int)k)) == trig ) {
				if( hlt_trigFired[k] == 1 ) {
					isTriggered = true;
					break;
				}
			}
		}

        if( !isTriggered ) continue;
		
		if( _channel == "mumu" ) {
			vector<TLorentzVector> muPlus, muMinus;
			vector<double> cktptPlus, cktPtMinus, etaPlus, etaMinus, RelIsoPlus, RelIsoMinus, dxyVTXPlus, dxyVTXMinus, dzVTXPlus, dzVTXMinus, resoPlus, resoMinus;
			vector<int> trkLayersPlus, trkLayersMinus, pixelHitsPlus, pixelHitsMinus, MuHitsPlus, MuHitsMinus, nMatchesPlus, nMatchesMinus;
			
			for( int j = 0; j < nMuon; j++ ) {
				// for checking muon charge
				TLorentzVector tmpMu;
				double mu_mass = 0.105658;
				double rpx = muon_px[j];
				double rpy = muon_py[j];
				double rpz = muon_pz[j];
				double rp = rpx*rpx + rpy*rpy + rpz*rpz;
				double re = sqrt(mu_mass*mu_mass + rp);
				tmpMu.SetPxPyPzE(rpx, rpy, rpz, re);
				
				// GLB muon only
				if( muon_type[j] != 0 && muon_type[j] != 1 ) continue;
				// relative track isolation 
				double _muIso = muon_trkiso[j];
				double _muRelIso = (_muIso)/muon_cktpt[j];

				// good muon selection
				if( muon_cktpt[j] > _ptCut 
						&& fabs(muon_eta[j]) < _etaCut 
						&& muon_trackerLayers[j] > 5
						&& muon_pixelHits[j] > 0 
						&& _muRelIso < 0.10
						&& fabs(muon_dxyVTX[j]) < 0.2
						&& fabs(muon_dzVTX[j]) < 0.5 
						&& muon_muonHits[j] > 0 
						&& muon_nMatches[j] > 1 
						&& muon_cktptError[j] / muon_cktpt[j] < 0.3 ) {
					if( muon_charge[j] > 0 ) {
						muPlus.push_back(tmpMu);
						cktptPlus.push_back(muon_cktpt[j]);
						etaPlus.push_back(muon_eta[j]);
						RelIsoPlus.push_back(_muRelIso);
						dxyVTXPlus.push_back(muon_dxyVTX[j]);
						dzVTXPlus.push_back(muon_dzVTX[j]);
						resoPlus.push_back(muon_cktptError[j] / muon_cktpt[j]);
						trkLayersPlus.push_back(muon_trackerLayers[j]);
						pixelHitsPlus.push_back(muon_pixelHits[j]);
						MuHitsPlus.push_back(muon_muonHits[j]);
						nMatchesPlus.push_back(muon_nMatches[j]);
						
					}
					if( muon_charge[j] < 0 ) {
						muMinus.push_back(tmpMu);					
						cktPtMinus.push_back(muon_cktpt[j]);
						etaMinus.push_back(muon_eta[j]);
						RelIsoMinus.push_back(_muRelIso);
						dxyVTXMinus.push_back(muon_dxyVTX[j]);
						dzVTXMinus.push_back(muon_dzVTX[j]);
						resoMinus.push_back(muon_cktptError[j] / muon_cktpt[j]);
						trkLayersMinus.push_back(muon_trackerLayers[j]);
						pixelHitsMinus.push_back(muon_pixelHits[j]);
						MuHitsMinus.push_back(muon_muonHits[j]);
						nMatchesMinus.push_back(muon_nMatches[j]);
					}
				}
			}
			// dilepton candidate
			TLorentzVector recoCand;
			double recoMass = 0;
			int iMuPlus = muPlus.size();
			int iMuMinus = muMinus.size();
			
			if( iMuPlus == 1 && iMuMinus == 1 ) {
				recoCand = muPlus[0] + muMinus[0];
			}
			recoMass =  recoCand.M();
			
			if( recoMass > 0 ) {
				if( variable == "diMuonMass") _h1->Fill(recoMass);
				else if( isPlus ){
					if( variable == "cktpt")		_h1->Fill(cktptPlus[0]);
					if( variable == "eta")			_h1->Fill(etaPlus[0]);
					if( variable == "trackerLayers")_h1->Fill(trkLayersPlus[0]);
					if( variable == "pixelHits")	_h1->Fill(pixelHitsPlus[0]);
					if( variable == "RelIso")		_h1->Fill(RelIsoPlus[0]);
					if( variable == "dxyVTX")		_h1->Fill(dxyVTXPlus[0]);
					if( variable == "dzVTX")		_h1->Fill(dzVTXPlus[0]);
					if( variable == "muonHits")		_h1->Fill(MuHitsPlus[0]);
					if( variable == "nMatches")		_h1->Fill(nMatchesPlus[0]);
					if( variable == "cktptReso")	_h1->Fill(RelIsoPlus[0]);
				}
				else if( !isPlus ){
					if( variable == "cktpt")		_h1->Fill(cktPtMinus[0]);
					if( variable == "eta")			_h1->Fill(etaMinus[0]);
					if( variable == "trackerLayers")_h1->Fill(trkLayersMinus[0]);
					if( variable == "pixelHits")	_h1->Fill(pixelHitsMinus[0]);
					if( variable == "RelIso")		_h1->Fill(RelIsoMinus[0]);
					if( variable == "dxyVTX")		_h1->Fill(dxyVTXMinus[0]);
					if( variable == "dzVTX")		_h1->Fill(dzVTXMinus[0]);
					if( variable == "muonHits")		_h1->Fill(MuHitsMinus[0]);
					if( variable == "nMatches")		_h1->Fill(nMatchesMinus[0]);
					if( variable == "cktptReso")	_h1->Fill(RelIsoMinus[0]);
				}
			}
		}
	}
	cout << "result = " << _h1->Integral() << " " << endl;
    return _h1;
}
