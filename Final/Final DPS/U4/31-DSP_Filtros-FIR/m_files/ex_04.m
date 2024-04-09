%% Trabajo practico SP - Filtrado por ventanas en MATLAB
%% a) Use la herramienta filterDesigner para diseñar un filtro 
% pasa-banda con frecuencias de corte de 300 Hz y 3.400 Hz 
% (canal telefónico), con ventana Kaiser con beta = 7.5, orden 10,
% con formato punto flotante, precisión doble

load Tchaikovsky.mat;
figure ; subplot (2,1,1), plot(signal(:,1),'r'),grid on, title 'original'; %ploteo señal sin ruido

%% c) Exporte el diseño del filtro como una función de MATLAB: File > Generate
%MATLAB Code > Filter Design Function.

% Hd = ejercicio_04_c_plantilla;
Hd = filtro_nacho_ej4;

%% e) Aplique a la señal de interés el filtro diseñado en el punto b).

% Coeficiente del filtro por ventana.
B = Hd.Numerator;
A = 1;

signal_f = filter(B,A,signal);

subplot (2,1,2), plot(signal(:,1),'b'),grid on, title 'filtrada';

%% f) Grafique los espectros de la señal original y filtrada con la función my_dft().

[frec,mag] = my_dft(signal,Fs);

figure ; subplot (2,1,1), plot(frec,mag,'r'),grid on, title 'original';

[frecf,magf] = my_dft(signal_f,Fs);

subplot (2,1,2), plot(frecf,magf,'b'),grid on, title 'filtrada';