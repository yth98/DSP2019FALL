#include "hmm.h"
#include <math.h>

int main(int argc, char* argv[])
{
	if(argc < 5) return 0;
	// model_init
	HMM hmm;
	loadHMM( &hmm, argv[2] );

	// training
	// N <-> MAX_STATE <-> .state_num ; T <-> MAX_SEQ
	int i, j, k, t; char* p;
	for( int iter = 0 ; iter < atoi(argv[1]) ; ++iter ) {
		double gamma_1[MAX_STATE] = {0};
		double gamma_acc[MAX_STATE] = {0};
		double gamma_acc_ob[MAX_OBSERV][MAX_STATE] = {0};
		double epsilon_acc[MAX_STATE][MAX_STATE] = {0};

		FILE *seq = open_or_die( argv[3], "r" );
		char seql[MAX_SEQ]; // a sequence line
		p = fgets( seql, MAX_SEQ, seq );
		int S = 0;
		do { // foreach seql
			++S;
			int T = strlen(seql) - 1;
			// Forward Procedure
			double alpha[MAX_SEQ][MAX_STATE] = {0};
			// alpha_1(i) = pi_i * b_i(y_1)
			for( i = 0 ; i < hmm.state_num ; ++i ) {
				alpha[0][i] = hmm.initial[i] * hmm.observation[seql[0]-'A'][i];
			}
			// alpha_t+1(j) = b_j(y_t+1) * sigma(i=1~N)[alpha_t(i) * a_ij]
			for( t = 1 ; t < T ; ++t )
				for( j = 0 ; j < hmm.state_num ; ++j ) {
					for( i = 0 ; i < hmm.state_num ; ++i )
						alpha[t][j] += alpha[t-1][i] * hmm.transition[i][j]; // CORRECTED
					alpha[t][j] *= hmm.observation[seql[t]-'A'][j];
				}
			// Backward Procedure
			double beta[MAX_SEQ][MAX_STATE] = {0};
			// beta_T(i) = 1
			for( i = 0 ; i < hmm.state_num ; ++i ) {
				beta[T-1][i] = 1;
			}
			// beta_t(i) = sigma(j=1~N)[a_ij * b_j(y_t+1) * beta_t+1(j)]
			for( t = T-2 ; t >= 0 ; --t )
				for( i = 0 ; i < hmm.state_num ; ++i ) {
					for( j = 0 ; j < hmm.state_num ; ++j )
						beta[t][i] += hmm.transition[i][j] * hmm.observation[seql[t+1]-'A'][j] * beta[t+1][j];
				}
			// Gamma Accumulate (N), (K x N)
			// gamma_t(i) = alpha_t(i) * beta_t(i)
			// 			  / sigma(i=i~N)[alpha_t(i) * beta_t(i)]
			for( t = 0 ; t < T ; ++t ) {
				double denom = 0;
				for( i = 0 ; i < hmm.state_num ; ++i )
					denom += alpha[t][i] * beta[t][i];
				for( i = 0 ; i < hmm.state_num ; ++i ) {
					if( t == 0 )
						gamma_1[i] += alpha[0][i] * beta[0][i] / denom;
					gamma_acc[i] += alpha[t][i] * beta[t][i] / denom;
					gamma_acc_ob[seql[t]-'A'][i] += alpha[t][i] * beta[t][i] / denom;
				}
			}
			// Epsilon Accumulate (N x N)
			// epsilon_t(i,j) = alpha_t(i) * a_ij * b_j(y_t+1) * beta_t+1(j)
			//   			  / sigma(i=i~N)(j=i~N)[alpha_t(i) * a_ij * b_j(y_t+1) * beta_t+1(j)]
			for( t = 0 ; t < T-1 ; ++t ) {
				double denom = 0;
				for( i = 0 ; i < hmm.state_num ; ++i )
					for( j = 0 ; j < hmm.state_num ; ++j )
						denom += alpha[t][i] * hmm.transition[i][j] * hmm.observation[seql[t+1]-'A'][j] * beta[t+1][j];
				for( i = 0 ; i < hmm.state_num ; ++i )
					for( j = 0 ; j < hmm.state_num ; ++j )
						epsilon_acc[i][j] += alpha[t][i] * hmm.transition[i][j] * hmm.observation[seql[t+1]-'A'][j] * beta[t+1][j] / denom;
			}
		} while (p = fgets( seql, MAX_SEQ, seq ), !feof(seq));
		fclose(seq);
		// Re-estimate Model Parameters
		for( i = 0 ; i < hmm.state_num ; ++i ) {
			hmm.initial[i] = gamma_1[i] / S;
			for( j = 0 ; j < hmm.state_num ; ++j )
				hmm.transition[i][j] = epsilon_acc[i][j] / gamma_acc[i];
			for( k = 0 ; k < hmm.observ_num ; ++k )
				hmm.observation[k][i] = gamma_acc_ob[k][i] / gamma_acc[i];
		}
		//dumpHMM( stderr, &hmm );
		//printf("Iter %d\n", iter+1);
	}

	// model_output
	FILE *fp = open_or_die( argv[4], "w" );
	dumpHMM( fp, &hmm );
	fclose(fp);

	return 0;
}
