project(ISENOS LANGUAGES C)

add_executable(ISENOS src/print.c
        src/print.h
        src/isenos.h
        src/main.c
        src/macros/macros.c
        src/macros/macros.h
        src/memory/physical_ram/physical_ram_range.h
        src/memory/physical_ram/physical_ram_manager.h
        src/memory/physical_ram/physical_ram_manager.c
        src/memory/efi_memory_types.h
        src/memory/kernel_memory_information.h
        src/memory/ram_allocation/ram_allocation_manager.h
        src/memory/ram_allocation/ram_allocation_manager_allocation.h
        src/memory/kernel_memory_information.c
        src/common/string.h
        src/common/os-config.h
        src/common/string.c
        src/memory/ram_allocation/ram_allocation_manager.c)