#ifndef FFT_H
#define FFT_H

#include <complex>
#include <cmath>
#include <climits>
#include <algorithm>

#ifndef M_PI
    #define M_PI (acos(-1.0))
#endif

template <class T>
T next_power_of_two(T x) {
    if (!x) {
        return 1;
    }
    if ((x & (x - 1)) == 0) {
        return x;
    }
    x--;
    for (size_t i = 1; i < sizeof(T) * CHAR_BIT; i *= 2) {
        x |= x >> i;
    }
    return x + 1;
};

template<class T>
void bitreversal_permutation(T *vec, size_t size) {
    size_t j = 0;
    for (size_t i = 0; i < size - 1; i++) {
        if (i < j) {
            std::swap(vec[i], vec[j]);
        }
        size_t k = size >> 1;
        while (k <= j) {
            j -= k;
            k >>= 1;
        }
        j += k;
    }
}

template <class T>
void fft(std::complex<T> *arr, size_t size, bool inverse) {
    size_t pwr = next_power_of_two(size);

    if (size <= 1) {
        return;
    }

    bitreversal_permutation(arr, size);

    for (size_t step_size = 2; step_size <= size; step_size <<= 1) {
        std::complex<T> w(1, 0);
        T angle = static_cast<T>(-2 * M_PI / step_size);
        if (inverse) {
            angle = -angle;
        }
        std::complex<T> wn(std::polar<T>(1, angle));

        size_t half_step = step_size / 2;
        for (size_t k = 0; k < half_step; k++) {
            for (size_t step = 0; step < size / step_size; step++) {
                size_t idx_even = step * step_size + k;
                size_t idx_odd = idx_even + half_step;

                std::complex<T> odd = w * arr[idx_odd];
                arr[idx_odd] = arr[idx_even] - odd;
                arr[idx_even] = arr[idx_even] + odd;

                if (inverse) {
                    arr[idx_even] /= 2;
                    arr[idx_odd] /= 2;
                }
            }
            w *= wn;
        }
    }
}

#endif // FFT_H
