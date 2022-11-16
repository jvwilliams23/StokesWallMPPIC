import matplotlib.pyplot as plt
import numpy as np
from sys import exit

baseline_coefficient_restitution = 0.92

simulation_data = np.loadtxt("results.dat")
simulation_data[:,1] /= baseline_coefficient_restitution
print(simulation_data)

modelSt = np.arange(0.0001, max(simulation_data[:,0]))
experimental_relation = np.exp(-35/modelSt)

plt.close()
plt.plot(modelSt, experimental_relation, lw=2, label="Legendre relation")
plt.plot(simulation_data[:,0], simulation_data[:,1], lw=0, marker="o", ms=5, label="Simulation")
plt.legend(loc='upper left', ncol=1, fontsize=11, frameon=False)
plt.xscale('log')
plt.ylim(0, 1.1)
plt.xlim(min(simulation_data[:, 0]), max(simulation_data[:,0]))
plt.xlabel("Stokes number, St")
plt.ylabel(r"$e$ / $e_{max}$")
for pos in ["top", "right"]:
  plt.gca().spines[pos].set_visible(False)
#plt.savefig("stokesCoeffRest02.pdf",pad_inches=0, format="pdf")
plt.show()

