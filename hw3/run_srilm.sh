#!/bin/bash

source setup.sh
<<LM
perl separator_big5.pl corpus.txt > T_seg_corpus.txt

ngram-count -text T_seg_corpus.txt -write T_bi.cnt -order 2
ngram-count -read T_bi.cnt -lm U_bigram.lm -order 2 -unk

ngram-count -text T_seg_corpus.txt -write T_tri.cnt -order 3
ngram-count -read T_tri.cnt -lm U_trigram.lm -order 3 -unk

rm T_*
LM
make map

mkdir tmp ans ans3 -p
<<SEG
perl separator_big5.pl test_data/example.txt > ./tmp/T_seg_example.txt
perl separator_big5.pl test_data/1.txt > ./tmp/T_seg_1.txt
perl separator_big5.pl test_data/2.txt > ./tmp/T_seg_2.txt
perl separator_big5.pl test_data/3.txt > ./tmp/T_seg_3.txt
perl separator_big5.pl test_data/4.txt > ./tmp/T_seg_4.txt
perl separator_big5.pl test_data/5.txt > ./tmp/T_seg_5.txt
perl separator_big5.pl test_data/6.txt > ./tmp/T_seg_6.txt
perl separator_big5.pl test_data/7.txt > ./tmp/T_seg_7.txt
perl separator_big5.pl test_data/8.txt > ./tmp/T_seg_8.txt
perl separator_big5.pl test_data/9.txt > ./tmp/T_seg_9.txt
perl separator_big5.pl test_data/10.txt > ./tmp/T_seg_10.txt
SEG
<<BI
disambig -text tmp/T_seg_example.txt -map ZhuYin-Big5.map -lm U_bigram.lm -order 2 -keep-unk > ans/A_example.txt
disambig -text tmp/T_seg_1.txt -map ZhuYin-Big5.map -lm U_bigram.lm -order 2 -keep-unk > ans/A_1.txt
disambig -text tmp/T_seg_2.txt -map ZhuYin-Big5.map -lm U_bigram.lm -order 2 -keep-unk > ans/A_2.txt
disambig -text tmp/T_seg_3.txt -map ZhuYin-Big5.map -lm U_bigram.lm -order 2 -keep-unk > ans/A_3.txt
disambig -text tmp/T_seg_4.txt -map ZhuYin-Big5.map -lm U_bigram.lm -order 2 -keep-unk > ans/A_4.txt
disambig -text tmp/T_seg_5.txt -map ZhuYin-Big5.map -lm U_bigram.lm -order 2 -keep-unk > ans/A_5.txt
disambig -text tmp/T_seg_6.txt -map ZhuYin-Big5.map -lm U_bigram.lm -order 2 -keep-unk > ans/A_6.txt
disambig -text tmp/T_seg_7.txt -map ZhuYin-Big5.map -lm U_bigram.lm -order 2 -keep-unk > ans/A_7.txt
disambig -text tmp/T_seg_8.txt -map ZhuYin-Big5.map -lm U_bigram.lm -order 2 -keep-unk > ans/A_8.txt
disambig -text tmp/T_seg_9.txt -map ZhuYin-Big5.map -lm U_bigram.lm -order 2 -keep-unk > ans/A_9.txt
disambig -text tmp/T_seg_10.txt -map ZhuYin-Big5.map -lm U_bigram.lm -order 2 -keep-unk > ans/A_10.txt
BI

time disambig -text tmp/T_seg_example.txt -map ZhuYin-Big5.map -lm U_trigram.lm -order 3 -keep-unk > ans3/C_example.txt
time disambig -text tmp/T_seg_1.txt -map ZhuYin-Big5.map -lm U_trigram.lm -order 3 -keep-unk > ans3/C_1.txt
time disambig -text tmp/T_seg_2.txt -map ZhuYin-Big5.map -lm U_trigram.lm -order 3 -keep-unk > ans3/C_2.txt
time disambig -text tmp/T_seg_3.txt -map ZhuYin-Big5.map -lm U_trigram.lm -order 3 -keep-unk > ans3/C_3.txt
time disambig -text tmp/T_seg_4.txt -map ZhuYin-Big5.map -lm U_trigram.lm -order 3 -keep-unk > ans3/C_4.txt
time disambig -text tmp/T_seg_5.txt -map ZhuYin-Big5.map -lm U_trigram.lm -order 3 -keep-unk > ans3/C_5.txt
time disambig -text tmp/T_seg_6.txt -map ZhuYin-Big5.map -lm U_trigram.lm -order 3 -keep-unk > ans3/C_6.txt
time disambig -text tmp/T_seg_7.txt -map ZhuYin-Big5.map -lm U_trigram.lm -order 3 -keep-unk > ans3/C_7.txt
time disambig -text tmp/T_seg_8.txt -map ZhuYin-Big5.map -lm U_trigram.lm -order 3 -keep-unk > ans3/C_8.txt
time disambig -text tmp/T_seg_9.txt -map ZhuYin-Big5.map -lm U_trigram.lm -order 3 -keep-unk > ans3/C_9.txt
time disambig -text tmp/T_seg_10.txt -map ZhuYin-Big5.map -lm U_trigram.lm -order 3 -keep-unk > ans3/C_10.txt
