import numpy as np
import matplotlib.pyplot as plt

# Parámetros de la señal
f = 5      # Frecuencia de la señal (Hz)
fs = 100     # Frecuencia de muestreo (Hz)
t_start = 0   # Tiempo de inicio (s)
t_end = 1 # Tiempo de finalización (s)

# Generar vector de tiempos
t = np.arange(t_start, t_end, 1/fs) 

# Generar señal senoidal
y = np.sin(2 * np.pi * f * t )
n=256
cuantum = np.digitize(y, np.linspace(-1,1,n ))
# Convert the array to 8-bit integer

int8_array = cuantum.astype(np.int8)
desp=[]
for elemento in cuantum:
    signo = elemento & 0b10000000
    if signo == 128 :
        desp.append ((elemento >> 4 )* -1)
    else:
        desp.append (elemento >> 4)


print (desp)
# Graficar
plt.figure(figsize=(10,6))
#plt.plot(t, cuantum, '-o', markersize=5, label="Puntos de Muestreo")  # Agregar puntos
plt.plot(t, cuantum, '-o', markersize=5, label="Puntos de Muestreo")  # Agregar puntos
plt.title("Señal Senoidal")
plt.xlabel("Tiempo (s)")
plt.ylabel("Amplitud")
plt.grid(True)
plt.tight_layout()
plt.show()

dumb = 0

