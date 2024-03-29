#define CONTROL
#include <math.h>
#include <stdio.h>
#include <stdlib.h>
// #include <time.h>
// #include <complex.h>
#include <cstring>
#include <fstream>
#include <iostream>
#include <string>

#include "global.hpp"
#include "read.hpp"
#include "resampling_new.hpp"

#include "linear_fit.hpp"
#include "mutils.hpp"
#include "various_fits.hpp"

#include "correlators_analysis.hpp"
#include "functions_analysis_omega.hpp"
struct kinematic kinematic_2pt;

generic_header read_head(FILE* stream) {
    generic_header header;
    return header;
}
void write_header_g2(FILE* jack_file, generic_header head) {
    int fi = 0;
    fi += fwrite(&head.T, sizeof(int), 1, jack_file);
    fi += fwrite(&head.L, sizeof(int), 1, jack_file);
    int nmu = head.mus.size();
    fi += fwrite(&nmu, sizeof(int), 1, jack_file);
    for (double mu : head.mus) {
        fi += fwrite(&mu, sizeof(double), 1, jack_file);
    }
}

char** argv_to_options(char** argv) {
    char** option;
    option = (char**)malloc(sizeof(char*) * 7);
    option[0] = (char*)malloc(sizeof(char) * NAMESIZE);
    option[1] = (char*)malloc(sizeof(char) * NAMESIZE);
    option[2] = (char*)malloc(sizeof(char) * NAMESIZE);
    option[3] = (char*)malloc(sizeof(char) * NAMESIZE);
    option[4] = (char*)malloc(sizeof(char) * NAMESIZE);
    option[5] = (char*)malloc(sizeof(char) * NAMESIZE);
    option[6] = (char*)malloc(sizeof(char) * NAMESIZE);

    mysprintf(option[1], NAMESIZE, "read_plateaux"); // blind/see/read_plateaux
    mysprintf(option[2], NAMESIZE, "-p");            // -p
    mysprintf(option[3], NAMESIZE, argv[2]);         // path
    mysprintf(option[4], NAMESIZE, argv[6]);         // resampling
    mysprintf(option[5], NAMESIZE, "no");            // pdf
    mysprintf(option[6], NAMESIZE, argv[3]);         // infile
    return option;
}

void init_global_head(generic_header head) {
    file_head.l1 = head.L;
    file_head.l0 = head.T;
    file_head.l2 = head.L;
    file_head.l3 = head.L;
    file_head.nk = 2;
    file_head.musea = head.mus[0];
    file_head.k = (double*)malloc(sizeof(double) * file_head.nk * 2);
    file_head.k[0] = 0;
    file_head.k[1] = 0;
    file_head.k[2] = head.mus[0];
    file_head.k[3] = head.mus[0];

    file_head.nmoms = 1;
    file_head.mom = (double**)malloc(sizeof(double*) * file_head.nmoms);
    for (int i = 0; i < file_head.nmoms; i++) {
        file_head.mom[i] = (double*)malloc(sizeof(double) * 4);
        file_head.mom[i][0] = 0;
        file_head.mom[i][1] = 0;
        file_head.mom[i][2] = 0;
        file_head.mom[i][3] = 0;
    }
}

void read_twopt(FILE* stream, double*** to_write, generic_header head) {
    // write your function to read the data
    int fi = 0;
    int id;
    int i = fread(&id, sizeof(int), 1, stream);
    for (int k = 0; k < head.ncorr; k++) {
        for (int t = 0; t < head.T; t++) {
            fi += fread(to_write[k][t], sizeof(double), 2, stream);
        }
    }
}

int main(int argc, char** argv) {
    error(argc != 7, 1, "nissa_mpcac ",
        "usage:./nissa_mpcac -p path file -bin $bin  jack/boot \n separate "
        "path and file please");

    char** option = argv_to_options(argv);

    char namefile[NAMESIZE];
    mysprintf(namefile, NAMESIZE, "%s/%s", option[3], option[6]);

    char namefile_plateaux[NAMESIZE];
    mysprintf(namefile_plateaux, NAMESIZE, "plateaux.txt");

    FILE* infile = open_file(namefile, "r");

    //////////////////////////////////// read and setup header
    generic_header head; // = read_head(infile);
    head.read_header_debug(infile);
    head.print_header();
    init_global_head(head);

    //////////////////////////////////// setup jackboot and binning
    int confs = head.Njack;
    int bin = atoi(argv[5]);
    int Neff = confs / bin;
    int Njack;
    if (strcmp(argv[6], "jack") == 0) {
        Njack = Neff + 1;
        myres = new resampling_jack(Neff);
    }
    else if (strcmp(argv[6], "boot") == 0) {
        Njack = (Neff * 2 + 1);
        myres = new resampling_boot(Neff * 2);
    }
    else {
        Njack = 0;
        error(1 == 1, 1, "main", "argv[7]= %s is not jack or boot", argv[7]);
    }
    // now Njack need to be the number of jacks
    head.Nconf = head.Njack;
    head.Njack = Njack;
    //////////////////////////////////// setup output files
    mysprintf(namefile, NAMESIZE, "%s/out/%s_output", option[3], option[6]);
    printf("writing output in :\n %s \n", namefile);
    FILE* outfile = open_file(namefile, "w+");

    mysprintf(namefile, NAMESIZE, "%s/jackknife/%s_%s", option[3], option[4],
        option[6]);
    FILE* jack_file = open_file(namefile, "w+");
    // write_header_g2(jack_file, head);
    head.write_header(jack_file);

    //////////////////////////////////// confs
    double**** data = calloc_corr(confs, head.ncorr, head.T);

    printf("confs=%d\n", confs);
    printf("ncorr=%d\n", head.ncorr);
    printf("kappa=%g\n", head.kappa);
    for (int iconf = 0; iconf < confs; iconf++) {
        read_twopt(infile, data[iconf], head);
    }

    double**** data_bin = binning(confs, head.ncorr, head.T, data, bin);
    double**** conf_jack = myres->create(Neff, head.ncorr, head.T, data_bin);
    free_corr(Neff, head.ncorr, head.T, data_bin);
    free_corr(confs, head.ncorr, head.T, data);

    /////////////////////////////////////////////////////////////////////////////////////////////////////////
    // print all the effective masses correlators
    // set the option to not read for a plateaux
    mysprintf(namefile, NAMESIZE, "%s/out/%s_meff_correlators", option[3],
        option[6]);
    FILE* outfile_meff_corr = open_file(namefile, "w+");
    mysprintf(namefile, NAMESIZE, "%s/out/%s_raw_correlators", option[3],
        option[6]);
    FILE* outfile_raw_corr = open_file(namefile, "w+");
    mysprintf(namefile, NAMESIZE, "%s/out/%s_shifted_correlators", option[3],
        option[6]);
    FILE* outfile_shifted_corr = open_file(namefile, "w+");
    mysprintf(namefile, NAMESIZE, "%s/out/%s_log_meff_shifted", option[3],
        option[6]);
    FILE* outfile_log_meff_shifted = open_file(namefile, "w+");
    mysprintf(namefile, NAMESIZE, "%s/out/%s_gamma", option[3], option[6]);
    FILE* out_gamma = open_file(namefile, "w+");

    mysprintf(namefile, NAMESIZE, "%s/out/%s_HLT_kernel", option[3],
        option[6]);
    FILE* outfile_HLT_kernel = open_file(namefile, "w+");
    mysprintf(namefile, NAMESIZE, "%s/out/%s_HLT_AoverB", option[3],
        option[6]);
    FILE* outfile_HLT_AoverB = open_file(namefile, "w+");


    char save_option[NAMESIZE];
    sprintf(save_option, "%s", option[1]);
    sprintf(option[1], "blind");
    FILE* dev_null = open_file("/dev/null", "w");
    struct fit_type fit_info_silent;
    fit_info_silent.verbosity = -1;
    fit_info_silent.chi2_gap_jackboot = 1e+6;
    fit_info_silent.guess_per_jack = 0;

    for (int icorr = 0; icorr < head.ncorr; icorr++) {
        // log effective mass
        double* tmp_meff_corr = plateau_correlator_function(
            option, kinematic_2pt, (char*)"P5P5", conf_jack, Njack,
            namefile_plateaux, outfile_meff_corr, icorr, "log", M_eff_log, dev_null,
            fit_info_silent);
        free(tmp_meff_corr);
        // raw correlator
        file_head.l0 = head.T * 2;
        tmp_meff_corr = plateau_correlator_function(
            option, kinematic_2pt, (char*)"P5P5", conf_jack, Njack,
            namefile_plateaux, outfile_raw_corr, icorr, "cor", identity, dev_null,
            fit_info_silent);
        free(tmp_meff_corr);
        tmp_meff_corr = plateau_correlator_function(
            option, kinematic_2pt, (char*)"P5P5", conf_jack, Njack,
            namefile_plateaux, outfile_raw_corr, icorr, "cor", identity_im,
            dev_null, fit_info_silent);
        free(tmp_meff_corr);
        file_head.l0 = head.T;
        // shifted correlator
        tmp_meff_corr = plateau_correlator_function(
            option, kinematic_2pt, (char*)"P5P5", conf_jack, Njack,
            namefile_plateaux, outfile_shifted_corr, icorr, "shift_cor", shift_corr,
            dev_null, fit_info_silent);
        free(tmp_meff_corr);
        // log_meff shifted correlator
        tmp_meff_corr = plateau_correlator_function(
            option, kinematic_2pt, (char*)"P5P5", conf_jack, Njack,
            namefile_plateaux, outfile_log_meff_shifted, icorr, "log_shift",
            M_eff_log_shift, dev_null, fit_info_silent);
        free(tmp_meff_corr);
    }
    fit_info_silent.restore_default();
    sprintf(option[1], "%s", save_option); // restore option
    corr_counter = -1;

    ////////////////////////////////////////////////////////////
    // symmetrize
    ////////////////////////////////////////////////////////////
    symmetrise_jackboot(Njack, 0, head.T, conf_jack);
    symmetrise_jackboot(Njack, 1, head.T, conf_jack);
    symmetrise_jackboot(Njack, 2, head.T, conf_jack);
    symmetrise_jackboot(Njack, 4, head.T, conf_jack);

    ////////////////////////////////////////////////////////////
    // start fitting
    //////////////////////////////
    corr_counter = -1;
    double* M_PS = plateau_correlator_function(
        option, kinematic_2pt, (char*)"P5P5", conf_jack, Njack,
        namefile_plateaux, outfile, 2, "M_{sigma}_shift", shift_and_M_eff_sinh_T, jack_file);
    free(M_PS);
    check_correlatro_counter(0);

    double* vev_sigma = (double*)malloc(sizeof(double) * Njack);
    for (int j = 0;j < Njack;j++) {
        vev_sigma[j] = 0;
        for (int t = 0;t < head.T;t++) {
            vev_sigma[j] += conf_jack[j][3][t][0];
        }
        vev_sigma[j] /= head.T;
    }
    printf("vev_sigma= %g  +- %g\n", vev_sigma[Njack - 1], myres->comp_error(vev_sigma));

    ///// vev_sigma subtracted
    struct fit_type fit_info;
    struct fit_result fit_out;

    fit_info.Nvar = 1;
    fit_info.Npar = 1;
    fit_info.N = 1;
    fit_info.Njack = Njack;
    fit_info.n_ext_P = 1;
    fit_info.ext_P = (double**)malloc(sizeof(double*) * fit_info.n_ext_P);
    fit_info.ext_P[0] = vev_sigma;
    fit_info.function = constant_fit;
    fit_info.linear_fit = true;
    fit_info.T = head.T;
    fit_info.corr_id = { 2 };

    struct fit_result corr_minus_vev = fit_fun_to_fun_of_corr(
        option, kinematic_2pt, (char*)"P5P5", conf_jack, namefile_plateaux,
        outfile, lhs_vev_sub, "corr-vev_sigma", fit_info,
        jack_file);
    check_correlatro_counter(1);
    // free_fit_result(fit_info, fit_out);
    fit_info.restore_default();

    fit_info.Nvar = 1;
    fit_info.Npar = 1;
    fit_info.N = 1;
    fit_info.Njack = Njack;
    fit_info.n_ext_P = 1;
    fit_info.ext_P = (double**)malloc(sizeof(double*) * fit_info.n_ext_P);
    fit_info.ext_P[0] = vev_sigma;
    fit_info.function = constant_fit;
    fit_info.linear_fit = true;
    fit_info.T = head.T;
    fit_info.corr_id = { 2 };

    struct fit_result fit_me_P5P5 = fit_fun_to_fun_of_corr(
        option, kinematic_2pt, (char*)"P5P5", conf_jack, namefile_plateaux,
        outfile, lhs_vev_sub_and_meff/* lhs_vev_sub */, "M_{sigma}-vev_sigma", fit_info,
        jack_file);
    check_correlatro_counter(2);
    // free_fit_result(fit_info, fit_out);
    fit_info.restore_default();




    /// GEVP sigma- pipi^I0
    double* vev_pipiI0 = (double*)malloc(sizeof(double) * Njack);
    for (int j = 0;j < Njack;j++) {
        vev_pipiI0[j] = 0;
        for (int t = 0;t < head.T;t++) {
            vev_pipiI0[j] += conf_jack[j][6][t][0];
        }
        vev_pipiI0[j] /= head.T;
    }
    printf("vev_pipiI0= %g  +- %g\n", vev_pipiI0[Njack - 1], myres->comp_error(vev_pipiI0));

    ////////////////////////////////////////////////////////////////
    /// sub vev and GEVP I0 pipi sigma
    ////////////////////////////////////////////////////////////////
    int ncorr_new = head.ncorr;

    fit_info.N = 3;
    fit_info.Njack = Njack;
    fit_info.T = head.T;
    fit_info.corr_id = { 2, 4 , 5 };
    fit_info.n_ext_P = 2;
    fit_info.ext_P = (double**)malloc(sizeof(double*) * fit_info.n_ext_P);
    fit_info.ext_P[0] = vev_sigma;
    fit_info.ext_P[1] = vev_pipiI0;
    add_correlators(option, ncorr_new, conf_jack, sub_vev, fit_info);


    //// struct to ignore the plateaux range
    fit_type tmp_info;
    tmp_info.codeplateaux = true;
    tmp_info.tmin = 1;
    tmp_info.tmax = 1;
    for (int i = 0; i < 3; i++) {
        char name[NAMESIZE];
        mysprintf(name, NAMESIZE, "corr_{I0}_vev_sub%d", i);
        double* tmp = plateau_correlator_function(
            option, kinematic_2pt, (char*)"P5P5", conf_jack, Njack,
            namefile_plateaux, outfile, ncorr_new - (3 - i), name, identity, jack_file, tmp_info);
        free(tmp);
        check_correlatro_counter(3 + i * 2);

        mysprintf(name, NAMESIZE, "M_{I0}_vev_sub%d", i);
        double* M_PS = plateau_correlator_function(
            option, kinematic_2pt, (char*)"P5P5", conf_jack, Njack,
            namefile_plateaux, outfile, ncorr_new - (3 - i), name, M_eff_T, jack_file);
        free(M_PS);
        check_correlatro_counter(3 + i * 2 + 1);
    }

    ///  GEVP
    fit_info.N = 2;
    fit_info.corr_id = { ncorr_new - 3, ncorr_new - 1, ncorr_new - 2 };//diag{ sigma, pipi  }
    fit_info.value_or_vector = 0; // 0= values
    fit_info.t0_GEVP = 3;
    fit_info.GEVP_ignore_warning_after_t = 10;
    fit_info.verbosity = 2;
    //fit_info.corr_id={1,2};
    printf("GEVP_sigma_pipi\n");
    add_correlators(option, ncorr_new, conf_jack, GEVP_matrix, fit_info);
    printf(" ncorr after GEVP %d\n", ncorr_new);
    int id_GEVP_sigma_pipi = ncorr_new;

    fit_info.restore_default();

    double* l0_GEVP = plateau_correlator_function(option, kinematic_2pt, (char*)"P5P5", conf_jack, Njack, namefile_plateaux, outfile, ncorr_new - 2, "GEVP_vev_sub_sigma_pipi_l0", M_eff_T, jack_file);
    check_correlatro_counter(9);

    double* l1_GEVP = plateau_correlator_function(option, kinematic_2pt, (char*)"P5P5", conf_jack, Njack, namefile_plateaux, outfile, ncorr_new - 1, "GEVP_vev_sub_sigma_pipi_l1", M_eff_T, jack_file);
    check_correlatro_counter(10);

    free(l0_GEVP);free(l1_GEVP);

    ////////////////////////////////////////////////////////////////
    /// shift and GEVP I0 pipi sigma
    ////////////////////////////////////////////////////////////////

    fit_info.N = 3;
    fit_info.Njack = Njack;
    fit_info.T = head.T;
    fit_info.corr_id = { 2, 4 , 5 };
    fit_info.n_ext_P = 0;
    add_correlators(option, ncorr_new, conf_jack, shift_corr, fit_info);


    //// struct to ignore the plateaux range
    tmp_info.codeplateaux = true;
    tmp_info.tmin = 1;
    tmp_info.tmax = 1;
    for (int i = 0; i < 3; i++) {
        char name[NAMESIZE];
        mysprintf(name, NAMESIZE, "corr_{I0}_shift%d", i);
        double* tmp = plateau_correlator_function(
            option, kinematic_2pt, (char*)"P5P5", conf_jack, Njack,
            namefile_plateaux, outfile, ncorr_new - (3 - i), name, identity, jack_file, tmp_info);
        free(tmp);
        check_correlatro_counter(11 + i * 2);

        mysprintf(name, NAMESIZE, "M_{I0}_shift%d", i);
        double* M_PS = plateau_correlator_function(
            option, kinematic_2pt, (char*)"P5P5", conf_jack, Njack,
            namefile_plateaux, outfile, ncorr_new - (3 - i), name, M_eff_sinh_T, jack_file);
        free(M_PS);
        check_correlatro_counter(11 + i * 2 + 1);
    }

    ///  GEVP
    fit_info.N = 2;
    fit_info.corr_id = { ncorr_new - 3, ncorr_new - 1, ncorr_new - 2 };//diag{ sigma, pipi  }
    fit_info.value_or_vector = 0; // 0= values
    fit_info.t0_GEVP = 3;
    fit_info.GEVP_ignore_warning_after_t = 10;
    fit_info.verbosity = 2;
    //fit_info.corr_id={1,2};
    printf("GEVP_sigma_pipi\n");
    add_correlators(option, ncorr_new, conf_jack, GEVP_matrix, fit_info);
    printf(" ncorr after GEVP %d\n", ncorr_new);
    int id_GEVP_shift_sigma_pipi = ncorr_new;

    fit_info.restore_default();

    l0_GEVP = plateau_correlator_function(option, kinematic_2pt, (char*)"P5P5", conf_jack, Njack, namefile_plateaux, outfile, ncorr_new - 2,
        "GEVP_shift_sigma_pipi_l0", M_eff_log, jack_file);
    check_correlatro_counter(17);

    l1_GEVP = plateau_correlator_function(option, kinematic_2pt, (char*)"P5P5", conf_jack, Njack, namefile_plateaux, outfile, ncorr_new - 1,
        "GEVP_shift_sigma_pipi_l1", M_eff_log, jack_file);
    check_correlatro_counter(18);

    free(l0_GEVP);free(l1_GEVP);

    // l0_GEVP = plateau_correlator_function(option, kinematic_2pt, (char*)"P5P5", conf_jack, Njack, namefile_plateaux, outfile, ncorr_new - 2, "GEVP_shift_sigma_pipi_lam0", identity, jack_file);
    // // check_correlatro_counter(17);

    // l1_GEVP = plateau_correlator_function(option, kinematic_2pt, (char*)"P5P5", conf_jack, Njack, namefile_plateaux, outfile, ncorr_new - 1, "GEVP_shift_sigma_pipi_lam1", identity, jack_file);
    // check_correlatro_counter(18);


}
