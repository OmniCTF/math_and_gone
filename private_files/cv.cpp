#include <iostream>
#include <cstring>
#include <cstdint>

char key_buffer[] = "MyL1ttleA3th3rs!";

// valorile tinta: OMNICTF{f31st3l} criptat cu chaining, un uint32 per caracter
uint32_t target[16] = {
    0x00014b84, 0x047d00de, 0x042f547b, 0x6a61044c,
    0x6ae92095, 0x4e416a74, 0x4eda5bc4, 0x437b4ed8,
    0x435df331, 0x2d564346, 0x2d0e9639, 0x63ef2ded,
    0x6360d70e, 0x090063f9, 0x095ab476, 0x95cb097f
};

void generate_sbox(const char* key, int sbox[256]) {
    int klen = strlen(key);
    for (int i = 0; i < 256; ++i)
        sbox[i] = ((key[i % klen] * 123) + 24) & 0xFF;
}

uint16_t F(uint16_t parte, int sbox[256]) {
    return sbox[parte & 0xFF];
}

uint32_t feistel(uint32_t value, int nr_rounds, int sbox[256]) {
    uint16_t L = (value >> 16) & 0xFFFF;
    uint16_t R = value & 0xFFFF;
    for (int i = 0; i < nr_rounds; ++i) {
        uint16_t L_new = R;
        uint16_t R_new = L ^ F(R, sbox);
        L = L_new;
        R = R_new;
    }
    return ((uint32_t)L << 16) | R;
}

int main() {
    std::cout << "hello and welcome to the game of choice here only the strong can reverse this!\n";

    char plaintext[33] = "";
    std::cin >> plaintext;

    int good = 1;
    long n = strlen(plaintext);

    // lungimea trebuie sa fie 16
    if ((unsigned)(n * 2654435761u) != 0xe3779b10u) good = 0;

    int sbox[256];
    generate_sbox(key_buffer, sbox);

    if (good) {
        uint32_t prev = 0x1337;   // IV
        for (int i = 0; i < n; i++) {
            uint32_t v = (uint32_t)plaintext[i] * 1117u;
            v ^= prev;                        // chaining
            uint32_t enc = feistel(v, i, sbox);
            if (enc != target[i]) { good = 0; break; }
            prev = enc;
        }
    }

    if (good)
        std::cout << "You might be onto something but I will leave it like that\n";
    else
        std::exit(0);

    return 0;
}