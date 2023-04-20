# Memory Management in ISENOS


## Memory map
ISENBL will pass a structure containing information pertaining to the memory map of the PC.
ISENOS will use this to set-up three additionnal structures at specific spaces

### Kernel Memory Information Struct: KMIS
This is a simple structure that contains the address of the Physical Ram Manager and the RAM allocation manager

### Physical Ram Manager: PRM
This is a table that contains usable memory ranges for the kernel. It is static

### RAM Allocation Manager: RAM (recurrent acronym intended, note RAManager)
This is a chained list that will retain information about allocated memory
Namely:
- ProcessID
- Size
- Starting address
- Ending address

The first idt_entries of the RAManager is self referencing, meaning that this allocates RAM for the idt_entries

## Virtual Memory
Kernel will have to update Virtual Memory, it's deduced from PRM and RAManager