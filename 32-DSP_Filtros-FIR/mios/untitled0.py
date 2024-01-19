from scipy.io import loadmat
import sounddevice as sd

# Cargar el archivo MAT
data = loadmat('Tchaikovsky.mat')

# Obtener los datos de audio del archivo (asegúrate de que 'audio' sea la clave correcta)
audio_data = data['audio']

# Reproducir el audio
sd.play(audio_data, data['Fs'][0, 0])  # 'Fs' es la frecuencia de muestreo
sd.wait()  # Esperar a que termine de reproducirse
