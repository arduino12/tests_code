gamma = 2.8  # Correction factor
max_in = 35  # Top end of INPUT range
max_out = 255  # Top end of OUTPUT range


with open("gamma.h", "wb") as f:
    f.write("#ifndef GAMMA_H\n#define GAMMA_H\n\n")
    f.write("static const uint8_t gamma[] PROGMEM = {")

    for i in xrange(max_in):
        f.write(" " if i % 16 else "\n\t")
        f.write("0x%02X," % (((float(i) / (max_in - 1)) ** gamma) * max_out + 0.5,))

    f.write("\n};\n\n#endif /* GAMMA_H */\n");
