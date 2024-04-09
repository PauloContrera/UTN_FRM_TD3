import numpy as np #realizar operaciones matemáticas y manipulación de arreglos
from scipy.signal import firwin #librería SciPy, para diseñar filtros FIR
from scipy.signal import freqz #calcular y trazar la respuesta en frecuencia de un filtro digital
import matplotlib.pyplot as plt

# Especificaciones del filtro
frecuencia_muestreo = 44100.0  # Hz (en punto flotante)
frecuencia_de_corte = [300.0, 3400.0]  # Frecuencias de corte en Hz (en punto flotante)
orden_del_filtro = 10
#frecuencia_Nyquist = 44100.0/2
# Definir las frecuencias de corte normalizadas
#permite especificar las frecuencias de corte de un filtro independientemente 
#de la frecuencia de muestreo.
#La frecuencia de Nyquist es la mitad de la frecuencia de muestreo y representa el límite superior.
#Entonces, normalizar las frecuencias de corte significa expresar las frecuencias de corte en términos de una fracción de la frecuencia de Nyquist.
#Las frecuencias de corte normalizadas se expresan generalmente en el rango de 0 a 1, donde:

#0 representa la frecuencia de corte más baja posible (correspondiente a 0 Hz en el dominio de las frecuencias).
#1 representa la frecuencia de corte más alta posible (correspondiente a la frecuencia de Nyquist).
f1 = frecuencia_de_corte[0] / frecuencia_muestreo
f2 = frecuencia_de_corte[1] / frecuencia_muestreo

# Diseñar el filtro pasa-banda con ventana Blackman
fir_coefficients = firwin(orden_del_filtro, [f1, f2], window='blackman', pass_zero=False)

# Visualizar la respuesta en frecuencia del filtro
#se utiliza para calcular la respuesta en frecuencia de un filtro FIR
#w, h: Estas son dos variables que almacenarán los resultados
#freqz para calcular la respuesta en frecuencia del filtro FIR.
#fir_coefficients: Es un arreglo que contiene los coeficientes del filtro FIR
#worN=8000 significa que se calculará la respuesta en frecuencia en 8000 puntos equiespaciados en el rango de 0 a π radianes por segundo

w, h = freqz(fir_coefficients, worN=8000)

# Graficar la respuesta en frecuencia
plt.plot(0.5 * w / np.pi, np.abs(h)) #convertir las frecuencias en radianes por segundo en una escala normalizada que varía de 0 a 1.
plt.xlabel('Frecuencia normalizada')
plt.ylabel('Magnitud de respuesta')
plt.title('Respuesta en Frecuencia del Filtro Pasa-Banda')
plt.grid()#agregar una cuadrícula a un gráfico
plt.show()# Mostrar el gráfico
#firwin funcion para diseñar el filtro
#f1 y f2, que representan las frecuencias de corte del filtro. Estos valores deben estar en unidades normalizadas, es decir, en el rango [0, 1], 
#donde 1 corresponde a la frecuencia de Nyquist.
# window='blackman' se ha elegido la ventana de Blackman. Las ventanas son funciones que se utilizan para dar forma a la respuesta en frecuencia del filtro.
#pass_zero=False: Indica que estás diseñando un filtro pasa bandas, ya que pass_zero es False