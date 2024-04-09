import numpy as np
import matplotlib.pyplot as plt
from scipy.signal import freqz
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

def my_awgn(signal, snr):

    # Calculate signal power and convert to dB
    Ps = 10 * np.log10(np.mean(signal ** 2))

    # Calculate noise power and convert to dB
    Pn = Ps - snr

    # Generate noise
    noise = np.random.normal(0, np.sqrt(10 ** (Pn / 10)), signal.shape)

    # Add noise to signal
    signal_noisy = signal + noise

    return signal_noisy


# Create a sine wave
# Parámetros de la señal
f = 100       # Frecuencia de la señal (Hz)
fs = 500     # Frecuencia de muestreo (Hz)
t_start = 0   # Tiempo de inicio (s)
t_end = 0.5# Tiempo de finalización (s)

# Generar vector de tiempos
t = np.arange(t_start, t_end, 1/fs) 
sine_wave = np.sin(2 * np.pi * 5 * t)
snr = 15

signal_noisy = my_awgn(sine_wave, snr)

# Parámetro lambda (ajusta este valor según sea necesario)
lambd = 0.7
#calculo de fco
fco=-np.log(lambd)*(fs/np.pi)

# Aplicar el filtro IIR "leaky integrator"
filtro_salida = np.zeros_like(signal_noisy)
for i in range(1, len(t)):
    filtro_salida[i] = lambd * filtro_salida[i-1] + (1 - lambd) * signal_noisy[i]
#respuesta en frecuencia
#calculo de coeficientes
a = [1, -lambd]
b = [1 - lambd]
zplane(b, a)

frequencies = np.linspace(0, fs / 2, 1000) # Vector de frecuencias
w_iir, h_iir = freqz(filtro_salida, worN=frequencies, fs=fs)

# Graficar
# Creamos los graficos en el tiempo
# Graficar respuesta en frecuencia de los filtros
plt.figure(figsize=(12, 6))

plt.subplot(221)
plt.plot(w_iir, 20 * np.log10(abs(h_iir)), label='IIR')
plt.title('Respuesta en Frecuencia de los Filtros Pasa-Banda')
plt.xlabel('Frecuencia [Hz]')
plt.ylabel('Amplitud [dB]')
plt.grid(True)
plt.legend()
plt.axvline(0, color='red', linestyle='--')  # Frecuencia de corte inferior
plt.axvline(fco, color='red', linestyle='--')  # Frecuencia de corte superior
plt.xlim(0, fs/2)

# Graficar fase de los filtros
plt.subplot(222)
plt.plot(w_iir, np.angle(h_iir), label='IIR ')
plt.xlabel('Frecuencia [Hz]')
plt.ylabel('Fase [radianes]')
plt.grid(True)
plt.legend()
plt.xlim(0, fs/2)
plt.subplot(223)
plt.plot(t,signal_noisy,'-o')
plt.ylabel("Coordenada Y")
plt.title("senoidal")
plt.subplot(224)
plt.plot(t,filtro_salida,'o')
plt.ylabel("Coordenada Y")
plt.title("ruido")
plt.show()









