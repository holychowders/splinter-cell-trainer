#include <stdio.h>
#include <stdint.h>

typedef uint8_t u8;
typedef int32_t bool;
#define false 0
#define true 1

// NOTE: THE OFFSET IS DIFFERENT FOR DIFFERENT LEVELS
// Flag: bNoThermalAvailable (available in the first two levels of the game)
static const int nothermal_flag_size = 19;
static const size_t nothermal_offset = 0x0000182B;
static const char nothermal_original_bytes[nothermal_flag_size] = "bNoThermalAvailable";
static const char nothermal_patchbytes[nothermal_flag_size] = {0x00};

int main(void) {
    puts("Splinter Cell Basic Hacks Trainer");
    char s_training_level_part_1[] = "C:/Program Files (x86)/Steam/steamapps/common/Splinter Cell/maps/0_0_2_Training.scl";

    { // Training level part 1
        FILE* f_training_level_part_1 = {0};
        // FIXME: CLOSE FILE
        if (0 == fopen_s(&f_training_level_part_1, s_training_level_part_1, "r+b")) {
            puts("INFO: Opened training level part 1");
            if (0 == fseek(f_training_level_part_1, nothermal_offset, SEEK_SET)) {
                puts("INFO: bNoThermalAvailable flag offset seek");

                /** ** MEMORY PATCHES HERE ** **/
                const bool patch = true;
                const bool restore = false;
                size_t objects_written = {0};
                if (patch) {
                    objects_written = fwrite(&nothermal_patchbytes, sizeof(nothermal_patchbytes[0]), nothermal_flag_size, f_training_level_part_1);
                } else if (restore) {
                    objects_written = fwrite(&nothermal_original_bytes, sizeof(nothermal_original_bytes[0]), nothermal_flag_size, f_training_level_part_1);
                }

                if (objects_written == nothermal_flag_size) {
                    puts("INFO: Patched bNoThermalAvailable");
                } else {
                    puts("fail");
                }

                rewind(f_training_level_part_1);
            }
        }
    }
}
