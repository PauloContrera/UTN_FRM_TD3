% Generación de una señal con cierta frecuencia de muestreo
%1. Escriba un programa en MATLAB que grafique una función senoidal con las siguientes
%especificaciones:
%   • Frecuencia: 100 Hz.
%   • Frecuencia de muestreo: 1000 Hz.
%   • Tiempo inicio: 0 s.
%   • Tiempo final: 1 s.

close all

f = 100; %Frecuencia.
fm = 1000; %Frecuencia de muestreo.

tm = 1/(fm*6); %Periodo de muestreo

t = 0:tm:1; % Crea un vector que va desde 0 hasta 1 con un incremento de "tm".
y = sin(2*pi*f*t);

grid on;
plot(t,y);

title('Función senoidal.');
xlabel('Tiempo[seg]');
ylabel('Amplitud');