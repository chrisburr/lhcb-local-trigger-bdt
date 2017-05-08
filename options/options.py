import DecayTreeTuple.Configuration  # Needed to set up addBranches

from Configurables import BBDecTreeTool
from Configurables import DaVinci
from Configurables import DecayTreeTuple
from Configurables import FilterDesktop
from Configurables import LoKi__Hybrid__DictOfFunctors
from GaudiConf import IOHelper
from PhysSelPython.Wrappers import DataOnDemand
from PhysSelPython.Wrappers import Selection
from PhysSelPython.Wrappers import SelectionSequence
# from Gaudi.Configuration import DEBUG


# Configuration
decay_descriptor = (
    '[(D*(2010)+ -> ^(D0 -> ^(phi(1020) -> ^K+ ^K-) ^gamma) ^pi+),'
    ' (D*(2010)- -> ^(D0 -> ^(phi(1020) -> ^K+ ^K-) ^gamma) ^pi-)]'
)
_decay_descriptor = decay_descriptor.replace('^', '')
decay_branches = {
    'Dstar': _decay_descriptor.replace('(D*', '^(D*'),
    'D0': _decay_descriptor.replace('(D0', '^(D0'),
    'phi': _decay_descriptor.replace('(phi', '^(phi'),
    'Kminus': _decay_descriptor.replace('K-', '^K-'),
    'Kplus': _decay_descriptor.replace('K+', '^K+'),
    'pisoft': _decay_descriptor.replace('pi', '^pi'),
}
bdt_variables = {
    'D0CHI2': 'CHILDFUN(VFASPF(VCHI2), 1)',
    'D0FD': 'CHILDFUN(BPVVD, 1)',
    'DSTDOCA': 'DOCAMAX',
    'DSTFD': 'BPVVD',
    'SLPCOSTHETA': 'LV02',
    'SLPPT': 'CHILDFUN(PT, 2)',
    'TRSUMPT': 'CHILDFUN(CHILDFUN(PT, 1) + CHILDFUN(PT, 2), 1)'
}
# /MC/2012/Beam4000GeV-2012-MagDown-Nu2.5-Pythia8/Sim09b/Trig0x409f0045/Reco14c/Stripping21Filtered/27163203/PROMPTD2PHIGAMMA.STRIPTRIG.DST
input_data = DataOnDemand("/Event/PromptD2PhiGamma.StripTrig/Phys/MyLoosePromptD2PhiGammaLine/Particles")
dddb_tag = "dddb-20150928"
conddb_tag = "sim-20160321-2-vc-md100"

# Configure the DictOfFunctors variable handler
varHandler = LoKi__Hybrid__DictOfFunctors('VarHandler', Variables=bdt_variables)

# Configure the BBDT selection tool
bdtTool = BBDecTreeTool(
    'DstBBDT',
    Threshold=0,  # 1.4550000,
    ParamFile="$PARAMFILESROOT/data/Hlt2Dst2PiD02HHX_BDTParams_v2r0.txt",
    ParticleDictTool="LoKi::Hybrid::DictOfFunctors/VarHandler"
)
bdtTool.addTool(varHandler)

# Configure a filter that applies the tool
dstBDTTagger = FilterDesktop(
    "DstBDTTagger",
    Code="FILTER('BBDecTreeTool/DstBBDT')"
)
# bdtTool.OutputLevel = DEBUG
dstBDTTagger.addTool(bdtTool)

bdt_sel = Selection(
    'bdt_sel',
    Algorithm=dstBDTTagger,
    RequiredSelections=[input_data]
)
bdt_seq = SelectionSequence('bdt_seq', TopSelection=bdt_sel)
DaVinci().UserAlgorithms += [bdt_seq.sequence()]

# Make the DecayTreeTuple using the BDT to Filter
dtt = DecayTreeTuple('DstToD0pi_D0Tophigamma_phiToKK')
dtt.Inputs = [bdt_seq.outputLocation()]
dtt.Decay = decay_descriptor
dtt.addBranches(decay_branches)
dstar_hybrid = dtt.Dstar.addTupleTool('LoKi::Hybrid::TupleTool/LoKi_Dstar')
dstar_hybrid.Variables = {'LoKi_'+k: v for k, v in bdt_variables.items()}
DaVinci().UserAlgorithms += [dtt]

# Setup DaVinci
DaVinci().PrintFreq = 100
DaVinci().EvtMax = -1
DaVinci().SkipEvents = 0
DaVinci().DataType = "2012"
DaVinci().InputType = "DST"
DaVinci().Simulation = True
DaVinci().DDDBtag = dddb_tag
DaVinci().CondDBtag = conddb_tag
DaVinci().Lumi = not DaVinci().Simulation
DaVinci().TupleFile = "DVntuple.root"

# Use the local input data
IOHelper().inputFiles([
    'root://bw27-9.grid.sara.nl:1094/pnfs/grid.sara.nl/data/lhcb/MC/2012/PROMPTD2PHIGAMMA.STRIPTRIG.DST/00056190/0000/00056190_00000001_1.promptd2phigamma.striptrig.dst',
    'root://f01-080-123-e.gridka.de:1094/pnfs/gridka.de/lhcb/MC/2012/PROMPTD2PHIGAMMA.STRIPTRIG.DST/00056190/0000/00056190_00000002_1.promptd2phigamma.striptrig.dst',
    'root://f01-080-123-e.gridka.de:1094/pnfs/gridka.de/lhcb/MC/2012/PROMPTD2PHIGAMMA.STRIPTRIG.DST/00056190/0000/00056190_00000003_1.promptd2phigamma.striptrig.dst',
    'root://eoslhcb.cern.ch//eos/lhcb/grid/prod/lhcb/MC/2012/PROMPTD2PHIGAMMA.STRIPTRIG.DST/00056190/0000/00056190_00000004_1.promptd2phigamma.striptrig.dst',
    'root://door01.pic.es:1094/pnfs/pic.es/data/lhcb/MC/2012/PROMPTD2PHIGAMMA.STRIPTRIG.DST/00056190/0000/00056190_00000005_1.promptd2phigamma.striptrig.dst',
    'root://gfe02.grid.hep.ph.ic.ac.uk:1094/pnfs/hep.ph.ic.ac.uk/data/lhcb/MC/2012/PROMPTD2PHIGAMMA.STRIPTRIG.DST/00056190/0000/00056190_00000006_1.promptd2phigamma.striptrig.dst',
], clear=True)
