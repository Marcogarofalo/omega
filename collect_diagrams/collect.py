import numpy as np
import matplotlib.pyplot as plt

dir='/hiskp4/hyan/'
confs=["2700", "2720", "2740", "2760", "2780", "2800", "2820", "2840", "2860", "2880", "2900", "2920",
 "2940", "2960", "2980", "3000", "3020", "3040", "3060", "3080", "3100", "3120", "3140", "3160", "3180",
  "3200", "3220", "3240", "3260", "3280", "3300", "3320", "3340", "3360", "3380", "3400", "3420", "3440",
   "3460", "3480", "3500", "3520", "3540", "3560", "3580", "3600", "3620", "3640", "3660", "3680", "3700",
    "3720", "3740", "3760", "3780", "3800", "3820", "3840", "3860", "3880", "3900", "3920", "3940", "3960",
     "3980", "4000", "4020", "4040", "4060", "4080", "4100", "4120", "4140", "4160", "4180", "4200", "4220",
      "4240", "4260", "4280", "4300", "4320", "4340", "4360", "4380", "4400", "4420", "4440", "4460", "4480",
       "4500", "4520", "4540", "4560", "4580", "4600", "4620", "4640", "4660", "4680", "4700", "4720", "4740",
        "4760", "4780", "4800", "4820", "4840", "4860", "4880", "4900", "4920", "4940", "4960", "4980", "5000",
         "5020", "5040", "5060", "5080", "5100", "5120", "5140", "5160", "5180", "5200", "5220", "5240", "5260",
          "5280", "5300", "5320", "5340", "5360", "5380", "5400", "5420", "5440", "5460", "5480", "5500", "5520",
           "5540", "5560", "5580", "5600", "5620", "5640", "5660", "5680", "5700", "5720", "5740", "5760", "5780",
            "5800", "5820", "5840", "5860", "5880", "5900", "5920", "5940", "5960", "5980", "6000", "6020", "6040",
             "6060", "6080", "6100", "6120", "6140", "6160", "6180", "6200", "6220", "6240", "6260", "6280", "6300",
              "6320", "6340", "6360", "6380", "6400", "6420", "6440", "6460", "6480", "6500", "6520", "6540", "6560",
               "6580", "6600", "6620", "6640", "6660", "6680", "6700"]

# iconf='2700'
imom='000-000-000-000'
mom_two_to_two='000-000-000-000'
imom_twop='000-000'
imom_one='000'
imom_twop_to_one='000-000-000'
import time

outfile = "twop.dat"
ncorr=6
T=96
L=T//2
###########################
import struct
def write_int(binary_file, d ):
    s = struct.pack('i', d)
    binary_file.write(s)
def write_double(binary_file, d ):
    s = struct.pack('d', d)
    binary_file.write(s)

def write_corr(binary_file, d ):
    for i in range(len(d)):

        s = struct.pack('d', np.real(d[i]))
        binary_file.write(s)
        s = struct.pack('d', np.imag(d[i]))
        binary_file.write(s)
        


binary_file = open(outfile, "wb")
write_int(binary_file, len(confs))
write_int(binary_file, T)
write_int(binary_file, L)
write_int(binary_file, ncorr)

write_double(binary_file, 1.1)
write_double(binary_file, 1.1)

write_int(binary_file, 1)
write_double(binary_file, 1.1)

write_int(binary_file, 0)

write_int(binary_file, 0)

write_int(binary_file, 0)

write_int(binary_file, 0)

write_int(binary_file, 0)

write_int(binary_file, 0)

size=ncorr*2*T
# binary_file.write(size) 
write_int(binary_file, size)
####################################
# sigma_sigma=np.zeros(T,  dtype='complex128')
# print(sigma_sigma)
nhits=96
####################################
iconf=0
D1 = np.zeros(T,  dtype='complex128')
B1 = np.zeros(T,  dtype='complex128')
E1 = np.zeros(T,  dtype='complex128')
D2 = np.zeros(T,  dtype='complex128')
B2 = np.zeros(T,  dtype='complex128')
B3 = np.zeros(T,  dtype='complex128')
B4 = np.zeros(T,  dtype='complex128')
E2 = np.zeros(T,  dtype='complex128')
T1 = np.zeros(T,  dtype='complex128')
T2 = np.zeros(T,  dtype='complex128')
sigma=np.zeros(T,  dtype='complex128')
sigma_sigma=np.zeros(T,  dtype='complex128')
sigma_sigma_disc=np.zeros(T,  dtype='complex128')
sigma_disc= np.zeros(T,  dtype='complex128')
A = np.zeros(T,  dtype='complex128')
A1 = np.zeros(T,  dtype='complex128')

for conf in confs:
    print(conf)
    tstart = time.time()
    write_int(binary_file, iconf)
    sigma_sigma.fill(0)
    for s in range(0,T):
        sigma = np.load(dir+'/onepi/'+conf+'/corr_onepi_conf_'+conf+'_gi_0_gj_0_mom_'+imom_twop+'_src_'+str(s)+'.npy')
        sigma_sigma-=sigma 
   
    sigma_sigma/=T
    write_corr(binary_file, sigma_sigma)

    sigma_disc = np.load(dir+'/annhilation/loop-1/'+conf+'/corr_annhilation_loop-1_conf_'+conf+'_g0_0_mom_'+imom_one+'.npy')
    
    # sigma_disc =np.sqrt(2)
    sigma_sigma_disc.fill(0)
    for t in range(T):
        for t0 in range(T):
            sigma_sigma_disc[t] +=sigma_disc[t0]*sigma_disc[(t+t0)%T]
    sigma_sigma_disc *= 2/T 
    write_corr(binary_file, sigma_sigma_disc)
    sigma_sigma +=sigma_sigma_disc
    
    write_corr(binary_file, sigma_sigma)
    write_corr(binary_file, sigma_disc)  # 3
    ####################
    D1.fill(0)
    B1.fill(0)
    E1.fill(0)
    
    for s in range(0,T):
        pi_to_pi = np.load(dir+'/onepi/'+conf+'/corr_onepi_conf_'+conf+'_gi_5_gj_5_mom_'+imom_twop+'_src_'+str(s)+'.npy')
        D1 += pi_to_pi*pi_to_pi
        B1 += np.load(dir+'/two/pipi-pipi-B1/'+conf+'/corr_two_pipi-pipi-B1_conf_'+conf+'_g0_5_g1_5_g2_5_g3_5_mom_'+mom_two_to_two+'_src_'+str(s)+'.npy')
        E1 += np.load(dir+'/two/pipi-pipi-E1/'+conf+'/corr_two_pipi-pipi-E1_conf_'+conf+'_g0_5_g1_5_g2_5_g3_5_mom_'+mom_two_to_two+'_src_'+str(s)+'.npy')

    D1 /= T
    B1 /= T
    E1 /= T 
    
    D2 = D1
    B2 = B1
    B3 = B1
    B4 = B1
    E2 = E1
    L2 = np.load(dir+'/annhilation/loop-2/'+conf+'/corr_annhilation_loop-2_conf_'+conf+'_g0_5_g1_5_mom_'+imom_twop+'.npy')
    A.fill(0)
    for t in range(T):
        for t0 in range(T):
            A[t] += L2[t0]*L2[(t+t0)%T]
    A /= T
    write_corr(binary_file, D1+D2-1.5*(B1+B2+B3+B4)+0.5*(E1+E2)+3*A) # 4

    T1.fill(0)
    T2.fill(0)
    A1.fill(0)
    for s in range(0,T):
        T1 = np.load(dir+'/two/pipi-one-T1/'+conf+'/corr_two_pipi-one-T1_conf_'+conf+'_g0_5_g1_5_g2_0_mom_'+imom_twop_to_one+'_src_'+str(s)+'.npy')
        T2 = np.load(dir+'/two/pipi-one-T2/'+conf+'/corr_two_pipi-one-T2_conf_'+conf+'_g0_5_g1_5_g2_0_mom_'+imom_twop_to_one+'_src_'+str(s)+'.npy')
        for t in range(T):
            A1[s] += L2[t]* sigma_disc[(s+t)%T]
    write_corr(binary_file, np.sqrt(6)*(T1+T2)/2.0 - np.sqrt(6)* A) # 5
    tend = time.time()
    print("time: ",tend-tstart)
     # print(sigma)
    # D1 = np.load('./'+dir+'/two/pipi-pipi-D1/2700/corr_two_pipi-pipi-D1_conf_'+iconf+'_g0_5_g1_5_g2_5_g3_5_mom_'+imom+'_src_3.npy')
    # D2 = np.load('./'+dir+'/two/pipi-pipi-D2/2700/corr_two_pipi-pipi-D2_conf_'+iconf+'_g0_5_g1_5_g2_5_g3_5_mom_'+imom+'_src_3.npy')

    # B1 = np.load('.//two/pipi-pipi-B1/2700/corr_two_pipi-pipi-B1_conf_'+iconf+'_g0_5_g1_5_g2_5_g3_5_mom_'+imom+'_src_3.npy')
    # B2 = np.load('.//two/pipi-pipi-B2/2700/corr_two_pipi-pipi-B2_conf_'+iconf+'_g0_5_g1_5_g2_5_g3_5_mom_'+imom+'_src_3.npy')
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

binary_file.close()
