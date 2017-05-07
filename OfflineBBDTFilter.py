# ======================================================================
# OfflineBBDTFilter.py
#
# Configuration of a BBDT filter for offline use that parallels that used
# in the Hlt2CharmHadInclDst2PiD02HHXBDT line in Hlt2 TCK 0x2139160f
# ======================================================================
from Configurables import LoKi__Hybrid__DictOfFunctors
from Configurables import BBDecTreeTool
from Configurables import FilterDesktop

# Configure the DictOfFunctors variable handler
varHandler = LoKi__Hybrid__DictOfFunctors('VarHandler', Variables={
    'D0CHI2': 'CHILDFUN(VFASPF(VCHI2), 1)',
    'D0FD': 'CHILDFUN(BPVVD, 1)',
    'DSTDOCA': 'DOCAMAX',
    'DSTFD': 'BPVVD',
    'SLPCOSTHETA': 'LV02',
    'SLPPT': 'CHILDFUN(PT, 2)',
    'TRSUMPT': 'CHILDFUN(CHILDFUN(PT, 1) + CHILDFUN(PT, 2), 1)'
})

# Configure the BBDT selection tool
bdtTool = BBDecTreeTool(
    'DstBBDT',
    Threshold=1.4550000,
    ParamFile="$PARAMFILESROOT/data/Hlt2Dst2PiD02HHX_BDTParams_v2r0.txt",
    ParticleDictTool="LoKi::Hybrid::DictOfFunctors/VarHandler"
)
bdtTool.addTool(varHandler)

# Configure a filter that applies the tool
dstBDTTagger = FilterDesktop(
    "DstBDTTagger",
    Code="FILTER('BBDecTreeTool/DstBBDT')",
    Inputs=["YOUR INPUTS HERE"]
)
dstBDTTagger.addTool(bdtTool)
