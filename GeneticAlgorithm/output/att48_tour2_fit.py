import matplotlib.pyplot as plt
import numpy as np

data = np.loadtxt('att48_tour2.f')
data = np.transpose(data)

plt.figure(1)
plt.plot(data[0, :], data[1, :], 'r')
plt.plot(data[0, :], data[2, :], 'b')
plt.legend(['Fitness of best solution', 'Average population fitness'])
plt.savefig('plots/att48_tour2_fit.png')
plt.figure(2)
plt.loglog(data[0, :], data[1, :], 'r')
plt.loglog(data[0, :], data[2, :], 'b')
plt.legend(['Fitness of best solution', 'Average population fitness'])
plt.savefig('plots/att48_tour2_fit_log.png')
