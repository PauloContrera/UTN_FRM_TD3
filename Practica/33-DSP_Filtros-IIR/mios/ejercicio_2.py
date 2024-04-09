import numpy as np
import matplotlib.pyplot as plt
import math
import numpy as np
import scipy.io
import os
import sounddevice as sd


from scipy.io import loadmat

from commpy.channels import awgn
from sklearn.metrics import mean_squared_error
from scipy.signal import lfilter
from scipy.signal import firwin
from scipy.signal import freqz
from scipy import signal

import matplotlib.pyplot as plt

import numpy as np

#FUNCION GRAFICAR
def graficar_tiempo(tiempos, signal, titulo):
    plt.figure(figsize=(10,6))
    plt.plot(tiempos, signal, '-o', markersize=5, label="Puntos de Muestreo")  # Agregar puntos
    plt.title(titulo)
    plt.xlabel("Tiempo (s)")
    plt.ylabel("Amplitud")
    plt.grid(True)
    plt.tight_layout()
    plt.show()

def graficar_frec(frec, resp, titulo):
    plt.subplot(211)
    plt.plot(frec, np.abs(resp))   #Grafica MAGNITUD
    plt.title('Respuesta en frecuencia %s' %titulo)
    plt.xlabel('Frecuencia [radianes/muestra]')
    plt.ylabel('Amplitud [dB]')
    plt.grid()
    
    plt.subplot(212)
    plt.plot(frec, np.angle(resp))  #Grafica FASE
    plt.title('Fase %s' %titulo)
    plt.xlabel('Frecuencia [radianes/muestra]')
    plt.ylabel('Amplitud [dB]')
    plt.grid()
    plt.show()
    
#FUNCION RUIDO
def my_awgn(signal, snr):
    
    # Calcular la potencia de la señal
    signal_power = np.mean(np.abs(signal) ** 2)

    # Calcular la potencia del ruido en función del SNR
    noise_power = signal_power / (10 ** (snr / 10))

    # Generar ruido blanco aditivo gaussiano
    noise = np.random.normal(0, np.sqrt(noise_power), len(signal))

    # Sumar el ruido a la señal
    noisy_signal = signal + noise

    return noisy_signal

#FILTRO FIR
def fir(frecuencia_signal,fs,ruido):
    #Frecuencia de corte
    fco=frecuencia_signal*2
    
    N = round (math.sqrt(((0.885894**2)*(fs**2)/(fco**2))-1))
    valor = 1 / N

    filter_coefficients = np.full (N, valor)   #Genero vector de N elementos de valor 1/N
    
    filtrada = lfilter (filter_coefficients, 1, ruido)
    
    return filtrada, filter_coefficients

def leaky_integrator(input_signal, alpha):
    """
    Implementa un filtro leaky integrador en Python.

    Args:
    input_signal (list): La señal de entrada que deseas suavizar.
    alpha (float): El factor de "fuga" (leak), generalmente un valor entre 0 y 1.

    Returns:
    list: La señal suavizada.
    """
    smoothed_signal = [0]  # Inicializa la señal suavizada con un valor inicial de 0.

    for i in range(1, len(input_signal)):
        smoothed_value = alpha * smoothed_signal[i - 1] + (1 - alpha) * input_signal[i]
        smoothed_signal.append(smoothed_value)

    return smoothed_signal

#RESPUESTA EN FRECUENCIA Y FASE filtro
def respuesta_filtro(b, a, alpha, fs):

    frequencies, response = freqz(b, a)
    fco = -np.log(alpha) * fs / np.pi  # Calcular la frecuencia de corte
    print ("La frecuencia de corte es: ", fco)
    graficar_frec(frequencies, response, 'filtro')
    
    
#RESPUESTA EN FRECUENCIA Y FASE señal
def respuesta_signal(fs, signal, titulo):
    # Calcular la transformada de Fourier de la señal con ruido
    transformada = np.fft.fft(signal)

    #Respuesta en Frecuencia de la Señal con Ruido
    #frecuencias = np.fft.fftfreq(1000, 1/fs)
    frec = np.fft.fftfreq(len(transformada), 1/fs)
    graficar_frec(frec, transformada, titulo)

def zplane(b, a):
    """
    Plot the complex z-plane given a transfer function.
    """
    
    # Calculate zeros and poles
    zeros = np.roots(b)
    poles = np.roots(a)

    # Plot the unit circle
    unit_circle = plt.Circle((0, 0), 1, color='blue', fill=False, linestyle='dashed')
    
    # Create figure and axis
    fig, ax = plt.subplots()
    ax.add_artist(unit_circle)

    # Plot zeros and poles
    plt.scatter(np.real(zeros), np.imag(zeros), marker='o', color='red', label='Zeros')
    plt.scatter(np.real(poles), np.imag(poles), marker='x', color='green', label='Poles')

    # Set labels and title
    plt.title('Pole-Zero Plot')
    plt.xlabel('Real')
    plt.ylabel('Imaginary')

    # Set equal aspect ratio
    plt.axis('equal')

    # Set grid
    plt.grid(which='both', color='grey', linestyle=':', linewidth=0.5)

    # Add legend
    plt.legend()

    # Show plot
    plt.show()
    
# Cargar el archivo "Tchaikovsky.mat"
data = loadmat('Tchaikovsky.mat')

# Cargar la matriz de datos de la clave 'signal'
signal1 = data['signal']

# Genera señal con ruido
noisy_signal = my_awgn(signal1[:, 0], snr=10) #	Cambiar a menos para escuchar mejor el ruido

# Parametros Filtro IIR Pasa-Banda
orden = 6
fs = 44100 # Frecuencia de muestreo
f1, f2 = 300, 3400 # Frecuencias de corte
rp = 0.5 # Ripple en la banda de paso en dB
rs = 60 # Atenuación en la banda de rechazo en dB
 
# Diseñar filtro pasa-banda Elliptic
b, a = signal.ellip(orden, rp, rs, [f1, f2], btype='bandpass',fs=fs) # 'b' (Numeroador)  - 'a' (Denominador)

# Respuesta en frecuencia del filtro
w, h = signal.freqz(b, a, worN=2000)

# Graficar respuesta en frecuencia
plt.figure(figsize=(12, 6))
plt.subplot(2, 1, 1)
plt.plot(w * fs / (2 * np.pi), 20 * np.log10(abs(h)),
color='blue')
plt.title('Respuesta en Frecuencia del Filtro EllipticPasa-Banda')
plt.xlabel('Frecuencia [Hz]')
plt.ylabel('Amplitud [dB]')
plt.grid(True)
plt.axvline(f1, color='green') # Frecuencia de corte inferior
plt.axvline(f2, color='green') # Frecuencia de corte superior
plt.axhline(-rp, color='red', linestyle='--') # Ripple de la banda de paso
plt.axhline(-rs, color='red', linestyle='--') # Atenuación de la banda de rechazo
plt.xlim(0, fs/2)
plt.ylim(-80, 5)
# Graficar respuesta de fase
plt.subplot(2, 1, 2)
plt.plot(w * fs / (2 * np.pi), np.unwrap(np.angle(h)),
color='blue')
plt.xlabel('Frecuencia [Hz]')
plt.ylabel('Fase [radianes]')
plt.grid(True)
plt.xlim(0, fs/2)

# Mostrar gráficos
plt.tight_layout()
plt.show()

# Convertir los coeficientes b y a a la representación SOS
sos = signal.tf2sos(b, a)

# Filtrar una señal x usando la representación SOS
y = signal.sosfilt(sos, noisy_signal)


# Parámetros de la señal
#f = 100       # Frecuencia de la señal (Hz)
#fs = 1600     # Frecuencia de muestreo (Hz)
#t_start = 0   # Tiempo de inicio (s)
#t_end = 0.1  # Tiempo de finalización (s)

# Generar vector de tiempos
#t = np.arange(t_start, t_end, 1/fs)

# Generar señal senoidal
#y = np.sin(2 * np.pi * f * t + np.pi/2)

# Genera señal con ruido
#noisy_signal = my_awgn(y, snr=15)

# Ejemplo de uso:
#alpha = 0.7  # Puedes ajustar este valor para controlar la cantidad de suavizado.

# Coeficientes del filtro Leaky Integrator
#b = [1 - alpha]  # Coeficiente del filtro
#a = [1, -alpha]  # Denominador, en este caso, solo un coeficiente 1

# Señal filtrada
#signal_filtrada = leaky_integrator(noisy_signal, alpha)

# Respuesta en frecuencia filtro
#respuesta_filtro(b, a, alpha, fs)

# Ejemplo de uso de la función
#zplane(b, a)

#Respuesta en frecuencia señal con ruido y la filtrada
respuesta_signal(fs, noisy_signal,'señal con ruido')
respuesta_signal(fs, y,'señal filtrada')

# Graficar
#graficar_tiempo(t,y,"Señal Senoidal")
#graficar_tiempo(t,noisy_signal,"Señal de ruido")
#graficar_tiempo(t,signal_filtrada,"Señal Filtrada")