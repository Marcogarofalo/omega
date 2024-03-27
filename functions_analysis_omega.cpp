#define functions_analysis_omega_C
#include "functions_analysis_omega.hpp"
#include "correlators_analysis.hpp"
#include "tower.hpp"

double lhs_vev_sub_and_meff(int j, double**** in, int t, struct fit_type fit_info) {
    int id = fit_info.corr_id[0];
    double vev = fit_info.ext_P[0][j];
    int T = fit_info.T;

    double ct = in[j][id][t][0] - vev * vev;
    double ctp1 = in[j][id][t + 1][0] - vev * vev;

    double mass = M_eff_T_ct_ctp1(t, T, ct, ctp1);
    return mass;
}


double lhs_vev_sub(int j, double**** in, int t, struct fit_type fit_info) {
    int id = fit_info.corr_id[0];
    double vev = fit_info.ext_P[0][j];

    return in[j][id][t][0] - vev * vev;

}

double** sub_vev(int j, double**** in, int t, struct fit_type fit_info) {
    double** r = malloc_2<double>(fit_info.N, 2);
    int isigma = fit_info.corr_id[0];
    int ipipi = fit_info.corr_id[1];
    int imix = fit_info.corr_id[2];
    double vev_sigma = fit_info.ext_P[0][j];
    double vev_pipi = fit_info.ext_P[1][j];
    

    r[0][0] = in[j][isigma][t][0] - vev_sigma * vev_sigma;
    r[0][1] = in[j][isigma][t][1] - vev_sigma * vev_sigma;

    r[1][0] = in[j][ipipi][t][0] - vev_pipi * vev_pipi;
    r[1][1] = in[j][ipipi][t][1] - vev_pipi * vev_pipi;

    r[2][0] = in[j][imix][t][0] - vev_pipi * vev_sigma;
    r[2][1] = in[j][imix][t][1] - vev_pipi * vev_sigma;

    return r;
}



double** shift_corr(int j, double**** in, int t, struct fit_type fit_info) {
    double** r = malloc_2<double>(fit_info.N, 2);
    int isigma = fit_info.corr_id[0];
    int ipipi = fit_info.corr_id[1];
    int imix = fit_info.corr_id[2];
    int T = fit_info.T;

    r[0][0] = in[j][isigma][t][0] - in[j][isigma][(t+1)%T][0]  ;
    r[0][1] = in[j][isigma][t][1] - in[j][isigma][(t+1)%T][1]  ;

    r[1][0] = in[j][ipipi][t][0] - in[j][ipipi][(t+1)%T][0];
    r[1][1] = in[j][ipipi][t][1] - in[j][ipipi][(t+1)%T][1];

    r[2][0] = in[j][imix][t][0] - in[j][imix][(t+1)%T][0];
    r[2][1] = in[j][imix][t][1] - in[j][imix][(t+1)%T][1];

    return r;
}
