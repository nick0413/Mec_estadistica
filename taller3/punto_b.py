import numpy as np
import matplotlib.pyplot as plt
import matplotlib.colors as mcolors
import matplotlib.animation as anim
from scipy import stats
import os 


files_b = os.listdir('positions2')
files_b = [file_b for file_b in files_b if file_b.endswith('.bin')]


with open('config2.txt','r') as f:
	lines_b = [float(line_b) for line_b in f.readlines()]

dt_b=lines_b[1]
Temp_b=lines_b[3]
gamma_value_b=lines_b[4]
particle_positions_b = []
ii=0
N=len(files_b)
plt.rcParams['font.size'] = 20
plt.figure(figsize=(18,8),dpi=200)
for file_b in files_b:
	# check the file ends with .bin
	if not file_b.endswith('.bin'):
		continue
	ii+=1
	x=np.fromfile('positions2/'+file_b,dtype=np.float64)
	particle_positions_b.append(x)
	hsv=((N-ii)/N,1,0.5)

	rgb=mcolors.hsv_to_rgb(hsv)
	plt.plot(np.linspace(0,250,len(x)),x,linewidth=0.4,color=rgb)
plt.title("Posición de las partículas en el tiempo")
plt.ylabel("Posición")
plt.xlabel("Tiempo")
plt.show()
plt.clf()	

files_bv = os.listdir('velocities2')
files_bv = [file_bv for file_bv in files_bv if file_bv.endswith('.bin')]


particle_velocities_bv = []
ii=0
N=len(files_bv)
plt.figure(figsize=(18,8),dpi=200)
for file_bv in files_bv:
	# check the file ends with .bin
	if not file_bv.endswith('.bin'):
		continue
	ii+=1
	x=np.fromfile('velocities2/'+file_bv,dtype=np.float64)
	particle_velocities_bv.append(x)
	hsv=((N-ii)/N,1,0.5)

	rgb=mcolors.hsv_to_rgb(hsv)
	plt.plot(np.linspace(0,250,len(x)),x,linewidth=0.4,color=rgb)
	
particle_velocities_b_2d = np.stack(particle_velocities_bv)
velocities_b_avgerge=np.mean(particle_velocities_b_2d,axis=0)
plt.plot(velocities_b_avgerge,color='blue',linewidth=1,label='Velocidad promedio')
plt.legend()
plt.hlines(0,-10,250,colors='black',linewidth=2)
plt.title("Velocidad de las partículas en el tiempo")
plt.ylabel("Velocidad")
plt.xlabel("Tiempo")
plt.xlim(xmin=-1)