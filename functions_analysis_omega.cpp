#define functions_analysis_omega_C
#include "functions_analysis_omega.hpp"
#include "correlators_analysis.hpp"

double lhs_vev_sub_and_meff(int j, double**** in, int t, struct fit_type fit_info) {
    int id = fit_info.corr_id[0];
    double vev = fit_info.ext_P[0][j];
    int T = fit_info.T;

    double ct =  in[j][id][t][0] - vev * vev;
    double ctp1  = in[j][id][t + 1][0] - vev * vev;

    double mass = M_eff_T_ct_ctp1(t, T, ct, ctp1);
    return mass;
}


double lhs_vev_sub(int j, double**** in, int t, struct fit_type fit_info) {
    int id = fit_info.corr_id[0];
    double vev = fit_info.ext_P[0][j];

    return in[j][id][t][0] - vev * vev;

}