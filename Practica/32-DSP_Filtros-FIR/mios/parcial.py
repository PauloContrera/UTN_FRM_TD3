import numpy as np
from scipy.signal import butter, sosfreqz, sosfilt

# Parámetros del filtro
orden_filtro = 10
frecuencias_corte = [300, 3400]  # Frecuencias de corte en Hz
frecuencia_muestreo = 44100  # Frecuencia de muestreo en Hz

# Diseñar el filtro Butterworth
sos = butter(orden_filtro, frecuencias_corte, btype='band', fs=frecuencia_muestreo, output='sos')

# Sumar todos los elementos de la matriz SOS
resp = np.sum(sos)

# Imprimir la respuesta con 4 decimales y coma para los decimales
print("{:.4f}".format(resp).replace('.', ','))
