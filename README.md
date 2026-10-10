# AliExpress smartwatch dispute: false storage reporting

The purchased variant was advertised as **4 GB RAM / 64 GB storage**. This bundle documents the storage discrepancy on the examined H800/H20 smartwatch firmware sample.

**The evidence is stronger than a capacity-label mismatch: the firmware contains code that changes the storage counts returned to apps and the Android shell. The saved measurements show an exact 48 GiB addition.**

Measurements were collected read-only with Watch Audit 0.19 on **9 October 2026**. No destructive fill test was performed.

| Measurement | Bytes | Meaning |
| --- | ---: | --- |
| Entire MMC block device | 15,762,194,432 | 30,785,536 sectors × 512 bytes; 15.76 decimal GB |
| Real `/data` total | 10,043,224,064 | Native libc `statfs` and direct Linux syscall agree |
| Android-reported `/data` total | 61,582,831,616 | Inflated result |
| Real `/data` available | 656,318,464 | About 626 MiB |
| Android-reported `/data` available | 52,195,926,016 | About 52.20 decimal GB |
| Added to total, free and available | **51,539,607,552** | **Exactly 48 GiB** |

The firmware properties also report `persist.sys.rom=64G` and `persist.sys.truerom=16GB`.

## Evidence and verification

- [Measurements](evidence/measurements.json): privacy-minimized extract, with retained numeric values unchanged.
- [Firmware disassembly](evidence/libc-storage-disassembly.txt): relevant `statfs` / `statvfs` functions from `/system/lib64/libc.so`.
- [Firmware literals](evidence/libc-literals.json): strings read from the corresponding ELF addresses.
- [Technical explanation](ANALYSIS.md): instruction addresses, interpretation and limits.
- [Original read-only probe excerpt](tools/original_storage_probe_excerpt.c): the storage JNI function used by Watch Audit, with its required declarations. This is source code, not a standalone application.
- [Privacy and provenance](PRIVACY.md): what has been omitted and what this public bundle can establish.

Run the arithmetic and consistency checks with Python 3:

```sh
python3 tools/verify_measurements.py
```

Verify file integrity on macOS with `shasum -a 256 -c SHA256SUMS`, or on Linux with `sha256sum -c SHA256SUMS`.

**Limits:** The entire MMC capacity and the `/data` partition capacity are different measurements. Free space changes over time. At the recorded sample, available space exceeded the 500 MiB low-space threshold; the earlier warning trigger was not captured. These findings establish false storage reporting in this firmware sample. They do not establish who authored the modification, that the device was completely full, or that every watch sold under this name has identical firmware. This bundle does not independently establish physical RAM capacity.

## 中文说明（供速卖通工作人员审核）

购买的规格为 **4GB 内存 / 64GB 存储**。本证据包针对存储容量虚报问题。

实际读取到的整块闪存容量为 **15,762,194,432 字节（约 15.76GB）**。原生 libc `statfs` 与直接 Linux 系统调用分别读取 `/data`，两者结果一致：实际可用空间为 **656,318,464 字节（约 626MiB）**，而 Android 向应用报告 **52,195,926,016 字节（约 52.20GB）**。

反汇编显示，固件的 `statvfs` 函数会根据调用者 UID、路径和 `persist.sys.rom` 属性修改返回值。记录中的总容量、空闲容量和可用容量均被增加 **51,539,607,552 字节，即整整 48GiB**。固件同时保存 `persist.sys.rom=64G` 和 `persist.sys.truerom=16GB`。

这说明存在固件层面的容量数据修改，并非单纯的 GB/GiB 换算差异。采样时实际可用空间高于 500MiB 的低空间警告阈值，因此本证据不声称已确定此前警告的具体触发原因，也不声称当时存储已完全耗尽。公开文件已去除个人信息、账号、网络地址和设备唯一标识。
