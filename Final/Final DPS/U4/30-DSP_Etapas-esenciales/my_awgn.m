function [my_signal] = my_awgn(signal,SNR)
% [SignalWithNoise] = my_awgn(signal,SNR) (2 parametros, 1 salida)
% Retorna la señal "signal" con una adicion de ruido blanco.
% SNR es la relacion señal a ruido resultante.
variance=var(signal);
standard_deviation=(variance/(10^(SNR/20)));
noise=standard_deviation*randn(size(signal));
my_signal=signal+noise;
end