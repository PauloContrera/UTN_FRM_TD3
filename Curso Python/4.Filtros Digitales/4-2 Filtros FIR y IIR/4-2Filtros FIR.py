from scipy.signal import firwin, lfilter
import matplotlib.pyplot as plt
import numpy as np

# Diseño de un filtro FIR
coeficientes_fir = firwin(numtaps=100, cutoff=0.1, window='hamming')

# Aplicación del filtro FIR a una señal de entrada
señal_entrada = np.sin(2 * np.pi * 5 * np.arange(1000) / 1000)
señal_filtrada = lfilter(coeficientes_fir, 1.0, señal_entrada)

# Visualización de la señal de entrada y la señal filtrada
plt.plot(señal_entrada, label='Señal de Entrada')
plt.plot(señal_filtrada, label='Señal Filtrada')
plt.legend()
plt.show()
