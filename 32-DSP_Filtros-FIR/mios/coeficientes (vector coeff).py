import scipy.signal
import numpy as np
import matplotlib.pyplot as plt

# Parámetros del filtro
order = 50  # Orden del filtro
cutoff = 2000  # Frecuencia de corte en Hz
fs = 44100  # Frecuencia de muestreo en Hz

# Calcula los coeficientes del filtro FIR con ventana de Blackman
coeff = scipy.signal.firwin(order + 1, cutoff / (0.5 * fs), window='blackman')

# Suma de los primeros 25 coeficientes
suma_primeros_25_coeficientes = np.sum(coeff[:25])

# Imprimir solo los coeficientes
print("Coeficientes del filtro FIR con ventana de Blackman:", coeff)

# Grafica la respuesta en frecuencia
w, h = scipy.signal.freqz(coeff)
plt.figure()
plt.plot(w, 20 * np.log10(abs(h)))
plt.title("Respuesta en Frecuencia del Filtro FIR con Ventana de Blackman")
plt.xlabel("Frecuencia [radianes/segundo]")
plt.ylabel("Ganancia [dB]")
plt.grid()

print("Suma de los primeros 25 coeficientes:", suma_primeros_25_coeficientes)

plt.show()  # Esta línea muestra la gráfica
