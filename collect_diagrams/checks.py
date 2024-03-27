import numpy as np
import matplotlib.pyplot as plt

dir='/hiskp4/hyan/'
confs=["2700"]

imom='000-000-000-000'
imom_twop='000-000'
imom_one='000'
imom_twop_to_one='000-000-000'

outfile = "twop.dat"
ncorr=4
T=96
L=T//2

iconf=0
for conf in confs:
   

     # print(sigma)
#     D1 = np.load(dir+'/two/pipi-pipi-D1/2700/corr_two_pipi-pipi-D1_conf_'+conf+'_g0_5_g1_5_g2_5_g3_5_mom_'+imom+'_src_3.npy')
#     D2 = np.load(dir+'/two/pipi-pipi-D2/2700/corr_two_pipi-pipi-D2_conf_'+conf+'_g0_5_g1_5_g2_5_g3_5_mom_'+imom+'_src_3.npy')
#     print(D1-D2)
#     pi_to_pi = np.load(dir+'/onepi/'+conf+'/corr_onepi_conf_'+conf+'_gi_5_gj_5_mom_'+imom_twop+'_src_3.npy')
    
    
#     print(D1[0:4])
#     print(pi_to_pi[0:4]*pi_to_pi[0:4])
#     print("diff D1 pitopi^2: ",D1[0:4]-pi_to_pi[0:4]*pi_to_pi[0:4])
#     imom='110-000-001-00-1'
#     B1 = np.load(dir+'/two/pipi-pipi-B1/2700/corr_two_pipi-pipi-B1_conf_'+conf+'_g0_5_g1_5_g2_5_g3_5_mom_'+imom+'_src_3.npy')
#     imom='000-000-00-1-001'
#     B2 = np.load(dir+'/two/pipi-pipi-B2/2700/corr_two_pipi-pipi-B2_conf_'+conf+'_g0_5_g1_5_g2_5_g3_5_mom_'+imom+'_src_3.npy')
#     B3 = np.load(dir+'/two/pipi-pipi-B3/2700/corr_two_pipi-pipi-B3_conf_'+conf+'_g0_5_g1_5_g2_5_g3_5_mom_'+imom+'_src_3.npy')
#     B4 = np.load(dir+'/two/pipi-pipi-B4/2700/corr_two_pipi-pipi-B4_conf_'+conf+'_g0_5_g1_5_g2_5_g3_5_mom_'+imom+'_src_3.npy')
#     print(" B1  ",B1[0:4])
#     print(" B2  ",B2[0:4])
#     print(" B3  ",B3[0:4])
#     print(" B4  ",B4[0:4])
#     print("diff B1 B2",B1[0:4]-B2[0:4])
#     print("diff B1 B3",B1[0:4]-B3[0:4])
#     print("diff B1 B4",B1[0:4]-B4[0:4])
#     print("##################### E")
#     E1 = np.load(dir+'/two/pipi-pipi-E1/2700/corr_two_pipi-pipi-E1_conf_'+conf+'_g0_5_g1_5_g2_5_g3_5_mom_'+imom+'_src_3.npy')
#     E2 = np.load(dir+'/two/pipi-pipi-E2/2700/corr_two_pipi-pipi-E2_conf_'+conf+'_g0_5_g1_5_g2_5_g3_5_mom_'+imom+'_src_3.npy')
#     print(E1[0:4])
#     print(E2[0:4])
#     print("diff E1 E2",E1[0:4]-E2[0:4])
#     print("##################### A")
#     A = np.load(dir+'/two/pipi-pipi-A/2700/corr_two_pipi-pipi-A_conf_'+conf+'_g0_5_g1_5_g2_5_g3_5_mom_'+imom+'_src_3.npy')
#     print(A[0:4])
#     L2 = np.load(dir+'/annhilation/loop-2/'+conf+'/corr_annhilation_loop-2_conf_'+conf+'_g0_5_g1_5_mom_'+imom_twop+'.npy')
#     print( L2[3]* L2[3])
    
#     T1 = np.load(dir+'/two/pipi-one-T1/2700/corr_two_pipi-one-T1_conf_'+conf+'_g0_5_g1_5_g2_0_mom_'+imom_twop_to_one+'_src_3.npy')

#     T2 = np.load(dir+'/two/pipi-one-T2/2700/corr_two_pipi-one-T2_conf_'+conf+'_g0_5_g1_5_g2_0_mom_'+imom_twop_to_one+'_src_3.npy')
#     print(T1[0:4])
#     print(T2[0:4])
#     print("diff T1 T2",T1[0:4]-T2[0:4])
    
    
     
    
    name_A1_clock="corr_annhilation_loop-3_conf_2700_g0_5_g1_5_g2_5_mom_000-00-1-001.npy"
    name_A1_anticlock="corr_annhilation_loop-3_conf_2700_g0_5_g1_5_g2_5_mom_001-00-1-000.npy"
    name_A1_clock_minus="corr_annhilation_loop-3_conf_2700_g0_5_g1_5_g2_5_mom_000-001-00-1.npy"
    name_A1_anticlock_minus="corr_annhilation_loop-3_conf_2700_g0_5_g1_5_g2_5_mom_00-1-001-000.npy"
    dir="/hiskp4/hyan/research/omega/jump/000/F48P30/annhilation/loop-3/2700/"
    A1_clock = np.load(dir+name_A1_clock)
    A1_anticlock = np.load(dir+name_A1_anticlock)
    A1_clock_minus = np.load(dir+name_A1_clock_minus)
    A1_anticlock_minus = np.load(dir+name_A1_anticlock_minus)
    print("################################################## A diagram")
    print("clock(p)      :",A1_clock[0:4])
    print("anti-clock(p) :",A1_anticlock[0:4])
    print("clock(-p)     :",A1_clock_minus[0:4])
    print("anti-clock(-p):",A1_anticlock_minus[0:4])
    # B3 = np.load('.//two/pipi-pipi-B3/2700/corr_two_pipi-pipi-B3_conf_'+iconf+'_g0_5_g1_5_g2_5_g3_5_mom_'+imom+'_src_3.npy')
    # B4 = np.load('.//two/pipi-pipi-B4/2700/corr_two_pipi-pipi-B4_conf_'+iconf+'_g0_5_g1_5_g2_5_g3_5_mom_'+imom+'_src_3.npy')

    # E1 = np.load('.//two/pipi-pipi-E1/2700/corr_two_pipi-pipi-E1_conf_'+iconf+'_g0_5_g1_5_g2_5_g3_5_mom_'+imom+'_src_3.npy')
    # E2 = np.load('.//two/pipi-pipi-E2/2700/corr_two_pipi-pipi-E2_conf_'+iconf+'_g0_5_g1_5_g2_5_g3_5_mom_'+imom+'_src_3.npy')

    # A = np.load('.//two/pipi-pipi-A/2700/corr_two_pipi-pipi-A_conf_'+iconf+'_g0_5_g1_5_g2_5_g3_5_mom_'+imom+'_src_3.npy')

    # corr=np.real(D1+D2-(1.5)*(B1+B2+B3+B4)-0.5*(E1+E2)+3*A)
    # t = np.arange(0, len(corr), 1)
    # T=len(corr)
    # for i in range(1,T//2):
    #     print(i, (T-i))
    #     corr[i]+=corr[(T-i)] 
    #     corr[i]/=2
    #     corr[(T-i)] =corr[i]
    iconf +=1

