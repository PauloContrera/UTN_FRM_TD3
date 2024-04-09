import numpy as np
import matplotlib.pyplot as plt
#import commpy.channels as chan

def my_awgn(signal, snr):
    vars=np.var(signal)
    #varr=np.sqrt((np.var(signal)^2)/(10^(snr/10)))
    v= 10**(snr/10)
    c= np.var(signal)**2
    b= c/v
    varr = np.sqrt(b)
    print(varr)

# Parámetros de la señal
f = 100       # Frecuencia de la señal (Hz)
fs = 1000     # Frecuencia de muestreo (Hz)
t_start = 0   # Tiempo de inicio (s)
t_end = 0.2 # Tiempo de finalización (s)

# Generar vector de tiempos
t = np.arange(t_start, t_end, 1/fs) 

# Generar señal senoidal
y = np.sin(2 * np.pi * f * t ) 
noise= np.random.normal(0, 0.1, len(t))
x= y + noise
#print(noise)
# Graficar
plt.figure(figsize=(10,6))
plt.plot(t, y, '-o', markersize=5, label="Puntos de Muestreo")  # Agregar puntos
plt.title("Señal Senoidal")
plt.xlabel("Tiempo (s)")
plt.ylabel("Amplitud")
plt.grid(True)
plt.tight_layout()
plt.show()
# Graficar
plt.figure(figsize=(10,6))
plt.plot(t, noise, '-o', markersize=5, label="Puntos de Muestreo")  # Agregar puntos
plt.title("Señal Senoidal")
plt.xlabel("Tiempo (s)")
plt.ylabel("Amplitud")
plt.grid(True)
plt.tight_layout()
plt.show()

dumb = 0

my_awgn(y, 15)
