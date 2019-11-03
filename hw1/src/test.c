#include "hmm.h"
#include <math.h>

int main(int argc, char* argv[])
{
	if(argc < 4) return 0;

	HMM hmms[5];
	load_models( argv[1], hmms, 5);

	// Testing
	FILE *seq = open_or_die( argv[2], "r" );
	FILE *res = open_or_die( argv[3], "w" );
	FILE *lbl = open_or_die( "data/test_lbl.txt", "r" );
	char seql[MAX_SEQ]; char* p;
	p = fgets( seql, MAX_SEQ, seq );
	char lbll[MAX_SEQ];
	p = fgets( lbll, MAX_SEQ, lbl );
	
	int acc = 0, tot = 0;
	do {
		int i, j, t, T = strlen(seql) - 1;
		int res_m = -1; double res_p = -1;
		for( int m = 0 ; m < 5; ++m ) {
			double delta[MAX_SEQ][MAX_STATE] = {0};
			for( i = 0 ; i < hmms[m].state_num ; ++i )
				delta[0][i] = hmms[m].initial[i] * hmms[m].observation[seql[0]-'A'][i];
			for( t = 1 ; t < T ; ++t )
				for( j = 0 ; j < hmms[m].state_num ; ++j ) {
					double da_max = 0;
					for( i = 0 ; i < hmms[m].state_num ; ++i )
						if( da_max < delta[t-1][i] * hmms[m].transition[i][j] )
							da_max = delta[t-1][i] * hmms[m].transition[i][j];
					delta[t][j] = da_max * hmms[m].observation[seql[t]-'A'][j];
				}
			double p_max = 0;
			for( i = 0 ; i < hmms[m].state_num ; ++i )
				if( p_max < delta[T-1][i] )
					p_max = delta[T-1][i];
			if( res_p < p_max ) {
				res_p = p_max;
				res_m = m;
			}
		}
		fprintf( res, "%s %e\n", hmms[res_m].model_name, res_p );
		*strchr(lbll, '\n') = '\0';
		// Calculate accuracy
		if( strcmp(hmms[res_m].model_name, lbll) == 0 ) ++acc;
		++tot;
	} while (p = fgets( lbll, MAX_SEQ, lbl ), p = fgets( seql, MAX_SEQ, seq ), !feof(seq));
	fclose(seq);
	fclose(res);
	printf("Accuracy: %.1lf%% (%d/%d)\n", 100.0*acc/tot, acc, tot);

	return 0;
}
