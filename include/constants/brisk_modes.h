#ifndef GUARD_CONSTANTS_BRISK_MODES_H
#define GUARD_CONSTANTS_BRISK_MODES_H
#define BRISK_MODE_NORMAL 0
#define BRISK_MODE_EASY 1
#define BRISK_MODE_HARD 2
#define BRISK_MODE_KAIZO 3
// Keep legacy mode values fixed for existing saves. Expert is new slot 4.
#define BRISK_MODE_EXPERT 4
#define BRISK_MODE_MAX BRISK_MODE_EXPERT
// Compatibility alias: existing saves and internal callers retain value 3.
#define BRISK_MODE_EXTREME BRISK_MODE_KAIZO
#endif
