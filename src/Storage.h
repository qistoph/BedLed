#ifndef _STORAGE_H_
#define _STORAGE_H_

#define STORAGE_DIMM_ADDRESS 0
// sizeof(simLevel) == sizeof(int) == 2

void eepromLoad(unsigned int address, void *value, size_t len);
void eepromSave(unsigned int address, void *value, size_t len);

#endif
