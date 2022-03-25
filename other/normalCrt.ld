.segment CARTRIDGE_FILE [outBin="%O"]
    .segmentout [segments ="BANK1"]

.segmentdef BANK1 [segments="Startup, Code, Data", min=$8000, max=$bfff, fill]
.segmentdef Startup [start=$8000]
.segmentdef Code [startAfter="Startup"]
.segmentdef Data [startAfter="Code"]

.segment Startup
.byte <main,>main,$bc,$fe,$c3,$c2,$cd,$38,$30
