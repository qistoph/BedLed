#ifndef _STORAGE_H_
#define _STORAGE_H_

#define STORAGE_DIMM_ADDRESS 0
// sizeof(simLevel) == sizeof(int) == 2

#define STORAGE_KAKU_BASE_ADDR 2
#define STORAGE_KAKU_MAX_ADDRESS 5
// 5*sizeof(kakuEeprom_t) = 5*(4+1) = 25

#define STORAGE_END 27

typedef struct {
  unsigned long address;
  byte channel;
} kakuEeprom_t;

void eepromLoad(unsigned int address, void *value, size_t len);
void eepromSave(unsigned int address, void *value, size_t len);

#endif
