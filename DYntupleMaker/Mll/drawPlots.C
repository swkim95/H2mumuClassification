#include "./setTDRStyle.C"

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

void DrawHist(TH1D* hdata, TH1D* hmc, TString higgsName, TString dyName, TString muCharge, TString mode){
	hdata->SetLineColor(kBlue);
	hdata->SetLineWidth(2);
	hmc->SetLineColor(kRed);
	hmc->SetLineWidth(2);
	
	// Normalize to the number of data
	hmc->Scale(hdata->Integral()/hmc->Integral());
	
	TCanvas *c1 = new TCanvas("c1", "c1", 1000, 1000);
	c1->cd();
	c1->SetLogy();
	TPad *c1_1 = new TPad("padc1_1","padc1_1",0.01,0.05,0.99,0.99);
	c1_1->Draw();
	c1_1->cd();
	c1_1->SetTopMargin(0.01);
	c1_1->SetBottomMargin(0.3);
	c1_1->SetRightMargin(0.1);
	c1_1->SetLeftMargin(0.109);
	c1_1->SetFillStyle(0);
	
	if (higgsName == "Higgs2mumu_Run2012_MuPlus_cktpt" || higgsName == "Higgs2mumu_Run2012_MuMinus_cktpt") {
		hmc->SetTitle("CMS Preliminary at #sqrt{s} = 8 TeV, " + muCharge + mode);
		hmc->Draw("hist");
		hmc->SetLabelSize(0.0);
		hmc->GetYaxis()->SetTitleOffset(1.5);
		hmc->GetYaxis()->SetTitle("Events");
		hmc->Draw("histsame");
		hdata->Draw("histsame");
	}
	else {
		hdata->SetTitle("CMS Preliminary at #sqrt{s} = 8 TeV, " + muCharge + mode);
		hdata->Draw("hist");
		hdata->SetLabelSize(0.0);
		hdata->GetYaxis()->SetTitleOffset(1.5);
		hdata->GetYaxis()->SetTitle("Events");
		hmc->Draw("histsame");
		hdata->Draw("histsame");
	}
	
	TLegend *legend = new TLegend(0.80, 0.80, 0.90, 0.90);
	legend->AddEntry(hdata, "Higgs", "l");
	legend->AddEntry(hmc, "DY", "l");
	legend->SetFillColor(0);
	legend->Draw("0");
	
	c1_2 = new TPad("padc1_2","padc1_2",0.01,0.05,0.99,0.32);
	c1_2->Draw();
	c1_2->cd();
	c1_2->SetTopMargin(0.1);
	c1_2->SetBottomMargin(0.30);
	c1_2->SetRightMargin(0.091);
	c1_2->SetFillStyle(0);
	c1_2->SetGrid();
	TH1D* hratio = (TH1D*) hdata->Clone();
	hdata->Sumw2(); hmc->Sumw2();
	hratio->Divide(hdata, hmc);
	hratio->GetXaxis()->SetTitle(mode);
	hratio->SetTitle("");
	hratio->GetXaxis()->SetMoreLogLabels();
	hratio->GetXaxis()->SetNoExponent();
	hratio->GetYaxis()->SetTitle("data/MC");
	hratio->GetXaxis()->SetTitleSize(0.13);
	hratio->GetYaxis()->SetTitleSize(0.09);
	hratio->GetYaxis()->SetTitleOffset(0.4);
	hratio->GetXaxis()->SetLabelSize(0.11);
	hratio->GetYaxis()->SetLabelSize(0.07);
	
	gStyle->SetOptFit(0);
	hratio->SetMaximum(2.0);
	hratio->SetMinimum(0.0);
	hratio->SetMarkerSize(0.5);
	hratio->Draw("e1p");
	hratio->Fit("pol0");
	c1->SaveAs("plots/Histogram_"+ muCharge + mode + ".png");

	c1->Clear();
}

void drawPlots( void ) {
  gStyle->SetOptStat(0);

  // Directory of ROOT file containing histograms
  TFile* f1 = new TFile("rootfiles/variableSpec_mumu_pt30.root");
  f1->cd();

  std::vector<TString> mode;
  makeMode(mode);
  std::cout << mode[0] << std::endl;

  TString higgs = "Higgs2mumu_Run2012_";
  TString dy = "DYJets_";
  TString muPlus = "MuPlus_";
  TString muMinus = "MuMinus_";
  
  TString higgsPlusName = higgs + muPlus;
  TString higgsMinusName = higgs + muMinus;
  TString dyPlusName = dy + muPlus;
  TString dyMinusName = dy + muMinus;
  
  TH1D* hdata;
  TH1D* hmc;
  hdata = (TH1D*) Higgs2mumu_Run2012_diMuonMass;
  hmc = (TH1D*) DYJets_diMuonMass;
  cout << "nevents = (Higgs2mumu): " << hdata->Integral() << ";; (DY2mumu): " << hmc->Integral() << endl;

  //TString HiggsDimuonMass = "Higgs2mumu_Run2012_diMuonMass";
  //TString DYDimuonMass = "DYJets_diMuonMass";
  //TString dimuonCharge = " ";
  //TString dimuonMode = "diMuonMass";

  //DrawHist(hdata, hmc, HiggsDimuonMass, DYDimuonMass, dimuonCharge, dimuonMode);

  DrawHist(hdata, hmc, "Higgs2mumu_Run2012_diMuonMass", "DYJets_diMuonMass", " ", "diMuonMass")

  for(int i = 0; i < mode.size(); ++i) {
	  
	  TH1D* hdata_plus;
	  TH1D* hdata_minus;
	  TH1D* hmc_plus;
	  TH1D* hmc_minus;

	  TString higgsHistNamePlus = higgsPlusName + mode[i]; 
	  TString higgsHistNameMinus = higgsMinusName + mode[i];

	  TString dyHistNamePlus = dyPlusName + mode[i];
	  TString dyHistNameMinus = dyMinusName + mode[i];

	  hdata_plus = (TH1D*) f1->Get(higgsHistNamePlus);
	  hmc_plus = (TH1D*) f1->Get(dyHistNamePlus);
	  cout << "nevents = (data): " << hdata_plus->Integral() << ";; (DY MC): " << hmc_plus->Integral() << endl;
	  DrawHist(hdata_plus, hmc_plus, higgsHistNamePlus, dyHistNamePlus, muPlus, mode[i]);
	  
	  hdata_minus = (TH1D*) f1->Get(higgsHistNameMinus);
	  hmc_minus = (TH1D*) f1->Get(dyHistNameMinus);
	  cout << "nevents = (data): " << hdata_minus->Integral() << ";; (DY MC): " << hmc_minus->Integral() << endl;
	  DrawHist(hdata_minus, hmc_minus, higgsHistNameMinus, dyHistNameMinus, muMinus, mode[i]);

	
  }
  f1->Close();
}
