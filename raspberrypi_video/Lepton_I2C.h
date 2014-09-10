#ifndef LEPTON_I2C
#define LEPTON_I2C

void lepton_read_bytes(char *buffer, int length);
void lepton_write_bytes(char *buffer, int length);

void lepton_reboot();
void lepton_perform_ffc();

#endif
