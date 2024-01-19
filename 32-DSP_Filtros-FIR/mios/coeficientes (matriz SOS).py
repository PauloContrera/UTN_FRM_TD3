import numpy as np
from scipy import signal

# Especificaciones del filtro
orden = 20
frecuencia_corte = 9600  # Hz
frecuencia_muestreo = 48000  # Hz
ripple_en_banda_paso = 1  # dB

# Diseñar el filtro Chebyshev Tipo II
sos = signal.cheby2(orden, ripple_en_banda_paso, frecuencia_corte, 'low', fs=frecuencia_muestreo, output='sos')

# Sumar todos los elementos de la matriz SOS
resp = np.sum(sos)

print(resp)
