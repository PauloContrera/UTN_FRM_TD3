import numpy as np
from scipy.signal import firwin
from scipy.signal import freqz
import matplotlib.pyplot as plt

# Especificaciones del filtro
frecuencia_muestreo = 44100.0  # Hz (en punto flotante)
frecuencia_de_corte = [300.0, 3400.0]  # Frecuencias de corte en Hz (en punto flotante)
orden_del_filtro = 50
frecuencia_Nyquist = 44100.0/2
# Definir las frecuencias de corte normalizadas
f1 = frecuencia_de_corte[0] / frecuencia_muestreo
f2 = frecuencia_de_corte[1] / frecuencia_muestreo
# Diseñar el filtro pasa-banda con ventana Blackman
fir_coefficients = firwin(orden_del_filtro, [f1, f2], window='blackman', pass_zero=False)

# Visualizar la respuesta en frecuencia del filtro
w, h = freqz(fir_coefficients, worN=8000)

# Graficar la respuesta en frecuencia
plt.plot(0.5 * w / np.pi, np.abs(h))
plt.xlabel('Frecuencia normalizada')
plt.ylabel('Magnitud de respuesta')
plt.title('Respuesta en Frecuencia del Filtro Pasa-Banda')
plt.grid()
plt.show()

#f1 y f2, que representan las frecuencias de corte del filtro. Estos valores deben estar en unidades normalizadas, es decir, en el rango [0, 1], 
#donde 1 corresponde a la frecuencia de Nyquist.
# window='blackman' se ha elegido la ventana de Blackman. Las ventanas son funciones que se utilizan para dar forma a la respuesta en frecuencia del filtro.
#pass_zero=False: Este argumento indica que el filtro no es de paso cero. El filtro tendrá una fase no nula.

#AL AUMENTAR EL ORDEN DEL FILTRO LO QUE ESTOY HACIENDO ES DISMINUIR EL ANCHO DE LA BANDA DE TRANSCIÓN