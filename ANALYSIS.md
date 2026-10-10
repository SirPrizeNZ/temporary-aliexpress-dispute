# Technical interpretation

## Two independent measurement paths

The saved audit reports successful return codes (`0`) for both native libc `statfs("/data", ...)` and Linux AArch64 syscall 43. Both returned:

- Total: 10,043,224,064 bytes.
- Available: 656,318,464 bytes.
- Free: 847,241,216 bytes.

Android Java File / StatFs readings were larger by exactly 51,539,607,552 bytes in each comparable count. A Java class called `StatFs` does not establish that it bypasses the modified `statvfs` path. The sector count provides a separate whole-device capacity check; it is not a measurement of the `/data` partition alone.

## Relevant firmware code

Addresses below are virtual addresses in the collected AArch64 ELF, identified by the SHA-256 recorded in `evidence/measurements.json`.

| Address | Observed instruction behavior |
| --- | --- |
| `0x287a4` | Start of `statfs` / `statfs64`; calls `__statfs` at `0x6a1fc` without the later block-count additions |
| `0x287d4` | Start of `statvfs` / `statvfs64`; obtains underlying `__statfs` results and copies fields |
| `0x2886c–0x28884` | Calls `getuid`; continues to modification only for UID **above 10000** or UID **2000**; system UID 1000 is excluded |
| `0x28888–0x2889c` | Reads property `persist.sys.rom`; skips modification if the property is absent/empty |
| `0x288a0–0x288cc` | Checks prefixes `/storage/emulated` (17 bytes) and `/data` (5 bytes) |
| `0x288d0–0x28908` | Loads a cached offset or derives one from the numeric property and underlying block count |
| `0x2890c–0x28928` | Shifts the offset by 18, then adds it to total, free and available block counts |

The literal strings at `0xa32aa`, `0xa32ba` and `0xa2b53` were separately read from the saved ELF and are included with their file offsets.

The code intentionally changes the returned counts for selected callers and paths. The **observed** byte addition is exactly 48 GiB. The saved results and firmware instructions establish this addition.

## Measurement provenance

The read-only results were collected on 9 October 2026 inside Watch Audit 0.19 using JNI, with libc `statfs` and direct syscall 43. The retained source excerpt includes the original storage function and its declarations.

The original firmware ELF is identified by its SHA-256. The whole-device sector count corresponds to 15.76 decimal GB, consistent with a nominal 16 GB device.
