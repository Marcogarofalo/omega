#ifndef functions_analysis_omega_H
#define functions_analysis_omega_H
#include "non_linear_fit.hpp"

double lhs_vev_sub_and_meff(int j, double**** in, int t, struct fit_type fit_info);
double lhs_vev_sub(int j, double**** in, int t, struct fit_type fit_info);

double** sub_vev(int j, double**** in, int t, struct fit_type fit_info);
double** shift_corr(int j, double**** in, int t, struct fit_type fit_info);
#endif
