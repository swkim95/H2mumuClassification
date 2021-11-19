#include <TChain.h>
void chain_Higgs2mumu_Run2012( TChain* chain )
{
      // Replace below file with your ntuple file
      chain->Add("/scratch/swkim/ntuples_DataOn/ntuple_skim_mc_Higgs.root");
}
