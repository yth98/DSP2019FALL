#!/bin/bash

make
<<BI
./mydisambig tmp/T_seg_example.txt ZhuYin-Big5.map U_bigram.lm ans/B_example.txt
./mydisambig tmp/T_seg_1.txt ZhuYin-Big5.map U_bigram.lm ans/B_1.txt
./mydisambig tmp/T_seg_2.txt ZhuYin-Big5.map U_bigram.lm ans/B_2.txt
./mydisambig tmp/T_seg_3.txt ZhuYin-Big5.map U_bigram.lm ans/B_3.txt
./mydisambig tmp/T_seg_4.txt ZhuYin-Big5.map U_bigram.lm ans/B_4.txt
./mydisambig tmp/T_seg_5.txt ZhuYin-Big5.map U_bigram.lm ans/B_5.txt
./mydisambig tmp/T_seg_6.txt ZhuYin-Big5.map U_bigram.lm ans/B_6.txt
./mydisambig tmp/T_seg_7.txt ZhuYin-Big5.map U_bigram.lm ans/B_7.txt
./mydisambig tmp/T_seg_8.txt ZhuYin-Big5.map U_bigram.lm ans/B_8.txt
./mydisambig tmp/T_seg_9.txt ZhuYin-Big5.map U_bigram.lm ans/B_9.txt
./mydisambig tmp/T_seg_10.txt ZhuYin-Big5.map U_bigram.lm ans/B_10.txt
BI
time ./mydisambig tmp/T_seg_example.txt ZhuYin-Big5.map U_trigram.lm ans3/D_example.txt
time ./mydisambig tmp/T_seg_1.txt ZhuYin-Big5.map U_trigram.lm ans3/D_1.txt
time ./mydisambig tmp/T_seg_2.txt ZhuYin-Big5.map U_trigram.lm ans3/D_2.txt
time ./mydisambig tmp/T_seg_3.txt ZhuYin-Big5.map U_trigram.lm ans3/D_3.txt
time ./mydisambig tmp/T_seg_4.txt ZhuYin-Big5.map U_trigram.lm ans3/D_4.txt
time ./mydisambig tmp/T_seg_5.txt ZhuYin-Big5.map U_trigram.lm ans3/D_5.txt
time ./mydisambig tmp/T_seg_6.txt ZhuYin-Big5.map U_trigram.lm ans3/D_6.txt
time ./mydisambig tmp/T_seg_7.txt ZhuYin-Big5.map U_trigram.lm ans3/D_7.txt
time ./mydisambig tmp/T_seg_8.txt ZhuYin-Big5.map U_trigram.lm ans3/D_8.txt
time ./mydisambig tmp/T_seg_9.txt ZhuYin-Big5.map U_trigram.lm ans3/D_9.txt
time ./mydisambig tmp/T_seg_10.txt ZhuYin-Big5.map U_trigram.lm ans3/D_10.txt
