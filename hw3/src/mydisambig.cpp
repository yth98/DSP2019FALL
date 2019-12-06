#include <stdio.h>
//#include <iostream>
#include <set>
#include <unordered_map>
#include <vector>
#include <utility>
#include <algorithm>
#include "Ngram.h"

#define BUFFER_SIZE 4096
#define BEAM_LIMIT 786432

typedef pair<unsigned,float> ppair;

using namespace std;

bool cmp1(ppair &a, ppair &b) {return a.first < b.first;}
bool cmp2(ppair &a, ppair &b) {return a.second > b.second;}

int main(int argc, char* argv[]) {
    char sen[BUFFER_SIZE]; // sentence
    char cbuf[BUFFER_SIZE];
    VocabIndex context[3];
    FILE *inpF, *mapF, *outF;
    string buf;
    unordered_map<string,vector<string> > zbmap;
    vector<string> seq;               // word sequence
    vector<vector<VocabIndex> > iseq; // id sequence
    vector<vector<ppair> > pseq;      // probability sequence (2D array)
    vector<vector<unsigned> > bptr;   // backtrack pointer (2D array)
    Vocab voc;
    Ngram lm(voc, 3); // order=3

    // read files
    if(argc < 5) return -1;
    inpF = fopen(argv[1], "r");
    mapF = fopen(argv[2], "r");
    outF = fopen(argv[4], "w");
    if(inpF==NULL || mapF==NULL || outF==NULL) {cout<<"No such file."; return 0;}
    {
        File lmFile(argv[3], "r");
        lm.read(lmFile);
        lmFile.close();
    }

    // read in ZhuYin-Big5 mapping
    while(!feof(mapF)) {
        if(fgets(sen, BUFFER_SIZE, mapF)==NULL) continue;
        // split sentence to word sequence
        // https://blog.csdn.net/Mary19920410/article/details/77372828
        char *p = strtok(sen, " ");
        while(p != NULL) {
            buf = p;
            seq.push_back(buf);
            p = strtok(NULL, " \n");
        }
        // save to hash map
        zbmap[seq[0]] = vector<string>(seq.begin()+1, seq.end());
        seq.clear();
    }

    // for each sentence
    while(!feof(inpF)) {
        if(fgets(sen, BUFFER_SIZE, inpF)==NULL) continue;

        // split sentence to word sequence
        char *p = strtok(sen, " ");
        while(p != NULL) {
            buf = p;
            seq.push_back(buf);
            p = strtok(NULL, " \n");
        }
        iseq.resize(seq.size());
        pseq.resize(seq.size());
        bptr.resize(seq.size());

        // for each word
        short idx = 0;
        for(auto &word : seq) {
            // extract state list
            if(!zbmap.count(word))
                iseq[idx].push_back(voc.getIndex(Vocab_Unknown));
            else for(unsigned int i=0; i<zbmap[word].size(); ++i) {
                iseq[idx].push_back(voc.getIndex(zbmap[word][i].c_str()));
                if(iseq[idx][i] == Vocab_None) iseq[idx][i] = voc.getIndex(Vocab_Unknown);
            }

            // beam set
            int quan = (idx>=2) ? iseq[idx].size()*iseq[idx-1].size()*iseq[idx-2].size() : 0;
            set<unsigned> blist;
            if(idx>=2 && quan > BEAM_LIMIT) {
                sort(pseq[idx-1].begin(), pseq[idx-1].end(), cmp2);
                for(unsigned l=0; l<(BEAM_LIMIT/6/iseq[idx].size()); ++l)
                    blist.insert(pseq[idx-1][l].first);
                sort(pseq[idx-1].begin(), pseq[idx-1].end(), cmp1);
            }

            // recursion
            //int ccnt = 0;
            for(unsigned i=0; i<iseq[idx].size(); ++i) {
                switch(idx) {
                    case 0:
                        context[0] = context[1] = context[2] = Vocab_None;
                        pseq[idx].push_back(make_pair(i, lm.wordProb(iseq[idx][i], context)));
                        break;
                    case 1:
                        for(unsigned j=0; j<iseq[idx-1].size(); ++j) {
                            context[0] = iseq[idx-1][j];
                            context[1] = context[2] = Vocab_None;
                            pseq[idx].push_back(make_pair(i*iseq[idx-1].size()+j, pseq[idx-1][j].second + lm.wordProb(iseq[idx][i], context)));
                        }
                        break;
                    default:
                        for(unsigned j=0; j<iseq[idx-1].size(); ++j) {
                            float m_prob = -1E+37;
                            unsigned m_ptr = 0;
                            for(unsigned k=0; k<iseq[idx-2].size(); ++k) {
                                unsigned jk = j*iseq[idx-2].size()+k;
                                // pruning
                                float prob = pseq[idx-1][jk].second;
                                if(prob < -1E+3*idx) continue;
                                if(quan > BEAM_LIMIT && !blist.count(j*iseq[idx-2].size()+k)) continue;
                                // conditional probability
                                context[0] = iseq[idx-1][j];
                                context[1] = iseq[idx-2][k];
                                context[2] = Vocab_None;
                                prob += lm.wordProb(iseq[idx][i], context);
                                if(prob > m_prob) {
                                    m_prob = prob;
                                    m_ptr = k;
                                }
                                //++ccnt;
                            }
                            pseq[idx].push_back(make_pair(i*iseq[idx-1].size()+j, m_prob));
                            bptr[idx].push_back(m_ptr);
                        }
                }
            }
            //if(idx>=2&&quan>BEAM_LIMIT) cout<<idx<<" "<<ccnt<<" "<<quan<<endl;
            ++idx;
        }

        // find max termination
        float m_prob = -1E+37;
        vector<unsigned> path(seq.size());
        for(unsigned ij=0; ij<pseq[idx-1].size(); ++ij) if(pseq[idx-1][ij].second > m_prob) {
            m_prob = pseq[idx-1][ij].second;
            path[idx-1] = ij / iseq[idx-2].size();
            path[idx-2] = ij % iseq[idx-2].size();
        }
        // backtrack
        for(int i=idx-3; i>=0; --i)
            path[i] = bptr[i+2][path[i+2]*iseq[i+1].size()+path[i+1]];
        // dump string path
        sprintf(cbuf, "<s>");
        for(int i=0; i<idx; ++i) {
            sprintf(cbuf, "%s %s", cbuf, zbmap.count(seq[i]) ? zbmap[seq[i]][path[i]].c_str() : seq[i].c_str());
        }
        sprintf(cbuf, "%s </s>\n", cbuf);
        fputs(cbuf, outF);

        seq.clear();
        iseq.clear();
        pseq.clear();
        bptr.clear();
    }

    fclose(inpF), fclose(mapF), fclose(outF);
    return 0;
}
