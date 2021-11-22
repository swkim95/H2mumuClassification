import FWCore.ParameterSet.Config as cms

isMC = True

process = cms.Process("DYSkim")

## MessageLogger
process.load("FWCore.MessageLogger.MessageLogger_cfi")

## Options and Output Report
process.options   = cms.untracked.PSet( 
  wantSummary = cms.untracked.bool(True) 
)
process.MessageLogger.cerr.FwkReport.reportEvery = 1000
SkipEvent = cms.untracked.vstring('ProductNotFound')

## Source
process.source = cms.Source("PoolSource",
    fileNames = cms.untracked.vstring(
    # replace below link by your input DY AODSIM ROOT files
    'file:/scratch/swkim/DY2mumu/1/0002E9AF-1712-E411-A720-0025905A612A.root',
    'file:/scratch/swkim/DY2mumu/1/001D1120-D910-E411-BFFC-002618FDA207.root',
    'file:/scratch/swkim/DY2mumu/1/002FB2D8-3A11-E411-B238-0026189438CC.root',
    'file:/scratch/swkim/DY2mumu/1/003E4503-1312-E411-B184-002618943811.root',
    'file:/scratch/swkim/DY2mumu/1/0040C0E0-2711-E411-BF0D-0025905A6088.root',
    'file:/scratch/swkim/DY2mumu/1/00475FD4-4211-E411-A0AA-00261894396F.root',
    'file:/scratch/swkim/DY2mumu/1/004C1AAB-2E11-E411-97E0-0025905A48D0.root',
    'file:/scratch/swkim/DY2mumu/1/004C462C-B910-E411-B7F9-002618943981.root',
    'file:/scratch/swkim/DY2mumu/1/00541174-3E11-E411-B3E7-003048D15E36.root',
    'file:/scratch/swkim/DY2mumu/1/0059EB07-BD10-E411-89BE-002354EF3BDB.root',
    'file:/scratch/swkim/DY2mumu/1/005FEDBE-0211-E411-8522-0025905A6076.root',
    'file:/scratch/swkim/DY2mumu/1/00623FA5-FE10-E411-9DDC-003048FFD76E.root',
    'file:/scratch/swkim/DY2mumu/1/007C14F6-C810-E411-B44B-00259059642A.root',
    'file:/scratch/swkim/DY2mumu/1/008C6A2A-F210-E411-8F96-0025905A6090.root',
    'file:/scratch/swkim/DY2mumu/1/00935DF4-EC10-E411-A415-0025905938AA.root',
    'file:/scratch/swkim/DY2mumu/1/0095FB21-CF10-E411-AF4A-002354EF3BDD.root',
    'file:/scratch/swkim/DY2mumu/1/00A43CEA-1011-E411-9B87-0025905964C0.root',
    'file:/scratch/swkim/DY2mumu/1/00AE23A8-C511-E411-9FBB-003048FFCB8C.root',
    'file:/scratch/swkim/DY2mumu/1/00B16583-0A11-E411-A84C-003048FFCC2C.root',
    'file:/scratch/swkim/DY2mumu/1/00BEE1B3-BE11-E411-A021-002590596468.root',
    'file:/scratch/swkim/DY2mumu/1/00C5082C-1411-E411-9FF2-0025905A6136.root',
    'file:/scratch/swkim/DY2mumu/1/00D0C1BB-D410-E411-AA36-0025905A48BC.root',
    'file:/scratch/swkim/DY2mumu/1/00D123AC-3A11-E411-A3D8-002618943973.root',
    'file:/scratch/swkim/DY2mumu/1/00D2080E-1111-E411-9133-0026189438F2.root',
    'file:/scratch/swkim/DY2mumu/1/00D584BE-D710-E411-9AAA-002618943901.root',
    'file:/scratch/swkim/DY2mumu/1/00E4898F-B511-E411-8843-0026189438EF.root',
    'file:/scratch/swkim/DY2mumu/1/00EC5745-3411-E411-AF02-0025905A4964.root',
    'file:/scratch/swkim/DY2mumu/1/00ECFB1A-3311-E411-8565-002618943821.root',
    'file:/scratch/swkim/DY2mumu/1/00F1DF05-CD11-E411-A7E6-003048FFD770.root',
    'file:/scratch/swkim/DY2mumu/1/00F645AB-D110-E411-A6F5-00261894387A.root',
    'file:/scratch/swkim/DY2mumu/1/02030F84-D911-E411-9072-0026189438D5.root',
    'file:/scratch/swkim/DY2mumu/1/022CAE84-B210-E411-AAC7-0025905A48E4.root',
    'file:/scratch/swkim/DY2mumu/1/0231E1CF-1011-E411-9F4C-0026189438ED.root',
    'file:/scratch/swkim/DY2mumu/1/02425B1B-A910-E411-AD4F-0025905A60C6.root',
    'file:/scratch/swkim/DY2mumu/1/0262602E-0711-E411-983C-0025905A60F2.root',
    'file:/scratch/swkim/DY2mumu/1/0263F460-C111-E411-BE22-002590593876.root',
    'file:/scratch/swkim/DY2mumu/1/026BF53E-3511-E411-B1DD-003048FF9AC6.root',
    'file:/scratch/swkim/DY2mumu/1/026D643F-1411-E411-B7E8-002618943959.root',
    'file:/scratch/swkim/DY2mumu/1/028920AE-7310-E411-BD05-00261894393D.root',
    'file:/scratch/swkim/DY2mumu/1/02A48F35-3911-E411-B33C-0025905A6110.root',
    'file:/scratch/swkim/DY2mumu/1/02AEF812-1111-E411-A204-0025905A6092.root',
    'file:/scratch/swkim/DY2mumu/1/02CF1CD6-D610-E411-AEA0-002618943925.root',
    'file:/scratch/swkim/DY2mumu/1/02E4DD67-9C10-E411-B731-003048FFCB96.root',
    'file:/scratch/swkim/DY2mumu/1/02ED2EBF-C710-E411-A08F-0025905A6132.root',
    'file:/scratch/swkim/DY2mumu/1/02F72FFD-B510-E411-A61F-002618943829.root',
    'file:/scratch/swkim/DY2mumu/1/04033435-E311-E411-8673-002618943985.root',
    'file:/scratch/swkim/DY2mumu/1/04229AFF-B210-E411-BA99-0025905A60B2.root',
    'file:/scratch/swkim/DY2mumu/1/0422AEFD-C211-E411-A06E-00259059642A.root',
    'file:/scratch/swkim/DY2mumu/1/0425C5E8-0E11-E411-B665-0025905A6090.root',
    'file:/scratch/swkim/DY2mumu/1/04363183-2E11-E411-AC02-003048FFCB8C.root',
    )
)

process.maxEvents = cms.untracked.PSet( input = cms.untracked.int32(-1) )

## Geometry and Detector Conditions (needed for a few patTuple production steps)
process.load("Configuration.Geometry.GeometryIdeal_cff")
process.load("Configuration.StandardSequences.FrontierConditions_GlobalTag_cff")
process.GlobalTag.globaltag = cms.string('START53_V27::All')

process.load("Configuration.StandardSequences.MagneticField_cff")

## Output Module Configuration (expects a path 'p')
from PhysicsTools.PatAlgos.patEventContent_cff import patEventContent
process.out = cms.OutputModule("PoolOutputModule",
    fileName = cms.untracked.string('patTuple_skim.root'),
    splitLevel = cms.untracked.int32(0),
    # save only events passing the full path
    #SelectEvents   = cms.untracked.PSet( SelectEvents = cms.vstring('p') ),
    # save PAT Layer 1 output; you need a '*' to
    # unpack the list of commands 'patEventContent'
    outputCommands = cms.untracked.vstring('drop *')
)

import HLTrigger.HLTfilters.hltHighLevel_cfi
process.dimuonsHLTFilter = HLTrigger.HLTfilters.hltHighLevel_cfi.hltHighLevel.clone()

process.dimuonsHLTFilter.TriggerResultsTag = cms.InputTag("TriggerResults","","HLT")
process.dimuonsHLTFilter.HLTPaths = ["HLT_Mu*","HLT_DoubleMu*","HLT_IsoMu*"]

process.TFileService = cms.Service("TFileService",
  fileName = cms.string('ntuple_skim_mc_DY_50files.root')
)

process.goodOfflinePrimaryVertices = cms.EDFilter("VertexSelector",
   src = cms.InputTag("offlinePrimaryVertices"),
   cut = cms.string("!isFake && ndof > 4 && abs(z) < 24 && position.Rho < 2"), # tracksSize() > 3 for the older cut
   filter = cms.bool(True),   # otherwise it won't filter the events, just produce an empty vertex collection.
)

process.noscraping = cms.EDFilter("FilterOutScraping",
   applyfilter = cms.untracked.bool(True),
   debugOn = cms.untracked.bool(False),
   numtrack = cms.untracked.uint32(10),
   thresh = cms.untracked.double(0.25)
)

process.FastFilters = cms.Sequence( process.goodOfflinePrimaryVertices + process.noscraping )

from H2mumuClassification.DYntupleMaker.DYntupleMaker_cfi import *
from H2mumuClassification.DYntupleMaker.DYntupleMaker_cfi import *

process.recoTree = DYntupleMaker.clone()
process.recoTree.isMC = isMC
process.recoTree.Muon = "patMuons"

# load the PAT config
process.load("PhysicsTools.PatAlgos.producersLayer1.patCandidates_cff")

from PhysicsTools.PatAlgos.patEventContent_cff import *
process.out.outputCommands += patTriggerEventContent
process.out.outputCommands += patExtraAodEventContent
process.out.outputCommands += patEventContentNoCleaning
process.out.outputCommands.extend(cms.untracked.vstring(
  'keep *_*_*_*',
))

# Let it run
process.p = cms.Path(
  process.FastFilters *
  process.patCandidates *
  # process.patDefaultSequence
  process.recoTree
)
process.p.remove(process.makePatPhotons)
process.p.remove(process.makePatJets)
process.p.remove(process.makePatTaus)
process.p.remove(process.makePatMETs)
process.p.remove(process.patCandidateSummary)

#not for MC	
#process.p.remove(process.electronMatch)
#process.p.remove(process.muonMatch)
