%% Trabajo Practico FIR - Filtro Moving Average con se�ales senoidales en MATLAB
clear
clc
close all
%% a) Genere una se�al senoidal con frecuencia fundamental fn = 100 Hz. Elija una frecuencia de muestreo adecuada.

fn = 100; %Frecuencia Fundamental.
fs = 2000; %Frecuencia de muestreo.

t = 0:1/fs:1;
signal = sin(2*pi*fn*t);

%% b) Agregue ruido gaussiano a la se�al senoidal tal que la relaci�n se�al-ruido entre la se�al senoidal y la se�al con ruido sea de 15 dB.

SNR = 15; %Relacion se�al a ruido.

signal_n = my_awgn(signal,SNR); %Se�al con ruido gauseano.
%signal_n = awgn(signal,SNR); %Se�al con ruido gauseano.

%Verificacion del snr.
SNR_test = 10*log(var(signal)/var(signal_n-signal));

%% c) Calcule el valor m�ximo del orden del filtro (N_max) fco = 2 fn.

fco = 2*fn/fs; %Frecuencia de corte normalizada.

Nmax = sqrt((0.885894.^2)/(fco.^2) + 1);

%% d) Aplique filtrado del tipo moving average a la se�al con ruido para un filtro MA con dimensi�n igual N = N_max. Utilice la funci�n filter() (help filter).
% M = round(Nmax);
% M = round(Nmax/2);
M = round(Nmax*10);

%Coeficientes del filtro MA.
% El filtro MA tiene la siguiente estructura:
% a*y[n] = b*(x[n]+x[n-1]+...+x[n-(N-1)])
% Donde a = 1 y b = 1/N (N es el orden del filtro).

a = 1;
b = ones(1,M)/M; % ones crea 1 matriz de unos(1), en este caso de 1 fila y M columnas

signal_ma = filter(b,a,signal_n);

%% e) Grafique la respuesta en frecuencia y fase del filtro MA. Use la funci�n freqz().

figure 

freqz(b,a); %Respuesra en frecuencia y fase del filtro MA.

%% f) Grafique las se�ales en el dominio del tiempo sin ruido, con ruido y filtrada, y compare las tres.

figure
plot(t,signal,'b--o',t,signal_n,'g--*',t,signal_ma,'r');
legend('Senoidal original.','Con ruido','Filtrada');

%% g) Grafique la respuesta en frecuencia de las se�ales original y filtrada y compare. Utilice la funci�n provista my_dft.

[frq,mag] = my_dft(signal,fs); %Respuesta en frecuencia se�al original.
[frq_n,mag_n] = my_dft(signal_n,fs); %Respuesta en frecuencia se�al con ruido.
[frq_ma,mag_ma] = my_dft(signal_ma,fs); %Respuesta en frecuencia se�al filtrada.

figure
plot(frq,mag,'-b',frq_n,mag_n,'.-r',frq_ma,mag_ma,'--g');
legend('Se�al original','Con ruido','Filtrada');

%% h) Repita los puntos d) a g) para N = N_max / 2 y N = N_max * 10.