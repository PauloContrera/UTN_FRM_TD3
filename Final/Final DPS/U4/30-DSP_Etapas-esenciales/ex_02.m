%%Trabajo Practico DSP
%% Agregar ruido a una señal
% 2. Escriba un programa en MATLAB que permita agregar a la señal del Ejercicio 1 cierta cantidad de ruido blanco gaussiano.
%       1.Obtenga la varianza del ruido a partir de la relación.
%       2.Utilice la función randn() para simular el ruido (help randn).
%       3.El propotipo de la función debe ser signal_n = my_awgn(signal,snr). Los datos de entrada son el vector signal y el escalar snr.
%       4.La varianza de signal se puede calcular con la función var (help var).
%       5.Compare las salidas de las funciones my_awgn y awgn (help awgn).

close all;
%% Función senoidal

f = 100; %Frecuencia.
fm = 1000; %Frecuencia de muestreo.
tm = 1/(6*fm); %Periodo de muestreo.

t = 0:tm:1;
signal = sin(2*pi*f*t); %Senoidal de 100Hz.

%% Ruido randn - Varianza noise - Varianza signal.

[n,m] = size(signal);
noise = 0.5*randn(n,m); %Ruido.

signal_n = signal + noise;

SNR = snr(signal,signal_n); %SNR.

var_sig = var(signal); %Varianza signal. % medida de dispersión. Eso significa que pretende capturar en qué medida los datos están en torno a la media. 

var_noise = (var_sig)/(10.^(SNR/10)); %Varianza noise.

%% Función my_awgn - signal noise

signal_myawgn = my_awgn(signal,SNR); %signal noise.

%% Función awgn - ruido blanco gaussiano

signal_awgn = awgn(signal,SNR); % signal + ruido blanco gaussiano.


%% Plot.

grid on;

ax1 = subplot(2,1,1);
plot(ax1,t,signal,t,signal_n);
title('Señal sin ruido y con ruido');
xlabel('Tiempo.');
ylabel('Amplitud');
legend('Signal = Original Signal','Signal noise = Signal + noise');

ax2 = subplot(2,1,2);
plot(ax2,t,signal,t,signal_myawgn,t,signal_awgn);
title('Signal + signal noise + signal white noise');
xlabel('Tiempo.');
ylabel('Amplitud');
legend('Signal','Signal my_awgn','Signal awgn (White noise)');

