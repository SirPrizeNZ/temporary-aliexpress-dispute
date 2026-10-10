# Verification tools

`verify_measurements.py` checks the published saved extract; it does not contact the watch. Run it with Python 3 from the repository root.

`original_storage_probe_excerpt.c` preserves the original storage JNI function and its required declarations from the saved Watch Audit source. The unrelated memory-probe function has been omitted. The retained source matches the local original exactly. The file contains the JNI source excerpt.

The JNI function calls libc `statfs` and direct AArch64 syscall 43 for `/data`, returning each status code, total bytes, available bytes and free bytes. It performs no writes, network requests or unique-identifier collection. JNI table indices 180 and 212 correspond to `NewLongArray` and `SetLongArrayRegion`. The Java package label `local.watchaudit` is the generic audit tool identifier.

