#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <math.h>

void f_mode(float value) {
    uint32_t binary_rep = *(uint32_t*)&value;

    // Print binary representation
    printf("Binary Representation: ");
    for (int i = 31; i >= 0; i--) {
        printf("%d", (binary_rep >> i) & 1);
    }
    printf("\n");

    // Extract sign (bit 31)
    int sign = (binary_rep >> 31) & 1;

    // Extract exponent (bits 30-23)
    int exponent = (binary_rep >> 23) & 0xFF;

    // Extract mantissa (bits 22-0)
    int mantissa_int = binary_rep & 0x7FFFFF;
    float mantissa;

    // Check for special cases
    if (exponent == 0xFF) {
        if (mantissa_int == 0) {
            // Handle infinity (positive or negative)
            if (sign == 1) {
                printf("Sign: 1\nExponent: 128\nMantissa: 1\nValue: -inf\n");
            } else {
                printf("Sign: 0\nExponent: 128\nMantissa: 1\nValue: inf\n");
            }
        } else {
            // Handle NaN (Not a Number)
            printf("Sign: %d\nExponent: 128\nMantissa: 1.5\nValue: NaN\n", sign);
        }
        return;
    }

    // Handle zero case (both positive and negative zero)
    if (exponent == 0 && mantissa_int == 0) {
        if (sign == 1) {
            printf("Sign: 1\nExponent: 0\nMantissa: 0\nValue: -0.0\n");
        } else {
            printf("Sign: 0\nExponent: 0\nMantissa: 0\nValue: 0.0\n");
        }
        return;
    }

    // Handle denormalized numbers (exponent is 0 but mantissa is non-zero)
    if (exponent == 0) {
        mantissa = mantissa_int / (float)(1 << 23); // No implicit leading 1 for denormals
        printf("Sign: %d\nExponent: 0\nMantissa: %.7g\nValue: %.7g\n", sign, mantissa, value);
        return;
    }

    // Handle normalized numbers
    mantissa = 1.0 + (mantissa_int / (float)(1 << 23)); 
    int true_exponent = exponent - 127; 
    printf("Sign: %d\nExponent: %d\nMantissa: %.7g\nValue: %.7g\n", sign, true_exponent, mantissa, value);
}

void b_mode(char* binary_string) {
    uint32_t binary_rep = 0;

    int bit_count = 0;
    while (binary_string[bit_count] != '\0') {
        bit_count++;
    }

    if (bit_count != 32) {
        printf("Error: Binary string must be exactly 32 bits long.\n");
        return;
    }
    for (int i = 0; i < 32; i++) {
        binary_rep = (binary_rep << 1) | (binary_string[i] - '0');
    }

    float value = *(float*) &binary_rep;

    f_mode(value); 
}

// Main function
int main(int argc, char* argv[]) {
    // Check for correct number of arguments
    if (argc != 3) {
        printf("Usage: %s -f <float> or %s -b <binary_string>\n", argv[0], argv[0]);
        return 1;
    }

    if (argv[1][0] == '-' && argv[1][1] == 'f' && argv[1][2] == '\0') {
        float value = strtof(argv[2], NULL);
        f_mode(value);
    } else if (argv[1][0] == '-' && argv[1][1] == 'b' && argv[1][2] == '\0') {
        b_mode(argv[2]);
    } else {
        printf("Error: Invalid mode.");
        return 1;
    }

    return 0;
}





