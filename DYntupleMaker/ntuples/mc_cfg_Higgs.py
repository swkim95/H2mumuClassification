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
    # replace below link by your input Higgs AODSIM ROOT files
    'file:/scratch/swkim/H2mumu/2EAD2BB4-EEE4-E211-A4CA-20CF300E9EAD.root',
    'file:/scratch/swkim/H2mumu/6AC7B644-EBE4-E211-9D57-485B3977172E.root',
    'file:/scratch/swkim/H2mumu/6EB43B8D-F0E4-E211-995B-485B39800B83.root',
    'file:/scratch/swkim/H2mumu/726B4351-7DE5-E211-B1AB-001EC9D80A9D.root',
    'file:/scratch/swkim/H2mumu/807FB25B-7DE5-E211-9C3F-001E4F3F355E.root',
    'file:/scratch/swkim/H2mumu/8ED88131-F4E4-E211-960D-20CF305B0509.root',
    'file:/scratch/swkim/H2mumu/945BD160-7DE5-E211-B214-90E6BA19A231.root',
    'file:/scratch/swkim/H2mumu/AE8A190B-FCE4-E211-B859-90E6BA19A215.root',
    'file:/scratch/swkim/H2mumu/B05A10F3-D7E4-E211-AA24-00259074AE8A.root',
    'file:/scratch/swkim/H2mumu/C81F57D0-F4E4-E211-9B00-00259073E30E.root',
    'file:/scratch/swkim/H2mumu/C895B04A-7DE5-E211-AE67-002590747E24.root',
    'file:/scratch/swkim/H2mumu/E88B3A69-F1E4-E211-B2F3-485B39800BB3.root',
    'file:/scratch/swkim/H2mumu/EA1E6E64-7DE5-E211-83A2-001EC9D8B54A.root',
    'file:/scratch/swkim/H2mumu/F211C105-F3E4-E211-BF00-20CF3027A61B.root',
    'file:/scratch/swkim/H2mumu/F6B993C3-F7E4-E211-9FFE-20CF300E9ECF.root',
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
    # save PAT Layer 1 output; you need a '*' to
    # unpack the list of commands 'patEventContent'
    outputCommands = cms.untracked.vstring('drop *')
)

import HLTrigger.HLTfilters.hltHighLevel_cfi
process.dimuonsHLTFilter = HLTrigger.HLTfilters.hltHighLevel_cfi.hltHighLevel.clone()

process.dimuonsHLTFilter.TriggerResultsTag = cms.InputTag("TriggerResults","","HLT")
process.dimuonsHLTFilter.HLTPaths = ["HLT_Mu*","HLT_DoubleMu*","HLT_IsoMu*"]

process.TFileService = cms.Service("TFileService",
  fileName = cms.string('ntuple_skim_Higgs.root')
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

from Phys.DYntupleMaker.DYntupleMaker_cfi import *
from Phys.DYntupleMaker.DYntupleMaker_cfi import *

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
  process.recoTree
)
process.p.remove(process.makePatPhotons)
process.p.remove(process.makePatJets)
process.p.remove(process.makePatTaus)
process.p.remove(process.makePatMETs)
process.p.remove(process.patCandidateSummary)
