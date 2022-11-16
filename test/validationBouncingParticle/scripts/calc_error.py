import matplotlib.pyplot as plt
import numpy as np
from sys import exit

baseline_coefficient_restitution = 0.92

simulation_data = np.loadtxt("results.dat")
simulation_data[:,1] /= baseline_coefficient_restitution

experimental_relation = np.exp(-35/simulation_data[:,0])

error = experimental_relation - simulation_data[:,1]

passed_test = np.all(abs(error) < 1.0e-2)

print(f"passed test? {passed_test}")


