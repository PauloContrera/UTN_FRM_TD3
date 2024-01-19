from scipy.signal import iirfilter, lfilter
import matplotlib.pyplot as plt
import numpy as np

# Diseño de un filtro IIR
coeficientes_iir = iirfilter(N=4, Wn=0.1, rp=1, rs=20, btype='low', ftype='cheby2')

# Aplicación del filtro IIR a una señal de entrada
señal_entrada = np.sin(2 * np.pi * 5 * np.arange(1000) / 1000)
señal_filtrada = lfilter(coeficientes_iir[0], coeficientes_iir[1], señal_entrada)

# Visualización de la señal de entrada y la señal filtrada
plt.plot(señal_entrada, label='Señal de Entrada')
plt.plot(señal_filtrada, label='Señal Filtrada')
plt.legend()
plt.show()
