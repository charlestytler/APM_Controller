# Fourier Transform Plot
# Created by Charlie Tytler  2018

import numpy as np
import matplotlib.pyplot as plt

def fft_plot(time, signal, N_bins=0):
    #
    # fft_plot(time, signal, [N_bins])
    #
    # Plots FFT of a provided time history of a signal
    # [Optional] Sepcify number of bins with N_bins.
    #
    
    assert len(time) == len(signal), "Lenths of time and signal inputs do not match."
    
    #Make signals have even number of samples
    if len(time) % 2 != 0:
        time = time[0:-1]
        signal = signal[0:-1]
    
    if (N_bins == 0):
        N_bins = len(signal)
    
    time_step = np.mean(np.diff(time))
    
    Y = np.fft.fft(signal, N_bins)
    P2 = abs(Y / N_bins)
    P1 = P2[0 : int(N_bins/2) + 1]
    P1[1:-1] = 2 * P1[1:-1]
    
    freq = 1 / time_step * np.arange(0, int(N_bins/2) + 1) / N_bins
    
    fig = plt.figure()
    ax = fig.add_subplot(111)
    ax.plot(freq, P1)
    ax.set_xlabel('Frequency (Hz)')
    ax.set_ylabel('Amplitude')
    ax.set_title('Single Sided Amplitude FFT')
    ax.grid(True)
    