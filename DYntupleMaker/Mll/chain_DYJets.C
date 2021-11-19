#include <TChain.h>
void chain_DYJets( TChain* chain )
{
      // Replace below file with your ntuple file
      chain->Add("/scratch/swkim/ntuples_DataOn/ntuple_skim_mc_DY.root");
}
