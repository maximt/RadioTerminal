#!/bin/bash

echo -e "#I1=12.3A U1=27.9V FWD=030% FWD=0123W REF=000W EFF=33.7% TEMP=16.5C\r">/dev/tnt0
echo -e "#FREQ=098.00FM MODLEV=057% RLEV=081% LLEV=058%\r">/dev/tnt0
echo -e "#F1=OK\r">/dev/tnt0

echo -e "#FWD PWR MAX\r">/dev/tnt0
echo -e "#REF PWR MAX\r">/dev/tnt0
echo -e "#IPA1 MAX\r">/dev/tnt0
echo -e "#VPA1 MAX\r">/dev/tnt0
echo -e "#VPA1 MIN\r">/dev/tnt0
echo -e "#OVER TEMP L1\r">/dev/tnt0
echo -e "#OVER TEMP L2\r">/dev/tnt0
echo -e "#TEMP IS NORMAL\r">/dev/tnt0
echo -e "#NO FUSE\r">/dev/tnt0
echo -e "#FWD PWR MAX\r\n#REF PWR MAX\r">/dev/tnt0
