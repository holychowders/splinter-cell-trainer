#include <stdio.h>
#include <stdint.h>

typedef uint8_t u8;
typedef int32_t bool;
#define false 0
#define true 1

// NOTE: THE OFFSET IS DIFFERENT FOR DIFFERENT LEVELS
// Flag: bNoThermalAvailable (available in the first two levels of the game)
const size_t bNoThermalAvailable_offset = 0x0000182B;
const int bNoThermalAvailable_size = 19;

int main(void) {
    puts("Splinter Cell Basic Hacks Trainer");
    char s_training_level_part_1[] = "C:/Program Files (x86)/Steam/steamapps/common/Splinter Cell/maps/0_0_2_Training.scl";

    { // Training level part 1
        FILE* f_training_level_part_1 = {0};
        // FIXME: CLOSE FILE
        if (0 == fopen_s(&f_training_level_part_1, s_training_level_part_1, "rb")) {
            puts("INFO: Opened training level part 1");
            if (0 == fseek(f_training_level_part_1, 0, SEEK_END)) {
                puts("INFO: SEEK_END successful");
                long size = ftell(f_training_level_part_1);
                rewind(f_training_level_part_1);
                if (size > 0) {
                    printf("INFO: Size of level file: %ld\n", size);
                    char bytes[size];
                    size_t objects_read = fread(bytes, size, 1, f_training_level_part_1);
                    if (objects_read == 1) {
                        puts("INFO: We have successfully read in part 1 of the training level");

                        /************************
                        ** MEMORY PATCHES HERE **
                        *************************/




#if 0  // Let's not try to do this dynamically. The offset should never ever change. Just hardcode the memory patch.
                        const char thermal_flag[] = "bNoThermalAvailable";
                        const int thermal_flag_size = 19;
                        bool matching_thermal_flag = false;
                        for (u8 ibyte = 0; ibyte < size; ++ibyte) {
                            printf("Byte Index: %d out of %d\n", ibyte, size);
                            if (size - thermal_flag_size > 0) { // If there's still room for the flag exist
                                for (u8 i_possible_byte = 0; i_possible_byte < thermal_flag_size; ++i_possible_byte) { // Search the next X bytes for the flag
                                    if (bytes[i_possible_byte] == thermal_flag[i_possible_byte]) {
                                        matching_thermal_flag = true;
                                        puts("matched");
                                    } else {
                                        puts("failed match");
                                        matching_thermal_flag = false;
                                        break;
                                    }
                                }
                            }
                        }
#endif
                    }
                }
            }
        }
    }
}
