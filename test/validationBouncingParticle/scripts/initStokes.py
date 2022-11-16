import numpy as np

NUM_SAMPLES = 25

rhop = 1000
CMinf = 0.5 
coeffRhof=1000
Un = 1
dp = 0.1

stokes_all = np.geomspace(1e-1, 1e5, num=NUM_SAMPLES)
#viscosity_all = stokes_all * 9.0 / ((rhop+CMinf*coeffRhof)*Un*dp)
viscosity_all = ((rhop+CMinf*coeffRhof)*Un*dp) / (9.0 * stokes_all)


np.savetxt("stokes_numbers.txt", stokes_all)
np.savetxt("viscosity_list.txt", viscosity_all)

