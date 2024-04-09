%% Trabajo Practico FIR - Filtro Moving Average con oversampling
clear
clc
close all
%% a) Genere una se�al senoidal con frecuencia fundamental fn = 100 Hz. Elija una frecuencia de muestreo adecuada.

fn = 100; %Frecuencia Fundamental.
fs = 2000; %Frecuencia de muestreo (2000,5000 y 10000).

t = 0:1/fs:1;
signal = sin(2*pi*fn*t);

%% b) Agregue ruido gaussiano a la se�al senoidal tal que la relaci�n se�al-ruido entre la se�al senoidal y la se�al con ruido sea de 15 dB.

SNR = 15; %Relacion se�al a ruido.

signal_n = my_awgn(signal,SNR); %Se�al con ruido gauseano.

%Verificacion del snr.
SNR_test = 10*log(var(signal)/var(signal_n-signal));

%% c) Calcule el valor m�ximo del orden del filtro (N_max) fco = 2 fn.

fco = 2*fn/fs; %Frecuencia de corte normalizada.

Nmax = sqrt((0.885894.^2)/(fco.^2) + 1);

%% d) Aplique filtrado del tipo moving average a la se�al con ruido para un filtro MA con dimensi�n igual N = N_max. Utilice la funci�n filter() (help filter).

M = round(Nmax);

%Coeficientes del filtro MA.
a = 1;
b = ones(1,M)/M;

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

%% Ejercicio 3 
%a) Tome el ejercicio 1 ya resuelto.
% b) Eleve la frecuencia de muestreo, fs, y el orden N, pero sin alterar la
% frecuencia de corte, fco, del filtro MA. Tome los valores de fs iguales a 2.000, 5.000 y 10.000 Hz.
% c) Cuantifique la diferencia entre la se�al de entrada y la se�al de
% salida utilizando la funci�n rmse() (root mean squarred error).
% d) Determine qu� efecto tiene en el error entre ambas se�ales aumentar la
% frecuencia de muestreo.

rmserr_n = rmse (signal, signal_n);
disp('rmserr_n = '),disp(rmserr_n);

rmserr_ma = rmse (signal, signal_ma);
disp('rmserr_ma = '),disp(rmserr_ma);

rmserr_ma_n = rmse (signal_n, signal_ma);
disp('rmserr_ma_n = '),disp(rmserr_ma_n);

% A medida que incremento la frecuencia de muestreo, 
% el rmserr se incrementa ya que el orden del filtro se incrementa 
% (directamente proporcional al cuadrado de fs)