import numpy as np
from scipy.signal import firwin, convolve
import matplotlib.pyplot as plt
from scipy.io import loadmat
from scipy.fft import fft

# Cargar el archivo "Tchaikovsky.mat"
data = loadmat('Tchaikovsky.mat')

# Cargar la matriz de datos de la clave 'signal'
signal_data = data['signal']

# Especificaciones del filtro
frecuencia_muestreo = 44100.0
frecuencia_de_corte = [300.0, 3400.0]
orden_del_filtro = 50

# Definir las frecuencias de corte normalizadas
f1 = frecuencia_de_corte[0] / frecuencia_muestreo
f2 = frecuencia_de_corte[1] / frecuencia_muestreo

# Diseñar el filtro FIR pasa-banda con ventana Blackman
fir_coefficients = firwin(orden_del_filtro, [f1, f2], window='blackman', pass_zero=False)

# Aplicar el filtro FIR a la señal utilizando convolución
# Aplicar el filtro FIR a la señal utilizando convolución
#matrix[:, 0]: Esto accede a la primera columna de la matriz matrix, que contiene la señal de entrada.
#mode='same': Este argumento controla el modo de la convolución. En este caso, se utiliza el modo 'same', que hace que la salida de
# la convolución tenga la misma longitud que la señal de entrada
fir_output = convolve(signal_data[:, 0], fir_coefficients, mode='same')

# Calcular la transformada de Fourier de la señal original y la señal filtrada
spectrum_signal = fft(signal_data[:, 0])
spectrum_fir_output = fft(fir_output)

# Calcular las frecuencias correspondientes
n = len(signal_data)
#fftfreq devuelve un arreglo NumPy que contiene las frecuencias correspondientes a las componentes de la FFT
#n es el número de puntos de datos en la señal que estás analizando.
#1 / frecuencia_muestreo representa el espaciado en Hz entre las frecuencias en el dominio de la frecuencia.
frequencies = np.fft.fftfreq(n, 1 / frecuencia_muestreo)

# Crear una figura con cuatro subplots
fig, (ax1, ax2, ax3, ax4) = plt.subplots(4, 1, figsize=(12, 12))

# Panel 1: Dominio del tiempo - Señal Original
ax1.plot(signal_data[:, 0])
ax1.set_xlabel('Muestra')
ax1.set_ylabel('Amplitud')
ax1.set_title('Señal Original en el Dominio del Tiempo')

# Panel 2: Dominio del tiempo - Señal Filtrada
ax2.plot(fir_output)
ax2.set_xlabel('Muestra')
ax2.set_ylabel('Amplitud')
ax2.set_title('Señal Filtrada en el Dominio del Tiempo')

# Panel 3: Dominio de la frecuencia - Señal Original
ax3.plot(frequencies, np.abs(spectrum_signal))
ax3.set_xlabel('Frecuencia (Hz)')
ax3.set_ylabel('Amplitud')
ax3.set_title('Espectro de la Señal Original')
ax3.set_xlim(-4000, 3200)  # Establecer el rango de -4000 Hz a 4000 Hz en el eje x

# Panel 4: Dominio de la frecuencia - Señal Filtrada
ax4.plot(frequencies, np.abs(spectrum_fir_output))
ax4.set_xlabel('Frecuencia (Hz)')
ax4.set_ylabel('Amplitud')
ax4.set_title('Espectro de la Señal Filtrada')
ax4.set_xlim(-4000, 3200)  # Establecer el rango de -4000 Hz a 4000 Hz en el eje x

plt.tight_layout()
plt.show()
#La diferencia principal que se observa entre ambas señales en las gráficas de espectro es la atenuación de las 
#componentes de frecuencia en la señal filtrada en comparación con la señal original.
#en la gráfica del espectro de la "Señal Filtrada", las componentes de frecuencia fuera del 
#rango de frecuencias especificado por el filtro (300 Hz a 3400 Hz) están atenuadas o prácticamente eliminadas.
