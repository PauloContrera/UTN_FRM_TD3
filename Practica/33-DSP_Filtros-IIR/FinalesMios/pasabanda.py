import numpy as np
from scipy import signal
import matplotlib.pyplot as plt

# Parámetros de la señal
Fs = 10000  # Frecuencia de muestreo (Hz)
t = np.arange(0, 1, 1/Fs)  # Vector de tiempo de 1 segundo
frequencies = [100, 1000, 5000]  # Frecuencias de las señales en Hz
amplitudes = [1, 1, 1]  # Amplitudes de las señales

# Generación de la señal compuesta
senal_entrada = np.sum([amplitudes[i] * np.sin(2*np.pi*frequencies[i]*t) for i in range(len(frequencies))], axis=0)

# Paso 2: Diseño del Filtro IIR Pasabandas
# Frecuencias de corte del filtro
frecuencia_de_corte_baja = 100  # Hz
frecuencia_de_corte_alta = 5000  # Hz

# Asegurarse de que las frecuencias de corte estén dentro del rango válido
if frecuencia_de_corte_baja >= 0.5 * Fs:
    frecuencia_de_corte_baja = 0.499 * Fs
    print("Advertencia: La frecuencia de corte baja está fuera del rango válido. Se ajustará a", frecuencia_de_corte_baja, "Hz.")

if frecuencia_de_corte_alta >= 0.5 * Fs:
    frecuencia_de_corte_alta = 0.499 * Fs
    print("Advertencia: La frecuencia de corte alta está fuera del rango válido. Se ajustará a", frecuencia_de_corte_alta, "Hz.")

# Orden del filtro (puedes ajustar este valor según sea necesario)
orden = 8

# Cálculo de las frecuencias normalizadas
frecuencia_de_corte_baja_normalizada = frecuencia_de_corte_baja / (0.5 * Fs)
frecuencia_de_corte_alta_normalizada = frecuencia_de_corte_alta / (0.5 * Fs)

print("Frecuencia de corte baja normalizada:", frecuencia_de_corte_baja_normalizada)
print("Frecuencia de corte alta normalizada:", frecuencia_de_corte_alta_normalizada)

# Diseño del filtro IIR pasabandas
b, a = signal.iirfilter(orden, [frecuencia_de_corte_baja_normalizada, frecuencia_de_corte_alta_normalizada], btype='band', ftype='butter')

print("Coeficientes del filtro (b):", b)
print("Coeficientes del filtro (a):", a)

# Paso 3: Implementación del Filtro

# Aplicar el filtro diseñado a la señal de prueba utilizando la función scipy.signal.filtfilt
senal_filtrada = signal.filtfilt(b, a, senal_entrada)

# Visualización de la señal original y la señal filtrada

# Vector de tiempo
tiempo = np.arange(0, len(senal_entrada) / Fs, 1 / Fs)

plt.figure(figsize=(10, 6))
# Gráfico de la señal original
plt.subplot(2, 1, 1)
plt.plot(tiempo, senal_entrada, label='Señal original', color='b')
plt.title('Señal original y señal filtrada')
plt.xlabel('Tiempo (s)')
plt.ylabel('Amplitud')
plt.legend()

# Gráfico de la señal filtrada
plt.subplot(2, 1, 2)
plt.plot(tiempo, senal_filtrada, label='Señal filtrada', color='r')
plt.xlabel('Tiempo (s)')
plt.ylabel('Amplitud')
plt.legend()

plt.tight_layout()
plt.show()