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
print(t)
#bins = np.array([-1,-1/3,1/3,1])
n=16.
cuantum = np.digitize(y, np.linspace(-1,1,n ))
print(cuantum)
# Graficar
plt.figure(figsize=(10,6))
plt.plot(t, cuantum, '-o', markersize=5, label="Puntos de Muestreo")  # Agregar puntos
plt.plot(t, y, '-o', markersize=5, label="Puntos de Muestreo")  # Agregar puntos
plt.title("Señal Senoidal")
plt.xlabel("Tiempo (s)")
plt.ylabel("Amplitud")
plt.grid(True)
plt.tight_layout()
plt.show()

dumb = 0