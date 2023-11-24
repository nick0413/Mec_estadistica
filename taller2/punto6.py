import numpy as np

def erf(z):
	return np.math.erf(z)

D=10
t=5

a_r=1/(2*np.sqrt(D*t)) # es el termino de sqrt(a)

p0=50 	# para los puntos a la izquierda del orgien se uso el valor absoluto
p1=20	# dado que la gaussiana es simetrica
p2=10
p3=40

L0=-30 #los 3 pozos que se esta tomando en cuenta
L1=0
L2=30

A0=0.5*(erf(a_r*p0)-erf(a_r*p1))
A1=0.5*(erf(a_r*p1)+erf(a_r*p2))
A2=0.5*(erf(a_r*p3)-erf(a_r*p2))

Pos_m=A0*L0+A1*L1+A2*L2
#change the prints so it only prints 3 decimals


# print(" A0:",A0*100 ,"%\n","A1:",A1*100,"%\n","A2:",A2*100,"%")
# print("Escape:",1-(A0+A1+A2))
# print("Posicion media:",Pos_m)
# print("Velocidad media:",Pos_m/t)	


print(" A0: {:.3f}%\n".format(A0*100), "A1: {:.3f}%\n".format(A1*100), "A2: {:.3f}%\n".format(A2*100))
print("Escape: ",(1-(A0+A1+A2)))
print("Posicion media: {:.3f}".format(Pos_m))
print("Velocidad media: {:.3f}".format(Pos_m/t))