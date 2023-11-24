import numpy as np
import matplotlib.pyplot as plt
import matplotlib.colors as mcolors
import matplotlib.animation as anim
from scipy import stats
import os 



files = os.listdir('positions1')
files = [file for file in files if file.endswith('.bin')]

with open('config1.txt','r') as f:
	lines = [float(line) for line in f.readlines()]



print(lines)
t_max_a = lines[0]
dt=lines[1]
V0=lines[2]
T=lines[3]

particle_postions_a = []
ii=0
N=len(files)
plt.figure(figsize=(18,8),dpi=200)
for file in files:
	# check the file ends with .bin
	if not file.endswith('.bin'):
		continue
	ii+=1
	x=np.fromfile('positions1/'+file,dtype=np.float64)
	particle_postions_a.append(x)
	hsv=((N-ii)/N,1,1)

	rgb=mcolors.hsv_to_rgb(hsv)
	# print(len(x))
	plt.plot(np.linspace(0,10000,len(x)),x,linewidth=0.4,color=rgb)


plt.xlim(xmin=-10)
plt.title("Posición como funcion del tiempo")
plt.xlabel("Tiempo")
plt.ylabel("Posición")
plt.show()

plt.clf()



particle_positions_2d_a = np.stack(particle_postions_a)
print(particle_positions_2d_a.shape)
variance_at_each_time_a = np.var(particle_positions_2d_a, axis=0)
print(variance_at_each_time_a.shape)

time_points_a = np.linspace(0,10000,len(variance_at_each_time_a))
coeffs = np.polyfit(time_points_a, variance_at_each_time_a, 1)
fitted_data = coeffs[0] * time_points_a + coeffs[1]

plt.figure(dpi=100)


slope, intercept, r_value, p_value, std_err = stats.linregress(time_points_a, variance_at_each_time_a)

# slope=2DT
D_a=slope/(2*T)
print(D_a)

print('slope =', slope)
print('intercept =', intercept)
print('r_value =', r_value)
print('p_value =', p_value)
print('std_err =', std_err)
scale=0.5
plt.figure(dpi=300,figsize=(16*scale,9*scale))
plt.text(0.05, 0.95, f'$\sigma^2$ = {coeffs[0]:.2f}x + {coeffs[1]:.2f}\nD={D_a:.3f}  $R^2=${r_value*r_value:.4f}', transform=plt.gca().transAxes, 
         fontsize=14, verticalalignment='top', bbox=dict(boxstyle='round', facecolor='white', alpha=0.5))

plt.plot(fitted_data, label='Fitted line',color='black')
plt.plot(time_points_a,variance_at_each_time_a,linewidth=0.8,color='red')
plt.title("Varianza como función del tiempo")
plt.xlabel("Tiempo")
plt.ylabel("Varianza")
