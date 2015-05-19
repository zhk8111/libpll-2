/*
    Copyright (C) 2015 Diego Darriba

    This program is free software: you can redistribute it and/or modify
    it under the terms of the GNU Affero General Public License as
    published by the Free Software Foundation, either version 3 of the
    License, or (at your option) any later version.

    This program is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU Affero General Public License for more details.

    You should have received a copy of the GNU Affero General Public License
    along with this program.  If not, see <http://www.gnu.org/licenses/>.

    Contact: Diego Darriba <Diego.Darriba@h-its.org>,
    Exelixis Lab, Heidelberg Instutute for Theoretical Studies
    Schloss-Wolfsbrunnenweg 35, D-69118 Heidelberg, Germany
*/
#include "pll.h"

#define N_CAT_GAMMA 4
#define ALPHA 1

#define N_PROT_MODELS 19
#define N_STATES 20
#define N_PROT_RATES 190

#define FLOAT_PRECISION 5

  static double * prot_matrices[N_PROT_MODELS] =
      {
      pll_aa_rates_dayhoff,  pll_aa_rates_lg,
      pll_aa_rates_dcmut,    pll_aa_rates_jtt,
      pll_aa_rates_mtrev,    pll_aa_rates_wag,
      pll_aa_rates_rtrev,    pll_aa_rates_cprev,
      pll_aa_rates_vt,       pll_aa_rates_blosum62,
      pll_aa_rates_mtmam,    pll_aa_rates_mtart,
      pll_aa_rates_mtzoa,    pll_aa_rates_pmb,
      pll_aa_rates_hivb,     pll_aa_rates_hivw,
      pll_aa_rates_jttdcmut, pll_aa_rates_flu,
      pll_aa_rates_stmtrev
      };

  static double * prot_freqs[N_PROT_MODELS] =
      {
      pll_aa_freqs_dayhoff,  pll_aa_freqs_lg,
      pll_aa_freqs_dcmut,    pll_aa_freqs_jtt,
      pll_aa_freqs_mtrev,    pll_aa_freqs_wag,
      pll_aa_freqs_rtrev,    pll_aa_freqs_cprev,
      pll_aa_freqs_vt,       pll_aa_freqs_blosum62,
      pll_aa_freqs_mtmam,    pll_aa_freqs_mtart,
      pll_aa_freqs_mtzoa,    pll_aa_freqs_pmb,
      pll_aa_freqs_hivb,     pll_aa_freqs_hivw,
      pll_aa_freqs_jttdcmut, pll_aa_freqs_flu,
      pll_aa_freqs_stmtrev
      };

  static char * prot_model_names[N_PROT_MODELS] =
      {
          "Dayhoff", "LG",
          "DCMut", "JTT",
          "MtREV", "WAG",
          "RtREV", "CpREV",
          "VT", "Blosum62",
          "MtMam", "MtArt",
          "MtZoa", "PMB",
          "HIVb", "HIVw",
          "JTT-DCMut", "FLU",
          "StmtREV"
      };

int main(int argc, char * argv[])
{
  int i, cur_model;


  pll_partition_t * partition;
  pll_operation_t * operations;

  printf ("Creating PLL partition\n");

  partition = pll_create_partition(5,                            /* tips */
                                   4,                     /* clv buffers */
                                   N_STATES,                   /* states */
                                   113,                           /* sites */
                                   1,       /* different rate parameters */
                                   8,            /* probability matrices */
                                   N_CAT_GAMMA,       /* rate categories */
                                   1,
                                   1);
  
  double branch_lengths[4] = { 0.1, 0.2, 1, 1};
  int matrix_indices[4] = { 0, 1, 2, 3 };

  double * rate_cats = (double *) malloc(N_CAT_GAMMA * sizeof(double));

  if (pll_compute_gamma_cats(ALPHA, N_CAT_GAMMA, rate_cats) == PLL_FAILURE)
  {
    printf("Fail computing the gamma rates\n");
    exit(1);
  }
  pll_set_category_rates (partition, rate_cats);
  free(rate_cats);

  pll_set_tip_states (partition, 0, pll_map_aa,
     "PIGLRVTLRRDRMWIFLEKLLNVALPRIRDFRGLN--PNSFDGRGNYNLGLREQLIFPEITYDMVDALRGMDIAVVT------TAETDEE----------ARALLELLGFPFR");
  pll_set_tip_states (partition, 1, pll_map_aa,
     "PIGLKVTLRGARMYNFLYKLINIVLPKVRDFRGLD--PNSFDGRGNYSFGLSEQLVFPELNPDEVRRIQGMDITIVT------TAKTDQE----------ARRLLELFGMPFK");
  pll_set_tip_states (partition, 2, pll_map_aa,
     "AIGAKVTLRGKKMYDFLDKLINVALPRVRDFRGVS--KTSFDGFGNFYTGIKEQIIFPEVDHDKVIRLRGMDITIVT------SAKTNKE----------AFALLQKIGMPFE");
  pll_set_tip_states (partition, 3, pll_map_aa,
     "PIGVMVTLRGDYMYAFLDRLINLSLPRIRDFRGIT--AKSFDGRGNYNLGLKEQLIFPEVDYDGIEQIRGMDISIVT------TAKTDQE----------GLALLKSLGMPFA");
  pll_set_tip_states (partition, 4, pll_map_aa,
     "PIGTHATLRGDRMWEFLDRLVTLPLPRIRDFRGLS--DRQFDGNGNYTFGLSEQTVFHEIDQDKIDRVRGMDITVVT------TAKNDDE----------GRALLKALGFPFK");
  
  operations = (pll_operation_t *)malloc(4* sizeof(pll_operation_t));

  operations[0].parent_clv_index    = 5;
  operations[0].child1_clv_index    = 0;
  operations[0].child2_clv_index    = 1;
  operations[0].child1_matrix_index = 1;
  operations[0].child2_matrix_index = 1;

  operations[1].parent_clv_index    = 6;
  operations[1].child1_clv_index    = 5;
  operations[1].child2_clv_index    = 2;
  operations[1].child1_matrix_index = 0;
  operations[1].child2_matrix_index = 1;

  operations[2].parent_clv_index    = 7;
  operations[2].child1_clv_index    = 3;
  operations[2].child2_clv_index    = 4;
  operations[2].child1_matrix_index = 1;
  operations[2].child2_matrix_index = 1;

  for (cur_model=0; cur_model < N_PROT_MODELS; cur_model++) {

    printf ("\nSetting model %s...\n", prot_model_names[cur_model]);

    pll_set_subst_params(partition, 0, prot_matrices[cur_model]);
    pll_set_frequencies(partition, 0, prot_freqs[cur_model]);

    double sum_freqs = 0.0;
    for (i = 0; i < N_STATES; ++i)
    {
      sum_freqs += partition->frequencies[0][i];
    }
	if (fabs(sum_freqs - 1.0) > 1e-10)
      printf (" WARNING: Freq sum diff: %e\n", sum_freqs - 1.0);

    printf ("Updating prob matrices...\n");

   // pll_update_invariant_sites_proportion(partition, 0.17);
    pll_update_prob_matrices(partition, 0, matrix_indices, branch_lengths, 4);
    for (i = 0; i < 4; ++i)
    {
      printf ("P-matrix for branch length %f\n", branch_lengths[i]);
      pll_show_pmatrix(partition, i, FLOAT_PRECISION);
      printf ("\n");
    }

    pll_update_partials(partition, operations, 3);
    
    printf ("Tip 0: ");
    pll_show_clv(partition,0,FLOAT_PRECISION+1);
    printf ("Tip 1: ");
    pll_show_clv(partition,1,FLOAT_PRECISION+1);
    printf ("Tip 2: ");
    pll_show_clv(partition,2,FLOAT_PRECISION+1);
    printf ("Tip 3: ");
    pll_show_clv(partition,3,FLOAT_PRECISION+1);
    printf ("Tip 4: ");
    pll_show_clv(partition,4,FLOAT_PRECISION+1);
    printf ("CLV 5: ");
    pll_show_clv(partition,5,FLOAT_PRECISION+1);
    printf ("CLV 6: ");
    pll_show_clv(partition,6,FLOAT_PRECISION+1);
    printf ("CLV 7: ");
    pll_show_clv(partition,7,FLOAT_PRECISION+1);

    double logl = pll_compute_edge_loglikelihood(partition,6,7,0,0);

    printf("Log-L (%s): %.12f\n", prot_model_names[cur_model], logl);
  }

  free(operations);
  pll_destroy_partition(partition);

  return (0);
}