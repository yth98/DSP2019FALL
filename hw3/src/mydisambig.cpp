#include <stdio.h>
#include <iostream>
#include <vector>
#include <unordered_map>
#include "Ngram.h"

#define BUFFER_SIZE 4096

using namespace std;

int main(int argc, char* argv[]) {
    char sen[BUFFER_SIZE]; // sentence
    char cbuf[BUFFER_SIZE];
    VocabIndex context[2];
    FILE *inpF, *mapF, *outF;
    string buf;
    unordered_map<string,vector<string> > zbmap;
    vector<string> seq;               // word sequence
    vector<vector<VocabIndex> > iseq; // id sequence
    vector<vector<float> > pseq;      // probability sequence
    vector<vector<unsigned> > bptr;   // backtrack pointer
    Vocab voc;
    Ngram lm(voc, 2); // order=2

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
        //if(zbmap[seq[0]].size() > 1) {cout<<seq[0]<<" "<<zbmap[seq[0]].size()<<endl;}
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

            // recursion
            for(unsigned i=0; i<iseq[idx].size(); ++i) {
                float m_prob = -1E+37;
                unsigned m_ptr = 0;
                switch(idx) {
                    case 0:
                        context[0] = context[1] = Vocab_None;
                        m_prob = lm.wordProb(iseq[idx][i], context);
                        break;
                    default:
                        for(unsigned j=0; j<iseq[idx-1].size(); ++j) {
                            context[0] = iseq[idx-1][j];
                            context[1] = Vocab_None;
                            float prob = pseq[idx-1][j] + lm.wordProb(iseq[idx][i], context);
                            if(prob > m_prob) {
                                m_prob = prob;
                                m_ptr = j;
                            }
                        }
                }
                pseq[idx].push_back(m_prob);
                bptr[idx].push_back(m_ptr);

                //sprintf(cbuf, "%s %.6f %u\n", zbmap[word][i].c_str(), pseq[idx][i], bptr[idx][i]);
                //fputs(cbuf, outF);
            }
            ++idx;
        }

        // find max termination
        float m_prob = -1E+37;
        vector<unsigned> path(seq.size());
        for(unsigned i=0; i<pseq[idx-1].size(); ++i) if(pseq[idx-1][i] > m_prob) {
            m_prob = pseq[idx-1][i];
            path[idx-1] = i;
        }
        // backtrack
        for(int i=idx-2; i>=0; --i)
            path[i] = bptr[i+1][path[i+1]];
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
